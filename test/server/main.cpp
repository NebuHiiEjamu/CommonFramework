#include <csignal>

#include "../../src/logger.hpp"
#include "server.hpp"

int main(int argc, char **argv)
{
	std::signal(SIGSEGV, &dumpStack);
	std::signal(SIGABRT, &dumpStack);

	TestServer::getInstance()->run(argc, argv);
	
	return 0;
}
