
int tag_next(str_t s, str_t* ptn, str_t* pta, str_t* ptd, str_t* ptl)
{
  int i, j, k;
  str_t tn = {0}; // tag name
  str_t td = {0}; // tag data
  str_t ta = {0}; // tag atri
  str_t tl = {0}; // tag last
  s = STRtrim(s, " \t\r\n");

  if (0 == s.l) {
    return 0;
  }

  ASSERT('<' == *s.s);

  if (s.l > 0 && '!' == s.s[1]) {
    for (i = 1; i < s.l && '>' != s.s[i]; ++i);

    *ptl = STRright(s, i + 1);
    return 0;
  }

  for (i = 1; i < s.l; ++i) {
    if ('>' == s.s[i] || isspace(s.s[i])) {
      tn = STR2(s.s + 1, i - 1);

      if ('>' != s.s[i]) {
        ta.s = s.s + i + 1;

        for (; i < s.l && '>' != s.s[i]; ++i);

        ta.l = s.s + i - ta.s;
      }

      td.s = s.s + i + 1;
      break;
    }
  }

  if (tn.l == 0) {
    return 0;
  }

  k = 1;
  j = 0;

  for (i = td.l + 2; i < s.l - tn.l; ++i) {
    if (s.s[i] != ('<')) {
      continue;
    }

    if (('>' == s.s[i + tn.l + 1] || isspace(s.s[i + tn.l + 1])) && 0 == memicmp(s.s + i + 1, tn.s, tn.l)) {
      i += tn.l + 2;
      k++;
      continue;
    }

    if ('/' == s.s[i + 1] && '>' == s.s[i + tn.l + 2] && 0 == memicmp(s.s + i + 2, tn.s, tn.l)) {
      k--;

      if (0 == k) {
        td.l = s.s + i - td.s;
        tl.s = s.s + i + tn.l + 3;
        tl.l = s.s + s.l - tl.s;
        break;
      }

      i += tn.l + 3;
    }
  }

  if (0 == td.l) {
    td.l = s.s + s.l - td.s;
  }

  *ptn = tn, *pta = ta, *ptd = td, *ptl = tl;
  return tn.l > 0;
}

char* pszTags[] = {
  "<br",      //  0
  "<i",     //  1
  "</i",      //  2
  "<b",     //  3
  "</b",      //  4
  "<strong",    //  5
  "</strong",   //  6
  "<u",     //  7
  "</u",      //  8
  "<a",     //  9
  "</a",      // 10
  "<small",   // 11
  "</small",    // 12
  "<big",     // 13
  "</big",    // 14
  "<center",    // 15
  "</center",   // 16
  "<p",     // 17
  "<font",    // 18
  "</font",   // 19
  "<img",     // 20
  "<h1",      // 21
  "</h1",     // 22
  "<div",     // 23
  "</div",    // 24
  "<hr",      // 25
  "<table",   // 26
  "</table",    // 27
  "<tr",      // 28
  "</tr",     // 29
  "<td",      // 30
  "</td",     // 31
  "<h2",      // 32
  "</h2",     // 33
  "<h3",      // 34
  "</h3",     // 35
  "<pre",     // 36
  "</pre",    // 37
  "<input",   // 38
  "<ul",      // 39
  "<ol",      // 40
  "<li",      // 41
  "</ul",     // 42
  "</ol",     // 43
  "<sub",     // 44
  "</sub",    // 45
  "<sup",     // 46
  "</sup",    // 47
  "<strike",    // 48
  "</strike",   // 49
  "<select",    // 50
  "</select",   // 51
  "<option",    // 52
  "<textarea",  // 53
  "</textarea", // 54
  "<th",      // 55
  "</th",     // 56
  "<form",    // 57
  "</form",   // 58
  "<blockquote",  // 59
  "</blockquote", // 60
  "</p",          // 61
  NULL
};

enum {
#define HTMTAGDEF(tag)  htt_##tag,
#include "htmtag.txt"
#undef HTMTAGDEF
};
typedef struct htmtag_t {
  str_t s;
  int t;
} htmtag_t;
htmtag_t g_htmtag[] = {
#define HTMTAGDEF(tag)  sizeof(#tag)-1, #tag, htt_##tag,
#include "htmtag.txt"
#undef HTMTAGDEF
};
typedef struct htmcol_t {
  str_t s;
  COLORREF colVal;
} htmcol_t;
htmcol_t g_htmcol[] = {
#define HTMCOLDEF(name, color)  sizeof(#name)-1, #name, color,
#include "htmcol.txt"
#undef HTMCOLDEF
};

typedef struct htmcode_t {
  char szCode[10];
  int code;
} htmcode_t;

htmcode_t g_htmcode[] = {
#include "htmcode.txt"
};

