# leetcode-56. Merge Intervals 合并区间 - 别说话写代码的博客 - CSDN博客





2019年01月11日 22:18:53[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：26标签：[合并区间																[Merge Intervals																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Merge Intervals&t=blog)



|Given a collection of intervals, merge all overlapping intervals.**Example 1:****Input:** [[1,3],[2,6],[8,10],[15,18]]**Output:** [[1,6],[8,10],[15,18]]**Explanation:** Since intervals [1,3] and [2,6] overlaps, merge them into [1,6].**Example 2:****Input:** [[1,4],[4,5]]**Output:** [[1,5]]**Explanation:** Intervals [1,4] and [4,5] are considered overlapping.|给出一个区间的集合，请合并所有重叠的区间。**示例 1:****输入:** [[1,3],[2,6],[8,10],[15,18]]**输出:** [[1,6],[8,10],[15,18]]**解释:** 区间 [1,3] 和 [2,6] 重叠, 将它们合并为 [1,6].**示例 2:****输入:** [[1,4],[4,5]]**输出:** [[1,5]]**解释:** 区间 [1,4] 和 [4,5] 可被视为重叠区间。|
|----|----|

思路：先对按照区间的左半部分排序，然后数据就变为在左半部分有序并且 右半部分也 有序。这样顺序往后遍历，若遇到未重叠情况，res.back().end<intervals[i].start的情况就将 当前区间对intervals[i]放入res中，否则就说明为重叠，则将res数组中res.back().end置为res最后一个元素end和intervals[i].end 之间的最大值。

```cpp
/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 *     Interval() : start(0), end(0) {}
 *     Interval(int s, int e) : start(s), end(e) {}
 * };
 */
class Solution {
public:
    vector<Interval> merge(vector<Interval>& intervals) {
        if(intervals.empty()) return {};   
        sort(intervals.begin(),intervals.end(),[](Interval &a, Interval &b) {return a.start < b.start;});
        vector<Interval> res{intervals[0]};
        for(int i=1;i<intervals.size();++i)
        {
            if(res.back().end<intervals[i].start)
                res.push_back(intervals[i]);
            else res.back().end = max(res.back().end,intervals[i].end);
        }
        return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=合并区间&t=blog)




