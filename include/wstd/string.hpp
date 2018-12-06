
#ifndef _WSTD_STRING_HPP_
#define _WSTD_STRING_HPP_


//#include <varargs.h>
#include <assert.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <iomanip>
#include <sstream>

#include "stdc/string_c.h"

typedef unsigned char uchar;
#ifdef _WIN32
typedef __int64 int64;
//typedef __int64 int64_t;
//typedef unsigned __int64 uint64_t;
#endif


namespace wstd {

  template <typename T, int N>
  struct bufvec {
    T buf_[N];
    T* data_;
    int size_;
    bufvec() {
      data_ = buf_, size_ = N;
    }
    bufvec(int n) {
      data_ = buf_, size_ = N;
      resize(n);
    }
    ~bufvec() {
      clear();
    }
    void clear() {
      if (data_ != buf_) {
        if (data_) free(data_);
        data_ = buf_;
      }
      size_ = N;
    }
    int size() { return size_; }
    void resize(int n) {
      if (n > size_ && n > N) {
        if (data_ == buf_) {
          data_ = (T*)malloc(n * sizeof(T));
        }
        else {
          data_ = (T*)realloc(data_, n * sizeof(T));
        }
      }
      size_ = n;
    }
    T& operator[](int i) { return data_[i]; }
    const T& operator[](int i) const { return data_[i]; }
    T* data() { return data_; }
    const T* data() const { return data_; }
    T* begin() { return data_; }
    const T* begin() const { return data_; }
    T* end() { return data_ + size_; }
    const T* end() const { return data_ + size_; }
  };
  using namespace std;
  static std::string itos(int value, int radix = 10) {
    char buf[32] = "";
    itoa_c(value, buf, radix);
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
      return 1 == ucmap[index & 0xff];
    }
  };

  inline int trim_lindex(const char* s, int l, int r, const unsigned char* ucmap)
  {
    for (; l < r && ucmap[(unsigned char)s[l]]; l++);
    return l;
  }

  inline int trim_rindex(const char* s, int l, int r, const unsigned char* ucmap)
  {
    for (; l < r && ucmap[(unsigned char)s[r - 1]]; --r);
    return r;
  }
  static string trim(const string& s, const char* delims)
  {
    const string_set_256 strset(delims);
    int l = trim_lindex(s.data(), 0, (int)s.size(), strset.ucmap);
    int r = trim_rindex(s.data(), 0, (int)s.size(), strset.ucmap);
    return s.substr(l, r - l);
  }

  static std::string format_int(int n, int numberOfLeadingZeros = 0)
  {
    std::ostringstream s;
    s << std::setw(numberOfLeadingZeros) << std::setfill('0') << n;
    return s.str();
  }

  static string format(const char* fmt, ...)
  {
    bufvec<char, 1024> buf;
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
  static int split(vector<string>& resultStringVector, const string& srcStr, const std::string& delimStr, int minLen = 0) {
    const char* s = srcStr.c_str();
    IRANGE r = iRANGE(0, srcStr.size()), b;
    uchar delims_set[256] = { 0 };
    uchar trims_set[256] = { 0 };
    get_delims_set(delims_set, delimStr.c_str());
    get_delims_set(trims_set, " \r\n\t");
    for (; split_c_(s, &r, &b, 1, delims_set, trims_set, minLen);) {
      resultStringVector.push_back(srcStr.substr(b.s, b.e - b.s));
    }
    return (int)resultStringVector.size();
  }

  static std::string strtime(const char* fmt) {
    time_t rawtime;
    tm *time1;
    char buf[256];
    time(&rawtime);
    time1 = localtime(&rawtime);
    fmt = fmt ? fmt : "%Y-%m-%d %H:%M:%S";
    strftime(buf, 256, fmt, time1);
    return buf;
  }

  inline char* strend(const char* str) {
    return (char*)str + strlen(str);
  }
  static void path_split(const char* fullpath, string* path, string* filename, string* filenameext, string* ext) {
    const char* p;
    const char *e;
    p = strrchr(fullpath, '\\');
    e = strrchr(fullpath, '/');
    p = max(p + 1, e + 1);
    p = max(p, fullpath);
    e = (e = strrchr(p, '.')) ? e : strend(p);
    if (path) path->assign(fullpath, p);
    if (filename) filename->assign(p, e);
    if (filenameext) filenameext->assign(p);
    if (ext) ext->assign(e);
  }
  static string path_split_filename(const char* fullpath) {
    string ret;
    path_split(fullpath, NULL, &ret, NULL, NULL);
    return ret;
  }
  static string path_split_filenameext(const char* fullpath) {
    string ret;
    path_split(fullpath, NULL, NULL, &ret, NULL);
    return ret;
  }
  static string path_split_ext(const char* fullpath) {
    string ret;
    path_split(fullpath, NULL, NULL, NULL, &ret);
    return ret;
  }
  static string path_fmt(const char* fullpath, const char* fmt) {
    char flag[256] = { 0 };
    string ret;
    for (const char*p = fmt; *p; ++p) {
      flag[(unsigned char)(*p)] = 1;
    }
    const char* d;
    const char* p;
    const char *e;
    d = strchr(fullpath, ':');
    d = max(fullpath, d + 1);
    p = strrchr(fullpath, '\\');
    e = strrchr(fullpath, '/');
    p = max(p + 1, e + 1);
    p = max(p, fullpath);
    e = (e = strrchr(p, '.')) ? e : strend(p);
    if (flag['d']) ret.assign(fullpath, d);
    if (flag['p']) ret += string(d, p);
    if (flag['n']) ret += string(p, e);
    if (flag['x']) ret += string(e);
    return ret;
  }

} // namespace wstd

#endif // _WSTD_STRING_HPP_

