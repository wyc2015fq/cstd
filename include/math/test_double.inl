//#include "test_panorama.inl"
// "异常", // FPE_DENORMAL
// "除数为零", // FPE_ZERODIVIDE
// "上溢出", // FPE_OVERFLOW
// "下溢出", // FPE_UNDERFLOW
// "精度损失", // FPE_INEXACT
// "仿真出错", // FPE_UNEMULATED
// "负数开方", // FPE_SQRTNEG
// "未知", // Unknown error
// "未知", // Unknown error
// "堆栈上溢", // FPE_STACKOVERFLOW
// "堆栈下溢", // FPE_STACKUNDERFLOW
// "用户激发", // FPE_EXPLICITGEN
//  1.#INF00 0x7FF0000000000000 正无穷，如1.0/0.0会产生一个INF无穷大
// -1.#INF00 0xFFF0000000000000 负无穷，如-1.0/0.0会产生一个负无穷大
//  1.#QNAN0 0x7fffffffffffffff 指Quiet Not a Number
// 给负数开平方根等溢出或无效运算就会产生一个NAN结果的表示值
// IND 则表示无限小，但不确定
// 特殊浮点数1.#IND 1.#INF的判断
//一、特殊浮点数的含义
// 1.#INF / inf：这个值表示“无穷大 (infinity 的缩写)”，即超出了计算机可以表示的浮点数的最大范围（或者说超过了 double 类型的最大值）。例如，当用 0 除一个整数时便会得到一个1.#INF / inf值；相应的，如果用 0 除一个负整数也会得到 -1.#INF / -inf 值。
// -1.#IND / nan：这个的情况更复杂，一般来说，它们来自于任何未定义结果（非法）的浮点数运算。"IND"是 indeterminate 的缩写，而"nan"是 not a number 的缩写。产生这个值的常见例子有：对负数开平方，对负数取对数，0.0/0.0，0.0*∞, ∞/∞ 等。
// 二、特殊浮点数的判断
// C 库提供了一组函数用来判断一个浮点数是否是无穷大或 NaN。
// int _isnan(double x) 函数用来判断一个浮点数是否是 NaN，而 int _finite(double x) 用以判断一个浮点数是否是无穷大。
// 使用时要 #include "float.h"
// 也可以用下面的代码实现：
// 这里的比较操作看上去总是会得到 true
// 但有趣的是对于浮点数 NaN 来说总会得到 false!
// bool IsNumber(double x) { return (x == x); }
// 而下面的代码可以判断一个浮点数是否是有限的（finite, 即既不是 NaN 又不是 infinite）：
// bool IsFiniteNumber(double x) { return (x <= DBL_MAX && x >= -DBL_MAX); }
// 其中，DBL_MAX 是 <float.h> 中预定义的常量： #define DBL_MAX         1.7976931348623158e+308
int test_double()
{
  int t = 0;
  union {
    uint64 i;
    double d;
  } num;
  num.i = 0x7fefffffffffffffu; // DBL_MAX
  t = _isnan(num.d);
  t = _finite(num.d);
  //printf("%f\n", num.d);
  num.i = 0x7fffffffffffffff;
  t = _isnan(num.d);
  t = _finite(num.d);
  printf("%f\n", num.d); // 1.#QNAN0
  num.i = 0x7FF0000000000000;
  t = _isnan(num.d);
  t = _finite(num.d);
  printf("%f\n", num.d); // 1.#INF00
  num.i = 0xfff8000000000000;
  t = _isnan(num.d);
  t = _finite(num.d);
  printf("%f\n", num.d); // -1.#IND00
  return 0;
}

