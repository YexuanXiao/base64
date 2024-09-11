#include <iterator>
#include <type_traits>
#include <concepts>
#include <memory>


namespace bizwen
{

	template <typename CharType>
	struct rfc4648_traits;
	template <>
	struct rfc4648_traits<char>
	{
		static inline constexpr auto base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
		static inline constexpr auto base64_url = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
		static inline constexpr auto base32 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		static inline constexpr auto base32_lower = "abcdefghijklmnopqrstuvwxyz234567";
		static inline constexpr auto base32_hex = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
		static inline constexpr auto base32_hex_lower = "0123456789abcdefghijklmnopqrstuv";
		static inline constexpr auto base32_crockford = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
		static inline constexpr auto base32_crockford_lower = "0123456789abcdefghjkmnpqrstvwxyz";
		static inline constexpr auto base16 = base32_hex;
		static inline constexpr auto base16_lower = base32_hex_lower;
	};
	template <>
	struct rfc4648_traits<wchar_t>
	{
		static inline constexpr auto base64 = L"=ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
		static inline constexpr auto base64_url = L"=ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
		static inline constexpr auto base32 = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		static inline constexpr auto base32_lower = L"abcdefghijklmnopqrstuvwxyz234567";
		static inline constexpr auto base32_hex = L"0123456789ABCDEFGHIJKLMNOPQRSTUV";
		static inline constexpr auto base32_hex_lower = L"0123456789abcdefghijklmnopqrstuv";
		static inline constexpr auto base32_crockford = L"0123456789ABCDEFGHJKMNPQRSTVWXYZ";
		static inline constexpr auto base32_crockford_lower = L"0123456789abcdefghjkmnpqrstvwxyz";
		static inline constexpr auto base16 = base32_hex;
		static inline constexpr auto base16_lower = base32_hex_lower;
	};
	template <>
	struct rfc4648_traits<char8_t>
	{
		static inline constexpr auto base64 = u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
		static inline constexpr auto base64_url = u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
		static inline constexpr auto base32 = u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		static inline constexpr auto base32_lower = u8"abcdefghijklmnopqrstuvwxyz234567";
		static inline constexpr auto base32_hex = u8"0123456789ABCDEFGHIJKLMNOPQRSTUV";
		static inline constexpr auto base32_hex_lower = u8"0123456789abcdefghijklmnopqrstuv";
		static inline constexpr auto base32_crockford = u8"0123456789ABCDEFGHJKMNPQRSTVWXYZ";
		static inline constexpr auto base32_crockford_lower = u8"0123456789abcdefghjkmnpqrstvwxyz";
		static inline constexpr auto base16 = base32_hex;
		static inline constexpr auto base16_lower = base32_hex_lower;
	};
	template <>
	struct rfc4648_traits<char16_t>
	{
		static inline constexpr auto base64 = u"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
		static inline constexpr auto base64_url = u"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
		static inline constexpr auto base32 = u"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		static inline constexpr auto base32_lower = u"abcdefghijklmnopqrstuvwxyz234567";
		static inline constexpr auto base32_hex = u"0123456789ABCDEFGHIJKLMNOPQRSTUV";
		static inline constexpr auto base32_hex_lower = u"0123456789abcdefghijklmnopqrstuv";
		static inline constexpr auto base32_crockford = u"0123456789ABCDEFGHJKMNPQRSTVWXYZ";
		static inline constexpr auto base32_crockford_lower = u"0123456789abcdefghjkmnpqrstvwxyz";
		static inline constexpr auto base16 = base32_hex;
		static inline constexpr auto base16_lower = base32_hex_lower;
	};
	template <>
	struct rfc4648_traits<char32_t>
	{
		static inline constexpr auto base64 = U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
		static inline constexpr auto base64_url = U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
		static inline constexpr auto base32 = U"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		static inline constexpr auto base32_lower = U"abcdefghijklmnopqrstuvwxyz234567";
		static inline constexpr auto base32_hex = U"0123456789ABCDEFGHIJKLMNOPQRSTUV";
		static inline constexpr auto base32_hex_lower = U"0123456789abcdefghijklmnopqrstuv";
		static inline constexpr auto base32_crockford = U"0123456789ABCDEFGHJKMNPQRSTVWXYZ";
		static inline constexpr auto base32_crockford_lower = U"0123456789abcdefghjkmnpqrstvwxyz";
		static inline constexpr auto base16 = base32_hex;
		static inline constexpr auto base16_lower = base32_hex_lower;
	};

