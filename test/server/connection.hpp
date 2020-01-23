#ifndef _TEST_CONNECTION_H
#define _TEST_CONNECTION_H

#include "../../src/forward.hpp"
#include "../../src/connection.hpp

class TestConnection : public Connection<boost::asio::tcp>
{
public:
	void setSession(SessionRef);
protected:
	TestConnection(HiveRef, AcceptorPtr);
	void onAccept(const std::string_view&, uint16) override;
	void onSend(const std::string&) override;
	void onReceive(std::string&) override;
	void onError(Error) override;
	void onDisconnect() override;
private:
	SessionRef session;
	AcceptorPtr listener;
};

#endif // _TEST_CONNECTION_H
