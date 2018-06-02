#ifndef _XML_INL_
#define _XML_INL_
#include "str.h"
#include "cfile.h"
static const char* xml_type_name[] = {"null", "comment", "text", "elem", "attr", "attr_key", "attr_value"};
#ifndef countof
#define countof(arr)   (sizeof(arr)/sizeof(arr[0]))
#endif // countof
#define NODEPRINTF printf

int dstr_print(FILE* pf, const dstr_t* ds, int len) {
  const char* typestring = "_ctenv";
  int i, j;
  const char* INDSTR = " ";
  for (i=0; i<len; ++i) {
    dstr_t nn = ds[i];
    printf("%c%d ", typestring[nn.t], nn.d);
    for (j=0; j<nn.d; ++j) {
      fprintf(pf, INDSTR);
    }
  }
  return 0;
}

int ParseXml(dstr_t* ds, int* opos, const char* buf, int i, int len, int d);
int ParseText(dstr_t* ds, int* opos, const char* buf, int i, int len, int d)
{
  int isstr = 0;
  int p0 = i;
  for (; i < len; i++) {
    if ('"' == buf[i]) {
      isstr = !isstr;
    }
    if (isstr) {
      continue;
    }
    if ('<' == buf[i]) {
      push_dstr(p0, i - p0, d, TT_TEXT);
      break;
    }
  }
  return i;
}
int ParseNodeAttributeName(dstr_t* ds, int* opos, const char* buf, int i, int len, int d)
{
  int p0 = i;
  for (; i < len; i++) {
    if ('=' == buf[i]) {
      break;
    }
    if (' ' >= buf[i] && buf[i] > 0) {
      break;
    }
  }
  push_dstr(p0, i - p0, d, TT_ATTR_KEY);
  return i;
}
int ParseNodeAttributeValue(dstr_t* ds, int* opos, const char* buf, int i, int len, int d)
{
  int p0, isstr = 0, p1;
  SKIPSPACE1;
  p0 = i;
  for (; i < len; i++) {
    if ('"' == buf[i]) {
      isstr = !isstr;
    }
    if (isstr) {
      continue;
    }
    if (' ' >= buf[i] && buf[i] > 0) {
      break;
    }
    if ('/' == buf[i] && '>' == buf[i + 1]) {
      break;
    }
    if ('?' == buf[i] && '>' == buf[i + 1]) {
      break;
    }
    if ('/' == buf[i] && '?' == buf[i + 1] && '>' == buf[i + 2]) {
      i += 3;
      break;
    }
    if ('>' == buf[i]) {
      break;
    }
  }
//#define STRtrim(s1, sp)  _cstrtrim_c(s1.s, s1.l, sp)
  //node.name = STRtrim(node.name, "\"");
  p1 = i;
  if ('"'==buf[p1-1] && '"'==buf[p0]) {
    --p1;
    ++p0;
  }
  push_dstr(p0, p1-p0, d, TT_ATTR_VALUE);
  return i;
}
int ParseComment(dstr_t* ds, int* opos, const char* buf, int i, int len, int d)
{
  int p0, nattr=0;
  ASSERT('<' == buf[i] && '?' == buf[i + 1]);
  i += 2;
  SKIPSPACE1;
  p0 = i;
  for (; i < len; i++) {
    if (' ' >= buf[i] && buf[i] > 0) {
      break;
    }
    if ('?' == buf[i] && '>' == buf[i + 1]) {
      break;
    }
  }
  push_dstr(p0, i-p0, d, TT_COMMENT);
  for (; i < len;) {
    SKIPSPACE1;
    if ('?' == buf[i] && '>' == buf[i + 1]) {
      i += 2;
      break;
    }
    if ('/' == buf[i] && '?' == buf[i + 1] && '>' == buf[i + 2]) {
      i += 3;
      break;
    }
    if (0==nattr) {
      push_dstr(i, 0, d+1, TT_ATTR);
    }
    ++nattr;
    i = ParseNodeAttributeName(ds, opos, buf, i, len, d + 2);
    if (i<len) {
      SKIPSPACE1;
      ASSERT('=' == buf[i]);
      i++;
      i = ParseNodeAttributeValue(ds, opos, buf, i, len, d + 3);
    }
  }
  return i;
}
int ParseElement(dstr_t* ds, int* opos, const char* buf, int i, int len, int d)
{
  int isstr = 0, isend = 0, p0 = i, namelen=0, nattr = 0;
  const char* name=NULL;
  SKIPSPACE1;
  name = buf + i;
  for (; i < len; i++) {
    if (' ' >= buf[i] && buf[i] > 0) {
      break;
    }
    if ('/' == buf[i] && '>' == buf[i + 1]) {
      break;
    }
    if ('>' == buf[i]) {
      break;
    }
  }
  namelen = i - p0;
  push_dstr(p0, i - p0, d, TT_ELEM);
  for (; i < len;) {
    SKIPSPACE1;
    if ('/' == buf[i] && '>' == buf[i + 1]) {
      i += 2;
      isend = 1;
      break;
    }
    if ('>' == buf[i]) {
      i++;
      break;
    }
    if (0==nattr) {
      push_dstr(i, 0, d+1, TT_ATTR);
    }
    ++nattr;
    i = ParseNodeAttributeName(ds, opos, buf, i, len, d+2);
    SKIPSPACE1;
    if (i<len) {
      ASSERT('=' == buf[i]);
      i++;
      i = ParseNodeAttributeValue(ds, opos, buf, i, len, d+3);
    } else {
      ASSERT(0);
      return len;
    }
  }
  if (!isend) {
    const char* name1;
    int namelen1 = 0;
    i = ParseXml(ds, opos, buf, i, len, d + 1);
    ASSERT(i<len);
    ASSERT('<' == buf[i] && '/' == buf[i + 1]);
    i += 2;
    SKIPSPACE1;
    p0 = i;
    name1= buf + i;
    for (; i < len; i++) {
      if ('>' == buf[i]) {
        namelen1 = i - p0;
        ASSERT(0 == cstr_cmp(name, namelen, name1, namelen1));
        i++;
        return i;
      }
    }
    ASSERT(0);
    return i;
  }
  return i;
}
int ParseXml(dstr_t* ds, int* opos, const char* buf, int i, int len, int d)
{
  for (; i < len;) {
    if ('<' == buf[i]) {
      if ('/' == buf[i + 1]) {
        break;
      }
      else if ('?' == buf[i + 1]) {
        i = ParseComment(ds, opos, buf, i, len, d);
      }
      else if ('!' == buf[i + 1]) {
        for (i++; '>' != buf[i]; i++);
        i++;
      }
      else {
        i = ParseElement(ds, opos, buf, i+1, len, d);
      }
    }
    else if (' ' >= buf[i] && buf[i] > 0) {
      i++;
    }
    else {
      i = ParseText(ds, opos, buf, i, len, d);
      //ASSERT(0);
    }
  }
  return i;
}
int vdstr_free(vdstr_t* dv)
{
  if (dv) {
    str_free(dv->s);
    if (dv->v) {
      FREE(dv->v);
      dv->v = NULL;
    }
    memset(dv, 0, sizeof(*dv));
  }
  return 0;
}
int vdstr_set_xml(vdstr_t* dv, const char* buf, int i, int len) {
  int opos = 0;//mode = XML_MARK_ALL;
  ParseXml(NULL, &opos, buf, i, len, 0);
  MYREALLOC(dv->v, opos);
  opos = 0;
  i = ParseXml(dv->v, &opos, buf, i, len, 0);
  dv->l = opos;
  return opos;
}
int vdstr_load_xml(vdstr_t* dv, const char* xmlfn)
{
  str_loadasni(xmlfn, dv->s);
  return vdstr_set_xml(dv, dv->s->s, 0, dv->s->l);
}
int XmlFindFirstNode(vdstr_t* dv, int begpos, DWORD mark)
{
  int i, d = dv->v[begpos].d;
  for (i = begpos + 1; i < dv->l && dv->v[i].d > d; ++i) {
    if (((1 << dv->v[i].t)&mark)) {
      return i;
    }
  }
  return -1;
}
#define XMLFINDSUBNODE(dv, begpos, d, mark, s) XmlFindSubNode(dv, begpos, d, mark, s, strlen(s))
int XmlFindSubNode(vdstr_t* dv, int begpos, int d, DWORD mark, const char* s, int l)
{
  int i;
  for (i = begpos; i < dv->l && dv->v[i].d >= d; ++i) {
    if (dv->v[i].d == d && ((1 << dv->v[i].t)&mark) && 0 == cstr_cmp(dv->v[i].s->s, dv->v[i].s->l, s, l)) {
      return i;
    }
  }
  return -1;
}
int XmlCountSubNode(vdstr_t* dv, int begpos, int d, DWORD mark, const char* s, int l)
{
  int i, n = 0;
  for (i = begpos; i < dv->l && dv->v[i].d >= d; ++i) {
    if (dv->v[i].d == d && ((1 << dv->v[i].t)&mark) && 0 == cstr_cmp(dv->v[i].s->s, dv->v[i].s->l, s, l)) {
      ++n;
    }
  }
  return n;
}
#define XMLFINDNODE(dv, begpos, mark, s)  XmlFindNode(dv, begpos, mark, s, strlen(s))
int XmlFindNode(vdstr_t* dv, int begpos, DWORD mark, const char* s, int l)
{
  int i, d = dv->v[begpos].d;
  for (i = begpos + 1; i < dv->l && dv->v[i].d > d; ++i) {
    if (((1 << dv->v[i].t)&mark) && 0 == cstr_cmp(dv->v[i].s->s, dv->v[i].s->l, s, l)) {
      return i;
    }
  }
  return -1;
}
int XmlFindElemWithAttribute(vdstr_t* dv, const char* ns, int nl, const char* vs, int vl)
{
  int i;
  for (i = 0; i < dv->l; ++i) {
    dstr_t* n = dv->v + i;
    if (n[0].t == TT_ATTR_KEY) {
      if (n[0].t == TT_ATTR_KEY
          && 0 == cstr_cmp(n[0].s->s, n[0].s->l, ns, nl)
          && 0 == cstr_cmp(n[1].s->s, n[1].s->l, vs, vl)
         ) {
        for (; i-- >= 0;) {
          if (TT_ELEM == dv->v[i].t) {
            break;
          }
        }
        return i;
      }
    }
  }
  return -1;
}
int XmlGetAttributes(vdstr_t* dv, int pos, str_t** s)
{
  int i, k = 0;
  int d = dv->v[pos].d;
  for (i = pos + 1; i < dv->l && dv->v[i].d > d;) {
    dstr_t* n = dv->v + i;
    if (n[0].t == TT_ATTR_KEY && n[1].t == TT_ATTR_VALUE) {
      s[k + 0] = n[0].s;
      s[k + 1] = n[1].s;
      i += 2;
      k += 2;
    }
    else {
      ++i;
    }
  }
  return k;
}

