/**
*	Basic Types definition
*	Nana C++ Library(http://www.nanapro.org)
*	Copyright(C) 2003-2018 Jinhao(cnjinhao@hotmail.com)
*
*	Distributed under the Boost Software License, Version 1.0.
*	(See accompanying file LICENSE_1_0.txt or copy at
*	http://www.boost.org/LICENSE_1_0.txt)
*
*	@file nana/basic_types.hpp
*/

#ifndef NANA_BASIC_TYPES_HPP
#define NANA_BASIC_TYPES_HPP

#include <nana/deploy.hpp>
#include <cctype>
#include <nana/push_ignore_diagnostic>
#include <algorithm>
#include <vector>

namespace nana
{
	/// A constant value for the invalid position.
	const std::size_t npos = static_cast<std::size_t>(-1);


	template<typename CharT>
	struct casei_char_traits
		: public std::char_traits<CharT>
	{
		typedef CharT char_type;


		//static constexpr bool eq(char_type c1, char_type c2) noexcept
		//VC2012 does not support constexpr and noexcept keywords
		static bool eq(char_type c1, char_type c2)
		{
			return std::toupper(c1) == std::toupper(c2);
		}

		//static constexpr bool lt(char_type c1, char_type c2) noexcept
		//VC2012 does not support constexpr and noexcept keywords
		static bool lt(char_type c1, char_type c2)
		{
			return std::toupper(c1) < std::toupper(c2);
		}

		static int compare(const char_type* s1, const char_type* s2, std::size_t n)
		{
			while (n--)
			{
				char_type c1 = std::toupper(*s1);
				char_type c2 = std::toupper(*s2);
				if (c1 < c2) return -1;
				if (c1 > c2) return 1;
				++s1;
				++s2;
			}
			return 0;
		}

		static const char_type* find(const char_type* s, std::size_t n, const char_type& a)
		{
			char_type ua = std::toupper(a);
			const char_type * end = s + n;
			while ((s != end) && (std::toupper(*s) != ua))
				++s;
			return (s == end ? nullptr : s);
		}
	};

	using cistring = std::basic_string<char, casei_char_traits<char>>;
	using ciwstring = std::basic_string<wchar_t, casei_char_traits<wchar_t>>;


	namespace detail
	{
		struct drawable_impl_type;	//declearation, defined in platform_spec.hpp
	}

	namespace paint
	{
		typedef nana::detail::drawable_impl_type*	drawable_type;
	}

	enum class mouse_action
	{
		begin, normal = begin, normal_captured, hovered, pressed, end
	};

	enum class element_state
	{
		normal,
		hovered,
		focus_normal,
		focus_hovered,
		pressed,
		disabled
	};

	union pixel_argb_t
	{
		struct element_tag
		{
			unsigned char blue;
			unsigned char green;
			unsigned char red;
			unsigned char alpha_channel;
		}element;
		unsigned value;
	};

	union pixel_rgba_t
	{
		struct element_tag
		{
			unsigned char alpha_channel;
			unsigned char blue;
			unsigned char green;
			unsigned char red;
		}element;
		unsigned value;
	};

	using pixel_color_t = pixel_argb_t;

