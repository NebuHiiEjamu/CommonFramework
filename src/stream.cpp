#include <algorithm>
#include <boost/endian/conversion.hpp>
#include <iterator>
#include <utility>

#include "stream.hpp"

Stream::Stream():
	positionI(data.begin()),
	positionN(0)
{
}

void Stream::flush()
{
	data.clear();
}

ByteString& Stream::get()
{
	return data;
}

Byte Stream::peek() const
{
	return data.at(positionN);
}

void Stream::seek(long pos, bool beginEnd = false)
{
	if (beginEnd && pos < 0)
		positionN = data.size() + pos;
	else if (beginEnd && pos >= 0)
		positionN = pos;
	else
		positionN += pos;
	positionI = data.begin() + positionN;
}

Size Stream::tell() const
{
	return positionN;
}

InStream::InStream(ByteString &input):
	data(input)
{
}

template <class String> String&& InStream::read(Size bytes)
{
	String str(positionI, positionI + bytes);
	seek(bytes);
	return std::move(str);
}

template <class T> T InStream::read(bool reverseEndian = false)
{
	T t;
	std::copy(position, position + sizeof(T), reinterpret_cast<T*>(&t));
	if (reverseEndian) t = boost::endian::endian_reverse(t);
	seek(sizeof(T));
	return t;
}

OutStream::OutStream()
{
}

void OutStream::pad(Size bytes)
{
	for (Size i = 0; i < bytes; i++) data.push_back(0);
	seek(bytes);
}

template <class String> void OutStream::write(const String &s)
{
	data.insert(std::back_inserter(data), s.begin(), s.end());
}

template <class T> void OutStream::write(T t, bool reverseEndian = false)
{
	if (reverseEndian) t = boost::endian::endian_reverse(t);
	for (int i = sizeof(T) - 1; i >= 0; i--)
		data.push_back(static_cast<Byte>((t & (0xFF << (i * 8))) >> (i * 8)));
	seek(sizeof(T));
}

void OutStream::write8(Byte b)
{
	data.push_back(b);
	seek(1);
}

void OutStream::write16(uint16 i, bool reverseEndian = false)
{
	if (reverseEndian) i = boost::endian::endian_reverse(i);
	data.push_back(static_cast<Byte>((i & 0xFF00) >> 8);
	data.push_back(static_cast<Byte>(i & 0xFF);
	seek(2);
}

void OutStream::write32(uint32 i, bool reverseEndian = false)
{
	if (reverseEndian) i = boost::endian::endian_reverse(i);
	data.push_back(static_cast<Byte>((i & 0xFF000000) >> 24);
	data.push_back(static_cast<Byte>((i & 0xFF0000) >> 16);
	data.push_back(static_cast<Byte>((i & 0xFF00) >> 8);
	data.push_back(static_cast<Byte>(i & 0xFF);
	seek(4);
}
