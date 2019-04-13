
# 621. Task Scheduler - OraYang的博客 - CSDN博客

2017年12月26日 13:08:08[OraYang](https://me.csdn.net/u010665216)阅读数：199所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 621. Task Scheduler
标签（空格分隔）： leetcode array medium
---题目

---Given a char array representing tasks CPU need to do. It contains capital letters A to Z where different letters represent different tasks.Tasks could be done without original order. Each task could be done in one interval. For each interval, CPU could finish one task or just be idle.

---However, there is a non-negative cooling interval

---n

---that means between two

---same tasks

---, there must be at least n intervals that CPU are doing different tasks or just be idle.

---You need to return the

---least

---number of intervals the CPU will take to finish all the given tasks.

---Example 1:


---Input:

---tasks = ["A","A","A","B","B","B"], n = 2

---Output:

---8

---Explanation:

---A -> B -> idle -> A -> B -> idle -> A -> B.

---Note:


---The number of tasks is in the range [1, 10000].

---The integer n is in the range [0, 100].


---思路

---本题是一个CPU资源调度题，给定一个字符数组，里面的字符是大写字母”A”-“Z”,一个字符代表一个CPU任务，执行一个任务需要一个始终周期，执行不同的任务可以直接执行，执行相同的任务需要间隔n个时钟周期，这里的n，题目会给出。

---学过体系结构的童鞋，做这个题目就得心应手，我们首先找到最大频繁项的任务TASK，然后我们可以通过将其分隔成M块（M表示TASK的个数）。然后再TASK之间插入其他不同的任务，我们直接来举例说明。

---AAAABBBEEFFGG 3

---Frame: "AXXXAXXXAXXXA"

---insert

---'B'

---:

---"ABXXABXXABXXA"

---<

------ 'B' has higher frequency than the other characters, insert it first.

---insert

---'E'

---:

---"ABEXABEXABXXA"

---insert

---'F'

---:

---"ABEFABEXABFXA"

---<

------ each time try to fill the k-1 gaps as full or evenly as possible.

---insert

---'G'

---:

---"ABEFABEGABFGA"

---AACCCBEEE 2

---3

---identical chunks

---"CE"

---,

---"CE CE CE"

---<--

---this

---is

---a frame
insert

---'A'

---among the gaps

---of

---chunks since it has higher frequency than

---'B'

------>

---"CEACEACE"

---insert

---'B'

------>

---"CEABCEACE"

---<----- result

---is

---tasks.length;

---AACCCDDEEE 3

---3

---identical chunks

---"CE"

---,

---"CE CE CE"

---<---

---this

---is

---a frame.
Begin to insert

---'A'

---->

---"CEA CEA CE"

---Begin to insert

---'B'

---->

---"CEABCEABCE"

---<---- result

---is

---tasks.length;

---ACCCEEE 2

---3

---identical chunks

---"CE"

---,

---"CE CE CE"

---<--

---this

---is

---a frame
Begin to insert

---'A'

----->

---"CEACE CE"

---<-- result

---is

---(c[

---25

---] -

---1

---) * (n +

---1

---) +

---25

----i =

---2

---*

---3

---+

---2

---=

---8

---最终我们总结出了一个公式：(c[25] - 1) * (n + 1) + 25 -i

---代码

---class

---Solution {

---public

---:

---int

---leastInterval(

---vector

---<

---char

--->

---& tasks,

---int

---n) {

---vector

---<

---int

--->

---res(

---26

---);

---for

---(

---auto

---task:tasks){
            res[task-

---'A'

---]++;
        }
        sort(res.begin(),res.end());

---int

---i=

---25

---;

---while

---(i>=

---0

---&res[i]==res[

---25

---]){
            i--;
        }

---return

---tasks.size()>(res[

---25

---]-

---1

---)*(n+

---1

---)+

---25

----i?tasks.size():(res[

---25

---]-

---1

---)*(n+

---1

---)+

---25

----i;
    }
};