	/// See extended CSS color keywords (4.3) in http://www.w3.org/TR/2011/REC-css3-color-20110607/
	enum class colors
	{
		alice_blue = 0xf0f8ff,
		antique_white = 0xfaebd7,
		aqua = 0xFFFF,
		aquamarine = 0x7fffd4,
		azure = 0xf0ffff,
		beige = 0xf5f5dc,
		bisque = 0xffe4ce,
		black = 0x0,
		blanched_almond = 0xffebcd,
		blue = 0x0000FF,
		blue_violet = 0x8a2be2,
		brown = 0xa52a2a,
		burly_wood = 0xdeb887,
		cadet_blue = 0x5f9ea0,
		chartreuse = 0x7fff00,
		chocolate = 0xd2691e,
		coral = 0xff7f50,
		cornflower_blue = 0x6495ed,
		cornsilk = 0xfff8dc,
		crimson = 0xdc143c,
		cyan = 0xffff,
		dark_blue = 0x8b,
		dark_cyan = 0x8b8b,
		dark_goldenrod = 0xb8860b,
		dark_gray = 0xa9a9a9,
		dark_green = 0x6400,
		dark_grey = dark_gray,
		dark_khaki = 0xbdb76b,
		dark_magenta = 0x8b008b,
		dark_olive_green = 0x556b2f,
		dark_orange = 0xff8c00,
		dark_orchid = 0x9932cc,
		dark_red = 0x8b0000,
		dark_salmon = 0xe9976a,
		dark_sea_green = 0x8fbc8f,
		dark_slate_blue = 0x483d8b,
		dark_slate_gray = 0x2f4f4f,
		dark_slate_grey = 0x2f4f4f,
		dark_turquoise = 0xced1,
		dark_violet = 0x9400d3,
		deep_pink = 0xff1493,
		deep_sky_blue = 0xbfff,
		dim_gray = 0x696969,
		dim_grey = dim_gray,
		dodger_blue = 0x1e90ff,
		firebrick = 0xb22222,
		floral_white = 0xfffaf0,
		forest_green = 0x228b22,
		fuchsia = 0xFF00FF,
		gainsboro = 0xdcdcdc,
		ghost_white = 0xf8f8ff,
		gold = 0xffd700,
		goldenrod = 0xdaa520,
		gray = 0x808080,
		green = 0x008000,
		green_yellow = 0xadff2f,
		grey = gray,
		honeydew = 0xf0fff0,
		hot_pink = 0xff69b4,
		indian_red = 0xcd5c5c,
		indigo = 0x4b0082,
		ivory = 0xfffff0,
		khaki = 0xf0e68c,
		lavendar = 0xe6e6fa,
		lavender_blush = 0xfff0f5,
		lawn_green = 0x7cfc00,
		lemon_chiffon = 0xfffacd,
		light_blue = 0xadd8e6,
		light_coral = 0xf08080,
		light_cyan = 0xe0ffff,
		light_goldenrod_yellow = 0xfafad2,
		light_gray = 0xd3d3d3,
		light_green = 0x90ee90,
		light_grey = light_gray,
		light_pink = 0xffb6c1,
		light_salmon = 0xffa07a,
		light_sea_green = 0x20b2aa,
		light_sky_blue = 0x87cefa,
		light_slate_gray = 0x778899,
		light_slate_grey = light_slate_gray,
		light_steel_blue = 0xb0c4de,
		light_yellow = 0xffffe0,
		lime = 0x00FF00,
		lime_green = 0x32cd32,
		linen = 0xfaf0e6,
		magenta = 0xff00ff,
		maroon = 0x800000,
		medium_aquamarine = 0x66cdaa,
		medium_blue = 0xcd,
		medium_orchid = 0xba55d3,
		medium_purple = 0x9370db,
		medium_sea_green = 0x3cb371,
		medium_slate_blue = 0x7b68ee,
		medium_spring_green = 0xfa9a,
		medium_turquoise = 0x48d1cc,
		medium_violet_red = 0xc71585,
		midnight_blue = 0x191970,
		mint_cream = 0xf5fffa,

		misty_rose = 0xffe4e1,
		moccasin = 0xffe4b5,
		navajo_white = 0xffdead,
		navy = 0x000080,
		old_lace = 0xfdf5e6,
		olive = 0x808000,
		olive_drab = 0x6b8e23,
		orange = 0xffa500,
		orange_red = 0xff4500,
		orchid = 0xda70d6,
		pale_goldenrod = 0xeee8aa,
		pale_green = 0x98fb98,
		pale_turquoise = 0xafeeee,
		pale_violet_red = 0xdb7093,
		papaya_whip = 0xffefd5,
		peach_puff = 0xffdab9,
		peru = 0xcd853f,
		pink = 0xffc0cb,
		plum = 0xdda0dd,
		powder_blue = 0xb0e0e6,
		purple = 0x800080,
		red = 0xFF0000,
		rosy_brown = 0xbc8f8f,
		royal_blue = 0x4169e1,
		saddle_brown = 0x8b4513,
		salmon = 0xfa8072,
		sandy_brown = 0xf4a460,
		sea_green = 0x2e8b57,
		sea_shell = 0xfff5ee,
		sienna = 0xa0522d,
		silver = 0xc0c0c0,
		sky_blue = 0x87ceeb,
		slate_blue = 0x6a5acd,
		slate_gray = 0x708090,
		slate_grey = 0x708090,
		snow = 0xfffafa,
		spring_green = 0xff7f,
		steel_blue = 0x4682b4,
		tan = 0xd2b48c,
		teal = 0x008080,
		thistle = 0xd8bfd8,
		tomato = 0xff6347,
		turquoise = 0x40e0d0,
		violet = 0xee82ee,
		wheat = 0xf5deb3,
		white = 0xFFFFFF,
		white_smoke = 0xf5f5f5,
		yellow = 0xFFFF00,
		yellow_green = 0x9acd32,

