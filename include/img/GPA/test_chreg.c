
#include "chreg.h"



int test_chreg() {
  chreg_regsave("test");
  assert(chreg_regload("test"));
  return 0;
}

int main() {
  test_chreg();
  return 0;
}
