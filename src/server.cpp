#include "hive.hpp"
#include "logger.hpp"
#include "server.hpp"

Server::Server():
	hive(new Hive()),
	logger(new Logger())
{
}

HiveRef Server::getHive()
{
	return hive;
}

LoggerRef Server::getLogger()
{
	return logger;
}

LogLevel Server::getLogLevel() const
{
	return logger->getLevel();
}

void Server::setLogger(LoggerPtr log)
{
	logger = log;
}
