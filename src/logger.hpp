#ifndef _LOGGER_H
#define _LOGGER_H

#include <memory>
#include <ostream>
#include <string_view>
#include "typedefs.hpp"

extern "C" void dumpStack(int);

enum class LogLevel : Byte
{
	fatal,
	error,
	warn,
	info,
	trace,
	debug
};

class Logger : public std::enable_shared_from_this<Logger>
{
public:
	Logger(std::ostream&, LogLevel);
	LogLevel getLevel() const;
	void setLevel(LogLevel);
	void debug(const std::string_view&);
	template <typename Args...> void debug(const std::string_view&, const Args&...);
	void error(const std::string_view&);
	template <typename Args...> void error(const std::string_view&, const Args&...);
	void fatal(const std::string_view&);
	template <typename Args...> void fatal(const std::string_view&, const Args&...);
	void info(const std::string_view&);
	template <typename Args...> void info(const std::string_view&, const Args&...);
	void trace(const std::string_view&);
	template <typename Args...> void trace(const std::string_view&, const Args&...);
	void warn(const std::string_view&);
	template <typename Args...> void warn(const std::string_view&, const Args&...);
private:
	std::ostream &stream;
	LogLevel level;
};

#endif // _LOGGER_H
