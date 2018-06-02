#include "math/var.inl"
int test_var()
{
  var_t aa[1] = {0}, cc[1] = {0}, bb[1] = {0};
  int tt = sizeof(var_t);
  //int tt1=sizeof(arr1_t);
  int tt2 = sizeof(arr2_t);
  var_set(aa, CC_8F, f8, 100);
  var_a2_setsize(aa, CC_1U, 10, 10);
  var_a2_setsize(bb, CC_1U, 10, 10);
  memset(aa->a2->u1, 1, 10 * 10);
  memset(bb->a2->u1, 3, 10 * 10);
  var_add(aa, bb);
  var_printf(aa);
  var_free(aa);
  var_free(bb);
  var_free(cc);
  return 0;
}

