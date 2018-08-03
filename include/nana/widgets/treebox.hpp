/**
 *	A Tree Box Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *   @file   nana/gui/widgets/treebox.hpp
 *	 @brief
 *		The treebox organizes the nodes by a key string. 
 *		The treebox would have a vertical scrollbar if there are too many nodes
 *	    to display. It does not have an horizontal scrollbar:
 *	    the widget will adjust the node's displaying position for fitting.
 */

#ifndef NANA_GUI_WIDGETS_TREEBOX_HPP
#define NANA_GUI_WIDGETS_TREEBOX_HPP


#include "widget.hpp"
#include "detail/compset.hpp"
#include "detail/tree_cont.hpp"
#include <nana/gui/timer.hpp>
#include <nana/any.hpp>
#include <nana/pat/cloneable.hpp>
#include <stdexcept>

namespace nana
{
	class treebox;

	namespace drawerbase
	{
		namespace treebox
		{
			enum component
			{
				begin, expander = begin, crook, icon, text, bground, end
			};

			struct node_image_tag
			{
				image normal;
				image hovered;
				image expanded;
			};

			struct node_attribute
			{
				bool has_children;
				bool expended;
				checkstate checked;
				bool selected;
				bool mouse_pointed;
				image icon_normal;
				image icon_hover;
				image icon_expanded;
				::string text;
			};

			typedef widgets::compset<component, node_attribute> compset_interface;
			typedef widgets::compset_placer<component, node_attribute> compset_placer_interface;
			
			class renderer_interface
			{
			public:
				typedef drawerbase::treebox::component component;
				typedef graphics& graph_reference;
				typedef drawerbase::treebox::compset_interface compset_interface;
				typedef compset_interface::item_attribute_t item_attribute_t;
				typedef compset_interface::comp_attribute_t comp_attribute_t;

				virtual ~renderer_interface() = default;

				virtual void begin_paint(widget&) = 0;
				virtual void bground(graph_reference, const compset_interface *) const = 0;
				virtual void expander(graph_reference, const compset_interface *) const = 0;
				virtual void crook(graph_reference, const compset_interface *) const = 0;
				virtual void icon(graph_reference, const compset_interface *) const = 0;
				virtual void text(graph_reference, const compset_interface *) const = 0;
			};

			class item_proxy;

			class trigger
				:public drawer_trigger
			{
				template<typename Renderer>
				struct basic_implement;

				class item_renderer;
				class item_locator;

				typedef basic_implement<item_renderer> implement;
			public:
				struct treebox_node_type
				{
					treebox_node_type();
					treebox_node_type(string);
					treebox_node_type& operator=(const treebox_node_type&);

					::string text;
					any value;
					bool expanded;
					checkstate checked;
					::string img_idstr;
				};

				struct pseudo_node_type{};

				using tree_cont_type = widgets::tree_cont<treebox_node_type>;
				using node_type = tree_cont_type::node_type;

				trigger();
				~trigger();

				implement * impl() const;

				void check(node_type*, checkstate);

				void renderer(pat::cloneable<renderer_interface>&&);
				const pat::cloneable<renderer_interface>& renderer() const;
				void placer(pat::cloneable<compset_placer_interface>&&);
				const pat::cloneable<compset_placer_interface>& placer() const;

				node_type* insert(node_type*, const char* key, char*&);
				node_type* insert(const char* path, char*&);

				node_type * selected() const;
				void selected(node_type*);

				node_image_tag& icon(const char*) const;
				void icon_erase(const char*);
				void node_icon(node_type*, const char* id);
				unsigned node_width(const node_type*) const;

				bool rename(node_type*, const char* key, const char* name);
			private:
				//Overrides drawer_trigger methods
				void attached(widget_reference, graph_reference)		override;
				void detached();
				void refresh(graph_reference)	override;
				void dbl_click(graph_reference, const arg_mouse&)	override;
				void mouse_down(graph_reference, const arg_mouse&)	override;
				void mouse_up(graph_reference, const arg_mouse&)	override;
				void mouse_move(graph_reference, const arg_mouse&)	override;
				void mouse_wheel(graph_reference, const arg_wheel&)	override;
				void mouse_leave(graph_reference, const arg_mouse&)	override;
				void resized(graph_reference, const arg_resized&)		override;
				void key_press(graph_reference, const arg_keyboard&)	override;
				void key_char(graph_reference, const arg_keyboard&)	override;
			private:
				implement * const impl_;
			}; //end class trigger


