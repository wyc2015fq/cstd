
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

#ifdef _WIN32
#if _MSC_VER < 1300
#define vsnprintf _vsnprintf
#endif
#endif

typedef unsigned char uchar;
#ifdef _WIN32
typedef __int64 int64;
//typedef __int64 int64_t;
//typedef unsigned __int64 uint64_t;
#endif

int my_atoi(const char *nptr)
{
  int c;              /* current char */
  int total;      /* current total */
  int sign;           /* if '-', then negative, otherwise positive */

  /* skip whitespace */
  while (isspace((int)(unsigned char)*nptr))
    ++nptr;

  c = (int)(unsigned char)*nptr++;
  sign = c;           /* save sign indication */
  if (c == '-' || c == '+')
    c = (int)(unsigned char)*nptr++;    /* skip sign */

  total = 0;

  while (isdigit(c)) {
    total = 10 * total + (c - '0');     /* accumulate digit */
    c = (int)(unsigned char)*nptr++;    /* get next char */
  }

  if (sign == '-')
    return -total;
  else
    return total;   /* return result, negated if necessary */
}

int64_t my_atoi64(const char *nptr)
{
  int c;              /* current char */
  int64_t total;      /* current total */
  int sign;           /* if '-', then negative, otherwise positive */

  /* skip whitespace */
  while (isspace((int)(unsigned char)*nptr))
    ++nptr;

  c = (int)(unsigned char)*nptr++;
  sign = c;           /* save sign indication */
  if (c == '-' || c == '+')
    c = (int)(unsigned char)*nptr++;    /* skip sign */

  total = 0;

  while (isdigit(c)) {
    total = 10 * total + (c - '0');     /* accumulate digit */
    c = (int)(unsigned char)*nptr++;    /* get next char */
  }

  if (sign == '-')
    return -total;
  else
    return total;   /* return result, negated if necessary */
}

static void my_xtoa(unsigned long val, char *buf, unsigned radix, int is_neg)
{
  char *p;                /* pointer to traverse string */
  char *firstdig;         /* pointer to first digit */
  char temp;              /* temp char */
  unsigned digval;        /* value of digit */

  p = buf;

  if (is_neg) {
    /* negative, so output '-' and negate */
    *p++ = '-';
    val = (unsigned long)(-(long)val);
  }

  firstdig = p;           /* save pointer to first digit */

  do {
    digval = (unsigned)(val % radix);
    val /= radix;       /* get next digit */

    /* convert to ascii and store */
    if (digval > 9)
      *p++ = (char)(digval - 10 + 'a');  /* a letter */
    else
      *p++ = (char)(digval + '0');       /* a digit */
  } while (val > 0);

  /* We now have the digit of the number in the buffer, but in reverse
     order.  Thus we reverse them now. */

  *p-- = '\0';            /* terminate string; p points to last digit */

  do {
    temp = *p;
    *p = *firstdig;
    *firstdig = temp;   /* swap *p and *firstdig */
    --p;
    ++firstdig;         /* advance to next two digits */
  } while (firstdig < p); /* repeat until halfway */
}

/* Actual functions just call conversion helper with neg flag set correctly,
   and return pointer to buffer. */

char * my_itoa(int val, char *buf, int radix)
{
  if (radix == 10 && val < 0)
    my_xtoa((unsigned long)val, buf, radix, 1);
  else
    my_xtoa((unsigned long)(unsigned int)val, buf, radix, 0);
  return buf;
}

char * my_ltoa(long val, char *buf, int radix)
{
  my_xtoa((unsigned long)val, buf, radix, (radix == 10 && val < 0));
  return buf;
}

char * my_ultoa(unsigned long val, char *buf, int radix)
{
  my_xtoa(val, buf, radix, 0);
  return buf;
}
/* stdcall is faster and smaller... Might as well use it for the helper. */
static void my_x64toa(uint64_t val, char *buf, unsigned radix, int is_neg)
{
  char *p;                /* pointer to traverse string */
  char *firstdig;         /* pointer to first digit */
  char temp;              /* temp char */
  unsigned digval;        /* value of digit */

  p = buf;

  if (is_neg)
  {
    *p++ = '-';         /* negative, so output '-' and negate */
    val = (uint64_t)(-(int64_t)val);
  }

  firstdig = p;           /* save pointer to first digit */

  do {
    digval = (unsigned)(val % radix);
    val /= radix;       /* get next digit */

    /* convert to ascii and store */
    if (digval > 9)
      *p++ = (char)(digval - 10 + 'a');  /* a letter */
    else
      *p++ = (char)(digval + '0');       /* a digit */
  } while (val > 0);

  /* We now have the digit of the number in the buffer, but in reverse
     order.  Thus we reverse them now. */

  *p-- = '\0';            /* terminate string; p points to last digit */

  do {
    temp = *p;
    *p = *firstdig;
    *firstdig = temp;   /* swap *p and *firstdig */
    --p;
    ++firstdig;         /* advance to next two digits */
  } while (firstdig < p); /* repeat until halfway */
}

