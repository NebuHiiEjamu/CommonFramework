#include <boost/predef.h>
#include <boost/stacktrack.hpp>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <fmt/core.h>

#include "logger.hpp"

void dumpStack(int signal)
{
	std::signal(signal, SIG_DFL);
#if BOOST_OS_WINDOWS
	boost::stacktrace::safe_dump_to(fmt::format("{}\\Local\\CrashDumps\\LoggerDump.txt",
		std::getenv("APPDATA")));
#else
	boost::stacktrace::safe_dump_to(fmt::format("{}/LoggerDump.txt", std::getenv("HOME")));
#endif
	std::raise(SIGABRT);
}

Logger::Logger(std::ostream &stream, Byte level):
	stream(stream),
	level(level)
{
}

Byte Logger::getLevel() const
{
	return level;
}

void Logger::setLevel(Byte b)
{
	level = b;
}

void Logger::debug(const std::string_view &text)
{
	std::time_t time = std::time(nullptr);
	stream << fmt::format("\x1B[96m[DEBUG] {:%F %T}: {}\033[0m", text);
}

template <typename Args...> void Logger::debug(const std::string_view& text, const Args&... args)
{
	debug(fmt::format(text, args...));
}

void Logger::error(const std::string_view &text)
{
	stream << fmt::format("\x1B[91m[ERROR] {:%F %T}: {}\033[0m", text);
}

template <typename Args...> void Logger::error(const std::string_view& text, const Args&... args)
{
	error(fmt::format(text, args...));
}

void Logger::fatal(const std::string_view&)
{
	stream << fmt::format("\x1B[91m[FATAL] {:%F %T}: {}\033[0m", text);
}

template <typename Args...> void Logger::fatal(const std::string_view& text, const Args&... args)
{
	fatal(fmt::format(text, args...));
}

void Logger::info(const std::string_view&)
{
	stream << fmt::format("{:%F %T}: {}", text);
}

template <typename Args...> void Logger::info(const std::string_view& text, const Args&... args)
{
	info(fmt::format(text, args...));
}

void Logger::trace(const std::string_view&)
{
	stream << fmt::format("\x1B[96m[TRACE] {:%F %T}: {}\033[0m", text);
}

template <typename Args...> void Logger::trace(const std::string_view& text, const Args&... args)
{
	trace(fmt::format(text, args...));
}

void Logger::warn(const std::string_view&)
{
	stream << fmt::format("\x1B[93m[WARN] {:%F %T}: {}\033[0m", text);
}

template <typename Args...> void Logger::warn(const std::string_view& text, const Args&... args)
{
	warn(fmt::format(text, args...));
}
