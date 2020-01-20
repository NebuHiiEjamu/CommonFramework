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

	template <class T> T read(bool reverseEndian = false)
	{
		T t;
		internal.read(reinterpret_cast<char*>(&t), sizeof(T));
		if (reverseEndian) return boost::endian::big_to_native(t);
		return t;
	}

	template <class String> String&& readPString()
	{
		Byte length = read();
		String s = read(length);
		return std::move(s);
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

	template <class String> void writeString(const String &s)
	{
		internal.write(reinterpret_cast<const char*>(s.data()), s.size());
	}

	void write(T t, bool reverseEndian = false)
	{
		if (reverseEndian) boost::endian::big_to_native_inplace(t);
		internal.write(reinterpret_cast<const char*>(&t), sizeof(T);
	}

	void write8(Byte b) // for ambiguity
	{
		internal.put(static_cast<char>(b));
	}

	void write16(uint16 i, bool reverseEndian = false) // for ambiguity
	{
		write(i, reverseEndian);
	}

	void write32(uint32 i, bool reverseEndian = false) // for ambiguity
	{
		write(i, reverseEndian);
	}

	template <class String> void writePString(const String &s)
	{
		write8(s.size());
		writeString(s);
	}
	
private:
	std::ostringstream internal;
};

#endif // _STREAM_H
