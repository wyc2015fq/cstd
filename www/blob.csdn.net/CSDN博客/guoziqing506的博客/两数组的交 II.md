# 两数组的交 II - guoziqing506的博客 - CSDN博客





2016年06月02日 19:21:20[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1789
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：计算两个数组的交

样例：nums1 = [1, 2, 2, 1], nums2 = [2, 2], 返回 [2, 2].

跟上一道题一样（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51565291)），都是计算两个数组的交集。不同的是，这道题求得交集不用去重（也就是有多少重复的，就把他们全部找到）。

上一道题我们一共用了三种方法解决问题，其实这道题方法也不止一种，不过我不一个个列举了。只提供一种解法：排序+两根指针（也就是对上一道题的第三种方法稍微做一点修改）

思路是这样：先把两个数组排序，然后用两根指针从前往后扫描两个数组，若元素相同，则将这个元素记录在结果列表中；若元素不同，则将小的那一个的指针后移；直到其中有一个数组遍历完毕。

代码如下：



```python
class Solution:
    # @param {int[]} nums1 an integer array
    # @param {int[]} nums2 an integer array
    # @return {int[]} an integer array
    def intersection(self, nums1, nums2):
        result = []
        nums1.sort()
        nums2.sort()
        index1, index2 = 0, 0
        n1, n2 = len(nums1), len(nums2)
        while index1 < n1 and index2 < n2:
            if nums1[index1] == nums2[index2]:
                result.append(nums1[index1])
                index1 += 1
                index2 += 1
            elif nums1[index1] > nums2[index2]:
                index2 += 1
            else:
                index1 += 1
        return result
        # Write your code here
```
代码的细节我之前（上一道题中）已经讲过，此处不再啰嗦。不明白的话，请回看给出的链接（[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51565291)）




