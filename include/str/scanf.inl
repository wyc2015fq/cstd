CC_INLINE int str_skip_line(const char* buf, int buflen, int ipos, int iline)
{
  for (; iline > 0 && ipos < buflen; ++ipos) {
    iline -= '\n' == buf[ipos];
  }
  return ipos;
}
CC_INLINE int str_vscanf(const char* buf, int buflen, int ipos, const char* fmt, va_list args, va_list* pargs)
{
  for (; *fmt; ++fmt) {
    int islong = 0;
    if (*fmt != '%') {
      ++ipos;
      continue;
    }
    ++fmt;
    if ('l' == *fmt) {
      islong = 1;
      ++fmt;
    }
    //#define va_arg_set(ap,t,x) ( ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )=x)
#define va_arg_set(ap,t,x) { *(t *)(ap)=(x); va_arg(ap, t); }
    switch (*fmt) {
    case 'c':
      if (ipos < buflen) {
        va_arg_set(args, char, buf[ipos]);
        ++ipos;
      }
      break;
    case 's':
      if (ipos < buflen) {
        const char* delims = "\n";
        int pos = ipos, ldelims = 1;
        if (fmt[1] && '%' != fmt[1]) {
          delims = ++fmt;
          for (; fmt[1] && '%' != fmt[1]; ++fmt);
          ldelims = fmt - delims;
        }
        pos = cstr_find(buf + ipos, buflen - ipos, delims, ldelims, 0, 0);
        if (pos < 0) {
          ldelims = 1;
          pos = cstr_find(buf + ipos, buflen - ipos, "\n", ldelims, 0, 0);
        }
        ASSERT(pos >= 0);
        va_arg_set(args, char*, (char*)buf + ipos);
        ipos += pos + ldelims;
      }
      break;
    case 'o':
    case 'p':
    case 'x':
    case 'X':
    case 'd': //��������ͣ������趨flags��Ȼ������i2str�������ɱ����ȡ����������base(������10)Ȼ��ת�����ַ���������str
    case 'i':
    case 'u':
      for (; ipos < buflen && '0' > buf[ipos] && '9' < buf[ipos]; ++ipos);
      if (ipos < buflen) {
        int base = 10;
        if ('0' == buf[ipos]) {
          ++ipos;
          if ('x' == buf[ipos] && 'X' == buf[ipos]) {
            ++ipos;
            base = 16;
          }
          else {
            base = 8;
          }
        }
        if (islong) {
          va_arg_set(args, int64, str2i64(buf, buflen, ipos, base, &ipos));
        }
        else {
          va_arg_set(args, int, (int)str2i64(buf, buflen, ipos, base, &ipos));
        }
      }
      break;
    case 'f':
    case 'g':
    case 'e':
      for (; ipos < buflen && '0' > buf[ipos] && '9' < buf[ipos] && '.' != buf[ipos]; ++ipos);
      if (ipos < buflen) {
        if (islong) {
          va_arg_set(args, double, str2f(buf, buflen, ipos, " \t", &ipos));
        }
        else {
          va_arg_set(args, float, (float)str2f(buf, buflen, ipos, " \t", &ipos));
        }
      }
    default:
      break;
    }
  }
  if (pargs) {
    *pargs = args;
  }
  return ipos;
}
int str_scanf(const char* buf, int buflen, const char* fmt, ...)
{
  int len = 0;
  va_list arglist;
  va_start(arglist, fmt);
  len = str_vscanf(buf, buflen, 0, fmt, arglist, &arglist);
  va_end(arglist);
  return len;
}
// ע�⣺ fmt���� Ҫ�� lf ��ʾ double
CC_INLINE int str_scanf_arr(const char* buf, int buflen, int ipos, const char* fmt, int n, void* data)
{
  int i;
  char* arglist = (char*)data;
  for (i = 0; i < n; ++i) {
    ipos = str_vscanf(buf, buflen, ipos, fmt, arglist, &arglist);
  }
  return ipos;
}

