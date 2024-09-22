#pragma once

#include <bit>
#include <climits>
#include <concepts>
#include <cstring>
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>

namespace bizwen
{
namespace detail
{
template <typename CharType> struct rfc4648_traits;
template <> struct rfc4648_traits<char>
{
    static inline constexpr auto base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    static inline constexpr auto base64_url = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
    static inline constexpr auto base32 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";
    static inline constexpr auto base32_lower = "abcdefghijklmnopqrstuvwxyz234567=";
    static inline constexpr auto base32_hex = "0123456789ABCDEFGHIJKLMNOPQRSTUV=";
    static inline constexpr auto base32_hex_lower = "0123456789abcdefghijklmnopqrstuv=";
    static inline constexpr auto base32_crockford = "0123456789ABCDEFGHJKMNPQRSTVWXYZ=";
    static inline constexpr auto base32_crockford_lower = "0123456789abcdefghjkmnpqrstvwxyz=";
    static inline constexpr auto base16 = base32_hex;
    static inline constexpr auto base16_lower = base32_hex_lower;
};
template <> struct rfc4648_traits<wchar_t>
{
    static inline constexpr auto base64 = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    static inline constexpr auto base64_url = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
    static inline constexpr auto base32 = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";
    static inline constexpr auto base32_lower = L"abcdefghijklmnopqrstuvwxyz234567=";
    static inline constexpr auto base32_hex = L"0123456789ABCDEFGHIJKLMNOPQRSTUV=";
    static inline constexpr auto base32_hex_lower = L"0123456789abcdefghijklmnopqrstuv=";
    static inline constexpr auto base32_crockford = L"0123456789ABCDEFGHJKMNPQRSTVWXYZ=";
    static inline constexpr auto base32_crockford_lower = L"0123456789abcdefghjkmnpqrstvwxyz=";
    static inline constexpr auto base16 = base32_hex;
    static inline constexpr auto base16_lower = base32_hex_lower;
};
template <> struct rfc4648_traits<char8_t>
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
};
template <> struct rfc4648_traits<char16_t>
{
    static inline constexpr auto base64 = u"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    static inline constexpr auto base64_url = u"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
    static inline constexpr auto base32 = u"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";
    static inline constexpr auto base32_lower = u"abcdefghijklmnopqrstuvwxyz234567=";
    static inline constexpr auto base32_hex = u"0123456789ABCDEFGHIJKLMNOPQRSTUV=";
    static inline constexpr auto base32_hex_lower = u"0123456789abcdefghijklmnopqrstuv=";
    static inline constexpr auto base32_crockford = u"0123456789ABCDEFGHJKMNPQRSTVWXYZ=";
    static inline constexpr auto base32_crockford_lower = u"0123456789abcdefghjkmnpqrstvwxyz=";
    static inline constexpr auto base16 = base32_hex;
    static inline constexpr auto base16_lower = base32_hex_lower;
};
template <> struct rfc4648_traits<char32_t>
{
    static inline constexpr auto base64 = U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    static inline constexpr auto base64_url = U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
    static inline constexpr auto base32 = U"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";
    static inline constexpr auto base32_lower = U"abcdefghijklmnopqrstuvwxyz234567=";
    static inline constexpr auto base32_hex = U"0123456789ABCDEFGHIJKLMNOPQRSTUV=";
    static inline constexpr auto base32_hex_lower = U"0123456789abcdefghijklmnopqrstuv=";
    static inline constexpr auto base32_crockford = U"0123456789ABCDEFGHJKMNPQRSTVWXYZ=";
    static inline constexpr auto base32_crockford_lower = U"0123456789abcdefghjkmnpqrstvwxyz=";
    static inline constexpr auto base16 = base32_hex;
    static inline constexpr auto base16_lower = base32_hex_lower;
};

struct rfc4648_ctx_impl
{
    unsigned char effective{}; // 0、1、2 for base64; 0 - 4 for base32
    unsigned char buf[4];
};
} // namespace detail

