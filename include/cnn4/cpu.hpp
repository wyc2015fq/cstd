
#ifndef _CPU_HPP_
#define _CPU_HPP_

#include "wstd/logging.hpp"
#include "rng.hpp"
#include <random>
#include <ctime>
using namespace std;

#define NOT_IMPLEMENTED
enum Brew { CPU, GPU };

#define CPU_KERNEL_LOOP(i, n)  for (int i = 0; i < (n); ++i )

#if 0
struct Context {
  virtual void* ReAlloc(void* ptr, size_t nbytes) = 0;
  virtual void Memset(size_t nbytes, void* ptr) = 0;
  virtual void Memcpy(size_t nbytes, void* dst, const void* src) = 0;
  virtual void Free(void* data) = 0;
  virtual void MemcpyAsync(size_t nbytes, void* dst, const void* src) = 0;
};
#endif

//Context* context[2] = {0};

struct Buffer {
  void* data;
  int size;
};

typedef float(*f32_10_t)[100];

#define CONTEXTDEF   \
int size;  \
union { \
  void* data; \
  float* fl; \
  double* db; \
  int* i; \
  char* str; \
  unsigned char* bytes; \
  f32_10_t f100_; \
}

struct CPUContext {
  CONTEXTDEF;
};

//CPUContext() { context[CPU] = this;    }
static void Memcpy(CPUContext* dst, const CPUContext* src, int nbytes) {
  CHECK_LE(nbytes, dst->size);
  CHECK_LE(nbytes, dst->size);
  memcpy(dst->data, src->data, nbytes);
}
static void Free(CPUContext* ptr) {
  if (ptr->data) {
    free(ptr->data);
    ptr->data = NULL;
    ptr->size = 0;
  }
}
static void ReAlloc(CPUContext* ptr, size_t nbytes) {
  if (ptr->size<nbytes) {
    ptr->data = realloc(ptr->data, ptr->size = nbytes);
  }
}
static void Memset(CPUContext* ptr, size_t nbytes) {
  CHECK_LE(nbytes, ptr->size);
  memset(ptr->data, 0, ptr->size);
}
#if 0
static void MemcpyAsync(size_t nbytes, CPUContext* dst, const void* src, void* stream) { memcpy(dst, src, nbytes); }
//static CPUContext* BeginStream(CPUContext* ptr) { return 0; }
//static void Synchronize(CPUContext* ptr);
//static void EndStream(CPUContext* ptr) {}
#endif


//#define mode()  globel.mode_
struct Globel
{
  //SHARED_PTR<RNG> random_generator_;
  Brew mode_;
  // Parallel training
  int solver_count_;
  int solver_rank_;
  bool multiprocess_;
  Globel() {
    Globel* s = this;
    //s->mode_ = (CPU);
    s->solver_count_ = (1);
    s->solver_rank_ = (0);
    s->multiprocess_ = (false);
  }
  ~Globel() {}
  static Globel& Get() {
    static Globel globel_;
    return globel_;
  }
};
#define globel Globel::Get()
#define mode()  globel.mode_

inline static int solver_count() { return globel.solver_count_; }
inline static void set_solver_count(int val) { globel.solver_count_ = val; }
inline static int solver_rank() { return globel.solver_rank_; }
inline static void set_solver_rank(int val) { globel.solver_rank_ = val; }
inline static bool multiprocess() { return globel.multiprocess_; }
inline static void set_multiprocess(bool val) { globel.multiprocess_ = val; }
inline static bool root_solver() { return globel.solver_rank_ == 0; }


 
#ifdef CPU_ONLY
//#include "cpu_only.hpp"
typedef CPUContext Context;
//typedef GPUContext Context;
struct GPUContext : public CPUContext {};
#else
#include "cuda.hpp"
typedef GPUContext Context;
#endif

#define CPUCONTEXT (CPUContext*)0
#define GPUCONTEXT (GPUContext*)0
#define CONTEXT (Context*)0

#endif // _CPU_HPP_
