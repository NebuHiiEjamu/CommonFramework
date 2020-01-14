#ifndef _STREAM_H
#define _STREAM_H

#include "typedefs.hpp"

class Stream
{
public:
	Stream();
	void flush();
	ByteString& get();
	Byte peek() const;
	void seek(int, bool);
	Size tell() const;
protected:
	ByteString data;
	ByteString::iterator positionI;
	Size positionN;
};

class InStream : public Stream
{
public:
	InStream(ByteString&);
	template <class String> String&& read(Size);
	template <class T> T read(bool);
	Byte read();
};

class OutStream : public Stream
{
public:
	OutStream();
	void pad(Size);
	template <class String> void writeString(const String&);
	template <class T> void write(T, bool);
	void write8(Byte); // for ambiguity
	virtual void write16(uint16, bool); // for ambiguity
	virtual void write32(uint32, bool); // for ambiguity
};

#endif // _STREAM_H
