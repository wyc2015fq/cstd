# 第k个排列 - guoziqing506的博客 - CSDN博客





2016年07月03日 10:42:56[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1068
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给定 n 和 k，求123..n组成的排列中的第 k 个排列。

样例：对于 n = 3, 所有的排列如下：
123

132

213

231

312

321

如果 k = 4, 第4个排列为，231.




关于字典序和排列的概念请翻阅我之前的博文（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51787763)），由这些概念可知，如果一个排列是第k个排列，那么一定满足他之前的k - 1个排列是按字典序排列好的。而且满足以下方程：

k - 1 = [order(result[0]) - 1] * (n - 1)! + [order(result[1]) - 1] * (n - 2)! + ...... + [order(result[n - 1]) - 1] * 1

我解释以下上面方程的含义：result表示我们要求的第k个排列，只不过此处用一个整型数组表示出来。order(result[i])表示result[i]这个数在升序的数组当中的次序（当然这个升序数组是排除了result之前的元素构成的）。

举个例子：比如给出n = 3，那么原始的升序数组应该是[1, 2, 3]，假设result = [2, 3, 1]，那么，order(result[0]) = 2，因为2是[1, 2, 3]中的第2个数，而order(result[1]) = 2，因为排除了结果列表的第一个数2后，升序数组变为[1, 3]，所以说，3在第2位，order(result[1])
 = 2。

那么，[order(result[i]) - 1] * (n - i -1)! 就表示以结果列表的第 i 个数为开头，一直到最后的的可能的排列数。




反过来推，如果已知某个排列是第k个排列，那么可以依次从前往后，确定这个排列每个位置的数字。原则是求使得[order(num) - 1] * (n - 1 - index)不超出界限的最大num。所谓不超过界限就是和前面计算结果的累计小于k。举例来说：

已知n = 3, k = 4，

1. (order(1) - 1) * (3 - 1)! = 0 < 4，继续；

2. (order(2) - 1) * (3 - 1)! = 2 < 4，继续；

3. (order(3) - 1) * (3 - 1)! = 4 == 4，说明结果的第一位不能是3，应该是2；

那也就是说，这个结果前面的排列中以1打头的两个排列一定都存在，我们排除掉，此时， k = 4 - 2 = 2，同时，升序列表变为[1, 3]

4.
(order(1) - 1) * (2 - 1)! = 0 < 2，继续；

5. (order(3) - 1) * (2 - 1)! = 1 * 1 = 1 < 2，但是升序列表遍历完，说明结果的第二位是3；

此时， k = 2 - 1 = 1，升序列表变为[1]

6.
(order(1) - 1) * (1 - 1)! = 0 * 0 = 0 < 1，但是升序列表遍历完，说明结果的第二位是1；



得到第4个排列：231




代码如下：



```python
class Solution:
    """
    @param n: n
    @param k: the k-th permutation
    @return: a string, the k-th permutation
    """
    def getPermutation(self, n, k):
        # 构建升序列表
        nums = [i for i in range(1, n + 1)]
        index = 0
        result = ""
        # index表示结果中数字的位置
        while index < n:
            i = 0
            # 不能超出界限
            while i < len(nums) and i * self.fact(n - 1 - index) < k:
                i += 1
            # 变更界限
            k -= (i - 1) * self.fact(n - 1 - index)
            # 插入结果列表
            result += str(nums[i - 1])
            # 从升序列表中将已插入结果列表的去除
            nums.remove(nums[i - 1])
            index += 1
        return result

    def fact(self, n):
        if n == 0:
            return 0
        result = 1
        for i in range(1, n + 1):
            result *= i
        return result
        # write your code here
```








