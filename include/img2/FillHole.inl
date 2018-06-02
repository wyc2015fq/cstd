
#include "Threshold.inl"
int* GetHistGram(const img_t* Src, int* HistGram)
{
  int x, y;
  for (y = 0; y < 256; y++) {
    HistGram[y] = 0;
  }
  for (y = 0; y < Src->h; y++) {
    const uchar* SrcP = Src->tt.data + y * Src->s;
    for (x = 0; x < Src->w; x++, SrcP++) {
      HistGram[*SrcP]++;
    }
  }
  return HistGram;
}
void DoBinaryzation1(const img_t* Src, img_t* Dest, int Threshold)
{
  int x, y;
  imsetsize(Dest, Src->h, Src->w, 1, 1);
  for (y = 0; y < Src->h; y++) {
    const uchar* SrcP = Src->tt.data + y * Src->s; // 必须在某个地方开启unsafe功能，其实C#中的unsafe很safe，搞的好吓人。
    uchar* DestP = Dest->tt.data + y * Dest->s;
    for (x = 0; x < Src->w; x++, SrcP++, DestP++) {
      *DestP = *SrcP > Threshold ? byteMaxValue : byteMinValue; // 写成255和0，C#编译器不认。
    }
  }
  return ;
}
void DoBinaryzation(const img_t* Src, img_t* Dest)
{
  int HistGram[256];
  int x, y, Threshold;
  GetHistGram(Src, HistGram);
  Threshold = GetHuangFuzzyThreshold(HistGram, countof(HistGram));
  DoBinaryzation1(Src, Dest, Threshold);
  return ;
}
void FloodFill(img_t* Src, uchar* ptr, int Stride, IPOINT location, byte FillValue)
{
  int w = Src->w;
  int h = Src->h;
  int PickColor;
  IPOINT* fillPoints = NULL;
  int fillPoints_top = 0;
  MYREALLOC(fillPoints, w * h);
  int bytes = Stride * Src->h;
  byte* grayValues = NULL;
  byte* temp = NULL;
  MYREALLOC(grayValues, bytes);
  MYREALLOC(temp, bytes);
  memcpy(grayValues, ptr, bytes);
  memcpy(temp, grayValues, bytes);
  PickColor = ptr[Stride * location.y + location.x];
  if (!(location.x < 0 || location.x >= w || location.y < 0 || location.y >= h)) {
    int* mask = NULL;
    fillPoints[fillPoints_top++] = iPOINT(location.x, location.y);
    MYREALLOC(mask, w * h);
    while (fillPoints_top > 0) {
      IPOINT p = fillPoints[--fillPoints_top];
      mask[p.x, p.y] = 1;
      temp[p.x + p.y * Stride] = FillValue;
      if (p.x > 0 && temp[(p.x - 1) + p.y * Stride] == PickColor) {
        temp[(p.x - 1) + p.y * Stride] = FillValue;
        fillPoints[fillPoints_top++] = (iPOINT(p.x - 1, p.y));
        mask[p.x - 1, p.y] = 1;
      }
      if (p.x < w - 1 && temp[(p.x + 1) + p.y * Stride] == PickColor) {
        temp[(p.x + 1) + p.y * Stride] = FillValue;
        fillPoints[fillPoints_top++] = (iPOINT(p.x + 1, p.y));
        mask[p.x + 1, p.y] = 1;
      }
      if (p.y > 0 && temp[p.x + (p.y - 1) * Stride] == PickColor) {
        temp[ p.x + (p.y - 1) * Stride] = FillValue;
        fillPoints[fillPoints_top++] = (iPOINT(p.x, p.y - 1));
        mask[p.x, p.y - 1] = 1;
      }
      if (p.y < h - 1 && temp[p.x + (p.y + 1) * Stride] == PickColor) {
        temp[ p.x + (p.y + 1) * Stride] = FillValue;
        fillPoints[fillPoints_top++] = (iPOINT(p.x, p.y + 1));
        mask[p.x, p.y + 1] = 1;
      }
    }
    fillPoints_top = 0;
    memcpy(grayValues, temp, bytes);
    memcpy(ptr, grayValues, bytes);
    FREE(mask);
  }
  FREE(grayValues);
  FREE(temp);
  FREE(fillPoints);
  return ;
}
void FillHole(img_t* Src, bool FillBackGround /* = false */)
{
  int x, y;
  int Width, Height, Stride;
  byte* Pointer, *Scan0;
  Width = Src->w;
  Height = Src->h;
  Scan0 = Src->tt.data;
  Stride = Src->s;
  byte Color;
  if (FillBackGround == false) {
    Color = 255;
  }
  else {
    Color = 0;
  }
  for (y = 0; y < Height; y++) {
    Pointer = Scan0 + y * Stride;
    if (Pointer[0] == Color) {
      FloodFill(Src, Scan0, Stride, iPOINT(0, y), 127);
    }
    if (Pointer[Width - 1] == Color) {
      FloodFill(Src, Scan0, Stride, iPOINT(Width - 1, y), 127);
    }
  }
  for (x = 0; x < Width; x++) {
    Pointer = Scan0 + x;
    if (Pointer[0] == Color) {
      FloodFill(Src, Scan0, Stride, iPOINT(x, 0), 127);
    }
    if (Pointer[(Height - 1) * Stride] == Color) {
      FloodFill(Src, Scan0, Stride, iPOINT(x, Height - 1), 127);
    }
  }
  for (y = 0; y < Height; y++) {
    Pointer = Scan0 + y * Stride;
    for (x = 0; x < Width; x++) {
      if (Pointer[x] == 127) {
        Pointer[x] = Color;
      }
      else {
        Pointer[x] = (byte)(255 - Color);
      }
    }
  }
  return ;
}
void Invert(img_t* Src)
{
  int y;
  for (y = 0; y < Src->h * Src->s; y++) {
    Src->tt.data[y] = (byte)(255 - Src->tt.data[y]);
  }
}

