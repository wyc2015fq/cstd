
#pragma once

/**
 * define if you want to use intel TBB library
 */
//#define CNN_USE_TBB

/**
 * define to enable avx vectorization
 */
//#define CNN_USE_AVX

/**
 * define to enable sse2 vectorization
 */
//#define CNN_USE_SSE

/**
 * number of task in batch-gradient-descent.
 * @todo automatic optimization
 */
#define CNN_TASK_SIZE 8
