# base64

A plain header-only [RFC 4648](https://www.rfc-editor.org/rfc/rfc4648) implementation, but with the most modern C++ API design.

All variants of RFC 4648 are supported and the Crockford variant is available.

Supports input from discontinuous multiple buffers.

Support padding for secure base64 url variant.

Support `constexpr` compile-time caculation.

C++20 required (`consteval` and concepts).

## Status

base64/url encode done.

## Interface

```cpp
template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename In, typename Out>
Out rfc4648_copy(In begin, In end, Out first);

template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename R, typename Out>
Out rfc4648_copy(R&& r, Out first);

template <rfc4648_kind Kind = rfc4648_kind::base64, typename In, typename Out>
Out rfc4648_copy(rfc4648_ctx& ctx, In begin, In end, Out first);

template <rfc4648_kind Kind = rfc4648_kind::base64, typename R, typename Out>
Out rfc4648_copy(rfc4648_ctx& ctx, R&& r, Out first);

template <rfc4648_kind Kind = rfc4648_kind::base64, bool Padding = true, typename Out>
Out rfc4648_copy(rfc4648_ctx& ctx, Out first);

```

Where `R` must satisfy ContinuousContainer and `In` and `Out` must satisfy ContinuousIterator.

If [`begin`, `end`) is not a valid range, or [`first`, `first + (end - begin)`) is not a valid range, or if [`begin`, `end`) and [`first`, `first + (end - begin)`) overlap, or if `r` and `first` overlap, the behavior is undefined.

Throws any exceptions from incrementing `Out`.

## Example

```cpp
#include <string>
#include "rfc4648.hpp"

int main()
{
    std::string src{ "ABCDEFGHIJKLMN" };

    // single input bffer
    std::string dest;
    dest.resize((src.size() + 3) / 3 * 4);
    bizwen::rfc4648_copy(src.begin(), src.end(), dest.begin());

    // discontinuous multiple buffers
    std::string dest1;
    dest1.resize((src.size() * 3 + 3) / 3 * 4);
    // init context
    bizwen::rfc4648_ctx ctx;
    // init copy
    auto it = bizwen::rfc4648_copy(ctx, src.begin(), src.end(), dest1.begin());
    it = rfc4648_copy(ctx, src.begin(), src.end(), it);
    it = rfc4648_copy(ctx, src.begin(), src.end(), it);
    // final
    it = rfc4648_copy(ctx, it);

    // ranges
    std::string dest2;
    dest2.resize((src.size() + 3) / 3 * 4);
    bizwen::rfc4648_copy(src, dest2.begin());

    return 0;
}
```
