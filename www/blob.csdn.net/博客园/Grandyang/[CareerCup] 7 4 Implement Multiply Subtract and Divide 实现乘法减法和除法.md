# [CareerCup] 7.4 Implement Multiply Subtract and Divide 实现乘法减法和除法 - Grandyang - 博客园







# [[CareerCup] 7.4 Implement Multiply Subtract and Divide 实现乘法减法和除法](https://www.cnblogs.com/grandyang/p/4770285.html)







7.4 Write methods to implement the multiply, subtract, and divide operations for integers. Use only the add operator.



这道题让我们实现乘法加法和除法，而且规定了只能使用加法。那么我们先来看如何用加法来实现减法，我们知道，减去一个数就等于加上这个数的负数。那么我们先写一个求负数的函数，对于n来说，我们累计n个-1，对于-n来说，我们累计n个1。这样减法的就搞定了，我们再来看乘法，乘法也可以看做加法的一种，比如a乘以b，就是b个a相加的结果，我们最后来判断正负，如果a和b的符号相反，我们再调用之前的求负数的函数即可。最后就是除法了，除法也可以转为加法，比如a除以b，那么我们可以先初始化商为0，然后用商加上b和a比较，如果小的话，商就加一，再比较，以此类推直到大于等于a，符号也是最后来判断，这样我们就用加法实现了乘法减法和除法了，参见代码如下：



```
class Solution {
public:
    int negate(int a) {
        int res = 0, d = a < 0 ? 1 : -1;
        while (a != 0) {
            res += d;
            a += d;
        }
        return res;
    }
    int minus(int a, int b) {
        return a + negate(b);
    }
    int multiply(int a, int b) {
        if (a < b) return multiply(b, a);
        int res = 0;
        for (int i = 0; i < abs(b); ++i) {
            res += a;
        }
        return b > 0 ? res : negate(res);
    }
    int divide(int a, int b) {
        if (b == 0) return INT_MAX;
        int m = abs(a), n = abs(b);
        int res = 0, product = 0;
        while (product + n <= m) {
            product += n;
            ++res;
        }
        if ((a < 0 && b < 0) || (a > 0 && b > 0)) return res;
        else return negate(res);
    }
};
```



LeetCode中有一道[Divide Two Integers 两数相除](http://www.cnblogs.com/grandyang/p/4431949.html)的题，那道题的除法实现过程较为复杂，但是算法很高效，可以看看。














