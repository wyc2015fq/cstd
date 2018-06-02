#include "include/graphics.h"


#define PICNAME _T("D:\\bak\\pub\\pic\\aa_08.jpg")


#include "liquid.inl"
#include "android.inl"
#include "doraemon.inl"
#include "visual_illusion1.inl"
//#include "visual_illusion2.inl"
#include "visual_illusion3.inl"
#include "visual_illusion4.inl"
#include "PieMinimumRect.inl"
#include "Lighting.inl"
#include "mosaic.inl"
#include "blur.inl"
#include "spring.inl"
#include "ball.inl"
#include "beatles.inl"
#include "rose.inl"
#include "fire.inl"
#include "ChangePal.inl"
#include "waveline.inl"
#include "RoseCurve.inl"
#include "iceBlur.inl"
#include "life.inl"
#include "free.inl"
#include "pixel.inl"
#include "arc.inl"
#include "wanhuachi.inl"
#include "Mystify.inl"
#include "clock.inl"
#include "heat.inl"
#include "ball3d.inl"
#include "rainbow.inl"
#include "mouse.inl"
#include "star.inl"
#include "chrmatrix.inl"

#pragma comment(lib,"easyx6.lib")

int test_easyx() {
  test_android();
  test_liquid();
  test_chrmatrix();
  test_star();
  test_mouse();
  test_rainbow();
  test_ball3d();
  test_heat();
  test_clock();
  test_Mystify();
  test_wanhuachi();
  test_arc();
  test_pixel();
  test_free();
  test_life();
  test_iceBlur();
  test_RoseCurve();
  test_waveline();
  test_ChangePal();
  test_fire();
  test_beatles();
  test_rose();
  test_ball();
  test_spring();
  test_blur();
  test_mosaic();
  test_Lighting();
  test_PieMinimumRect();
  test_visual_illusion1();
  //test_visual_illusion2();
  test_visual_illusion3();
  test_visual_illusion4();
  test_doraemon();
  return 0;
}
