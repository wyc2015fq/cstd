#define CV_CPU_SIMD_FILENAME "opencv2/core/mathfuncs_core.simd.hpp"

#undef CV_CPU_DISPATCH_MODE
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"

#define CV_CPU_DISPATCH_MODE SSE2
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"

#define CV_CPU_DISPATCH_MODE AVX
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"

#define CV_CPU_DISPATCH_MODE AVX2
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"

#define CV_CPU_DISPATCH_MODES_ALL AVX2, AVX, SSE2, BASELINE
