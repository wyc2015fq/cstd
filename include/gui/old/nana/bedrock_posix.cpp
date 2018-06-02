/*
 *	A Bedrock Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://nanapro.sourceforge.net/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/detail/linux_X11/bedrock.cpp
 */

#include <nana/detail/platform_spec_selector.hpp>
#if defined(NANA_POSIX) && defined(NANA_X11)
#include <nana/gui/detail/bedrock_pi_data.hpp>
#include <nana/gui/detail/event_code.hpp>
#include <nana/system/platform.hpp>
#include <nana/gui/detail/inner_fwd_implement.hpp>
#include <nana/gui/detail/native_window_interface.hpp>
#include <nana/gui/layout_utility.hpp>
#include <nana/gui/detail/element_store.hpp>
#include <errno.h>
#include <algorithm>

namespace nana
{
namespace detail
{
#pragma pack(1)
		union event_mask
		{
			struct
			{
				short x;
				short y;
			}pos;

			struct
			{
				short width;
				short height;
			}size;

			struct
			{
				unsigned short vkey;
				short delta;
			}wheel;
		};
#pragma pack()

	struct bedrock::thread_context
	{
		unsigned event_pump_ref_count{0};

		int		window_count{0};	//The number of windows
		core_window_t* event_window{nullptr};
		bool	is_alt_pressed{false};
		bool	is_ctrl_pressed{false};

		struct platform_detail_tag
		{
			native_window_type	motion_window;
			point		motion_pointer_pos;
		}platform;

		struct cursor_tag
		{
			core_window_t * window;
			native_window_type native_handle;
			cursor predef_cursor;
			Cursor handle;
		}cursor;

		thread_context()
		{
			cursor.window = nullptr;
			cursor.native_handle = nullptr;
			cursor.predef_cursor = cursor::arrow;
			cursor.handle = 0;
		}
	};
	
	struct bedrock::private_impl
	{
		typedef std::map<unsigned, thread_context> thr_context_container;
		std::recursive_mutex mutex;
		thr_context_container thr_contexts;
		
		element_store estore;

		struct cache_type
		{
			struct thread_context_cache
			{
				unsigned tid{ 0 };
				thread_context *object{ nullptr };
			}tcontext;
		}cache;
	};

	void timer_proc(unsigned);
	void window_proc_dispatcher(Display*, detail::msg_packet_tag&);
	void window_proc_for_packet(Display *, detail::msg_packet_tag&);
	void window_proc_for_xevent(Display*, XEvent&);

	//class bedrock defines a static object itself to implement a static singleton
	//here is the definition of this object
	bedrock bedrock::bedrock_object;

	bedrock::bedrock()
		: pi_data_(new pi_data), impl_(new private_impl)
	{
		detail::platform_spec::instance().msg_set(timer_proc, window_proc_dispatcher);
	}

	bedrock::~bedrock()
	{
		delete pi_data_;
		delete impl_;
	}

	void bedrock::flush_surface(core_window_t* wd, bool forced, const rectangle* update_area)
	{
		wd->drawer.map(reinterpret_cast<window>(wd), forced, update_area);
	}

	//inc_window
	//@biref: increament the number of windows
	int bedrock::inc_window(unsigned tid)
	{
		private_impl * impl = instance().impl_;
		std::lock_guard<decltype(impl->mutex)> lock(impl->mutex);

		int & cnt = (impl->thr_contexts[tid ? tid : system::this_thread_id()].window_count);
		return (cnt < 0 ? cnt = 1 : ++cnt);
	}

	bedrock::thread_context* bedrock::open_thread_context(unsigned tid)
	{
		if(0 == tid) tid = system::this_thread_id();

		std::lock_guard<decltype(impl_->mutex)> lock(impl_->mutex);
		if(impl_->cache.tcontext.tid == tid)
			return impl_->cache.tcontext.object;

		bedrock::thread_context* context = nullptr;

		private_impl::thr_context_container::iterator i = impl_->thr_contexts.find(tid);
		if(i == impl_->thr_contexts.end())
			context = &(impl_->thr_contexts[tid]);
		else
			context = &(i->second);

		impl_->cache.tcontext.tid = tid;
		impl_->cache.tcontext.object = context;
		return context;
	}

