
#include <filesystem>
#include <time.h>
#include <math.h>

//using namespace 
using namespace std::experimental;

namespace wstd {
  using filesystem::path;
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

}