# lintcode - 加油站 - guoziqing506的博客 - CSDN博客





2016年05月20日 09:34:20[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：358
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：在一条环路上有 N 个加油站，其中第 i 个加油站有汽油gas[i]，并且从第_i_个加油站前往第_i_+1个加油站需要消耗汽油cost[i]。你有一辆油箱容量无限大的汽车，现在要从某一个加油站出发绕环路一周，一开始油箱为空。求可环绕环路一周时出发的加油站的编号，若不存在环绕一周的方案，则返回-1。

样例：现在有4个加油站，汽油量gas[i]=[1, 1, 3, 1]，环路旅行时消耗的汽油量cost[i]=[2, 2, 1, 1]。则出发的加油站的编号为2。

我们不妨先想想一下，汽车从某一点出发，是如何行走的。在第i个位置的时候，要想从i 前往i + 1，必须满足的条件是：gas[i] - cost[i] > 0，也就是要对两个数组对应位求差，这个差值是判断的依据。

然后再来看车在整个公路上是如何行走的。从第 i 位出发，假设能一直走下去的话，先走到第n位（数组的最后一位），再走到第0位（就是数组的第一位），从第0位开始，继续走一直到第 i 位结束。

因此，假设汽车从第 i 位出发，想要遍历环形的公路，必须要满足两个条件：

1. 从第 i 位到第n位，我们对两个数组对应位的差值求和（这个结果其实是当前汽车的油量），这个值不能小于0

2. 到第n位之后，汽车所保存的油量，加上从第0位开始，到第 i 位的差值之和，大于0

第一条很好理解，第二条有些绕。你可能会问，第n位之后，汽车以油箱那时候的存量，从第0位开始走，即便从第0位到第 i 位的差值和大于0，就一定不会中间走不下去吗，这显然不一定。

但是题目有个前提，就是答案唯一。第n位之后，汽车油箱的存量，加上第0位开始，到第 i 位的差值之和，这个值如果大于0，则一定存在一个点，从这里开始，能完成行驶任务，而这个点就是第 i 位。

看代码：



```python
class Solution:
    # @param gas, a list of integers
    # @param cost, a list of integers
    # @return an integer
    def canCompleteCircuit(self, gas, cost):
        n = len(gas)
        sum1, sum2,  = 0, 0
        cur, index = 0, 0
        while cur != n:
            diff = gas[cur] - cost[cur]
            sum1 += diff
            sum2 += diff
            if sum2 < 0:
                sum2 = 0
                index = cur + 1
            cur += 1
        return index if sum1 >= 0 else -1    
        # write your code here
```
sum1求取的是差值总和，sum2是在判断那个点能开始行驶。你看sum2的求法和求最大子数组（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51383459)）非常相似，所以，index就是唯一可能成为答案的那个位置






