# lintcode - 丑数 - guoziqing506的博客 - CSDN博客





2016年08月28日 19:10:46[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1438
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：写一个程序来检测一个整数是不是丑数。丑数的定义是，只包含质因子 2, 3, 5 的正整数。比如 6, 8 就是丑数，但是 14 不是丑数以为他包含了质因子 7。可以认为 1 是一个特殊的丑数。




样例：

给出 num = 8，返回 true。

给出 num = 14，返回 false。





显然，这是一个新颖的概念，在传统的数学概念里，找不到“丑数”这个概念，但是这种新概念的题目有时反而不难，只需要仔细分析其定义即可。




根据定义，丑数是只包含质因子2,3,5的正整数。那么不妨可以由以下4步来判定一个整数是不是丑数：

1. 判断正负：若是负数，可直接返回False

2. 不断除2，直到商还是整数的一次为最后一次

3. 对2步结束后的值，不断除3，直到商还是整数的一次为最后一次

4. 对3步结束后的值，不断除5，直到商还是整数的一次为最后一次




此时，我们看4步结束后的值是不是1：是1，则说明是丑数；不是1，则说明不是丑数。




思路再清晰不过了，给出代码：



```python
class Solution:
    # @param {int} num an integer
    # @return {boolean} true if num is an ugly number or false
    def isUgly(self, num):
        if num <= 0:
            return False
        while num % 2 == 0:
            num = num / 2
        while num % 3 == 0:
            num = num / 3
        while num % 5 == 0:
            num = num / 5
        if num == 1:
            return True
        else:
            return False
```







