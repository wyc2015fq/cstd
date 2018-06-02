/*
 *	The fundamental widget class implementation
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/widgets/widget.cpp
 */

#include <nana/gui/widgets/widget.hpp>
#include <tooltip.hpp>
#include <nana/gui/detail/widget_notifier_interface.hpp>


	//class widget
	//@brief:The definition of class widget
		class widget::inner_widget_notifier : public widget_notifier_interface
		{
		public:
			inner_widget_notifier(widget& wdg)
				: wdg_(wdg)
			{}

		private:
			//implementation of widget_notifier_interface
			widget* widget_ptr() const
			{
				return &wdg_;
			}

			void destroy()
			{
				wdg_._m_notify_destroy();
			}

			native_string_type caption()
			{
				return wdg_._m_caption();
			}

			void caption(native_string_type text)
			{
				wdg_._m_caption(move(text));
			}
		private:
			widget& wdg_;
		};

		string widget::caption() const
		{
			return to_utf8(_m_caption());
		}

		wstring widget::caption_wstring() const
		{
#if defined(NANA_WINDOWS)
			return _m_caption();
#else
			return to_wstring(_m_caption());
#endif
		}

		auto widget::caption_native() const -> native_string_type
		{
			return _m_caption();
		}

		widget& widget::caption(string utf8)
		{
			throw_not_utf8(utf8);
			native_string_type str = to_nstring(utf8);
			_m_caption(move(str));
			return *this;
		}

		widget& widget::caption(wstring text)
		{
			native_string_type str = to_nstring(text);
			_m_caption(move(str));
			return *this;
		}

		void widget::i18n(i18n_eval eval)
		{
			if (handle())
			{
				native_string_type str = to_nstring(eval());
				_m_caption(move(str));
				internationalization_parts::set_eval(handle(), move(eval));
			}
		}

		cursor widget::cursor() const
		{
			return _m_cursor();
		}

		void widget::cursor(cursor cur)
		{
			_m_cursor(cur);
		}

		void widget::typeface(const font& font)
		{
			_m_typeface(font);
		}

		font widget::typeface() const
		{
			return _m_typeface();
		}

		void widget::close()
		{
			_m_close();
		}

		window widget::parent() const
		{
			return API::get_parent_window(handle());
		}

		bool widget::enabled() const
		{
			return API::window_enabled(handle());
		}

		void widget::enabled(bool value)
		{
			_m_enabled(value);
		}

		void widget::enable_dropfiles(bool enb)
		{
			API::enable_dropfiles(handle(), enb);
		}

		bool widget::empty() const
		{
			return (NULL == handle());
		}

		void widget::focus()
		{
			API::focus_window(handle());
		}

		bool widget::focused() const
		{
			return (API::focus_window() == handle());
		}

		void widget::show()
		{
			_m_show(true);
		}

		void widget::hide()
		{
			_m_show(false);
		}

		bool widget::visible() const
		{
			return _m_visible();
		}

		size widget::size() const
		{
			return API::window_size(handle());
		}

		void widget::size(const size& sz)
		{
			_m_size(sz);
		}

		void widget::set_capture(bool ignore_children)
		{
			API::set_capture(*this, ignore_children);
		}

		void widget::release_capture()
		{
			API::release_capture(*this);
		}

		point widget::pos() const
		{
			return API::window_position(handle());
		}

		void widget::move(int x, int y)
		{
			_m_move(x, y);
		}

		void widget::move(const point& pos)
		{
			_m_move(pos.x, pos.y);
		}

		void widget::move(const rectangle& r)
		{
			_m_move(r);
		}

		void widget::fgcolor(const color& col)
		{
			_m_fgcolor(col);
		}

		color widget::fgcolor() const
		{
			return _m_fgcolor();
		}

		void widget::bgcolor(const color& col)
		{
			_m_bgcolor(col);
		}

		color widget::bgcolor() const
		{
			return _m_bgcolor();
		}

		general_events& widget::events() const
		{
			return _m_get_general_events();
		}

		void widget::umake_event(event_handle eh) const
		{
			API::umake_event(eh);
		}

		widget& widget::register_shortkey(wchar_t key)
		{
			if (key)
				API::register_shortkey(handle(), static_cast<unsigned long>(key));
			else
				API::unregister_shortkey(handle());
			return *this;
		}

		widget& widget::take_active(bool activated, window take_if_not_activated)
		{
			API::take_active(handle(), activated, take_if_not_activated);
			return *this;
		}

		widget& widget::tooltip(const char* text)
		{
			tooltip::set(*this, text);
			return *this;
		}

		widget::operator widget::dummy_bool_type() const
		{
			return (handle()? dummy_bool_type(1):0);
		}

		widget::operator window() const
		{
			return handle();
		}

		unique_ptr<widget_notifier_interface> widget::_m_wdg_notifier()
		{
			return unique_ptr<widget_notifier_interface>(new inner_widget_notifier(*this));
		}

		void widget::_m_complete_creation()
		{}

		auto widget::_m_caption() const -> native_string_type
		{
			return API::dev::window_caption(handle());
		}

		void widget::_m_caption(native_string_type&& str)
		{
			API::dev::window_caption(handle(), move(str));
		}

		cursor widget::_m_cursor() const
		{
			return API::window_cursor(handle());
		}

		void widget::_m_cursor(cursor cur)
		{
			API::window_cursor(handle(), cur);
		}

		void widget::_m_close()
		{
			API::close_window(handle());
		}

		bool widget::_m_enabled() const
		{
			return API::window_enabled(handle());
		}

		void widget::_m_enabled(bool value)
		{
			API::window_enabled(handle(), value);
		}

		bool widget::_m_show(bool visible)
		{
			API::show_window(handle(), visible);
			return visible;
		}

		bool widget::_m_visible() const
		{
			return API::visible(handle());
		}

		void widget::_m_size(const size& sz)
		{
			API::window_size(handle(), sz);
		}

		void widget::_m_move(int x, int y)
		{
			API::move_window(handle(), { x, y });
		}

		void widget::_m_move(const rectangle& r)
		{
			API::move_window(handle(), r);
		}

		void widget::_m_typeface(const font& font)
		{
			API::typeface(handle(), font);
		}

		font widget::_m_typeface() const
		{
			return API::typeface(handle());
		}

		void widget::_m_fgcolor(const color& col)
		{
			API::fgcolor(handle(), col);
		}

		color widget::_m_fgcolor() const
		{
			return API::fgcolor(handle());
		}

		void widget::_m_bgcolor(const color& col)
		{
			API::bgcolor(handle(), col);
		}

		color widget::_m_bgcolor() const
		{
			return API::bgcolor(handle());
		}
	//end class widget

	namespace detail
	{
		unique_ptr<widget_notifier_interface> widget_notifier_interface::get_notifier(widget* wdg)
		{
			return unique_ptr<widget_notifier_interface>(new widget::inner_widget_notifier(*wdg));
		}

		//class widget_base
			window widget_base::handle() const
			{
				return handle_;
			}

			void widget_base::_m_notify_destroy()
			{
				handle_ = NULL;
			}
		//end class widget_base
	}


