# 56. Merge Intervals - 李鑫o_O - CSDN博客





2016年03月30日 20:52:20[hustlx](https://me.csdn.net/HUSTLX)阅读数：295









Given a collection of intervals, merge all overlapping intervals.


For example,

Given `[1,3],[2,6],[8,10],[15,18]`,


return `[1,6],[8,10],[15,18]`.



```cpp
vector<Interval> merge(vector<Interval>& intervals) {
    vector<Interval> res;
    int n = intervals.size();
    if (n == 0) return res;
    sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {return a.start < b.start; });
    Interval temp = intervals[0];
    for (int i = 1; i < n; i++) {
        if (intervals[i].start <= temp.end) {
            temp.end = max(temp.end, intervals[i].end);
        }
        else {
            res.push_back(temp);
            temp = intervals[i];
        }
    }
    res.push_back(temp);
    return res;
}
```





