#ifndef _SERVER_H
#define _SERVER_H

#include <memory>
#include "forward.hpp"

class Server : public std::enable_shared_from_this<Server>
{
public:
	Server(LoggerPtr);
	LoggerRef getLogger();
	LogLevel getLogLevel() const;
	virtual void run(int, char**) = 0;
protected:
	LoggerPtr logger;
};

#endif // _SERVER_H
