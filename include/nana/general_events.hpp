/**
*	Definition of General Events
*	Nana C++ Library(http://www.nanapro.org)
*	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
*
*	Distributed under the Boost Software License, Version 1.0.
*	(See accompanying file LICENSE_1_0.txt or copy at
*	http://www.boost.org/LICENSE_1_0.txt)
*
*	@file: nana/gui/detail/general_events.hpp
*/
#ifndef NANA_DETAIL_GENERAL_EVENTS_HPP
#define NANA_DETAIL_GENERAL_EVENTS_HPP




#include "event_code.hpp"
#include "internal_scope_guard.hpp"
#include <vector>

namespace nana
{
#if 0
	namespace detail
	{
		bool check_window(window);
		void events_operation_register(event_handle);

		class event_interface
		{
		public:
			virtual ~event_interface() = default;
			virtual void remove(event_handle) = 0;
		};

		class docker_interface
		{
		public:
			virtual ~docker_interface() = default;
			virtual event_interface*	get_event() const = 0;
		};


		struct docker_base
			: public docker_interface
		{
			event_interface * event_ptr;
			bool flag_deleted{ false };
			const bool unignorable;

			docker_base(event_interface*, bool unignorable_flag);

			event_interface * get_event() const;
		};

		class event_base
			: public event_interface
		{
		public:
			~event_base();

			size_t length() const;
			void clear() noexcept;

			void remove(event_handle evt);
		protected:
			//class emit_counter is a RAII helper for emitting count
			//It is used for avoiding a try{}catch block which is required for some finial works when
			//event handlers throw exceptions. Precondition event_base.dockers_ != NULL.
			class emit_counter
			{
			public:
				emit_counter(event_base*);
				~emit_counter();
			private:
				event_base * const evt_;
			};
			
			event_handle _m_emplace(docker_interface*, bool in_front);
		protected:
			unsigned emitting_count_{ 0 };
			bool deleted_flags_{ false };
			vector<docker_interface*> * dockers_{ NULL };
		};
	}//end namespace detail

    /// base class for all event argument types
	class event_arg
	{
	public:
		virtual ~event_arg() = default;

        /// ignorable handlers behind the current one in a chain of event handlers will not get called.
		void stop_propagation() const;
		bool propagation_stopped() const;
	private:
		mutable bool stop_propagation_{ false };
	};

	struct general_events;

    /** @brief the type of the members of general_events. 
	*  
	*   It connect the functions to be called as response to the event and manages that chain of responses
	*   It is a functor, that get called to connect a "normal" response function, with normal "priority".
    *   If a response function need another priority (unignorable or called first) it will need to be connected with 
    *   the specific connect function not with the operator()	
	*   It also permit to "emit" that event, calling all the active responders.
	*/
	template<typename Arg>
	class basic_event : public event_base
	{
	public:
		using arg_reference = const typename remove_reference<Arg>::type &;
	private:
		struct docker
			: public docker_base
		{	
			/// the callback/response function taking the typed argument
			function<void(arg_reference)> invoke;

			docker(basic_event * evt, function<void(arg_reference)> && ivk, bool unignorable_flag)
				: docker_base(evt, unignorable_flag), invoke(move(ivk))
			{}

			docker(basic_event * evt, const function<void(arg_reference)> & ivk, bool unignorable_flag)
				: docker_base(evt, unignorable_flag), invoke(ivk)
			{}
		};
	public:
		/// Creates an event handler at the beginning of event chain
		template<typename Function>
		event_handle connect_front(Function && fn)
		{	
			using prototype = typename remove_reference<Function>::type;
			return _m_emplace(new docker(this, factory<prototype, is_bind_expression<prototype>::value>::build(forward<Function>(fn)), false), true);
		}

		/// It will not get called if stop_propagation() was called.
		event_handle connect(void (*fn)(arg_reference))
		{
			return connect([fn](arg_reference arg){
				fn(arg);
			});
		}

		/// It will not get called if stop_propagation() was called, because it is set at the end of the chain..
		template<typename Function>
		event_handle connect(Function && fn)
		{
			using prototype = typename remove_reference<Function>::type;
			return _m_emplace(new docker(this, factory<prototype, is_bind_expression<prototype>::value>::build(forward<Function>(fn)), false), false);
		}

