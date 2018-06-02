/*
 *	Platform Specification Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Nana Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://nanapro.org/LICENSE_1_0.txt)
 *
 *	@file: nana/detail/linux_X11/platform_spec.cpp
 *
 *	This file provides basis class and data structrue that required by nana
 *
 *	http://standards.freedesktop.org/clipboards-spec/clipboards-0.1.txt
 */

#include <nana/detail/platform_spec_selector.hpp>
#if defined(NANA_POSIX) && defined(NANA_X11)

#include <nana/push_ignore_diagnostic>

#include <X11/Xlocale.h>
#include <locale>
#include <map>
#include <set>
#include <algorithm>
#include <nana/paint/graphics.hpp>
#include <nana/gui/detail/bedrock.hpp>
#include <nana/gui/detail/basic_window.hpp>
#include <nana/gui/detail/window_manager.hpp>
#include <nana/system/platform.hpp>
#include <errno.h>
#include <sstream>

#include "x11/msg_dispatcher.hpp"

namespace nana
{
namespace detail
{
	typedef native_window_type native_window_type;
#if defined(NANA_USE_XFT)
	//class conf
		conf::conf(const char * file)
		{
			ifs_.open(file);
		}

		bool conf::open(const char* file)
		{
			ifs_.open(file);
			return static_cast<bool>(ifs_);
		}

		std::string conf::value(const char* key)
		{
			if((0 == key) || !ifs_) return "";
			size_t len = ::strlen(key);
			ifs_.seekg(0, std::ios::beg);
			ifs_.clear();
			std::string str;

			while(ifs_.good())
			{
				std::getline(ifs_, str);
				if(str.size() <= len + 1)
					continue;

				size_t kpos = str.find(key);
				if((kpos != str.npos) && ((kpos == 0) || (str.substr(0, kpos) == std::string(kpos, ' '))))
				{
					size_t aspos = str.find("=", kpos + len);
					if(aspos != str.npos)
					{
						if((aspos == kpos + len) || (str.substr(kpos + len, aspos) == std::string(aspos - kpos - len, ' ')))
						{
							std::string res = str.substr(aspos + 1);
							size_t beg = res.find_first_not_of(" ");
							if(beg && (beg != res.npos))
								res = res.substr(beg);
							beg = res.find("\"");
							if(beg == 0)
							{
								size_t end = res.find_last_of("\"");
								if(beg != end)
									return res.substr(beg + 1, (end == res.npos ? res.npos : (end - 1)));
							}
							return res;
						}
					}
				}
			}
			return std::string();
		}
	//end class conf

	//class charset_conv
		charset_conv::charset_conv(const char* tocode, const char* fromcode)
		{
			handle_ = ::iconv_open(tocode, fromcode);
		}

		charset_conv::~charset_conv()
		{
			::iconv_close(handle_);
		}

		std::string charset_conv::charset(const std::string& str) const
		{
			if(reinterpret_cast<iconv_t>(-1) == handle_)
				return std::string();

			char * inbuf = const_cast<char*>(str.c_str());
			std::size_t inleft = str.size();
			std::size_t outlen = (inleft * 4 + 4);
			char * strbuf = new char[outlen + 4];
			char * outbuf = strbuf;
			std::size_t outleft = outlen;
			::iconv(handle_, &inbuf, &inleft, &outbuf, &outleft);
			std::string rstr(strbuf, outbuf);
			delete [] strbuf;
			return rstr;
		}

		std::string charset_conv::charset(const char* buf, std::size_t len) const
		{
			if(reinterpret_cast<iconv_t>(-1) == handle_)
				return std::string();

			char * inbuf = const_cast<char*>(buf);
			std::size_t outlen = (len * 4 + 4);
			char * strbuf = new char[outlen + 4];
			char * outbuf = strbuf;
			std::size_t outleft = outlen;
			::iconv(handle_, &inbuf, &len, &outbuf, &outleft);
			std::string rstr(strbuf, outbuf);
			delete [] strbuf;
			return rstr;
		}
	//end class charset_conv
#endif

	//Caret implementation
	struct caret_rep
	{
		native_window_type window;
		bool has_input_method_focus{ false };
		bool visible{ false };
		point pos;
		size	size;
		rectangle rev;
		paint::graphics rev_graph;
		XIM	input_method{ 0 };
		XIC	input_context{ 0 };
		XFontSet input_font{ 0 };
		XRectangle input_spot;
		XRectangle input_status_area;
		long input_context_event_mask{ 0 };

		caret_rep(native_window_type wd)
			: window{ wd }
		{}

