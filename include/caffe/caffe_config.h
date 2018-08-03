
#ifdef _WIN64
#define WIN64
#endif

//#define CPU_ONLY
#ifndef CPU_ONLY
#define USE_CUDNN
#endif
#define USE_WARP_CTC
#define USE_OPENCV
#define USE_LMDB
#define USE_LEVELDB