		//temporary defintions, these will be replaced by color schema
		button_face_shadow_start = 0xF5F4F2,
		button_face_shadow_end = 0xD5D2CA,
		button_face = 0xD4D0C8, //,light_cyan
		dark_border = 0x404040,
		gray_border = 0x808080,
		highlight = 0x1CC4F7
	};

	//Some helper types to identify an integer as color.
	enum class color_rgb : unsigned {};
	enum class color_argb : unsigned {};
	enum class color_rgba : unsigned {};

	static std::string read_number(std::string& str, std::size_t& pos)
	{
		pos = str.find_first_of("0123456789", pos);
		if (pos == str.npos)
			return{};

		auto end = str.find_first_not_of("0123456789", pos + 1);
		//integer part
		if (end == str.npos)
		{
			pos = end;
			return str.substr(pos);
		}

		if (str[end] == '.')
		{
			auto decimal_end = str.find_first_not_of("0123456789", end + 1);
			if ((decimal_end == str.npos) || (decimal_end == end + 1)) //Because of missing %
				return{};

			end = decimal_end;
		}

		auto ch = str[end];
		if (ch == '%' || ch == ' ' || ch == ',' || ch == ')')
		{
			auto start = pos;
			pos = end + (ch == '%' ? 1 : 0);
			return str.substr(start, pos - start);
		}
		return{};
	}

	static double rgb_from_hue(double v1, double v2, double h)
	{
		if (h < 0.0)
			h += 1.0;
		else if (h > 1.0)
			h -= 1.0;

		if (h < 0.1666666) return v1 + (v2 - v1) * (6.0 * h);
		if (h < 0.5) return v2;
		if (h < 0.6666666) return v1 + (v2 - v1) * (4.0 - h * 6.0);
		return v1;
	}

	class color
	{
	public:
		color() = default;
		color(colors clr)
			: color((static_cast<unsigned>(clr) & 0xFF0000) >> 16, (static_cast<unsigned>(clr) & 0xFF00) >> 8, static_cast<unsigned>(clr) & 0xFF)
		{}
		color(colors clr, double alpha)
			: color((static_cast<unsigned>(clr) & 0xFF0000) >> 16, (static_cast<unsigned>(clr) & 0xFF00) >> 8, static_cast<unsigned>(clr) & 0xFF, alpha)
		{}
		color(color_rgb rgb)
			: r_((static_cast<int>(rgb) >> 16) & 0xFF),
			g_((static_cast<int>(rgb) >> 8) & 0xFF),
			b_(static_cast<int>(rgb) & 0xFF),
			a_(1.0)
		{}
		color(color_argb argb)
			: r_((static_cast<int>(argb) >> 16) & 0xFF),
			g_((static_cast<int>(argb) >> 8) & 0xFF),
			b_(static_cast<int>(argb) & 0xFF),
			a_(((static_cast<int>(argb) >> 24) & 0xFF) / 255.0)
		{}
		color::color(color_rgba rgba)
			: r_((static_cast<int>(rgba) >> 24) & 0xFF),
			g_((static_cast<int>(rgba) >> 16) & 0xFF),
			b_((static_cast<int>(rgba) >> 8) & 0xFF),
			a_((static_cast<int>(rgba) & 0xFF) / 255.0)
		{}
		color(unsigned red, unsigned green, unsigned blue, double alpha = 1.0)
			: r_(red), g_(green), b_(blue), a_(alpha)
		{
			if (alpha < 0.0)
				a_ = 0.0;
			else if (alpha > 1.0)
				a_ = 1.0;
		}

