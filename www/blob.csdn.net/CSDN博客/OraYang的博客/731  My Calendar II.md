
# 731. My Calendar II - OraYang的博客 - CSDN博客

2017年12月18日 15:28:29[OraYang](https://me.csdn.net/u010665216)阅读数：449所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



---题目

---Implement a

---MyCalendarTwo

---class to store your events. A new event can be added if adding the event will not cause a

---triple

---booking.

---Your class will have one method,

---book(int start, int end)

---.  Formally, this represents a booking on the half open interval

---[start, end)

---, the range of real numbers

---x

---such that

---start <= x < end

---.

---A

---triple booking

---happens when

---three

---events have some non-empty intersection (ie., there is some time that is common to all 3 events.)

---For each call to the method

---MyCalendar.book

---, return

---true

---if the event can be added to the calendar successfully without causing a

---triple

---booking.  Otherwise, return

---false

---and do not add the event to the calendar.

---Your class will be called like this:

---MyCalendar cal = new MyCalendar();

---MyCalendar.book(start, end)

---Example 1:


---MyCalendar();
MyCalendar.book(10, 20); // returns true
MyCalendar.book(50, 60); // returns true
MyCalendar.book(10, 40); // returns true
MyCalendar.book(5, 15); // returns false
MyCalendar.book(5, 10); // returns true
MyCalendar.book(25, 55); // returns true

---Explanation:

---The first two events can be booked.  The third event can be double booked.
The fourth event (5, 15) can't be booked, because it would result in a triple booking.
The fifth event (5, 10) can be booked, as it does not use time 10 which is already double booked.
The sixth event (25, 55) can be booked, as the time in [25, 40) will be double booked with the third event;
the time [40, 50) will be single booked, and the time [50, 55) will be double booked with the second event.

---Note:

---The number of calls to

---MyCalendar.book

---per test case will be at most

---1000

---.

---In calls to

---MyCalendar.book(start, end)

---,

---start

---and

---end

---are integers in the range

---[0, 10^9]

---.


---思路

---这道题目每次调用方法book,一旦发现第三次区间重叠就返回false。这道题目理解起来不困难，但是代码写起来比较繁琐。

---我们先来看下可能前后两次区间可能出现的情况（假定a< b)：一共有三种

---case

---1

---: b ends before a

---ends:

---a:

---a

---0

---|-------------|

---a1

---b:

---b

---0

---|-----|

---b1

---case

---2

---: b ends after a

---ends:

---a:

---a

---0

---|--------|

---a1

---b:

---b

---0

---|--------|

---b1

---case

---3

---: b starts after a

---ends:

---(negative overlap)

---a:

---a

---0

---|----|

---a1

---b:

---b

---0

---|----|

---b1

---通过观察我们发现：

---如果存在

---max(a0, b0) < min(a1, b1)

---那么必定存在重叠区间：

---( max(a0, b0) < min(a1, b1) )

---那么我们实现两个类 MyCalendar II及MyCalendar I。我们先调用MyCalendar II类。我们每进行一次测试先判断是否存在重叠，如果存在则再调用MyCalendar I判断是否第二次重叠。如果发生第二次重叠则返回false,否则返回true。我们需要维护vector数组用来保存每次测试的数据。在MyCalendar II类中我们需要维护vector数组用来存储(start,end),在MyCalendar I中我们需要维护vector数组来保存前面已经发生重叠的重叠区间。

---代码

---class

---MyCalendar {

---vector

---<pair<

---int

---,

---int

--->

---> books;

---public

---:

---bool

---book(

---int

---start,

---int

---end) {

---for

---(pair<

---int

---,

---int

---> p : books)

---if

---(max(p.first, start) < min(end, p.second))

---return

---false

---;
        books.push_back({start, end});

---return

---true

---;
    }
};

---class

---MyCalendarTwo {

---vector

---<pair<

---int

---,

---int

--->

---> books;

---public

---:

---bool

---book(

---int

---start,

---int

---end) {
        MyCalendar overlaps;

---for

---(pair<

---int

---,

---int

---> p : books) {

---if

---(max(p.first, start) < min(end, p.second)) {

---// overlap exist

---pair<

---int

---,

---int

---> overlapped = getOverlap(p.first, p.second, start, end);

---if

---(!overlaps.book(overlapped.first, overlapped.second))

---return

---false

---;

---// overlaps overlapped

---}
        }
        books.push_back({ start, end });

---return

---true

---;
    }
    pair<

---int

---,

---int

---> getOverlap(

---int

---s0,

---int

---e0,

---int

---s1,

---int

---e1) {

---return

---{ max(s0, s1), min(e0, e1)};
    }
};

---/**
 * Your MyCalendarTwo object will be instantiated and called as such:
 * MyCalendarTwo obj = new MyCalendarTwo();
 * bool param_1 = obj.book(start,end);
 */