struct UTF8 {
  unsigned char code;
  unsigned char szCode[3];
} g_Utf8[] = {
#include "UTF8.txt"
};

char* ConvertFromHTMLCodes(char* szText)
{
  int i, j, k;

  for (k = j = 0; szText[j]; j++) {
    for (i = 0; g_htmcode[i].code; i++) {
      if (!strncmp(&szText[j], g_htmcode[i].szCode, strlen(g_htmcode[i].szCode))) {
        szText[k++] = g_htmcode[i].code;
        j += strlen(g_htmcode[i].szCode) - 1;
        break;
      }
    }

    if (!g_htmcode[i].code) {
      if (!strnicmp(&szText[j], "&#", 2)) {
        j += 2;
        szText[k++] = atoi(&szText[j]);

        while (szText[j] && szText[j] != ';' && szText[j] != ' ') {
          j++;
        }

        if (!szText[j]) {
          break;
        }
      }
      else {
        szText[k++] = szText[j];
      }
    }
  }

  szText[k] = 0;
  return szText;
}
char* ConvertToHTMLCodes(str_t s, str_t* s2)
{
  int nLen = 0;
  int i, j, k;

  for (i = 0; i < s.l; i++) {
    for (j = 0; g_htmcode[j].code; j++)
      if (s.s[i] == g_htmcode[j].code) {
        break;
      }

    if (g_htmcode[j].code) {
      nLen += strlen(g_htmcode[j].szCode);
    }
    else {
      nLen++;
    }
  }

  str_setsize(s2, nLen);
  k = 0;

  for (i = 0; i < s.l; i++) {
    for (j = 0; g_htmcode[j].code; j++)
      if (s.s[i] == g_htmcode[j].code) {
        break;
      }

    if (g_htmcode[j].code) {
      strcpy(&s2->s[k], g_htmcode[j].szCode);
      k += strlen(g_htmcode[j].szCode);
    }
    else {
      s2->s[k++] = s.s[i];
    }
  }

  s2->s[k] = 0;
  s2->l = k;
  return 0;
}


int ConvertFromUtf8Codes(str_t* s)
{
  int i, j, k;

  for (k = j = 0; j < s->l; j++) {
    for (i = 0; g_Utf8[i].code; i++) {
      if (!strncmp(&s->s[j], (char*)g_Utf8[i].szCode, strlen((char*)g_Utf8[i].szCode))) {
        s->s[k++] = g_Utf8[i].code;
        j += strlen((char*)g_Utf8[i].szCode) - 1;
        break;
      }
    }

    if (!g_Utf8[i].code) {
      s->s[k++] = s->s[j];
    }
  }

  s->s[k] = 0;
  s->l = k;
  return 0;
}


char* ConvertToUtfCodes(str_t s, str_t* s2)
{
  int nLen = 0;
  int i, j, k;

  for (i = 0; i < s.l; i++) {
    for (j = 0; g_Utf8[j].code; j++)
      if ((unsigned char)s.s[i] == g_Utf8[j].code) {
        break;
      }

    if (g_Utf8[j].code) {
      nLen += strlen((char*)g_Utf8[j].szCode);
    }
    else {
      nLen++;
    }
  }

  str_setsize(s2, nLen);
  k = 0;

  for (i = 0; i < s.l; i++) {
    for (j = 0; g_Utf8[j].code; j++)
      if ((unsigned char)s.s[i] == g_Utf8[j].code) {
        break;
      }

    if (g_Utf8[j].code) {
      strcpy(&s2->s[k], (char*)g_Utf8[j].szCode);
      k += strlen((char*)g_Utf8[j].szCode);
    }
    else {
      s2->s[k++] = s.s[i];
    }
  }

  s2->s[k] = 0;
  s2->l = k;
  return 0;
}

int htm_find_tag(str_t s)
{
  int i;

  for (i = 0; i < countof(g_htmtag); ++i) {
    if (!STRcmp(g_htmtag[i].s, s)) {
      return g_htmtag[i].t;
    }
  }

  return -1;
}

#include <direct.h>

int test_tag_next()
{
  str_t s = {0};
  str_t tn = {0};
  str_t ta = {0};
  str_t td = {0};
  str_t tl = {0};
  str_t tt = {0};
  int i;
  _chdir("D:\\code\\c\\html\\htmlviewer_demo\\");
  str_load("test2.htm", &s);

  tl = s;

  for (i = 0; i < 100; ++i) {
    if (tag_next(tl, &tn, &ta, &td, &tl)) {
      int t = htm_find_tag(tn);
      str_setstr1(&tt, tn);
      printf("%s\n", tt.s);
    }
  }

  str_free(&s);
  return 0;
}
