#ifndef _TEST_FORWARD_H
#define _TEST_FORWARD_H

#include <memory>

class TestConnection;
class TestServer;
class Session;

using TestConnectionPtr = std::shared_ptr<TestConnection>;
using TestConnectionRef = std::weak_ptr<TestConnection>;
using TestServerRef = std::weak_ptr<TestServer>;
using SessionPtr = std::shared_ptr<Session>;
using SessionRef = std::weak_ptr<Session>;

#endif // _TEST_FORWARD_H