		/// Initializes the color with a CSS-like rgb string.
		//contributor: BigDave(mortis2007 at hotmail co uk)
		//date: February 3, 2015
		//maintainor: Jinhao, extended the support of CSS-spec
		explicit color(std::string css_color)
			: a_(1.0) {
			const char * excpt_what = "color: invalid rgb format";

			auto pos = css_color.find_first_not_of(' ');
			if (pos == css_color.npos)
				throw std::invalid_argument(excpt_what);

			if ('#' == css_color[pos])
			{
				if (css_color.size() < pos + 4)
					throw std::invalid_argument(excpt_what);

				auto endpos = css_color.find_first_not_of("0123456789abcdefABCDEF", pos + 1);
				if (endpos == css_color.npos)
					endpos = static_cast<decltype(endpos)>(css_color.size());

				if ((endpos - pos != 4) && (endpos - pos != 7))
					throw std::invalid_argument(excpt_what);

				auto n = std::stoi(css_color.substr(pos + 1, endpos - pos - 1), nullptr, 16);

				if (endpos - pos == 4)
				{
					r_ = ((0xF00 & n) >> 4) | ((0xF00 & n) >> 8);
					g_ = (0xF0 & n) | ((0xF0 & n) >> 4);
					b_ = (0xF & n) | ((0xF & n) << 4);
				}
				else
				{
					r_ = (0xFF0000 & n) >> 16;
					g_ = (0xFF00 & n) >> 8;
					b_ = (0xFF & n);
				}

				return;
			}

			//std::tolower is not allowed because of concept requirements
			std::transform(css_color.begin(), css_color.end(), css_color.begin(), [](char ch) {
				if ('A' <= ch && ch <= 'Z')
					return static_cast<char>(ch - ('A' - 'a'));
				return ch;
			});
			auto endpos = css_color.find(' ', pos + 1);
			if (endpos == css_color.npos)
				endpos = css_color.size();

			if ((endpos - pos == 11) && (css_color.substr(pos, 11) == "transparent"))
			{
				r_ = 0;
				g_ = 0;
				b_ = 0;
				a_ = 0;
				return;
			}

			auto type_end = css_color.find_first_of(" (", pos + 1);

			if (type_end == css_color.npos || ((type_end - pos != 3) && (type_end - pos != 4)))	//rgb/hsl = 3, rgba/hsla = 4
				throw std::invalid_argument(excpt_what);

			bool has_alpha = false;
			if (type_end - pos == 4) //maybe rgba/hsla
			{
				if (css_color[pos + 3] != 'a')
					throw std::invalid_argument(excpt_what);
				has_alpha = true;
			}

#if defined(USE_STD_REGEX)
			std::regex pat;
			std::regex_iterator<std::string::iterator> i, end;
			auto type_name = css_color.substr(pos, 3);
			if ("rgb" == type_name)
			{
				pat.assign("(\\d*\\.)?\\d+\\%?");
				i = std::regex_iterator<std::string::iterator>(css_color.begin() + pos, css_color.end(), pat);

				if (i == end)
					throw std::invalid_argument(excpt_what);

				std::vector<std::string> rgb;
#ifdef _nana_std_has_emplace_return_type
				auto const is_real = (rgb.emplace_back(i->str()).back() == '%');
#else
				rgb.emplace_back(i->str());
				const bool is_real = (rgb.back().back() == '%');
#endif
				pat.assign(is_real ? "(\\d*\\.)?\\d+\\%" : "\\d+");

				for (++i; i != end; ++i)
				{
					rgb.emplace_back(i->str());
					if (rgb.size() == 3)
						break;
				}

				if (rgb.size() != 3)
					throw std::invalid_argument(excpt_what);

				if (is_real)
				{
					auto pr = ::nana::stod(rgb[0].substr(0, rgb[0].size() - 1));
					r_ = (pr > 100 ? 255.0 : 2.55 * pr);

					pr = ::nana::stod(rgb[1].substr(0, rgb[1].size() - 1));
					g_ = (pr > 100 ? 255.0 : 2.55 * pr);

					pr = ::nana::stod(rgb[2].substr(0, rgb[2].size() - 1));
					b_ = (pr > 100 ? 255.0 : 2.55 * pr);
				}
				else
				{
					r_ = ::nana::stod(rgb[0]);
					if (r_ > 255.0)	r_ = 255;

					g_ = ::nana::stod(rgb[1]);
					if (g_ > 255.0)	g_ = 255;

					b_ = ::nana::stod(rgb[2]);
					if (b_ > 255.0)	b_ = 255;
				}
			}
			else if ("hsl" == type_name)
			{
				pat.assign("(\\d*\\.)?\\d+");
				i = std::regex_iterator<std::string::iterator>(css_color.begin() + pos, css_color.end(), pat);

				if (i == end)
					throw std::invalid_argument(excpt_what);

				auto h = ::nana::stod(i->str());

				pat.assign("(\\d*\\.)?\\d+\\%");

				if (++i == end)
					throw std::invalid_argument(excpt_what);

				auto str = i->str();
				auto s = ::nana::stod(str.substr(0, str.size() - 1));

				if (++i == end)
					throw std::invalid_argument(excpt_what);

				str = i->str();
				auto l = ::nana::stod(str.substr(0, str.size() - 1));

				from_hsl(h, s / 100, l / 100);
			}
			else
				throw std::invalid_argument(excpt_what);	//invalid color type

			if (has_alpha)
			{
				pat.assign("(\\d*\\.)?\\d+");
				if (++i == end)
					throw std::invalid_argument(excpt_what);	//invalid alpha value
				a_ = ::nana::stod(i->str());
			}
#else
			auto type_name = css_color.substr(pos, 3);
			pos = css_color.find_first_not_of(' ', type_end);
			if (pos == css_color.npos || css_color[pos] != '(')
				throw std::invalid_argument(excpt_what);

			auto str = read_number(css_color, ++pos);
			if (str.empty())
				throw std::invalid_argument(excpt_what);

			if ("rgb" == type_name)
			{
				std::vector<std::string> rgb;

#ifdef _nana_std_has_emplace_return_type
				auto const is_real = (rgb.emplace_back(std::move(str)).back() == '%');
#else
				rgb.emplace_back(std::move(str));

				const bool is_real = (rgb.back().back() == '%');
#endif

				for (int i = 0; i < 2; ++i)
				{
					pos = css_color.find_first_not_of(' ', pos);
					if (pos == css_color.npos || css_color[pos] != ',')
						throw std::invalid_argument(excpt_what);

					str = read_number(css_color, ++pos);
					if (str.empty())
						throw std::invalid_argument(excpt_what);

					rgb.emplace_back(std::move(str));
					if (rgb.size() == 3)
						break;
				}

				if (rgb.size() != 3)
					throw std::invalid_argument(excpt_what);

				if (is_real)
				{
					auto pr = std::stod(rgb[0].substr(0, rgb[0].size() - 1));
					r_ = (pr > 100 ? 255.0 : 2.55 * pr);

					pr = std::stod(rgb[1].substr(0, rgb[1].size() - 1));
					g_ = (pr > 100 ? 255.0 : 2.55 * pr);

					pr = std::stod(rgb[2].substr(0, rgb[2].size() - 1));
					b_ = (pr > 100 ? 255.0 : 2.55 * pr);
				}
				else
				{
					r_ = std::stod(rgb[0]);
					if (r_ > 255.0)	r_ = 255;

					g_ = std::stod(rgb[1]);
					if (g_ > 255.0)	g_ = 255;

					b_ = std::stod(rgb[2]);
					if (b_ > 255.0)	b_ = 255;
				}
			}
			else if ("hsl" == type_name)
			{
				if (str.back() == '%')
					throw std::invalid_argument(excpt_what);

				auto h = std::stod(str);

				pos = css_color.find_first_not_of(' ', pos);
				if (pos == css_color.npos || css_color[pos] != ',')
					throw std::invalid_argument(excpt_what);

				str = read_number(css_color, ++pos);
				if (str.empty() || str.back() != '%')
					throw std::invalid_argument(excpt_what);

				auto s = std::stod(str.substr(0, str.size() - 1));

				pos = css_color.find_first_not_of(' ', pos);
				if (pos == css_color.npos || css_color[pos] != ',')
					throw std::invalid_argument(excpt_what);

				str = read_number(css_color, ++pos);
				if (str.empty() || str.back() != '%')
					throw std::invalid_argument(excpt_what);

				auto l = std::stod(str.substr(0, str.size() - 1));

				from_hsl(h, s / 100, l / 100);
			}
			else
				throw std::invalid_argument(excpt_what);	//invalid color type

			if (has_alpha)
			{
				str = read_number(css_color, ++pos);
				if (str.empty() || str.back() == '%')
					throw std::invalid_argument(excpt_what); //invalid alpha value

				a_ = std::stod(str);
			}
#endif
		}

