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
};

class OutStream : public Stream
{
public:
	OutStream();
	void pad(Size);
	template <class String> void write(const String&);
	template <class T> void write(T, bool);
	void write8(uint16); // for ambiguity
	virtual void write16(uint16, bool); // for ambiguity
	virtual void write32(uint32, bool); // for ambiguity
};

#endif // _STREAM_H