	bedrock::thread_context* bedrock::get_thread_context(unsigned tid)
	{
		if(0 == tid) tid = system::this_thread_id();

		std::lock_guard<decltype(impl_->mutex)> lock(impl_->mutex);
		if(impl_->cache.tcontext.tid == tid)
			return impl_->cache.tcontext.object;

		private_impl::thr_context_container::iterator i = impl_->thr_contexts.find(tid);
		if(i != impl_->thr_contexts.end())
		{
			impl_->cache.tcontext.tid = tid;
			return (impl_->cache.tcontext.object = &(i->second));
		}

		impl_->cache.tcontext.tid = 0;
		return 0;
	}

	void bedrock::remove_thread_context(unsigned tid)
	{
		if(0 == tid) tid = system::this_thread_id();

		std::lock_guard<decltype(impl_->mutex)> lock(impl_->mutex);

		if(impl_->cache.tcontext.tid == tid)
		{
			impl_->cache.tcontext.tid = 0;
			impl_->cache.tcontext.object = nullptr;
		}

		impl_->thr_contexts.erase(tid);
	}

	bedrock& bedrock::instance()
	{
		return bedrock_object;
	}

	bedrock::core_window_t* bedrock::focus()
	{
		core_window_t* wd = wd_manager().root(native_interface::get_focus_window());
		return (wd ? wd->other.attribute.root->focus : 0);
	}

	void bedrock::get_key_state(arg_keyboard& arg)
	{
		XKeyEvent xkey;
		detail::platform_spec::instance().read_keystate(xkey);
		arg.ctrl = (xkey.state & ControlMask);
		arg.shift = (xkey.state & ShiftMask);
	}

	element_store& bedrock::get_element_store() const
	{
		return impl_->estore;
	}

	void bedrock::map_through_widgets(core_window_t*, native_drawable_type)
	{
		//No implementation for Linux
	}

	bool bedrock::emit(event_code evt_code, core_window_t* wd, const ::event_arg& arg, bool ask_update, thread_context* thrd)
	{
		if(wd_manager().available(wd) == false)
			return false;

		core_window_t * prev_wd;
		if(thrd)
		{
			prev_wd = thrd->event_window;
			thrd->event_window = wd;
			_m_event_filter(evt_code, wd, thrd);
		}

		if(wd->other.upd_state == core_window_t::update_state::none)
			wd->other.upd_state = core_window_t::update_state::lazy;

		_m_emit_core(evt_code, wd, false, arg);

		//A child of wd may not be drawn if it was out of wd's range before wd resized,
		//so refresh all children of wd when a resized occurs.
		if(ask_update || (event_code::resized == evt_code))
		{
			wd_manager().do_lazy_refresh(wd, false, (event_code::resized == evt_code));
		}
		else if(wd_manager().available(wd))
			wd->other.upd_state = core_window_t::update_state::none;

		if(thrd) thrd->event_window = prev_wd;
		return true;
	}

