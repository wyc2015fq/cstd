
#include "test_3dTrack.inl"
#include "..\window.inl"
#include "..\color.inl"
#include "..\imdraw.inl"

int main() {
#if 1
  test_3dTrack();
#else
  test_GMM3DTrack2();
  test_GMM3DTrack();
  test_3dTrack_cam();
#endif
  return 0;
}
