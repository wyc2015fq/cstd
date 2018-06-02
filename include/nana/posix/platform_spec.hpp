/*
 *	Platform Specification Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/detail/platform_spec.hpp
 *
 *	This file provides basis class and data structrue that required by nana
 *	This file should not be included by any header files.
 */

#if defined(NANA_LINUX) || defined(NANA_MACOS)

#ifndef NANA_DETAIL_PLATFORM_SPEC_HPP
#define NANA_DETAIL_PLATFORM_SPEC_HPP



#include <thread>
#include <mutex>

#include <condition_variable>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xos.h>

#include <nana/paint/image.hpp>
#include <nana/paint/graphics.hpp>
#include <nana/gui/detail/event_code.hpp>

#include <vector>
#include <map>
#include "msg_packet.hpp"
#include "../platform_abstraction_types.hpp"

#if defined(NANA_USE_XFT)
	#include <X11/Xft/Xft.h>
	#include <iconv.h>
	#include <fstream>
#endif

namespace nana
{
namespace detail
{
	class msg_dispatcher;

#if defined(NANA_USE_XFT)
	class conf
	{
	public:
		conf(const char * file);
		bool open(const char* file);
		string value(const char* key);
	private:
		ifstream ifs_;
	};

	class charset_conv
	{
		charset_conv(const charset_conv&) {}
		charset_conv& operator=(const charset_conv*) {}
	public:
		charset_conv(const char* tocode, const char* fromcode);
		~charset_conv();
		string charset(const char* str) const;
		string charset(const char * buf, size_t len) const;
	private:
		iconv_t handle_;
	};
#endif

	struct drawable_impl_type
	{
		using font_type = ::shared_ptr<font_interface>;

		Pixmap	pixmap;
		GC	context;

		font_type font;

		point	line_begin_pos;

		struct string_spec
		{
			unsigned tab_length;
			unsigned tab_pixels;
			unsigned whitespace_pixels;
		}string;

#if defined(NANA_USE_XFT)
		XftDraw * xftdraw{NULL};
		XftColor	xft_fgcolor;
		const string charset(const wchar* str, const char* strcode);
#endif
		drawable_impl_type();
		~drawable_impl_type();

		unsigned get_color() const;
		unsigned get_text_color() const;
		void set_color(const color&);
		void set_text_color(const color&);

		void update_color();
		void update_text_color();
	private:
		drawable_impl_type(const drawable_impl_type&) {}
		drawable_impl_type& operator=(const drawable_impl_type&) {}

		unsigned current_color_{ 0xFFFFFF };
		unsigned color_{ 0xFFFFFFFF };
		unsigned text_color_{ 0xFFFFFFFF };

#if defined(NANA_USE_XFT)
		struct conv_tag
		{
			iconv_t handle;
			string code;
		}conv_;
#endif
	};

	struct atombase_tag
	{
		Atom wm_protocols;
		//window manager support
		Atom wm_change_state;
		Atom wm_delete_window;
		//ext
		Atom net_wm_state;
		Atom net_wm_state_skip_taskbar;
		Atom net_wm_state_fullscreen;
		Atom net_wm_state_maximized_horz;
		Atom net_wm_state_maximized_vert;
		Atom net_wm_state_modal;
		Atom net_wm_name;
		Atom net_wm_window_type;
		Atom net_wm_window_type_normal;
		Atom net_wm_window_type_utility;
		Atom net_wm_window_type_dialog;
		Atom motif_wm_hints;

		Atom clipboard;
		Atom text;
		Atom text_uri_list;
		Atom utf8_string;
		Atom targets;

		Atom xdnd_aware;
		Atom xdnd_enter;
		Atom xdnd_position;
		Atom xdnd_status;
		Atom xdnd_action_copy;
		Atom xdnd_drop;
		Atom xdnd_selection;
		Atom xdnd_typelist;
		Atom xdnd_finished;
	};

	//A forward declaration of caret data
	struct caret_rep;

	class timer_runner;

