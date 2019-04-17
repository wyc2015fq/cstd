# [LeetCode] Meeting Rooms II 会议室之二 - Grandyang - 博客园







# [[LeetCode] Meeting Rooms II 会议室之二](https://www.cnblogs.com/grandyang/p/5244720.html)







Given an array of meeting time intervals consisting of start and end times `[[s1,e1],[s2,e2],...]` (si < ei), find the minimum number of conference rooms required.

For example,
Given `[[0, 30],[5, 10],[15, 20]]`,
return `2`.



这道题是之前那道[Meeting Rooms](http://www.cnblogs.com/grandyang/p/5240774.html)的拓展，那道题只让我们是否能参加所有的会，也就是看会议之间有没有时间冲突，而这道题让我们求最少需要安排几个会议室，有时间冲突的肯定需要安排在不同的会议室。这道题有好几种解法，我们先来看使用TreeMap来做的，我们遍历时间区间，对于起始时间，映射值自增1，对于结束时间，映射值自减1，然后我们定义结果变量res，和房间数rooms，我们遍历TreeMap，时间从小到大，房间数每次加上映射值，然后更新结果res，遇到起始时间，映射是正数，则房间数会增加，如果一个时间是一个会议的结束时间，也是另一个会议的开始时间，则映射值先减后加仍为0，并不用分配新的房间，而结束时间的映射值为负数更不会增加房间数，利用这种思路我们可以写出代码如下:



解法一：

```
class Solution {
public:
    int minMeetingRooms(vector<Interval>& intervals) {
        map<int, int> m;
        for (auto a : intervals) {
            ++m[a.start];
            --m[a.end];
        }
        int rooms = 0, res = 0;
        for (auto it : m) {
            res = max(res, rooms += it.second);
        }
        return res;
    }
};
```



第二种方法是用两个一维数组来做，分别保存起始时间和结束时间，然后各自排个序，我们定义结果变量res和结束时间指针endpos，然后我们开始遍历，如果当前起始时间小于结束时间指针的时间，则结果自增1，反之结束时间指针自增1，这样我们可以找出重叠的时间段，从而安排新的会议室，参见代码如下：



解法二：

```
class Solution {
public:
    int minMeetingRooms(vector<Interval>& intervals) {
        vector<int> starts, ends;
        int res = 0, endpos = 0;
        for (auto a : intervals) {
            starts.push_back(a.start);
            ends.push_back(a.end);
        }
        sort(starts.begin(), starts.end());
        sort(ends.begin(), ends.end());
        for (int i = 0; i < intervals.size(); ++i) {
            if (starts[i] < ends[endpos]) ++res;
            else ++endpos;
        }
        return res;
    }
};
```



再来一看一种使用最小堆来解题的方法，这种方法先把所有的时间区间按照起始时间排序，然后新建一个最小堆，开始遍历时间区间，如果堆不为空，且首元素小于等于当前区间的起始时间，我们去掉堆中的首元素，把当前区间的结束时间压入堆，由于最小堆是小的在前面，那么假如首元素小于等于起始时间，说明上一个会议已经结束，可以用该会议室开始下一个会议了，所以不用分配新的会议室，遍历完成后堆中元素的个数即为需要的会议室的个数，参见代码如下；



解法三：

```
class Solution {
public:
    int minMeetingRooms(vector<Interval>& intervals) {
        sort(intervals.begin(), intervals.end(), [](const Interval &a, const Interval &b){return a.start < b.start;});
        priority_queue<int, vector<int>, greater<int>> q;
        for (auto a : intervals) {
            if (!q.empty() && q.top() <= a.start) q.pop();
            q.push(a.end);
        }
        return q.size();
    }
};
```



类似题目：

[Meeting Rooms](http://www.cnblogs.com/grandyang/p/5240774.html)



参考资料：

[https://leetcode.com/discuss/50948/c-o-n-log-n-584-ms-3-solutions](https://leetcode.com/discuss/50948/c-o-n-log-n-584-ms-3-solutions)

[https://leetcode.com/discuss/71846/super-easy-java-solution-beats-98-8%25](https://leetcode.com/discuss/71846/super-easy-java-solution-beats-98-8%25)

[https://leetcode.com/discuss/64686/concise-c-solution-with-min_heap-sort-greedy](https://leetcode.com/discuss/64686/concise-c-solution-with-min_heap-sort-greedy)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












