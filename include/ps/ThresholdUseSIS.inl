//基于Simple Image Statistics（简单图像统计,SIS)的图像二值化算法。
// 这是个简单的算法，是全局二值算法的一种，算法执行速度快。
//  算法过程简单描述如下：
// 对于每一个像素，做如下处理
// 1、计算当前像素水平和垂直方向的梯度。 （two gradients are calculated  |I(x + 1, y) - I(x - 1, y)| and |I(x, y + 1) - I(x, y - 1)|);
// 2、取两个梯度的最大值作为权重。（weight is calculated as maximum of two gradients）;
// 3、更新权重的和。（sum of weights is updated (weightTotal += weight)）;
// 4、更新加权像素之和 （sum of weighted pixel values is updated (total += weight * I(x, y))）；
//   之后，最终的阈值去加权像素之和和权重之和相除的值。
//   关于填充数据，我还是喜欢自己分配内存，而且我更倾向于直接使用API，这个可能与个人习惯有关吧，你们也可以按照自己的方式来处理。

byte GetSimpleStatisticsThreshold(int Width, int Height, const byte* Scan0, int Stride)
{
  int X, Y;
  int Ex, Ey;
  int Weight = 0;
  long SumWeight = 0;                 //  对于大图像这个数字会溢出，所以用long类型的变量

  for (Y = 1; Y < Height - 1; Y++) {
    const byte* Pointer = Scan0 + (Y + 1) * Stride + 1;

    for (X = 1; X < Width - 1; X++) {
      Ex = *(Pointer - 1) - *(Pointer + 1);

      if (Ex < 0) {
        Ex = -Ex;
      }

      Ey = *(Pointer - Stride) - *(Pointer + Stride);

      if (Ey < 0) {
        Ey = -Ey;
      }

      if (Ex > Ey) {
        Weight += Ex;
        SumWeight += *Pointer * Ex;
      }
      else {
        Weight += Ey;
        SumWeight += *Pointer * Ey;
      }

      Pointer++;
    }
  }

  if (Weight == 0) {
    return *(Scan0);  //  说明所有的颜色值都相同
  }

  return (byte)(SumWeight / Weight);
}

