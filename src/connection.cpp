#include <functional>

#include "connection.hpp"
#include "hive.hpp"

using boost::asio;

template <class T>
Connection<T>::Connection(HiveRef hive):
	hive(hive),
	socket(hive->getIOContext()),
	strand(hive->getIOContext()),
	resolver(hive->getIOContext()),
	errorState(0)
{
}

template <class T>
HiveRef Connection<T>::getHive()
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
	return resolver.resolve(getAddress(), std::to_string(getPort()))->host_name();
}

template <class T>
uint16 Connection<T>::getPort() const
{
	return socket.remote_endpoint().port();
}

template <class T>
uint32 Connection<T>::getErrorState() const
{
	return errorState;
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
void Connection<T>::dispatchSend(const ByteString &data)
{
	bool shouldStartSend = pendingSends.empty();
	pendingSends.push(data);
	if (shouldStartSend) startSend();
}

template <class T>
void Connection<T>::startReceive(Size totalBytes)
{
	if (totalBytes > 0)
	{
		inBuffer.resize(totalBytes);
		asio::async_read(socket, asio::dynamic_buffer(inBuffer), asio::bind_executor(strand,
			std::bind(&Connection<T>::handleReceive, shared_from_this(), std::placeholders::_1,
				std::placeholders::_2)));
	}
	else
	{
		inBuffer.resize(inBufferSize);
		socket.async_read_some(asio::dynamic_buffer(inBuffer), asio::bind_executor(strand,
			std::bind(&Connection<T>::handleReceive, shared_from_this(), std::placeholders::_1,
				std::placeholders::_2)));
	}
}

template <class T>
void Connection<T>::startSend()
{
	if (!pendingSends.empty())
	{
		ByteString &front = pendingSends.front();
		asio::async_write(socket, asio::dynamic_buffer(front), asio::bind_executor(strand,
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
void Connection<T>::handleConnect(Error error)
{
	if (error || hasError() || hive->stopped()) startError(error);
	else
	{
		if (socket.is_open())
		{
			onConnect(getAddress().to_string(), getPort());
			receive();
		}
		else startError(error);
	}
}

template <class T>
void Connection<T>::receive(Size totalBytes = 0)
{
	strand.post(std::bind(&Connection<T>::dispatchReceive, shared_from_this(), totalBytes));
}

template <class T>
void Connection<T>::send(const ByteString &data)
{
	strand.post(std::bind(&Connection<T>::dispatchSend, shared_from_this(), data));
}

template <class T>
void Connection<T>::connect(const std::string_view &host, uint16 port)
{
	Resolver::iterator it = resolver.resolve(host, std::to_string(port));
	
	socket.async_connect(*it, asio::bind_executor(strand, std::bind(&Connection<T>::handleConnect,
		shared_from_this(), std::placeholders::_1)));
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
void Connection<T>::onAccept(const std::string_view&, uint16)
{
}

template <class T>
void Connection<T>::onConnect(const std::string_view&, uint16)
{
}

template <class T>
void Connection<T>::onSend(const ByteString&)
{
}

template <class T>
void Connection<T>::onReceive(ByteString&)
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