	struct rfc4648_ctx
	{
		unsigned char effective{}; // 0、1、2
		unsigned char buf0{};
		unsigned char buf1{};
	};

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
		template <typename T>
		inline constexpr unsigned char to_uc(T t) noexcept
		{
			// T is char, unsigned char or std::byte
			return static_cast<unsigned char>(t);
		}

		template <rfc4648_kind Kind, typename Traits>
		inline consteval auto get_alphabet() noexcept
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

		template <typename A, typename I, typename O>
		inline constexpr void copy_impl_b64_3(A alphabet, I begin, O& first)
		{
			auto a = *(begin++);
			auto b = a >> 2; // XXXXXX
			*(first++) = alphabet[b];
			auto c = (a << 4) & 0b110'000; // XX0000
			auto d = *(begin++);
			auto e = d >> 4; // XXXX
			auto f = c | e; // XXXXXX
			*(first++) = alphabet[f];
			auto g = (d << 2) & 0b111'100; // XXXX00
			auto h = *(begin++);
			auto i = h >> 6; // 0000XX
			auto j = i | g;
			*(first++) = alphabet[j];
			auto k = h & 0b00'111'111; // XXXXXX
			*(first++) = alphabet[k];
		}

		template <bool padding = true, typename A, typename I, typename O>
		inline constexpr void copy_impl_b64_2(A alphabet, I begin, O& first)
		{
			auto a = *(begin++);
			auto b = a >> 2; // XXXXXX
			*(first++) = alphabet[b];
			auto c = (a << 4) & 0b110'000; // XX0000
			auto d = *(begin);
			auto e = d >> 4; // XXXX
			auto f = c | e; // XXXXXX
			*(first++) = alphabet[f];
			auto g = (d << 2) & 0b111'100; // XXXX00
			*(first++) = alphabet[g];

			if constexpr (padding)
			{
				*first = alphabet[64];
			}
		}

		template <bool padding = true, typename A, typename I, typename O>
		inline constexpr void copy_impl_b64_1(A alphabet, I begin, O& first)
		{
			auto a = *(begin++);
			auto b = a >> 2; // XXXXXX
			*(first++) = alphabet[b];
			auto c = (a << 4) & 0b110'000; // XX0000
			*(first++) = alphabet[c];

			if constexpr (padding)
			{
				*(first++) = alphabet[64]; // pad1
				*first = alphabet[64]; // pad2
			}
		}

		template <bool padding = true, typename A, typename I, typename O>
		inline constexpr void copy_impl_b64(A alphabet, I begin, I end, O& first)
		{
			while (end - begin > 2)
			{
				copy_impl_b64_3(alphabet, begin, first);
				begin += 3;
			}

			if (end - begin == 2)
				copy_impl_b64_2<padding>(alphabet, begin, first);
			else if (end - begin != 0) // == 1
				copy_impl_b64_1<padding>(alphabet, begin, first);
		}

