
#include "json.inl"
#include "cfile.h"

int test_json2() {
  str_t s[1] = {0};
  sys_chdir("E:/pub/bin/json");
  if (0) {
    dstr_t* ds = NULL;
    char* aaa[] = {
      "menu|id",
        "menu|popup|menuitem|2|value",
        "menu|popup|menuitem|1|onclick",
    };
    int i, j=0, n = 0;
    str_load("test2.json", s);
    //str_loadasni("china.json", s);
    n = parse_json(&ds, s->s, s->l);
    dstr_print(ds, 0, MIN(n, 100));
    printf("\n");
    for (i = 0; i < countof(aaa); ++i) {
      j = dstr_findname(ds, n, aaa[i]);
      if (j >= 0 && j < n - 1) {
        dstr_print(ds, j, n);
      }
    }
    FREE(ds);
    printf("\n");
  }
  if (1) {
    dstr_t* ds = NULL;
    char* aaa[] = {
      "features|2|properties|name",
        "features|2|geometry|coordinates|0|0|0|0",
    };
    int i, j=0, n = 0;
    str_loadasni("china.json", s);
    n = parse_json(&ds, s->s, s->l);
    dstr_print(ds, 0, MIN(n, 100));
    printf("\n");
    for (i = 0; i < countof(aaa); ++i) {
      j = dstr_findname(ds, n, aaa[i]);
      if (j >= 0 && j < n - 1) {
        dstr_print(ds, j, n);
      }
    }
    FREE(ds);
    printf("\n");
  }
  return 0;
}
