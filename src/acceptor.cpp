#include <functional>

#include "acceptor.hpp"
#include "connection.hpp"
#include "hive.hpp"

using boost::asio;

Acceptor::Acceptor(HiveRef hive):
	hive(hive),
	acceptor(hive->getIOContext()),
	strand(hive->getIOContext()),
	resolver(hive->getIOContext()),
	errorState(0)
{
}

HiveRef Acceptor::getHive()
{
	return hive;
}

Acceptor& Acceptor::getAcceptor()
{
	return acceptor;
}

Strand& Acceptor::getStrand()
{
	return strand;
}

bool Acceptor::hasError()
{
	uint32 v1 = 1;
	uint32 v2 = 1;
	return errorState.compare_exchange_strong(v1, v2);
}

void Acceptor::listen(const std::string_view &host, uint16 port)
{
	try
	{
		Resolver::iterator it = resolver.resolve(host, std::to_string(port));

		acceptor.open(it->protocol());
		acceptor.set_option(Acceptor::reuse_address(false));
		acceptor.bind(*it);
		acceptor.listen(asio::socket_base::max_connections);
	}
	// should never happen
	catch (...)
	{
	}
}

void Acceptor::handleAccept(Error error, ConnectionPtr<asio::ip::tcp> connection)
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

void Acceptor::dispatchAccept(ConnectionPtr<asio::ip::tcp> connection)
{
	acceptor.async_accept(connection->getSocket(), asio::bind_executor(connection->getStrand(),
		std::bind(&Acceptor::handleAccept, shared_from_this(), std::placeholders::_1, connection)));
}

void Acceptor::accept(ConnectionPtr<asio::ip::tcp> connection)
{
	strand.post(std::bind(&Acceptor::dispatchAccept, shared_from_this(), connection));
}

bool Acceptor::onAccept(ConnectionRef<asio::ip::tcp>, const std::string_view&, uint16)
{
	return true;
}

void Acceptor::onError(Error)
{
}
