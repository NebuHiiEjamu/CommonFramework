#include <functional>

#include "connection.hpp"
#include "hive.hpp"

using boost::asio;

template <class T>
Connection<T>::Connection(HivePtr hive):
	hive(hive),
	socket(hive->getIOContext()),
	strand(hive->getIOContext()),
	errorState(0)
{
}

template <class T>
HivePtr Connection<T>::getHive()
{
	return hive;
}

template <class T>
Strand& Connection<T>::getStrand()
{
	return strand;
}

template <class T>
Connection<T>::Socket& Connection<T>::getSocket()
{
	return socket;
}

template <class T>
Address& Connection<T>::getAddress() const
{
	return socket.remote_endpoint().address();
}

template <class T>
std::string Connection<T>::getHostName() const
{
	Resolver resolver(hive->getService());
	return resolver.resolve(getAddress(), std::to_string(getPort()))->host_name();
}

template <class T>
uint16 Connection<T>::getPort() const
{
	return socket.remote_endpoint().port();
}

template <class T>
bool Connection<T>::hasError()
{
	uint32 v1 = 1;
	uint32 v2 = 1;
	return errorState.compare_exchange_strong(v1, v2);
}

template <class T>
void Connection<T>::disconnect()
{
	onDisconnect();
	socket.close();
}

template <class T>
void Connection<T>::startError(Error error)
{
	uint32 v1 = 1;
	uint32 v2 = 0;

	if (!errorState.compare_exchange_strong(v1, v2))
	{
		Error errorCode;

		socket.shutdown(Socket::shutdown_both, errorCode);
		socket.close(errorCode);
		onError(error);
	}
}

template <class T>
void Connection<T>::dispatchReceive(Size totalBytes)
{
	pendingReceives.push(totalBytes);
	if (pendingReceives.empty()) startReceive(totalBytes);
}

template <class T>
void Connection<T>::dispatchSend(const Buffer &buffer)
{
	bool shouldStartSend = pendingSends.empty();
	pendingSends.push(buffer);
	if (shouldStartSend) startSend();
}

template <class T>
void Connection<T>::startReceive(Size totalBytes)
{
	if (totalBytes > 0)
	{
		asio::async_read(socket, inBuffer.prepare(totalBytes), asio::bind_executor(strand,
			std::bind(&Connection<T>::handleReceive, shared_from_this(), std::placeholders::_1,
				std::placeholders::_2)));
	}
	else
	{
		MutableBuffer buffer = inBuffer.prepare(inBufferSize);
		asio::async_read_some(socket, inBuffer.prepare(inBufferSize), asio::bind_executor(strand,
			std::bind(&Connection<T>::handleReceive, shared_from_this(), std::placeholders::_1,
				std::placeholders::_2)));
	}
}

template <class T>
void Connection<T>::startSend()
{
	if (!pendingSends.empty())
	{
		Buffer &front = pendingSends.front()
		asio::async_write(socket, front.data()), asio::bind_executor(strand,
			std::bind(&Connection<T>::handleSend, shared_from_this(), std::placeholders::_1,
			pendingSends.front())));
	}
}

template <class T>
void Connection<T>::handleReceive(Error error, Size receivedBytes)
{
	if (error || hasError() || hive->stopped()) startError(error);
	else
	{
		inBuffer.resize(receivedBytes);
		onReceive(inBuffer);
		receive();
		pendingReceives.pop();
		if (!pendingReceives.empty()) startReceive(pendingReceives.front());
	}
}

template <class T>
void Connection<T>::handleSend(Error error, const Buffer &buffer)
{
	if (error || hasError() || hive->stopped()) startError(error);
	else
	{
		onSend(buffer);
		pendingSends.pop();
		startSend();
	}
}

template <class T>
void Connection<T>::receive(Size totalBytes = 0)
{
	strand.post(std::bind(&Connection<T>::dispatchReceive, shared_from_this(), totalBytes));
}

template <class T>
void Connection<T>::send(const Buffer &buffer)
{
	strand.post(std::bind(&Connection<T>::dispatchSend, shared_from_this(), buffer));
}

template <class T>
Size Connection<T>::getInBufferSize() const
{
	return inBufferSize;
}

template <class T>
void Connection<T>::setInBufferSize(Size newSize)
{
	inBufferSize = newSize;
}

template <class T>
void Connection<T>::onAccept(std::string_view, uint16)
{
}

template <class T>
void Connection<T>::onConnect(std::string_view, uint16)
{
}

template <class T>
void Connection<T>::onSend(const Buffer&)
{
}

template <class T>
void Connection<T>::onReceive(Buffer&)
{
}

template <class T>
void Connection<T>::onError(Error)
{
}

template <class T>
void Connection<T>::onDisconnect()
{
}
