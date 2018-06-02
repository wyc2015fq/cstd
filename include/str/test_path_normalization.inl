
#include "cstd.h"
#include "cfile.h"
//#include "math/rand.inl"

int rand_path(char* path, int len, int maxnamelen) {
  //char name[16];
  //maxnamelen = MIN(maxnamelen, countof(name)-1);
  for (;;) {
    //randstr(NULL, name, maxnamelen, NULL);
  }
  return 0;
}

int vstr_sort_alphanum(vstr_t* sv, int ignore_case) {
//#define STR_LESS(a, b)   ((a).l!=(b).l ? ((a).l<(b).l):str_cmp(a, b, ignore_case)<0)
//#define STR_LESS(a, b)   (str_cmp(a, b, ignore_case)<0)
#define STR_LESS(a, b)   (cstr_cmp_alphanum((a).s, (b).s, ignore_case)<0)
  QSORT(sv->v, sv->v+sv->n, STR_LESS, str_t);
#undef STR_LESS
  return 0;
}

int test_cstr_cmp_alphanum() {
  char* files[] = {
"z1.doc",
"z10.doc",
"z100.doc",
"z101.doc",
"z102.doc",
"z11.doc",
"z12.doc",
"z13.doc",
"z14.doc",
"z15.doc",
"z16.doc",
"z17.doc",
"z18.doc",
"z19.doc",
"z2.doc",
"z20.doc",
"z3.doc",
"z4.doc",
"z5.doc",
"z6.doc",
"z7.doc",
"z8.doc",
"z9.doc",
};
  int i, ignore_case = 1;
#define STR_LESS(a, b)   (cstr_cmp_alphanum((a), (b), ignore_case)<0)
  QSORT(files, files+countof(files), STR_LESS, char*);
#undef STR_LESS
  for (i=0;i<countof(files); ++i) {
    printf("%s\n", files[i]);
  }
  return 0;
}

int test_path_normalization() {
  char* aa[] = {
    "aaa/...bbb.././..ccc.././..ddd../../../..eee./a/","aaa/...bbb../..eee./a/",
    //"aa/\../", "",
    "../././.././../", "../../../",
    "././", "",
    "aa../", "aa../",
    "../", "../",
    "../././.././../.aaa./../././...bbb/./ccc/./ddd/../../eee./a/", "../../../...bbb/eee./a/",
  };
  char buf[256];
  int i, n=countof(aa)/2;
  for (i=0; i<n; ++i) {
    path_normalization2(aa[i*2], -1, buf);
    ASSERT(0==strcmp(buf, aa[i*2+1]));
  }
  return 0;
}
