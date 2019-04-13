
# 29. My Calendar I - OraYang的博客 - CSDN博客

2017年12月21日 16:29:40[OraYang](https://me.csdn.net/u010665216)阅读数：433标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 29. My Calendar I
标签（空格分隔）： leetcode array medium
---题目

---Implement a

---MyCalendar

---class to store your events. A new event can be added if adding the event will not cause a double booking.

---Your class will have the method,

---book(int start, int end)

---.  Formally, this represents a booking on the half open interval

---[start, end)

---, the range of real numbers

---x

---such that

---start <= x < end

---.

---A

---double booking

---happens when two events have some non-empty intersection (ie., there is some time that is common to both events.)

---For each call to the method

---MyCalendar.book

---, return

---true

---if the event can be added to the calendar successfully without causing a double booking.  Otherwise, return

---false

---and do not add the event to the calendar.

---Your class will be called like this:

---MyCalendar cal = new MyCalendar();

---MyCalendar.book(start, end)

---Example 1:


---MyCalendar();
MyCalendar.book(10, 20); // returns true
MyCalendar.book(15, 25); // returns false
MyCalendar.book(20, 30); // returns true

---Explanation:

---The first event can be booked.  The second can't because time 15 is already booked by another event.
The third event can be booked, as the first event takes every time less than 20, but not including 20.

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

---本题定义一个数值对数组A存储{start,end},要想发生double booking，必须满足当前的

---max(start,A.start)

---代码

---class

---MyCalendar {

---public

---:

---vector

---<pair<

---int

---,

---int

--->

---> books;
    MyCalendar() {
    }

---bool

---book(

---int

---start,

---int

---end) {

---for

---(

---auto

---item:books)
        {

---if

---(max(item.first,start)<min(item.second,end))

---return

---false

---;
        }
        books.push_back({start,end});

---return

---true

---;
    }
};

---/**
 * Your MyCalendar object will be instantiated and called as such:
 * MyCalendar obj = new MyCalendar();
 * bool param_1 = obj.book(start,end);
 */


