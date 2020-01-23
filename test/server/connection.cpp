#include "../../src/acceptor.hpp"
#include "../../src/hive.hpp"
#include "connection.hpp"

TestConnection::TestConnection(HiveRef hive, AcceptorPtr acceptor):
	Connection(hive),
	acceptor(acceptor)
{
}

void TestConnection::setSession(SessionRef session)
{
	this->session = session;
}

void TestConnection::onAccept(const std::string_view&, uint16)
{
	TestConnectionPtr connection(new TestConnection(hive, acceptor));
	acceptor->accept(connection);

	TestServer::getInstance()->createSession(id, std::dynamic_pointer_cast<TestConnection>
		shared_from_this()));
}

void TestConnection::onSend(const std::string&)
{
}

void TestConnection::onReceive(std::string &data)
{
	TestServer::getInstance()->sendToAll(session->getId(), data);
}

void TestConnection::onError(Error)
{
}

void TestConnection::onDisconnect()
{
}
