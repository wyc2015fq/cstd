/*
 *	A textbase class implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/widgets/skeletons/textbase.hpp
 *	@description: This class manages the multi-line text and provides some operation on text
 */

#ifndef NANA_GUI_WIDGET_DETAIL_TEXTBASE_HPP
#define NANA_GUI_WIDGET_DETAIL_TEXTBASE_HPP


#include <nana/charset.hpp>
#include <nana/>
#include <nana/traits.hpp>
#include "textbase_export_interface.hpp"

#include <deque>

#include <fstream>
#include <stdexcept>

namespace nana
{
namespace widgets
{
namespace skeletons
{
	template<typename CharT>
	class textbase
		: public noncopyable
	{
	public:
		typedef CharT						char_type;
		typedef basic_string<CharT>	string_type;
		typedef typename string_type::size_type	size_type;

		textbase()
		{
			attr_max_.reset();
			//Insert an empty string for the first line of empty text.
			text_cont_.emplace_back(new string_type);
		}

		void set_event_agent(textbase_event_agent_interface * evt)
		{
			evt_agent_ = evt;
		}

		bool empty() const
		{
			return (text_cont_.empty() ||
					((text_cont_.size() == 1) && (text_cont_.front()->empty())));
		}

		bool load(const char* file_utf8)
		{
			if (!file_utf8)
				return false;

			ifstream ifs(to_osmbstr(file_utf8));
			if (!ifs)
				return false;

			ifs.seekg(0, ios::end);
			size_t bytes = static_cast<size_t>(ifs.tellg());
			ifs.seekg(0, ios::beg);

			if(bytes >= 2)
			{
				int ch = ifs.get();
				if(0xEF == ch && bytes >= 3)
				{
					//UTF8
					ch = ifs.get();
					if(0xBB == ch && 0xBF == ifs.get())
					{
						ifs.close();
						return load(file_utf8, unicode::utf8);
					}
				}
				else if(0xFF == ch)
				{
					if(0xFE == ifs.get())
					{
						//UTF16,UTF32
						if(bytes >= 4)
						{
							if(ifs.get() == 0 && ifs.get() == 0)
							{
								ifs.close();
								return load(file_utf8, unicode::utf32);
							}
						}
						ifs.close();
						return load(file_utf8, unicode::utf16);
					}
				}
				else if(0xFE == ch)
				{
					if(ifs.get() == 0xFF)
					{
						//UTF16(big-endian)
						ifs.close();
						return load(file_utf8, unicode::utf16);
					}
				}
				else if(0 == ch)
				{
					if(bytes >= 4 && ifs.get() == 0)
					{
						ch = ifs.get();
						if(0xFE == ch && ifs.get() == 0xFF)
						{
							//UTF32(big_endian)
							ifs.close();
							return load(file_utf8, unicode::utf32);
						}
					}
				}
			}

			ifs.clear();
			ifs.seekg(0, ios::beg);

			text_cont_.clear();		//Clear only if the file can be opened.
			attr_max_.reset();

			string str_mbs;
			while(ifs.good())
			{
				getline(ifs, str_mbs);
				text_cont_.emplace_back(new string_type(static_cast<string_type&&>(charset{ str_mbs })));
				if(text_cont_.back()->size() > attr_max_.size)
				{
					attr_max_.size = text_cont_.back()->size();
					attr_max_.line = text_cont_.size() - 1;
				}
			}

			_m_saved(file_utf8);
			return true;
		}

		static void byte_order_translate_2bytes(char* str)
		{
			char * s = const_cast<char*>(str);
			char * end = s + str.size();
			for(; s < end; s += 2)
			{
				char c = *s;
				*s = *(s + 1);
				*(s + 1) = c;
			}
		}

		static void byte_order_translate_4bytes(char* str)
		{
			char * s = const_cast<char*>(str);
			char * end = s + str.size();
			for(; s < end; s += 4)
			{
				char c = *s;
				*s = *(s + 3);
				*(s + 3) = c;

				c = *(s + 1);
				*(s + 1) = *(s + 2);
				*(s + 2) = c;
			}
		}

		bool load(const char* file_utf8, unicode encoding)
		{
			if (!file_utf8)
				return false;

			ifstream ifs(to_osmbstr(file_utf8));

			if (!ifs)
				return false;

			string str;
			bool big_endian = true;

			if(ifs.good())
			{
				text_cont_.clear();		//Clear only if the file can be opened.
				attr_max_.reset();

				getline(ifs, str);

				size_t len_of_BOM = 0;
				switch(encoding)
				{
				case unicode::utf8:
					len_of_BOM = 3;	break;
				case unicode::utf16:
					len_of_BOM = 2;	break;
				case unicode::utf32:
					len_of_BOM = 4;	break;
				default:
					throw runtime_error("Specified a wrong UTF");
				}

				big_endian = (str[0] == 0x00 || str[0] == char(0xFE));
				str.erase(0, len_of_BOM);
				if(big_endian)
				{
					if(unicode::utf16 == encoding)
						byte_order_translate_2bytes(str);
					else
						byte_order_translate_4bytes(str);
				}

				text_cont_.emplace_back(new string_type(static_cast<string_type&&>(charset{ str, encoding })));

				attr_max_.size = text_cont_.back()->size();
				attr_max_.line = 0;
			}

			while(ifs.good())
			{
				getline(ifs, str);

				if(big_endian)
				{
					if(unicode::utf16 == encoding)
						byte_order_translate_2bytes(str);
					else
						byte_order_translate_4bytes(str);
				}

				text_cont_.emplace_back(new string_type(static_cast<string_type&&>(charset{ str, encoding })));
				if(text_cont_.back()->size() > attr_max_.size)
				{
					attr_max_.size = text_cont_.back()->size();
					attr_max_.line = text_cont_.size() - 1;
				}
			}

			_m_saved(file_utf8);
			return true;
		}

		void store(string fs, bool is_unicode, unicode encoding) const
		{
			ofstream ofs(to_osmbstr(fs), ios::binary);
			if(ofs && text_cont_.size())
			{
				auto i = text_cont_.cbegin();
				auto const count = text_cont_.size() - 1;

				string last_mbs;

				if (is_unicode)
				{
					const char * le_boms[] = { "\xEF\xBB\xBF", "\xFF\xFE", "\xFF\xFE\x0\x0" };	//BOM for little-endian
					int bytes = 0;
					switch (encoding)
					{
					case unicode::utf8:
						bytes = 3;	break;
					case unicode::utf16:
						bytes = 2;	break;
					case unicode::utf32:
						bytes = 4;	break;
					}

					if (bytes)
						ofs.write(le_boms[static_cast<int>(encoding)], bytes);

					for (size_t pos = 0; pos < count; ++pos)
					{
						auto mbs = charset(**(i++)).to_bytes(encoding);
						ofs.write(mbs, static_cast<streamsize>(mbs.size()));
						ofs.write("\r\n", 2);
					}

					last_mbs = charset(*text_cont_.back()).to_bytes(encoding);
				}
				else
				{
					for (size_t pos = 0; pos < count; ++pos)
					{
						string mbs = charset(**(i++));
						ofs.write(mbs, mbs.size());
						ofs.write("\r\n", 2);
					}

					last_mbs = charset(*text_cont_.back());
				}

				ofs.write(last_mbs, static_cast<streamsize>(last_mbs.size()));
				_m_saved(move(fs));
			}
		}

		size_type lines() const
		{
			return text_cont_.size();
		}

		const string_type& getline(size_type pos) const
		{
			if (pos < text_cont_.size())
				return *text_cont_[pos];

			return nullstr_;
		}

		pair<size_t, size_t> max_line() const
		{
			return make_pair(attr_max_.line, attr_max_.size);
		}
	public:
		void replace(size_type pos, string_type && text)
		{
			if (text_cont_.size() <= pos)
			{
				text_cont_.emplace_back(new string_type(move(text)));
				pos = text_cont_.size() - 1;
			}
			else
				_m_at(pos).swap(text);

			_m_make_max(pos);
			_m_edited();
		}

		void insert(upoint pos, string_type && str)
		{
			if(pos.y < text_cont_.size())
			{
				string_type& lnstr = _m_at(pos.y);

				if(pos.x < lnstr.size())
					lnstr.insert(pos.x, str);
				else
					lnstr += str;
			}
			else
			{
				text_cont_.emplace_back(new string_type(move(str)));
				pos.y = static_cast<unsigned>(text_cont_.size() - 1);
			}

			_m_make_max(pos.y);
			_m_edited();
		}

		void insertln(size_type pos, string_type&& str)
		{
			if (pos < text_cont_.size())
				text_cont_.emplace(_m_iat(pos), new string_type(move(str)));
			else
				text_cont_.emplace_back(new string_type(move(str)));

			_m_make_max(pos);
			_m_edited();
		}

		void erase(size_type line, size_type pos, size_type count)
		{
			if (line < text_cont_.size())
			{
				string_type& lnstr = _m_at(line);
				if ((pos == 0) && (count >= lnstr.size()))
					lnstr.clear();
				else
					lnstr.erase(pos, count);

				if (attr_max_.line == line)
					_m_scan_for_max();

				_m_edited();
			}
		}

		bool erase(size_type pos, size_t n)
		{
			//Bounds checking
			if ((pos >= text_cont_.size()) || (0 == n))
				return false;
			
			if (pos + n > text_cont_.size())
				n = text_cont_.size() - pos;

			text_cont_.erase(_m_iat(pos), _m_iat(pos + n));

			if (pos <= attr_max_.line && attr_max_.line < pos + n)
				_m_scan_for_max();
			else if (pos < attr_max_.line)
				attr_max_.line -= n;

			_m_edited();
			return true;
		}

		void erase_all()
		{
			text_cont_.clear();
			attr_max_.reset();
			text_cont_.emplace_back(new string_type);	//text_cont_ must not be empty

			_m_saved(string());
		}

		void merge(size_type pos)
		{
			if(pos + 1 < text_cont_.size())
			{
				auto i = _m_iat(pos + 1);
				_m_at(pos) += **i;
				text_cont_.erase(i);
				_m_make_max(pos);

				//If the maxline is behind the pos line,
				//decrease the maxline. Because a line between maxline and pos line
				//has been deleted.
				if(pos < attr_max_.line)
					--attr_max_.line;

				_m_edited();
			}
		}

		const char* filename() const
		{
			return filename_;
		}

		bool edited() const
		{
			return changed_;
		}

		void edited_reset()
		{
			changed_ = false;
		}

		void reset()
		{
			filename_.clear();
			changed_ = false;
		}

		bool saved() const
		{
			return ! not_saved();
		}

		bool not_saved() const
		{
			return edited() || filename_.empty();
		}
	private:
		string_type& _m_at(size_type pos)
		{
			return **_m_iat(pos);
		}

		typename deque<unique_ptr<string_type>>::iterator _m_iat(size_type pos)
		{
			return text_cont_.begin() + pos;
		}

		void _m_make_max(size_t pos)
		{
			const string_type& str = _m_at(pos);
			if(str.size() > attr_max_.size)
			{
				attr_max_.size = str.size();
				attr_max_.line = pos;
			}
		}

		void _m_scan_for_max()
		{
			attr_max_.size = 0;
			size_t n = 0;
			for(auto & p : text_cont_)
			{
				if(p->size() > attr_max_.size)
				{
					attr_max_.size = p->size();
					attr_max_.line = n;
				}
				++n;
			}
		}

		void _m_first_change() const
		{
			if (evt_agent_)
				evt_agent_->first_change();
		}

		void _m_saved(string && filename) const
		{
			if(filename_ != filename)
			{
				filename_ = move(filename);
				_m_first_change();
			}
			else if(changed_)
				_m_first_change();

			changed_ = false;
		}

		void _m_edited()
		{
			if(!changed_)
			{
				_m_first_change();
				changed_ = true;
			}

			if (evt_agent_)
				evt_agent_->text_changed();
		}
	private:
		deque<unique_ptr<string_type>>	text_cont_;
		textbase_event_agent_interface* evt_agent_{ NULL };

		mutable bool			changed_{ false };
		mutable string	filename_;	//A string for the saved filename.
		const string_type nullstr_;

		struct attr_max
		{
			size_t line;
			size_t size;

			void reset()
			{
				line = 0;
				size = 0;
			}
		}attr_max_;
	};

}//end namespace detail
}//end namespace widgets
}//end namespace nana


#endif
