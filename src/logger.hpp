#ifndef _LOGGER_H
#define _LOGGER_H

#include <memory>
#include <ostream>
#include <string_view>
#include "typedefs.h"

extern "C" void dumpStack(int);

class Logger : public std::enable_shared_from_this<Logger>
{
public:
	Logger(std::ostream&, Byte);
	Byte getLevel() const;
	void setLevel(Byte);
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
	Byte level;
};

#endif // _LOGGER_H
