#ifndef _UTILS_H
#define _UTILS_H

#include <boost/endian/conversion.hpp>
#include <utility>

#include "typedefs.hpp"

template <class String> uint32 opcodeToInt(const String &s, bool reverseEndian = false)
{
	uint32 i = static_cast<uint32>(s[0] << 24) | static_cast<uint32>(s[1] << 16) |
		static_cast<uint32>(s[2] << 8) | static_cast<uint32>(s[3]);
	if (reverseEndian) return boost::endian::endian_reverse(i);
	return i;
}

template <class String> String&& intToOpcode(uint32 i, bool reverseEndian = false)
{
	if (reverseEndian) boost::endian::endian_reverse_inplace(i);
	String s(static_cast<char>((i & 0xFF000000) >> 24));
	s.push_back(static_cast<char>((i & 0xFF0000) >> 16));
	s.push_back(static_cast<char>((i & 0xFF00) >> 8));
	s.push_back(static_cast<char>(i & 0xFF));
	return std::move(s);
}

#endif // _UTILS_H