			/// \brief A proxy for accessing the node. The key string is case sensitive.
			class item_proxy
				: public iterator<input_iterator_tag, item_proxy>
			{
			public:
				item_proxy() = default;           ///< The default constructor creates an end iterator.

				//Undocumented constructor.
				item_proxy(trigger*, trigger::node_type*);

				/// Append a child.
				item_proxy append(const char* key, ::string name);

				/// Append a child with a specified value (user object.).
				template<typename T>
				item_proxy append(const char* key, ::string name, const T&t)
				{
					item_proxy ip = append(key, move(name));
					if(false == ip.empty())
						ip.value(t);
					return ip;
				}

				/// Return true if the proxy does not refer to a node, as an end iterator.
				bool empty() const;

				/// \brief Return the distance between the ROOT node and this node.
				/// @return  only available when emtpy() is false.
				size_t level() const;

				/// Return the check state
				bool checked() const;

				/// Set the check state, and it returns itself.
				item_proxy& check(bool);

				/// Clears the child nodes
				item_proxy& clear();

				/// Return true when the node is expanded  \todo change to expanded ??
				bool expanded() const;

				/// Expand/Shrink children of the node, and returns itself.  \todo change to expand ??
				item_proxy& expand(bool);

				/// Return true when the node is selected.
				bool selected() const;

				/// Select the node, and returns itself..
				item_proxy& select(bool);

				/// Return the icon.
				const char* icon() const;

				/// Set the icon, and returns itself..
				item_proxy& icon(const char* id);

				/// Return the text.
				const char* text() const;

				/// Set the text, and returns itself.
				item_proxy& text(const char*);

				/// Set a new key, and returns itself..
				item_proxy& key(const char* s);

				/// Return the key.
				const char* key() const;

				size_t size() const; ///< Returns the number of child nodes.

				/// Return the first child of the node.
				item_proxy child() const;

				/// Return the owner of the node.
				item_proxy owner() const;

				/// Return the sibling of the node.
				item_proxy sibling() const;

				/// Return the first child of the node
				item_proxy begin() const;

				/// An end node.
				item_proxy end() const;

				/// Makes an action for each sub item recursively, returns the item that stops the action where action returns false.
				item_proxy visit_recursively(function<bool(item_proxy)> action);

				bool operator==(const char* s) const; ///< Compare the text of node with s.
				bool operator==(const char* s ) const;        ///< Compare the text of node with s.
				bool operator==(const wchar_t* s ) const;     ///< Compare the text of node with s.

				/// Behavior of Iterator
				item_proxy& operator=(const item_proxy&);

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

				template<typename T>
				const T * value_ptr() const
				{
					return any_cast<T>(&_m_value());
				}

				template<typename T>
				const T& value() const
				{
					auto p = any_cast<T>(&_m_value());
					if(NULL == p)
						throw runtime_error("treebox::value<T>() Invalid type of value.");
					return *p;
				}

				template<typename T>
				item_proxy & value(const T& t)
				{
					_m_value() = t;
					return *this;
				}

				template<typename T>
				item_proxy & value(T&& t)
				{
					_m_value() = move(t);
					return *this;
				}

				// Undocumentated methods for internal use
				trigger::node_type * _m_node() const;
			private:
				any& _m_value();
				const any& _m_value() const;
			private:
				trigger * trigger_{NULL};
				trigger::node_type * node_{NULL};
			};//end class item_proxy
		}//end namespace treebox
	}//end namespace drawerbase

    ///  a type of treebox event parameter
	struct arg_treebox 
		: public event_arg
	{
		treebox& widget;                          ///< where the event occurs
		drawerbase::treebox::item_proxy & item;   ///< the operated node
		bool	operated;                         ///< operation state of the event

		arg_treebox(treebox&, drawerbase::treebox::item_proxy&, bool operated);
	};

	namespace drawerbase
	{
		namespace treebox
		{
			struct treebox_events
				: public general_events
			{
				basic_event<arg_treebox> expanded; ///< a user expands or shrinks a node
				basic_event<arg_treebox> checked;  ///< a user checks or unchecks a node
				basic_event<arg_treebox> selected; ///< a user selects or unselects a node
				basic_event<arg_treebox> hovered;  ///< a user moves the cursor over a node
			};
		}//end namespace treebox
	}//end namespace drawerbase