		template <typename A, typename I, typename O>
		inline constexpr void copy_impl_b64_ctx(A alphabet, rfc4648_ctx& ctx, I begin, I end, O& first)
		{
			// assume begin and end are pointers
			auto effective = ctx.effective; // 0, 1, 2

			if (effective == 2)
			{
				if (end - begin == 0) // < 1
					return;

				unsigned char buf[3];
				buf[0] = ctx.buf0;
				buf[1] = ctx.buf1;
				buf[2] = *(begin++);

				copy_impl_b64_3(alphabet, std::begin(buf), first);
			}
			else if (effective != 0) // == 1
			{
				if (end - begin == 1)
				{
					ctx.buf1 = *(begin++);
					ctx.effective = 2;

					return;
				}
				else if (end - begin == 0)
				{
					return;
				}

				// >= 2

				unsigned char buf[3];
				buf[0] = ctx.buf0;
				buf[1] = *(begin++);
				buf[2] = *(begin++);

				copy_impl_b64_3(alphabet, std::begin(buf), first);
			}

			// init or aligned
			while (end - begin > 2)
			{
				copy_impl_b64_3(alphabet, begin, first);
				begin += 3;
			}

			if (end - begin == 2)
			{
				ctx.buf0 = *(begin++);
				ctx.buf1 = *begin;
				ctx.effective = 2;
			}
			else if (end - begin != 0) // == 1
			{
				ctx.buf1 = *begin;
				ctx.effective = 1;
			}
			else // NB: clear effective
			{
				ctx.effective = 0;
			}
		}
	}

	template <rfc4648_kind Kind = rfc4648_kind::base64, bool padding = true, typename In, typename Out>
	inline constexpr Out rfc4648_copy(In begin, In end, Out first)
	{
		static_assert(std::contiguous_iterator<In>);
		using in_char = std::remove_cvref_t<decltype(*begin)>;
		static_assert(std::is_same_v<in_char, char> || std::is_same_v<in_char, unsigned char> || std::is_same_v<in_char, std::byte>);
		using out_char = std::remove_cvref_t<decltype(*first)>;
		using traits = rfc4648_traits<out_char>;
		auto begin_ptr = std::to_address(begin);
		auto end_ptr = std::to_address(end);

		detail::copy_impl_b64(detail::get_alphabet<rfc4648_kind::base64, traits>(), begin_ptr, end_ptr, first);

		return first;
	}

	template <rfc4648_kind Kind = rfc4648_kind::base64, typename In, typename Out>
	inline constexpr Out rfc4648_copy(rfc4648_ctx& ctx, In begin, In end, Out first)
	{
		static_assert(std::contiguous_iterator<In>);
		using in_char = std::remove_cvref_t<decltype(*begin)>;
		static_assert(std::is_same_v<in_char, char> || std::is_same_v<in_char, unsigned char> || std::is_same_v<in_char, std::byte>);
		using out_char = std::remove_cvref_t<decltype(*first)>;
		using traits = rfc4648_traits<out_char>;
		auto begin_ptr = std::to_address(begin);
		auto end_ptr = std::to_address(end);

		detail::copy_impl_b64_ctx(detail::get_alphabet<rfc4648_kind::base64, traits>(), ctx, begin_ptr, end_ptr, first);

		return first;
	}

	template <rfc4648_kind Kind = rfc4648_kind::base64, bool padding = true, typename Out>
	inline constexpr Out rfc4648_copy(rfc4648_ctx& ctx, Out first)
	{
		using out_char = std::remove_cvref_t<decltype(*first)>;
		using traits = rfc4648_traits<out_char>;

		auto effective = ctx.effective;

		if (effective == 2)
		{
			unsigned char buf[2];
			buf[0] = ctx.buf0;
			buf[1] = ctx.buf1;
			detail::copy_impl_b64_2<padding>(detail::get_alphabet<rfc4648_kind::base64, traits>(), std::begin(buf), first);
		}
		else if (effective != 0) // == 1
		{
			unsigned char buf[1];
			buf[0] = ctx.buf0;
			detail::copy_impl_b64_1<padding>(detail::get_alphabet<rfc4648_kind::base64, traits>(), std::begin(buf), first);
		}

		ctx.effective = 0;

		return first;
	}

}