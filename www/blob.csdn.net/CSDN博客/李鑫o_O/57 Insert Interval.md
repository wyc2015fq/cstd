# 57. Insert Interval - 李鑫o_O - CSDN博客





2016年03月30日 22:07:00[hustlx](https://me.csdn.net/HUSTLX)阅读数：253











iven a set of non-overlapping intervals, insert a new interval into the intervals (merge if necessary).


You may assume that the intervals were initially sorted according to their start times.

Example 1:

Given intervals `[1,3],[6,9]`, insert and merge `[2,5]` in
 as `[1,5],[6,9]`.

Example 2:

Given `[1,2],[3,5],[6,7],[8,10],[12,16]`, insert and merge `[4,9]` in
 as `[1,2],[3,10],[12,16]`.


This is because the new interval `[4,9]` overlaps with `[3,5],[6,7],[8,10]`.







```cpp
vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
    vector<Interval> res;
    int i = 0;
    int n = intervals.size();
    while (i < n&&newInterval.start > intervals[i].end)  res.push_back(intervals[i++]);
    newInterval.start = ((i == n) ? newInterval.start : min(newInterval.start, intervals[i].start));
    while (i < n&&newInterval.end >= intervals[i].start) i++;
    newInterval.end = ((i == 0) ? newInterval.end:max(intervals[i - 1].end, newInterval.end));
    res.push_back(newInterval);
    while (i < n&&newInterval.end < intervals[i].start) res.push_back(intervals[i++]);
    return res;
}
```