		color& alpha(double al)	///< Sets alpha channel
		{
			if (al < 0.0)
				a_ = 0.0;
			else if (al > 1.0)
				a_ = 1.0;
			else
				a_ = al;
			return *this;
		}
		color& from_rgb(unsigned red, unsigned green, unsigned blue) 		///< immutable alpha channel
		{
			r_ = red;
			g_ = green;
			b_ = blue;
			return *this;
		}
																			/// Sets color with a HSL value.
																			/// @param hue in range of [0, 360]
																			/// @param saturation in range of [0, 1]
																			/// @param lightness  in range of [0, 1]
		color& from_hsl(double hue, double saturation, double lightness) 	///< immutable alpha channel
		{
			if (0.0 == saturation)
			{
				r_ = lightness * 255.0;
				g_ = r_;
				b_ = r_;
			}
			else
			{
				double var2;
				if (lightness < 0.5)
					var2 = lightness * (1.0 + saturation);
				else
					var2 = (lightness + saturation) - (saturation * lightness);

				double var1 = 2.0 * lightness - var2;

				hue /= 360;
				r_ = 255.0 * rgb_from_hue(var1, var2, hue + 0.33333);
				g_ = 255.0 * rgb_from_hue(var1, var2, hue);
				b_ = 255.0 * rgb_from_hue(var1, var2, hue - 0.33333);
			}
			return *this;
		}
																			/// Blends color
																			/**
																			* Returns a color which is blended as this * (1 - fade_rate) + blending_color * fade_rate
																			* @param blending_color Color to blend
																			* @param fade_rate Blending rate for blending_color
																			* @return a blended color
																			*/
		color blend(const color& bgcolor, double alpha) const
		{
			color result;
			result.r_ = r_ * (1.0 - alpha) + bgcolor.r_ * alpha;
			result.g_ = g_ * (1.0 - alpha) + bgcolor.g_ * alpha;
			result.b_ = b_ * (1.0 - alpha) + bgcolor.b_ * alpha;
			result.a_ = 1.0;
			return result;
		}
		/// Determines whether the color is completely transparent.
		bool invisible() const
		{
			return (a_ == 0.0);
		}
		pixel_color_t px_color() const
		{
			return argb();
		}
		pixel_argb_t argb() const
		{
			pixel_argb_t argb;
			argb.element.red = static_cast<unsigned>(r_);
			argb.element.green = static_cast<unsigned>(g_);
			argb.element.blue = static_cast<unsigned>(b_);
			argb.element.alpha_channel = static_cast<unsigned>(a_ * 255);
			return argb;
		}
		pixel_rgba_t rgba() const
		{
			pixel_rgba_t rgba;
			rgba.element.red = static_cast<unsigned>(r_);
			rgba.element.green = static_cast<unsigned>(g_);
			rgba.element.blue = static_cast<unsigned>(b_);
			rgba.element.alpha_channel = static_cast<unsigned>(a_ * 255);
			return rgba;
		}

