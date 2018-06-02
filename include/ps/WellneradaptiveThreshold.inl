
int WellneradaptiveThreshold1(int Width, int Height, byte* bmp, int Stride, int Radius/* = 5*/, int Threshold/* = 15*/)
{
  int X, Y;
  int Sum, InvertThreshold, XX;
  byte* Pointer;
  byte* Row;

  if (bmp == NULL) {
    return ArgumentNullException();
  }

  //Binaryzation functions can only be applied to 8bpp graymode Image.");
  if (Radius < 0 || Radius > 255) {
    return ArgumentOutOfRangeException();
  }

  if (Threshold < 0 || Threshold > 100) {
    return ArgumentOutOfRangeException();
  }

  Pointer = bmp;
  InvertThreshold = 100 - Threshold;
  Row = MALLOC(byte, Width);

  for (Y = 0; Y < Height; Y++) {
    Pointer = bmp + Stride * Y;
    Sum = *Pointer * Radius;
    CopyMemory(Row, Pointer, Width);

    for (X = 0; X < Width; X++) {
      XX = X - Radius;

      if (XX < 0) {
        XX = 0;
      }

      Sum += Row[X] - Row[XX];

      if (Row[X] * 100 * Radius < Sum * InvertThreshold) {
        Pointer[X] = 0;
      }
      else {
        Pointer[X] = 255;
      }
    }
  }

  free(Row);
  return 0;
}

// ¶þÎ¬µÄWellnerËã·¨
int WellneradaptiveThreshold2(int Height, int Width, byte* bmp, int Stride, int Radius/* = 5*/, int Threshold/* = 50*/)
{
  int X, Y;
  int Sum, X1, X2, Y1, Y2, Y2Y1, InvertThreshold;
  byte* Pointer;
  int* Integral;
  int* IndexOne, *IndexTwo;

  if (bmp == NULL) {
    return ArgumentNullException();
  }

  if (Radius < 0 || Radius > 255) {
    return ArgumentOutOfRangeException();
  }

  if (Threshold < 0 || Threshold > 100) {
    return ArgumentOutOfRangeException();
  }

  Pointer = bmp;
  InvertThreshold = 100 - Threshold;
  Integral = MALLOC(int, Width * Height);

  for (Y = 0; Y < Height; Y++) {
    Sum = 0;
    Pointer = bmp + Stride * Y;
    IndexOne = Integral + Width * Y;

    for (X = 0; X < Width; X++) {
      Sum += *Pointer;

      if (Y == 0) {
        *IndexOne = Sum;
      }
      else {
        *IndexOne = *(IndexOne - Width) + Sum;
      }

      IndexOne++;
      Pointer++;
    }
  }

  for (Y = 0; Y < Height; Y++) {
    Pointer = bmp + Stride * Y;
    Y1 = Y - Radius;
    Y2 = Y + Radius;

    if (Y1 < 0) {
      Y1 = 0;
    }

    if (Y2 >= Height) {
      Y2 = Height - 1;
    }

    IndexOne = Integral + Y1 * Width;
    IndexTwo = Integral + Y2 * Width;
    Y2Y1 = (Y2 - Y1) * 100;

    for (X = 0; X < Width; X++) {
      X1 = X - Radius;
      X2 = X + Radius;

      if (X1 < 0) {
        X1 = 0;
      }

      if (X2 >= Width) {
        X2 = Width - 1;
      }

      Sum = *(IndexTwo + X2) - *(IndexOne + X2) - *(IndexTwo + X1) + *(IndexOne + X1);

      if (*Pointer * (X2 - X1) * Y2Y1 < Sum * InvertThreshold) {
        *Pointer = 0;
      }
      else {
        *Pointer = 255;
      }

      Pointer++;
    }
  }

  free(Integral);
  return 0;
}

