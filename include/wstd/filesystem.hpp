

#ifndef _FILESYSTEM_HPP_
#define _FILESYSTEM_HPP_

#include <filesystem>
#include <time.h>
#include <math.h>
#include <io.h>
#include "os.hpp"
#include "string.hpp"

//using namespace 
using namespace std::experimental;
using filesystem::path;

namespace wstd {
  namespace filesystem {
    static path unique_path(const path& model) {
      std::wstring s(model.wstring());  // std::string ng for MBCS encoded POSIX
      const wchar_t hex[] = L"0123456789abcdef";
      char ran[] = "123456789abcdef";  // init to avoid clang static analyzer message
                                       // see ticket #8954
      assert(sizeof(ran) == 16);
      srand((unsigned int)time(NULL));
      for (std::wstring::size_type i = 0; i < s.size(); ++i)
      {
        if (s[i] == L'%')                        // digit request
        {
          int c = rand();
          s[i] = hex[c & 0xf];             // convert to hex digit and replace
        }
      }

      return s;
    }
  }

  static int readlines(const char* testfile, std::vector<string>& strs) {
    FILE* pf = NULL;
    pf = fopen(testfile, "rb");
    if (pf) {
      char buf[1024];
      for (; fgets(buf, 1024, pf) > 0; ) {
        string str = trim(buf, "\r\n");
        strs.push_back(str);
      }
      fclose(pf);
    }
    return (int)strs.size();
  }

}

/*
For Windows
value	mode
00		Existence only
02		Write-only
04		Read-only
06		Read and write

For linux
F_OK	Existence only
R_OK	Read-only
W_OK	Write-only
X_OK	Executable
*/
static bool CheckFileExist(const char* szFile)
{
#ifdef WIN32
  return _access(szFile, 0) != -1;
#else
  return access(szFile, F_OK) != -1;
#endif

}

#endif // _FILESYSTEM_HPP_
