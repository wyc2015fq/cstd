# [LeetCode] My Calendar I 我的日历之一 - Grandyang - 博客园







# [[LeetCode] My Calendar I 我的日历之一](https://www.cnblogs.com/grandyang/p/7920253.html)








Implement a `MyCalendar` class to store your events. A new event can be added if adding the event will not cause a double booking.

Your class will have the method, `book(int start, int end)`. Formally, this represents a booking on the half open interval `[start, end)`, the range of real numbers `x` such that `start <= x < end`.

A *double booking* happens when two events have some non-empty intersection (ie., there is some time that is common to both events.)

For each call to the method `MyCalendar.book`, return `true` if the event can be added to the calendar successfully without causing a double booking. Otherwise, return `false` and do not add the event to the calendar.

Your class will be called like this: `MyCalendar cal = new MyCalendar();``MyCalendar.book(start, end)`
Example 1:
MyCalendar();
MyCalendar.book(10, 20); // returns true
MyCalendar.book(15, 25); // returns false
MyCalendar.book(20, 30); // returns true
Explanation: 
The first event can be booked.  The second can't because time 15 is already booked by another event.
The third event can be booked, as the first event takes every time less than 20, but not including 20.



Note:
- The number of calls to `MyCalendar.book` per test case will be at most `1000`.
- In calls to `MyCalendar.book(start, end)`, `start` and `end` are integers in the range `[0, 10^9]`.



这道题让我们设计一个我的日历类，里面有一个book函数，需要给定一个起始时间和结束时间，与Google Calendar不同的是，我们的事件事件上不能重叠，实际上这道题的本质就是检查区间是否重叠。那么我们可以暴力搜索，对于每一个将要加入的区间，我们都和已经已经存在的区间进行比较，看是否有重复。而新加入的区间和当前区间产生重复的情况有两种，一种是新加入区间的前半段重复，并且，另一种是新加入区间的后半段重复。比如当前区间如果是[3, 8)，那么第一种情况下新加入区间就是[6, 9)，那么触发条件就是当前区间的起始时间小于等于新加入区间的起始时间，并且结束时间大于新加入区间的结束时间。第二种情况下新加入区间就是[2,5)，那么触发条件就是当前区间的起始时间大于等于新加入区间的起始时间，并且起始时间小于新加入区间的结束时间。这两种情况均返回false，否则就将新区间加入数组，并返回true即可，参见代码如下：



解法一：

```
class MyCalendar {
public:
    MyCalendar() {}
    
    bool book(int start, int end) {
        for (auto a : cal) {
            if (a.first <= start && a.second > start) return false;
            if (a.first >= start && a.first < end) return false;
        }
        cal.push_back({start, end});
        return true;
    }

private:
    vector<pair<int, int>> cal;
};
```



下面这种方法将上面方法的两个if判断融合成为了一个，我们来观察两个区间的起始和结束位置的关系发现，如果两个区间的起始时间中的较大值小于结束区间的较小值，那么就有重合，返回false。比如 [3, 8) 和 [6, 9)，3和6中的较大值6，小于8和9中的较小值8，有重叠。再比如[3, 8) 和 [2, 5)，3和2中的较大值3，就小于8和5中的较小值5，有重叠。而对于[3, 8) 和 [9, 10)，3和9中的较大值9，不小于8和10中的较小值8，所以没有重叠，参见代码如下：



解法二：

```
class MyCalendar {
public:
    MyCalendar() {}
    
    bool book(int start, int end) {
        for (auto a : cal) {
            if (max(a.first, start) < min(a.second, end)) return false;
        }
        cal.push_back({start, end});
        return true;
    }

private:
    vector<pair<int, int>> cal;
};
```



上面两种解法都是线性搜索，我们起始可以优化搜索时间，如果我们的区间是有序的话。所以我们用一个map来建立起始时间和结束时间的映射，map会按照起始时间进行自动排序。然后对于新进来的区间，我们在已有区间中查找第一个不小于新入区间的起始时间的区间，如果这个区间存在的话，说明新入区间的起始时间小于等于当前区间，也就是解法一中的第二个if情况，当前区间起始时间小于新入区间结束时间的话返回false。我们还要跟前面一个区间进行查重叠操作，那么判断如果当前区间不是第一个区间的话，就找到前一个区间，此时是解法一中第一个if情况，并且如果前一个区间的结束时间大于新入区间的起始时间的话，返回false。否则就建立新的映射，返回true即可，参见代码如下：



解法三：

```
class MyCalendar {
public:
    MyCalendar() {}
    
    bool book(int start, int end) {
        auto it = cal.lower_bound(start);
        if (it != cal.end() && it->first < end) return false;
        if (it != cal.begin() && prev(it)->second > start) return false;
        cal[start] = end;
        return true;
    }

private:
    map<int, int> cal;
};
```



参考资料：

[https://discuss.leetcode.com/topic/111205/java-8-liner-treemap](https://discuss.leetcode.com/topic/111205/java-8-liner-treemap)

[https://discuss.leetcode.com/topic/111244/simple-c-o-n-solution](https://discuss.leetcode.com/topic/111244/simple-c-o-n-solution)

[https://discuss.leetcode.com/topic/111306/clean-c-o-logn-solution](https://discuss.leetcode.com/topic/111306/clean-c-o-logn-solution)


[https://discuss.leetcode.com/topic/111194/java-c-clean-code-with-explanation](https://discuss.leetcode.com/topic/111194/java-c-clean-code-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












