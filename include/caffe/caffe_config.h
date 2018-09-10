
#ifdef _WIN64
#define WIN64
#endif

//#define __CUDACC_VER__ 9200

//#define WITH_PYTHON_LAYER
#define CPU_ONLY
#ifndef CPU_ONLY
#define USE_CUDNN
#endif
#define USE_WARP_CTC
#define USE_OPENCV
#define USE_LMDB
//#define USE_LEVELDB

#ifdef _MSC_VER
#endif

#pragma warning(disable:4819) //È«²¿¹Øµô