/* Actual functions just call conversion helper with neg flag set correctly,
   and return pointer to buffer. */

char * my_i64toa(int64_t val, char *buf, int radix)
{
  my_x64toa((uint64_t)val, buf, radix, (radix == 10 && val < 0));
  return buf;
}

char * my_ui64toa(uint64_t val, char *buf, int radix)
{
  my_x64toa(val, buf, radix, 0);
  return buf;
}


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
    my_itoa(value, buf, radix);
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
#define CC_INLINE static
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
  struct IRANGE { int s, l; };
  CC_INLINE IRANGE iRANGE(int s, int l) { IRANGE r;    r.s = s, r.l = l;    return r; }
  CC_INLINE uchar* get_delims_set(uchar* delims_set, const char* delims) {
    for (; *delims;) {
      delims_set[(uchar)(*delims++)] = 1;
    }
    return delims_set;
  }
  CC_INLINE int mem_icmp(const char* s1, const char* s2, int n, int ignore_case)
  {
    static uchar map[256] = { 0 };
    static int inited = 0;
    if (!inited) {
      int i;
      for (i = 0; i < 256; ++i) {
        map[i] = ('A' <= i && i <= 'Z') ? (i - 'A' + 'a') : i;
      }
    }
    if (ignore_case) {
      while (--n && map[(uchar)*s1] == map[(uchar)*s2]) {
        s1++;
        s2++;
      }
      return(map[(uchar)* s1] - map[(uchar)* s2]);
    }
    while (--n && *s1 == *s2) {
      s1++;
      s2++;
    }
    return(*s1 - *s2);
  }
  CC_INLINE int findchr_c(const char* s, IRANGE r, int ch)
  {
    for (; r.s < r.l; ++r.s) {
      if (s[r.s] == ch) {
        return r.s;
      }
    }
    return -1;
  }
  CC_INLINE int rfindchr_c(const char* s, IRANGE r, int ch)
  {
    for (; r.s < r.l--; ) {
      if (s[r.l] == ch) {
        return r.l;
      }
    }
    return -1;
  }
  CC_INLINE int findchrs_c(const char* s, IRANGE r, const uchar* chs_set) {
    //i = i<0 ? 0 : i;
    for (; r.s < r.l; ++r.s) {
      if (chs_set[(uchar)s[r.s]]) {
        return r.s;
      }
    }
    return -1;
  }
  CC_INLINE int rfindchrs_c(const char* s, IRANGE r, const uchar* chs_set) {
    //i = i<0 ? 0 : i;
    for (; r.s < r.l--; ) {
      if (chs_set[(uchar)s[r.l]]) {
        return r.l;
      }
    }
    return -1;
  }
  CC_INLINE int findstr_c(const char* s, IRANGE r, const char* s2, int l2, int ignore_case)
  {
    int l = r.l - l2;
    for (; r.s < l; ++r.s) {
      if (0 == mem_icmp(s + r.s, s2, l2, ignore_case)) {
        return r.s;
      }
    }
    return -1;
  }
  CC_INLINE int rfindstr(const char* s, IRANGE r, const char* s2, int l2, int ignore_case)
  {
    int l = r.l - l2;
    for (; r.s < l; --l) {
      if (0 == mem_icmp(s + l, s2, l2, ignore_case)) {
        return l;
      }
    }
    return -1;
  }
  CC_INLINE int cstr_count(const char* s1, int l1, const char* s2, int l2, int ignore_case)
  {
    int i, j = 0;
    for (i = 0; i <= l1 - l2;) {
      if (0 == mem_icmp(s1 + i, s2, l2, ignore_case)) {
        ++j;
        i += l2;
      }
      else {
        ++i;
      }
    }
    return j;
  }
  CC_INLINE int replacestr(char* s1, int l1, int len, const char* s2, int l2, const char* s3, int l3, int ignore_case, int pl1, int* pj) {
    int i, j = 0, m, ret = l1;
    if (l2 >= l3) {
      for (i = 0, m = 0; i <= l1 - l2;) {
        if (0 == mem_icmp(s1 + i, s2, l2, ignore_case)) {
          memcpy(s1 + m, s3, l3);
          m += l3;
          ++j;
          i += l2;
        }
        else {
          s1[m++] = s1[i++];
        }
      }
      for (; i < l1;) {
        s1[m++] = s1[i++];
      }
      ret = (l1 + j * (l3 - l2));
    }
    else {
      if (pl1 <= 0) {
        j = cstr_count(s1, l1, s2, l2, ignore_case);
        ret = l1 + j * (l3 - l2);
        pl1 = ret;
      }
      if (ret < len && pl1 < len) {
        s1[pl1] = 0;
        for (i = l1, m = pl1; i >= l2;) {
          if (s2[0] == s1[i - l2] && 0 == mem_icmp(s1 + i - l2, s2, l2, ignore_case)) {
            m -= l3;
            memcpy(s1 + m, s3, l3);
            i -= l2;
            ++j;
          }
          else {
            s1[--m] = s1[--i];
          }
        }
        for (; i > 0;) {
          s1[--m] = s1[--i];
        }
        ret = pl1;
      }
    }
    if (pj) {
      *pj = j;
    }
    return ret;
  }

  static IRANGE trim_c(const char* s, IRANGE r, const uchar* trims_set)
  {
    for (; r.s < r.l && trims_set[(uchar)s[r.l - 1]]; r.l--);
    for (; r.s < r.l && trims_set[(uchar)s[r.s]]; r.s++);
    return r;
  }

  static int split_c_(const char* s, IRANGE* r, IRANGE* out, int maxout, const uchar* delims_set, const uchar* trims_set, bool repeatedCharIgnored = true)
  {
    int out_count = 0;
    for (; r->s < r->l && out_count < maxout;) {
      IRANGE ret = *r;
      for (; r->s < r->l; ++r->s) {
        if (delims_set[(uchar)s[r->s]]) {
          ret.l = r->s;
          ++r->s;
          break;
        }
      }
      if (trims_set) {
        ret = trim_c(s, ret, trims_set);
      }
      // save
      if (repeatedCharIgnored) {
        if (ret.s < ret.l) {
          out[out_count++] = ret;
        }
      }
      else {
        out[out_count++] = ret;
      }
    }
    return out_count;
  }
  static char* strcpy_c(char* s, IRANGE* r, const char* s1, IRANGE r1) {
    int l = r->l - r->s;
    int l1 = r1.l - r1.s;
    assert(l > l1);
    memcpy(s + r->s, s1 + r1.s, l1);
    r->s += l1;
    s[r->s] = 0;
    return s;
  }
  static char* strcpy_c1(char* s, IRANGE* r, const char* s1) {
    return strcpy_c(s, r, s1, iRANGE(0, strlen(s1)));
  }
