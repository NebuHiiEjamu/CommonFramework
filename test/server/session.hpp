#ifndef _SESSION_H
#define _SESSION_H

#include "forward.hpp"
#include "../../src/forward.hpp"
#include "../../src/typedefs.hpp"

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(int32, TestConnectionPtr);
	sint32 getId() const;
private:
	TestConnectionPtr connection;
	int32 id;
};

#endif // _SESSION_H
