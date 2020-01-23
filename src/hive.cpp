#include "hive.hpp"

using boost::asio;

Hive::Hive():
	workGuard(asio::make_work_guard(ioContext)),
	shutdownSignal(0)
{
}

asio::io_context& Hive::getIOContext()
{
	return ioContext;
}

bool Hive::stopped()
{
	uint32 v1 = 1;
	uint32 v2 = 1;
	return shutdownSignal.compare_exchange_strong(v1, v2);
}

void Hive::poll()
{
	ioContext.poll();
}

void Hive::run()
{
	ioContext.run();
}

void Hive::stop()
{
	uint32 v1 = 1;
	uint32 v2 = 0;

	if (!shutdownSignal.compare_exchange_strong(v1, v2))
	{
		workGuard.reset();
		ioContext.run();
		ioContext.stop();
	}
}

void Hive::reset()
{
	uint32 v1 = 0;
	uint32 v2 = 1;

	if (shutdownSignal.compare_exchange_strong(v1, v2))
	{
		ioContext.reset();
		workGuard.reset(asio::make_work_guard(ioContext)),
	}
}
