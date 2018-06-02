/*
 *	Definition of Notifier
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/notifier.hpp
 */

#ifndef NANA_GUI_NOTIFIER_HPP
#define NANA_GUI_NOTIFIER_HPP

#include <nana/gui/detail/general_events.hpp>


namespace nana
{
	class notifier;

	struct arg_notifier
		: public event_arg
	{
		event_code	evt_code;
		notifier*	notifier_ptr;
		point	pos;
		bool	left_button;
		bool	mid_button;
		bool	right_button;

		operator arg_mouse() const;
	};

	namespace detail
	{
		struct notifier_events
		{
			basic_event<arg_notifier> mouse_move;
			basic_event<arg_notifier> mouse_down;
			basic_event<arg_notifier> mouse_up;
			basic_event<arg_notifier> mouse_leave;
			basic_event<arg_notifier> dbl_click;
		};
	}

	class notifier
	{
		struct implement;
		notifier(const notifier&) {}
		notifier(notifier&) {}
		notifier& operator=(const notifier&) {}
		notifier& operator=(notifier&) {}
	public:
		notifier(window);
		~notifier();
		void close();
		void text(const char*);
		void icon(const char* icon_file);
		void insert_icon(const char* icon_file);
		void period(unsigned millisecond);
		notifier_events& events();
		window handle() const;
	private:
		implement * impl_;
	};
}//end namespace nana

#endif
