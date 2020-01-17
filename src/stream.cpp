#include <boost/endian/conversion.hpp>
<<<<<<< HEAD
#include <cstring>
#include <iterator>
=======
>>>>>>> parent of 30a51ef... Iterators are more efficient than streams
#include <utility>
#include "stream.hpp"

Stream::Stream(Buffer &buffer):
	buffer(buffer)
{
}

Buffer& Stream::getBuffer()
{
	return buffer;
}

InStream::InStream(Buffer &buffer):
	Stream(buffer),
	internal(std::basic_istream<Byte>(buffer))
{
}

Size InStream::getPosition() const
{
	return internal.tellg();
}

Byte InStream::peek()
{
	return internal.peek();
}

template <class String> String&& InStream::read(Size bytes)
{
	String data(bytes, 0);
	internal.read(reinterpret_cast<Byte*>(data.data()), bytes);
	return std::move(bytes);
}

template <class T> T InStream::read(bool reverseEndian = false)
{
	T t;
	internal.read(reinterpret_cast<Byte*>(&t), sizeof(T));
	if (reverseEndian) t = boost::endian::endian_reverse(t);
	return t;
}

<<<<<<< HEAD
Byte InStream::read()
{
	Byte b = data.at(positionN);
	seek(1);
	return b;
}

template <class String> String&& InStream::read(Size bytes)
=======
void InStream::seek(Size position)
>>>>>>> parent of 30a51ef... Iterators are more efficient than streams
{
	internal.seekg(position);
}

void InStream::skip(Size bytes)
{
	internal.ignore(bytes);
}

OutStream::OutStream(Buffer &buffer):
	Stream(buffer),
	internal(std::basic_ostream<Byte>(buffer))
{
}

void OutStream::flush()
{
	internal.flush();
}

Size OutStream::getPosition() const
{
	return internal.tellp();
}

void OutStream::pad(Size bytes)
{
	for (Size i = 0; i < bytes; i++) internal.put(0);
}

<<<<<<< HEAD
template <class String> void OutStream::writeString(const String &s)
{
	data.insert(std::back_inserter(data), s.begin(), s.end());
	seek(s.size());
}

void OutStream::writeString(const char *s)
{
	Size len = std::strlen(s);
	data.insert(std::back_inserter(data), s, s + len);
	seek(len);
=======
void OutStream::seek(Size position)
{
	internal.seekp(position);
>>>>>>> parent of 30a51ef... Iterators are more efficient than streams
}

template <class String> void OutStream::write(const String &s)
{
	internal.write(reinterpret_cast<const Byte*>(&s[0]), s.size());
}

template <class T> void OutStream::write(T t, bool reverseEndian = false)
{
	if (reverseEndian) t = boost::endian::endian_reverse(t);
	internal.write(reinterpret_cast<const Byte*>(&t), sizeof(T));
}

void OutStream::write16(uint16 i, bool reverseEndian = false)
{
	if (reverseEndian) i = boost::endian::endian_reverse(i);
	internal.write(reinterpret_cast<const Byte*>(&i), sizeof(uint16));
}

void OutStream::write32(uint32 i, bool reverseEndian = false)
{
	if (reverseEndian) i = boost::endian::endian_reverse(i);
	internal.write(reinterpret_cast<const Byte*>(&i), sizeof(uint32));
}
