
#ifndef _CPU_HPP_
#define _CPU_HPP_

#include "wstd/logging.hpp"
#include "rng.hpp"
#include <random>
#include <ctime>
using namespace std;

#define NOT_IMPLEMENTED
enum Brew { CPU, GPU };

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

struct CPUContext {
  //CPUContext() { context[CPU] = this;    }
  static void* ReAlloc(void* ptr, size_t size) { return ptr = realloc(ptr, size); }
  static void Memset(size_t nbytes, void* ptr) { memset(ptr, 0, nbytes); }
  static void Memcpy(size_t nbytes, void* dst, const void* src) { memcpy(dst, src, nbytes); }
  static void Free(void* data) { free(data); }
  static void MemcpyAsync(size_t nbytes, void* dst, const void* src) { NOT_IMPLEMENTED; }
};


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


//#define CPU_ONLY
#ifdef CPU_ONLY
#include "cpu_only.hpp"
typedef CPUContext Context;
#else
#define USE_GPU
#include "cuda.hpp"
typedef GPUContext Context;
#endif

#define CPUCONTEXT (CPUContext*)0
#define GPUCONTEXT (GPUContext*)0
#define CONTEXT (Context*)0

#endif // _CPU_HPP_
