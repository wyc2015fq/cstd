# x的平方根 - guoziqing506的博客 - CSDN博客





2016年04月03日 09:27:53[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：932
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：实现 
```
int
 sqrt(int x)
```
 函数，计算并返回 x 的平方根。




样例：


sqrt(3) = 1

sqrt(4) = 2

sqrt(5) = 2

sqrt(10) = 3




解法：还是典型的二分法的使用，实际上求取的是平方不大于x的最大整数。那么可以用left，right两个指针分别指向两个整数，逼近要求取的值。于是，按照二分法的“标配”（[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/50957775)），经过while循环得到left，这里的left的平方刚刚大于整数x。于是令返回left-1即可。



```python
class Solution:
    """
    @param x: An integer
    @return: The sqrt of x
    """
    def sqrt(self, x):
        left, right = 0, x
        while left <= right:
            mid = (left + right) // 2
            if pow(mid, 2) > x:
                right = mid - 1
            else:
                left = mid + 1
        return left - 1
        # write your code here
```









