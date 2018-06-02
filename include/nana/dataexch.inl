/*
 *	Data Exchanger Implementation
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file:			nana/system/dataexch.cpp
 *	@description:	An implementation of a data exchange mechanism through Windows Clipboard, X11 Selection.
 */

#include <nana/system/dataexch.hpp>
#include <nana/traits.hpp>
#include <nana/paint/graphics.hpp>
#include <nana/paint/pixel_buffer.hpp>
#include <vector>
#include <cassert>
#include <cstring>

#if defined(NANA_WINDOWS)
#	include <windows.h>
#elif defined(NANA_X11)
#	include "../detail/platform_spec_selector.hpp"
#	include <nana/gui/detail/bedrock.hpp>
#	include <nana/gui/detail/basic_window.hpp>
#endif

namespace nana{ namespace system{

	//class dataexch
		void dataexch::set(const char* text, native_window_type owner)
		{
#ifdef NANA_WINDOWS
			wstring wstr = charset(text, unicode::utf8);
			_m_set(format::text, wstr, (wstr.length() + 1) * sizeof(wchar_t), owner);
#elif defined(NANA_X11)
			_m_set(format::text, text, text.length() + 1, owner);
#endif
		}


		void dataexch::set(const wchar* text, native_window_type owner)
		{
#ifdef NANA_WINDOWS
			_m_set(format::text, text, (text.length() + 1) * sizeof(wchar_t), owner);
#else
			string str = to_utf8(text);
			_m_set(format::text, str, str.size() + 1, owner);
#endif
		}

		bool dataexch::set(const graphics& g, native_window_type owner)
		{
#if defined(NANA_WINDOWS)
			size sz = g.size();
			pixel_buffer pbuffer;
			rectangle r;
			r.x = 0;
			r.y = 0;
			r.width = sz.width;
			r.height = sz.height;
			pbuffer.attach(g.handle(), r);
			size_t bytes_per_line = pbuffer.bytes_per_line();
			size_t bitmap_bytes = bytes_per_line * r.height;

			struct {
				BITMAPINFOHEADER bmiHeader;
				RGBQUAD bmiColors[256];
			} bmi = {0};
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			HDC hDC = ::GetDC(NULL);
			if (::GetDIBits(hDC, (HBITMAP)g.pixmap(), 0, 1, NULL, (BITMAPINFO *)&bmi, DIB_RGB_COLORS) == 0) {
				assert(false);
				::ReleaseDC(NULL, hDC);
				return false;
			}
			if (!::ReleaseDC(NULL, hDC)) {
				return false;
			}

			size_t header_size = sizeof(bmi.bmiHeader);

			// Bitmaps are huge, so to avoid unnegligible extra copy, this routine does not use private _m_set method.
			HGLOBAL h_gmem = ::GlobalAlloc(GHND | GMEM_SHARE, header_size + bitmap_bytes);
			void * gmem = ::GlobalLock(h_gmem);
			if (gmem)
			{
				char* p = (char*)gmem;
				// Fix BITMAPINFOHEADER obtained from GetDIBits WinAPI
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biHeight = ::abs(bmi.bmiHeader.biHeight);
				memcpy(p, &bmi, header_size);
				p += header_size;
				// many programs do not support bottom-up DIB, so reversing row order is needed.
				for (int y=0; y<bmi.bmiHeader.biHeight; ++y)
				{
					memcpy(p, pbuffer.raw_ptr(bmi.bmiHeader.biHeight - 1 - y), bytes_per_line);
					p += bytes_per_line;
				}

				if (::GlobalUnlock(h_gmem) || GetLastError() == NO_ERROR)
					if (::OpenClipboard(reinterpret_cast<HWND>(owner)))
					{
						if (::EmptyClipboard())
							if (::SetClipboardData(CF_DIB, h_gmem))
								if (::CloseClipboard())
									return true;
						::CloseClipboard();
					}
			}
			assert(false);
			::GlobalFree(h_gmem);
			return false;

//#elif defined(NANA_X11)
#else
			static_cast<void>(g); //eliminate unused parameter compiler warning.
			static_cast<void>(owner);
			throw logic_error("dataexch::set(const graphics&, native_window_type owner) not implemented yet.");

			return false;
#endif
		}


