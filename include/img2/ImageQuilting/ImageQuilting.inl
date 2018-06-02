#include "Core.inl"
#include "Utility.inl"
///     编写者： laviewpbt, 编写时间： 2015.9 - 10， 联系QQ: 33184777
/// <summary>
///     简单高效的灰度化算法。
/// </summary>
/// <param name="Src">需要处理的源图像的数据结构。</param>
/// <param name="Dest">保存处理后的图像的数据结构。</param>
///     <remarks> Delta未在LAB空间进行，只在RGB空间做的处理,处理速度和图像大小关系不大，和Level成线性关系。</remarks>
IS_RET __stdcall ClassicDecolorization(TMatrix* Src, TMatrix* Dest)
{
  if (Src == NULL || Dest == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width != Dest->Width || Src->Height != Dest->Height || Src->Depth != Dest->Depth) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_8U) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (Src->Channel != 3 || (Dest->Channel != 3 && Dest->Channel != 1)) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  int X, Y, Width = Src->Width, Height = Src->Height;
  unsigned char* LinePS, *LinePD;
  for (Y = 0; Y < Height; Y++) {
    LinePS = Src->Data + Y * Src->WidthStep;
    LinePD = Dest->Data + Y * Dest->WidthStep;
    for (X = 0; X < Width; X++) {
      LinePD[X] = (LinePS[0] + LinePS[1] + LinePS[1] + LinePS[2]) / 4;
      LinePS += 3;
    }
  }
  return IS_RET_OK;
}
/// <summary>
/// 针对本函数写的矩阵数据转置算法，只支持单通道的byte及int类型矩阵.（2015.9.10日）
/// </summary>
/// <param name="Src">需要处理的源图像的数据结构。</param>
/// <param name="Dest">保存处理后的图像的数据结构。</param>
/// <remarks> 1: Src和Dest不能相同。</remarks>
IS_RET TransposeMatrix(TMatrix* Src, TMatrix* Dest)
{
  if (Src == NULL || Dest == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width != Dest->Height || Src->Height != Dest->Width || Src->Channel != Dest->Channel || Src->Depth != Dest->Depth) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Src->Depth != IS_DEPTH_8U && Src->Depth != IS_DEPTH_32S) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (Src->Channel != 1) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  unsigned char* LinePS, *LinePD;
  int X, Y, Width, Height, StrideS, StrideD, *LinePSI, *LinePDI;
  Width = Dest->Width, Height = Dest->Height, StrideS = Src->WidthStep;
  if (Src->Depth == IS_DEPTH_8U) {
    for (Y = 0; Y < Height; Y++) {
      LinePS = Src->Data + Y;
      LinePD = Dest->Data + Y * Dest->WidthStep;
      for (X = 0; X < Width; X++) {
        LinePD[X] = LinePS[0];
        LinePS += StrideS;
      }
    }
  }
  else {
    for (Y = 0; Y < Height; Y++) {
      LinePSI = (int*)Src->Data + Y;
      LinePDI = (int*)(Dest->Data + Y * Dest->WidthStep);
      for (X = 0; X < Width; X++) {
        LinePDI[X] = LinePSI[0];
        LinePSI += Height;
      }
    }
  }
  return IS_RET_OK;
}
/// <summary>
/// 计算沿着垂直方向的局部最小误差路径，只支持单通道的数据矩阵.（2015.9.10日）
/// </summary>
/// <param name="Src">需要从中提取路径的数据,必须是int类型矩阵。</param>
/// <param name="Dest">得到的路径，0表示保留的部分，255表示需要替换掉的部分，必须是byte型图像数据。</param>
/// <remarks> 1:原始的算法路径虽然是优化，但是还是 。</remarks>
IS_RET MinCutVertical(TMatrix* Src, TMatrix* Dest)
{
  if (Src == NULL || Dest == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width != Dest->Width || Src->Height != Dest->Height || Src->Channel != Dest->Channel) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Src->Depth != IS_DEPTH_32S || Dest->Depth != IS_DEPTH_8U) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (Src->Channel != 1) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  IS_RET Ret = IS_RET_OK;
  int X, Y, Index, MinValue, Width, Height, *LinePL, *LinePD, *LinePS;
  unsigned char* LinePM;
  TMatrix* Energy = NULL;
  Width = Src->Width, Height = Src->Height;
  Ret = IS_CreateMatrix(Src->Width, Src->Height, IS_DEPTH_32S, 1, &Energy);                               //      分配数据
  if (Ret != IS_RET_OK) {
    return Ret;
  }
  memcpy(Energy->Data, Src->Data, Src->WidthStep);                                                                                //      第一行的能量就是原始数据的第一行
  LinePL = (int*)Energy->Data;                                                                                                                    //      上一行的能量
  LinePD = (int*)(Energy->Data + Energy->WidthStep);                                                                              //      当前行的累积能量（待计算）
  for (Y = 1; Y < Height; Y++) {                                                                                                                          //      for i=2:size(E,1), 从第二行开始
    LinePS = (int*)(Src->Data + Y * Src->WidthStep);                                                                        //      当前行的单个能量值
    LinePD[0] = LinePS[0] + MIN(LinePL[0] , LinePL[1]);                                                                     //      E(i,1) = X(i,1) + MIN( E(i-1,1), E(i-1,2) );
    for (X = 1; X < Width - 1; X++) {                                                                                                       //      for j=2:size(E,2)-1
      LinePD[X] = LinePS[X] + MIN(MIN(LinePL[X - 1], LinePL[X]), LinePL[X + 1]);              //      E(i,j) = X(i,j) + MIN( [E(i-1,j-1), E(i-1,j), E(i-1,j+1)] );
    }
    LinePD[Width - 1] = LinePS[Width - 1] + MIN(LinePL[Width - 2], LinePL[Width - 1]);      //      E(i,end) = X(i,end) + MIN( E(i-1,end-1), E(i-1,end) );
    LinePL += Energy->Width;
    LinePD += Energy->Width;
  }
  MinValue = MaxValueI;
  LinePD = (int*)(Energy->Data + (Height - 1) * Energy->WidthStep);
  for (X = 0; X < Width; X++) {
    if (LinePD[X] < MinValue) {                                                                                                                     //      求最后一行内的最小累积能量值
      Index = X;                                                                                                                                              //      [cost, idx] = MIN(E(end, 1:end));
      MinValue = LinePD[X];
    }
  }
  LinePM = Dest->Data + (Height - 1) * Dest->WidthStep;
  for (X = 0; X < Index; X++) {
    LinePM[X] = 0;  //      C(i, 1:idx-1) = -1;
  }
  for (X = Index; X < Width; X++) {
    LinePM[X] = 255;  //      C(i, idx+1:end) = +1;
  }
  for (Y = Height - 2; Y >= 0; Y--) {                                                                                                             //      for i=size(E,1)-1:-1:1,
    LinePD = (int*)(Energy->Data + Y * Energy->WidthStep);                                                          //      从最后一行向上开始搜索其8领域（实际上只涉及上面一行的三个元素)的局部最小值
    if (Index > 0) {
      MinValue = MIN(LinePD[Index - 1], LinePD[Index]);                                                               //       if( idx > 1 && E(i,idx-1) == MIN(E(i,idx-1:MIN(idx+1,size(E,2))) ) )
      if (Index + 1 < Width) {
        MinValue = MIN(MinValue, LinePD[Index + 1]);
      }
      if (MinValue == LinePD[Index - 1]) {
        Index--;  //      idx = idx-1;
      }
      else if (MinValue == LinePD[Index + 1]) {
        Index++;
      }
    }
    else {
      if (LinePD[Index] > LinePD[Index + 1]) {
        Index++;
      }
    }
    LinePM = Dest->Data + Y * Dest->WidthStep;
    for (X = 0; X < Index; X++) {
      LinePM[X] = 0;  //      赋值为0表示不需要改变的部分，为255则表示需要用新的块的颜色来代替
    }
    for (X = Index; X < Width; X++) {
      LinePM[X] = 255;
    } ;
  }
  IS_FreeMatrix(&Energy);
  return IS_RET_OK;
}
/// <summary>
/// 计算沿着水平方向的局部最小误差路径，只支持单通道的数据矩阵.（2015.9.10日）
/// </summary>
/// <param name="Src">需要从中提取路径的数据,必须是int类型矩阵。</param>
/// <param name="Dest">得到的路径，0表示保留的部分，255表示需要替换掉的部分，必须是byte型图像数据。</param>
/// <remarks> 1:直接用水平方向的算法原理进行处理速度回比较慢，因为计算地址部分涉及到*Width,不如先转置再调用垂直方向的计算。</remarks>
/// <remarks> 2:转置的优点是两次转置后数据不会有问题，如果用旋转，比如先顺时针旋转90度，在逆时针旋转90度，得到的数据还必须进行反色处理才能获得正确的效果。</remarks>
IS_RET MinCutHorizontal(TMatrix* Src, TMatrix* Dest)
{
  TMatrix* TransposeToVertical = NULL, *TransposeMaskToVertical = NULL;
  IS_RET Ret = IS_CreateMatrix(Src->Height, Src->Width, IS_DEPTH_32S, 1, &TransposeToVertical);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(Src->Height, Src->Width, IS_DEPTH_8U, 1, &TransposeMaskToVertical);       //      分配数据
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = TransposeMatrix(Src, TransposeToVertical);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = MinCutVertical(TransposeToVertical, TransposeMaskToVertical);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = TransposeMatrix(TransposeMaskToVertical, Dest);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
Done:
  IS_FreeMatrix(&TransposeToVertical);
  IS_FreeMatrix(&TransposeMaskToVertical);
  return Ret;
}
/// <summary>
/// 计算图像的局部平方和，速度已经优化，支持1和3通道图像。（2015.9.10日）
/// </summary>
/// <param name="Src">待求平方和的图像。</param>
/// <param name="Dest">平方和数据，需要使用int类型矩阵保存。</param>
/// <param name="SizeX">在水平方向使用的模板大小，如果是半径模式，对应的量为2*Radius + 1。</param>
/// <param name="SizeY">在垂直方向使用的模板大小，如果是半径模式，对应的量为2*Radius + 1。</param>
/// <remarks> 1:使用了类似BoxBlur里的优化算法，耗时和参数基本无关。</remarks>
/// <remarks> 2:也可以使用积分图实现。</remarks>
IS_RET GetLocalSquareSum(TMatrix* Src, TMatrix* Dest, int SizeX, int SizeY)
{
  if (Src == NULL || Dest == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width != Dest->Width || Src->Height != Dest->Height || Src->Channel != Dest->Channel) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_32S) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (SizeX < 0 || SizeY < 0) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  IS_RET Ret = IS_RET_OK;
  int X, Y, Z, Width, Height, Left, Right, Top, Bottom;
  int SumRed, SumGreen, SumBlue, Sum, LastIndex, NextIndex;
  int* ColSum, *ColSumB, *ColSumG, *ColSumR, *LinePD;
  unsigned char* SamplePS, *LastAddress, *NextAddress;
  TMatrix* Row = NULL, *Col = NULL;
  Width = Src->Width, Height = Src->Height;
  Left = SizeX / 2, Right = SizeX - Left, Top = SizeY / 2, Bottom = SizeY - Top;
  ColSum = (int*)IS_AllocMemory(Width * Src->Channel * sizeof(int), true);
  if (ColSum == NULL) {
    Ret = IS_RET_ERR_OUTOFMEMORY;
    goto Done;
  }
  Ret = GetValidCoordinate(Width, Height, Left, Right, Top, Bottom, EdgeMode_Tile, &Row, &Col);                           //      获取坐标偏移量
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  if (Src->Channel == 1) {
    for (Y = 0; Y < Height; Y++) {
      LinePD = (int*)(Dest->Data + Y * Dest->WidthStep);
      if (Y == 0) {
        for (X = 0; X < Width; X++) {
          Sum = 0;
          for (Z = -Top; Z < Bottom; Z++) {
            SamplePS = Src->Data + ((int*)Col->Data)[Z + Top] * Src->WidthStep + X;
            Sum += SamplePS[0] * SamplePS[0] ;
          }
          ColSum[X] = Sum;
        }
      }
      else {
        LastAddress = Src->Data + ((int*)Col->Data)[Y - 1] * Src->WidthStep;
        NextAddress = Src->Data + ((int*)Col->Data)[Y + SizeY - 1] * Src->WidthStep;
        for (X = 0; X < Width; X++) {
          ColSum[X] -= LastAddress[0] * LastAddress[0] - NextAddress[0] * NextAddress[0];
          LastAddress++;
          NextAddress++;
        }
      }
      for (X = 0; X < Width; X++) {
        if (X == 0) {
          Sum = 0;
          for (Z = -Left; Z < Right; Z++) {
            Sum += ColSum[((int*)Row->Data)[Z + Left]];
          }
        }
        else {
          LastIndex = ((int*)Row->Data)[X - 1];
          NextIndex = ((int*)Row->Data)[X + SizeX - 1];
          Sum -= ColSum[LastIndex] - ColSum[NextIndex];
        }
        LinePD[0] = Sum;
        LinePD++;
      }
    }
  }
  else if (Src->Channel == 3) {
    ColSumB = ColSum;
    ColSumG = ColSumB + Width;
    ColSumR = ColSumG + Width;
    for (Y = 0; Y < Height; Y++) {
      LinePD = (int*)(Dest->Data + Y * Dest->WidthStep);
      if (Y == 0) {
        for (X = 0; X < Width; X++) {
          SumBlue = 0;
          SumGreen = 0;
          SumRed = 0;
          for (Z = -Top; Z < Bottom; Z++) {
            SamplePS = Src->Data + ((int*)Col->Data)[Z + Top] * Src->WidthStep + X * 3;
            SumBlue += SamplePS[0] * SamplePS[0];
            SumGreen += SamplePS[1] * SamplePS[1];
            SumRed += SamplePS[2] * SamplePS[2];
          }
          ColSumB[X] = SumBlue;
          ColSumG[X] = SumGreen;
          ColSumR[X] = SumRed;
        }
      }
      else {
        LastAddress = Src->Data + ((int*)Col->Data)[Y - 1] * Src->WidthStep;
        NextAddress = Src->Data + ((int*)Col->Data)[Y + SizeY - 1] * Src->WidthStep;
        for (X = 0; X < Width; X++) {
          ColSumB[X] -= LastAddress[0] * LastAddress[0] - NextAddress[0] * NextAddress[0];
          ColSumG[X] -= LastAddress[1] * LastAddress[1] - NextAddress[1] * NextAddress[1];
          ColSumR[X] -= LastAddress[2] * LastAddress[2] - NextAddress[2] * NextAddress[2];
          LastAddress += 3;
          NextAddress += 3;
        }
      }
      for (X = 0; X < Width; X++) {
        if (X == 0) {
          SumBlue = 0;
          SumGreen = 0;
          SumRed = 0;                  //方便四舍五入
          for (Z = -Left; Z < Right; Z++) {
            SumBlue += ColSumB[((int*)Row->Data)[Z + Left]];
            SumGreen += ColSumG[((int*)Row->Data)[Z + Left]];
            SumRed += ColSumR[((int*)Row->Data)[Z + Left]];
          }
        }
        else {
          LastIndex = ((int*)Row->Data)[X - 1];
          NextIndex = ((int*)Row->Data)[X + SizeX - 1];
          SumBlue -= ColSumB[LastIndex] - ColSumB[NextIndex];
          SumGreen -= ColSumG[LastIndex] - ColSumG[NextIndex];
          SumRed -= ColSumR[LastIndex] - ColSumR[NextIndex];
        }
        LinePD[0] = SumBlue;
        LinePD[1] = SumGreen;
        LinePD[2] = SumRed;
        LinePD += 3;
      }
    }
  }
