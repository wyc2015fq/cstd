int cn = MIN(ai, bi);

switch (cn)
{
#if 0

case 1: {
  enum {__cn = 1};
  typedef uchar arrtype;
#define OPTREPT(_i, OP)   {OP(_i*__cn+0);}
#include "imresize_case_cn.inl"
}
break;
#else

case 1: {
  enum {__cn = 1};
  typedef uchar arrtype;
#define OPTREPT(_i, OP)   {OP(_i*__cn+0);}
#include "imresize_case_cn.inl"
}
break;

case 2: {
  enum {__cn = 3};
  typedef ushort arrtype;
#define OPTREPT(_i, OP)   {OP(_i*__cn+0);OP(_i*__cn+1);}
#include "imresize_case_cn.inl"
}
break;

case 3: {
  enum {__cn = 3};
  typedef struct {
    uchar t[3];
  } arrtype;
#define OPTREPT(_i, OP)   {OP(_i*__cn+0);OP(_i*__cn+1);OP(_i*__cn+2);}
#include "imresize_case_cn.inl"
}
break;

case 4: {
  enum {__cn = 4};
  typedef uint arrtype;
#define OPTREPT(_i, OP)   {OP(_i*__cn+0);OP(_i*__cn+1);OP(_i*__cn+2);OP(_i*__cn+3);}
#include "imresize_case_cn.inl"
}
break;
#endif

default:
  //ASSERT(0);
  break;
}

#undef IMMALLOC
#undef IMFREE
