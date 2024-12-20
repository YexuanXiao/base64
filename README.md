# rfc4648/base64/base32/base16-hex

A plain header-only [RFC 4648](https://www.rfc-editor.org/rfc/rfc4648) encoding/decoding library, but with the most modern C++ API design.

All variants of RFC 4648 are supported and the Crockford variant is available.

Supports input from discontinuous multiple buffers.

Support non-padding for secure base64 url variant.

Support `constexpr` compile-time caculation.

C++23 required (`std::byteswap`).

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
//
template <typename In, typename Out>
struct rfc4648_decode_result
{
    In end;
    Out out;
};
// All overloads are constexpr
// Encode
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
// Decode
template <rfc4648_kind Kind = rfc4648_kind::base64, typename In, typename Out>
rfc4648_decode_result<In, Out> rfc4648_decode(In begin, In end, Out first);
template <rfc4648_kind Kind = rfc4648_kind::base64, typename R, typename Out>
rfc4648_decode_result<In, Out> rfc4648_decode(R&& r, Out first);
template <rfc4648_kind Kind = rfc4648_kind::base64, typename In, typename Out>
rfc4648_decode_result<In, Out> rfc4648_decode(rfc4648_ctx& ctx, In begin, In end, Out first);
template <rfc4648_kind Kind = rfc4648_kind::base64, typename R, typename Out>
rfc4648_decode_result<In, Out> rfc4648_decode(rfc4648_ctx& ctx, R&& r, Out first);
template <rfc4648_kind Kind = rfc4648_kind::base64, typename Out>
Out rfc4648_decode(rfc4648_ctx& ctx, Out first);
```

`R` must model `std::contiguous_range` , `In` must satisfy *ContinuousIterator* and `Out` must satisfy *OutputIterator*.

Let `n - 1` is the length of the output as specified by RFC 4648.

If [`begin`, `end`) is not a valid range, or [`first`, `first + n`) is not a valid range, or if [`begin`, `end`) and [`first`, `first + n`) overlap, or if `r` and [`first`, `first + n`) overlap, the behavior is undefined.

If the template parameter `Padding` is false then the padding character `=` is not written.

The decode functions will return immediately if there are invalid characters (including `=`) within the range [`begin`, `end`), then `rfc4648_decode_result<In, Out>::end` points to the first invalid character.

Throws any exceptions from incrementing `first`, no other exceptions will be thrown. After an exception is thrown, `ctx` will be in an unspecified state.

## Example

```cpp
#include "decode.hpp"
#include "encode.hpp"
#include <cassert>
#include <string>
#include <string_view>
int main()
{
    std::string_view src{"ABCDEFGHIJKLMN"};
    std::string encoded;
    encoded.resize((src.size() + 3) / 3 * 4);
    bizwen::rfc4648_encode(src.begin(), src.end(), encoded.begin());
    std::string decoded;
    decoded.resize(src.size());
    bizwen::rfc4648_decode(encoded.begin(), encoded.end(), decoded.begin());
    assert(encoded == decoded);

    std::string dest1;
    dest1.resize((src.size() * 3 + 3) / 3 * 4);
    bizwen::rfc4648_ctx ctx;
    auto it = bizwen::rfc4648_encode(ctx, src.begin(), src.end(), dest1.begin());
    it = bizwen::rfc4648_encode(ctx, src.begin(), src.end(), it);
    it = bizwen::rfc4648_encode(ctx, src.begin(), src.end(), it);
    bizwen::rfc4648_encode(ctx, it);

    std::string dest2;
    dest2.resize((src.size() + 3) / 3 * 4);
    bizwen::rfc4648_encode(src, dest2.begin());

    std::wstring dest3;
    dest3.resize((src.size() + 3) / 3 * 4);
    bizwen::rfc4648_encode((std::byte *)src.data(), (std::byte *)src.data() + src.size(), dest3.begin());
}
```
