#ifndef _STREAM_H
#define _STREAM_H

#include <istream>
#include <ostream>
#include "typedefs.hpp"

class InStream
{
public:
	InStream(Buffer&);
	Size getPosition() const;
	Byte peek();
	template <class String> String&& read(Size);
	template <class T> T read(bool);
	void seek(Size);
	void skip(Size);
protected:
	Buffer &buffer;
	std::basic_istream<Byte> internal;
};

class OutStream
{
public:
	OutStream(Buffer&);
	void flush();
	Size getPosition() const;
	void pad(Size);
	void seek(Size);
	template <class String> void write(const String&);
	template <class T> void write(T, bool);
	void write16(uint16, bool); // for ambiguity
	void write32(uint32, bool); // for ambiguity
protected:
	Buffer &buffer;
	std::basic_ostream<Byte> internal;
};

#endif // _STREAM_H
