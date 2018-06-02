// 用于外圆的极坐标转化
int iris_to_polar_coordinate(buf_t* bf, const img_t* pic, int x0, int y0, int Rmax, int pic_xsize, int pic_ysize, img_t* polarv)
{
  // 定义极坐标图像大小 与 相位间距（分360份）
  int ii, i, j, num_i, np = 446;//, dp = PI / 180;
  const uchar* pos;

  bf_imsetsize(bf, polarv, np, Rmax, 1, 1);
  num_i = 1;

  for (i = 1; i <= 45; i += 2) {      // 在0到44°间隔为2°，进行变化
    for (j = 1; j <= Rmax; ++j) {
#include "iris_to_polar_coordinate_for.inl"
    }

    num_i = num_i + 1;
  }

  for (ii = 46 * 2; ii <= 134 * 2; ii += 1) { // 在0到44°间隔为2°，进行变化
    i = ii / 2;

    for (j = 1; j < Rmax; ++j) {
#include "iris_to_polar_coordinate_for.inl"
    }

    num_i = num_i + 1;
  }

  for (i = 135; i <= 225; i += 2) {       // 在0到44°间隔为2°，进行变化
    for (j = 1; j < Rmax; ++j) {
#include "iris_to_polar_coordinate_for.inl"
    }

    num_i = num_i + 1;
  }

  for (ii = 226 * 2; ii <= 314 * 2; ii += 1) { // 在0到44°间隔为2°，进行变化
    i = ii / 2;

    for (j = 1; j < Rmax; ++j) {
#include "iris_to_polar_coordinate_for.inl"
    }

    num_i = num_i + 1;
  }

  for (i = 315; i < 360; i += 2) { // 在0到44°间隔为2°，进行变化
    for (j = 1; j < Rmax; ++j) {
#include "iris_to_polar_coordinate_for.inl"
    }

    num_i = num_i + 1;
  }

  ASSERT(num_i == np + 1);
  return 0;
}
// 显示极坐标下的图像
// figure; imshow(polarv,[]);