		//Copy the reversed graphics to the window
		bool reinstate()
		{
			if(rev.width && rev.height)
			{
				rev_graph.paste(window, rev, 0, 0);
				//Drop the reversed graphics in order to draw the
				//caret in the next flash.
				rev.width = rev.height = 0;
				return true;
			}
			return false;
		}

		void twinkle()
		{
			if(!visible)
				return;

			if(!reinstate())
			{
				rev_graph.bitblt(rectangle{size}, window, pos);
				rev.width = size.width;
				rev.height = size.height;
				rev.x = pos.x;
				rev.y = pos.y;

				paint::pixel_buffer pxbuf;
				pxbuf.open(rev_graph.handle());

				auto pxsize = pxbuf.size();
				for(int y = 0; y < static_cast<int>(pxsize.height); ++y)
					for(int x = 0; x < static_cast<int>(pxsize.width); ++x)
					{
						auto px = pxbuf.at({x, y});
						px->element.red = ~px->element.red;
						px->element.green = ~px->element.green;
						px->element.blue = ~px->element.blue;
					}
				pxbuf.paste(window, {rev.x, rev.y});
			}
		}
	};
	
	class timer_runner
	{
		typedef void (*timer_proc_t)(std::size_t id);

		struct timer_tag
		{
			std::size_t id;
			unsigned tid;
			std::size_t interval;
			std::size_t timestamp;
			timer_proc_t proc;
		};

		//timer_group
		//It owns a set of timers' identifier, and a container for the delay deletion
		//The delay delection is used for storing a timer id when the timer is deleted in a timer's
		//event handler function. If the timer is deleted directly in timer's event handler function,
		//it will cause a crash because the deletion operation invalidates iterator.
		//According to ISO C++ 2011, 23.2.4 9 the erase members shall invalidate only iterators and
		//references to the erased elements(timer_group::timers is an associative container),
		//although the iterator can be moved to next before calling the timer handler function, the delay
		//deletion is still required. Becuase a timer which is erased in another timer's handler function
		//happens to be refereneced by the "next" iterator.
		struct timer_group
		{
			bool proc_entered{false};	//This flag indicates whether the timers are going to do event.
			std::set<std::size_t> timers;
			std::vector<std::size_t> delay_deleted;
		};
	public:
		timer_runner()
			: is_proc_handling_(false)
		{}

		void set(std::size_t id, std::size_t interval, timer_proc_t proc)
		{
			auto i = holder_.find(id);
			if(i != holder_.end())
			{
				i->second.interval = interval;
				i->second.proc = proc;
				return;
			}
			unsigned tid = system::this_thread_id();
			threadmap_[tid].timers.insert(id);

			timer_tag & tag = holder_[id];
			tag.id = id;
			tag.tid = tid;
			tag.interval = interval;
			tag.timestamp = 0;
			tag.proc = proc;
		}

		bool is_proc_handling() const
		{
			return is_proc_handling_;
		}

		void kill(std::size_t id)
		{
			auto i = holder_.find(id);
			if(i != holder_.end())
			{
				auto tid = i->second.tid;
				
				auto ig = threadmap_.find(tid);
				if(ig != threadmap_.end())	//Generally, the ig should not be the end of threadmap_
				{
					auto & group = ig->second;
					if(!group.proc_entered)
					{
						group.timers.erase(id);
						if(group.timers.empty())
							threadmap_.erase(ig);
					}
					else
						group.delay_deleted.push_back(id);
				}
				holder_.erase(i);
			}
		}

		bool empty() const
		{
			return (holder_.empty());
		}

		void timer_proc(unsigned tid)
		{
			is_proc_handling_ = true;
			auto i = threadmap_.find(tid);
			if(i != threadmap_.end())
			{
				auto & group = i->second;
				group.proc_entered = true;
				unsigned ticks = system::timestamp();
				for(auto timer_id : group.timers)
				{
					auto & tag = holder_[timer_id];
					if(tag.timestamp)
					{
						if(ticks >= tag.timestamp + tag.interval)
						{
							tag.timestamp = ticks;
							try
							{
								tag.proc(tag.id);
							}catch(...){}	//nothrow
						}
					}
					else
						tag.timestamp = ticks;
				}
				group.proc_entered = false;
				for(auto tmr: group.delay_deleted)
					group.timers.erase(tmr);
			}
			is_proc_handling_ = false;
		}
	private:
		bool is_proc_handling_;
		std::map<unsigned, timer_group> threadmap_;
		std::map<std::size_t, timer_tag> holder_;
	};

