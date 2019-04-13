
# 162. Find Peak Element - OraYang的博客 - CSDN博客

2017年12月18日 17:03:47[OraYang](https://me.csdn.net/u010665216)阅读数：346所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



\# 162. Find Peak Element
标签（空格分隔）： leetcode array
—



\# 题目A peak element is an element that is greater than its neighbors.
Given an input array where`num[i] ≠ num[i+1]`, find a peak element and return its index.
The array may contain multiple peaks, in that case return the index to any one of the peaks is fine.
You may imagine that`num[-1] = num[n] = -∞`.
For example, in array`[1, 2, 3, 1]`, 3 is a peak element and your function should return the index number 2.
[click to show spoilers.](#)
**Note:**Your solution should be in logarithmic complexity.

# 思路
这道题目由于题目的特殊性，因此我们可以确定该数组必定存在peak,然后我们可以应用二分搜索。唯一要注意的是这里要维护两个中间值middle1,middle2,通过比价这两个值来决定接下来的迭代方向。
# 代码
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
int
```
```python
findPeakElement(
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
& nums) {
```
```python
int
```
```python
start=
```
```python
0
```
```python
,end=nums.size()-
```
```python
1
```
```python
;
```
```python
while
```
```python
(start<end)
        {
```
```python
int
```
```python
middle1=(start+end)/
```
```python
2
```
```python
;
```
```python
int
```
```python
middle2=middle1+
```
```python
1
```
```python
;
```
```python
if
```
```python
(nums[middle1]>nums[middle2])
                end=middle1;
```
```python
else
```
```python
start=middle2;
        }
```
```python
return
```
```python
start;
    }
};
```

