#include "str/FloatToStr.inl"
// 下面是运行结果：
//
// -10000
// 1.23456789012346e+017
// 1234567890.12346
// 0.000123456789
// -1.23456789e-005
// -1.23e+045
// 0
// INF
int test_FloatToStr()
{
  CHAR s[32];
  UINT64 inf = 0x7ff0000000000000;
  double v = -1.230E45;
  //INT a, b;
  puts(FloatToStr(s, -10000));
  puts(FloatToStr(s, (double)123456789012345678));
  puts(FloatToStr(s, 1234567890.12345678));
  puts(FloatToStr(s, 0.0001234567890));
  puts(FloatToStr(s, -0.00001234567890));
  puts(FloatToStr(s, v));
  puts(FloatToStr(s, 0));
  puts(FloatToStr(s, *(double*)&inf));
  system("pause");
  return 0;
}

