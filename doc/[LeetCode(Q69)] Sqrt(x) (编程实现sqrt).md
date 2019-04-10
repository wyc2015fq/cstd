#  			[[LeetCode(Q69)\] Sqrt(x) (编程实现sqrt)](https://www.cnblogs.com/AnnieKim/archive/2013/04/18/3028607.html) 		



**Q:** 

Implement *int sqrt(int x)*.

Compute and return the square root of x.

**A:**

这里给出两种实现方法：一是二分搜索，二是牛顿迭代法。

#### 1. 二分搜索

对于一个非负数n，它的平方根不会小于大于（n/2+1）（谢谢@linzhi-cs提醒）。在[0, n/2+1]这个范围内可以进行二分搜索，求出n的平方根。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 int sqrt(int x) {
 2     long long i = 0;
 3     long long j = x / 2 + 1;
 4     while (i <= j)
 5     {
 6         long long mid = (i + j) / 2;
 7         long long sq = mid * mid;
 8         if (sq == x) return mid;
 9         else if (sq < x) i = mid + 1;
10         else j = mid - 1;
11     }
12     return j;
13 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

注：在中间过程计算平方的时候可能出现溢出，所以用long long。

#### 2. 牛顿迭代法

![img](https://images0.cnblogs.com/blog/300640/201304/18155235-b272cc444a1845d3aede4c72a87f83dc.jpg)

   为了方便理解，就先以本题为例：

   计算x2 = n的解，令f(x)=x2-n，相当于求解f(x)=0的解，如左图所示。

   首先取x0，如果x0不是解，做一个经过(x0,f(x0))这个点的切线，与x轴的交点为x1。

   同样的道理，如果x1不是解，做一个经过(x1,f(x1))这个点的切线，与x轴的交点为x2。

   以此类推。

   以这样的方式得到的xi会无限趋近于f(x)=0的解。

   判断xi是否是f(x)=0的解有两种方法：

   一是直接计算f(xi)的值判断是否为0，二是判断前后两个解xi和xi-1是否无限接近。

 

经过(xi, f(xi))这个点的切线方程为f(x) = f(xi) + f’(xi)(x - xi)，其中f'(x)为f(x)的导数，本题中为2x。令切线方程等于0，即可求出xi+1=xi - f(xi) / f'(xi)。

继续化简，xi+1=xi - (xi2 - n) / (2xi) = xi - xi / 2 + n / (2xi) = xi / 2 + n / 2xi = (xi + n/xi) / 2。

有了迭代公式，程序就好写了。关于牛顿迭代法，可以参考[wikipedia](http://en.wikipedia.org/wiki/Newton%27s_method)以及[百度百科](http://baike.baidu.com/view/643093.htm)。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 int sqrt(int x) {
 2     if (x == 0) return 0;
 3     double last = 0;
 4     double res = 1;
 5     while (res != last)
 6     {
 7         last = res;
 8         res = (res + x / res) / 2;
 9     }
10     return int(res);
11 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

牛顿迭代法也同样可以用于求解多次方程的解。

P.S. 本题是求解整数的平方根，并且返回值也是整型。在上述代码基础上稍微做修改，就可以同样适用于double（仅限方法2）。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 double sqrt(double x) {
 2     if (x == 0) return 0;
 3     double last = 0.0;
 4     double res = 1.0;
 5     while (res != last)
 6     {
 7         last = res;
 8         res = (res + x / res) / 2;
 9     }
10     return res;
11 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

关于[LeetCode](http://www.leetcode.com/)的其他题目，可以参考我的[GitHub](https://github.com/AnnieKim/Leetcode)。

references：本文讲解牛顿迭代法使用的图片来自wikipedia。

原创文章，转载请注明出处：<http://www.cnblogs.com/AnnieKim/>。