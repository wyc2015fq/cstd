
# 80. Remove Duplicates from Sorted Array II - OraYang的博客 - CSDN博客

2017年10月30日 15:40:34[OraYang](https://me.csdn.net/u010665216)阅读数：463标签：[Remove																](https://so.csdn.net/so/search/s.do?q=Remove&t=blog)[Duplicates																](https://so.csdn.net/so/search/s.do?q=Duplicates&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Remove&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Follow up for “Remove Duplicates”:
What if duplicates are allowed at most twice?
For example,
Given sorted array nums = [1,1,1,2,2,3],
Your function should return length = 5, with the first five elements of nums being 1, 1, 2, 2 and 3. It doesn’t matter what you leave beyond the new length.
## 思路
本题计算数组个数，因为数组有序，且重复项不超过俩个，那么只要从数组index=2处将nums[index]与nums[index-2]比大小，大的就计数加一，否则不变
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
removeDuplicates(
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
i =
```
```python
0
```
```python
;
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
n : nums)
```
```python
if
```
```python
(i <
```
```python
2
```
```python
|| n > nums[i-
```
```python
2
```
```python
])
                nums[i++] = n;
```
```python
return
```
```python
i;
}
};
```

