
#include "cblas.hpp"
#include "types.h"
//#include "cpu.hpp"

struct CPUContext;
struct GPUContext;

#define _CONTEXT CPUContext* context
#include "math_functions_impl.h"
#undef _CONTEXT
#ifndef CPU_ONLY
#define _CONTEXT GPUContext* context
#include "math_functions_impl.h"
#undef _CONTEXT
#endif

template <typename Dtype, typename Context>
struct BufData : public Context {
  ~BufData() { Free(this); }
  BufData(int count, Dtype x) {
    memset(this, 0, sizeof(*this));
    Context* ptr = this;
    ReAlloc(ptr, count * sizeof(Dtype));
    caffe_set<Dtype>(ptr, count, x, (Dtype*)ptr->data);
  }
  const Dtype* get() const { return (const Dtype*)data; }
};

