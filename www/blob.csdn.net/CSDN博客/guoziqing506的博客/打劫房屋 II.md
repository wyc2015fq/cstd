# 打劫房屋 II - guoziqing506的博客 - CSDN博客





2016年06月16日 15:32:23[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：773
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)








题目描述：在上次打劫完一条街道之后，窃贼又发现了一个新的可以打劫的地方，但这次所有的房子围成了一个圈，这就意味着第一间房子和最后一间房子是挨着的。每个房子都存放着特定金额的钱。你面临的唯一约束条件是：相邻的房子装着相互联系的防盗系统，且 当相邻的两个房子同一天被打劫时，该系统会自动报警。给定一个非负整数列表，表示每个房子中存放的钱， 算一算，如果今晚去打劫，你最多可以得到多少钱 在不触动报警装置的情况下。


样例：给出nums = [3,6,4], 返回　6，　你不能打劫3和4所在的房间，因为它们围成一个圈，是相邻的．




这是上一道题“打劫房屋”（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51690021)）的延伸。所以如果你上一道题没搞懂，应该先去看上一题。看明白了再来做这道题。很多上一题的东西我在这里会略过不讲。

和上一题唯一的不同在于现在房屋的排列变成了一个环。因此，我们设计算法的时候也就多了一个需要考虑的因素，那就是打劫前 n 个房屋的最大收益与打劫前 n - 1个房屋取得最大收益的方案中是否打劫了第1个房屋有直接关系。我们还是用一个列表record记录结果，record[i]表示打劫前 i 个房屋的最大收益

试想，如果我们规定不能打劫第1个房屋，那么，这时就跟上一题没存在环的情况是一样的，所以我们用上一题的算法计算从第2个元素开始到最后一个元素的最大收益就行（从第2个开始是因为第1个不打劫，忽略）；而如果规定必须打劫第1个房屋，那么，最后一个房屋肯定不能被打劫（有环），因此，这时候用上一题的算法计算从第1个元素开始到倒数第二个房屋构成的序列，就可以得到在打劫第1个房屋的前提下，获得的最大收益，两个值一比较，取最大值。




代码如下：



```python
class Solution:
    # @param nums: A list of non-negative integers.
    # return: an integer
    def houseRobber2(self, nums):
        if len(nums) == 0:
            return 0
        if len(nums) == 1:
            return nums[0]
        # begin1, end1表示必须打劫第1个房屋的情况
        # begin2, end2表示不能打劫第1个房屋的情况
        begin1, begin2 = 0, 1
        end1, end2 = len(nums) - 2, len(nums) - 1

        return max(self.helper(nums, begin1, end1), self.helper(nums, begin2, end2))

    def helper(self, nums, begin, end):
        n = end - begin + 1
        if n == 0:
            return 0
        if n == 1:
            return nums[begin]
        record = [0 for i in range(n)]
        record[0] = nums[begin]
        record[1] = max(nums[begin], nums[begin + 1])
        cur = 2
        while cur < n:
            record[cur] = max(record[cur - 1], record[cur - 2] + nums[begin + cur])
            cur += 1
        return record[n - 1]
        # write your code here
```



我用了一个helper函数实现上一题中的算法（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51690021)），只不过加了两个参数begin和end，表示参加计算的数组的开始和结束部分。



