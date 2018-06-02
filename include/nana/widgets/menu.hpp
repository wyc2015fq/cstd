/**
 *	A Menu implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2009-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/widgets/menu.hpp
 *
 */

#ifndef NANA_GUI_WIDGETS_MENU_HPP
#define NANA_GUI_WIDGETS_MENU_HPP
#include "widget.hpp"
#include <nana/pat/cloneable.hpp>


namespace nana
{
	class menu;

	namespace drawerbase
	{
		namespace menu
		{
			struct menu_type; //declaration

			using native_string_type = native_string_type;

			enum checks
			{
				none,
				option,
				highlight
			};

			struct menu_item_type
			{
				/// This class is used as parameter of menu event function.
				class item_proxy
				{
				public:
					item_proxy(size_t pos, menu*);
					item_proxy& enabled(bool);
					bool		enabled() const;

					item_proxy&	check_style(checks);
					item_proxy&	checked(bool);
					bool		checked() const;

					item_proxy& text(string title_utf8);
					string text() const;

					size_t index() const;
				private:
					size_t const	pos_;
					menu* const	menu_;
				};
				    /// A callback functor type.  
				typedef function<void(item_proxy&)> event_fn_t;

				//Default constructor initializes the item as a splitter
				menu_item_type();
				menu_item_type(string, const event_fn_t&);

				struct
				{
					bool enabled:1;
					bool splitter:1;
					bool checked:1;
				}flags;

				struct
				{
					bool			own_creation;	//Indicates the menu_ptr is created by create_sub_menu
					menu_type*		menu_ptr;
				}linked;

				string	text;
				event_fn_t	event_handler;
				checks			style{checks::none};
				image	image;
				mutable wchar_t	hotkey{0};
			};

			class renderer_interface
			{
			public:
				using graph_reference = graphics &;

				enum state
				{
					normal, active
				};

				struct attr
				{
					state item_state;
					bool enabled;
					bool checked;
					checks check_style;
				};

				virtual ~renderer_interface() = default;

				virtual void background(graph_reference, window) = 0;
				virtual void item(graph_reference, const rectangle&, const attr&) = 0;
				virtual void item_image(graph_reference, const point&, unsigned image_px, const image&) = 0;
				virtual void item_text(graph_reference, const point&, const char*, unsigned text_pixels, const attr&) = 0;
				virtual void sub_arrow(graph_reference, const point&, unsigned item_pixels, const attr&) = 0;
			};
		}//end namespace menu
	}//end namespace drawerbase

	class menu
		: private noncopyable
	{
		struct implement;

		//let menubar access the private _m_popup() method.
		friend class menu_accessor;
	public:
		typedef drawerbase::menu::checks checks;

		typedef drawerbase::menu::renderer_interface renderer_interface;
		typedef drawerbase::menu::menu_item_type::item_proxy item_proxy;
		typedef drawerbase::menu::menu_item_type::event_fn_t event_fn_t;	///< A callback functor type. Prototype: `void(item_proxy&)`

		menu();										///< The default constructor. NO OTHER CONSTRUCTOR.
		~menu();

			/// Appends an item to the menu.
		item_proxy	append(string text_utf8, const event_fn_t& handler = {});
		void		append_splitter();

		/// Inserts new item at specified position
		/**
		 * It will invalidate the existing item proxies from the specified position.
		 * @param pos The position where new item to be inserted
		 * @param text_utf8 The title of item
		 * @param handler The event handler for the item.
		 * @return the item proxy to the new inserted item.
		 */
		item_proxy	insert(size_t pos, string text_utf8, const event_fn_t& handler = {});

		void clear();								///< Erases all of the items.
		/// Closes the menu. It does not destroy the menu; just close the window for the menu.
		void close();
		void image(size_t pos, const image& icon);
		void text(size_t pos, string text_utf8);
		string text(size_t pos) const;
		void check_style(size_t pos, checks);
		void checked(size_t pos, bool);
		bool checked(size_t pos) const;
		void enabled(size_t pos, bool);///< Enables or disables the mouse or keyboard input for the item.
		bool enabled(size_t pos) const;
		void erase(size_t pos);			 	 ///< Removes the item
		bool link(size_t pos, menu& menu_obj);///< Link a menu to the item as a sub menu.
		menu * link(size_t pos) const;		 	     ///< Retrieves a linked sub menu of the item.
		menu *create_sub_menu(size_t pos);
		void popup(window owner, int x, int y);     ///< Popup the menu at the owner window. 
		void popup_await(window owner, int x, int y);
		void answerer(size_t index, const event_fn_t&);  ///< Modify answerer of the specified item.
		void destroy_answer(function<void()>);  ///< Sets an answerer for the callback while the menu window is closing.
		void gaps(const point&);				///< Sets the gap between a menu and its sub menus.(\See Note4)
		void goto_next(bool forward);				///< Moves the focus to the next or previous item.
		bool goto_submen();///< Popup the submenu of the current item if it has a sub menu. Returns true if succeeds.
		bool exit_submenu();						///< Closes the current window of the sub menu.
		size_t size() const;					///< Return the number of items.
		int send_shortkey(wchar_t key);
		void pick();

		menu& max_pixels(unsigned);				    ///< Sets the max width in pixels of the item.
		unsigned max_pixels() const;

		menu& item_pixels(unsigned);				///< Sets the height in pixel for the items.
		unsigned item_pixels() const;

		void renderer(const pat::cloneable<renderer_interface>&);	///< Sets a user-defined renderer. 
		const pat::cloneable<renderer_interface>& renderer() const;

		/// Returns the handle of menu window
		/**
		 * @return handle of menu window, NULL if the menu hasn't been popped up.
		 */
		window handle() const;
	private:
		void _m_popup(window, const point& position, bool called_by_menubar);
	private:
		implement * impl_;
	};

	namespace detail
	{
		class popuper
		{
		public:
			popuper(menu&, mouse);
			popuper(menu&, window owner, const point&, mouse);
			void operator()(const arg_mouse&);
		private:
			menu & mobj_;
			window owner_;
			bool take_mouse_pos_;
			point pos_;
			mouse mouse_;
		};
	}

	popuper menu_popuper(menu&, mouse = mouse::right_button);
	popuper menu_popuper(menu&, window owner, const point&, mouse = mouse::right_button);
}//end namespace nana


#endif