    /// \brief  Displays a hierarchical list of items, such as the files and directories on a disk.
    /// See also in [documentation](http://nanapro.org/en-us/documentation/widgets/treebox.htm)
    class treebox
		:public widget_object < category::widget_tag, drawerbase::treebox::trigger, drawerbase::treebox::treebox_events>
	{
	public:
        /// A type refers to the item and is also used to iterate through the nodes.
		typedef drawerbase::treebox::item_proxy	item_proxy;

        /// state images for the node
		typedef drawerbase::treebox::node_image_tag node_image_type;

		/// The interface of treebox user-defined item renderer
		typedef drawerbase::treebox::renderer_interface renderer_interface;

		/// The interface of treebox compset_placer to define the position of node components
		typedef drawerbase::treebox::compset_placer_interface compset_placer_interface;

		/// The default constructor without creating the widget.
		treebox();

		/// \brief The construct that creates a widget.
		/// @param wd  A handle to the parent window of the widget being created.
		/// @param visible  specifying the visibility after creating.
		treebox(window wd, bool visible);

		/// \brief  The construct that creates a widget.
		/// @param wd  A handle to the parent window of the widget being created.
		/// @param r  the size and position of the widget in its parent window coordinate.
		/// @param visible  specifying if visible after creating.
		treebox(window, const rectangle& = rectangle(), bool visible = true);

		template<typename ItemRenderer>
		treebox & renderer(const ItemRenderer & rd) ///< set user-defined node renderer
		{
			get_drawer_trigger().renderer(pat::cloneable<renderer_interface>(rd));
			return *this;
		}

		const pat::cloneable<renderer_interface> & renderer() const;  ///< get user-defined node renderer

		template<typename Placer>
		treebox & placer(const Placer & r) ///< location of a node components
		{
			get_drawer_trigger().placer(pat::cloneable<compset_placer_interface>(r));
			return *this;
		}

		const pat::cloneable<compset_placer_interface> & placer() const;

		/// \brief  Eanble the widget to be draws automatically when it is operated.
        ///
        /// The treebox automatically redraws after certain operations, but, 
        /// under some circumstances, it is good to disable the automatic drawing mode, 
        /// for example, before adding nodes in a loop, disable the mode avoiding 
        /// frequent and useless refresh for better performance, and then, after 
        /// the operations, enable the automatic redraw mode again.
		/// @param enable bool  whether to enable.
		void auto_draw(bool enable);

		/// \brief  Enable the checkboxs for each item of the widget.
		/// @param enable bool  indicates whether to show or hide the checkboxs.
		treebox & checkable(bool enable);

		
		bool checkable() const; ///< Are the checkboxs are enabled?

		/// Clears the contents
		void clear();

        /// \brief Creates an icon scheme with the specified name.
        ///
        /// The icon scheme includes 3 images for node states. 
        /// These states are 'normal', 'hovered' and 'expanded'. 
        /// If 'hovered' or 'expanded' are not set, it uses 'normal' state image for these 2 states.
        /// See also in [documentation](http://nanapro.org/en-us/help/widgets/treebox.htm)
		node_image_type& icon(const char* id ///< the name of an icon scheme. If the name is not existing, it creates a new scheme for the name.
                               ) const;

		void icon_erase(const char* id);

		item_proxy find(const char* keypath);  ///< Find an item through a specified keypath.

        /// Inserts a new node to treebox, but if the keypath exists change and returns the existing node.
		item_proxy insert(const char* path_key,   ///< specifies the node hierarchy
                           ::string title      ///< used for displaying
                           ); 

        /// Inserts a new node to treebox, but if the keypath exists change and returns the existing node.
		item_proxy insert( item_proxy pos,             ///< the parent item node
                           const char* key,    ///< specifies the new node
                           ::string title   ///< title used for displaying in the new node.
                           );

		item_proxy erase(item_proxy i); ///< Removes the node at i and return the Item proxy following the removed node

		void erase(const char* keypath); ///< Removes the node by the key path. 

		::string make_key_path(item_proxy i, const char* splitter) const;///<returns the key path

		item_proxy selected() const; ///< returns the selected node

	};//end class treebox
}//end namespace nana



#endif