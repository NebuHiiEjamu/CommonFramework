#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <atomic>
#include <queue>
#include <string>
#include <string_view>

#include "forward.hpp"
#include "typedefs.hpp"

template <class T>
class Connection : public std::enable_shared_from_this<Connection<T>>
{
public:
	using Resolver = T::resolver;
	using Socket = T::socket;

	HiveRef getHive();
	Strand& getStrand();
	Socket& getSocket();
	Address& getAddress() const;
	std::string getHostName() const;
	uint16 getPort() const;
	uint32 getErrorState() const;
	Size getInBufferSize() const;
	void setInBufferSize(Size);
	bool hasError();
	void disconnect();
	void receive(Size);
	void send(const std::string&);
	void connect(const std::string_view&, uint16);
private:
	void dispatchReceive(Size);
	void dispatchSend(const std::string&);
	void startReceive(Size);
	void startSend();
	void handleReceive(Error, Size);
	void handleSend(Error, const std::string&);
	void handleConnect(Error);
	void startError(Error);
protected:
	Connection(HiveRef);
	virtual void onAccept(const std::string_view&, uint16);
	virtual void onConnect(const std::string_view&, uint16);
	virtual void onSend(const std::string&);
	virtual void onReceive(std::string&);
	virtual void onError(Error);
	virtual void onDisconnect();
protected:
	HiveRef hive;
	Socket socket;
	Strand strand;
	std::string inBuffer;
	Resolver resolver;
	std::atomic_uint32_t errorState;
	std::queue<Size> pendingReceives;
	std::queue<std::string> pendingSends;
	Size inBufferSize;
};

#endif // _CONNECTION_H
