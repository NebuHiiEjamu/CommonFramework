#ifndef _TEST_SERVER_H
#define _TEST_SERVER_H

#include <map>
#include <mutex>
#include <string_view>

#include "forward.hpp"
#include "../../src/server.hpp"
#include "../../src/typedefs.hpp"

class TestServer final : public Server
{
public:
	static TestServerRef getInstance();

	~TestServer();
	bool createSession(int32, TestConnectionPtr);
	void removeSession(int32);
	int32 getNextUserId();
	uint32 getUserCount();
	void sendToAll(int32, const std::string_view&);
	void run(int, char**) override;
private:
	TestServer();

private:
	static std::shared_ptr<TestServer> instance;

	std::map<int32, SessionPtr> sessionMap;
	std::mutex mutex;
	sint32 nextUserId;
};

#endif // _TEST_SERVER_H
