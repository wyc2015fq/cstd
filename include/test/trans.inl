//轮换
void Cycle(int Data[], int Lenth, int Start)
{
  int Cur_index, Temp1, Temp2;
  Cur_index = (Start * 2) % (Lenth + 1);
  Temp1 = Data[Cur_index - 1];
  Data[Cur_index - 1] = Data[Start - 1];
  while (Cur_index != Start) {
    Temp2 = Data[(Cur_index * 2) % (Lenth + 1) - 1];
    Data[(Cur_index * 2) % (Lenth + 1) - 1] = Temp1;
    Temp1 = Temp2;
    Cur_index = (Cur_index * 2) % (Lenth + 1);
  }
}
//满足Lenth=3^k-1的perfect shfulle的实现
void Perfect1(int Data[], int Lenth)
{
  int i = 1;
  if (Lenth == 2) {
    i = Data[Lenth - 1];
    Data[Lenth - 1] = Data[Lenth - 2];
    Data[Lenth - 2] = i;
    return;
  }
  while (i < Lenth) {
    Cycle(Data, Lenth, i);
    i = i * 3;
  }
}
//数组循环移位 参考编程珠玑
void Reverse(int Data[], int Len)
{
  int i, Temp;
  for (i = 0; i < Len / 2; i++) {
    Temp = Data[i];
    Data[i] = Data[Len - i - 1];
    Data[Len - i - 1] = Temp;
  }
}
void ShiftN(int Data[], int Len, int N)
{
  Reverse(Data, Len - N);
  Reverse(&Data[Len - N], N);
  Reverse(Data, Len);
}
//查找最接近N的3^k
int LookUp(int N)
{
  int i = 3;
  while (i <= N + 1) {
    i *= 3;
  }
  if (i > 3) {
    i = i / 3;
  }
  return i;
}
void perfect(int Data[], int Lenth)
{
  int i, startPos = 0;
  while (startPos < Lenth) {
    i = LookUp(Lenth - startPos);
    ShiftN(&Data[startPos + (i - 1) / 2], (Lenth - startPos) / 2, (i - 1) / 2);
    Perfect1(&Data[startPos], i - 1);
    startPos += (i - 1);
  }
}
int tradsf(int h, int w, int* src, int srcstep, int* dst, int dststep)
{
  int i, j, s0, d0;
  for (j = 0; j < h; ++j) {
    s0 = (j * srcstep);
    d0 = (j);
    for (i = 0; i < w; ++i) {
      dst[d0 + i * dststep] = src[s0 + i];
    }
  }
  return 0;
}
int test_TransposeInplace()
{
  int i, j = 0;
  int h = 2, w = 3;
#if 0
  int* aa = NULL;
  int* bb = NULL;
  MYREALLOC(aa, 3 * h * w);
  bb = aa + h * w;
#else
  int aa[6], bb[6];
#endif
  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      aa[i * w + j] = bb[i * w + j] = i + 1;
    }
  }
  TRANS_COPY(h, w, aa, w, bb, h, 1, 0, 0);
  tradsf(h, w, aa, w, bb, h);
  perfect(aa, h * w);
  for (i = 0; i < h * w; ++i) {
    if (aa[i] != bb[i]) {
      int asdf = 0;
    }
  }
  return 0;
}

