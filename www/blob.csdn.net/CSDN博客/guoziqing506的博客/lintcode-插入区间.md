# lintcode - 插入区间 - guoziqing506的博客 - CSDN博客





2018年01月12日 16:02:19[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：269
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









### 题目描述

给出一个无重叠的按照区间起始端点排序的区间列表。在列表中插入一个新的区间，你要确保列表中的区间仍然有序且不重叠（如果有必要的话，可以合并区间）。

### 样例
- 
插入区间[2, 5] 到 [[1,2], [5,9]]，我们得到 [[1,9]]。

- 
插入区间[3, 4] 到 [[1,2], [5,9]]，我们得到 [[1,2], [3,4], [5,9]]。


### 解题思路

这道题所涉及的情况相对有点复杂，因为插入的区间可能和已有的（多个）区间发生合并，且这种合并一定会导致原区间列表中的一些区间被删除。所以解题的关键在于对所有可能出现的情况有一个思路清晰地分析。具体如下：

首先，我们要在原有的区间列表中，找到第一个和插入区间（`newInterval`）有关系的区间（`Interval`）。这里我说的“有关系”指，要么这个区间与插入区间重叠，要么这个区间是紧邻新区间的后一个区间。

具体的做法是以插入区间（`newInterval`）的左边端点（`newInterval.start`）为整个问题的切入点。因为原有区间是按序排列的，所以现在我们依次遍历区间列表，显然对于每个原来区间列表中的元素（`Interval`）来说，新区间的左边端点与它的关系，一定是以下3种情况之一：

(1) 插入区间的左端点在当前区间（即原有区间列表的某个元素）左端点的左侧，如图Fig.1所示。这种情况下，显然当前区间就是第一个和插入区间有关系的区间（因为我们是按照排列顺序依次遍历的）。然后，我们需要进一步判断插入区间右端点（`newInterval.end`）与当前区间的关系，这时又分为两种情况：
- 插入区间右端点在当前区间的左端点的左边，即`newInterval.end < Interval.start`。这说明插入区间与当前区间不相交，如图Fig.1(a)。这时，直接在当前区间前面插入这个区间即可；
- 插入区间右端点在当前区间之中，或者在当前区间的右端点之后，即`newInterval.end >= Interval.start`。这说明插入区间一定会与当前区间（甚至其后的更多区间发生合并），如图Fig.1(b, c)所示。这时需要执行合并操作（具体做法一会再说）；



![](https://img-blog.csdn.net/20180112153346385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VvemlxaW5nNTA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


(2) 插入区间的左端点在当前区间之中，如图Fig.2所示。这种情况下，当前区间就是第一个和新区间有关系的区间。且一定发生合并操作。



![](https://img-blog.csdn.net/20180112154424202?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VvemlxaW5nNTA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


(3) 插入区间的左端点在当前区间右端点的右边，如图Fig.3所示。这种情况下，当前区间不是第一个和新区间有关系的区间。此时不做操作，继续向后遍历原来的区间列表，分析下一个区间，直到找到第一个有关系的区间，再根据上面的两种情况做相应的操作。需要注意的是，存在一种特殊情况，那就是整个区间列表遍历完毕也找不到第一个和插入区间有关系的区间，这就说明，插入的区间左端点的值大于原有区间最后一个元素右端点的值，那么在区间列表后面直接插入（append）新的区间即可。



![](https://img-blog.csdn.net/20180112154924285?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VvemlxaW5nNTA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 代码

按以上3种情况的分析写出代码就可以解决这个问题了。下面重点说一下合并操作。方法是从当前区间（也就是第一个和插入区间有关系的区间）开始，向后遍历区间列表，如果插入区间的右端点大于或等于当前遍历到的区间的左端点（`newInterval.end >= interval.start`），则更新一个值：`second`。second代表合并后的区间的右端点，计算方法是求取当前区间和插入区间右端点的最大值，然后删除当前区间，直到遍历到第一个与插入区间“无关”的区间（即插入区间的右端点小于这个区间的左端点），或者将整个区间列表遍历完。最后插入合并生成的新区间即可。这个新区间的左端点就是第一个和插入区间“有关系”的区间左端点和插入区间左端点的最小值：（`min(newInterval.start, interval.start)`）。插入的位置是这第一个“有关系”区间的位置。

思路很简单，就是逻辑有点绕。多写几遍就好了。代码如下：

```python
class Solution:
    """
    @param: intervals: Sorted interval list.
    @param: newInterval: new interval.
    @return: A new interval list.
    """
    def insert(self, intervals, newInterval):

        index = 0

        if len(intervals) == 0:
            intervals.append(newInterval)
            return intervals

        while index < len(intervals):

            first = min(intervals[index].start, newInterval.start)
            curIndex = index

            # The condition-1
            if newInterval.start < intervals[index].start:

                if newInterval.end < intervals[index].start:
                    intervals.insert(index, newInterval)
                else:
                    self.merge(index, first, curIndex, intervals, newInterval)
                break

            # The condition-2
            elif newInterval.start >= intervals[index].start and newInterval.start <= intervals[index].end:
                self.merge(index, first, curIndex, intervals, newInterval)
                break

            # The condition-3
            else:
                index += 1
                if index == len(intervals):
                    intervals.append(newInterval)
        return intervals

    def merge(self, index, first, curIndex, intervals, newInterval):
        second = 0
        while curIndex < len(intervals):
            temp = intervals[curIndex]

            if temp.start <= newInterval.end:
                second = max(temp.end, newInterval.end)
                intervals.remove(temp)
            else:
                break
        intervals.insert(index, Interval(first, second))
        # write your code here
```

代码中，我写了一个合并函数，为了使代码的思路更加简洁。