#define GET_DELIMS_SET(nameset, str)  uchar nameset[256] = {0};uchar* p##nameset = get_delims_set(nameset, str)
  static int split_c(const char* s, IRANGE* r, IRANGE* out, int maxout, const char* delims, const char* trims, bool repeatedCharIgnored = true) {
    uchar delims_set[256] = { 0 };
    uchar trims_set[256] = { 0 };
    get_delims_set(delims_set, delims);
    if (trims) {
      get_delims_set(trims_set, " \r\n\t");
    }
    return split_c_(s, r, out, maxout, delims_set, trims ? trims_set : NULL, repeatedCharIgnored);
  }
#if _MSC_VER < 1300
  static int split(vector<string>& resultStringVector, const string& srcStr, const std::string& delimStr, bool repeatedCharIgnored = true) {
    const char* s = srcStr.c_str();
    IRANGE r = iRANGE(0, srcStr.size()), b;
    uchar delims_set[256] = { 0 };
    uchar trims_set[256] = { 0 };
    get_delims_set(delims_set, delimStr.c_str());
    get_delims_set(trims_set, " \r\n\t");
    for (; split_c_(s, &r, &b, 1, delims_set, trims_set, repeatedCharIgnored);) {
      resultStringVector.push_back(srcStr.substr(b.s, b.l - b.s));
    }
    return (int)resultStringVector.size();
  }
#else
  // repeatedCharIgnored是指是否忽视待分割字符串中的重复分隔符
  static int split(vector<string>& resultStringVector, string srcStr, const std::string& delimStr, bool repeatedCharIgnored = true)
  {
    std::replace_if(srcStr.begin(), srcStr.end(), [&](const char& c) {if (delimStr.find(c) != std::string::npos) { return true; } else { return false; }}/*pred*/, delimStr.at(0));//将出现的所有分隔符都替换成为一个相同的字符（分隔符字符串的第一个）
    size_t pos = srcStr.find(delimStr.at(0));
    std::string addedString = "";
    resultStringVector.reserve(0);
    while (pos != std::string::npos) {
      addedString = srcStr.substr(0, pos);
      if (!addedString.empty() || !repeatedCharIgnored) {
        addedString = trim(addedString, " \r\t");
        resultStringVector.push_back(addedString);
      }
      srcStr.erase(srcStr.begin(), srcStr.begin() + pos + 1);
      pos = srcStr.find(delimStr.at(0));
    }
    addedString = srcStr;
    if (!addedString.empty() || !repeatedCharIgnored) {
      addedString = trim(addedString, " \r\t");
      resultStringVector.push_back(addedString);
    }
    return (int)resultStringVector.size();
  }
#endif

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

