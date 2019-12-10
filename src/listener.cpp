#include <functional>

#include "listener.hpp"
#include "connection.hpp"
#include "hive.hpp"

using boost::asio;

template <class T>
Listener<T>::Listener(HiveRef hive):
	hive(hive),
	acceptor(hive->getIOContext()),
	strand(hive->getIOContext()),
	errorState(0)
{
}

template <class T>
HiveRef Listener<T>::getHive()
{
	return hive;
}

template <class T>
Acceptor& Listener<T>::getListener()
{
	return acceptor;
}

template <class T>
Strand& Listener<T>::getStrand()
{
	return strand;
}

template <class T>
bool Listener<T>::hasError()
{
	uint32 v1 = 1;
	uint32 v2 = 1;
	return errorState.compare_exchange_strong(v1, v2);
}

template <class T>
void Listener<T>::listen(const std::string_view &host, uint16 port)
{
	try
	{
		Resolver resolver(hive->getService());
		Endpoint endpoint = resolver.resolve(host, std::to_string(port))->endpoint();

		acceptor.open(endpoint.protocol());
		acceptor.set_option(Acceptor::reuse_address(false));
		acceptor.bind(endpoint);
		acceptor.listen(asio::socket_base::max_connections);
	}
	// should never happen
	catch (...)
	{
	}
}

template <class T>
void Listener<T>::handleAccept(Error error, ConnectionPtr<T> connection)
{
	if (error || hasError() || hive->stopped())
		connection->startError(error);
	else
	{
		if (connection->getSocket().is_open())
		{
			if (onAccept(connection,
				connection->getAddress().to_string(),
				connection->getPort()))
			{
				connection->onAccept(acceptor.local_endpoint().address().to_string(),
					acceptor.local_endpoint().port());
				connection->receive();
			}
		}
		else
			connection->startError(error);
	}
}

template <class T>
void Listener<T>::dispatchAccept(ConnectionPtr<T> connection)
{
	acceptor.async_accept(connection->getSocket(), asio::bind_executor(connection->getStrand(),
		std::bind(&Listener<T>::handleAccept, shared_from_this(), std::placeholders::_1, connection)));
}

template <class T>
void Listener<T>::accept(ConnectionPtr<T> connection)
{
	strand.post(std::bind(&Listener<T>::dispatchAccept, shared_from_this(), connection));
}

template <class T>
bool Listener<T>::onAccept(ConnectionRef<T>, const std::string_view&, uint16)
{
	return true;
}

template <class T>
void Listener<T>::onError(Error)
{
}
