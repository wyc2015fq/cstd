# 背包问题 II - guoziqing506的博客 - CSDN博客





2016年06月27日 17:02:46[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：449
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给出n个物品的体积A[i]和其价值V[i]，将他们装入一个大小为m的背包，最多能装入的总价值有多大？

样例：对于物品体积[2, 3, 5, 7]和对应的价值[1, 5, 2, 4], 假设背包大小为10的话，最大能够装入的价值为9。




做这道题之前，如果你不知道在不考虑对应价值的情况下的“背包问题”，请出门左转，看我的上一篇博文（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51768819)）。

因为很多思路和上一篇是类似的，所以这篇我会假设你已经完全搞懂了上一篇博文所描述的问题，省去大量细节。




用二维数组record[i][j]表示前 i 个物品装在空间为 j 的背包中所能获得的最大价值，注意，和上一道背包问题不同，这里记录的是最大价值，而不是“恰好”的价值，那么相似的逻辑，可以得到状态转移方程：

record[i + 1][j] = max(record[i][j], V[i] + record[i][j - A[i]])

跟上一题的思路一样，要么不放A[i]，要么放A[i]，放了A[i]就一定需要先知道减去A[i]这个大小后所能达到的最大收益。当然，代码中实现状态转移方程的时候要注意判断 j - A[i] 是否是大于等于0的，这一点与上一题类似。

代码如下：



```python
class Solution:
    # @param m: An integer m denotes the size of a backpack
    # @param A & V: Given n items with size A[i] and value V[i]
    # @return: The maximum value
    def backPackII(self, m, A, V):
        n = len(A)
        if m == 0 or len(A) == 0:
            return 0
        record = [0 for j in range(m + 1)]
        j = 0
        while j <= m:
            if j >= A[0]:
                record[j] = V[0]
            j += 1

        i = 1
        result = V[0]
        while i < n:
            j = 0
            temp = []
            while j <= m:
                p = record[j - A[i]] + V[i] if j - A[i] >= 0 else 0
                temp.append(max(record[j], p))
                j += 1
            result = max(result, max(temp))
            record = temp
            i += 1
        return result
        # write your code here
```


是上一题基础上的延伸，本身没什么可多说的。
            


