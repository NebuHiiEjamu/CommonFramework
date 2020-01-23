#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include <atomic>
#include <string_view>

#include "forward.hpp"
#include "typedefs.hpp"

class Acceptor : public std::enable_shared_from_this<Acceptor>
{
public:
	Acceptor(HiveRef);
	HiveRef getHive();
	boost::asio::ip::tcp::acceptor& getAcceptor();
	Strand& getStrand();
	bool hasError();
	void listen(const std::string_view&, uint16);
	void accept(ConnectionPtr<boost::asio::ip::tcp>);
	void stop();
private:
	void dispatchAccept(ConnectionPtr<boost::asio::ip::tcp>);
	void handleAccept(Error, ConnectionPtr<boost::asio::ip::tcp>);
protected:
	virtual bool onAccept(ConnectionRef<boost::asio::ip::tcp>, const std::string_view&, uint16);
	virtual void onError(Error);
protected:
	HiveRef hive;
	boost::asio::ip::tcp::acceptor acceptor;
	Strand strand;
	boost::asio::ip::tcp::resolver resolver;
	std::atomic_uint32_t errorState;
};

#endif // _ACCEPTOR_H
