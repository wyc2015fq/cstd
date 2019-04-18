# Divide Two Integers ——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月15日 13:23:12[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：521











    【题目】




Divide two integers without using multiplication, division and mod operator.


If it is overflow, return MAX_INT.


    【分析】

    基本思想：任何数都可以分成二进制的幂的线性表示。

    例子：以87除4举例， (4 * 2 = 8) => (8 * 2 = 16) => (16 * 2 = 32) => (32 * 2) => 64，因为64 * 2 = 128大于87，现在我们可以确定4 * 16 = 64小于87，那么再处理87 - 64 = 23，23除4的话用上面方法可以得到5，还余3，因为小于4，扔掉，所以最后结果是16 + 5 = 21。因为需要考虑溢出问题，所以先把数字转换为unsigned
 long long，最后转成int返回。




    【代码】



```cpp
class Solution {
     public:
    int divide(int dividend, int divisor) {
        long long dividend2 = dividend;
        long long divisor2 = divisor;
        if (divisor2 == 0 || (dividend == 1 << 31 && divisor == -1))
            return INT_MAX;
        int negtive = (dividend2 < 0) ^ (divisor2 < 0);
        dividend2 = abs(dividend2);
        divisor2 = abs(divisor2);
        long long tmp = divisor2;
        int r = 1;
        while ((tmp << 1) <= dividend2)
        {
            tmp <<= 1;
            r <<= 1;
        }
        int res = 0;
        while (tmp >= divisor2)
        {
            if (dividend2 >= tmp)
            {
                res += r;
                dividend2 -= tmp;
            }
            tmp >>= 1;
            r >>= 1;
        }

        return (negtive) ? -res : res;
    }
};
```





    始终有临界值报错的代码：（后面再修改）



```cpp
class Solution {
public:
    int divide(int dividend, int divisor) {
        if(divisor == 0 || (dividend == 1 << 31 && divisor == -1))
            return INT_MAX;
        int flag;
        if(dividend * divisor < 0)
            flag = -1;
        else 
            flag = 1;
        
        unsigned long long dividend2 = abs((long long)dividend);
        unsigned long long divisor2 = abs((long long)divisor);
        
        long long res = 0;
        while(divisor2 <= dividend2)
        {
            unsigned long long tmp = divisor2, left;
            long long r = 1; 
            while(tmp <= dividend2)
            {
                tmp = tmp<<1;
                r = r<<1;
            }
            res += (r>>1);
            dividend2 -= (tmp>>1);
        }
        return res * flag;
    }
};
```