enum {
  ST_CONST,
  ST_VAR,
  ST_FUNC,
  ST_UADD,
  ST_USUB,
  ST_BADD,
  ST_BSUB,
  ST_BMUL,
  ST_BDIV,
  ST_BPOW,
};


#define xml_print(v, i, n)   dstr_print(v, i, n, xml_type_name, countof(xml_type_name))

#ifdef TEST
//#include "svg_parser.inl"
//#include "excel_parser.inl"
int test_xml()
{
  vdstr_t dv[1] = {0};
  //logclear();
  //logtime(0);
  //Workbook wb;
  char* xmlfn;
  sys_chdir("E:/pub/bin/json");
  if (1) {
    xmlfn = "1.xml";
    xmlfn = "1.xml";
    xmlfn = "test001.xml";
    xmlfn = "test_GameDemo.xml";
    xmlfn = "tiger.svg";
    //svg_parser(xmlfn);
    //return i; loadXmlExcel(&wb, xmlfn);  return i;
    //dv->mode = XML_MARK_COMMENT | XML_MARK_ELEMENT | XML_MARK_ATTR_KEY | TT_ATTR_VALUE;
    //dv->mode = XML_MARK_ALL;
    vdstr_load_xml(dv, xmlfn);
    //xml_print(dv->v, 0, dv->l);
  }
  vdstr_free(dv);
  return 0;
}
#endif
#endif // _XML_INL_
