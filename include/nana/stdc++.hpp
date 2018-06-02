/**
 *	Standard Library for C++11/14/17
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file  nana/stdc++.hpp
 *
 *	@brief Implement the lack support of standard library.
 */

#ifndef NANA_STDCXX_INCLUDED
#define NANA_STDCXX_INCLUDED

#include "c++defines.hpp"

//Implement workarounds for GCC/MinGW which version is below 4.8.2
#if defined(STD_NUMERIC_CONVERSIONS_NOT_SUPPORTED)
namespace std
{
	//Workaround for no implemenation of stoi in MinGW.
	int stoi(const char*, size_t * pos = NULL, int base = 10);
	int stoi(const wchar*, size_t* pos = NULL, int base = 10);

	//Workaround for no implemenation of stof in MinGW.
	float stof(const char*, size_t * pos = NULL);
	float stof(const wchar*, size_t* pos = NULL);

	//Workaround for no implemenation of stod in MinGW.
	double stod(const char*, size_t * pos = NULL);
	double stod(const wchar*, size_t* pos = NULL);

	//Workaround for no implemenation of stold in MinGW.
	long double stold(const char*, size_t * pos = NULL);
	long double stold(const wchar*, size_t* pos = NULL);

	//Workaround for no implemenation of stol in MinGW.
	long stol(const char*, size_t* pos = NULL, int base = 10);
	long stol(const wchar*, size_t* pos = NULL, int base = 10);

	//Workaround for no implemenation of stoll in MinGW.
	long long stoll(const char*, size_t* pos = NULL, int base = 10);
	long long stoll(const wchar*, size_t* pos = NULL, int base = 10);

	//Workaround for no implemenation of stoul in MinGW.
	unsigned long stoul(const char*, size_t* pos = NULL, int base = 10);
	unsigned long stoul(const wchar*, size_t* pos = NULL, int base = 10);

	//Workaround for no implemenation of stoull in MinGW.
	unsigned long long stoull(const char*, size_t* pos = NULL, int base = 10);
	unsigned long long stoull(const wchar*, size_t* pos = NULL, int base = 10);
}
#endif //STD_NUMERIC_CONVERSIONS_NOT_SUPPORTED

#ifdef STD_TO_STRING_NOT_SUPPORTED
namespace std
{
	//Workaround for no implemenation of to_string/to_wstring in MinGW.
	string to_string(long double);
	string to_string(double);
	string to_string(unsigned);
	string to_string(int);
	string to_string(long);
	string to_string(unsigned long);
	string to_string(long long);
	string to_string(unsigned long long);
	string to_string(float);
}
#endif

#ifdef STD_TO_WSTRING_NOT_SUPPORTED
namespace std
{
	wstring to_wstring(long double);
	wstring to_wstring(double);
	wstring to_wstring(unsigned);
	wstring to_wstring(int);
	wstring to_wstring(long);
	wstring to_wstring(unsigned long);
	wstring to_wstring(long long);
	wstring to_wstring(unsigned long long);
	wstring to_wstring(float);
}
#endif

#ifdef _enable_std_make_unique
	// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3656.htm

#include <cstddef>

#include <type_traits>
#include <utility>

namespace std {
	template<class T> struct _Unique_if {
		typedef unique_ptr<T> _Single_object;
	};

	template<class T> struct _Unique_if<T[]> {
		typedef unique_ptr<T[]> _Unknown_bound;
	};

	template<class T, size_t N> struct _Unique_if<T[N]> {
		typedef void _Known_bound;
	};

	template<class T, class... Args>
	typename _Unique_if<T>::_Single_object
		make_unique(Args&&... args) {
		return unique_ptr<T>(new T(forward<Args>(args)...));
	}

	template<class T>
	typename _Unique_if<T>::_Unknown_bound
		make_unique(size_t n) {
		typedef typename remove_extent<T>::type U;
		return unique_ptr<T>(new U[n]());
	}

	template<class T, class... Args>
	typename _Unique_if<T>::_Known_bound
		make_unique(Args&&...) {}
}
#endif //_enable_std_make_unique

#ifdef _enable_std_put_time
#include <ctime>
#include <string>
namespace std
{
	//Workaround for no implemenation of put_time in gcc < 5.
	/* std unspecified return type */
	//template< class CharT, class RTSTR >// let fail for CharT != char / wchar_t
	//RTSTR put_time(const tm* tmb, const CharT* fmt);

	//template<   >
	string put_time/*<char, string>*/(const tm* tmb, const char* fmt);

	//Defined in header <ctime>
	//	size_t strftime(char* str, size_t count, const char* format, const tm* time);
	//template<>
	//wstring put_time<wchar_t, wstring>(const tm* tmb, const wchar_t* fmt);
}
#endif  // _enable_std_put_time

#if defined(_enable_std_clamp)
namespace std
{
	//<algorithm> since C++17
	template<typename T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		return (v < lo ? lo : (hi < v ? hi : v));
	}
}
#endif

#endif // NANA_STDCXX_INCLUDED
