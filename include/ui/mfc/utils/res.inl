typedef struct {
  int helpId;
  DWORD style;
  DWORD dwExStyle;
  //WORD cdit;
  short x;
  short y;
  short cx;
  short cy;
  short pointSize;
  short weight;
  short italic;
  const char* faceName;
} MYDLGTEMPLATE;
typedef struct {
  const char* clsName;
  const char* text;
  WORD id;
  DWORD style;
  DWORD dwExStyle;
  short x;
  short y;
  short cx;
  short cy;
} MYDLGITEMTEMPLATE;
#define DLGTOK_DEFDEF(DLGTOK_DEF) \
  DLGTOK_DEF(DIALOG) \
  DLGTOK_DEF(DIALOGEX) \
  DLGTOK_DEF(CAPTION) \
  DLGTOK_DEF(FONT) \
  DLGTOK_DEF(BEGIN) \
  DLGTOK_DEF(END) \
  DLGTOK_DEF(ICON) \
  DLGTOK_DEF(LTEXT) \
  DLGTOK_DEF(DEFPUSHBUTTON) \
  DLGTOK_DEF(STYLE) \
  DLGTOK_DEF(DISCARDABLE)
int dlg_gettok(const char* s, int l, int i, int sp, char* tok, int* ptype)
{
  int i1 = i, i2 = i, len = 0;
  l = l < 0 ? strlen(s) : l;
  if (i >= l) {
    return -1;
  }
  for (; i < l && ' ' == s[i] || '\t' == s[i] || '\r' == s[i] || '\n' == s[i]; ++i);
  i1 = i;
  *ptype = 0;
  if (',' == s[i]) {
    len = 1;
    ++i;
    *ptype = 4;
  }
  else if ('|' == s[i]) {
    len = 1;
    ++i;
    *ptype = 5;
  }
  else if ('\"' == s[i]) {
    ++i;
    for (; i < l && !('\"' == s[i] && '\\' != s[i - 1]); ++i);
    ++i;
    len = i - i1;
    *ptype = 1; // string
  }
  else {
    *ptype = ('0' <= s[i] && s[i] <= '9') ? 2 : 3;
    for (; i < l && !('\n' == s[i] || ',' == s[i] || sp == s[i]); ++i);
    len = i - i1;
  }
  strncpy(tok, s + i1, len);
  tok[len] = 0;
  for (; i < l && ' ' == s[i] || '\t' == s[i] || '\r' == s[i] || '\n' == s[i]; ++i);
  return i;
}
static int rc_to_inl_1(const char* s, int l, FILE* fout, char* helpId)
{
  char tok[256];
  char style[256] = {0};
  char exstyle[256] = {0};
  char caption[256] = {0};
  char faceName[256] = {0};
  int i = 0, rcarr[4] = {0}, j = 0, pointSize, type = 0;
  i = dlg_gettok(s, l, i, ' ', helpId, &type);
  for (; (i = dlg_gettok(s, l, i, ' ', tok, &type)) > 0;) {
    if (0 == strcmp(tok, "DIALOG") || 0 == strcmp(tok, "DIALOGEX") || 0 == strcmp(tok, "DISCARDABLE") || 0 == strcmp(tok, ",")) {
      continue;
    }
    else if (j < 4 && '0' <= tok[0] && tok[0] <= '9') {
      rcarr[j++] = atoi(tok);
    }
    else if (0 == strcmp(tok, "STYLE")) {
      int i1 = i;
      i = cstr_findchr(s, l, '\n', i);
      cstr_cpy(style, 256, s + i1, i - i1);
      cstr_trim(style, -1, "\r\n \t");
    }
    else if (0 == strcmp(tok, "EXSTYLE")) {
      int i1 = i;
      i = cstr_findchr(s, l, '\n', i);
      cstr_cpy(exstyle, 256, s + i1, i - i1);
      cstr_trim(exstyle, -1, "\r\n \t");
    }
    else if (0 == strcmp(tok, "CAPTION")) {
      i = dlg_gettok(s, l, i, 0, caption, &type);
    }
    else if (0 == strcmp(tok, "FONT")) {
      i = dlg_gettok(s, l, i, 0, tok, &type);
      pointSize = atoi(tok);
      i = dlg_gettok(s, l, i, 0, tok, &type);
      i = dlg_gettok(s, l, i, 0, faceName, &type);
    }
    else {
      break;
    }
  }
  for (; (i = dlg_gettok(s, l, i, ' ', tok, &type)) > 0;) {
    //switch() {}
  }
  printf("const MYDLGTEMPLATE %s_dlg_template = {\n", helpId);
  printf(" /*style*/%s,\n", style);
  printf(" /*exStyle*/%s,\n", exstyle);
  //printf(" /*helpId*/%s,\n", helpId);
  printf(" /*nbItems*/%d,\n", 0);
  printf(" /*x*/%d,\n", rcarr[0]);
  printf(" /*y*/%d,\n", rcarr[1]);
  printf(" /*cx;*/%d,\n", rcarr[2]);
  printf(" /*cy;*/%d,\n", rcarr[3]);
  //printf(" /*menuName;*/%d,\n", 0);
  //printf(" /*className;*/%d,\n", 0);
  printf(" /*caption;*/%s,\n", caption);
  printf(" /*pointSize;*/%d,\n", pointSize);
  printf(" /*weight;*/%d,\n", 0);
  printf(" /*italic;*/%d,\n", 0);
  printf(" /*faceName;*/%s,\n", faceName);
  //printf(" /*dialogEx;*/%d,\n", 0);
  printf("};\n");
  return 0;
}
static int rc_to_inl_3(const char* s, int l, FILE* fout)
{
  char tok[256]={0};
  char strs[5][256] = {0};
  char ids[5][256] = {0};
  int i1=0, i2=0, i3=0;
  int intarr[6]={0};
  //printf("%s\n", s);
  int i = 0, rcarr[4] = {0}, j = 0, type = 0, line_begin_space = 0;
  for (i=0; i<5; ++i) {
    strcpy(strs[i1++], "NULL");
    strcpy(ids[i3++], "0");
  }
  i1 = i2 = i3 = 0;
  i=0;
  i = dlg_gettok(s, l, i, ' ', tok, &type);
  strcpy(ids[i3++], tok);
  ASSERT(i > 0);
  for (; (i1<5)&&(i2<5)&&(i3<5)&&(i = dlg_gettok(s, l, i, 0, tok, &type)) > 0;) {
    switch (type) {
    case 1: // string
      //printf("string ");
      strcpy(strs[i1++], tok);
      break;
    case 2://number
      //printf("number ");
      intarr[i2++] = atoi(tok);
      break;
    case 3://id
      //printf("id ");
      strcpy(ids[i3++], tok);
      break;
    }
  }
  if (0==stricmp("CONTROL", ids[0])) {
    strcpy(ids[0], strs[1]);
  }
  i1 = i2 = i3 = 0;
  //printf("\n");
  printf("{");
  printf("%s,", ids[i3++]); // clsName
  printf("%s,", strs[i1++]); // text
  printf("%s,", ids[i3++]); // id
  printf("%s,", ids[i3++]); // style
  printf("%s,", ids[i3++]); // dwExStyle
  printf("%d,", intarr[i2++]); // x
  printf("%d,", intarr[i2++]); // y
  printf("%d,", intarr[i2++]); // cx
  printf("%d", intarr[i2++]); // cy
  printf("},\n");
  return 0;
}
static int strcpyline(char* a, int la, const char* b, int i, int lb, int iscat)
{
  int j = iscat ? strlen(a) : 0, prev = 0;
  for (; j < la && i < lb && b[i] && '\n' != b[i]; ++i) {
    if ('\r' != b[i]) {
      if (' ' == b[i] && ' ' == prev) {
        continue;
      }
      prev = b[i];
      a[j++] = b[i];
    }
  }
  ASSERT(j < (la - 2));
  a[j] = 0;
  for (; i < lb && '\n' == b[i]; ++i);
  return i;
}
static int rc_to_inl_2(const char* s, int l, FILE* fout)
{
  int i = 0, type, j = 0, lbuf;
  char tok[256] = {0};
  char sbuf[256] = {0};
  const char* ctrs = "DEFPUSHBUTTON|PUSHBUTTON|LTEXT|GROUPBOX|CONTROL|LISTBOX|EDITTEXT|ICON";
  for (; i < l;) {
    int i1 = i, iscat = 1;
    i = dlg_gettok(s, l, i, ' ', tok, &type);
    if (i < 0) {
      break;
    }
    j = strlen(tok);
    if (j > 0 && strstr(ctrs, tok)) {
      lbuf = strlen(sbuf);
      if (lbuf > 0) {
        rc_to_inl_3(sbuf, lbuf, fout);
      }
      iscat = 0;
    }
    i = strcpyline(sbuf, 256, s, i1, l, iscat);
  }
  lbuf = strlen(sbuf);
  if (lbuf > 0) {
    rc_to_inl_3(sbuf, lbuf, fout);
  }
  return 0;
}
static int rc_to_inl(const char* fn)
{
  FILE* pf;
  FILE* fout;
  char buf[256];
  int l, len = 0;
  char* str = NULL;
  char helpId[256] = {0};
  pf = fopen(fn, "rb");
  if (NULL == pf) {
    return 0;
  }
  _snprintf(buf, 256, "%s.inl", fn);
  fout = fopen(buf, "wb");
  for (; (fgets(buf, 256, pf)) > 0;) {
    if (!(strstr(buf, " DIALOG ") || strstr(buf, " DIALOGEX "))) {
      continue;
    }
    len = 0;
    l = strlen(buf);
    MYREALLOC(str, len + l + 2);
    len = cstr_cat(str, len, len + l + 2, buf, l);
    for (; (fgets(buf, 256, pf)) > 0;) {
      if (strstr(buf, "BEGIN")) {
        break;
      }
      l = strlen(buf);
      MYREALLOC(str, len + l + 2);
      len = cstr_cat(str, len, len + l + 2, buf, l);
    }
    printf("%s", str);
    rc_to_inl_1(str, len, fout, helpId);
    len = 0;
    for (; (fgets(buf, 256, pf)) > 0;) {
      if (strstr(buf, "END")) {
        break;
      }
      l = strlen(buf);
      MYREALLOC(str, len + l + 2);
      len = cstr_cat(str, len, len + l + 2, buf, l);
    }
    printf("%s", str);
    printf("const MYDLGTEMPLATE %s_dlg_item_template = {\n", helpId);
    rc_to_inl_2(str, len, fout);
    printf("};\n");
  }
  FREE(str);
  fclose(pf);
  fclose(fout);
  return 0;
}
int ren_file_1(const WCHAR* fn, WCHAR* fn2)
{
  int i, j=0, n = wcslen(fn), ret=0, i1=0, n1=n-3;
  char aaa[256] = {0};
  for (i = 0; i < n1 && j<6; ++i) {
    if ('0' <= fn[i] && fn[i] <= '9') {
      fn2[j++] = fn[i];
      ret = 1;
      aaa[i] = 1;
    }
  }
  for (i = 0; i < n1; ++i) {
    if (0==aaa[i]) {
      fn2[j++] = fn[i];
    }
  }
  for (; i < n; ++i) {
    fn2[j++] = fn[i];
  }
  fn2[j] = 0;
  return ret;
}
#include "cfile.h" 

int test_rename()
{
  const WCHAR* path = NULL;
  path = L"G:/MV/MV/·¹ÅÄ/Laysha/aaa";
  findinfo_t s[1] = {0};
  fileinfoW_t f[1] = {0};
  setlocale(LC_CTYPE, ".936");
  setlocale(LC_ALL, "chs");
  for (;find_next_fileW(s, path, L"*.mp4", f, 0);) {
    WCHAR fn2[MAX_PATH]={0};
    //wprintf(L"%s %s\n", f->name, fn2);
    ren_file_1(f->name, fn2);
    wprintf(L"%s \r\n", f->name);
    wprintf(L" --> %s \r\n", fn2);
    //MoveFileW(f->name, fn2);
  }
  return 0;
}
int test_rc_to_inl()
{
  const char* path = NULL;
  path = "E:/code/cstd/include/mfc/testdlg";
  _chdir(path);
  //test_rename();
  rc_to_inl("testdlg.rc");
  return 0;
}

