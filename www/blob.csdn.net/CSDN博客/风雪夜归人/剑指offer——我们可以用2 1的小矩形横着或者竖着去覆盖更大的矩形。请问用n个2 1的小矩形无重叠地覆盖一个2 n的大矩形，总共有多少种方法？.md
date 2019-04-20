# 剑指offer——我们可以用2*1的小矩形横着或者竖着去覆盖更大的矩形。请问用n个2*1的小矩形无重叠地覆盖一个2*n的大矩形，总共有多少种方法？ - 风雪夜归人 - CSDN博客
2018年04月26日 21:46:49[cdjccio](https://me.csdn.net/qq_34624951)阅读数：516
Q：我们可以用2*1的小矩形横着或者竖着去覆盖更大的矩形。请问用n个2*1的小矩形无重叠地覆盖一个2*n的大矩形，总共有多少种方法？
A： 既然是长条形的，那么从后向前，最后一个矩形2*2的，只有两种情况：
       第一种是最后是由一个2*(n-1)的矩形加上一个竖着的2*1的矩形
　　另一种是由一个2*(n-2)的矩形，加上两个横着的2*1的矩形
　　因此我们可以得出，
　　第2*n个矩形的覆盖方法等于第2*(n-1)加上第2*(n-2)的方法。
      1、递归
      2、迭代
```cpp
class Solution {
public:
    int rectCover(int number) {
        // 递归
        /*
        if(number <= 0)
        {
            return 0;
        }
        else if(number == 1 || number == 2)
        {
            return number;
        }
        else
        {
            return rectCover(number - 1) + rectCover(number - 2);
        }
    }
    */
        // 迭代
        int g = 1, f = 2;
        if(number <= 0)
        {
            return 0;
        }
        else if(number == 1 || number == 2)
        {
            return number;
        }
        else
        {
            while(--number)
            {
                f = f + g; 
                g = f - g;
            }
            return g;
        }
    }
};
```
