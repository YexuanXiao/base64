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
