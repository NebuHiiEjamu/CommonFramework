#ifndef _TYPEDEFS_H
#define _TYPEDEFS_H

#include <boost/asio.hpp>
#include <boost/safe_numerics/safe_integer.hpp>
#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <mutex>
#include <vector>

using Address = boost::asio::ip::address;
using Byte = std::uint8_t;
using ByteString = std::vector<Byte>;
using Error = boost::asio::error_code;
using FilePath = std::filesystem::path;
using int16 = std::int16_t;
using int32 = std::int32_t;
using LockGuard = std::lock_guard<std::mutex>;
using sint16 = boost::numeric::safe<int16>;
using sint32 = boost::numeric::safe<int32>;
using Size = std::size_t;
using Strand = boost::asio::io_context::strand;
using Timestamp = std::chrono::time_point<std::chrono::system_clock>;
using Uuid = boost::uuids::uuid;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using suint16 = boost::numeric::safe<uint16>;
using suint32 = boost::numeric::safe<uint32>;

#endif // _TYPEDEFS_H