		void dataexch::get(char* text_utf8)
		{
			size_t size;
			auto res = _m_get(format::text, size);
			if (res)
			{
#if defined(NANA_WINDOWS)
				size /= sizeof(wchar_t);
				wstring wstr;
				wstr.reserve(size);
				wstr.append(reinterpret_cast<wchar_t*>(res), reinterpret_cast<wchar_t*>(res) + size);

				auto pos = wstr.find_last_not_of(L'\0');
				if (pos != wstr.npos)
					wstr.erase(pos + 1);

				text_utf8 = to_utf8(wstr);
#else
				text_utf8.reserve(size);
				text_utf8.append(reinterpret_cast<char*>(res), reinterpret_cast<char*>(res) + size);

				auto pos = text_utf8.find_last_not_of('\0');
				if (pos != text_utf8.npos)
					text_utf8.erase(pos + 1);
				::XFree(res);
#endif
			}
		}

		void dataexch::get(wchar* str)
		{
			size_t size;
			auto res = _m_get(format::text, size);
			if(res)
			{
#if defined(NANA_WINDOWS)
				str.clear();

				size /= sizeof(wchar_t);

				str.reserve(size);
				str.append(reinterpret_cast<wchar_t*>(res), reinterpret_cast<wchar_t*>(res) + size);

				auto pos = str.find_last_not_of(L'\0');
				if (pos != str.npos)
					str.erase(pos + 1);
#else
				string text_utf8;
				text_utf8.reserve(size);
				text_utf8.append(reinterpret_cast<char*>(res), reinterpret_cast<char*>(res) + size);

				auto pos = text_utf8.find_last_not_of('\0');
				if (pos != text_utf8.npos)
					text_utf8.erase(pos + 1);
				::XFree(res);

				str = to_wstring(text_utf8);
#endif
			}
		}

		wstring dataexch::wget()
		{
			wstring str;
			this->get(str);
			return str;
		}

	//private:
		bool dataexch::_m_set(format fmt, const void* buf, size_t size, native_window_type owner)
		{
			bool res = false;

#if defined(NANA_WINDOWS)
			if(::OpenClipboard(reinterpret_cast<HWND>(owner)))
			{
				if(::EmptyClipboard())
				{
					HGLOBAL g = ::GlobalAlloc(GHND | GMEM_SHARE, size);
					void * addr = ::GlobalLock(g);

					memcpy(addr, buf, size);
					::GlobalUnlock(g);

					unsigned data_format;
					switch(fmt)
					{
					case format::text:		data_format = CF_UNICODETEXT;	break;
					case format::pixmap:	data_format = CF_BITMAP;		break;
					}
					
					res = (NULL != ::SetClipboardData(data_format, g));
				}
				::CloseClipboard();
			}
#elif defined(NANA_X11)
			auto & spec = platform_spec::instance();
			
			Atom atom_type;
			switch(fmt)
			{
			case format::text:	atom_type = spec.atombase().utf8_string;	break;
			default:
				return false;
			}

			spec.write_selection(owner, atom_type, buf, size);
			return true;
			
#endif
			return res;
		}

		void* dataexch::_m_get(format fmt, size_t& size)
		{
			size = 0;
			void* res = NULL;
#if defined(NANA_WINDOWS)
			if(::OpenClipboard(::GetFocus()))
			{
				unsigned data_format;
				switch(fmt)
				{
				case format::text:		data_format = CF_UNICODETEXT;	break;
				case format::pixmap:	data_format = CF_BITMAP;	break;
				}
				HANDLE handle = ::GetClipboardData(data_format);
				if(handle)
				{
					res = reinterpret_cast<HGLOBAL>(::GlobalLock(handle));
					if(res)
						size = ::GlobalSize(handle);
				}

				::CloseClipboard();
			}
#elif defined(NANA_X11)
			platform_spec & spec = platform_spec::instance();
			native_window_type requester = NULL;
			spec.lock_xlib();

			{
				internal_scope_guard isg;
				bedrock::core_window_t * wd = bedrock::instance().focus();
				if(wd)	requester = wd->root;
			}
			spec.unlock_xlib();

			if(requester)
			{
				Atom atom;

				switch(fmt)
				{
				case format::text:	atom = spec.atombase().utf8_string;	break;
				default:
					return NULL;
				}
				res = spec.request_selection(requester, atom, size);
			}
#endif
			return res;
		}
	//end class dataexch

}//end namespace system
}//end namespace nana

