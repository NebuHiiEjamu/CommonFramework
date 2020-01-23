#include <functional>
#include <thread>

#include "server.hpp"
#include "connection.hpp"
#include "session.hpp"
#include "../../src/hive.hpp"

std::shared_ptr<TestServer> TestServer::instance = std::make_shared<TestServer>();

TestServer::TestServer():
	nextUserId(1)
{
}

bool TestServer::createSession(int32 id, TestConnectionPtr connection)
{
	LockGuard lock(mutex);
	SessionPtr newSession = std::make_shared<Session>(id, connection);
	connection->setSession(newSession);
	sessionMap[id] = newSession;

	return true;
}

void TestServer::removeSession(int32 id)
{
	sessionMap.erase(id);
}

sint32 TestServer::getNextUserId()
{
	LockGuard lock(mutex);
	return nextUserId++;
}

uint32 TestServer::getUserCount()
{
	LockGuard lock(mutex);
	return sessionMap.size();
}

TestServerRef TestServer::getInstance()
{
	return instance;
}

void TestServer::sendToAll(int32 sess, const std::string_view &data)
{
	for (auto &[id, user] : sessionMap)
		if (sess != id) user->connection->send(data);
}

void TestServer::run(int, char**)
{
	std::vector<std::thread> pool;
	pool.emplace_back(std::thread(std::bind(&Hive::run, TestServer::getInstance()->getHive().get())));

	for (auto &t : pool) t.join();
}
