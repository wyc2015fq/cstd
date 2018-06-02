
#include "cstd.h"
//#include "test_base64.inl"

#include "FastColoredTextBox/libFastColoredTextBox.inl"
int test_FastColoredTextBox() {
  //test_base64();
  Place pp(12, 12);
  string ss = pp.ToString();
  string sss = ss;
  printf("%s", ss.c_str());
  return 0;
}
