#ifndef _STREAM_H
#define _STREAM_H

#include <boost/endian/conversion.hpp>
#include <sstream>
#include <string>
#include <utility>

#include "typedefs.hpp"

class InStream
{
public:
	void ignore(Size count)
	{
		internal.ignore(count);
	}

	Byte peek() const
	{
		return internal.peek();
	}

	std::string&& rdbuf()
	{
		return internal.str();
	}

	template <class String> String&& read(Size count)
	{
		String s(count, 0);
		internal.read(reinterpret_cast<char*>(s.data()), count);
		return std::move(s);
	}

	template <class T> T read(bool bigEndian = false)
	{
		T t;
		internal.read(reinterpret_cast<char*>(&t), sizeof(T));
		if (bigEndian) return boost::endian::big_to_native(t);
		return t;
	}

	void seek(Size position, std::ios_base::seekdir direction = std::ios_base::cur)
	{
		internal.seekg(position, direction);
	}

	Size tell() const
	{
		return internal.tellg();
	}
private:
	std::istringstream internal;
};

class OutStream
{
public:
	void ignore(Size count)
	{
		for (Size i = 0; i < count; i++) internal.put(0);
	}

	std::string&& rdbuf()
	{
		return internal.str();
	}

	void seek(Size position, std::ios_base::seekdir direction = std::ios_base::cur)
	{
		internal.seekp(position, direction);
	}

	Size tell() const
	{
		return internal.tellp();
	}

	template <class String> void write(const String &s)
	{
		internal.write(reinterpret_cast<const char*>(s.data()), s.size());
	}

	void write(T t, bool bigEndian = false)
	{
		if (bigEndian) boost::endian::big_to_native_inplace(t);
		internal.write(reinterpret_cast<const char*>(&t), sizeof(T);
	}

	void write8(Byte b) // for ambiguity
	{
		internal.put(static_cast<char>(b));
	}

	void write16(uint16 i, bool bigEndian = false) // for ambiguity
	{
		write(i, bigEndian);
	}

	void write32(uint32 i, bool bigEndian = false) // for ambiguity
	{
		write(i, bigEndian);
	}
	
private:
	std::ostringstream internal;
};

#endif // _STREAM_H
