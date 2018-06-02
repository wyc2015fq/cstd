{
  switch (rows) {
  case 2: {
    enum {__cols = 2};
#define PIXADDX(_i)
#include "impl/imresize_area_case_cols.inl"
  }
  break;

  default:
    ASSERT(0);
    break;
  }
}


switch (cols)
{
case 2: {
  enum {__cols = 2};
#define PIXADDX(_i)
#include "impl/imresize_area_case_cols.inl"
}
break;

default:
  ASSERT(0);
  break;
}


