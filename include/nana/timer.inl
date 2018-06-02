/*
 *	A Timer Implementation
 *  Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2014 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/timer.hpp
 *	@description:
 *		A timer can repeatedly call a piece of code. The duration between
 *	calls is specified in milliseconds. Timer is defferent from other graphics
 *	controls, it has no graphics interface.
 *	@contributors: Benjamin Navarro(pr#81)
 */
#include <nana/deploy.hpp>
#include <nana/gui/timer.hpp>
#include <map>


#if defined(STD_THREAD_NOT_SUPPORTED)
    #include <nana/std_mutex.hpp>
#else
    #include <mutex>
#endif

#if defined(NANA_WINDOWS)
#include <windows.h>
#elif defined(NANA_LINUX) || defined(NANA_MACOS)
#include "../detail/platform_spec_selector.hpp"
#include <nana/system/platform.hpp>
#endif

namespace nana
{
    class timer_core;
#if defined(NANA_WINDOWS)
	typedef UINT_PTR timer_identifier;
#else
	typedef timer_core* timer_identifier;
#endif

	class timer_driver
	{
		typedef lock_guard<recursive_mutex> lock_guard;

		friend class timer_core;

		timer_driver() = default;
	public:
		static timer_driver& instance()
		{
			static timer_driver obj;
			return obj;
		}

		template<typename Factory>
		timer_core* create(unsigned ms, Factory && factory)
		{
#if defined(NANA_WINDOWS)
			auto tmid = ::SetTimer(NULL, 0, ms, &timer_driver::_m_timer_proc);
#endif
			try
			{
#if defined(NANA_WINDOWS)
				auto p = factory(tmid);
#else
				auto p = factory();
				auto tmid = p;
				platform_spec::instance().set_timer(reinterpret_cast<size_t>(tmid), ms, &timer_driver::_m_timer_proc);
#endif
				lock_guard lock(mutex_);
				timer_table_[tmid].reset(p);
				return p;
			}
			catch (...)
			{
			}
			return NULL;
		}

		void destroy(timer_identifier tid)
		{
			lock_guard lock(mutex_);
			auto i = timer_table_.find(tid);
			if (i != timer_table_.end())
			{
#if defined(NANA_WINDOWS)
				::KillTimer(NULL, tid);
#else
				platform_spec::instance().kill_timer(reinterpret_cast<size_t>(tid));
#endif
				timer_table_.erase(i);
			}
		}
	private:
#if defined(NANA_WINDOWS)
		static void __stdcall _m_timer_proc(HWND hwnd, UINT uMsg, UINT_PTR id, DWORD dwTime);
#else
		static void _m_timer_proc(size_t id);
#endif
	private:
		recursive_mutex	mutex_;
		map<timer_identifier, unique_ptr<timer_core>>	timer_table_;
	};

	class timer_core
	{
	public:
#if defined(NANA_WINDOWS)
		timer_core(timer_identifier tmid, basic_event<arg_elapse>& evt_elapse)
			: timer_(tmid), evt_elapse_(evt_elapse)
		{}
#else
		timer_core(basic_event<arg_elapse>& evt_elapse)
			: timer_(this), evt_elapse_(evt_elapse)
		{}
#endif

		timer_identifier id() const
		{
			return timer_;
		}

		void interval(unsigned ms)
		{
#if defined(NANA_WINDOWS)
			::SetTimer(NULL, timer_, ms, &timer_driver::_m_timer_proc);
#else
			platform_spec::instance().set_timer(reinterpret_cast<size_t>(timer_), ms, &timer_driver::_m_timer_proc);
#endif
		}

		void emit(const arg_elapse& arg)
		{
			evt_elapse_.emit(arg, NULL);
		}
	private:
		const timer_identifier timer_;
		basic_event<arg_elapse> & evt_elapse_;
	}; //end class timer_core

#if defined(NANA_WINDOWS)
	void __stdcall timer_driver::_m_timer_proc(HWND /*hwnd*/, UINT /*uMsg*/, UINT_PTR id, DWORD /*dwTime*/)
#else
	void timer_driver::_m_timer_proc(size_t id)
#endif
	{
		auto & driver = instance();

		lock_guard lock(driver.mutex_);
#if defined(NANA_WINDOWS)
		auto i = driver.timer_table_.find(id);
#else
		auto i = driver.timer_table_.find(reinterpret_cast<timer_identifier>(id));
#endif
		if (i == driver.timer_table_.end())
			return;

		arg_elapse arg;
		arg.id = id;
		i->second->emit(arg);
	}

	struct timer::implement
	{
		unsigned interval		= 1000; //Defaultly 1 second.
		timer_core * tm_core	= NULL;
	};

	//class timer
		timer::timer()
			: impl_(new implement)
		{
		}

		timer::~timer()
		{
			if (impl_->tm_core)
				timer_driver::instance().destroy(impl_->tm_core->id());
			delete impl_;
		}

		void timer::reset()
		{
			stop();
			elapse_.clear();
		}

		void timer::start()
		{
			if (impl_->tm_core)
				return;
#if defined(NANA_WINDOWS)
			impl_->tm_core = timer_driver::instance().create(impl_->interval, [this](timer_identifier id)
			{
				return new timer_core(id, elapse_);
			});
#else
			impl_->tm_core = timer_driver::instance().create(impl_->interval, [this]
			{
				return new timer_core(elapse_);
			});
#endif
		}

		bool timer::started() const
		{
			return (NULL != impl_->tm_core);
		}

		void timer::stop()
		{
			if (NULL == impl_->tm_core)
				return;

			auto tmid = impl_->tm_core->id();
			impl_->tm_core = NULL;
			timer_driver::instance().destroy(tmid);
		}

		void timer::interval(unsigned ms)
		{
			if (ms != impl_->interval)
			{
				impl_->interval = ms;
				if (impl_->tm_core)
					impl_->tm_core->interval(ms);
			}
		}

		unsigned timer::interval() const
		{
			return impl_->interval;
		}
	//end class timer
}//end namespace nana
