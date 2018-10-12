
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

#if 0
#include "boost/shared_ptr.hpp"
#include <boost/thread.hpp>
//#define SHARED_PTR boost::shared_ptr
//#define WEAK_PTR boost::weak_ptr
#else
#define SHARED_PTR std::shared_ptr
#define WEAK_PTR std::weak_ptr
#endif

#ifdef WITH_PYTHON_LAYER
#include <boost/get_pointer.hpp>
#endif
//#include <boost/get_pointer.hpp>
//#define GET_POINTER_DWA20021219_HPP


#ifdef _MSC_VER
#endif

#pragma warning(disable:4819) //È«²¿¹Øµô
