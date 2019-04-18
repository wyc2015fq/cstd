# C++：C++在图片特定区域之外产生随机数 - wishchinYang的专栏 - CSDN博客
2017年03月27日 10:54:43[wishchin](https://me.csdn.net/wishchin)阅读数：316
           参考原文：[C++产生随机数](http://www.cnblogs.com/kaituorensheng/archive/2013/03/05/2944008.html) （整数）
           C++在图片特定区域之外产生随机数，避开正则表达式，可以分为两种情况。
           第一种：在某个数之前生成随机数；第二种，生成随机数，加上某个数，然后截断；第三种，指定范围内生成随机数；
1.在某个数之前生成随机数
```cpp
int Thres; //产生[0, dis)之间的随机数，注意不包括dis
printf( "%d\n", Random( Thres ) );
```
2.在两个数之间生成随机数
```cpp
int Random(int m, int n)
{
		int pos, dis;
		if (m == n){
			return m;
		}
		else if (m > n){
			pos = n;
			dis = m - n + 1;
			return rand() % dis + pos;
		}
		else
		{
			pos = m;
			dis = n - m + 1;
			return rand() % dis + pos;
		}
}
```
测试代码：
```cpp
int TestRand(int argc, _TCHAR* argv[])
{
		srand((int)time(NULL));//用于随机 随机种子
		int m = 7;
		int n = 23;
		for (int i = 0; i < 100; ++i)
		{
			float f = Random(m, n);
			std::cout << f << std::endl;
		}
		return 1;
}
```
产生浮点数随机数
