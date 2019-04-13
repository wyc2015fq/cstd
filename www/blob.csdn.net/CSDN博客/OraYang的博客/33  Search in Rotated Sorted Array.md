
# 33. Search in Rotated Sorted Array - OraYang的博客 - CSDN博客

2017年09月15日 09:17:35[OraYang](https://me.csdn.net/u010665216)阅读数：106个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand.
(i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).
You are given a target value to search. If found in the array return its index, otherwise return -1.
You may assume no duplicate exists in the array.
## 思路
本题数据“亚有序”本来可以用二分查找的，思路就是先用二分法找最小值，时间复杂度log(n)，再从最小值及数组首端处或最小值处和数组末端处用二分查找。
但是，比较懒，就直接用了标准库函数
## 代码
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
search(
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
& nums,
```
```python
int
```
```python
target) {
```
```python
int
```
```python
index = find(nums.begin(),nums.end(),target)-nums.begin();
```
```python
return
```
```python
index>=
```
```python
0
```
```python
&&index<nums.size()?index:-
```
```python
1
```
```python
;
    }
};
```

