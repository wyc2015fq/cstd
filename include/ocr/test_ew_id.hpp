
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "std/iconv_c.h"
#include "std/algo.h"
#include "ocr/idcard.h"
#include "std/str_c.h"
//#include <>

int get_idcard(const char* buf, idcard* id) {
  int i, len = strlen(buf), j = 0;
  const char* p = buf;
  memset(id, 0, sizeof(*id));
  for (i = 0; i < len; ++i) {
    if (buf[i] == '\t' || i == (len - 1)) {
      const char* q = buf + i;
      switch (j)
      {
      case 2:
        strncpy(id->number, p, q - p);
        break;
      case 3:
        strncpy(id->name, p, q - p);
        break;
      case 4:
        strncpy(id->birthday, p, q - p);
        break;
      case 5:
        strncpy(id->gender, p, q - p);
        break;
      case 6:
        strncpy(id->address, p, q - p);
        break;
      default:
        break;
      }
      p = buf + i + 1;
      ++j;
    }
  }
  return 0;
}

static const char* split_word[] = { "广西", "内蒙古", "省", "市", "县", "区", "乡", "镇", "村委会", "村", "办事处", "路", "街道", "居委会"
//, "组", "栋", "单元", "号"
};

int vstr_find(str_t* v, int n, int i, const char* str, int len) {
  for (; i < n && v[i].s[0] >= *str; ++i) {
    if (len == v[i].l && *str == v[i].s[0] && 0 == strcmp_c(str, len, v[i].s, v[i].l)) {
      return i;
    }
  }
  return -1;
}
const char* split_by_word(const char* buf, const char* word[], int word_num) {
  int i = 0;
  const char* p = NULL;
  for (i = 0; i < word_num; ++i) {
    const char* s = strstr(buf, word[i]);
    if (s) {
      if ((p && s < p) || (!p)) {
        p = s + strlen(word[i]);
        return p;
      }
    }
  }
  return p;
}
int set_smallstr(const char* addr, vstr_t* vs) {
  int n = countof(split_word), len = strlen(addr);
  const char* p = addr;
  char buf[256];
  int k = 0, pre = 0;
  const char* q;
  if (0 == memcmp(addr, "湖南", 4)) {
    int asdf = 0;
  }
  for (int i = 0; q = split_by_word(p, split_word, n); ++i) {
    int len = q - p;
    buf[0] = '0' + i;
    buf[1] = '*';
    strncpy(buf + 2, p, len);
    len += 2;
    buf[len] = 0;
    k = vstr_find(vs->v, vs->n, pre, buf, len);
    if (k >= 0) {
      pre = k+1;
    }
    else {
      vstr_ins_cstr(vs, pre, buf, len);
      pre++;
    }
    p = q;
  }
  return 0;
}

int vstr_print(vstr_t* vs) {
  int i = 0;
  char buf[100];
  for (; i < vs->n; ++i) {
    const char* s = vs->v[i].s;
    int k = *s - '0';
    memset(buf, ' ', k);
    buf[k] = 0;
    printf("%5d : %s%s\n", i, buf, s);
  }
  return 0;
}
int vstr_fprint(FILE* pf, vstr_t* vs) {
  int i = 0;
  char buf[100];
  for (; i < vs->n; ++i) {
    const char* s = vs->v[i].s;
    int k = *s - '0';
    memset(buf, ' ', k);
    buf[k] = 0;
    fprintf(pf, "%5d : %s%s\n", i, buf, s);
  }
  return 0;
}
int test_ew_id() {
  const char* file = "E:/data/ew_id/t1_0911.txt";
  const char* outlist = "E:/data/ew_id/outlist.txt";
  const char* addrfile = "D:/code/git/cstd/include/ocr/train/nlp/addr.txt";
  if (0) {
    FILE* pf = fopen(file, "rb");
    char buf[1024];
    char gbbuf[1024];
    int j = 0;
    idcard id[1] = { 0 };
    vstr_t vs[1] = { 0 };
    int sslen = 0;
    for (j = 0; fgets(buf, 1024, pf) > 0 && j < 100000; ++j) {
      int len = strlen(buf);
      len = iconv_c(ICONV_UTF8, ICONV_GB2312, buf, len, gbbuf, 1024);
      gbbuf[len] = 0;
      get_idcard(gbbuf, id);
      set_smallstr(id->address, vs);
    }
    if (1) {
      FILE* pf = fopen(outlist, "wb");
      vstr_fprint(pf, vs);
      fclose(pf);
    }
    vstr_free(vs);
    fclose(pf);
  }
  if (1) {
    FILE* pf = fopen(file, "rb");
    char buf[1024];
    char buf2[256];
    char gbbuf[1024];
    wchar_t wbuf[256];
    size_t* cnt = (size_t*)malloc(sizeof(size_t)*(1 << 16));
    wchar_t* idx = (wchar_t*)malloc(sizeof(wchar_t)*(1 << 16));
    int j = 0;
    memset(cnt, 0, sizeof(size_t)*(1 << 16));
    idcard id[1] = { 0 };
    for (size_t i = 0; i < (1 << 16); ++i) {
      idx[i] = i;
    }
    for (j = 0; fgets(buf, 1024, pf) > 0 && j < 10000000; ++j) {
      int len = strlen(buf);
      len = iconv_c(ICONV_UTF8, ICONV_GB2312, buf, len, gbbuf, 1024);
      gbbuf[len] = 0;
      buf2[0] = 0;
      get_idcard(gbbuf, id);
      strcat(buf2, id->name);
      strcat(buf2, id->address);
      len = iconv_c(ICONV_GB2312, ICONV_UCS2LE, buf2, -1, (char*)wbuf, 1024) / 2;
      wbuf[len] = 0;
      for (int i = 0; i < len; ++i) {
        if (L'爫' == wbuf[i]) {
          printf("%s\n", buf2);
        }
        cnt[wbuf[i]]++;
      }
      //printf("%s", gbbuf);
    }
#define IDXGRT(a, b)  (cnt[a]>cnt[b])
    QSORT(idx, idx + (1 << 16), IDXGRT, wchar_t);
#undef IDXGRT
    size_t i = 0;
    FILE* p = fopen("D:\\out.txt", "wb");
    for (i = 0; i < (1 << 16) && cnt[idx[i]]>2; ++i) {
      char buf3[8] = { 0 };
      wchar_t ww = idx[i];
      int len = iconv_c(ICONV_UCS2LE, ICONV_GB2312, (char*)&ww, 2, (char*)buf3, 8);
      buf3[len] = 0;
      printf("%3d %s\n", i, buf3);
      if (ww > 256) {
        fprintf(p, "%s", buf3);
      }
    }
    fclose(p);
    free(idx);
    free(cnt);
    fclose(pf);
  }
  return 0;
}
