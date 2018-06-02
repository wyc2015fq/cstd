/**
 *	A Tabbar implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *   @file:  nana/gui/widgets/tabbar.hpp
 *	 @brief A tabbar contains tab items and toolbox for scrolling, closing, selecting items.
 *
 */
#ifndef NANA_GUI_WIDGET_TABBAR_HPP
#define NANA_GUI_WIDGET_TABBAR_HPP


#include "widget.hpp"
#include <nana/pat/cloneable.hpp>
#include <nana/any.hpp>

namespace nana
{
	template<typename T> class tabbar;

	template<typename T>
	struct arg_tabbar
		: public event_arg
	{
		tabbar<T> & widget;
		T & value;

		arg_tabbar(tabbar<T>& wdg, T& v)
			: widget(wdg), value{ v }
		{}
	};

	template<typename T>
	struct arg_tabbar_removed : public arg_tabbar<T>
	{
		arg_tabbar_removed(tabbar<T>& wdg, T& v)
			: arg_tabbar<T>({wdg, v})
		{}

		bool remove = true;					///< determines whether to remove the item
		bool close_attach_window = true;	///< determines whether to close the attached window. It is ignored if remove is false

	};

	namespace drawerbase
	{
		namespace tabbar
		{
			template<typename T>
			struct tabbar_events
				: public general_events
			{
				using value_type = T;

				basic_event<arg_tabbar<value_type>> added;
				basic_event<arg_tabbar<value_type>> activated;
				basic_event<arg_tabbar_removed<value_type>> removed;
			};

			class event_agent_interface
			{
			public:
				virtual ~event_agent_interface() = default;
				virtual void added(size_t) = 0;
				virtual void activated(size_t) = 0;
				virtual bool removed(size_t, bool & close_attached) = 0;
			};

			class item_renderer
			{
			public:
				typedef item_renderer item_renderer_type;
				typedef graphics & graph_reference;
				enum state_t{disable, normal, highlight, press};

				struct item_t
				{
					rectangle r;
					color	bgcolor;
					color	fgcolor;
				};

				virtual ~item_renderer() = default;
				virtual void background(graph_reference, const rectangle& r, const color& bgcolor) = 0;
				virtual void item(graph_reference, const item_t&, bool active, state_t) = 0;
				virtual void close_fly(graph_reference, const rectangle&, bool active, state_t) = 0;

				virtual void add(graph_reference, const rectangle&, state_t) = 0;
				virtual void close(graph_reference, const rectangle&, state_t) = 0;
				virtual void back(graph_reference, const rectangle&, state_t) = 0;
				virtual void next(graph_reference, const rectangle&, state_t) = 0;
				virtual void list(graph_reference, const rectangle&, state_t) = 0;
			};

			template<typename T, typename DrawerTrigger>
			class event_agent
				: public event_agent_interface
			{
			public:
				using arg_tabbar = arg_tabbar<T>;

				event_agent(tabbar<T>& tb, DrawerTrigger & dtr)
					: tabbar_(tb), drawer_trigger_(dtr)
				{}

				void added(size_t pos)
				{
					if(pos != npos)
					{
						drawer_trigger_.at_no_bound_check(pos) = T();
						tabbar_.events().added.emit(arg_tabbar({ tabbar_, tabbar_[pos] }), tabbar_);
					}
				}

				void activated(size_t pos)
				{
					if(pos != npos)
						tabbar_.events().activated.emit(arg_tabbar({ tabbar_, tabbar_[pos]}), tabbar_);
				}

				bool removed(size_t pos, bool & close_attach)
				{
					if (pos != npos)
					{
						arg_tabbar_removed<T> arg(tabbar_, tabbar_[pos]);
						tabbar_.events().removed.emit(arg, tabbar_);
						close_attach = arg.close_attach_window;
						return arg.remove;
					}
					close_attach = true;
					return true;
				}
			private:
				tabbar<T> & tabbar_;
				DrawerTrigger& drawer_trigger_;
			};

			class layouter;