	void assign_arg(arg_mouse& arg, basic_window* wd, unsigned msg, const XEvent& evt)
	{
		arg.window_handle = reinterpret_cast<window>(wd);
		arg.button = ::mouse::any_button;

		int mask_state = 0;
		if (ButtonPress == msg || ButtonRelease == msg)
		{
			mask_state = evt.xbutton.state;
			if (evt.xbutton.button != Button4 && evt.xbutton.button != Button5)
			{
				arg.evt_code = (ButtonPress == msg ? event_code::mouse_down : event_code::mouse_up);
				arg.pos.x = evt.xbutton.x - wd->pos_root.x;
				arg.pos.y = evt.xbutton.y - wd->pos_root.y;

				switch (evt.xbutton.button)
				{
				case Button1:
					arg.button = ::mouse::left_button;
					break;
				case Button2:
					arg.button = ::mouse::middle_button;
					break;
				case Button3:
					arg.button = ::mouse::right_button;
					break;
				}
			}
		}
		else if (msg == MotionNotify)
		{
			mask_state = evt.xmotion.state;
			arg.evt_code = event_code::mouse_move;
			arg.pos.x = evt.xmotion.x - wd->pos_root.x;
			arg.pos.y = evt.xmotion.y - wd->pos_root.y;
		}
		else if (EnterNotify == msg)
		{
			mask_state = evt.xcrossing.state;
			arg.evt_code = event_code::mouse_enter;
			arg.pos.x = evt.xcrossing.x - wd->pos_root.x;
			arg.pos.y = evt.xcrossing.y - wd->pos_root.y;
		}

		arg.left_button		= ((Button1Mask & mask_state) != 0) || (::mouse::left_button == arg.button) ;
		arg.right_button	= ((Button2Mask & mask_state) != 0) || (::mouse::right_button == arg.button);
		arg.mid_button		= ((Button3Mask & mask_state) != 0) || (::mouse::middle_button == arg.button);
		arg.alt		= ((Mod1Mask & mask_state) != 0);
		arg.shift	= ((ShiftMask & mask_state) != 0);
		arg.ctrl	= ((ControlMask & mask_state) != 0);

	}

	void assign_arg(arg_focus& arg, basic_window* wd, native_window_type recv, bool getting)
	{
		arg.window_handle = reinterpret_cast<window>(wd);
		arg.receiver = recv;
		arg.getting = getting;
        arg.focus_reason = arg_focus::reason::general;
	}

	void assign_arg(arg_wheel& arg, basic_window* wd, const XEvent& evt)
	{
		arg.evt_code = event_code::mouse_wheel;
		arg.window_handle = reinterpret_cast<window>(wd);
		if (ButtonRelease == evt.type && (evt.xbutton.button == Button4 || evt.xbutton.button == Button5))
		{
			arg.evt_code = event_code::mouse_wheel;
			arg.pos.x = evt.xbutton.x - wd->pos_root.x;
			arg.pos.y = evt.xbutton.y - wd->pos_root.y;

			arg.upwards = (evt.xbutton.button == Button4);
			arg.left_button = arg.mid_button = arg.right_button = false;
			arg.shift = arg.ctrl = false;
			arg.distance = 120;
			arg.which = arg_wheel::wheel::vertical;
		}
		
	}

	void timer_proc(unsigned tid)
	{
		detail::platform_spec::instance().timer_proc(tid);
	}

	void window_proc_dispatcher(Display* display, detail::msg_packet_tag& msg)
	{
		switch(msg.kind)
		{
		case detail::msg_packet_tag::kind_xevent:
			window_proc_for_xevent(display, msg.u.xevent);
			break;
		case detail::msg_packet_tag::kind_mouse_drop:
			window_proc_for_packet(display, msg);
			break;
		default: break;
		}
	}

	void window_proc_for_packet(Display * /*display*/, detail::msg_packet_tag& msg)
	{
		static auto& brock = detail::bedrock::instance();

		auto native_window = reinterpret_cast<native_window_type>(msg.u.packet_window);
		auto root_runtime = brock.wd_manager().root_runtime(native_window);

		if(root_runtime)
		{
			auto msgwd = root_runtime->window;

			switch(msg.kind)
			{
			case detail::msg_packet_tag::kind_mouse_drop:
				msgwd = brock.wd_manager().find_window(native_window, msg.u.mouse_drop.x, msg.u.mouse_drop.y);
				if(msgwd)
				{
					arg_dropfiles arg;
					arg.window_handle = reinterpret_cast<window>(msgwd);
					arg.files.swap(*msg.u.mouse_drop.files);
					delete msg.u.mouse_drop.files;
					arg.pos.x = msg.u.mouse_drop.x - msgwd->pos_root.x;
					arg.pos.y = msg.u.mouse_drop.y - msgwd->pos_root.y;
					msgwd->annex.events_ptr->mouse_dropfiles.emit(arg, reinterpret_cast<window>(msgwd));
					brock.wd_manager().do_lazy_refresh(msgwd, false);
				}
				break;
			default:
				throw std::runtime_error("Nana.GUI.Bedrock: Undefined message packet");
			}
		}
	}

