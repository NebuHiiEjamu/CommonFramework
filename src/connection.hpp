#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <atomic>
#include <queue>
#include <string_view>

#include "forward.hpp"
#include "typedefs.hpp"

template <class T> class Connection : public std::enable_shared_from_this<Connection<T>>
{
public:
	using Endpoint = T::endpoint;
	using Resolver = T::resolver;
	using Socket = T::socket;

	HivePtr getHive();
	Strand& getStrand();
	Socket& getSocket();
	Address& getAddress() const;
	std::string getHostName() const;
	uint16 getPort() const;
	Size getInBufferSize() const;
	void setInBufferSize(Size);
	bool hasError();
	void disconnect();
	void receive(Size);
	void send(const Buffer&);
private:
	void dispatchReceive(Size);
	void dispatchSend(const Buffer&);
	void startReceive(Size);
	void startSend();
	void handleReceive(Error, Size);
	void handleSend(Error, const Buffer&);
	void startError(Error);
protected:
	Connection(HivePtr);
	virtual void onAccept(std::string_view, uint16);
	virtual void onConnect(std::string_view, uint16);
	virtual void onSend(const Buffer&);
	virtual void onReceive(Buffer&);
	virtual void onError(Error);
	virtual void onDisconnect();
protected:
	HivePtr hive;
	Socket socket;
	Strand strand;
	Buffer inBuffer;
	std::atomic_uint32_t errorState;
	std::queue<Size> pendingReceives;
	std::queue<Buffer> pendingSends;
	Size inBufferSize;
};

#endif // _CONNECTION_H
