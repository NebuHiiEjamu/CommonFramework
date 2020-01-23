#include <boost/predef.h>
#include <boost/stacktrace.hpp>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <fmt/core.h>
#include <iostream>

#include "logger.hpp"

void dumpStack(int signal)
{
	std::signal(signal, SIG_DFL);
#if BOOST_OS_WINDOWS
	boost::stacktrace::safe_dump_to(fmt::format("{}\\Local\\CrashDumps\\LoggerDump.txt",
		std::getenv("APPDATA")).c_str());
#else
	boost::stacktrace::safe_dump_to(fmt::format("{}/LoggerDump.txt", std::getenv("HOME")));
#endif
	std::raise(SIGABRT);
}

Logger::Logger(std::ostream &stream = std::cout, LogLevel level = LogLevel::debug):
	stream(stream),
	level(level)
{
}

LogLevel Logger::getLevel() const
{
	return level;
}

void Logger::setLevel(LogLevel ll)
{
	level = ll;
}

void Logger::debug(const std::string_view &text)
{
	if (level >= LogLevel::debug)
	{
		std::time_t time = std::time(nullptr);
		stream << fmt::format("\x1B[96m[DEBUG] {:%F %T}: {}\033[0m", text) << std::endl;
	}
}

template <class ...Args> void Logger::debug(const std::string_view& text, const Args&... args)
{
	if (level >= LogLevel::debug) debug(fmt::format(text, args...));
}

void Logger::error(const std::string_view &text)
{
	if (level >= LogLevel::error)
		stream << fmt::format("\x1B[91m[ERROR] {:%F %T}: {}\033[0m", text) << std::endl;
}

template <class ...Args> void Logger::error(const std::string_view& text, const Args&... args)
{
	if (level >= LogLevel::error) error(fmt::format(text, args...));
}

void Logger::fatal(const std::string_view &text)
{
	if (level >= LogLevel::fatal)
		stream << fmt::format("\x1B[91m[FATAL] {:%F %T}: {}\033[0m", text) << std::endl;
}

template <class ...Args> void Logger::fatal(const std::string_view &text, const Args&... args)
{
	if (level >= LogLevel::fatal) fatal(fmt::format(text, args...));
}

void Logger::info(const std::string_view &text)
{
	if (level >= LogLevel::info) stream << fmt::format("{:%F %T}: {}", text) << std::endl;
}

template <class ...Args> void Logger::info(const std::string_view& text, const Args&... args)
{
	if (level >= LogLevel::info) info(fmt::format(text, args...));
}

void Logger::trace(const std::string_view &text)
{
	if (level >= LogLevel::trace)
		stream << fmt::format("\x1B[96m[TRACE] {:%F %T}: {}\033[0m", text) << std::endl;
}

template <class ...Args> void Logger::trace(const std::string_view& text, const Args&... args)
{
	if (level >= LogLevel::trace) trace(fmt::format(text, args...));
}

void Logger::warn(const std::string_view &text)
{
	if (level >= LogLevel::warn)
		stream << fmt::format("\x1B[93m[WARN] {:%F %T}: {}\033[0m", text) << std::endl;
}

template <class ...Args> void Logger::warn(const std::string_view& text, const Args&... args)
{
	if (level >= LogLevel::warn) warn(fmt::format(text, args...));
}
