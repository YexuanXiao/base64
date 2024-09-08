#include <string>
#include <base64.hpp>
int main()
{
	std::string src{ "ABCDEFGHIJKLMN" };
	std::string dest;
	dest.resize((src.size() + 3) / 3 * 4);
	rfc4648_copy(src.begin(), src.end(), dest.begin());
}