Done:
  IS_FreeMatrix(&Row);
  IS_FreeMatrix(&Col);
  IS_FreeMemory(ColSum);
  return Ret;
}
/// <summary>
/// 基于SSE的字节数据的乘法。
/// </summary>
/// <param name="Kernel">需要卷积的核矩阵。</param>
/// <param name="Conv">卷积矩阵。</param>
/// <param name="Length">矩阵所有元素的长度。</param>
/// <remarks> 1: 使用了SSE优化。
/// <remarks> 2: 感谢采石工（544617183）提供的SSE代码<。</remarks>
///     https://msdn.microsoft.com/zh-cn/library/t5h7783k(v=vs.90).aspx
int MultiplySSE(unsigned char* Kernel, unsigned char* Conv, int Length)
{
  int Y = 0, Sum = 0;
#if 0
  __m128i vsum = _mm_set1_epi32(0);
  __m128i vk0 = _mm_set1_epi8(0);
  for (Y = 0; Y <= Length - 16; Y += 16) {
    __m128i v0 = _mm_loadu_si128((__m128i*)(Kernel + Y));                           //      对应movdqu指令,不需要16字节对齐
    __m128i v0l = _mm_unpacklo_epi8(v0, vk0);
    __m128i v0h = _mm_unpackhi_epi8(v0, vk0);                                                       //      此两句的作用是把他们分别加载到两个128位寄存器中，供下面的_mm_madd_epi16的16位SSE函数调用（vk0的作用主要是把高8位置0）
    __m128i v1 = _mm_loadu_si128((__m128i*)(Conv + Y));
    __m128i v1l = _mm_unpacklo_epi8(v1, vk0);
    __m128i v1h = _mm_unpackhi_epi8(v1, vk0);
    vsum = _mm_add_epi32(vsum, _mm_madd_epi16(v0l, v1l));                           //      _mm_madd_epi16 可以一次性进行8个16位数的乘法，然后把两个16的结果在加起来，放到一个32数中， r0 := (a0 * b0) + (a1 * b1)，详见 https://msdn.microsoft.com/zh-cn/library/yht36sa6(v=vs.90).aspx
    vsum = _mm_add_epi32(vsum, _mm_madd_epi16(v0h, v1h));
  }
  for (; Y <= Length - 8; Y += 8) {
    __m128i v0 = _mm_loadl_epi64((__m128i*)(Kernel + Y));
    __m128i v0l = _mm_unpacklo_epi8(v0, vk0);
    __m128i v1 = _mm_loadl_epi64((__m128i*)(Conv + Y));
    __m128i v1l = _mm_unpacklo_epi8(v1, vk0);
    vsum = _mm_add_epi32(vsum, _mm_madd_epi16(v0l, v1l));
  }
  vsum = _mm_add_epi32(vsum, _mm_srli_si128(vsum, 8));
  vsum = _mm_add_epi32(vsum, _mm_srli_si128(vsum, 4));
  Sum = _mm_cvtsi128_si32(vsum);                                                                                  //      MOVD函数，Moves the least significant 32 bits of a to a 32-bit integer： r := a0
#endif
  for (; Y < Length; Y++) {
    Sum += Kernel[Y] * Conv[Y];
  }
  return Sum;
}
/// <summary>
/// 基于SSE的图像卷积算法，整理时间2014.11.27
/// </summary>
/// <param name="Src">需要处理的源图像的数据结构。</param>
/// <param name="Conv">卷积矩阵,必须是图像数据。</param>
/// <param name="Dest">保存处理后的卷积结果的数据结构，必须是int型矩阵。</param>
/// <remarks> 1: 使用了SSE优化。
/// <remarks> 2: 使用字节数组的SSE乘法，比普通的SSE速度要快了更多。</remarks>
IS_RET FastConv2(TMatrix* Src, TMatrix* Conv, TMatrix* Dest)
{
  if (Src == NULL || Dest == NULL || Conv == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL || Conv->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width != Dest->Width || Src->Height != Dest->Height || Src->Channel != Dest->Channel) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Conv->Width < 1 || Conv->Height < 1) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_32S) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  IS_RET Ret = IS_RET_OK;
  int IndexD, IndexE, IndexK, ExpHeight, ExpStride, *DestP;
  int X, Y, Width, Height, ConvW, ConvH, Length, Left, Top, Right, Bottom;
  unsigned char* PtExp, *CurKer;;
  TMatrix* Expand = NULL;
  Width = Src->Width, Height = Src->Height, ConvW = Conv->Width, ConvH = Conv->Height, Length = ConvW * ConvH;
  Left = ConvW / 2, Top = ConvH / 2, Right = ConvW - Left - 1, Bottom = ConvH - Top - 1, ExpHeight = Height + ConvH - 1;  //      注意核中心那个元素不用扩展，比如核的宽度为3，则只要左右各扩展一个像素就可以了
  Ret = GetExpandImage(Src, &Expand, Left, Right, Top, Bottom, EdgeMode_Tile);                                                           //      得到扩展后的数据，可以提速和方便编程，但是多占用一份内存
  if (Ret != IS_RET_OK) {
    IS_FreeMatrix(&Expand);
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  unsigned char* Conv16 = (unsigned char*)IS_AllocMemory(ConvW * ConvH);                                         //      保存卷积矩阵，去除原始Conv中可能无用的数据，注意这里不用16字节对齐
  if (Conv16 == NULL) {
    Ret = IS_RET_ERR_OUTOFMEMORY;
    IS_FreeMatrix(&Expand);
    return IS_RET_ERR_OUTOFMEMORY;
  }
  unsigned char* Kernel = (unsigned char*)IS_AllocMemory(ConvW * ExpHeight);                                     //      保存16字节对齐的卷积核矩阵，以方便使用SSE
  if (Kernel == NULL) {
    Ret = IS_RET_ERR_OUTOFMEMORY;
    IS_FreeMatrix(&Expand);
    IS_FreeMemory(Conv16);
    return IS_RET_ERR_OUTOFMEMORY;
  }
  PtExp = Expand->Data;
  ExpStride = Expand->WidthStep;
  for (Y = 0; Y < ConvH; Y++) {
    memcpy(Conv16 + Y * ConvW, Conv->Data + Y * Conv->WidthStep , ConvW);  //      复制卷积矩阵的数据
  }
  for (Y = 0; Y < ExpHeight; Y++) {
    IndexE = Y * ExpStride;
    CurKer = Kernel + Y * ConvW;                                           //      计算第一列所有像素将要取样的卷积核数据
    for (X = 0; X < ConvW; X++) {
      CurKer[X] = PtExp[IndexE++];
    }
  }
  for (X = 0 ; X < Width ; X ++) {
    if (X != 0) {                                                                                                  //      如果不是第一列，需要更新卷积核的数据
      memcpy(Kernel, Kernel + 1, (ConvW * ExpHeight - 1));   //      往前移动一个数据
      IndexK = ConvW - 1 ;
      IndexE = IndexK + X;
      for (Y = 0; Y < ExpHeight; Y++) {
        Kernel[IndexK] = PtExp[IndexE];                                                //      只要刷新下一个元素
        IndexK += ConvW;
        IndexE += ExpStride;
      }
    }
    CurKer = Kernel, IndexD = X, DestP = (int*)Dest->Data;
    for (Y = 0; Y < Height; Y ++) {                                                        //      沿列的方向进行更新
      DestP[IndexD] = MultiplySSE(Conv16, CurKer, Length);
      CurKer += ConvW;
      IndexD += Width;
    }
  }
  IS_FreeMemory(Conv16);
  IS_FreeMemory(Kernel);
  IS_FreeMatrix(&Expand);
  return IS_RET_OK;
}
/// <summary>
/// 计算图像的累积平方差，速度已优化。（2015.9.10日）
/// </summary>
/// <param name="Src">源图像，我们的模板图像在该图像中寻找累积平方差。</param>
/// <param name="Template">模板图像，其宽度和高度都应该比源图像小。</param>
/// <param name="Dest">保存结果值，使用int类型矩阵，其大小必须为Src->Width - Template->Width, Src->Height - Template->Height。</param>
/// <remarks> 1:累积均方差即计算两幅图像对应位置像素的差的平方的累积和：(a-b)^2 = a^2 + b^2 - 2ab。</remarks>
/// <remarks> 2:a(模板)的平方是个定值，b（源图里的小图）的平方可以用快速积分图实现，a*b可以用快速卷积实现。</remarks>
IS_RET GetSSD(TMatrix* Src, TMatrix* Template, TMatrix* Dest)
{
  if (Src == NULL || Dest == NULL || Template == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL || Template->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Width <= Template->Width || Src->Height <= Template->Height || Src->Channel != Template->Channel || Src->Depth != Template->Depth) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Dest->Width != Src->Width - Template->Width || Dest->Height != Src->Height - Template->Height || Dest->Channel != 1) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_32S) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (Src->Channel == 1) {
    IS_RET Ret = IS_RET_OK;
    TMatrix* LocalSquareSum = NULL, *XY = NULL;
    Ret = IS_CreateMatrix(Src->Width, Src->Height, IS_DEPTH_32S, 1, &LocalSquareSum);       //      分配数据
    do {
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = IS_CreateMatrix(Src->Width, Src->Height, IS_DEPTH_32S, 1, &XY);   //      分配数据
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = GetLocalSquareSum(Src, LocalSquareSum, Template->Width, Template->Height);
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = FastConv2(Src, Template, XY);
      if (Ret != IS_RET_OK) {
        break;
      }
      int X, Y, Sum, *LinePP, *LinePXY, *LinePD;
      int Left = Template->Width / 2, Right = Template->Width - Left, Top = Template->Height / 2, Bottom = Template->Height - Top;
      int Width = Dest->Width, Height = Dest->Height;
      for (Y = 0, Sum = 0; Y < Template->Height * Template->WidthStep; Y++) {
        Sum += Template->Data[Y] * Template->Data[Y] ;
      }
      for (Y = 0; Y < Height; Y++) {
        LinePP = (int*)(LocalSquareSum->Data + (Y + Top) * LocalSquareSum->WidthStep) + Left;
        LinePXY = (int*)(XY->Data + (Y + Top) * XY->WidthStep) + Left;
        LinePD = (int*)(Dest->Data + Y * Dest->WidthStep);
        for (X = 0; X < Width; X++) {
          LinePD[X] = Sum + LinePP[X] - 2 * LinePXY[X];                                   //      a^2 + b^2 - 2ab
        }
      }
    }
    while (0);
    IS_FreeMatrix(&LocalSquareSum);
    IS_FreeMatrix(&XY);
    return Ret;
  }
  else {
    TMatrix* GraySrc = NULL, *GrayTemplate = NULL;
    IS_RET Ret = IS_CreateMatrix(Src->Width, Src->Height, Src->Depth, 1, &GraySrc);
    do {
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = IS_CreateMatrix(Template->Width, Template->Height, Template->Depth, 1, &GrayTemplate);
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = ClassicDecolorization(Src, GraySrc);
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = ClassicDecolorization(Template, GrayTemplate);
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = GetSSD(GraySrc, GrayTemplate, Dest);
      if (Ret != IS_RET_OK) {
        break;
      }
    }
    while (0);
    IS_FreeMatrix(&GraySrc);
    IS_FreeMatrix(&GrayTemplate);
    return Ret;
  }
}

