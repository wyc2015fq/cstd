# [LeetCode] Insert Interval 插入区间 - Grandyang - 博客园







# [[LeetCode] Insert Interval 插入区间](https://www.cnblogs.com/grandyang/p/4367569.html)







Given a set of *non-overlapping* intervals, insert a new interval into the intervals (merge if necessary).

You may assume that the intervals were initially sorted according to their start times.

**Example 1:**

Given intervals `[1,3],[6,9]`, insert and merge `[2,5]` in as `[1,5],[6,9]`.


**Example 2:**

Given `[1,2],[3,5],[6,7],[8,10],[12,16]`, insert and merge `[4,9]` in as `[1,2],[3,10],[12,16]`.



This is because the new interval `[4,9]` overlaps with `[3,5],[6,7],[8,10]`.




这道题让我们在一系列非重叠的区间中插入一个新的区间，可能还需要和原有的区间合并，那么我们需要对给区间集一个一个的遍历比较，那么会有两种情况，重叠或是不重叠，不重叠的情况最好，直接将新区间插入到对应的位置即可，重叠的情况比较复杂，有时候会有多个重叠，我们需要更新新区间的范围以便包含所有重叠，之后将新区间加入结果res，最后将后面的区间再加入结果res即可。具体思路是，我们用一个变量cur来遍历区间，如果当前cur区间的结束位置小于要插入的区间的起始位置的话，说明没有重叠，则将cur区间加入结果res中，然后cur自增1。直到有cur越界或有重叠while循环退出，然后再用一个while循环处理所有重叠的区间，每次用取两个区间起始位置的较小值，和结束位置的较大值来更新要插入的区间，然后cur自增1。直到cur越界或者没有重叠时while循环退出。之后将更新好的新区间加入结果res，然后将cur之后的区间再加入结果res中即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
        vector<Interval> res;
        int n = intervals.size(), cur = 0;
        while (cur < n && intervals[cur].end < newInterval.start) {
            res.push_back(intervals[cur++]);
        }
        while (cur < n && intervals[cur].start <= newInterval.end) {
            newInterval.start = min(newInterval.start, intervals[cur].start);
            newInterval.end = max(newInterval.end, intervals[cur].end);
            ++cur;
        }
        res.push_back(newInterval);
        while (cur < n) {
            res.push_back(intervals[cur++]);
        }
        return res;
    }
};
```



下面这种方法的思路跟上面的解法很像，只不过没有用while循环，而是使用的是for循环，但是思路上没有太大的区别，变量cur还是用来记录新区间该插入的位置，稍有不同的地方在于在for循环中已经将新区间后面不重叠的区间也加进去了，for循环结束后就只需要插入新区间即可，参见代码如下：



解法二：

```
class Solution {
public:
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



下面这种解法就是把上面解法的for循环改为了while循环，其他的都没有变，代码如下：



解法三：

```
class Solution {
public:
    vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
        vector<Interval> res;
        int n = intervals.size(), cur = 0, i = 0;
        while (i < n) {
            if (intervals[i].end < newInterval.start) {
                res.push_back(intervals[i]);
                ++cur;
            } else if (intervals[i].start > newInterval.end) {
                res.push_back(intervals[i]);
            } else {
                newInterval.start = min(newInterval.start, intervals[i].start);
                newInterval.end = max(newInterval.end, intervals[i].end);
            }
            ++i;
        }
        res.insert(res.begin() + cur, newInterval);
        return res;
    }
};
```



如果学过Design Pattern的，对Iterator Pattern比较熟悉的也可应用Iterator来求解，本质还是一样的，只是写法略有不同，代码如下：



解法四：

```
class Solution {
public:
    vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
        vector<Interval> res;
        vector<Interval>::iterator it = intervals.begin();
        int cur = 0;
        while (it != intervals.end()) {
            if (it->end < newInterval.start) {
                res.push_back(*it);
                ++cur;
            } else if (it->start > newInterval.end) {
                res.push_back(*it);
            } else {
                newInterval.start = min(newInterval.start, it->start);
                newInterval.end = max(newInterval.end, it->end);
            }
            ++it;
        }
        res.insert(res.begin() + cur, newInterval);
        return res;
    }
};
```



类似题目：

[Range Module](http://www.cnblogs.com/grandyang/p/8586531.html)

[Merge Intervals](http://www.cnblogs.com/grandyang/p/4370601.html)



参考资料：

[https://leetcode.com/problems/insert-interval/discuss/21669/Easy-and-clean-O(n)-C++-solution](https://leetcode.com/problems/insert-interval/discuss/21669/Easy-and-clean-O(n)-C++-solution)

[https://leetcode.com/problems/insert-interval/discuss/21602/Short-and-straight-forward-Java-solution](https://leetcode.com/problems/insert-interval/discuss/21602/Short-and-straight-forward-Java-solution)

[https://leetcode.com/problems/insert-interval/discuss/21676/Clean-and-short-Java-solution-with-explanation](https://leetcode.com/problems/insert-interval/discuss/21676/Clean-and-short-Java-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












