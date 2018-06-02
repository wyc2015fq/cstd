// 1、基于图像分析的偏色检测及颜色校正方法 徐晓昭 , 蔡轶珩 , 刘长江 , 贾克斌 , 沈兰荪
// 2、皮肤检测技术的研究及改进
// 为方便大家理解，这里还是从这些论文里摘取些具体的过程予以描述。
// 　　RGB颜色空间是最简单的一种颜色空间，但是RGB颜色空间最大的局限性在于当用欧氏距离来刻画两种颜色之间的差异时，所计算出的两种颜色之间的距无法正确表征人们实际所感知到的这两种颜色之间的真实差异。采用CIE Lab颜色空间，此空间所计算出来的颜色之间的距离与实际感知上的差别基本一致。其直方图可以客观的反映图像色偏程度，在CIE Lab下进行偏色图像的自动检测更为合理。
//       经过对正常图像和偏色图像的分析发现，如果在ab色度坐标平面上的直方图中，色度分布基本上为单峰值，或者分布较为集中，而色度平均值D又较大时，一般都存在色偏，而且色度平均值越大，色偏越严重。然而，如果在ab色度坐标平面上的直方图中色度分布存在明显的多峰值，而且较为分散时，图像色偏程度将大大减轻，甚至没有色偏。
double GetColorCastFactor(int Width, int Height, const byte* BmpData, int Stride)
{
  int X, Y;

  int SumA = 0, SumB = 0;
  double MsqA = 0, MsqB = 0, AvgA, AvgB;
  int A, B;

  int HistA[256] = {0};
  int HistB[256] = {0};
  int LabStride = Width * 3;
  byte Pointer[4];

  for (Y = 0; Y < Height; Y++) {
    for (X = 0; X < Width; X++) {
      RGBToLAB(BmpData + Y * Stride + X * 3, Pointer, 1);
      A = *(Pointer + 1);
      B = *(Pointer + 2);
      SumA += A;
      SumB += B;
      HistA[A]++;
      HistB[B]++;
    }
  }

  AvgA = 1.*SumA / (Width * Height) - 128;                  // 必须归一化到[-128，,127]范围内
  AvgB = 1.*SumB / (Width * Height) - 128;

  for (Y = 0; Y < 256; Y++) {
    MsqA += (fabs(Y - AvgA - 128) * HistA[Y]) / (Width * Height);           // 用方差的方式结果有问题
    MsqB += (fabs(Y - AvgB - 128) * HistB[Y]) / (Width * Height);
  }

  return sqrt(AvgA * AvgA + AvgB * AvgB) / sqrt(MsqA * MsqA + MsqB * MsqB);
}

