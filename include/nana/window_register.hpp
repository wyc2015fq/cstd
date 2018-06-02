#ifndef NANA_WINDOW_REGISTER_HEADER_INCLUDED
#define NANA_WINDOW_REGISTER_HEADER_INCLUDED

#include <nana/gui/detail/basic_window.hpp>
#include <set>
#include <vector>
#include <algorithm> //find

namespace nana
{
	namespace detail
	{
		template<typename Key, typename Value, size_t CacheSize>
		class cache
			: noncopyable
		{
		public:
			typedef Key	key_type;
			typedef Value value_type;
			typedef pair<key_type, value_type> pair_type;
			typedef size_t size_type;

			cache()
				:addr_(reinterpret_cast<pair_type*>(::operator new(sizeof(pair_type) * CacheSize)))
			{
				for (size_t i = 0; i < CacheSize; ++i)
				{
					bitmap_[i] = 0;
					seq_[i] = npos;
				}
			}

			~cache()
			{
				for (size_t i = 0; i < CacheSize; ++i)
				{
					if (bitmap_[i])
						addr_[i].~pair_type();
				}

				::operator delete(addr_);
			}

			bool insert(key_type k, value_type v)
			{
				size_type pos = _m_find_key(k);
				if (pos != npos)
				{
					addr_[pos].second = v;
				}
				else
				{
					//No key exists
					pos = _m_find_pos();

					if (pos == npos)
					{	//No room, and remove the last pair
						pos = seq_[CacheSize - 1];
						(addr_ + pos)->~pair_type();
					}

					if (seq_[0] != npos)
					{//Need to move
						for (int i = CacheSize - 1; i > 0; --i)
							seq_[i] = seq_[i - 1];
					}

					seq_[0] = pos;

					new (addr_ + pos) pair_type(k, v);
					bitmap_[pos] = 1;
				}
				return v;
			}

			value_type * get(key_type k)
			{
				size_type pos = _m_find_key(k);
				if (pos != npos)
					return &(addr_[pos].second);
				return 0;
			}
		private:
			size_type _m_find_key(key_type k) const
			{
				for (size_t i = 0; i < CacheSize; ++i)
				{
					if (bitmap_[i] && (addr_[i].first == k))
						return i;
				}
				return npos;
			}

			size_type _m_find_pos() const
			{
				for (size_t i = 0; i < CacheSize; ++i)
				{
					if (bitmap_[i] == 0)
						return i;
				}
				return npos;
			}
		private:
			char bitmap_[CacheSize];
			size_type seq_[CacheSize];
			pair_type * addr_;
		};

		class window_register
		{
		public:
			using window_handle_type = basic_window*;

			~window_register()
			{
				//Deleting a basic_window if thread never called exec(), the basic_window object
				//will always stay in trash.
				//
				//Empty the trash before destructs window register
				delete_trash(0);
			}

			void insert(window_handle_type wd)
			{
				if (wd)
				{
					base_.insert(wd);
					wdcache_.insert(wd, true);

					if (category::flags::root == wd->other.category)
						queue_.push_back(wd);
				}
			}

			void operator()(window_handle_type wd)
			{
				remove(wd);
			}

			void remove(window_handle_type wd)
			{
				if (base_.erase(wd))
				{
					wdcache_.insert(wd, false);
					trash_.push_back(wd);

					if (category::flags::root == wd->other.category)
					{
						auto i = find(queue_.begin(), queue_.end(), wd);
						if (i != queue_.end())
							queue_.erase(i);
					}
				}
			}

			void delete_trash(unsigned thread_id)
			{
				if (0 == thread_id)
				{
					for (auto wd : trash_)
						delete wd;

					trash_.clear();
				}
				else
				{
					for (auto i = trash_.begin(); i != trash_.end();)
					{
						if (thread_id == (*i)->thread_id)
						{
							delete (*i);
							i = trash_.erase(i);
						}
						else
							++i;
					}
				}
			}

			const vector<window_handle_type>& queue() const
			{
				return queue_;
			}

			/// Returns the number of registered windows
			size_t size() const
			{
				return base_.size();
			}

			bool available(window_handle_type wd) const
			{
				if (NULL == wd)
					return false;

				auto exists = wdcache_.get(wd);
				if (exists)
					return *exists;

				return wdcache_.insert(wd, (base_.count(wd) != 0));
			}
		private:
			mutable cache<window_handle_type, bool, 5> wdcache_;
			set<window_handle_type> base_;
			vector<window_handle_type> trash_;
			vector<window_handle_type> queue_;
		};
	}
}

#endif
