# base64

A plain header-only [RFC 4648](https://www.rfc-editor.org/rfc/rfc4648) encoding library, but with the most modern C++ API design.

All variants of RFC 4648 are supported and the Crockford variant is available.

Supports input from discontinuous multiple buffers.

Support non-padding for secure base64 url variant.

Support `constexpr` compile-time caculation.

C++23 required (`consteval`, concepts, endian and byteswap).

## Status

encode done.

## Synopsis

```cpp
enum class rfc4648_kind
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
// All special member functions are trivial and has non-trivial but noexcept default constructor
class rfc4648_ctx;
// All overloads are constexpr
template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename In, typename Out>
Out rfc4648_encode(In begin, In end, Out first);
template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename R, typename Out>
Out rfc4648_encode(R&& r, Out first);
template <rfc4648_kind Kind = rfc4648_kind::base64, typename In, typename Out>
Out rfc4648_encode(rfc4648_ctx& ctx, In begin, In end, Out first);
template <rfc4648_kind Kind = rfc4648_kind::base64, typename R, typename Out>
Out rfc4648_encode(rfc4648_ctx& ctx, R&& r, Out first);
template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename Out>
Out rfc4648_encode(rfc4648_ctx& ctx, Out first);
```

`R` must satisfy *ContinuousContainer* , `In` must satisfy *ContinuousIterator* and `Out` must satisfy *OutputIterator*.

Let `n` is the length of the output as specified by RFC 4648.

If [`begin`, `end`) is not a valid range, or [`first`, `first + n`) is not a valid range, or if [`begin`, `end`) and [`first`, `first + n`) overlap, or if `r` and `first` overlap, the behavior is undefined.

Throws any exceptions from incrementing `first`. After an exception is thrown, [`first`, `first + n`) holds unspecified values, and `ctx` will be in an unspecified state.

## Example

```cpp
#include <string>
#include "rfc4648.hpp"
int main()
{
    std::string src{ "ABCDEFGHIJKLMN" };

    // single input buffer
    std::string dest;
    dest.resize((src.size() + 3) / 3 * 4);
    bizwen::rfc4648_encode(src.begin(), src.end(), dest.begin());

    // discontinuous multiple buffers
    std::string dest1;
    dest1.resize((src.size() * 3 + 3) / 3 * 4);
    // init context
    bizwen::rfc4648_ctx ctx;
    // init encode
    auto it = bizwen::rfc4648_encode(ctx, src.begin(), src.end(), dest1.begin());
    it = bizwen::rfc4648_encode(ctx, src.begin(), src.end(), it);
    it = bizwen::rfc4648_encode(ctx, src.begin(), src.end(), it);
    // final
    it = rfc4648_encode(ctx, it);

    // ranges
    std::string dest2;
    dest2.resize((src.size() + 3) / 3 * 4);
    bizwen::rfc4648_encode(src, dest2.begin());

    // byte array and wstring
    std::wstring dest3;
    dest3.resize((src.size() + 3) / 3 * 4);
    bizwen::rfc4648_encode((std::byte*)src.data(), (std::byte*)src.data() + src.size(), dest3.begin());
}
```
