
# 209. Minimum Size Subarray Sum - OraYang的博客 - CSDN博客

2017年12月15日 15:00:48[OraYang](https://me.csdn.net/u010665216)阅读数：234标签：[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given an array of n positive integers and a positive integer s, find the minimal length of a contiguous subarray of which the sum ≥ s. If there isn’t one, return 0 instead.
For example, given the array [2,3,1,2,4,3] and s = 7,
the subarray [4,3] has the minimal length under the problem constraint.
# 思路
本题给出一个O(n)的思路。这道题目需要维护两个数组索引i，j。通过不断地增加i或增加j来判断是否有满足条件的值。具体思路看代码实现。
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
minSubArrayLen(
```
```python
int
```
```python
s,
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
sum=
```
```python
0
```
```python
,i=
```
```python
0
```
```python
,j=
```
```python
0
```
```python
,min_len=INT_MAX;
```
```python
while
```
```python
(j<nums.size()){
            sum+=nums[j++];
```
```python
while
```
```python
(sum>=s)
            {
                min_len = min(min_len,j-i);
                sum-=nums[i++];
            }
        }
```
```python
return
```
```python
min_len==INT_MAX?
```
```python
0
```
```python
:min_len;     
    }
};
```

