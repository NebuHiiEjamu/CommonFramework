#ifndef _STREAM_H
#define _STREAM_H

#include <istream>
#include <ostream>
#include "typedefs.hpp"

class Stream
{
public:
	Stream(Buffer&);
	Buffer& getBuffer();
protected:
	Buffer &buffer;
};

class InStream : public Stream
{
public:
	InStream(Buffer&);
	Size getPosition() const;
	Byte peek();
	template <class String> String&& read(Size);
	template <class T> T read(bool);
<<<<<<< HEAD
	Byte read();
=======
	void seek(Size);
	void skip(Size);
protected:
	std::basic_istream<Byte> internal;
>>>>>>> parent of 30a51ef... Iterators are more efficient than streams
};

class OutStream : public Stream
{
public:
	OutStream(Buffer&);
	void flush();
	Size getPosition() const;
	void pad(Size);
<<<<<<< HEAD
	template <class String> void writeString(const String&);
	void writeString(const char*);
	template <class T> void write(T, bool);
	void write8(Byte); // for ambiguity
	virtual void write16(uint16, bool); // for ambiguity
	virtual void write32(uint32, bool); // for ambiguity
=======
	void seek(Size);
	template <class String> void write(const String&);
	template <class T> void write(T, bool);
	void write16(uint16, bool); // for ambiguity
	void write32(uint32, bool); // for ambiguity
protected:
	std::basic_ostream<Byte> internal;
>>>>>>> parent of 30a51ef... Iterators are more efficient than streams
};

#endif // _STREAM_H
