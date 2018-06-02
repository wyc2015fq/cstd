#include "cstd.h"
#include <iostream>
#define ACCESS_MASK ACCESS_MASK1
#include "TrainDetector.hpp"
using namespace std;

#undef ACCESS_MASK

#include "common.inl"
#include "LearnGAB.inl"
#include "TrainDetector.inl"




#ifdef _WIN64
#pragma comment(lib,"opencv_world320.lib")
#pragma comment(lib,"opencv_ts320.lib")
#pragma comment(lib,"strmiids.lib")
#else
#ifdef _DEBUG
#pragma comment(lib,"opencv_world300d.lib")
#else
#pragma comment(lib,"opencv_world300.lib")
#endif
#endif




/*! \breif command help */
static const char help[] = "NPD\n\n"
"test:   test FDDB data\n"
"live:   live demo with camera support\n";

/*!
 * \breif Command Dispatch
 */
int test_npd_detection(int argc, char* argv[]){
  TrainDetector dector;
  sys_chdir("E:/code/cstd/include/facelib/NPD/detection");
  dector.Live();
  return 0;
  if (argc != 2) {
    printf(help);
  }
  else if (strcmp(argv[1], "test") == 0) {
    dector.Detect();
  }
  else if (strcmp(argv[1], "fddb") == 0) {
    dector.FddbDetect();
  }
  else if (strcmp(argv[1], "live") == 0) {
    dector.Live();
  }
  return 0;
}
