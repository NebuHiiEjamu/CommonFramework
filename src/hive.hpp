#ifndef _HIVE_H
#define _HIVE_H

#include <atomic>
#include <memory>

#include "typedefs.hpp"

using boost::asio;

class Hive : public std::enable_shared_from_this<Hive>
{
public:
	Hive();
	asio::io_context& getIOContext();
	bool stopped();
	void poll();
	void run();
	void stop();
	void reset();
private:
	asio::io_context ioContext;
	std::shared_ptr<asio::executor_work_guard<asio::io_context::executor_type>> workGuard;
	std::atomic_uint32_t shutdownSignal;
};

#endif // _HIVE_H
