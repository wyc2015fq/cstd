# Fizz Buzz 问题 - guoziqing506的博客 - CSDN博客





2016年06月07日 09:06:53[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：743
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给你一个整数n. 从 1 到 n 按照下面的规则打印每个数：

如果这个数被3整除，打印fizz.

如果这个数被5整除，打印buzz.

如果这个数能同时被3和5整除，打印fizz buzz.

样例：比如 n = 15, 返回一个字符串数组：
[

  "1", "2", "fizz",

  "4", "buzz", "fizz",

  "7", "8", "fizz",

  "buzz", "11", "fizz",

  "13", "14", "fizz buzz"

]




lintcode中最简单的题目，不讲了，把代码放这：



```python
class Solution:
    """
    @param n: An integer as description
    @return: A list of strings.
    For example, if n = 7, your code should return
        ["1", "2", "fizz", "4", "buzz", "fizz", "7"]
    """
    def fizzBuzz(self, n):
        results = []
        for i in range(1, n+1):
            if i % 15 == 0:
                results.append("fizz buzz")
            elif i % 5 == 0:
                results.append("buzz")
            elif i % 3 == 0:
                results.append("fizz")
            else:
                results.append(str(i))
        return results
```





