#include <functional>

#include "listener.hpp"
#include "connection.hpp"
#include "hive.hpp"

using boost::asio;

Listener::Listener(HivePtr hive):
	hive(hive),
	acceptor(hive->getIOContext()),
	strand(hive->getIOContext()),
	errorState(0)
{
}

HivePtr Listener::getHive()
{
	return hive;
}

Acceptor& Listener::getListener()
{
	return acceptor;
}

Strand& Listener::getStrand()
{
	return strand;
}

bool Listener::hasError()
{
	uint32 v1 = 1;
	uint32 v2 = 1;
	return errorState.compare_exchange_strong(v1, v2);
}

void Listener::listen(std::string_view host, uint16 port)
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

void Listener::handleAccept(Error error, ConnectionPtr connection)
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

void Listener::dispatchAccept(ConnectionPtr connection)
{
	acceptor.async_accept(connection->getSocket(), asio::bind_executor(connection->getStrand(),
		std::bind(&Listener::handleAccept, shared_from_this(), std::placeholders::_1, connection)));
}

void Listener::accept(ConnectionPtr connection)
{
	strand.post(std::bind(&Listener::dispatchAccept, shared_from_this(), connection));
}

bool Listener::onAccept(ConnectionPtr, std::string_view, uint16)
{
	return true;
}

void Listener::onError(Error)
{
}
