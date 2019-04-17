# lintcode - 丑数 II - guoziqing506的博客 - CSDN博客





2016年08月28日 19:38:05[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：2184
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)








题目描述：设计一个算法，找出只含素因子2，3，5 的第 n 大的数。符合条件的数如：1, 2, 3, 4, 5, 6, 8, 9, 10, 12...我们可以认为1也是一个丑数


样例：如果n = 9， 返回 10




之前，做过一道类似的题目：[丑数](http://blog.csdn.net/guoziqing506/article/details/52347148)。题目的要求是判别一个整数是不是丑数。而本题的目的是计算第n个丑数是多少。所以，一种朴素的思路是这样：设定一个计数器，初始为0. 然后从1开始，遍历正整数，每遍历到一个，就用上题（详情查看前面链接）中判断丑数的函数判断，是丑数，对计数器加1，知道计数器为n为止。




上面这个做法当然是行得通的，但是效率不好，因为每次对遍历到的正整数都要进行那样一个时间复杂度偏高的算法。所以，现在尝试看能不能有效率更高的办法。




首先，有这样一个逻辑：如果我们把所有的丑数依次按升序存储到一个列表里面，那么这个列表后面的某个元素，一定是前面的某个元素乘2、乘3、或乘5得到的。所以，假设现在我们已知这样一个列表的一部分，想要往列表里面添加新元素的话，那么列表的下一个要添加元素可以这样计算：

1. 依次遍历现在列表中的元素，并计算这些元素乘2的结果，直到遇到第一个比现在列表中最后一个元素（也就是最大的那个）大的元素，我们将这个元素乘2的结果记为M2

2. 与1步同理，处理这些元素乘3的结果，找到第一个乘3比最后元素大的元素，把他乘3的结果记为M5

3. 与2步同理，可得到相应的M5

4. 最后，将min(M2, M3, M5)添加到列表最后，作为新元素即可。




其实，效率还可以再高一点。因为这个列表已经是排好序的，所以我们可以设定三个指针index2, index3, index5，他们所指向的元素，乘2，乘3，乘5分别为此时的M2, M3, M5，每次遍历结束之后，只需要从这三个指针的位置（含）开始向后扫描即可。




代码如下：



```python
class Solution:
    """
    @param {int} n an integer.
    @return {int} the nth prime number as description.
    """
    def nthUglyNumber(self, n):
        record = [1]
        if n == 1:
            return record[0]

        index2, index3, index5 = 0, 0, 0
        M2, M3, M5 = record[index2] * 2, record[index3] * 3, record[index5] * 5

        while len(record) == n:

            while M2 <= record[-1]:
                index2 += 1
                M2 = record[index2] * 2
            while M3 <= record[-1]:
                index3 += 1
                M3 = record[index3] * 3
            while M5 <= record[-1]:
                index5 += 1
                M2 = record[index5] * 5

            record.append(min(M2, M3, M5))
        return record[-1]
```

思路很清楚，不给注释了。










