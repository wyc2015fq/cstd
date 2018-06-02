
// 去色的原理 取彩色图像RGB通道最大值和最小值的平均值作为新的三通道的颜色值。
int Desaturate(int Height, int Width, const unsigned char* Scan0, int Stride, unsigned char* b, int bl, int cn)
{
  int X, Y;
  byte Red, Green, Blue, Max, Min, Value;
  const byte* CurP;
  byte* CurPb;

  for (Y = 0; Y < Height; Y++) {
    CurP = Scan0 + Y * Stride;
    CurPb = b + Y * bl;

    for (X = 0; X < Width; X++) {
      Blue = *CurP;
      Green = *(CurP + 1);
      Red = *(CurP + 2);

      if (Blue > Green) {
        Max = Blue;
        Min = Green;
      }
      else {
        Max = Green;
        Min = Blue;
      }

      if (Red > Max) {
        Max = Red;
      }
      else if (Red < Min) {
        Min = Red;
      }

      Value = (byte)((Max + Min) >> 1);
      *(CurPb + 0) = Value;
      *(CurPb + 1) = Value;
      *(CurPb + 2) = Value;
      CurP += 3;
      CurPb += 3;
    }
  }

  return 0;
}

int GetLevelTable(unsigned char * Table, unsigned char InputLeftLimit, unsigned char InputMiddle, unsigned char InputRightLimit, unsigned char OutputLeftLimit , unsigned char OutputRightLimit)
{
  int Index;
  if (InputLeftLimit > 253) InputLeftLimit = 253;
  if (InputLeftLimit < 0)    InputLeftLimit = 0;
  if (InputRightLimit > 255)InputRightLimit = 255;
  if (InputRightLimit < 2) InputRightLimit = 2;
  if (InputMiddle > 254)InputMiddle = 254;
  if (InputMiddle < 1)InputMiddle = 1;
  if (InputMiddle > InputRightLimit)InputMiddle = InputRightLimit - 1;
  if (InputMiddle < InputLeftLimit)InputMiddle = InputLeftLimit + 1;
  if (OutputLeftLimit < 0)OutputLeftLimit = 0;
  if (OutputLeftLimit > 255)OutputLeftLimit = 255;
  if (OutputRightLimit < 0)OutputRightLimit = 0;
  if (OutputRightLimit > 255)OutputRightLimit = 255;
  
  for (Index = 0; Index <= 255; Index++)
  {
    double Temp = Index - InputLeftLimit;
    if (Temp < 0) 
    {
      Temp = OutputLeftLimit;
    }
    else if (Temp + InputLeftLimit > InputRightLimit)
    {
      Temp = OutputRightLimit;
    }
    else 
    {
      double Gamma = log(0.5) / log((double)(InputMiddle - InputLeftLimit) / (InputRightLimit - InputLeftLimit));
      Temp = OutputLeftLimit + (OutputRightLimit - OutputLeftLimit) * pow((Temp / (InputRightLimit - InputLeftLimit)), Gamma);
    }
    if (Temp > 255)
      Temp = 255;
    else if (Temp < 0)
      Temp = 0;
    Table[Index] = (unsigned char)Temp;
  }
  return 0;
}

int Curve(unsigned char * Src , int Width, int Height , int Stride ,unsigned char * TableB,unsigned char * TableG,unsigned char * TableR)
{
  int X,Y;
  int ByteCount = Stride / Width;
  unsigned char * Pointer;
  for (Y = 0; Y < Height; Y++)
  {
    Pointer = Src + Y * Stride;
    for (X = 0; X < Width; X++)
    {
      *Pointer++ = TableB[*Pointer];
      *Pointer++ = TableG[*Pointer];
      *Pointer++ = TableR[*Pointer];
    }
  }
  return 0;
}

int Level(int Height, int Width, unsigned char * Src, int Stride, int DestChannel,
          unsigned char InputLeftLimit, unsigned char InputMiddle,
          unsigned char InputRightLimit, unsigned char OutputLeftLimit , unsigned char OutputRightLimit)
{
  unsigned char * Table = (unsigned char *) malloc ( 256 * sizeof (unsigned char));
  unsigned char * LinearTable = (unsigned char *) malloc ( 256 * sizeof (unsigned char));
  int X;
  for (X=0;X<256;X++) {
    LinearTable[X] = X;
  }
  GetLevelTable(Table, InputLeftLimit,InputMiddle,InputRightLimit,OutputLeftLimit,OutputRightLimit);
  if (DestChannel == 0) // RGB
    Curve(Src,Width,Height,Stride,Table,Table,Table);
  else if (DestChannel == 1) // Blue
    Curve(Src,Width,Height,Stride,Table,LinearTable,LinearTable);
  else if (DestChannel == 2) // Green
    Curve(Src,Width,Height,Stride,LinearTable,Table,LinearTable);
  else if (DestChannel == 3) // Red
    Curve(Src,Width,Height,Stride,LinearTable,LinearTable,Table);
  free(Table);
  free(LinearTable);
  return 0;
}
