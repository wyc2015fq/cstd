
// PhotoShop算法原理解析系列 - 像素化---》碎片
// 将图像创建四个相互偏移的副本，产生类似重影的效果
// 偏移的中心就是以每个像素为中心，4个偏移分别以中心对称，斜45度均匀圆周布置，
// 水平和垂直偏移各45度，偏移量4个像素。
int fragment(int h, int w, const byte* A, int al, byte* B, int bl, int ai)
{
  int x, y, z, xx, yy;
  int Speed, Index;
  int SumR, SumG, SumB;

  // 每个点的偏移量
  int OffsetX[] = { 4, -4, -4, 4 };
  int OffsetY[] = { -4, -4, 4, 4 };

  ASSERT(ai >= 3);

  for (y = 0; y < h; y++) {
    Speed = y * bl;

    for (x = 0; x < w; x++) {
      SumB = 0;
      SumG = 0;
      SumR = 0;

      for (z = 0; z < 4; z++) {
        //  累积取样点的取样和
        xx = x + OffsetX[z], yy = y + OffsetY[z];
        // 注意越界
        xx = CLAMP(xx, 0, w - 1), yy = CLAMP(yy, 0, h - 1);
        Index = yy * al + xx * 3;
        SumB += A[Index + 0];
        SumG += A[Index + 1];
        SumR += A[Index + 2];
      }

      //  求平均值(Sum+2)/4,为什么要+2，就为了四舍五入。比如如果计算结果为108.6，则取像素109更为合理
      B[Speed + 0] = ((SumB + 2) >> 2);
      B[Speed + 1] = ((SumG + 2) >> 2);
      B[Speed + 2] = ((SumR + 2) >> 2);
      //  跳往下一个像素
      Speed += ai;
    }
  }

  return 0;
}
