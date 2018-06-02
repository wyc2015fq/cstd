/*
 *	Nana GUI Library Definition
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/wvl.cpp
 *	@description:
 *		the file contains the files required for running of Nana.GUI 
 */

#include <nana/gui/wvl.hpp>
#include <nana/gui/detail/bedrock.hpp>
#include <nana/std_thread.hpp>
#include <iostream> 

#ifdef STD_THREAD_NOT_SUPPORTED
#	include <boost/chrono.hpp>
#else
#	include <chrono>
#endif

//#define NANA_AUTOMATIC_GUI_TESTING
namespace nana
{
	namespace detail
	{
		void form_loader_private::insert_form(widget* p)
		{
			bedrock::instance().manage_form_loader(reinterpret_cast<basic_window*>(p->handle()), true);
		}
	}

#ifdef NANA_AUTOMATIC_GUI_TESTING

	void click(widget& w)
	{
		cout << "Automatically clicking widget "<<w.caption()<<":\n";
		arg_click arg;
		arg.window_handle = w.handle();
		w.events().click.emit(arg, w.handle());
	}

	/// in seconds
	void Wait(unsigned wait)
	{
		if (!wait) return;
		cout << "waiting " << wait << " sec...\n";
#	ifdef STD_THREAD_NOT_SUPPORTED
		boost::this_thread::sleep_for(boost::chrono::seconds{ wait });
#	else
		this_thread::sleep_for(chrono::seconds{ wait });
#	endif
	}

	void pump()
	{
		internal_scope_guard lock;
		bedrock::instance().pump_event(NULL, false);
	}


	void exec(
		unsigned wait,         // = 1,      ///< for the GUI to be constructed, in seconds  
		unsigned wait_end,     // = 1,      ///< for the GUI to be destructed, in seconds
		function<void()>f // = {}      ///< emit events to mimics user actions and may asert results
	)
	{
			
	    cout << "Will wait " << wait << " sec...\n";

	    thread t([wait, &f, wait_end]()
			{ 
				Wait( wait );
				cout << "running... \n"  ;
				if (f)
				{
					f();
					cout << "\nCongratulations, this was not trivial !" << endl;
				}else
				{
					cout << "\nJust a trivial test." << endl;
				}
				cout << "Done... \n";
				cout << "Now again ";
				Wait(wait_end);
				cout << "Done... Now API::exit all ...\n";
				API::exit_all();   
			});		

		pump();
		if (t.joinable())
			t.join();
	}
#else
	void exec()
	{
		internal_scope_guard lock;
		bedrock::instance().pump_event(NULL, false);
	}
#endif
}//end namespace nana
