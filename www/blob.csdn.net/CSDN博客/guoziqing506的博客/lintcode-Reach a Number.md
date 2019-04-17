# lintcode - Reach a Number - guoziqing506的博客 - CSDN博客





2018年04月09日 21:38:32[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：214
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：我简单将题目用汉语描述一下。说初始位置在0，给定一个目标值target（target是一个整数），现在让你每次走一步，每次走的步长和已经走的步数相等（第一次走1，第二次走2，。。。），每次可以向左或者向右走（也就是加正数或者负数），问最少走多少次可以走到target.

解题思路：

一开始想到的自然是广度优先搜索，但是我的运行时间出了问题。所以想到要优化算法。其实就我目前接触的算法而言，感觉如果要对算法的效率实现质的飞跃，靠的只能是通过其最本质的数学原理，这一点偷不了懒，没有捷径可走。

这道题目也不例外，我们这么来思考：不失一般性，考虑target为正的情况（为负的情况原理是一样的，你可以类比），现在假如存在$1 + 2 + \dots + k = target$（相当于一直往右走），那毫无疑问$k$就是我们要求的最优步数。但是更为普遍的情况是，不能恰好相等，那我们就找刚好大于target的情况。也就是说，存在一个k，使得$S = 1 + 2 + \dots + k$为大于target的最小值（$1 + 2 + \dots + k > target$ 且 $1 + 2 + \dots + k - 1 < target$）

现在考虑S与target的差值，我记为gap，$gap = S - target$，那么有两种情况：
- 
gap为偶数，那么因为$gap < k$（如果$gap \geq k$的话，$1 + 2 + \dots + k - 1 \geq target$，不合理），所以我们在$\{1, 2, \dots, k\}$中可以找到一个子集，使这个子集加和的值为$gap/2$，那么我们将这个子集的所有整数的符号变为负，就是走步的情况了，也就是说，此时$k$即使我们所求。

- 
gap为奇数，这时不论我们怎么做，都不可能通过改变$\{1, 2, \dots, k\}$中某个子集的符号，使gap被抵消（因为gap此时是奇数，除2除不尽），那么怎么办？继续加$(k + 1)$呗，此时$gap = gap + k + 1$，和上面同理，若是奇数，没办法，接着加$k + 2$吧，若是偶数，恭喜你，$k + 1$就是答案了。根据加法的基本原理，如果不是$k + 1$，那答案就只能是$k + 2$（这块你要不懂就没办法了，补小学数学吧）


给出代码如下：

```python
class Solution:
    """
    @param target: the destination
    @return: the minimum number of steps
    """
    def reachNumber(self, target):
        step, count = 0, 0
        while count < abs(target):
            step += 1
            count += step

        gap = count - target
        if gap % 2 == 0:
            return step
        elif (gap + (step + 1)) % 2 == 0:
            return step + 1
        else:
            return step + 2
        # Write your code here
```

有关于lintcode题目的答案代码，我打算整理一个完整的版本，并放在我的github上。已经在博客中写过的，我也会在后期慢慢加入。另外，一些我觉得过于简单的题目就不再博客中特别记录了。不过github上会有相关的代码。欢迎大家批评指正。网址：[Lintcode-Answer](https://github.com/guoziqingbupt/Lintcode-Answer)



