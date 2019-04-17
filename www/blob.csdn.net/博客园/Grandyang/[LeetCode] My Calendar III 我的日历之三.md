# [LeetCode] My Calendar III 我的日历之三 - Grandyang - 博客园







# [[LeetCode] My Calendar III 我的日历之三](https://www.cnblogs.com/grandyang/p/8005054.html)








Implement a `MyCalendarThree` class to store your events. A new event can always be added.

Your class will have one method, `book(int start, int end)`. Formally, this represents a booking on the half open interval `[start, end)`, the range of real numbers `x` such that `start <= x < end`.

A *K-booking* happens when K events have some non-empty intersection (ie., there is some time that is common to all K events.)

For each call to the method `MyCalendar.book`, return an integer `K` representing the largest integer such that there exists a `K`-booking in the calendar.

Your class will be called like this: `MyCalendarThree cal = new MyCalendarThree();``MyCalendarThree.book(start, end)`
Example 1:
MyCalendarThree();
MyCalendarThree.book(10, 20); // returns 1
MyCalendarThree.book(50, 60); // returns 1
MyCalendarThree.book(10, 40); // returns 2
MyCalendarThree.book(5, 15); // returns 3
MyCalendarThree.book(5, 10); // returns 3
MyCalendarThree.book(25, 55); // returns 3
Explanation: 
The first two events can be booked and are disjoint, so the maximum K-booking is a 1-booking.
The third event [10, 40) intersects the first event, and the maximum K-booking is a 2-booking.
The remaining events cause the maximum K-booking to be only a 3-booking.
Note that the last event locally causes a 2-booking, but the answer is still 3 because
eg. [10, 20), [10, 40), and [5, 15) are still triple booked.



Note:
- The number of calls to `MyCalendarThree.book` per test case will be at most `400`.
- In calls to `MyCalendarThree.book(start, end)`, `start` and `end` are integers in the range `[0, 10^9]`.



这道题是之前那两道题[My Calendar II](http://www.cnblogs.com/grandyang/p/7968035.html)，[My Calendar I](http://www.cnblogs.com/grandyang/p/7920253.html)的拓展，论坛上有人说这题不应该算是Hard类的，但实际上如果没有之前那两道题做铺垫，直接上这道其实还是还蛮有难度的。这道题博主在做完之前那道，再做这道一下子就做出来了，因为用的就是之前那道[My Calendar II](http://www.cnblogs.com/grandyang/p/7968035.html)的解法二，具体的讲解可以参见那道题，反正博主写完那道题再来做这道题就是秒解啊，参见代码如下：



```
class MyCalendarThree {
public:
    MyCalendarThree() {}
    
    int book(int start, int end) {
        ++freq[start];
        --freq[end];
        int cnt = 0, mx = 0;
        for (auto f : freq) {
            cnt += f.second;
            mx = max(mx, cnt);
        }
        return mx;
    }
    
private:
    map<int, int> freq;
};
```



类似题目：

[My Calendar II](http://www.cnblogs.com/grandyang/p/7968035.html)

[My Calendar I](http://www.cnblogs.com/grandyang/p/7920253.html)




参考资料：

[https://discuss.leetcode.com/topic/111978/java-c-clean-code](https://discuss.leetcode.com/topic/111978/java-c-clean-code)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












