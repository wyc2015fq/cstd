# [LeetCode] Sqrt(x) 求平方根 - Grandyang - 博客园







# [[LeetCode] Sqrt(x) 求平方根](https://www.cnblogs.com/grandyang/p/4346413.html)







Implement `int sqrt(int x)`.

Compute and return the square root of *x*.



这道题要求我们求平方根，我们能想到的方法就是算一个候选值的平方，然后和x比较大小，为了缩短查找时间，我们采用二分搜索法来找平方根，这里属于博主之前总结的[LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html)中的第三类的变形，找最后一个不大于目标值的数，代码如下：



解法一：

```
class Solution {
public:
    int mySqrt(int x) {
        if (x <= 1) return x;
        int left = 0, right = x;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (x / mid >= mid) left = mid + 1;
            else right = mid;
        }
        return right - 1;
    }
};
```



这道题还有另一种解法，是利用[牛顿迭代法](http://zh.wikipedia.org/wiki/%E7%89%9B%E9%A1%BF%E6%B3%95)，记得高数中好像讲到过这个方法，是用逼近法求方程根的神器，在这里也可以借用一下，可参见网友[Annie Kim's Blog的博客](http://www.cnblogs.com/AnnieKim/archive/2013/04/18/3028607.html)，因为要求x2 = n的解，令f(x)=x2-n，相当于求解f(x)=0的解，可以求出递推式如下：

xi+1=xi - (xi2 - n) / (2xi) = xi - xi / 2 + n / (2xi) = xi / 2 + n / 2xi = (xi + n/xi) / 2



解法二：

```
class Solution {
public:
    int mySqrt(int x) {
        if (x == 0) return 0;
        double res = 1, pre = 0;
        while (abs(res - pre) > 1e-6) {
            pre = res;
            res = (res + x / res) / 2;
        }
        return int(res);
    }
};
```



也是牛顿迭代法，写法更加简洁一些，注意为了防止越界，声明为长整型，参见代码如下：



解法三：

```
class Solution {
public:
    int mySqrt(int x) {
        long res = x;
        while (res * res > x) {
            res = (res + x / res) / 2;
        }
        return res;
    }
};
```



类似题目：

[Pow(x, n)](http://www.cnblogs.com/grandyang/p/4383775.html)

[Valid Perfect Square](http://www.cnblogs.com/grandyang/p/5619296.html)



参考资料：

[https://leetcode.com/problems/sqrtx/description/](https://leetcode.com/problems/sqrtx/description/)

[https://leetcode.com/problems/sqrtx/discuss/25130/My-clean-C++-code-8ms](https://leetcode.com/problems/sqrtx/discuss/25130/My-clean-C++-code-8ms)

[https://leetcode.com/problems/sqrtx/discuss/25047/A-Binary-Search-Solution](https://leetcode.com/problems/sqrtx/discuss/25047/A-Binary-Search-Solution)

[https://leetcode.com/problems/sqrtx/discuss/25057/3-4-short-lines-Integer-Newton-Every-Language](https://leetcode.com/problems/sqrtx/discuss/25057/3-4-short-lines-Integer-Newton-Every-Language)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












