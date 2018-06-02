
//#include "test_x11.inl"

#include "VideoSDK.inl"
#include "thread.h"
#include "test_net.inl"
#include "test_udp.inl"


int main(int argc, char* agrv[])
{
#if 1
  test_VideoSDK();
#else
  test_udp();
  main11();
  test_net();
  test_thread();
#endif
  return 0;
}
