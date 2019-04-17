# Wiggle Sort - guoziqing506的博客 - CSDN博客





2016年04月22日 16:17:34[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：544
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给出一个无序的数组，在原地将数组排列成符合以下规律：nums[0] <= nums[1] >= nums[2] <= nums[3]....

样例：Given nums = [3, 5, 2, 1, 6, 4], one possible answer is [1, 6, 2, 5, 3, 4].




题目Wiggle Sort中的Wiggle是“扭动，摇摆”的意思。你看这个规律就能发现，排好序的数组应该是一个波浪式的前进。

那么，现在先不着急解决这个问题，你先看看这样的一个波浪数组有什么规律呢，那就是所有index为偶数的元素都小于他的前（后）一个元素，同时，所有index为奇数的元素都大于他的前（后）一个元素，所以，既然题目还要求了是原地排序，那么不妨回忆一下之前最简单的那个“冒泡排序”的过程（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/50949698)），是通过扫描数组，同时根据大小关系对相邻元素交换。

这里呢，也可以尝试用类似的方法：

1. 如果index为偶数的元素大于他的前（后）一个元素了，就与前（后）一个元素交换

2. 如果index为奇数的元素小于他的前（后）一个元素了，就与前（后）一个元素交换


而且，我们观察到，这样交换是有好处的，比如，a[0] <= a[1] <= a[2]，按照刚才的策略，需要交换a[1]和a[2]，因为a[2] >= a[1]，所以交换之后，变成a[0] <= a[2] >= a[1]，并不影响a[0]和a[2]之间的顺序，因此，不需要向“冒泡排序”那样，多次扫描。

那么代码就比较简单了：



```python
class Solution(object):
    """
    @param {int[]} nums a list of integer
    @return nothing, modify nums in-place instead
    """
    def wiggleSort(self, nums):
        n = len(nums)
        if n == 0:
            return
        for i in range(1, n):
            if (i % 2 != 0 and nums[i] < nums[i - 1]) or (i % 2 == 0 and nums[i] > nums[i - 1]):
                temp = nums[i - 1]
                nums[i - 1] = nums[i]
                nums[i] = temp

        # for i in range(n - 1):
        #     if (i % 2 != 0 and nums[i] < nums[i + 1]) or (i % 2 == 0 and nums[i] > nums[i + 1]):
        #         temp = nums[i + 1]
        #         nums[i + 1] = nums[i]
        #         nums[i] = temp

        # Write your code here
```
注释部分的代码是说，也能写成和后一个元素比较的方法。

这道题关键在于对排序规律的观察





