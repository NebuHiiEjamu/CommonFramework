#include "session.hpp"
#include "connection.hpp"

Session::Session(int32 id, TestConnectionPtr connection):
	connection(connection),
	id(id)
{
}

int32 Session::getId() const
{
	return id;
}
