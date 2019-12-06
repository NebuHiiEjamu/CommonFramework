#ifndef _STREAM_H
#define _STREAM_H

#include <istream>
#include <ostream>
#include "typedefs.hpp"

class StreamBase
{
public:
	StreamBase(Buffer&);
	Buffer& getBuffer();
protected:
	Buffer &buffer;
};

class InStream : public StreamBase
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
	std::basic_istream<Byte> internal;
};

class OutStream : public StreamBase
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
	std::basic_ostream<Byte> internal;
};

#endif // _STREAM_H
