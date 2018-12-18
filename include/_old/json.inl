
#ifndef _JSON_INL_
#define _JSON_INL_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int skipspace(const char* buf, int i, int len)
{
  for (; i < len && (buf[i] == ' ' || buf[i] == '\r' || buf[i] == '\n' || buf[i] == '\t'); ++i) {}
  return i;
}
static int json_find_name(const char* buf, int i, int len, const char* sec, int seclen, int* p, int plen)
{
  int j = 0, d = 0, tt = 1, p0 = 0, p1 = 0, p2 = 0, p3 = 0;
  for (; i < len; ++i) {
    p0 = 0, p1 = 0, p2 = 0, p3 = 0;
    if ('{' == buf[i]) {
      ++d;
    }
    if ('}' == buf[i]) {
      --d;
    }
    if (d < 0) {
      return 0;
    }
    i = skipspace(buf, i, len);
    if ('{' != buf[i]) {
      p0 = i;
      for (++i; i < len && ';' != buf[i] && '{' != buf[i]; ++i) {
      }
      p1 = i;
      i = skipspace(buf, i, len);
    }
    if ('{' == buf[i]) {
      int dd = d + 1;
      ++i;
      p2 = i;
      for (; i < len && dd > d; ++i) {
        if ('{' == buf[i]) {
          ++dd;
        }
        if ('}' == buf[i]) {
          --dd;
        }
      }
      p3 = i - 1;
    }
    if (*sec == buf[p0] && ((p0 + seclen) < len) && (buf[p0 + seclen] == ' ' || buf[p0 + seclen] == '{' || buf[p0 + seclen] == '\n') && 0 == strncmp(buf + p0, sec, seclen)) {
      p0 += seclen;
      p0 = skipspace(buf, p0, len);
      //printf("%s\n", sec);
      if (p && (j + 4) <= plen) {
        p[j + 0] = p0;
        p[j + 1] = p1;
        p[j + 2] = p2;
        p[j + 3] = p3;
      }
      j += 4;
    }
  }
  return j;
}
static int json_find_name_1(const char* buf, int len, const char* name, int* p, int plen)
{
  int j, namelen0;
  const char* pname = name;
  p[0] = p[1] = p[2] = 0;
  p[3] = len;
  j = 4;
  for (; pname && *pname;) {
    const char* name0 = pname;
    pname = strchr(pname, '|');
    if (pname) {
      namelen0 = pname - name0;
      ++pname;
    }
    else {
      namelen0 = strlen(name0);
    }
    j = json_find_name(buf, p[2], p[3], name0, namelen0, p, plen);
  }
  return j;
}
static int json_get_value(const char* fn, const char* name, char* value, int maxlen)
{
  int i, len, d = 0, j = 0, p[100];
  char* _buf = NULL;
  const char* buf = NULL;
  FILE* pf = fopen(fn, "rb");
  if (pf) {
    fseek(pf, 0, SEEK_END);
    len = ftell(pf);
    fseek(pf, 0, SEEK_SET);
    _buf = (char*)malloc(len+1);
    if (buf) {
      len = fread(_buf, 1, len, pf);
    }
    buf = _buf;
    fclose(pf);
    _buf[len] = 0;
  } else {
    buf = fn;
    len = strlen(fn);
  }
  i = 0;
  j = json_find_name_1(buf, len, name, p, 100);
  if (j > 0) {
    int l = 0;
#if 0
    //È¥µôÊ×Î²µÄ""
    if ('\"' == buf[p[0]] && '\"' == buf[p[1] - 1]) {
      ++p[0];
      --p[1];
    }
#endif
    l = p[1] - p[0];
    if (l>maxlen-1) {
      l = maxlen-1;
    }
    strncpy(value, buf + p[0], l);
    value[l] = 0;
#ifdef TEST
    printf("%s = %s\n", name, value);
#endif // TEST
  }
  free(_buf);
  return j / 4;
}
#ifdef TEST
#include <direct.h>
int test_json_get_value()
{
  char value[256] = {0};
  _chdir("E:/code/c/net/802");
  json_get_value("dhclient.leases", "lease|fixed-address", value, 256);
  json_get_value("dhclient.leases", "lease6|ia-na", value, 256);
  json_get_value("dhclient.leases", "lease6|ia-na|iaaddr", value, 256);
  json_get_value("dhclient.leases", "lease6|ia-na|iaaddr|starts", value, 256);
  json_get_value("dhclient.leases", "lease6|ia-na|iaaddr|preferred-life", value, 256);
  json_get_value("dhclient.leases", "lease6|interface", value, 256);
  json_get_value("dhclient.leases", "lease6|option dhcp6.server-id", value, 256);
  json_get_value("dhclient.leases", "lease|option domain-name-servers", value, 256);
  json_get_value("dhclient.leases", "lease6|option dhcp6.domain-search", value, 256);
  return 0;
}
#endif // TEST
#endif // _JSON_INL_
