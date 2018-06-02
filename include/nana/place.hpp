/*
 *	An Implementation of Place for Layout
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2014 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/place.cpp
 *
 *	@contributions:
 *	min/max and splitter bar initial weight by Ariel Vina-Rodriguez.
 */

#ifndef NANA_GUI_PLACE_HPP
#define NANA_GUI_PLACE_HPP



#include <functional>

namespace nana
{
	namespace paint
	{
		class graphics;
	}

	class widget;
	namespace detail
	{
		class place_agent
		{
		public:
			virtual ~place_agent() = default;
			virtual unique_ptr<widget> create(window) const = 0;
		};
	}

	template<typename Widget>
	class agent
		: public place_agent
	{
	public:
		agent(function<void(Widget&)> initializer)
			: init_(move(initializer))
		{}

		agent(const char* text)
			: text_(text)
		{
			throw_not_utf8(text);
		}

		agent(string text, function<void(Widget&)> initializer = {})
			: text_(move(text)), init_(move(initializer))
		{
			throw_not_utf8(text_);
		}

	private:
		unique_ptr<widget> create(window handle) const
		{
			unique_ptr<Widget> ptr(new Widget(handle));
			ptr->caption(text_);
			if (init_)
				init_(*ptr);
			return move(ptr);
		}
	private:
		string text_;
		function<void(Widget&)> init_;
	};

    ///  Layout managment - an object of class place is attached to a widget, and it automatically positions and resizes the children widgets.
	class place
		: noncopyable
	{
		struct implement;

		class field_interface
		{
			field_interface(const field_interface&) {}
			field_interface& operator=(const field_interface&) {}
			field_interface(field_interface&) {}
			field_interface& operator=(field_interface&) {}
		public:
			field_interface() = default;
			virtual ~field_interface() = default;
			virtual field_interface& operator<<(const char* label) = 0;
			virtual field_interface& operator<<(string label) = 0;
			virtual field_interface& operator<<(window) = 0;
			virtual field_interface& fasten(window) = 0;
			
			template<typename Widget>
			field_interface& operator<<(const agent<Widget>& ag)
			{
				_m_add_agent(ag);
				return *this;
			}
		private:
			virtual void _m_add_agent(const place_agent&) = 0;
		};
	public:
        ///  reference to a field manipulator which refers to a field object created by place 
		using field_reference = field_interface &;

		place();
		place(window);///< Attaches to a specified widget.
		~place();

		/** @brief Bind to a window
		 *	@param handle	A handle to a window which the place wants to attach.
		 *	@remark	It will throw an exception if the place has already binded to a window.
		 */
		void bind(window handle);
		window window_handle() const;

		void splitter_renderer(function<void(window, graphics&, mouse_action)> fn);
        
		void div(const char* s);			///< Divides the attached widget into fields.
		const char* div() const noexcept;	///< Returns div-text that depends on fields status.
		void modify(const char* field_name, const char* div_text);	///< Modifies a specified field.

		field_reference field(const char* name);///< Returns a field with the specified name.

		void field_visible(const char* field_name, bool visible); ///<<Shows/Hides an existing field.
		bool field_visible(const char* field_name) const;	///<Determines whether the specified field is visible.

		void field_display(const char* field_name, bool display); ///<Displays/Discards an existing field.
		bool field_display(const char* field_name) const;	///<Determines whether the specified field is displayed.

		void collocate();                     ///< Layouts the widgets.

 		void erase(window handle);				///< Erases a window from field.

		field_reference operator[](const char* name); ///< Returns a field with the specified name. Equal to field();

		/// Add a panel factory
		template<typename Panel, typename ...Args>
		place& dock(const char* dockname, const char* factory_name, Args&& ... args)
		{
			return dock(dockname, factory_name, bind([](window parent, Args & ... args)
			{
				return unique_ptr<widget>(new Panel(parent, forward<Args>(args)...));
			}, placeholders::_1, args...));
		}

		place& dock(const char* dockname, string factory_name, function<unique_ptr<widget>(window)> factory);
		widget* dock_create(const char* factory);
	private:
		implement * impl_;
	};
}//end namespace nana


#endif //#ifndef NANA_GUI_PLACE_HPP
