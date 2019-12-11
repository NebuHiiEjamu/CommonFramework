#ifndef _SERVER_H
#define _SERVER_H

#include <memory>
#include "forward.hpp"

class Server : public std::enable_shared_from_this<Server>
{
public:
	HiveRef getHive();
	LoggerRef getLogger();
	LogLevel getLogLevel() const;
	void setLogger(LoggerPtr);
	virtual void run(int, char**) = 0;
protected:
	Server();
protected:
	HivePtr hive;
	LoggerPtr logger;
};

#endif // _SERVER_H