			class trigger
				: public drawer_trigger
			{
			public:
				using native_string_type = native_string_type;

				enum kits
				{
					add,	///< The type identifies the add button of the tabbar's toolbox.
					scroll,	///< The type identifies the scroll button of the tabbar's toolbox
					list,	///< The type identifies the list button of the tabbar's toolbox
					close	///< The type identifies the close button of the tabbar's toolbox
				};

				trigger();
				~trigger();
				void activate(size_t);
				size_t activated() const;
				any& at(size_t) const;
				any& at_no_bound_check(size_t) const;
				const pat::cloneable<item_renderer> & ext_renderer() const;
				void ext_renderer(const pat::cloneable<item_renderer>&);
				void set_event_agent(event_agent_interface*);
				void insert(size_t, native_string_type&&, any&&);
				size_t length() const;
				bool close_fly(bool);
				window attach(size_t, window, bool drop_other);
				void erase(size_t);
				void tab_color(size_t, bool is_bgcolor, const color&);
				void tab_image(size_t, const image&);
				void text(size_t, const native_string_type&);
				native_string_type text(size_t) const;
				bool toolbox(kits, bool);
			private:
				void attached(widget_reference, graph_reference)	override;
				void detached()	override;
				void refresh(graph_reference)	override;
				void mouse_down(graph_reference, const arg_mouse&)	override;
				void mouse_up(graph_reference, const arg_mouse&)	override;
				void mouse_move(graph_reference, const arg_mouse&)	override;
				void mouse_leave(graph_reference, const arg_mouse&)	override;
			private:
				layouter * layouter_;
			};
		}
	}//end namespace drawerbase
    /// Analogous to dividers in a notebook or the labels in a file cabinet
	template<typename Type>
	class tabbar
		: public widget_object<category::widget_tag, drawerbase::tabbar::trigger, drawerbase::tabbar::tabbar_events<Type>>
	{
		typedef drawerbase::tabbar::trigger drawer_trigger_t;
	public:
		using value_type = Type;            ///< The type of element data which is stored in the tabbar.
		using item_renderer = drawerbase::tabbar::item_renderer; ///< A user-defined item renderer should be derived from this interface.
		using kits = drawer_trigger_t::kits;

		tabbar()
		{
			evt_agent_.reset(new drawerbase::tabbar::event_agent<value_type, drawer_trigger_t>(*this, this->get_drawer_trigger()));
			this->get_drawer_trigger().set_event_agent(evt_agent_.get());
		}

		tabbar(window wd, bool visible)
			: tabbar()
		{
			this->create(wd, rectangle(), visible);
		}

		tabbar(window wd, const rectangle& r = rectangle(), bool visible = true)
			: tabbar()
		{
			this->create(wd, r, visible);
		}

		~tabbar()
		{
			this->get_drawer_trigger().set_event_agent(NULL);
		}

		value_type & operator[](size_t pos) const
		{
			return any_cast<value_type&>(this->get_drawer_trigger().at_no_bound_check(pos));
		}

		void activated(size_t pos)                  /// Activates a tab specified by pos.
		{
			this->get_drawer_trigger().activate(pos);
		}

		size_t activated() const
		{
			return this->get_drawer_trigger().activated();
		}

		value_type const & at(size_t pos) const        /// Returns pos'th element
		{
			return any_cast<value_type&>(this->get_drawer_trigger().at(pos));
		}

		void close_fly(bool fly)                    /// Draw or not a close button in each tab.
		{
			if (this->get_drawer_trigger().close_fly(fly))
				API::refresh_window(this->handle());
		}

		const pat::cloneable<item_renderer>& renderer() const
		{
			return this->get_drawer_trigger().ext_renderer();
		}

		void renderer(const pat::cloneable<item_renderer>& ir)
		{
			this->get_drawer_trigger().ext_renderer(ir);
		}

		size_t length() const                 /// Returns the number of items.
		{
			return this->get_drawer_trigger().length();
		}

		/// Append a new tab
		tabbar& append(string text, window attach_wd, value_type value = {})  // 2x text convertion. maybe better to duplicate code?
		{
			return this->append( static_cast<wchar*&>(charset(move(text), unicode::utf8)), attach_wd, move(value));
		}

		tabbar& append(wstring text, window attach_wd, value_type value = {})
		{
			if (attach_wd && API::empty_window(attach_wd))
				throw invalid_argument("Appening a tab to a tabbar - error: tabbar.attach: invalid window handle");

			this->get_drawer_trigger().insert(npos, to_nstring(move(text)), move(value));
			if (attach_wd)
				this->attach(this->get_drawer_trigger().length() - 1, attach_wd);
			
			API::update_window(*this);
			return *this;
		}

		void push_back(string text)  /// Append a new item.
		{
			this->get_drawer_trigger().insert(npos, to_nstring(move(text)), value_type());
			API::update_window(*this);
		}

		void insert(size_t pos, string text, value_type value = {})
		{
			if (pos > length())
				throw out_of_range("tabbar::insert invalid position");

			this->get_drawer_trigger().insert(pos, to_nstring(text), move(value));
			API::update_window(*this);
		}

		void insert(size_t pos, wstring text, value_type value = {})
		{
			if (pos > length())
				throw out_of_range("tabbar::insert invalid position");

			this->get_drawer_trigger().insert(pos, to_nstring(move(text)), move(value));
			API::update_window(*this);
		}

		/// Attach a window to a specified tab. When the tab is activated, tabbar shows the attached window. 
		/**
		 * @param pos The position of tab to set the attached window.
		 * @param attach_wd A handle to the window to be set.
		 * @param drop_other Drop the attached windows of other tabs whose attach windows are equal to the parameter attach_wd. If drop_other is true, the other tabs attached windows equal to attach_wd will be dropped.
		 * @return A handle to the last attached window of specified tab.
		 */
		window attach(size_t pos, window attach_wd, bool drop_other = true)
		{
			if (attach_wd && API::empty_window(attach_wd))
				throw invalid_argument("tabbar.attach: invalid window handle");

			return this->get_drawer_trigger().attach(pos, attach_wd, drop_other);
		}

		void erase(size_t pos)
		{
			this->get_drawer_trigger().erase(pos);
		}

		void tab_bgcolor(size_t pos, const color& clr)
		{
			this->get_drawer_trigger().tab_color(pos, true, clr);
		}

		void tab_fgcolor(size_t pos, const color& clr)
		{
			this->get_drawer_trigger().tab_color(pos, false, clr);
		}

		void tab_image(size_t pos, const image& img)
		{
			this->get_drawer_trigger().tab_image(pos, img);
		}

        /// Sets buttons of the tabbar's toolbox, refer to notes for more details.
		void toolbox(kits kit, bool enable)
		{
			if (this->get_drawer_trigger().toolbox(kit, enable))
				API::refresh_window(this->handle());
		}

		void text(size_t pos, const char* str) /// Sets the title of the specified item, If pos is invalid, the method throws an out_of_range object.
		{
			this->get_drawer_trigger().text(pos, to_nstring(str));
		}

		string text(size_t pos) const /// Returns a title of a specified item, If pos is invalid, the method trhows a out_of_range object.
		{
			return to_utf8(this->get_drawer_trigger().text(pos));
		}
	private:
		unique_ptr<drawerbase::tabbar::event_agent<value_type, drawerbase::tabbar::trigger> > evt_agent_;
	};
}//end namespace nana


namespace nana
{	
		namespace drawerbase
		{
			namespace tabbar_lite
			{
				class model;

				struct events
					: public general_events
				{
					basic_event<event_arg> selected;
				};

				class driver
					: public drawer_trigger
				{
				public:
					driver();
					~driver();

					model* get_model() const;
				private:
					//Overrides drawer_trigger's method
					void attached(widget_reference, graph_reference)	override;
					void refresh(graph_reference);
					void mouse_move(graph_reference, const arg_mouse&);
					void mouse_leave(graph_reference, const arg_mouse&);
					void mouse_down(graph_reference, const arg_mouse&);
				private:
					model* const model_;
				};
			}
		}//end namespace drawerbase

	class tabbar_lite
		: public widget_object<category::widget_tag, drawerbase::tabbar_lite::driver, drawerbase::tabbar_lite::events>
	{
	public:
		tabbar_lite() = default;
		tabbar_lite(window, bool visible = true, constrectangle& = {});

	public: //capacity
		size_t length() const;

	public: //modifiers
		void attach(size_t pos, window);
		window attach(size_t pos) const;

		void push_back(string text, any par = {});
		void push_front(string text, any par = {});

		size_t selected() const;
		void erase(size_t pos, bool close_attached = true);
	};
}


#endif