		/// It will not get called if stop_propagation() was called.
        template<typename Function>
		event_handle operator()(Function&& fn)
		{
			return connect(forward<Function>(fn));
		}

		/// It will get called because it is unignorable.
        template<typename Function>
		event_handle connect_unignorable(Function && fn, bool in_front = false)
		{			
			using prototype = typename remove_reference<Function>::type;

			return _m_emplace(new docker(this, factory<prototype, is_bind_expression<prototype>::value>::build(forward<Function>(fn)), true), in_front);
		}

		void emit(arg_reference& arg, window window_handle)
		{
			internal_scope_guard lock;
			if (NULL == dockers_)
				return;

			emit_counter ec(this);

			//The dockers may resize when a new event handler is created by a calling handler.
			//Traverses with position can avaid crash error which caused by a iterator which becomes invalid.

			auto i = dockers_->data();
			auto const end = i + dockers_->size();

			for (; i != end; ++i)
			{
				if (static_cast<docker*>(*i)->flag_deleted)
					continue;

				static_cast<docker*>(*i)->invoke(arg);

				if (window_handle && (!check_window(window_handle)))
					break;

				if (arg.propagation_stopped())
				{
					for (++i; i != end; ++i)
					{
						if (!static_cast<docker*>(*i)->unignorable || static_cast<docker*>(*i)->flag_deleted)
							continue;

						static_cast<docker*>(*i)->invoke(arg);
						if (window_handle && (!check_window(window_handle)))
							break;
					}
					break;
				}
			}
		}
	private:
		template<typename Fn, bool IsBind>
		struct factory
		{
			static function<void(arg_reference)> build(Fn && fn)
			{
				return move(fn);
			}

			static function<void(arg_reference)> build(const Fn & fn)
			{
				return fn;
			}
		};

		template<typename Fn>
		struct factory<Fn, false>
		{
			typedef typename remove_reference<arg_reference>::type arg_type;
			typedef typename remove_reference<Fn>::type fn_type;

			template<typename Tfn>
			static function<void(arg_reference)> build(Tfn && fn)
			{
				typedef typename remove_reference<Tfn>::type type;
				return build_second(forward<Tfn>(fn), &type::operator());
			}

			template<typename Tfn, typename Ret>
			static function<void(arg_reference)> build_second(Tfn&& fn, Ret(fn_type::*)())
			{
				return [fn](arg_reference) mutable
				{
					fn();
				};
			}

			template<typename Tfn, typename Ret>
			static function<void(arg_reference)> build_second(Tfn&& fn, Ret(fn_type::*)()const)
			{
				return [fn](arg_reference) mutable
				{
					fn();
				};
			}

			static function<void(arg_reference)> build_second(fn_type&& fn, void(fn_type::*)(arg_reference))
			{
				return move(fn);
			}

			static function<void(arg_reference)> build_second(fn_type&& fn, void(fn_type::*)(arg_reference) const)
			{
				return move(fn);
			}

			static function<void(arg_reference)> build_second(fn_type& fn, void(fn_type::*)(arg_reference))
			{
				return fn;
			}

			static function<void(arg_reference)> build_second(fn_type& fn, void(fn_type::*)(arg_reference) const)
			{
				return fn;
			}
		
			static function<void(arg_reference)> build_second(const fn_type& fn, void(fn_type::*)(arg_reference))
			{
				return fn;
			}

			static function<void(arg_reference)> build_second(const fn_type& fn, void(fn_type::*)(arg_reference) const)
			{
				return fn;
			}

			template<typename Tfn, typename Ret, typename Arg2>
			static function<void(arg_reference)> build_second(Tfn&& fn, Ret(fn_type::*)(Arg2))
			{
				static_assert(is_convertible<arg_type, Arg2>::value, "The parameter type is not allowed, please check the function parameter type where you connected the event function.");
				return[fn](arg_reference arg) mutable
				{
					fn(arg);
				};
			}
			
			template<typename Tfn, typename Ret, typename Arg2>
			static function<void(arg_reference)> build_second(Tfn&& fn, Ret(fn_type::*)(Arg2)const)
			{
				static_assert(is_convertible<arg_type, Arg2>::value, "The parameter type is not allowed, please check the function parameter type where you connected the event function.");
				return [fn](arg_reference arg) mutable
				{
					fn(arg);
				};
			}
		};
		
