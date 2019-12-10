#ifndef _LISTENER_H
#define _LISTENER_H

#include <atomic>
#include <string_view>

#include "forward.hpp"
#include "typedefs.hpp"

template <class T> class Listener : public std::enable_shared_from_this<Listener<T>>
{
public:
	using Acceptor = T::acceptor;
	using Endpoint = T::endpoint;
	using Resolver = T::resolver;

	Listener(HiveRef);
	HiveRef getHive();
	Acceptor& getAcceptor();
	Strand& getStrand();
	bool hasError();
	void listen(const std::string_view&, uint16);
	void accept(ConnectionPtr<T>);
	void stop();
private:
	void dispatchAccept(ConnectionPtr<T>);
	void handleAccept(Error, ConnectionPtr<T>);
protected:
	virtual bool onAccept(ConnectionRef<T>, const std::string_view&, uint16);
	virtual void onError(Error);
protected:
	HiveRef hive;
	Acceptor acceptor;
	Strand strand;
	std::atomic_uint32_t errorState;
};

#endif // _LISTENER_H
