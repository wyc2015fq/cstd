
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

namespace wstd {

  static string format(const char* fmt, ...)
  {
    vector<char> buf(1024);
    for (; ; ) {
      va_list va;
      va_start(va, fmt);
      int bsize = static_cast<int>(buf.size());
      int len = vsnprintf(buf.data(), bsize, fmt, va);
      va_end(va);
      assert(len >= 0 && "Check format string for errors");
      if (len >= bsize) {
        buf.resize(len + 1);
        continue;
      }
      buf[bsize - 1] = 0;
      return std::string(buf.data(), len);
    }
  }

}