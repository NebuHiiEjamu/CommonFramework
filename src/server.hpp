#ifndef _SERVER_H
#define _SERVER_H

#include <memory>

class Server : public std::enable_shared_from_this<Server>
{
public:
	virtual void run(int, char**) = 0;
};

#endif // _SERVER_H
