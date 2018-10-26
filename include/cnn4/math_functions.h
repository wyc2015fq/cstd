
#include "cblas.hpp"
//#include "cpu.hpp"

struct CPUContext;
struct GPUContext;

#define _CONTEXT CPUContext* context
#include "math_functions_impl.h"
#undef _CONTEXT
#define _CONTEXT GPUContext* context
#include "math_functions_impl.h"
#undef _CONTEXT
