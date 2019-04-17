# [LeetCode] Merge Intervals 合并区间 - Grandyang - 博客园







# [[LeetCode] Merge Intervals 合并区间](https://www.cnblogs.com/grandyang/p/4370601.html)







Given a collection of intervals, merge all overlapping intervals.

 For example,

Given `[1,3],[2,6],[8,10],[15,18]`,

return `[1,6],[8,10],[15,18]`.




这道和之前那道 [Insert Interval](http://www.cnblogs.com/grandyang/p/4367569.html) 很类似，这次题目要求我们合并区间，之前那题明确了输入区间集是有序的，而这题没有，所以我们首先要做的就是给区间集排序，由于我们要排序的是个结构体，所以我们要定义自己的comparator，才能用sort来排序，我们以start的值从小到大来排序，排完序我们就可以开始合并了，首先把第一个区间存入结果中，然后从第二个开始遍历区间集，如果结果中最后一个区间和遍历的当前区间无重叠，直接将当前区间存入结果中，如果有重叠，将结果中最后一个区间的end值更新为结果中最后一个区间的end和当前end值之中的较大值，然后继续遍历区间集，以此类推可以得到最终结果，代码如下：



解法一:

```
class Solution {
public:
    vector<Interval> merge(vector<Interval>& intervals) {
        if (intervals.empty()) return {};
        sort(intervals.begin(), intervals.end(), [](Interval &a, Interval &b) {return a.start < b.start;});
        vector<Interval> res{intervals[0]};
        for (int i = 1; i < intervals.size(); ++i) {
            if (res.back().end < intervals[i].start) {
                res.push_back(intervals[i]);
            } else {
                res.back().end = max(res.back().end, intervals[i].end);
            }
        }   
        return res;
    }
};
```



下面这种解法将起始位置和结束位置分别存到了两个不同的数组starts和ends中，然后分别进行排序，之后用两个指针i和j，初始化时分别指向starts和ends数组的首位置，然后如果i指向starts数组中的最后一个位置，或者当starts数组上i+1位置上的数字大于ends数组的i位置上的数时，此时说明区间已经不连续了，我们来看题目中的例子，排序后的starts和ends为：

starts:    1    2    8    15

ends:     36    10    18

红色为i的位置，蓝色为j的位置，那么此时starts[i+1]为8，ends[i]为6，8大于6，所以此时不连续了，将区间[starts[j], ends[i]]，即 [1, 6] 加入结果res中，然后j赋值为i+1继续循环，参见代码如下：



解法二：

```
class Solution {
public:
    vector<Interval> merge(vector<Interval>& intervals) {
        int n = intervals.size();
        vector<Interval> res;
        vector<int> starts, ends;
        for (int i = 0; i < n; ++i) {
            starts.push_back(intervals[i].start);
            ends.push_back(intervals[i].end);
        }
        sort(starts.begin(), starts.end());
        sort(ends.begin(), ends.end());
        for (int i = 0, j = 0; i < n; ++i) {
            if (i == n - 1 || starts[i + 1] > ends[i]) {
                res.push_back(Interval(starts[j], ends[i]));
                j = i + 1;
            }
        } 
        return res;
    }
};
```



这道题还有另一种解法，这个解法直接调用了之前那道题[Insert Interval](http://www.cnblogs.com/grandyang/p/4367569.html)的函数，由于插入的过程中也有合并的操作，所以我们可以建立一个空的集合，然后把区间集的每一个区间当做一个新的区间插入结果中，也可以得到合并后的结果，那道题中的四种解法都可以在这里使用，但是没必要都列出来，这里只选了那道题中的解法二放到这里，代码如下：



解法三：

```
class Solution {
public:
    vector<Interval> merge(vector<Interval>& intervals) {
        vector<Interval> res;
        for (int i = 0; i < intervals.size(); ++i) {
            res = insert(res, intervals[i]);
        }
        return res;
    }
    vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
        vector<Interval> res;
        int n = intervals.size(), cur = 0;
        for (int i = 0; i < n; ++i) {
            if (intervals[i].end < newInterval.start) {
                res.push_back(intervals[i]);
                ++cur;
            } else if (intervals[i].start > newInterval.end) {
                res.push_back(intervals[i]);
            } else {
                newInterval.start = min(newInterval.start, intervals[i].start);
                newInterval.end = max(newInterval.end, intervals[i].end);
            }
        }
        res.insert(res.begin() + cur, newInterval);
        return res;
    }
};
```



类似题目：

[Employee Free Time](http://www.cnblogs.com/grandyang/p/8552586.html)

[Insert Interval](http://www.cnblogs.com/grandyang/p/4367569.html)



参考资料：

[https://leetcode.com/problems/merge-intervals/](https://leetcode.com/problems/merge-intervals/)

[https://leetcode.com/problems/merge-intervals/discuss/21242/C++-10-line-solution.-easing-understanding](https://leetcode.com/problems/merge-intervals/discuss/21242/C++-10-line-solution.-easing-understanding)

[https://leetcode.com/problems/merge-intervals/discuss/21223/Beat-98-Java.-Sort-start-and-end-respectively](https://leetcode.com/problems/merge-intervals/discuss/21223/Beat-98-Java.-Sort-start-and-end-respectively.)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