	template<typename Arg>
	void draw_invoker(void(::detail::drawer::*event_ptr)(const Arg&), basic_window* wd, const Arg& arg, bedrock::thread_context* thrd)
	{
		if(bedrock::instance().wd_manager().available(wd) == false)
			return;
		basic_window * pre_wd;
		if(thrd)
		{
			pre_wd = thrd->event_window;
			thrd->event_window = wd;
		}

		if(wd->other.upd_state == basic_window::update_state::none)
			wd->other.upd_state = basic_window::update_state::lazy;

		(wd->drawer.*event_ptr)(arg);
		if(thrd) thrd->event_window = pre_wd;
	}

	wchar_t os_code_from_keysym(KeySym keysym)
	{
		switch(keysym)
		{
		case XK_Alt_L: case XK_Alt_R:
			keysym = keyboard::alt;	break;
		case XK_BackSpace:
			keysym = keyboard::backspace;	break;
		case XK_Tab:
			keysym = keyboard::tab;		break;
		case XK_Escape:
			keysym = keyboard::escape;	break;
		case XK_Return:
			keysym = keyboard::enter;	break;
		case XK_Cancel:
			keysym = keyboard::end_of_text;	break;	//Ctrl+C
		case XK_Page_Up:
			keysym = keyboard::os_pageup;	break;
		case XK_Page_Down:
			keysym = keyboard::os_pagedown; break;
		case XK_Left: case XK_Up: case XK_Right: case XK_Down:
			keysym = keyboard::os_arrow_left + (keysym - XK_Left); break;
		case XK_Insert:
			keysym = keyboard::os_insert; break;
		case XK_Delete:
			keysym = keyboard::os_del; break;
		case XK_Shift_L: case XK_Shift_R:	//shift
			keysym = keyboard::os_shift; break;
		case XK_Control_L: case XK_Control_R: //ctrl
			keysym = keyboard::os_ctrl;	break;
		default:
			do
			{
				//Transfer the keysym for key_press and key_release event to the same behavior with Windows
				if('a' <= keysym && keysym <= 'z')
				{
					keysym = keysym - 'a' + 'A';
					break;
				}
				//reverts the key
				static const char shift_num[]=")!@#$%^&*(";
				auto p = std::find(shift_num, shift_num + sizeof shift_num, char(keysym));
				if(p != shift_num + sizeof shift_num)
				{
					keysym = (p - shift_num + '0');
					break;
				}

				switch(keysym)
				{
				case '~': keysym = '`'; break;
				case '_': keysym = '-'; break;
				case '+': keysym = '='; break;
				case '{': keysym = '['; break;
				case '}': keysym = ']'; break;
				case '|': keysym = '\\'; break;
				case ':': keysym = ';'; break;
				case '"': keysym = '\''; break;
				case '<': keysym = ','; break;
				case '>': keysym = '.'; break;
				case '?': keysym = '/'; break;
				}
			}while(false);
		}
		return wchar_t(keysym);
	}

	void bedrock::thread_context_destroy(core_window_t * wd)
	{
		bedrock::thread_context * thr = get_thread_context(0);
		if(thr && thr->event_window == wd)
			thr->event_window = nullptr;
	}