	drawable_impl_type::drawable_impl_type()
	{
		string.tab_length = 4;
		string.tab_pixels = 0;
		string.whitespace_pixels = 0;
#if defined(NANA_USE_XFT)
		conv_.handle = ::iconv_open("UTF-8", "UTF-32");
		conv_.code = "UTF-32";
#endif
	}

	drawable_impl_type::~drawable_impl_type()
	{
#if defined(NANA_USE_XFT)
		::iconv_close(conv_.handle);
#endif
	}

	unsigned drawable_impl_type::get_color() const
	{
		return color_;
	}

	unsigned drawable_impl_type::get_text_color() const
	{
		return text_color_;
	}

	void drawable_impl_type::set_color(const ::color& clr)
	{
		color_ = (clr.px_color().value & 0xFFFFFF);
	}

	void drawable_impl_type::set_text_color(const ::color& clr)
	{
		text_color_ = (clr.px_color().value & 0xFFFFFF);
		update_text_color();
	}

	void drawable_impl_type::update_color()
	{
		if (color_ != current_color_)
		{
			auto & spec = detail::platform_spec::instance();
			platform_scope_guard lock;

			current_color_ = color_;
			auto col = color_;
			switch (spec.screen_depth())
			{
			case 16:
				col = ((((col >> 16) & 0xFF) * 31 / 255) << 11) |
					((((col >> 8) & 0xFF) * 63 / 255) << 5) |
					(col & 0xFF) * 31 / 255;
				break;
			}
			::XSetForeground(spec.open_display(), context, col);
			::XSetBackground(spec.open_display(), context, col);
		}
	}

	void drawable_impl_type::fgcolor(const ::color& clr)
	{
		auto rgb = clr.px_color().value;

		if (rgb != current_color_)
		{
			auto & spec = detail::platform_spec::instance();
			platform_scope_guard psg;

			current_color_ = rgb;
			switch(spec.screen_depth())
			{
			case 16:
				rgb = ((((rgb >> 16) & 0xFF) * 31 / 255) << 11) |
					((((rgb >> 8) & 0xFF) * 63 / 255) << 5)	|
					(rgb & 0xFF) * 31 / 255;
				break;
			}
			::XSetForeground(spec.open_display(), context, rgb);
			::XSetBackground(spec.open_display(), context, rgb);
#if defined(NANA_USE_XFT)
			xft_fgcolor.color.red = ((0xFF0000 & rgb) >> 16) * 0x101;
			xft_fgcolor.color.green = ((0xFF00 & rgb) >> 8) * 0x101;
			xft_fgcolor.color.blue = (0xFF & rgb) * 0x101;
			xft_fgcolor.color.alpha = 0xFFFF;
#endif
		}
	}

	class font_deleter
	{
    public:
        void operator()(const font_tag* fp) const
        {
            if(fp && fp->handle)
            {
                platform_scope_guard psg;
#if defined(NANA_USE_XFT)
                ::XftFontClose(detail::platform_spec::instance().open_display(), fp->handle);
#else
                ::XFreeFontSet(detail::platform_spec::instance().open_display(), fp->handle);
#endif
            }
            delete fp;
        }
	};//end class font_deleter

	platform_scope_guard::platform_scope_guard()
	{
		platform_spec::instance().lock_xlib();
	}

	platform_scope_guard::~platform_scope_guard()
	{
		platform_spec::instance().unlock_xlib();
	}

	int X11_error_handler(Display*, XErrorEvent* err)
	{
	    platform_spec::instance().error_code = err->error_code;
		return 0;
	}

	int X11_fatal_handler(Display*)
	{
		return 0;
	}

	platform_spec::timer_runner_tag::timer_runner_tag()
		: runner(0), delete_declared(false)
	{}

	platform_spec::platform_spec()
		:display_(0), colormap_(0), def_X11_error_handler_(0), grab_(0)
	{
	}

	platform_spec::~platform_spec()
	{
		delete msg_dispatcher_;

		//The font should be destroyed before closing display,
		//otherwise it crashs
		def_font_ptr_.reset();

		close_display();
	}

	const platform_spec::font_ptr_t& platform_spec::default_native_font() const
	{
		return def_font_ptr_;
	}
	
	void platform_spec::default_native_font(const font_ptr_t& fp)
	{
		def_font_ptr_ = fp;
	}

	unsigned platform_spec::font_size_to_height(unsigned size) const
	{
		return size;
	}

	unsigned platform_spec::font_height_to_size(unsigned height) const
	{
		return height;
	}

}//end namespace detail
}//end namespace nana

#include <nana/pop_ignore_diagnostic>
#endif //NANA_POSIX && NANA_X11