enum class rfc4648_kind : unsigned char
{
    base64,
    base64_url,
    base32,
    base32_lower,
    base32_hex,
    base32_hex_lower,
    base32_crockford,
    base32_crockford_lower,
    base16,
    base16_lower,
    hex = base16,
    hex_lower = base16_lower
};

namespace detail
{
template <typename T> inline constexpr unsigned char to_uc(T t) noexcept
{
    // T is char, unsigned char or std::byte
    return static_cast<unsigned char>(t);
}

template <rfc4648_kind Kind, typename Traits> inline consteval auto get_alphabet() noexcept
{
    if constexpr (Kind == rfc4648_kind::base64)
        return Traits::base64;
    if constexpr (Kind == rfc4648_kind::base64_url)
        return Traits::base64_url;
    if constexpr (Kind == rfc4648_kind::base32)
        return Traits::base32;
    if constexpr (Kind == rfc4648_kind::base32_lower)
        return Traits::base32_lower;
    if constexpr (Kind == rfc4648_kind::base32_hex)
        return Traits::base32_hex;
    if constexpr (Kind == rfc4648_kind::base32_hex_lower)
        return Traits::base32_hex_lower;
    if constexpr (Kind == rfc4648_kind::base32_crockford)
        return Traits::base32_crockford;
    if constexpr (Kind == rfc4648_kind::base32_crockford_lower)
        return Traits::base32_crockford_lower;
    if constexpr (Kind == rfc4648_kind::base16)
        return Traits::base16;
    if constexpr (Kind == rfc4648_kind::base16_lower)
        return Traits::base16_lower;
}

template <rfc4648_kind Kind> inline consteval auto get_family()
{
    if constexpr (Kind == rfc4648_kind::base64 || Kind == rfc4648_kind::base64_url)
        return rfc4648_kind::base64;
    if constexpr (Kind == rfc4648_kind::base16 || Kind == rfc4648_kind::base16_lower)
        return rfc4648_kind::base16;
    else
        return rfc4648_kind::base32;
}

template <std::size_t Count, typename T> inline constexpr auto chars_to_int_big_endian(T begin)
{
    constexpr auto size = Count <= 4 ? 4 : 8;

    using int32_type = std::conditional_t<sizeof(int) == 4, unsigned int, unsigned long>;
    using data_type = std::conditional_t<size == 4, int32_type, unsigned long long>;

    static_assert(Count < 9);
    static_assert(CHAR_BIT == 8);
    static_assert(std::endian::native == std::endian::big || std::endian::native == std::endian::little);
    static_assert(sizeof(int32_type) == 4);

#if defined(__cpp_if_consteval) && (__cpp_if_consteval >= 202106L)
    if consteval
#else
    if (::std::is_constant_evaluated())
#endif
    {
        unsigned char buf[size]{};

        for (std::size_t i{}; i < Count; ++i, ++begin)
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

template <typename A, typename I, typename O> inline constexpr void encode_impl_b64_6(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<6>(begin);

    *(first++) = alphabet[(data >> 58) & 63];
    *(first++) = alphabet[(data >> 52) & 63];
    *(first++) = alphabet[(data >> 46) & 63];
    *(first++) = alphabet[(data >> 40) & 63];
    *(first++) = alphabet[(data >> 34) & 63];
    *(first++) = alphabet[(data >> 28) & 63];
    *(first++) = alphabet[(data >> 22) & 63];
    *(first++) = alphabet[(data >> 16) & 63];
}

template <typename A, typename I, typename O> inline constexpr void encode_impl_b64_3(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<3>(begin);

    *(first++) = alphabet[(data >> 26) & 63];
    *(first++) = alphabet[(data >> 20) & 63];
    *(first++) = alphabet[(data >> 14) & 63];
    *(first++) = alphabet[(data >> 8) & 63];
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b64_2(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<2>(begin);

    *(first++) = alphabet[(data >> 26) & 63];
    *(first++) = alphabet[(data >> 20) & 63];
    *(first++) = alphabet[(data >> 14) & 63];

    if constexpr (Padding)
        *(first++) = alphabet[64];
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b64_1(A alphabet, I begin, O &first)
{
    auto a = to_uc(*begin);
    auto b = a >> 2;        // XXXXXX
    auto c = (a << 4) & 63; // XX0000

    *(first++) = alphabet[b];
    *(first++) = alphabet[c];

    if constexpr (Padding)
    {
        *(first++) = alphabet[64]; // pad1
        *(first++) = alphabet[64]; // pad2
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
    else if (end - begin != 0) // == 1
        encode_impl_b64_1<Padding>(alphabet, begin, first);

    /* == 0  fallthrough */
}

template <typename A, typename I, typename O>
inline constexpr void encode_impl_b64_ctx(A alphabet, rfc4648_ctx_impl &ctx, I begin, I end, O &first)
{
    auto effective = ctx.effective; // 0, 1, 2

    if (effective == 2)
    {
        if (end - begin == 0)
            return;

        unsigned char buf[3];
        buf[0] = ctx.buf[0];
        buf[1] = ctx.buf[1];
        buf[2] = to_uc(*(begin++));

        encode_impl_b64_3(alphabet, std::begin(buf), first);
    }
    else if (effective != 0) // == 1
    {
        if (end - begin == 0)
        {
            return;
        }
        else if (end - begin == 1)
        {
            ctx.buf[1] = to_uc(*(begin++));
            ctx.effective = 2;

            return;
        }
        else // >= 2
        {
            unsigned char buf[3];
            buf[0] = ctx.buf[0];
            buf[1] = to_uc(*(begin++));
            buf[2] = to_uc(*(begin++));

            encode_impl_b64_3(alphabet, std::begin(buf), first);
        }
    }

    // init or aligned

    if constexpr (sizeof(std::size_t) == 8)
    {
        for (; end - begin > 5; begin += 6)
            encode_impl_b64_6(alphabet, begin, first);
    }

    for (; end - begin > 3; begin += 3)
        encode_impl_b64_3(alphabet, begin, first);

    if (end - begin == 2)
    {
        ctx.buf[0] = to_uc(*(begin++));
        ctx.buf[1] = to_uc(*(begin));
        ctx.effective = 2;
    }
    else if (end - begin != 0) // == 1
    {
        ctx.buf[1] = to_uc(*(begin));
        ctx.effective = 1;
    }
    else // NB: clear ctx
    {
        ctx.effective = 0;
    }
}

template <bool Padding, typename A, typename O>
inline constexpr void encode_impl_b64_ctx(A alphabet, rfc4648_ctx_impl &ctx, O &first)
{
    auto effective = ctx.effective;

    if (effective == 2)
        detail::encode_impl_b64_2<Padding>(alphabet, std::begin(ctx.buf), first);
    else if (effective != 0) // == 1
        detail::encode_impl_b64_1<Padding>(alphabet, std::begin(ctx.buf), first);

    /* == 0  fallthrough */

    // clear ctx
    ctx.effective = 0;
}

template <typename A, typename I, typename O> inline constexpr void encode_impl_b32_5(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<5>(begin);

    *(first++) = alphabet[(data >> 59) & 31];
    *(first++) = alphabet[(data >> 54) & 31];
    *(first++) = alphabet[(data >> 49) & 31];
    *(first++) = alphabet[(data >> 44) & 31];
    *(first++) = alphabet[(data >> 39) & 31];
    *(first++) = alphabet[(data >> 34) & 31];
    *(first++) = alphabet[(data >> 29) & 31];
    *(first++) = alphabet[(data >> 24) & 31];
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b32_4(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<4>(begin);

    *(first++) = alphabet[(data >> 27) & 31];
    *(first++) = alphabet[(data >> 22) & 31];
    *(first++) = alphabet[(data >> 17) & 31];
    *(first++) = alphabet[(data >> 12) & 31];
    *(first++) = alphabet[(data >> 7) & 31];
    *(first++) = alphabet[(data >> 2) & 31];
    // NB: left shift
    *(first++) = alphabet[(data << 3) & 31];

    if constexpr (Padding)
        *(first++) = alphabet[32];
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b32_3(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<3>(begin);

    *(first++) = alphabet[(data >> 27) & 31];
    *(first++) = alphabet[(data >> 22) & 31];
    *(first++) = alphabet[(data >> 17) & 31];
    *(first++) = alphabet[(data >> 12) & 31];
    *(first++) = alphabet[(data >> 7) & 31];

    if constexpr (Padding)
    {
        *(first++) = alphabet[32];
        *(first++) = alphabet[32];
        *(first++) = alphabet[32];
    }
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b32_2(A alphabet, I begin, O &first)
{
    auto data = chars_to_int_big_endian<2>(begin);

    *(first++) = alphabet[(data >> 27) & 31];
    *(first++) = alphabet[(data >> 22) & 31];
    *(first++) = alphabet[(data >> 17) & 31];
    *(first++) = alphabet[(data >> 12) & 31];

    if constexpr (Padding)
    {
        *(first++) = alphabet[32];
        *(first++) = alphabet[32];
        *(first++) = alphabet[32];
        *(first++) = alphabet[32];
    }
}

template <bool Padding, typename A, typename I, typename O>
inline constexpr void encode_impl_b32_1(A alphabet, I begin, O &first)
{
    auto a = to_uc(*(begin));

    *(first++) = alphabet[a >> 3];
    *(first++) = alphabet[(a << 2) & 31];

    if constexpr (Padding)
    {
        *(first++) = alphabet[32];
        *(first++) = alphabet[32];
        *(first++) = alphabet[32];
        *(first++) = alphabet[32];
        *(first++) = alphabet[32];
        *(first++) = alphabet[32];
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
    else if (end - begin != 0) // == 1
        encode_impl_b32_1<Padding>(alphabet, begin, first);
    /* == 0  fallthrough */
}

template <typename A, typename I, typename O>
inline constexpr void encode_impl_b32_ctx(A alphabet, rfc4648_ctx_impl &ctx, I begin, I end, O &first)
{
    auto effective = ctx.effective;

#if __has_cpp_attribute(assume)
    [[assume(effective < 5)]];
    [[assume(end - begin >= 0)]];
#endif

    if (end - begin + effective < 5)
    {
        for (; begin != end; ++begin, ++ctx.effective)
            ctx.buf[ctx.effective] = to_uc(*begin);

        return;
    }

    if (effective)
    {
        unsigned char buf[5];

        std::copy(std::begin(ctx.buf), std::begin(ctx.buf) + effective, std::begin(buf));
        std::copy(begin, begin + (5 - effective), std::begin(buf) + effective);
        begin += (5 - effective);

        encode_impl_b32_5(alphabet, std::begin(buf), first);
    }

    for (; end - begin > 4; begin += 5)
        encode_impl_b32_5(alphabet, begin, first);

    effective = static_cast<unsigned char>(end - begin);

    for (std::size_t i{}; i < effective; ++i, ++begin)
        ctx.buf[i] = to_uc(*begin);

    ctx.effective = effective;
}

template <bool Padding, typename A, typename O>
inline constexpr void encode_impl_b32_ctx(A alphabet, rfc4648_ctx_impl &ctx, O &first)
{
    auto effective = ctx.effective;

    if (effective == 1)
        encode_impl_b32_1<Padding>(alphabet, std::begin(ctx.buf), first);
    else if (effective == 2)
        encode_impl_b32_2<Padding>(alphabet, std::begin(ctx.buf), first);
    else if (effective == 3)
        encode_impl_b32_3<Padding>(alphabet, std::begin(ctx.buf), first);
    else if (effective == 4)
        encode_impl_b32_4<Padding>(alphabet, std::begin(ctx.buf), first);

    ctx.effective = 0;
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
    else /* == 4 */
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

        *(first++) = alphabet[data >> 4];
        *(first++) = alphabet[data & 15];
    }
}
} // namespace detail

class rfc4648_ctx
{
    detail::rfc4648_ctx_impl ctx_impl;

    template <rfc4648_kind Kind, typename In, typename Out>
    friend inline constexpr Out rfc4648_encode(rfc4648_ctx &ctx, In begin, In end, Out first);

    template <rfc4648_kind Kind, typename R, typename Out>
    friend inline constexpr Out rfc4648_encode(rfc4648_ctx &ctx, R &&r, Out first);

    template <rfc4648_kind Kind, bool Padding, typename Out>
    friend inline constexpr Out rfc4648_encode(rfc4648_ctx &ctx, Out first);
};

template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename In, typename Out>
inline constexpr Out rfc4648_encode(In begin, In end, Out first)
{
    using in_char = std::remove_cvref_t<decltype(*begin)>;
    using out_char = std::remove_cvref_t<decltype(*first)>;
    using traits = detail::rfc4648_traits<out_char>;

    static_assert(std::contiguous_iterator<In>);
    static_assert(std::is_same_v<in_char, char> || std::is_same_v<in_char, unsigned char> ||
                  std::is_same_v<in_char, std::byte>);

    auto begin_ptr = std::to_address(begin);
    auto end_ptr = std::to_address(end);

    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base64)
        detail::encode_impl_b64<Padding>(detail::get_alphabet<Kind, traits>(), begin_ptr, end_ptr, first);
    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base32)
        detail::encode_impl_b32<Padding>(detail::get_alphabet<Kind, traits>(), begin_ptr, end_ptr, first);
    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base16)
        detail::encode_impl_b16(detail::get_alphabet<Kind, traits>(), begin_ptr, end_ptr, first);

    return first;
}

template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename R, typename Out>
inline constexpr Out rfc4648_encode(R &&r, Out first)
{
    return rfc4648_encode<Kind, Padding>(r.begin(), r.end(), first);
}

// NB: don't need padding
template <rfc4648_kind Kind = rfc4648_kind::base64, typename In, typename Out>
inline constexpr Out rfc4648_encode(rfc4648_ctx &ctx, In begin, In end, Out first)
{
    using in_char = std::remove_cvref_t<decltype(*begin)>;
    using out_char = std::remove_cvref_t<decltype(*first)>;
    using traits = detail::rfc4648_traits<out_char>;

    static_assert(std::contiguous_iterator<In>);
    static_assert(std::is_same_v<in_char, char> || std::is_same_v<in_char, unsigned char> ||
                  std::is_same_v<in_char, std::byte>);

    auto begin_ptr = std::to_address(begin);
    auto end_ptr = std::to_address(end);

    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base64)
        detail::encode_impl_b64_ctx(detail::get_alphabet<Kind, traits>(), ctx.ctx_impl, begin_ptr, end_ptr, first);
    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base32)
        detail::encode_impl_b32_ctx(detail::get_alphabet<Kind, traits>(), ctx.ctx_impl, begin_ptr, end_ptr, first);
    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base16)
        detail::encode_impl_b16(detail::get_alphabet<Kind, traits>(), begin_ptr, end_ptr, first);

    return first;
}

template <rfc4648_kind Kind = rfc4648_kind::base64, typename R, typename Out>
inline constexpr Out rfc4648_encode(rfc4648_ctx &ctx, R &&r, Out first)
{
    return rfc4648_encode<Kind>(ctx.ctx_impl, r.begin(), r.end(), first);
}

template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename Out>
inline constexpr Out rfc4648_encode(rfc4648_ctx &ctx, Out first)
{
    using out_char = std::remove_cvref_t<decltype(*first)>;
    using traits = detail::rfc4648_traits<out_char>;

    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base64)
        detail::encode_impl_b64_ctx<Padding>(detail::get_alphabet<Kind, traits>(), ctx.ctx_impl, first);
    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base32)
        detail::encode_impl_b32_ctx<Padding>(detail::get_alphabet<Kind, traits>(), ctx.ctx_impl, first);
    if constexpr (detail::get_family<Kind>() == rfc4648_kind::base16)
        /*no effect when CHAR_BIT is 8*/;

    return first;
}
} // namespace bizwen
