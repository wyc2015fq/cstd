
#ifndef _TCHAR_EX_
#define _TCHAR_EX_

//#include <tchar.h>
#include "tchar_x.h"
#include <string>
#include <sstream>
#include <fstream>

#if defined UNICODE || defined _UNICODE

typedef std::wstring         tstring;
typedef std::wstringbuf      tstringbuf;
typedef std::wstringstream   tstringstream;
typedef std::wostringstream  tostringstream;
typedef std::wistringstream  tistringstream;
typedef std::wstreambuf      tstreambuf;
typedef std::wistream        tistream;
typedef std::wiostream       tiostream;
typedef std::wostream        tostream;
typedef std::wfilebuf        tfilebuf;
typedef std::wfstream        tfstream;
typedef std::wifstream       tifstream;
typedef std::wofstream       tofstream;
typedef std::wios            tios;
#   define tcerr                 std::wcerr
#   define tcin                  std::wcin
#   define tclog                 std::wclog
#   define tcout                 std::wcout

#else // defined UNICODE || defined _UNICODE

typedef std::string          tstring;
typedef std::stringbuf       tstringbuf;
typedef std::stringstream    tstringstream;
typedef std::ostringstream   tostringstream;
typedef std::istringstream   tistringstream;
typedef std::streambuf       tstreambuf;
typedef std::istream         tistream;
typedef std::iostream        tiostream;
typedef std::ostream         tostream;
typedef std::filebuf         tfilebuf;
typedef std::fstream         tfstream;
typedef std::ifstream        tifstream;
typedef std::ofstream        tofstream;
typedef std::ios             tios;
#   define tcerr                 std::cerr
#   define tcin                  std::cin
#   define tclog                 std::clog
#   define tcout                 std::cout

#endif // defined UNICODE || defined _UNICODE

#endif