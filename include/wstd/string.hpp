
#ifndef _WSTD_STRING_HPP_
#define _WSTD_STRING_HPP_


#include <varargs.h>
#include <assert.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <iomanip>
#include <sstream>

namespace wstd {

  using namespace std;
  static std::string itos(int value, int radix = 10) {
    char buf[32] = "";
    _itoa(value, buf, radix);
    return buf;
  }
  static int stoi(const std::string& str) {
    return atoi(str.c_str());
  }
  struct string_set_256 {
    unsigned char ucmap[256];
    string_set_256(const char* p) {
      memset(ucmap, 0, 256);
      for (; *p; ) { ucmap[(unsigned char)(*p++)] = 1; }
    }
    bool operator [](int index) const {
      return 1==ucmap[index&0xff];
    }
  };

  inline int trim_lindex(const char* s, int l, int r, const unsigned char* ucmap)
  {
    for (; l<r && ucmap[(unsigned char)s[l]]; l++);
    return l;
  }

  inline int trim_rindex(const char* s, int l, int r, const unsigned char* ucmap)
  {
    for (; l<r && ucmap[(unsigned char)s[r]]; --r);
    return r;
  }
  static string trim(const string& s, const char* delims)
  {
    const string_set_256 strset(delims);
    int l = trim_lindex(s.data(), 0, (int)s.size(), strset.ucmap);
    int r = trim_rindex(s.data(), 0, (int)s.size(), strset.ucmap);
    return s.substr(l, r-l);
  }

  static std::string format_int(int n, int numberOfLeadingZeros = 0)
  {
    std::ostringstream s;
    s << std::setw(numberOfLeadingZeros) << std::setfill('0') << n;
    return s.str();
  }

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
  // repeatedCharIgnored是指是否忽视待分割字符串中的重复分隔符
  static int split(vector<string>& resultStringVector, string srcStr, const std::string& delimStr, bool repeatedCharIgnored = true)
  {
    std::replace_if(srcStr.begin(), srcStr.end(), [&](const char& c) {if (delimStr.find(c) != std::string::npos) { return true; } else { return false; }}/*pred*/, delimStr.at(0));//将出现的所有分隔符都替换成为一个相同的字符（分隔符字符串的第一个）
    size_t pos = srcStr.find(delimStr.at(0));
    std::string addedString = "";
    while (pos != std::string::npos) {
      addedString = srcStr.substr(0, pos);
      if (!addedString.empty() || !repeatedCharIgnored) {
        resultStringVector.push_back(addedString);
      }
      srcStr.erase(srcStr.begin(), srcStr.begin() + pos + 1);
      pos = srcStr.find(delimStr.at(0));
    }
    addedString = srcStr;
    if (!addedString.empty() || !repeatedCharIgnored) {
      resultStringVector.push_back(addedString);
    }
    return (int)resultStringVector.size();
  }
} // namespace wstd

#endif // _WSTD_STRING_HPP_