		const double& r() const
		{
			return r_;
		}
		const double& g() const
		{
			return g_;
		}
		const double& b() const
		{
			return b_;
		}
		const double& a() const
		{
			return a_;
		}

		bool operator==(const color& other) const
		{
			return (px_color().value == other.px_color().value);
		}
		bool operator!=(const color& other) const
		{
			return (px_color().value != other.px_color().value);
		}

	public:
		double r_{ 0.0 };
		double g_{ 0.0 };
		double b_{ 0.0 };
		double a_{ 0.0 };	//invisible
	};

	static inline color operator+(const color& x, const color& y)
	{
		double a = x.a_ + y.a_;
		auto r = static_cast<unsigned>(x.r_ + y.r_);
		auto g = static_cast<unsigned>(x.g_ + y.g_);
		auto b = static_cast<unsigned>(x.b_ + y.b_);

		return color{
			r > 255 ? 255 : r,
			g > 255 ? 255 : g,
			b > 255 ? 255 : b,
			a > 1.0 ? 1.0 : a };
	}

	template<typename T>
	struct basic_point
	{
		//typedef-names
		using value_type = T;

		//data member
		value_type x{};
		value_type y{};

		//member functions
		basic_point() = default;

		basic_point(value_type x, value_type y)
			: x{ x }, y{ y }
		{}

