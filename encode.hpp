#pragma once

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstring>
#include <iterator>

#include "./common.hpp"

namespace bizwen
{
namespace encode_impl
{
namespace pattern
{
static inline constexpr auto base64 = u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static inline constexpr auto base64_url = u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
static inline constexpr auto base32 = u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";
static inline constexpr auto base32_lower = u8"abcdefghijklmnopqrstuvwxyz234567=";
static inline constexpr auto base32_hex = u8"0123456789ABCDEFGHIJKLMNOPQRSTUV=";
static inline constexpr auto base32_hex_lower = u8"0123456789abcdefghijklmnopqrstuv=";
static inline constexpr auto base32_crockford = u8"0123456789ABCDEFGHJKMNPQRSTVWXYZ=";
static inline constexpr auto base32_crockford_lower = u8"0123456789abcdefghjkmnpqrstvwxyz=";
static inline constexpr auto base16 = base32_hex;
static inline constexpr auto base16_lower = base32_hex_lower;
}; // namespace pattern

template <typename T>
inline constexpr unsigned char to_uc(T t) noexcept
{
    // T is char, unsigned char or std::byte
    return static_cast<unsigned char>(t);
}

template <rfc4648_kind Kind>
inline consteval auto get_alphabet() noexcept
{
    if constexpr (Kind == rfc4648_kind::base64)
        return pattern::base64;
    if constexpr (Kind == rfc4648_kind::base64_url)
        return pattern::base64_url;
    if constexpr (Kind == rfc4648_kind::base32)
        return pattern::base32;
    if constexpr (Kind == rfc4648_kind::base32_lower)
        return pattern::base32_lower;
    if constexpr (Kind == rfc4648_kind::base32_hex)
        return pattern::base32_hex;
    if constexpr (Kind == rfc4648_kind::base32_hex_lower)
        return pattern::base32_hex_lower;
    if constexpr (Kind == rfc4648_kind::base32_crockford)
        return pattern::base32_crockford;
    if constexpr (Kind == rfc4648_kind::base32_crockford_lower)
        return pattern::base32_crockford_lower;
    if constexpr (Kind == rfc4648_kind::base16)
        return pattern::base16;
    if constexpr (Kind == rfc4648_kind::base16_lower)
        return pattern::base16_lower;
}

template <std::size_t Count, typename T>
inline constexpr auto chars_to_int_big_endian(T begin)
{
    static_assert(Count < 9);
    static_assert(std::endian::native == std::endian::big || std::endian::native == std::endian::little);

    constexpr auto size = Count <= 4 ? 4 : 8;

    using int32_type = std::conditional_t<sizeof(int) == 4, unsigned int, unsigned long>;
    using data_type = std::conditional_t<size == 4, int32_type, unsigned long long>;

#if defined(__cpp_if_consteval) && (__cpp_if_consteval >= 202106L)
    if consteval
#else
    if (::std::is_constant_evaluated())
#endif
    {
        unsigned char buf[size]{};

        for (std::size_t i{}; i != Count; ++i, ++begin)
            buf[i] = to_uc(*begin);

        if constexpr (std::endian::native == std::endian::little)
            return std::byteswap(std::bit_cast<data_type>(buf));
        else
            return std::bit_cast<data_type>(buf);
    }
    else
    {
        data_type buf{};

        std::memcpy(&buf, begin, Count);

        if constexpr (std::endian::native == std::endian::little)
            return std::byteswap(buf);
        else
            return buf;
    }
}

template <typename A, typename I, typename O>
inline constexpr void encode_impl_b64_6(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<6>(begin);

    *first = alphabet[(data >> 58) & 63];
    ++first;
    *first = alphabet[(data >> 52) & 63];
    ++first;
    *first = alphabet[(data >> 46) & 63];
    ++first;
    *first = alphabet[(data >> 40) & 63];
    ++first;
    *first = alphabet[(data >> 34) & 63];
    ++first;
    *first = alphabet[(data >> 28) & 63];
    ++first;
    *first = alphabet[(data >> 22) & 63];
    ++first;
    *first = alphabet[(data >> 16) & 63];
    ++first;
}

template <typename A, typename I, typename O>
inline constexpr void encode_impl_b64_3(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<3>(begin);

    *first = alphabet[(data >> 26) & 63];
    ++first;
    *first = alphabet[(data >> 20) & 63];
    ++first;
    *first = alphabet[(data >> 14) & 63];
    ++first;
    *first = alphabet[(data >> 8) & 63];
    ++first;
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b64_2(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<2>(begin);

    *first = alphabet[(data >> 26) & 63];
    ++first;
    *first = alphabet[(data >> 20) & 63];
    ++first;
    *first = alphabet[(data >> 14) & 63];

    if constexpr (Padding)
    {
        *first = alphabet[64];
        ++first;
    }
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b64_1(A alphabet, I begin, O &first)
{
    auto a = to_uc(*begin);
    auto b = a >> 2;        // XXXXXX
    auto c = (a << 4) & 63; // XX0000

    *first = alphabet[b];
    ++first;
    *first = alphabet[c];
    ++first;

    if constexpr (Padding)
    {
        *first = alphabet[64]; // pad1
        ++first;
        *first = alphabet[64]; // pad2
        ++first;
    }
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b64(A alphabet, I begin, I end, O &first)
{
    if constexpr (sizeof(std::size_t) == 8)
    {
        for (; end - begin > 5; begin += 6)
            encode_impl_b64_6(alphabet, begin, first);
    }

    for (; end - begin > 2; begin += 3)
        encode_impl_b64_3(alphabet, begin, first);

    if (end - begin == 2)
        encode_impl_b64_2<Padding>(alphabet, begin, first);
    else if (end - begin) // == 1
        encode_impl_b64_1<Padding>(alphabet, begin, first);

    // == 0  fallthrough
}

template <typename A, typename I, typename O>
inline constexpr void encode_impl_b64_ctx(A alphabet, detail::buf_ref buf, detail::sig_ref sig, I begin, I end, O &first)
{
    if (sig == 2) // 0, 1, 2
    {
        if (end - begin == 0)
            return;

        unsigned char buf[3];

        buf[0] = buf[0];
        buf[1] = buf[1];
        buf[2] = to_uc(*(begin++));

        encode_impl_b64_3(alphabet, std::begin(buf), first);
    }
    else if (sig) // == 1
    {
        if (end - begin == 0)
        {
            return;
        }
        else if (end - begin == 1)
        {
            buf[1] = to_uc(*(begin++));
            sig = 2;

            return;
        }
        else // >= 2
        {
            unsigned char lbuf[3];

            lbuf[0] = buf[0];
            lbuf[1] = to_uc(*(begin++));
            lbuf[2] = to_uc(*(begin++));

            encode_impl_b64_3(alphabet, std::begin(lbuf), first);
        }
    }

    if constexpr (sizeof(std::size_t) == 8)
    {
        for (; end - begin > 5; begin += 6)
            encode_impl_b64_6(alphabet, begin, first);
    }

    for (; end - begin > 3; begin += 3)
        encode_impl_b64_3(alphabet, begin, first);

    if (end - begin == 2)
    {
        buf[0] = to_uc(*(begin++));
        buf[1] = to_uc(*(begin));
        sig = 2;
    }
    else if (end - begin != 0) // == 1
    {
        buf[1] = to_uc(*(begin));
        sig = 1;
    }
    else // NB: clear ctx
    {
        sig = 0;
    }
}

template <bool Padding, typename A, typename O>
inline constexpr void encode_impl_b64_ctx(A alphabet, detail::buf_ref buf, detail::sig_ref sig, O &first)
{
    if (sig == 2)
        encode_impl::encode_impl_b64_2<Padding>(alphabet, std::begin(buf), first);
    else if (sig) // == 1
        encode_impl::encode_impl_b64_1<Padding>(alphabet, std::begin(buf), first);
    // == 0  fallthrough

    // clear ctx
    sig = 0;
}

template <typename A, typename I, typename O>
inline constexpr void encode_impl_b32_5(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<5>(begin);

    *first = alphabet[(data >> 59) & 31];
    ++first;
    *first = alphabet[(data >> 54) & 31];
    ++first;
    *first = alphabet[(data >> 49) & 31];
    ++first;
    *first = alphabet[(data >> 44) & 31];
    ++first;
    *first = alphabet[(data >> 39) & 31];
    ++first;
    *first = alphabet[(data >> 34) & 31];
    ++first;
    *first = alphabet[(data >> 29) & 31];
    ++first;
    *first = alphabet[(data >> 24) & 31];
    ++first;
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b32_4(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<4>(begin);

    *first = alphabet[(data >> 27) & 31];
    ++first;
    *first = alphabet[(data >> 22) & 31];
    ++first;
    *first = alphabet[(data >> 17) & 31];
    ++first;
    *first = alphabet[(data >> 12) & 31];
    ++first;
    *first = alphabet[(data >> 7) & 31];
    ++first;
    *first = alphabet[(data >> 2) & 31];
    ++first;
    // NB: left shift
    *first = alphabet[(data << 3) & 31];
    ++first;

    if constexpr (Padding)
    {
        *first = alphabet[32];
        ++first;
    }
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b32_3(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<3>(begin);

    *first = alphabet[(data >> 27) & 31];
    ++first;
    *first = alphabet[(data >> 22) & 31];
    ++first;
    *first = alphabet[(data >> 17) & 31];
    ++first;
    *first = alphabet[(data >> 12) & 31];
    ++first;
    *first = alphabet[(data >> 7) & 31];
    ++first;

    if constexpr (Padding)
    {
        *first = alphabet[32];
        ++first;
        *first = alphabet[32];
        ++first;
        *first = alphabet[32];
        ++first;
    }
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b32_2(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<2>(begin);

    *first = alphabet[(data >> 27) & 31];
    ++first;
    *first = alphabet[(data >> 22) & 31];
    ++first;
    *first = alphabet[(data >> 17) & 31];
    ++first;
    *first = alphabet[(data >> 12) & 31];
    ++first;

    if constexpr (Padding)
    {
        *first = alphabet[32];
        ++first;
        *first = alphabet[32];
        ++first;
        *first = alphabet[32];
        ++first;
        *first = alphabet[32];
        ++first;
    }
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b32_1(A alphabet, I begin, O &first)
{
    auto a = to_uc(*(begin));

    *first = alphabet[a >> 3];
    ++first;
    *first = alphabet[(a << 2) & 31];
    ++first;

    if constexpr (Padding)
    {
        *first = alphabet[32];
        ++first;
        *first = alphabet[32];
        ++first;
        *first = alphabet[32];
        ++first;
        *first = alphabet[32];
        ++first;
        *first = alphabet[32];
        ++first;
        *first = alphabet[32];
        ++first;
    }
}

template <bool Padding = true, typename A, typename I, typename O>
inline constexpr void encode_impl_b32(A alphabet, I begin, I end, O &first)
{
    for (; end - begin > 4; begin += 5)
        encode_impl_b32_5(alphabet, begin, first);

    if (end - begin == 4)
        encode_impl_b32_4<Padding>(alphabet, begin, first);
    else if (end - begin == 3)
        encode_impl_b32_3<Padding>(alphabet, begin, first);
    else if (end - begin == 2)
        encode_impl_b32_2<Padding>(alphabet, begin, first);
    else if (end - begin) // == 1
        encode_impl_b32_1<Padding>(alphabet, begin, first);
    // == 0  fallthrough
}

template <typename A, typename I, typename O>
inline constexpr void encode_impl_b32_ctx(A alphabet, detail::buf_ref buf, detail::sig_ref sig, I begin, I end, O &first)
{
#if __has_cpp_attribute(assume)
    [[assume(sig < 5)]];
    [[assume(end - begin >= 0)]];
#endif

    if (end - begin + sig < 5)
    {
        for (; begin != end; ++begin, ++sig)
            buf[sig] = to_uc(*begin);

        return;
    }

    if (sig)
    {
        unsigned char lbuf[5];

        std::copy(std::begin(buf), std::begin(buf) + sig, std::begin(lbuf));
        std::copy(begin, begin + (5 - sig), std::begin(lbuf) + sig);
        begin += (5 - sig);

        encode_impl_b32_5(alphabet, std::begin(lbuf), first);
    }

    for (; end - begin > 4; begin += 5)
        encode_impl_b32_5(alphabet, begin, first);

    sig = static_cast<unsigned char>(end - begin);

    for (std::size_t i{}; i != sig; ++i, ++begin)
        buf[i] = to_uc(*begin);
}

template <bool Padding, typename A, typename O>
inline constexpr void encode_impl_b32_ctx(A alphabet, detail::buf_ref buf, detail::sig_ref sig, O &first)
{
    if (sig == 1)
        encode_impl_b32_1<Padding>(alphabet, std::begin(buf), first);
    else if (sig == 2)
        encode_impl_b32_2<Padding>(alphabet, std::begin(buf), first);
    else if (sig == 3)
        encode_impl_b32_3<Padding>(alphabet, std::begin(buf), first);
    else if (sig == 4)
        encode_impl_b32_4<Padding>(alphabet, std::begin(buf), first);

    sig = 0;
}

template <typename A, typename I, typename O>
inline constexpr void encode_impl_b16(A alphabet, I begin, I end, O &first)
{
    if constexpr (sizeof(size_t) == 8)
    {
        for (; end - begin > 7; begin += 8)
        {
            auto data = chars_to_int_big_endian<8>(begin);

            for (std::size_t i{}; i < 16; ++i)
                *(first++) = alphabet[(data >> (64 - (i + 1) * 4)) & 15];
        }
    }
    else // 32-bit machine
    {
        for (; end - begin > 3; begin += 4)
        {
            auto data = chars_to_int_big_endian<4>(begin);

            for (std::size_t i{}; i < 8; ++i)
                *(first++) = alphabet[(data >> (32 - (i + 1) * 4)) & 15];
        }
    }

    for (; begin != end; ++begin)
    {
        auto data = to_uc(*begin);

        *first = alphabet[data >> 4];
        ++first;
        *first = alphabet[data & 15];
        ++first;
    }
}

struct rfc4648_encode_fn
{
    template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename In, typename Out>
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    static
#endif
        inline constexpr Out
        operator()(In begin, In end, Out first)
#if !defined(__cpp_static_call_operator) || __cpp_static_call_operator < 202207L
            const
#endif
    {
        using in_char = std::iterator_traits<In>::value_type;

        static_assert(std::contiguous_iterator<In>);
        static_assert(std::is_same_v<in_char, char> || std::is_same_v<in_char, unsigned char> ||
                      std::is_same_v<in_char, std::byte>);

        auto begin_ptr = detail::to_address_const(begin);
        auto end_ptr = detail::to_address_const(end);

        if constexpr (detail::get_family<Kind>() == rfc4648_kind::base64)
            encode_impl::encode_impl_b64<Padding>(encode_impl::get_alphabet<Kind>(), begin_ptr, end_ptr, first);
        if constexpr (detail::get_family<Kind>() == rfc4648_kind::base32)
            encode_impl::encode_impl_b32<Padding>(encode_impl::get_alphabet<Kind>(), begin_ptr, end_ptr, first);
        if constexpr (detail::get_family<Kind>() == rfc4648_kind::base16)
            encode_impl::encode_impl_b16(encode_impl::get_alphabet<Kind>(), begin_ptr, end_ptr, first);

        return first;
    }

    template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename R, typename Out>
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    static
#endif
        inline constexpr Out
        operator()(R &&r, Out first)
#if !defined(__cpp_static_call_operator) || __cpp_static_call_operator < 202207L
            const
#endif
    {
        return operator()<Kind, Padding>(std::ranges::begin(r), std::ranges::end(r), first);
    }

    // NB: don't need padding
    template <rfc4648_kind Kind = rfc4648_kind::base64, typename In, typename Out>
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    static
#endif
        inline constexpr Out
        operator()(rfc4648_context &ctx, In begin, In end, Out first)
#if !defined(__cpp_static_call_operator) || __cpp_static_call_operator < 202207L
            const
#endif
    {
        using in_char = std::iterator_traits<In>::value_type;

        static_assert(std::contiguous_iterator<In>);
        static_assert(std::is_same_v<in_char, char> || std::is_same_v<in_char, unsigned char> ||
                      std::is_same_v<in_char, std::byte>);

        auto begin_ptr = detail::to_address_const(begin);
        auto end_ptr = detail::to_address_const(end);

        if constexpr (detail::get_family<Kind>() == rfc4648_kind::base64)
            encode_impl::encode_impl_b64_ctx(encode_impl::get_alphabet<Kind>(), ctx.buf_, ctx.sig_, begin_ptr, end_ptr,
                                             first);
        if constexpr (detail::get_family<Kind>() == rfc4648_kind::base32)
            encode_impl::encode_impl_b32_ctx(encode_impl::get_alphabet<Kind>(), ctx.buf_, ctx.sig_, begin_ptr, end_ptr,
                                             first);
        if constexpr (detail::get_family<Kind>() == rfc4648_kind::base16)
            encode_impl::encode_impl_b16(encode_impl::get_alphabet<Kind>(), begin_ptr, end_ptr, first);

        return first;
    }

    template <rfc4648_kind Kind = rfc4648_kind::base64, typename R, typename Out>
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    static
#endif
        inline constexpr Out
        operator()(rfc4648_context &ctx, R &&r, Out first)
#if !defined(__cpp_static_call_operator) || __cpp_static_call_operator < 202207L
            const
#endif
    {
        return operator()<Kind>(ctx, std::ranges::begin(r), std::ranges::end(r), first);
    }

    template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename Out>
    inline constexpr Out operator()(rfc4648_context &ctx, Out first) const
    {
        if constexpr (detail::get_family<Kind>() == rfc4648_kind::base64)
            encode_impl::encode_impl_b64_ctx<Padding>(encode_impl::get_alphabet<Kind>(), ctx.buf_, ctx.sig_, first);
        if constexpr (detail::get_family<Kind>() == rfc4648_kind::base32)
            encode_impl::encode_impl_b32_ctx<Padding>(encode_impl::get_alphabet<Kind>(), ctx.buf_, ctx.sig_, first);
        // no effect when family is base16 and CHAR_BIT is 8

        return first;
    }
};
} // namespace encode_impl

inline constexpr encode_impl::rfc4648_encode_fn rfc4648_encode;
} // namespace bizwen
