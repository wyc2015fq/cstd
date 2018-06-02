
#if 1
#include "d3.h"
#include "d3.inl"
#include "layout_force.inl"
#include "geom_hull.inl"
#include "geom_quadtree.inl"
#else
int layout_force(ctrl_t* c) {
  return 0;
}
#endif

#define TESTD3DEF_DEF(TESTDEF) \
  TESTDEF(geom_hull) \
  TESTDEF(geom_quadtree) \
  TESTDEF(layout_force) \
  
int test_d3(ctrl_t* c) {
  const char* s_test = "|"
#define TESTDEF(FUN)   #FUN "|"
    TESTD3DEF_DEF(TESTDEF)
#undef TESTDEF
    ;
  const test_draw_f test_agg_fun[] = {
#define TESTDEF(FUN)   FUN,
    TESTD3DEF_DEF(TESTDEF)
#undef TESTDEF
  };
  test_draw_impl(c, test_agg_fun, countof(test_agg_fun), s_test);
  //tictoc(NULL);
  return 0;
}
