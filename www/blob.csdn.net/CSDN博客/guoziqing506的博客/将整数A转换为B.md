# 将整数A转换为B - guoziqing506的博客 - CSDN博客





2016年06月07日 09:40:33[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：935
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：如果要将整数A转换为B，需要改变多少个bit位？

样例：如把31转换为14，需要改变2个bit位。

(31)10=(11111)2

(14)10=(01110)2




之前，我说过位运算的一些基本操作（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51548511)），此处其实求的是两个二进制中不同的位的个数。所以，可以采用两种方法。

1. 直接求出两个数的二进制表示，然后比较两个01字符串的不同的位的个数

2. 先对两个数异或，这样，求出的二进制数中1的个数就是两个数的不同位的个数

我这里只给出第二种方法的代码。

因为之前已经知道怎么求出一个二进制数中1的个数的方法（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51548511)），所以，此处讲解忽略。还是注意python中对负数二进制的处理就行。代码如下：



```python
class Solution:
    """
    @param a, b: Two integer
    return: An integer
    """
    def bitSwapRequired(self, a, b):
        a ^= b
        if a > 0:
            return self.helper(a)
        else:
            temp = abs(a - 1)
            return 32 - self.helper(temp)

    # helper()统计正数的二进制中1的个数
    def helper(self, n):
        count = 0
        while n != 0:
            n = n & (n - 1)
            count += 1
        return count
        # write your code here
```






先对两个数异或，再求解异或之后的二进制值中1的个数。




