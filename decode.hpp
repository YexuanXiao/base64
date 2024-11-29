#pragma once

#include <algorithm>
#include <bit>
#include <climits>
#include <concepts>
#include <cstring>
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>

#include "./common.hpp"

namespace bizwen
{
namespace decode_impl
{
namespace data
{
static inline constexpr unsigned char base64[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 62,   0xFF, 0xFF, 0xFF, 63,   52,   53,   54,   55,   56,   57,   58,   59,   60,
    61,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    10,
    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
    43,   44,   45,   46,   47,   48,   49,   50,   51,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static inline constexpr unsigned char base64_url[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x62, 0xFF, 0xFF, 52,   53,   54,   55,   56,   57,   58,   59,   60,
    61,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    10,
    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   0xFF, 0xFF, 0xFF, 0xFF,
    63,   0xFF, 26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
    43,   44,   45,   46,   47,   48,   49,   50,   51,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static inline constexpr unsigned char base32[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 26,   27,   28,   29,   30,   31,   0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    10,
    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static inline constexpr unsigned char base32_lower[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 26,   27,   28,   29,   30,   31,   0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    10,   11,   12,   13,   14,   15,   16,
    17,   18,   19,   20,   21,   22,   23,   24,   25,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static inline constexpr unsigned char base32_hex[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,
    9,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 10,   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static inline constexpr unsigned char base32_hex_lower[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,
    9,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 10,   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
    27,   28,   29,   30,   31,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static inline constexpr unsigned char base32_crockford[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,
    9,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 10,   11,   12,   13,   14,   15,   16,   17,   0xFF, 18,   19,
    0xFF, 20,   21,   0xFF, 22,   23,   24,   25,   26,   0xFF, 27,   28,   29,   30,   31,   0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static inline constexpr unsigned char base32_crockford_lower[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,
    9,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 10,   11,   12,   13,   14,   15,   16,   17,   0xFF, 18,   19,   0xFF, 20,   21,   0xFF, 22,   23,
    24,   25,   26,   0xFF, 27,   28,   29,   30,   31,   32,   0xFF, 0xFF, 0xFF, 0xFF};
static inline constexpr unsigned char base16[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,
    9,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 10,   11,   12,   13,   14,   15,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static inline constexpr unsigned char base16_lower[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,
    9,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 10,   11,   12,   13,   14,   15,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
} // namespace data

template <typename T> unsigned char normalize(T t)
{
    using u = std::make_unsigned_t<T>;
    return static_cast<unsigned char>((u(t) >> 7) ? u(0xFF) : u(t));
}

template <rfc4648_kind Kind> inline consteval auto get_data() noexcept
{
    if constexpr (Kind == rfc4648_kind::base64)
        return data::base64;
    if constexpr (Kind == rfc4648_kind::base64_url)
        return data::base64_url;
    if constexpr (Kind == rfc4648_kind::base32)
        return data::base32;
    if constexpr (Kind == rfc4648_kind::base32_lower)
        return data::base32_lower;
    if constexpr (Kind == rfc4648_kind::base32_hex)
        return data::base32_hex;
    if constexpr (Kind == rfc4648_kind::base32_hex_lower)
        return data::base32_hex_lower;
    if constexpr (Kind == rfc4648_kind::base32_crockford)
        return data::base32_crockford;
    if constexpr (Kind == rfc4648_kind::base32_crockford_lower)
        return data::base32_crockford_lower;
    if constexpr (Kind == rfc4648_kind::base16)
        return data::base16;
    if constexpr (Kind == rfc4648_kind::base16_lower)
        return data::base16_lower;
}

template <typename In, typename Out> inline constexpr In decode_impl_b64(In begin, In end, Out &first)
{
    static_assert(std::is_pointer_v<In>);

    for (; begin != end; ++begin)
    {
    }
}
} // namespace decode_impl

template <typename End, typename Out> struct rfc4648_decode_result
{
    End end;
    Out out;
};

template <rfc4648_kind Kind = rfc4648_kind::base64, typename In, typename Out>
inline constexpr rfc4648_decode_result<In, Out> rfc4648_decode(In begin, In end, Out first)
{
    using in_char = std::iterator_traits<In>::value_type;

    static_assert(std::contiguous_iterator<In>);
    static_assert(std::is_same_v<in_char, char> || std::is_same_v<in_char, wchar_t> ||
                  std::is_same_v<in_char, char8_t> || std::is_same_v<in_char, char16_t> ||
                  std::is_same_v<in_char, char32_t>);

    auto begin_ptr = detail::to_address_const(begin);
    auto end_ptr = detail::to_address_const(end);

    decltype(begin_ptr) last_ptr = {};

    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base64)
        last_ptr = decode_impl::decode_impl_b64(decode_impl::get_data<Kind>(), begin_ptr, end_ptr, first);
    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base32)
        ;
    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base16)
        ;

    return {begin + (last_ptr - begin_ptr), std::move(first)};
}

} // namespace bizwen