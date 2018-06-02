// 求两帧的距离，Frame1和Frame2必须为结构体，且其域名为Centroid
// 返回的矩阵d是两两之间的距离
int Frame_Euler(int N1, POINT* Frame1, int N2, POINT* Frame2, double* d, int* Index32)
{
  int i, j;

  for (j = 0; j < N1; ++j) {
    double t = 9999999;
    Index32[j] = -1;

    for (i = 0; i < N2; ++i) {
      int x = Frame2[i].x - Frame1[j].x, y = Frame2[i].y - Frame1[j].y;
      d[i * N1 + j] = sqrt((double)x * x + y * y);

      if (d[i * N1 + j] < t) {
        // 按列求最小值,其中行数表示后一帧图像的目标数目
        // 列数表示前一帧图像的目标数目，返回的索引是对第3帧的索引号
        Index32[j] = i;
        t = d[i * N1 + j];
      }
    }
  }

  return 0;
}
