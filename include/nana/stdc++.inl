/**
*	Standard Library for C++11/14/17
*	Nana C++ Library(http://www.nanapro.org)
*	Copyright(C) 2017 Jinhao(cnjinhao@hotmail.com)
*
*	Distributed under the Boost Software License, Version 1.0.
*	(See accompanying file LICENSE_1_0.txt or copy at
*	http://www.boost.org/LICENSE_1_0.txt)
*
*	@file  nana/stdc++.cpp
*/

#include <nana/stdc++.hpp>

//Implement workarounds for GCC/MinGW which version is below 4.8.2
#if defined(STD_NUMERIC_CONVERSIONS_NOT_SUPPORTED)
#include <sstream>
#include <cstdlib>
#include <stdexcept>
namespace std
{
	int stoi(const char* str, size_t * pos, int base)
	{
		auto sptr = str;
		char *end;
		errno = 0;
		auto result = strtol(sptr, &end, base);

		if (sptr == end)
			throw invalid_argument("invalid stoi argument");
		if (errno == ERANGE)
			throw out_of_range("stoi argument out of range");

		if (pos)
			*pos = (size_t)(end - sptr);
		return ((int)result);
	}

	int stoi(const wchar* str, size_t* pos, int base)
	{
		auto sptr = str.data();
		wchar_t *end;
		errno = 0;
		auto result = wcstol(sptr, &end, base);

		if (sptr == end)
			throw invalid_argument("invalid stoi argument");
		if (errno == ERANGE)
			throw out_of_range("stoi argument out of range");

		if (pos)
			*pos = (size_t)(end - sptr);
		return ((int)result);
	}
	using ::strtof;
	using ::strtold;
	using ::wcstold;
	using ::strtoll;
	using ::wcstoll;
	using ::strtoull;
	using ::wcstoull;

