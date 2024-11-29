#pragma once

#include <memory>
#include <type_traits>
namespace bizwen
{

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
template <typename T> auto to_address_const(T t)
{
    auto ptr = std::to_address(t);
    using const_pointer = std::add_const_t<std::remove_reference_t<decltype(*t)>> *;
    return const_pointer(ptr);
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
} // namespace detail

} // namespace bizwen