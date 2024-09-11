#include <string>
#include "rfc4648.hpp"

int main()
{
	std::string src{ "ABCDEFGHIJKLMN" };
	std::string dest;
	dest.resize((src.size() + 3) / 3 * 4);
	bizwen::rfc4648_copy(src.begin(), src.end(), dest.begin());

	std::string dest1;
	dest1.resize((src.size() * 3 + 3) / 3 * 4);
	bizwen::rfc4648_ctx ctx;
	auto it = bizwen::rfc4648_copy(ctx, src.begin(), src.end(), dest1.begin());
	it = rfc4648_copy(ctx, src.begin(), src.end(), it);
	it = rfc4648_copy(ctx, src.begin(), src.end(), it);
	rfc4648_copy(ctx, it);

	std::string dest2;
	dest2.resize((src.size() + 3) / 3 * 4);
	bizwen::rfc4648_copy(src, dest2.begin());

	return 0;
}