	float stof(const char* str, size_t * pos)
	{
		auto *ptr = str.data();
		errno = 0;
		char *end;
		auto result = strtof(ptr, &end);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stof argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	float stof(const wchar* str, size_t* pos)
	{
		auto *ptr = str.data();
		errno = 0;
		wchar_t *end;
		auto result = wcstof(ptr, &end);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stof argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	double stod(const char* str, size_t * pos)
	{
		auto *ptr = str.data();
		errno = 0;
		char *end;
		auto result = strtod(ptr, &end);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stod argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	double stod(const wchar* str, size_t* pos)
	{
		auto *ptr = str.data();
		errno = 0;
		wchar_t *end;
		auto result = wcstod(ptr, &end);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stod argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	long double stold(const char* str, size_t * pos)
	{
		auto *ptr = str.data();
		errno = 0;
		char *end;
		auto result = strtold(ptr, &end);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stold argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	long double stold(const wchar* str, size_t* pos)
	{
		auto *ptr = str.data();
		errno = 0;
		wchar_t *end;
		auto result = wcstold(ptr, &end);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stold argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	long stol(const char* str, size_t* pos, int base)
	{
		auto *ptr = str.data();
		errno = 0;
		char *end;
		auto result = strtol(ptr, &end, base);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stol argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	long stol(const wchar* str, size_t* pos, int base)
	{
		auto *ptr = str.data();
		errno = 0;
		wchar_t *end;
		auto result = wcstol(ptr, &end, base);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stol argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	//Workaround for no implemenation of stoll in MinGW.
	long long stoll(const char* str, size_t* pos, int base)
	{
		auto *ptr = str.data();
		errno = 0;
		char* end;
		auto result = strtoll(ptr, &end, base);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stoll argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	long long stoll(const wchar* str, size_t* pos, int base)
	{
		auto *ptr = str.data();
		errno = 0;
		wchar_t* end;
		auto result = wcstoll(ptr, &end, base);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stoll argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	unsigned long long stoull(const char* str, size_t* pos, int base)
	{
		auto *ptr = str.data();
		errno = 0;
		char* end;
		auto result = strtoull(ptr, &end, base);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stoull argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	unsigned long long stoull(const wchar* str, size_t* pos, int base)
	{
		auto *ptr = str.data();
		errno = 0;
		wchar_t* end;
		auto result = wcstoull(ptr, &end, base);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stoull argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	//Workaround for no implemenation of stoul in MinGW.
	unsigned long stoul(const char* str, size_t* pos, int base)
	{
		auto *ptr = str.data();
		errno = 0;
		char* end;
		auto result = strtoul(ptr, &end, base);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stoul argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}

	unsigned long stoul(const wchar* str, size_t* pos, int base)
	{
		auto *ptr = str.data();
		errno = 0;
		wchar_t* end;
		auto result = wcstoul(ptr, &end, base);

		if (ptr == end)
			throw invalid_argument("invalid stod argument");
		if (errno == ERANGE)
			throw out_of_range("stoul argument out of range");
		if (pos)
			*pos = (size_t)(end - ptr);
		return result;
	}
}//end namespace std
#endif //STD_NUMERIC_CONVERSIONS_NOT_SUPPORTED

#ifdef STD_TO_STRING_NOT_SUPPORTED
#include <sstream>
namespace std
{
	string to_string(double v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	string to_string(long double v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	string to_string(unsigned v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	string to_string(int v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	string to_string(long v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	string to_string(unsigned long v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	string to_string(long long v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	string to_string(unsigned long long v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	string to_string(float v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}
}
#endif // STD_TO_STRING_NOT_SUPPORTED

#ifdef STD_TO_WSTRING_NOT_SUPPORTED
#include <sstream>
namespace std
{
	wstring to_wstring(double v)
	{
		wstringstream ss;
		ss << v;
		return ss.str();
	}

	wstring to_wstring(long double v)
	{
		wstringstream ss;
		ss << v;
		return ss.str();
	}

	wstring to_wstring(unsigned v)
	{
		wstringstream ss;
		ss << v;
		return ss.str();
	}

	wstring to_wstring(int v)
	{
		wstringstream ss;
		ss << v;
		return ss.str();
	}

	wstring to_wstring(long v)
	{
		wstringstream ss;
		ss << v;
		return ss.str();
	}

	wstring to_wstring(unsigned long v)
	{
		wstringstream ss;
		ss << v;
		return ss.str();
	}

	wstring to_wstring(long long v)
	{
		wstringstream ss;
		ss << v;
		return ss.str();
	}

	wstring to_wstring(unsigned long long v)
	{
		wstringstream ss;
		ss << v;
		return ss.str();
	}

	wstring to_wstring(float v)
	{
		wstringstream ss;
		ss << v;
		return ss.str();
	}
}
#endif

#ifdef _enable_std_put_time
#include <cwchar>
namespace std
{
	//Workaround for no implemenation of put_time in gcc < 5.
	/* std unspecified return type */
	//template< class CharT, class RTSTR >// let fail for CharT != char / wchar_t
	//RTSTR put_time(const tm* tmb, const CharT* fmt);

	//template<   >
	string put_time/*<char, string>*/(const tm* tmb, const char* fmt)
	{
		size_t sz = 200;
		string str(sz, '\0');
		sz = strftime(&str[0], str.size() - 1, fmt, tmb);
		str.resize(sz);
		return str;
	}
	//Defined in header <ctime>
	//	size_t strftime(char* str, size_t count, const char* format, const tm* time);
	//template<>
	//wstring put_time<wchar_t, wstring>(const tm* tmb, const wchar_t* fmt)
	//{
	//	unsigned sz = 200;
	//	wstring str(sz, L'\0');
	//	sz = wcsftime(&str[0], str.size() - 1, fmt, tmb);
	//	str.resize(sz);
	//	return str;
	//}
	// http://en.cppreference.com/w/cpp/chrono/c/wcsftime
	// Defined in header <cwchar>
	//	size_t wcsftime(wchar_t* str, size_t count, const wchar_t* format, const tm* time);
	// Converts the date and time information from a given calendar time time to a null - terminated
	// wide character string str according to format string format.Up to count bytes are written.
	//	Parameters
	//	str - pointer to the first element of the wchar_t array for output
	//	count - maximum number of wide characters to write
	//	format - pointer to a null - terminated wide character string specifying the format of conversion.

}
#endif  // _enable_std_put_time