	class platform_scope_guard
	{
	public:
		platform_scope_guard();
		~platform_scope_guard();
	};

	class platform_spec
	{
		typedef platform_spec self_type;

		struct window_context_t
		{
			native_window_type owner;
			vector<native_window_type> * owned;
		};
	public:
		int error_code;
	public:
		typedef void (*timer_proc_type)(unsigned tid);
		typedef void (*event_proc_type)(Display*, msg_packet_tag&);
		typedef event_code		event_code;
		typedef native_window_type	native_window_type;

		platform_spec(const platform_spec&) {}
		platform_spec& operator=(const platform_spec&) {}

		platform_spec();
		~platform_spec();

		Display* open_display();
		void close_display();

		void lock_xlib();
		void unlock_xlib();

		Window root_window();
		int screen_depth();
		Visual* screen_visual();

		Colormap& colormap();

		static self_type& instance();
		const atombase_tag & atombase() const;

		void make_owner(native_window_type owner, native_window_type wd);
		native_window_type get_owner(native_window_type) const;
		void remove(native_window_type);

		void write_keystate(const XKeyEvent&);
		void read_keystate(XKeyEvent&);

		XIC	caret_input_context(native_window_type) const;
		void caret_open(native_window_type, const size&);
		void caret_close(native_window_type);
		void caret_pos(native_window_type, const point&);
		void caret_visible(native_window_type, bool);
		bool caret_update(native_window_type, graphics& root_graph, bool is_erase_caret_from_root_graph);
		void set_error_handler();
		int rev_error_handler();

		//grab
		//register a grab window while capturing it if it is unviewable.
		//when native_interface::show a window that is registered as a grab
		//window, the native_interface grabs the window.
		Window grab(Window);
		void set_timer(size_t id, size_t interval, void (*timer_proc)(size_t id));
		void kill_timer(size_t id);
		void timer_proc(unsigned tid);

		//Message dispatcher
		void msg_insert(native_window_type);
		void msg_set(timer_proc_type, event_proc_type);
		void msg_dispatch(native_window_type modal);

		//X Selections
		void* request_selection(native_window_type requester, Atom type, size_t & bufsize);
		void write_selection(native_window_type owner, Atom type, const void* buf, size_t bufsize);

		//Icon storage
		//@biref: The image object should be kept for a long time till the window is closed,
		//			the image object is release in remove() method.
		const graphics& keep_window_icon(native_window_type, const image&);
	private:
		static int _m_msg_filter(XEvent&, msg_packet_tag&);
		void _m_caret_routine();
	private:
		Display*	display_;
		Colormap	colormap_;
		atombase_tag atombase_;

		XKeyEvent	key_state_;
		int (*def_X11_error_handler_)(Display*, XErrorEvent*);
		Window grab_;
		recursive_mutex xlib_locker_;
		struct caret_holder_tag
		{
			volatile bool exit_thread;
			unique_ptr<thread> thr;
			map<native_window_type, caret_rep*> carets;
		}caret_holder_;

		map<native_window_type, window_context_t> wincontext_;
		map<native_window_type, graphics> iconbase_;

		struct timer_runner_tag
		{
			timer_runner * runner;
			recursive_mutex mutex;
			bool delete_declared;
			timer_runner_tag();
		}timer_;

		struct selection_tag
		{
			struct item_t
			{
				Atom	type;
				Window	requestor;
				void*	buffer;
				size_t	bufsize;
				mutex cond_mutex;
				condition_variable cond;
			};

			vector<item_t*> items;

			struct content_tag
			{
				string * utf8_string;
			}content;
		}selection_;

		struct xdnd_tag
		{
			Atom good_type;
			int timestamp;
			Window wd_src;
			point pos;
		}xdnd_;

		msg_dispatcher * msg_dispatcher_;
	};//end class platform_X11

}//end namespace detail

}//end namespace nana


// .h ward
#endif

//#if defined(NANA_LINUX) || defined(NANA_MACOS)
#endif

