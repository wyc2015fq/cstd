# [算法] - insert interval Vector模板类的应用 - Snoopy_Dream - CSDN博客





2018年12月26日 10:27:40[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：46
个人分类：[算法 数据结构](https://blog.csdn.net/e01528/article/category/8547486)

所属专栏：[编程感悟](https://blog.csdn.net/column/details/31618.html)









## 问题描述

> 
Given a set of non-overlapping intervals, insert a new interval into the intervals (merge if necessary).

You may assume that the intervals were initially sorted according to their start times.

Example 1:

Given intervals[1,3],[6,9], insert and merge[2,5]in as[1,5],[6,9].

Example 2:

Given[1,2],[3,5],[6,7],[8,10],[12,16], insert and merge[4,9]in as[1,2],[3,10],[12,16].

This is because the new interval[4,9]overlaps with[3,5],[6,7],[8,10].


## 问题分析

这道题的意思是给出一个区间，和原区间进行比较后**若重合则覆盖**，最后**给出覆盖后的新区间。**
- 遍历原来的List
- 如果新区间的end < 当前区间的start，不用找下去了，把新区间插入到当前区间的前面，然后返回。
- 如果当前区间的end小于新区间的start，继续遍历找下一个区间。
- 如果当前区间和新区间发生重合，则start取两者最小的start，end取两者最大的end，生成一个新的区间。
- 继续遍历。

## 大体思路
- 复制一个新的列表出来，这样不会影响原先的列表。
- 考虑特殊情况，如果新区间的end < 当前区间的start，直接遍历结束，break。
- 如果当前区间的end小于新区间的start，继续遍历找下一个区间。这个if里不做任何操作。**{}**就行，表**示执行空语句**。
- 其他普通情况就是有覆盖的情况，可能会覆盖好几个（别慌，涉及一个新变量出来而已），我们只需要将新区间不断变化扩大为最终区间形态，然后把原先的那些区间删掉（当前遍历到的区间-重叠个数开始，到，当前遍历的位置结束，把这一段删除），再把新区加到指定的位置即可。编程知识点

## 编程知识点

vector模板类

**vector a;**

**a.size();   ****    a.begin();     ****            a.end(),                       a[i].begin也是可以的**

**a[i]==a.begin()+i, ****   a.erase(a.begin()+2,a.begin()+5)**表示删除的起始终点位置,

**a.insert(a.begin()+3,b.begin()+3,b.end())**三个形参，第一个表示插入的位置，其他为插入的起终，

也可以**a.insert(a.begin()+3,b)**直接将b插入;

**a.push_back(3)**将3插入a的末尾



## 代码实现

```cpp
class Solution {
public:
    vector<Interval> insert(vector<Interval> &intervals, Interval newInterval) {
        vector<Interval> res = intervals;//直接相等就可以了
        int i = 0, overlap = 0, n = res.size();//一个表示遍历到第几个了，一个遍历的总个数
        while (i < n) {//元列表按区间开始遍历
            if (newInterval.end < res[i].start) break; //先考虑特殊情况 ，如果新区的end小于这个的开头，直接写到前面 
            else if (newInterval.start > res[i].end) {} //说明还没找到，继续遍历
            else {//先只改动新区间，将新区改动成最终的模样
                newInterval.start = min(newInterval.start, res[i].start);
                newInterval.end = max(newInterval.end, res[i].end);
                ++overlap;//表示重叠了几个区间
            }
            ++i;
        }
        if (overlap > 0) res.erase(res.begin() + i - overlap, res.begin() + i);//删除重叠区域
        res.insert(res.begin() + i - overlap, newInterval)//第一个参数表示插入的位置，第二个第三个表示插入的起点和终点的位置
        return res;
    }
};
```





