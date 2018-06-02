
// 二值图像中封闭孔洞的高效填充算法

int FloodFill0(int w, int h, byte* ptr, int Stride, POINT location, byte FillValue)
{
  int PickColor;
  POINT* fillPOINTs = MALLOC(POINT, w * h);
  int fillPOINTs_Count = 0;

  int bytes = Stride * h;
  byte* temp = ptr;
  int* mask = MALLOC(int, w * h);

  PickColor = ptr[Stride * location.y + location.x];

  if (location.x < 0 || location.x >= w || location.y < 0 || location.y >= h) {
    return 0;
  }

  fillPOINTs[fillPOINTs_Count++] = (iPOINT(location.x, location.y));

  while (fillPOINTs_Count > 0) {
    POINT p = fillPOINTs[--fillPOINTs_Count];
    mask[p.x + p.y * w] = 1;
    temp[p.x + p.y * Stride] = FillValue;

    if (p.x > 0 && temp[(p.x - 1) + p.y * Stride] == PickColor) {
      temp[(p.x - 1) + p.y * Stride] = FillValue;
      fillPOINTs[fillPOINTs_Count++] = (iPOINT(p.x - 1, p.y));
      mask[p.x - 1, p.y] = 1;
    }

    if (p.x < w - 1 && temp[(p.x + 1) + p.y * Stride] == PickColor) {
      temp[(p.x + 1) + p.y * Stride] = FillValue;
      fillPOINTs[fillPOINTs_Count++] = (iPOINT(p.x + 1, p.y));
      mask[p.x + 1, p.y] = 1;
    }

    if (p.y > 0 && temp[p.x + (p.y - 1) * Stride] == PickColor) {
      temp[ p.x + (p.y - 1) * Stride] = FillValue;
      fillPOINTs[fillPOINTs_Count++] = (iPOINT(p.x, p.y - 1));
      mask[p.x, p.y - 1] = 1;
    }

    if (p.y < h - 1 && temp[p.x + (p.y + 1) * Stride] == PickColor) {
      temp[ p.x + (p.y + 1) * Stride] = FillValue;
      fillPOINTs[fillPOINTs_Count++] = (iPOINT(p.x, p.y + 1));
      mask[p.x, p.y + 1] = 1;
    }
  }

  free(mask);
  free(fillPOINTs);
  return 0;
}

int FillHole(int Height, int Width, byte* Src, int Stride, bool FillBackGround)
{
  int X, Y;
  byte* POINTer;
  byte Color;

  if (FillBackGround == false) {
    Color = 255;
  }
  else {
    Color = 0;
  }


  for (Y = 0; Y < Height; Y++) {
    POINTer = Src + Y * Stride;

    if (POINTer[0] == Color) {
      FloodFill0(Width, Height, Src, Stride, iPOINT(0, Y), 127);
    }

    if (POINTer[Width - 1] == Color) {
      FloodFill0(Width, Height, Src, Stride, iPOINT(Width - 1, Y), 127);
    }
  }

  for (X = 0; X < Width; X++) {
    POINTer = Src + X;

    if (POINTer[0] == Color) {
      FloodFill0(Width, Height, Src, Stride, iPOINT(X, 0), 127);
    }

    if (POINTer[(Height - 1) * Stride] == Color) {
      FloodFill0(Width, Height, Src, Stride, iPOINT(X, Height - 1), 127);
    }
  }

  for (Y = 0; Y < Height; Y++) {
    POINTer = Src + Y * Stride;

    for (X = 0; X < Width; X++) {
      if (POINTer[X] == 127) {
        POINTer[X] = Color;
      }
      else {
        POINTer[X] = (byte)(255 - Color);
      }
    }
  }

  return 0;
}