/// <summary>
/// 纹理图像的合成算法。（2015.9.10日）
/// </summary>
/// <param name="Src">源图像，我们的模板图像在该图像中寻找累积平方差。</param>
/// <param name="Dest">合成后的图像。</param>
/// <param name="TileSizeX">块的宽度。</param>
/// <param name="TileSizeY">块的高度。</param>
/// <param name="OverlapX">重叠部分的宽度。</param>
/// <param name="OverlapY">重叠部分的高度。</param>
/// <remarks> 1:速度还是很慢，关键在于找另外一种相似性度量的指标。</remarks>
IS_RET __stdcall ImageQuilting(TMatrix* Src, TMatrix* Dest, int TileSizeX, int TileSizeY, int OverlapX, int OverlapY)
{
  if (Src == NULL || Dest == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Data == NULL || Dest->Data == NULL) {
    return IS_RET_ERR_NULLREFERENCE;
  }
  if (Src->Channel != Dest->Channel || Src->Depth != Dest->Depth) {
    return IS_RET_ERR_PARAMISMATCH;
  }
  if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_8U) {
    return IS_RET_ERR_NOTSUPPORTED;
  }
  if (TileSizeX < 0 || TileSizeY < 0 || OverlapX < 0 || OverlapY < 0) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  if (TileSizeX >= Src->Width || TileSizeY >= Src->Height || OverlapX >= TileSizeX || OverlapY >= TileSizeY) {
    return IS_RET_ERR_ARGUMENTOUTOFRANGE;
  }
  int TileX = (int)ceil((double)(Dest->Width - OverlapX) / (TileSizeX - OverlapX));
  int TileY = (int)ceil((double)(Dest->Height - OverlapY) / (TileSizeY - OverlapY));                      //      每一块的大小
  int NewWidth = TileX * TileSizeX - (TileX - 1) * OverlapX;
  int NewHeight = TileY * TileSizeY - (TileY - 1) * OverlapY;                     //      为减轻编码时的麻烦，一种方式就是把图像的宽和高都扩展为可整分为TileX及TileY块大小
  if ((NewWidth != Dest->Width) || (NewHeight != Dest->Height)) {         //      如果图像不能被整划分，就建立一副能够被整划分的图像，这样编程处理起来方便多了
    TMatrix* Expand = NULL;
    IS_RET Ret = IS_CreateMatrix(NewWidth, NewHeight, Dest->Depth, Dest->Channel, &Expand);                //      建立一个空白的图像
    do {
      if (Ret != IS_RET_OK) {
        break;
      }
      Ret = ImageQuilting(Src, Expand, TileSizeX, TileSizeY, OverlapX, OverlapY);
      if (Ret != IS_RET_OK) {
        break;
      }
      for (int Y = 0; Y < Dest->Height; Y++) {               //      从最左侧开始拷贝
        memcpy(Dest->Data + Y * Dest->WidthStep, Expand->Data + Y * Expand->WidthStep, Dest->WidthStep);
      }
    }
    while (0);
    IS_FreeMatrix(&Expand);
    return Ret;
  }
  int X, Y, XX, YY, Width, Height, DistW, DistH;
  int StartX, StartY, Sum, Best, Left, Top, Count, Index, Alpha;
  unsigned char* LinePS, *LinePD, *LinePP, *LinePM;
  int* LinePH, *LinePV, *LinePHV, *LinePT;
  Width = Src->Width, Height = Src->Height;
  DistW = Width - TileSizeX, DistH = Height - TileSizeY;
  TMatrix* Mask = NULL, *MaskX = NULL, *MaskY = NULL;                     //      最小能量路径分割对应的蒙版图
  TMatrix* TileH = NULL, *TileV = NULL, *TileHV = NULL, *SmallTileH = NULL;
  TMatrix* Distance = NULL, *ImgOverlapX = NULL, *ImgOverlapY = NULL, *DistanceH = NULL, *DistanceV = NULL, *DistanceHV = NULL ;
  IS_RET Ret = IS_CreateMatrix(DistW, DistH, IS_DEPTH_32S, 1, &Distance);                                                         //      分配数据
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(TileSizeX, TileSizeY, IS_DEPTH_8U, 1, &Mask);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(OverlapX, TileSizeY, IS_DEPTH_32S, 1, &ImgOverlapX);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(TileSizeX, OverlapY, IS_DEPTH_32S, 1, &ImgOverlapY);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(OverlapX, TileSizeY, IS_DEPTH_8U, 1, &MaskX);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(TileSizeX, OverlapY, IS_DEPTH_8U, 1, &MaskY);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(TileSizeX, OverlapY, IS_DEPTH_8U, Src->Channel, &TileH);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(OverlapX, TileSizeY, IS_DEPTH_8U, Src->Channel, &TileV);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(OverlapX, OverlapY, IS_DEPTH_8U, Src->Channel, &TileHV);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(Width - TileSizeX, Height - OverlapY , IS_DEPTH_32S, 1, &DistanceH);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(Width - OverlapX, Height - TileSizeY, IS_DEPTH_32S, 1, &DistanceV);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  Ret = IS_CreateMatrix(Width - OverlapX, Height - OverlapY, IS_DEPTH_32S, 1, &DistanceHV);
  if (Ret != IS_RET_OK) {
    goto Done;
  }
  memset(Dest->Data, 0, Dest->Height * Dest->WidthStep);
  for (YY = 0; YY < TileY; YY++) {
    StartY = YY * TileSizeY - YY * OverlapY;                                //      新的块的起点位置
    for (XX = 0; XX < TileX; XX++) {
      StartX = XX * TileSizeX - XX * OverlapX;
      if (XX == 0 && YY == 0) {                                               //      对于第一个块，随机的从源图片中拷贝一个块的数据
        Left = Rand() * (Width - TileSizeX);                    //      起点的位置按照靠左的原则，不要注意越界
        Top = Rand() * (Height - TileSizeY);
        for (Y = 0; Y < TileSizeY; Y++) {
          memcpy(Dest->Data + Y * Dest->WidthStep, Src->Data + (Y + Top) * Src->WidthStep + Left * Src->Channel, TileSizeX * Src->Channel) ;
        }
      }
      else {
        ///     *****************************以下部分代码是用来计算最相似的块***************************************
        ///     1、 注意这里的块在很多情况下并不是方方正正的块，通常是L形的；
        ///     2、 如果用暴力实现，注意必须判断目标点是否已经有数据，如果还没有数据的就不能列入计算距离的行列中。
        memset(Distance->Data, 0, Distance->Height * Distance->WidthStep);                      //      把距离数据清零
        if (YY > 0) {                                                                           //      在垂直方向有重合了
          for (Y = 0; Y < OverlapY; Y++) {                        //      拷贝垂直方向重合的数据到一个单独的TileH数据结构中，主要是为了方便，大小TileSizeX * OverlapY
            memcpy(TileH->Data + Y * TileH->WidthStep, Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel, TileH->Width * TileH->Channel);
          }
          Ret = GetSSD(Src, TileH, DistanceH);                            //      计算这部分到原图各个块的距离
          if (Ret != IS_RET_OK) {
            goto Done;
          }
          for (Y = 0; Y < Distance->Height; Y++) {        //      把计算的结果的直接拷贝到距离矩阵中
            memcpy(Distance->Data + Y * Distance->WidthStep, DistanceH->Data + Y * DistanceH->WidthStep, Distance->WidthStep);
          }
        }
        if (XX > 0) {                                                                           //      在水平方向有重合了
          for (Y = 0; Y < TileSizeY; Y++) {                       //      拷贝水平方向重合的数据到一个单独的TileV数据结构中，主要是为了方便，大小OverlapX * TileSizeY
            memcpy(TileV->Data + Y * TileV->WidthStep, Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel, TileV->Width * TileV->Channel);
          }
          Ret = GetSSD(Src, TileV, DistanceV);
          if (Ret != IS_RET_OK) {
            goto Done;
          }
          if (YY > 0) {                                                                   //      此时肯定有重叠的部分了
            for (Y = 0; Y < OverlapY; Y++) {                //      拷贝重叠区域数据
              memcpy(TileHV->Data + Y * TileHV->WidthStep, Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel, TileHV->Width * TileHV->Channel);
            }
            Ret = GetSSD(Src, TileHV, DistanceHV);
            if (Ret != IS_RET_OK) {
              goto Done;
            }
            for (Y = 0; Y < DistH; Y++) {
              LinePT = (int*)(Distance->Data + Y * Distance->WidthStep);
              LinePV = (int*)(DistanceV->Data + Y * DistanceV->WidthStep);
              LinePHV = (int*)(DistanceHV->Data + Y * DistanceHV->WidthStep);
              for (X = 0; X < DistW; X++) {
                LinePT[X] += LinePV[X] - LinePHV[X];  //       需要减掉重叠的部分
              }
            }
          }
          else {
            for (Y = 0; Y < Distance->Height; Y++) {                                                                                                //      直接拷贝
              memcpy(Distance->Data + Y * Distance->WidthStep, DistanceV->Data + Y * DistanceV->WidthStep, Distance->WidthStep);
            }
          }
        }
        //      寻找最相似的块的起点（左上角）的坐标
        for (Y = 0, Best = MaxValueI; Y < DistH; Y++) {
          LinePT = (int*)(Distance->Data + Y * Distance->WidthStep);
          for (X = 0; X < DistW; X++) {
            if (LinePT[X] < Best) {
              Best = LinePT[X];                               //      有些算法建议增加个容差，从容差小于最佳匹配块的块中随机选取一个作为选中的Patch,这也未尝不可不过我这里还是直接选择了最佳块
              Left = X;
              Top = Y;
            }
          }
        }
        //Index = 0;
        /*for(Y = 0; Y < TileSizeY; Y++)
        {
                memcpy(Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel, Src->Data + (Y + Top) * Src->WidthStep + Left * Src->Channel, TileSizeX * Src->Channel) ;
        }*/
        memset(Mask->Data, 255, Mask->Height * Mask->WidthStep);
        if (YY > 0) {
          for (Y = 0; Y < OverlapY; Y++) {
            LinePT = (int*)(ImgOverlapY->Data + Y * ImgOverlapY->WidthStep);
            LinePS = Src->Data + (Y + Top) * Src->WidthStep + Left * Src->Channel;
            LinePD = Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel;
            for (X = 0; X < TileSizeX; X++) {
              LinePT[X] = (LinePD[0] - LinePS[0]) * (LinePD[0] - LinePS[0]) + (LinePD[1] - LinePS[1]) * (LinePD[1] - LinePS[1]) + (LinePD[2] - LinePS[2]) * (LinePD[2] - LinePS[2]);
              LinePD += Dest->Channel;
              LinePS += Src->Channel;
            }
          }
          Ret = MinCutHorizontal(ImgOverlapY, MaskY);
          if (Ret != IS_RET_OK) {
            goto Done;
          }
          for (Y = 0; Y < OverlapY; Y++) {
            memcpy(Mask->Data + Y * Mask->WidthStep, MaskY->Data + Y * MaskY->WidthStep, TileSizeX);
          }
        }
        if (XX > 0) {
          for (Y = 0; Y < TileSizeY; Y++) {
            LinePT = (int*)(ImgOverlapX->Data + Y * ImgOverlapX->WidthStep);
            LinePS = Src->Data + (Y + Top) * Src->WidthStep + Left * Src->Channel;
            LinePD = Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel;
            for (X = 0; X < OverlapX; X++) {
              LinePT[X] = (LinePD[0] - LinePS[0]) * (LinePD[0] - LinePS[0]) + (LinePD[1] - LinePS[1]) * (LinePD[1] - LinePS[1]) + (LinePD[2] - LinePS[2]) * (LinePD[2] - LinePS[2]);
              LinePD += Dest->Channel;
              LinePS += Src->Channel;
            }
          }
          Ret = MinCutVertical(ImgOverlapX, MaskX);
          if (Ret != IS_RET_OK) {
            goto Done;
          }
          for (Y = 0; Y < TileSizeY; Y++) {
            LinePS = MaskX->Data + Y * MaskX->WidthStep;
            LinePM = Mask->Data + Y * Mask->WidthStep;
            for (X = 0; X < OverlapX; X++) {
              LinePM[X] &= LinePS[X] ;
            }
          }
        }
        for (Y = 0; Y < TileSizeY; Y++) {
          LinePS = Src->Data + (Y + Top) * Src->WidthStep + Left * Src->Channel;
          LinePD = Dest->Data + (Y + StartY) * Dest->WidthStep + StartX * Dest->Channel;
          LinePM = Mask->Data + Y * Mask->WidthStep;
          for (X = 0; X < TileSizeX; X++) {
            Alpha = LinePM[X];
            if (Alpha == 255) {                                     //      大部分的对象都是255的
              LinePD[0] = LinePS[0];
              LinePD[1] = LinePS[1];
              LinePD[2] = LinePS[2];
            }
            LinePD += Dest->Channel;
            LinePS += Src->Channel;
          }
        }
      }
    }
  }
Done:
  IS_FreeMatrix(&Distance);
  IS_FreeMatrix(&Mask);
  IS_FreeMatrix(&ImgOverlapX);
  IS_FreeMatrix(&ImgOverlapY);
  IS_FreeMatrix(&MaskX);
  IS_FreeMatrix(&MaskY);
  IS_FreeMatrix(&TileH);
  IS_FreeMatrix(&TileV);
  IS_FreeMatrix(&TileHV);
  IS_FreeMatrix(&DistanceH);
  IS_FreeMatrix(&DistanceV);
  IS_FreeMatrix(&DistanceHV);
  return Ret;
}