		bool operator==(const basic_point& other) const noexcept
		{
			return (x == other.x && y == other.y);
		}

		bool operator!=(const basic_point& other) const noexcept
		{
			return (x != other.x || y != other.y);
		}

		bool operator<(const basic_point& other) const noexcept
		{
			return ((y < other.y) || (y == other.y && x < other.x));
		}

		bool operator<=(const basic_point& other) const noexcept
		{
			return ((y < other.y) || (y == other.y && x <= other.x));
		}

		bool operator>(const basic_point& other) const noexcept
		{
			return ((y > other.y) || (y == other.y && x > other.x));
		}

		bool operator>=(const basic_point& other) const noexcept
		{
			return ((y > other.y) || (y == other.y && x >= other.x));
		}

		basic_point operator-(const basic_point& other) const noexcept
		{
			return{ x - other.x, y - other.y };
		}

		basic_point operator+(const basic_point& other) const noexcept
		{
			return{ x + other.x, y + other.y };
		}

		basic_point& operator-=(const basic_point& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		basic_point& operator+=(const basic_point& other) noexcept
		{
			x += other.x;
			y += other.y;
			return *this;
		}
	};

	using point = basic_point<int>;
	using upoint = basic_point<unsigned>;

	struct size
	{
		//using value_type = unsigned;
		typedef unsigned value_type;
		size() : width(0), height(0) {}
		size(value_type width, value_type height) : width(width), height(height) {}

		bool empty() const		///< true if width * height == 0
		{
			return (0 == width || 0 == height);
		}
		bool is_hit(const point& pos) const 	///< Assume it is a rectangle at (0,0), and check whether a specified position is in the rectange.
		{
			return (0 <= pos.x && pos.x < static_cast<int>(width) && 0 <= pos.y && pos.y < static_cast<int>(height));
		}
		size& shift()
		{
			std::swap(width, height);
			return *this;
		}

		bool operator==(const size& rhs) const
		{
			return (width == rhs.width) && (height == rhs.height);
		}
		bool operator!=(const size& rhs) const
		{
			return (width != rhs.width) || (height != rhs.height);
		}
		size operator+(const size& sz) const
		{
			return{ width + sz.width, height + sz.height };
		}

		value_type width;
		value_type height;
	};

	struct rectangle
	{
		rectangle()										 ///< a zero-size rectangle at (0, 0).
			:x(0), y(0), width(0), height(0)
		{}
		rectangle(int x, int y, unsigned width, unsigned height)
			:x(x), y(y), width(width), height(height)
		{}
		explicit rectangle(const size & sz) 					///< a rectangle with specified size at coordinate (0, 0).
		: x(0), y(0), width(sz.width), height(sz.height)
		{}
		explicit rectangle(const point& pos, const size& sz = size())
			: x(pos.x), y(pos.y), width(sz.width), height(sz.height)
		{}

