g++ -std=gnu++17 main.cpp connection.cpp server.cpp session.cpp ../../src/acceptor.cpp ../../src/connection.cpp ../../src/hive.cpp ../../src/logger.cpp ../../src/server.cpp -o testserv -lws2_32 -lmswsock -lfmt -lboost_stacktrace_windbg-mt
