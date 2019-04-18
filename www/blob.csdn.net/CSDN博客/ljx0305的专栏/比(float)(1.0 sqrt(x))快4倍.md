# 比(float)(1.0/sqrt(x))快4倍 - ljx0305的专栏 - CSDN博客
2011年02月24日 14:38:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1322
Quake III公开源码后，有人在game/code/q_math.c里发现了这样一段代码。它的作用是将一个数开平方并取倒，经测试这段代码比(float)(1.0/sqrt(x))快4倍：
float Q_rsqrt( float number )
{
  long i;
  float x2, y;
  const float threehalfs = 1.5F;
  x2 = number * 0.5F;
  y  = number;
  i  = * ( long * ) &y;  // evil floating point bit level hacking
  i  = 0x5f3759df - ( i >> 1 ); // what the fuck?
  y  = * ( float * ) &i;
  y  = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
  // y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed
  #ifndef Q3_VM
  #ifdef __linux__
    assert( !isnan(y) ); // bk010122 - FPE?
  #endif
  #endif
  return y;
}
    code/common/cm_trace.c中也出现了这样一段解释sqrt(x)的函数，与上面的代码唯一不同的就是这个函数返回的是number*y：
/*
================
SquareRootFloat
================
*/
float SquareRootFloat(float number) {
    long i;
    float x, y;
    const float f = 1.5F;
    x = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( f - ( x * y * y ) );
    y  = y * ( f - ( x * y * y ) );
    return number * y;
}
    这样的代码速度肯定飞快，我就不用多说了；但算法的原理是什么呢？其实说穿了也不是很神，程序首先猜测了一个接近1/sqrt(number)的值，然后两次使用牛顿迭代法进行迭代。根号a的倒数实际上就是方程1/x^2 - a = 0的一个正实根，它的导数是-2/x^3。运用牛顿迭代公式x' = x - f(x)/f'(x)，式子化简为x' = x * (1.5 - 0.5a * x^2)。迭代几次后，x的值将趋于1/sqrt(a)。
    但这段代码真正牛B的是那个神秘的0x5f3759df，因为0x5f3759df - (i >> 1)出人意料地接近根号y的倒数。人们都不知道这个神秘的常数是怎么来的，只能把它当作神来膜拜。这个富有传奇色彩的常数到底咋回事，很少有人说得清楚
