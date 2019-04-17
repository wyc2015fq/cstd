# 硬币排成线 II - guoziqing506的博客 - CSDN博客





2016年06月26日 20:34:46[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：781
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：有 n 个不同价值的硬币排成一条线。两个参赛者轮流从左边依次拿走 1 或 2 个硬币，直到没有硬币为止。计算两个人分别拿到的硬币总价值，价值高的人获胜。请判定 第一个玩家 是输还是赢？

样例：
给定数组 A = [1,2,2], 返回 true.

给定数组 A = [1,2,4], 返回 false.




动态规划+博弈论的问题。

设现在的两个玩家分别为player1, player2，同时，我们记从第 i 个硬币开始一直向右拿，拿到最后所能得到的最大收益为record[i]. 这里需要注意一下，我所谓的一直向右拿，当然是player1, player2一起进行游戏的结果。那也就是说，在我们题目中所设定的游戏当中，record[0]就是player1所能得到的最大收益，而同理，硬币的总价值，我们记为sum(values)，减去record[0]的值就是player2所能得到的最大收益（因为player1先拿嘛，所以player2的最大收益只能“被动”取决于player1），用公式表示为sum(values)
 - record[0]

因此，需要解决的问题是：计算record[0].

容易知道，record[n - 1] = values[n - 1]；record[n - 2] = values[n - 1] + values[n - 2]。那么record[n - 3]呢？

从博弈的角度出发，record[i]应该满足以下的规则：




1. 若player1只拿了values[i]这个硬币：

（1）player2只拿了values[i + 1]，那么当游戏结束，player1能得到的最大收益为c1 = values[i] + record[i + 2]

（2）player2同时拿了values[i + 1]，values[i + 2]，那么当游戏结束，player1能得到的最大收益为c2 = values[i]
 + record[i + 3]

当然，player2不傻，他一定会选择最优策略，就是让player1收益最小的那一种，所以综合考虑的结果是record[i]
 = min(c1, c2)




2.
 若player1同时拿了values[i]，values[i + 1]这两个硬币：




（1）player2只拿了values[i + 2]，那么当游戏结束，player1能得到的最大收益为c3 = values[i] + 
values[i + 1] + record[i + 3]

（2）player2同时拿了values[i + 1]，values[i + 2]，那么当游戏结束，player1能得到的最大收益为c4 = 
values[i] + values[i + 1] + record[i + 4]

同理，player2选择最优策略，record[i] = min(c3, c4)

那你说player1怎么选择呢？选上面的1还是2？他也要选最优，max(min(c1, c2), min(c3, c4))




这就算是得到最优子结构了，给出代码就行，需要注意的是对于索引取值问题，必须加上合适的判断语句，防止出错。

代码如下：



```python
class Solution:
    # @param values: a list of integers
    # @return: a boolean which equals to True if the first player will win
    def firstWillWin(self, values):
        n = len(values)
        record = [0 for i in range(n)]
        record[n - 1] = values[n - 1]
        if n == 1:
            return record[n - 1]
        record[n - 2] = values[n - 1] + values[n - 2]
        i = n - 3
        while i >= 0:

            # player1只取values[i]
            # c1为player2只取values[i + 1]时，player1到最后能得到的最大收益
            # c2为player2同时取values[i + 1]，values[i + 2]时，player1到最后能得到的最大收益
            c1 = values[i] + record[i + 2]

            if i + 3 < n:
                c2 = values[i] + record[i + 3]
            else:  
                c2 = values[i]

            # player1同时取values[i]，values[i + 1]
            # c1为player2只取values[i + 2]时，player1到最后能得到的最大收益
            # c2为player2同时取values[i + 2]，values[i + 3]时，player1到最后能得到的最大收益
            if i + 3 < n:
                c3 = values[i] + values[i + 1] + record[i + 3]
            else:
                c3 = values[i] + values[i + 1]

            if i + 4 < n:
                c4 = values[i] + values[i + 1] + record[i + 4]
            else:
                c4 = values[i] + values[i + 1]

            record[i] = max(min(c1, c2), min(c3, c4))
            i -= 1
        return sum(values) - record[0] < record[0]
        # write your code here
```











