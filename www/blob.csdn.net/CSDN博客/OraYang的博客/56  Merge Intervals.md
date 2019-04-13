
# 56. Merge Intervals - OraYang的博客 - CSDN博客

2017年09月20日 11:27:41[OraYang](https://me.csdn.net/u010665216)阅读数：265个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a collection of intervals, merge all overlapping intervals.
For example,
Given [1,3],[2,6],[8,10],[15,18],
return [1,6],[8,10],[15,18].
## 思路
本题，说难不难，说简单也不简单，就是稍稍有些绕，特此总结一个清晰的思路，如下：
> step1:首先对
> vector<Interval>
> 进行排序，这里的compare函数需要自己写，如下：

```python
struct myclass {
        bool operator() (Interval i,Interval j)  { return (i.
```
```python
start
```
```python
<j.
```
```python
start
```
```python
);
```
```python
}
        } myobject; 
sort(intervals.
```
```python
begin
```
```python
(),intervals.
```
```python
end
```
```python
(),myobject);
```
> step2：然后维护两个变量start和end,来标记当前数组的起始与结束的位置，遍历数组，遍历的过程中需要判断相邻两个数组之间是否有重叠，重叠的触发条件如下：

```python
intervals[i].
```
```python
end
```
```python
>=intervals[i+
```
```python
1
```
```python
].
```
```python
start
```
step3 ：如果没有重叠，更新start、end并添加Interval，如果存在重叠，同样更新start、end;
step4：调出循环，添加临界数组即末尾数组
## 代码
```python
/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 *     Interval() : start(0), end(0) {}
 *     Interval(int s, int e) : start(s), end(e) {}
 * };
 */
```
```python
class
```
```python
Solution {
```
```python
public
```
```python
:
```
```python
struct
```
```python
myclass {
```
```python
bool
```
```python
operator
```
```python
() (Interval i,Interval j)  {
```
```python
return
```
```python
(i.start<j.start); }
        } myobject;
```
```python
vector
```
```python
<Interval>
```
```python
merge(
```
```python
vector
```
```python
<Interval>
```
```python
& intervals) {
```
```python
vector
```
```python
<Interval>
```
```python
res;
```
```python
if
```
```python
(intervals.empty())
```
```python
return
```
```python
res;
        Interval temp;
        sort(intervals.begin(),intervals.end(),myobject);
```
```python
//排序
```
```python
int
```
```python
start=intervals[
```
```python
0
```
```python
].start,end = intervals[
```
```python
0
```
```python
].end;
```
```python
//重叠的触发条件：intervals[i].end>=intervals[i+1].start
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<intervals.size()-
```
```python
1
```
```python
;i++)
        {
```
```python
if
```
```python
(intervals[i].end<intervals[i+
```
```python
1
```
```python
].start)
            {
                temp.start = start;
                temp.end = intervals[i].end;
                res.push_back(temp);
                start = intervals[i+
```
```python
1
```
```python
].start;
                end = intervals[i+
```
```python
1
```
```python
].end;
            }
```
```python
else
```
```python
//发生重叠
```
```python
{
```
```python
if
```
```python
(intervals[i].end>=intervals[i+
```
```python
1
```
```python
].end)
                {
                    start = intervals[i].start;
                    end = intervals[i].end;
                    intervals[i+
```
```python
1
```
```python
].start = start;
                    intervals[i+
```
```python
1
```
```python
].end = end;
                }
```
```python
else
```
```python
{
                    start = intervals[i].start;
                    end = intervals[i+
```
```python
1
```
```python
].end;
                    intervals[i+
```
```python
1
```
```python
].start = start ;
                    intervals[i+
```
```python
1
```
```python
].end = end;
                }
            }
        }
        temp.start = start;
        temp.end = intervals[intervals.size()-
```
```python
1
```
```python
].end;
        res.push_back(temp);
```
```python
return
```
```python
res; 
    }
};
```

