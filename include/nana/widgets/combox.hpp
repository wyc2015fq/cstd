/**
 *	A Combox Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @file: nana/gui/widgets/combox.hpp
 */

#ifndef NANA_GUI_WIDGETS_COMBOX_HPP
#define NANA_GUI_WIDGETS_COMBOX_HPP

#include "widget.hpp"
#include "float_listbox.hpp"
#include "skeletons/text_editor_part.hpp"
#include <nana/key_type.hpp>
#include <nana/concepts.hpp>
#include <functional>

namespace nana
{
	/// A list box combined with a textbox - the list box should drop down when the user selects the arrow next to the control
	class combox;

	struct arg_combox
		: public event_arg
	{
		combox & widget;
		arg_combox(combox&);
	};

	namespace drawerbase
	{
		namespace combox
		{
			struct combox_events
				: public general_events
			{
				basic_event<arg_combox>	selected;
				basic_event<arg_combox> text_changed;
			};

			class drawer_impl;

			class trigger
				: public drawer_trigger
			{
			public:
				trigger();
				~trigger();

				drawer_impl& get_drawer_impl();
				const drawer_impl& get_drawer_impl() const;
			private:
				void attached(widget_reference, graph_reference)	override;
				void detached()	override;
				void refresh(graph_reference)	override;
				void focus(graph_reference, const arg_focus&)	override;
				void mouse_enter(graph_reference, const arg_mouse&)	override;
				void mouse_leave(graph_reference, const arg_mouse&)	override;
				void mouse_down(graph_reference, const arg_mouse&)	override;
				void mouse_up(graph_reference, const arg_mouse&)	override;
				void mouse_move(graph_reference, const arg_mouse&)	override;
				void mouse_wheel(graph_reference, const arg_wheel&)	override;
				void key_press(graph_reference, const arg_keyboard&)	override;
				void key_char(graph_reference, const arg_keyboard&)	override;
			private:
				drawer_impl * const drawer_;
			};

			class item_proxy
				: public iterator<input_iterator_tag, item_proxy>
			{
			public:
				item_proxy(drawer_impl*, size_t pos);
				item_proxy&		text(const char*);
				::string	text() const;
				item_proxy&	select();
				bool		selected() const;
				item_proxy& icon(const image&);
				image icon() const;

				template<typename T>
				T * value_ptr() const
				{
					return any_cast<T>(_m_anyobj(false));
				}

				template<typename T>
				T & value() const
				{
					auto * pany = _m_anyobj(false);
					if (NULL == pany)
						throw runtime_error("combox::item_proxy.value<T>() is empty");

					T * p = any_cast<T>(pany);
					if (NULL == p)
						throw runtime_error("combox::item_proxy.value<T>() invalid type of value");
					return *p;
				}

				template<typename T>
				item_proxy& value(const T& t)
				{
					*_m_anyobj(true) = t;
					return *this;
				}

				template<typename T>
				item_proxy& value(T&& t)
				{
					*_m_anyobj(true) = ::move(t);
					return *this;
				}
			public:
				/// Behavior of Iterator's value_type
				bool operator==(const char*) const;
				bool operator==(const char*) const;

				/// Behavior of Iterator
				item_proxy & operator=(const item_proxy&);

				/// Behavior of Iterator
				item_proxy & operator++();

				/// Behavior of Iterator
				item_proxy	operator++(int);

				/// Behavior of Iterator
				item_proxy& operator*();

				/// Behavior of Iterator
				const item_proxy& operator*() const;

				/// Behavior of Iterator
				item_proxy* operator->();

				/// Behavior of Iterator
				const item_proxy* operator->() const;

				/// Behavior of Iterator
				bool operator==(const item_proxy&) const;

				/// Behavior of Iterator
				bool operator!=(const item_proxy&) const;
			private:
				any * _m_anyobj(bool alloc_if_empty) const;
			private:
				drawer_impl * impl_;
				size_t pos_;
			};
		}//end namespace combox
	}//end namespace drawerbase

	class combox
		:	public widget_object<category::widget_tag, drawerbase::combox::trigger, drawerbase::combox::combox_events, widgets::skeletons::text_editor_scheme>,
			public concepts::any_objective<size_t, 1>
	{
	public:
		typedef float_listbox::item_renderer item_renderer;
		typedef drawerbase::combox::item_proxy item_proxy;

		combox();
		combox(window, bool visible);
		combox(window, ::string, bool visible = true);
		combox(window, const char*, bool visible = true);
		combox(window, const rectangle& r = rectangle(), bool visible = true);

		void clear();
		void editable(bool);
		bool editable() const;
		void set_accept(function<bool(wchar_t)>);
		combox& push_back(string);
		size_t the_number_of_options() const;
		size_t option() const;   ///< Index of the last selected, from drop-down list, item.
		void option(size_t);   ///< Select the text specified by index
		::string text(size_t) const;
		void erase(size_t pos);

		template<typename Key>
		item_proxy operator[](const Key& kv)
		{
			typedef typename type_escape<Key>::type key_t;
			shared_ptr<key_interface> p(new key<key_t, less<key_t> >(kv), [](key_interface*p)
			{
				delete p;
			});

			return _m_at_key(move(p));
		}

		template<typename Key>
		void erase_key(const Key& kv)
		{
			typedef typename type_escape<Key>::type key_t;
			unique_ptr<key_interface> p(new key<key_t, less<key_t> >(kv));
			_m_erase(p.get());
		}

		template<typename Key>
		void erase_key(Key&& kv)
		{
			typedef typename type_escape<Key>::type key_t;
			unique_ptr<key_interface> p(new key<key_t, less<key_t> >(move(kv)));
			_m_erase(p.get());
		}

		/// \brief Set user-defined item renderer object.
		/// It is an address therefore the user should not destroy the renderer object
		/// after it is set to the combox. Passing null_ptr cancels the user-defined renderer object.
		void renderer(item_renderer*);

		void image(size_t, const image&);
		image image(size_t) const;
		void image_pixels(unsigned);  ///<Sets the width of image area. Default is 16 pixels.
	private:
		item_proxy _m_at_key(shared_ptr<key_interface>&&);
		void _m_erase(key_interface*);
		drawerbase::combox::drawer_impl & _m_impl();
		const drawerbase::combox::drawer_impl& _m_impl() const;
	private:
		//Overrides widget's virtual functions
		native_string_type _m_caption() const;
		void _m_caption(native_string_type&&);
		any * _m_anyobj(size_t pos, bool alloc_if_empty) const;
	};
}

#endif
