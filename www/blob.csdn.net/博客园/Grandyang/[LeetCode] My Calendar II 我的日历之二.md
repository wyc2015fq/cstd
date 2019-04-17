# [LeetCode] My Calendar II 我的日历之二 - Grandyang - 博客园







# [[LeetCode] My Calendar II 我的日历之二](https://www.cnblogs.com/grandyang/p/7968035.html)







Implement a `MyCalendarTwo` class to store your events. A new event can be added if adding the event will not cause a triple booking.

Your class will have one method, `book(int start, int end)`. Formally, this represents a booking on the half open interval `[start, end)`, the range of real numbers `x` such that `start <= x < end`.

A *triple booking* happens when three events have some non-empty intersection (ie., there is some time that is common to all 3 events.)

For each call to the method `MyCalendar.book`, return `true` if the event can be added to the calendar successfully without causing a triple booking. Otherwise, return `false` and do not add the event to the calendar.

Your class will be called like this: `MyCalendar cal = new MyCalendar();``MyCalendar.book(start, end)`

Example 1:
MyCalendar();
MyCalendar.book(10, 20); // returns true
MyCalendar.book(50, 60); // returns true
MyCalendar.book(10, 40); // returns true
MyCalendar.book(5, 15); // returns false
MyCalendar.book(5, 10); // returns true
MyCalendar.book(25, 55); // returns true
Explanation: 
The first two events can be booked.  The third event can be double booked.
The fourth event (5, 15) can't be booked, because it would result in a triple booking.
The fifth event (5, 10) can be booked, as it does not use time 10 which is already double booked.
The sixth event (25, 55) can be booked, as the time in [25, 40) will be double booked with the third event;
the time [40, 50) will be single booked, and the time [50, 55) will be double booked with the second event.



Note:
- The number of calls to `MyCalendar.book` per test case will be at most `1000`.
- In calls to `MyCalendar.book(start, end)`, `start` and `end` are integers in the range `[0, 10^9]`.



这道题是[My Calendar I](http://www.cnblogs.com/grandyang/p/7920253.html)的拓展，之前那道题说是不能有任何的重叠区间，而这道题说最多容忍两个重叠区域，注意是重叠区域，不是事件。比如事件A，B，C互不重叠，但是有一个事件D，和这三个事件都重叠，这样是可以的，因为重叠的区域最多只有两个。所以关键还是要知道具体的重叠区域，如果两个事件重叠，那么重叠区域就是它们的交集，求交集的方法是两个区间的起始时间中的较大值，到结束时间中的较小值。那么我们可以用一个集合来专门存重叠区间，再用一个集合来存完整的区间，那么我们的思路就是，先遍历专门存重叠区间的集合，因为能在这里出现的区间，都已经是出现两次了，如果当前新的区间跟重叠区间有交集的话，说明此时三个事件重叠了，直接返回false。如果当前区间跟重叠区间没有交集的话，那么再来遍历完整区间的集合，如果有交集的话，那么应该算出重叠区间并且加入放重叠区间的集合中。最后记得将新区间加入完整区间的集合中，参见代码如下：



解法一：

```
class MyCalendarTwo {
public:
    MyCalendarTwo() {}
    
    bool book(int start, int end) {
        for (auto a : s2) {
            if (start >= a.second || end <= a.first) continue;
            else return false;
        }
        for (auto a : s1) {
            if (start >= a.second || end <= a.first) continue;
            else s2.insert({max(start, a.first), min(end, a.second)});
        }
        s1.insert({start, end});
        return true;
    }

private:
    set<pair<int, int>> s1, s2;
};
```



下面这种方法相当的巧妙，我们建立一个时间点和次数之间的映射，规定遇到起始时间点，次数加1，遇到结束时间点，次数减1。那么我们首先更改新的起始时间start和结束时间end的映射，start对应值增1，end对应值减1。然后定义一个变量cnt，来统计当前的次数。我们使用treemap具有自动排序的功能，所以我们遍历的时候就是按时间顺序的，最先遍历到的一定是一个起始时间，所以加上其映射值，一定是个正数。那么我们想，如果此时只有一个区间，就是刚加进来的区间的话，那么首先肯定遍历到start，那么cnt此时加1，然后就会遍历到end，那么此时cnt减1，最后下来cnt为0，没有重叠。还是用具体数字来说吧，我们现在假设treemap中已经加入了一个区间[3, 5)了，那么我们就有下面的映射：

3 -> 1

5 -> -1

假如我们此时要加入的区间为[6, 8)的话，那么在遍历到6的时候，前面经过3和5，分别加1减1，那么cnt又重置为0了，而后面的6和8也是分别加1减1，还是0。那么加入我们新加入的区间为[3, 8]时，那么此时的映射为：

3 -> 2

5 -> -1

8 -> -1

那么我们最先遍历到3，cnt为2，没有超过3，我们知道此时有两个事件有重叠，是允许的。然后遍历5和8，分别减去1，最终又变成0了，始终cnt没有超过2，所以是符合题意的。如果此时我们再加入一个新的区间[1, 4)，那么此时的映射为：

1 -> 1

3 -> 2

4 -> -1

5 -> -1

8 -> -1

那么我们先遍历到1，cnt为1，然后遍历到3，此时cnt为3了，那么我们就知道有三个事件有重叠区间了，所以这个新区间是不能加入的，那么我们要还原其start和end做的操作，把start的映射值减1，end的映射值加1，然后返回false。否则没有三个事件有共同重叠区间的话，返回true即可，参见代码如下：



解法二：

```
class MyCalendarTwo {
public:
    MyCalendarTwo() {}
    
    bool book(int start, int end) {
        ++freq[start];
        --freq[end];
        int cnt = 0;
        for (auto f : freq) {
            cnt += f.second;
            if (cnt == 3) {
                --freq[start];
                ++freq[end];
                return false;
            }
        }
        return true;
    }

private:
    map<int, int> freq;
};
```



类似题目：

[My Calendar I](http://www.cnblogs.com/grandyang/p/7920253.html)



参考资料：

[https://discuss.leetcode.com/topic/111276/simplified-winner-s-solution](https://discuss.leetcode.com/topic/111276/simplified-winner-s-solution)

[https://discuss.leetcode.com/topic/111279/c-solution-easy-to-understand](https://discuss.leetcode.com/topic/111279/c-solution-easy-to-understand)

[https://discuss.leetcode.com/topic/111198/java-c-clean-code-with-explanation](https://discuss.leetcode.com/topic/111198/java-c-clean-code-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