		bool operator==(const rectangle& rhs) const
		{
			return (width == rhs.width) && (height == rhs.height) && (x == rhs.x) && (y == rhs.y);
		}
		bool operator!=(const rectangle& rhs) const
		{
			return (width != rhs.width) || (height != rhs.height) || (x != rhs.x) || (y != rhs.y);
		}

		point position() const noexcept
		{
			return{ x, y };
		}
		rectangle& position(const point& p) noexcept
		{
			x = p.x;
			y = p.y;
			return *this;
		}

		size dimension() const noexcept
		{
			return{ width, height };
		}
		rectangle& dimension(const size& sz) noexcept
		{
			width = sz.width;
			height = sz.height;
			return *this;
		}

		/// Pares the specified pixels off the rectangle.
		/**
		* It's equal to x += pixels; y + pixels; width -= (pixels << 1); height -= (pixels << 1);
		* @param pixels The number of pixels to be pared. If the number that multiples pixels twice is larger than width/height, the width/height will be zero. If the pixels is a negative number, the width/height is add the number that multiple pixels twice.
		* @return The reference of *this.
		*/
		rectangle& pare_off(int pixels)
		{
			x += pixels;
			y += pixels;
			auto const px_twice = (pixels << 1);
			if (px_twice > static_cast<int>(width))
				width = 0;
			else
				width -= px_twice;

			if (px_twice > static_cast<int>(height))
				height = 0;
			else
				height -= px_twice;

			return *this;
		}

		int right() const noexcept
		{
			return x + static_cast<int>(width);
		}
		int bottom() const noexcept
		{
			return y + static_cast<int>(height);
		}
		bool is_hit(int pos_x, int pos_y) const
		{
			return	(x <= pos_x && pos_x < x + static_cast<int>(width)) &&
				(y <= pos_y && pos_y < y + static_cast<int>(height));
		}
		bool is_hit(const point& pos) const
		{
			return	(x <= pos.x && pos.x < x + static_cast<int>(width)) &&
				(y <= pos.y && pos.y < y + static_cast<int>(height));
		}
		bool empty() const		///< true if width * height == 0.
		{
			return (0 == width) || (0 == height);
		}
		rectangle& shift() 	///< Swap position x and y, size width and height.
		{
			std::swap(x, y);
			std::swap(width, height);
			return *this;
		}

		int x;
		int y;
		unsigned width;
		unsigned height;
	};

	class rectangle_rotator
	{
	public:
		rectangle_rotator(bool rotated, const ::nana::rectangle& area)
			: rotated_(rotated),
			area_(area)
		{}

		int x() const
		{
			return (rotated_ ? area_.y : area_.x);
		}
		int & x_ref()
		{
			return (rotated_ ? area_.y : area_.x);
		}
		int y() const
		{
			return (rotated_ ? area_.x : area_.y);
		}
		int & y_ref()
		{
			return (rotated_ ? area_.x : area_.y);
		}
		unsigned w() const
		{
			return (rotated_ ? area_.height : area_.width);
		}
		unsigned & w_ref()
		{
			return (rotated_ ? area_.height : area_.width);
		}
		unsigned h() const
		{
			return (rotated_ ? area_.width : area_.height);
		}
		unsigned & h_ref()
		{
			return (rotated_ ? area_.width : area_.height);
		}

		int right() const
		{
			return (rotated_ ? area_.y + static_cast<int>(area_.height) : area_.x + static_cast<int>(area_.width));
		}
		int bottom() const
		{
			return (rotated_ ? area_.x + static_cast<int>(area_.width) : area_.y + static_cast<int>(area_.height));
		}
		const ::nana::rectangle& result() const
		{
			return area_;
		}
	private:
		bool rotated_;
		::nana::rectangle area_;
	};//end class rectangle_rotator

	enum class arrange
	{
		unknown, horizontal, vertical, horizontal_vertical
	};

	///The definition of horizontal alignment
	enum class align
	{
		left, center, right
	};

	///The definition of vertical alignment
	enum class align_v
	{
		top, center, bottom
	};

	///The definition of the four corners of the world
	enum class direction
	{
		north,
		south,
		east,
		west,
		southeast
	};
}//end namespace nana
#include <nana/pop_ignore_diagnostic>
#endif


