# lintcode - 用递归打印数字 - guoziqing506的博客 - CSDN博客





2016年09月01日 10:33:29[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：901
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：用递归的方法找到从1到最大的N位整数。



样例：

给出 N = 1, 返回[1,2,3,4,5,6,7,8,9].

给出 N = 2, 返回[1,2,3,4,5,6,7,8,9,10,11,...,99].




题目要求用递归的方法。那就先来看一下，当N = i 和 N = i + 1时，返回的这个结果列表时怎样转化的。找到“升级”的方式，是递归算法最难的一环。




拿 N = 1 和 N = 2为例，我们可以总结出这样的规律：N = 2时，返回的数都是两位数，所以如果用1~9分别乘10^1，就得到了10, 20, 30, ..., 90，这些数中，我们给每个数分别加上N = 1时返回的列表中的数，就构成了10, 11, 12, ..., 99，当然，再给这个新构成的序列前头附上N = 1时的序列，就是N = 2时的序列。如果再尝试一下N = 3时与N = 2时的升级过程也是这样的。




而这个递归算法“触底”的条件显而易见：N = 1时，返回列表[1, 2, ..., 9]




按照上面的思路写出代码即可：



```python
class Solution:
    # @param n: An integer.
    # return : A list of integer storing 1 to the largest number with n digits.
    def numbersByRecursion(self, n):

        # 触底条件
        if n == 1:
            return [i for i in range(1, 10)]

        # 初始化列表
        result = []
        
        if n >= 2:
            temp = self.numbersByRecursion(n - 1)
            # i: 1~9
            for i in range(1, 10):
                result.append(i * pow(10, n - 1))
                for j in temp:
                    result.append(j + i * pow(10, n - 1))
            result = temp + result
        return result
```













