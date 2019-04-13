
# 4. Median of Two Sorted Arrays - OraYang的博客 - CSDN博客

2017年09月13日 08:50:42[OraYang](https://me.csdn.net/u010665216)阅读数：258个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
There are two sorted arrays nums1 and nums2 of size m and n respectively.
Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).
Example 1:
nums1 = [1, 3]
nums2 = [2]
The median is 2.0
Example 2:
nums1 = [1, 2]
nums2 = [3, 4]
The median is (2 + 3)/2 = 2.5
## 思路：
本题，没有时间复杂度的要求，那么使用STL中函数，时间复杂度为（M+n)log(M+N)，时间复杂度log(M+N）的解法暂时还没想到一个易于理解的，故之后再补上。。。
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
double
```
```python
findMedianSortedArrays(
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
& nums1,
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
& nums2) {
        nums2.insert(nums2.end(),nums1.begin(),nums1.end());
```
```python
int
```
```python
size = nums2.size();
        sort(nums2.begin(),nums2.end());
```
```python
if
```
```python
(size%
```
```python
2
```
```python
==
```
```python
0
```
```python
)
```
```python
return
```
```python
(nums2[size/
```
```python
2
```
```python
-
```
```python
1
```
```python
]+nums2[size/
```
```python
2
```
```python
])/
```
```python
2.0
```
```python
;
```
```python
else
```
```python
return
```
```python
nums2[size/
```
```python
2
```
```python
];
    }
};
```