		template<typename Ret, typename Arg2>
		struct factory < function<Ret(Arg2)>, false>
		{
			typedef typename remove_reference<arg_reference>::type arg_type;
			static_assert(is_convertible<arg_type, Arg2>::value, "The parameter type is not allowed, please check the function parameter type where you connected the event function.");

			static function<void(arg_reference)> build(const function<Ret(Arg2)>& fn)
			{
				return [fn](arg_reference arg) mutable{
					fn(arg);
				};
			}

			static function<void(arg_reference)> build_second(function<void(arg_reference)> && fn)
			{
				return move(fn);
			}
		};

		template<typename Ret>
		struct factory < function<Ret()>, false>
		{
			static function<void(arg_reference)> build(const function<Ret()>& fn)
			{
				return[fn](arg_reference) mutable{
					fn();
				};
			}
		};

		template<typename Ret>
		struct factory < Ret(*)(), false>
		{
			static function<void(arg_reference)> build(Ret(*fn)())
			{
				return[fn](arg_reference) mutable{
					fn();
				};
			}
		};

		template<typename Ret, typename Arg2>
		struct factory < Ret(*)(Arg2), false>
		{
			typedef typename remove_reference<arg_reference>::type arg_type;
			static_assert(is_convertible<arg_type, Arg2>::value, "The parameter type is not allowed, please check the function parameter type where you connected the event function.");

			static function<void(arg_reference)> build(Ret(*fn)(Arg2))
			{
				return[fn](arg_reference arg) mutable {
					fn(arg);
				};
			}
		};

		template<typename Ret>
		struct factory < Ret(), false>
		{
			static function<void(arg_reference)> build(Ret(*fn)())
			{
				return[fn](arg_reference){
					fn();
				};
			}
		};

		template<typename Ret, typename Arg2>
		struct factory < Ret(Arg2), false>
		{
			typedef typename remove_reference<arg_reference>::type arg_type;
			static_assert(is_convertible<arg_type, Arg2>::value, "The parameter type is not allowed, please check the function parameter type where you connected the event function.");

			static function<void(arg_reference)> build(Ret(*fn)(Arg))
			{
				return[fn](arg_reference arg){
					fn(arg);
				};
			}
		};
	};
#endif

#if 0
    /// provides some fundamental events that every widget owns.
	struct general_events
	{
		basic_event<arg_mouse> mouse_enter;	///< the cursor enters the window
		basic_event<arg_mouse> mouse_move;	///< the cursor moves on the window
		basic_event<arg_mouse> mouse_leave;	///< the cursor leaves the window
		basic_event<arg_mouse> mouse_down;	///< the user presses the mouse button
		basic_event<arg_mouse> mouse_up;	///< the user presses the mouse button
		basic_event<arg_click> click;		///< the window is clicked, but occurs after mouse_down and before mouse_up
		basic_event<arg_mouse> dbl_click;	///< the window is double clicked
		basic_event<arg_wheel> mouse_wheel;	///< the mouse wheel rotates while the window has focus
		basic_event<arg_dropfiles>	mouse_dropfiles; ///< the mouse drops some external data while the window enable accepting files
		basic_event<arg_expose>	expose;		///< the visibility changes
		basic_event<arg_focus>	focus;		///< the window receives or loses keyboard focus
		basic_event<arg_keyboard>	key_press;   ///< a key is pressed while the window has focus. event code is event_code_key_press
		basic_event<arg_keyboard>	key_release; ///< a key is released while the window has focus. event code is event_code_key_release
		basic_event<arg_keyboard>	key_char;	///< a character, whitespace or backspace is pressed. event code is event_code_key_char
		basic_event<arg_keyboard>	shortkey;	///< a defined short key is pressed. event code is event_code_shortkey

		basic_event<arg_move>		move;		///< the window changes position
		basic_event<arg_resizing>	resizing;	///< the window is changing its size
		basic_event<arg_resized>	resized;	///< the window is changing its size

		basic_event<arg_destroy>	destroy;	///< the window is destroyed, but occurs when all children have been destroyed
	};

	namespace detail
	{
		struct events_root_extension
			: public general_events
		{
			basic_event<arg_unload>	unload;
		};
	}//end namespace detail
#endif

}//end namespace nana



#endif
