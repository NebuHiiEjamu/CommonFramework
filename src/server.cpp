#include "server.hpp"

LoggerRef Server::getLogger()
{
	return logger;
}

LogLevel Server::getLogLevel() const
{
	return logger->getLevel();
}