	void bedrock::thread_context_lazy_refresh()
	{
		thread_context* thrd = get_thread_context(0);
		if(thrd && thrd->event_window)
		{
			//the state none should be tested, becuase in an event, there would be draw after an update,
			//if the none is not tested, the draw after update will not be refreshed.
			switch(thrd->event_window->other.upd_state)
			{
			case core_window_t::update_state::none:
			case core_window_t::update_state::lazy:
				thrd->event_window->other.upd_state = core_window_t::update_state::refresh;
			default:	break;
			}
		}
	}

	//Dynamically set a cursor for a window
	void bedrock::set_cursor(core_window_t* wd, cursor cur, thread_context* thrd)
	{
		if (nullptr == thrd)
			thrd = get_thread_context(wd->thread_id);

		if ((cursor::arrow == cur) && !thrd->cursor.native_handle)
			return;

		thrd->cursor.window = wd;
		if ((thrd->cursor.native_handle == wd->root) && (cur == thrd->cursor.predef_cursor))
			return;

		auto & spec = detail::platform_spec::instance();
		Display * disp = spec.open_display();

		if (thrd->cursor.native_handle && (thrd->cursor.native_handle != wd->root))
			::XUndefineCursor(disp, reinterpret_cast<Window>(thrd->cursor.native_handle));

		thrd->cursor.native_handle = wd->root;
		if (thrd->cursor.predef_cursor != cur)
		{
			thrd->cursor.predef_cursor = cur;
			if (thrd->cursor.handle)
			{
				::XFreeCursor(disp, thrd->cursor.handle);
				thrd->cursor.handle = 0;
			}
		}

		if (cursor::arrow == cur)
		{
			thrd->cursor.native_handle = nullptr;
			thrd->cursor.window = nullptr;
			::XUndefineCursor(disp, reinterpret_cast<Window>(wd->root));
		}
		else
		{
			if (!thrd->cursor.handle)
				thrd->cursor.handle = ::XCreateFontCursor(disp, static_cast<unsigned>(cur));
			::XDefineCursor(disp, reinterpret_cast<Window>(wd->root), thrd->cursor.handle);
		}
	}

	void bedrock::define_state_cursor(core_window_t* wd, cursor cur, thread_context* thrd)
	{
		wd->root_widget->other.attribute.root->state_cursor = cur;
		wd->root_widget->other.attribute.root->state_cursor_window = wd;
		set_cursor(wd, cur, thrd);
	}

	void bedrock::undefine_state_cursor(core_window_t * wd, thread_context* thrd)
	{
		if (!wd_manager().available(wd))
			return;
	
		wd->root_widget->other.attribute.root->state_cursor = cursor::arrow;
		wd->root_widget->other.attribute.root->state_cursor_window = nullptr;

		auto pos = native_interface::cursor_position();
		auto native_handle = native_interface::find_window(pos.x, pos.y);
		if (!native_handle)
			return;

		native_interface::calc_window_point(native_handle, pos);
		auto rev_wd = wd_manager().find_window(native_handle, pos.x, pos.y);
		if (rev_wd)
			set_cursor(rev_wd, rev_wd->predef_cursor, thrd);
	}

	void bedrock::_m_event_filter(event_code event_id, core_window_t * wd, thread_context * thrd)
	{
		auto not_state_cur = (wd->root_widget->other.attribute.root->state_cursor == cursor::arrow);

		switch(event_id)
		{
		case event_code::mouse_enter:
			if (not_state_cur)
				set_cursor(wd, wd->predef_cursor, thrd);
			break;
		case event_code::mouse_leave:
			if (not_state_cur && (wd->predef_cursor != cursor::arrow))
				set_cursor(wd, cursor::arrow, thrd);
			break;
		case event_code::destroy:
			if (wd->root_widget->other.attribute.root->state_cursor_window == wd)
				undefine_state_cursor(wd, thrd);

			if(wd == thrd->cursor.window)
				set_cursor(wd, cursor::arrow, thrd);
			break;
		default:
			break;
		}
	}
}//end namespace detail
}//end namespace nana
#endif //NANA_POSIX && NANA_X11
