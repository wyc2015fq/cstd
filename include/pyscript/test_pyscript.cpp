
#include "scanner.h"
#include "pyscript.h"
#include "cfile.h"

int test_python() {
  buf_t bf[1] = { 0 };
  const char* fn = "D:/code/git/cstd/py/py100/001.py";
  buf_load(fn, bf);
  PyScanner t = PyScanner_New((char*)bf->data, bf->len);
  PyErr err;
  for (; (err = PyScanner_next(&t))==PyErr_OK;) {
    printf("%s:%s %.*s\n", pyerrmsg[err], pytokopstr[t.t.t], t.t.l, t.t.s);
  }
  printf("%s\n", bf->data);
  bffree(bf);
  return 0;
}

