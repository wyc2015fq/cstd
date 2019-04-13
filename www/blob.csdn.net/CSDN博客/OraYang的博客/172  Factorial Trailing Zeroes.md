
# 172. Factorial Trailing Zeroes - OraYang的博客 - CSDN博客

2017年07月27日 10:57:35[OraYang](https://me.csdn.net/u010665216)阅读数：177标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目:
Given an integern, return the number of trailing zeroes inn!.
Note:Your solution should be in logarithmic time complexity.
思路：
1. 本题在没时间与空间复杂度的情况下，思路很简单，即先求阶乘再求尾随0个数
代码：

```python
class Solution {
public:
    int trailingZeroes(int n) {
    unsigned long long nValue = 1;
    for (int i = 2; i <= n; i ++)
    {
        nValue *=i;
    }
    int Count = 0;
        
    while(0 ==nValue%10)
    {
        Count ++;
        nValue /= 10;
    }
    return Count;
    }
};
```
2. 但是要在对数时间内完成算法，这需要对n！有一定分析，分析发现：
1！ = 1
2！ = 1 * 2 = 2
3！ = 1 * 2 *3 = 6
4！ = 1 * 2 * 3 * 4 = 24
5！ = 1 * 2 * 3 * 4 * 5 = 120
........
1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13
* 14 * 15 * 16 * 17 * 18 * 19 * 20 * 21 * 22 *23 * 24 * 25从中我们得到一个因子2和因子5组合产生一个0，这样我们只需统计1到n有多少个因子对，即n！的尾随零个数，因子2的个数比因子5的个数多，因此我们只需统计出因子5的个数即可，但是像5的m幂次存在m个5的组合，所以这些再次统计，用递归的思想实现。
代码：

```python
class Solution {
public:
    int trailingZeroes(int n) {
    int count = 0;
    if (n == 0)
        return 0;
    else
        return n/5 + trailingZeroes(n / 5);
    }
};
```



