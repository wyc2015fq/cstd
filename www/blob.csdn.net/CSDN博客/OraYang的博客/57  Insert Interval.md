
# 57. Insert Interval - OraYang的博客 - CSDN博客

2018年03月04日 14:18:48[OraYang](https://me.csdn.net/u010665216)阅读数：255所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given a set of*non-overlapping*intervals, insert a new interval into the intervals (merge if necessary).
You may assume that the intervals were initially sorted according to their start times.
**Example 1:**
Given intervals`[1,3],[6,9]`, insert and merge`[2,5]`in as`[1,5],[6,9]`.
**Example 2:**
Given`[1,2],[3,5],[6,7],[8,10],[12,16]`, insert and merge`[4,9]`in as`[1,2],[3,10],[12,16]`.
This is because the new interval`[4,9]`overlaps with`[3,5],[6,7],[8,10]`.

# 思路
这道题目比较简单，就是逻辑结构有点繁琐。
# 代码
```python
/**
 * Definition for an interval.
 * public class Interval {
 *     int start;
 *     int end;
 *     Interval() { start = 0; end = 0; }
 *     Interval(int s, int e) { start = s; end = e; }
 * }
 */
```
```python
class Solution {
```
```python
public
```
```python
List<Interval>
```
```python
insert
```
```python
(List<Interval> intervals, Interval newInterval) {
```
```python
if
```
```python
(intervals.size()<=
```
```python
0
```
```python
)
        {
            intervals.add(newInterval);
```
```python
return
```
```python
intervals;
        }
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
;i<intervals.size();i++)
        {
```
```python
int
```
```python
astart = intervals.get(i).start;
```
```python
int
```
```python
aend = intervals.get(i).end;
```
```python
int
```
```python
bstart = newInterval.start;
```
```python
int
```
```python
bend = newInterval.end;
```
```python
if
```
```python
(bstart<astart)
            {
```
```python
if
```
```python
(bend<astart)
                {
                    intervals.add(i,newInterval);
```
```python
return
```
```python
intervals;
                }
```
```python
else
```
```python
{
```
```python
if
```
```python
(bend<=aend)
                    {
                        intervals.get(i).start = bstart;
```
```python
return
```
```python
intervals;
                    }
```
```python
else
```
```python
{
                        intervals.remove(i);
                        i--;
                        System.out.println(i);
```
```python
continue
```
```python
;
                    }
                }
            }
```
```python
else
```
```python
{
```
```python
if
```
```python
(bstart<=aend){
```
```python
if
```
```python
(bend<=aend)
                {
```
```python
return
```
```python
intervals;
                }
```
```python
else
```
```python
{
                    newInterval.start = astart;
                    intervals.remove(i);
                    i--;
```
```python
continue
```
```python
;
                }
                }
```
```python
else
```
```python
{
```
```python
continue
```
```python
;
                }
            }
        }
        intervals.add(newInterval);
```
```python
return
```
```python
intervals;
    }
}
```

