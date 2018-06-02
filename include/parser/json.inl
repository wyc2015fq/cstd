
static const char* json_type_name[] = {"null", "array", "object", "key", "string", "number"};

#define json_print(v, i, n)   dstr_print(v, i, n, json_type_name, countof(json_type_name))
static int dstr_print(const dstr_t* v, int i, int n, const char** type_name, int maxtype)
{
  uint j, d = v[i].d;
  int tabw = 2;
  char buf[256];
  for (; i < n && v[i].d>=d; ++i) {
    int d = v[i].d;
    CElemType t = v[i].t;
    const char* stype = GetCElemTypeName(t);
    j = d*tabw;
    memset(buf, ' ', j);
    cstr_cpy(buf + j, 256-j, v[i].s->s, v[i].s->l);
    
    printf("%4d: %s(%s)(d=%d)\n", i, buf, stype, d);
  }
  return 0;
}

static uint parse_json_string(dstr_t* ds, uint* opos, const char* buf, uint i, uint len, int d, CElemType t)
{
  uint p0 = 0, p1 = 0, have = '\"' == buf[i];
  i += have;
  p0 = i;
  for (; i < len && '\"' != buf[i] && ':' != buf[i] && '\\' != buf[i - 1]; ++i);
  push_dstr(p0, i-p0, d, t);
  if (i<len && '\"' == buf[i]) {
    ++i;
  }
  p1 = i;
  return i;
}

static uint parse_json_string2(dstr_t* ds, uint* opos, const char* buf, uint i, uint len, int d, CElemType t)
{
  uint p0 = i;
  for (; i < len && '\n' != buf[i] && ':' != buf[i] && '{' != buf[i] && '[' != buf[i] && '}' != buf[i] && ']' != buf[i] && ',' != buf[i]; ++i);
  push_dstr(p0, i-p0, d, t);
  return i;
}

uint parse_json_object(dstr_t* ds, uint* opos, const char* buf, uint i, uint len, int d);
uint parse_json_array(dstr_t* ds, uint* opos, const char* buf, uint i, uint len, int d);
uint parse_json_value(dstr_t* ds, uint* opos, const char* buf, uint i, uint len, int d);

uint parse_json_array(dstr_t* ds, uint* opos, const char* buf, uint i, uint len, int d)
{
  int j = 0;
  int p0 = 0, p1 = 0;
  for (; i < len; ++j) {
    SKIPSPACE1;
    i = parse_json_value(ds, opos, buf, i, len, d);
    SKIPSPACE1;
    if (',' == buf[i]) {
      ++i;
    }
    else {
      break;
    }
  }
  return i;
}
uint parse_json_object(dstr_t* ds, uint* opos, const char* buf, uint i, uint len, int d)
{
  uint p0 = 0, p1 = 0;
  for (; i < len; ) {
    SKIPSPACE1;
    i = parse_json_string(ds, opos, buf, i, len, d, TT_KEY);
    SKIPSPACE1;
    if (':' == buf[i]) {
      i = parse_json_value(ds, opos, buf, i + 1, len, d + 1);
    }
    SKIPSPACE1;
    if (',' == buf[i]) {
      ++i;
    }
    else {
      break;
    }
  }
  return i;
}
#define JSON_ERROR(x)  if (!(x)) {return -i;}

uint parse_json_value(dstr_t* ds, uint* opos, const char* buf, uint i, uint len, int d)
{
  char c;
  char str[16] = {0};
  SKIPSPACE1;
  c = buf[i];
  if ('\"' == c) {
    i = parse_json_string(ds, opos, buf, i + 1, len, d, TT_STRING);
  }
  else if ('}' == c || ']' == c) {
    return i;
  }
  else if (',' != c && c > 32 && '{' != c && '[' != c) {
    i = parse_json_string2(ds, opos, buf, i, len, d, TT_NUMBER);
  }
  else {
    if ('[' == c) {
      push_dstr(i + 1, 0, d, TT_ARRAY);
      i = parse_json_array(ds, opos, buf, i + 1, len, d+1);
      SKIPSPACE1;
      JSON_ERROR(']' == buf[i]);
      ++i;
    }
    else if ('{' == c) {
      push_dstr(i + 1, 0, d, TT_OBJECT);
      i = parse_json_object(ds, opos, buf, i + 1, len, d+1);
      SKIPSPACE1;
      JSON_ERROR('}' == buf[i]);
      ++i;
    }
  }
  return i;
}
uint parse_json(dstr_t** pds, const char* buf, int len) {
  uint i=0, j=0, opos = 0;
  len = len<0 ? strlen(buf) : len;
  while(buf[j] !='{') {++j;}
  i = parse_json_value(NULL, &opos, buf, j, len, 0);
  MYREALLOC(*pds, opos);
  opos = 0;
  i = parse_json_value(*pds, &opos, buf, j, len, 0);
  return opos;
}

int dstr_findname_1(dstr_t* v, int i, int n, const char* name, int namelen)
{
  int d = v[i].d+1, t = v[i].t;
  if (TT_OBJECT==t) {
    for (++i; i < n && v[i].d >= d; ++i) {
      str_t* s = v[i].s;
      if (d == v[i].d && namelen == s->l && *name == s->s[0] && 0 == cstr_nicmp(s->s, s->l, name, namelen, namelen, 1)) {
        return i + 1;
      }
    }
  } else if (TT_ARRAY==t) {
    int j=0, index = atoi(name);
    for (++i; i < n && v[i].d >= d; ++i) {
      if (d == v[i].d) {
        if (j==index) {
          return i;
        }
        ++j;
      }
    }
  }
  return -1;
}
int dstr_findname(dstr_t* ds, int n, const char* name)
{
  int j = 0, namelen0;
  const char* pname = name;
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
    j = dstr_findname_1(ds, j, n, name0, namelen0);
    if (j < 0) {
      return -1;
    }
  }
  return j;
}
static int dstr_get_value(dstr_t* ds, int i, int n, const char* key) {
  int d = ds[i].d;
  int keylen = strlen(key);
  for (; d==ds[i].d && i<n; ++i) {
    if (0==cstr_cmp(ds[i].s->s, ds[i].s->l, key, keylen)) {
      return i;
    }
  }
  return -1;
}

//int dstr_findname2_v(dstr_t* ds, int n, const char* name, myva_list arg) {}
//int dstr_findname2(dstr_t* ds, int n, const char* name, myva_list arg) {}