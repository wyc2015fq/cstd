
# 【LeetCode】283. Move Zeroes - jiange_zh的博客 - CSDN博客


2016年01月07日 13:16:16[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：400标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[算法与数据结构																](https://blog.csdn.net/jiange_zh/article/category/5787591)



## Problem
Given an array nums, write a function to move all 0’s to the end of it while maintaining the relative order of the non-zero elements.
For example, given nums = [0, 1, 0, 3, 12], after calling your function, nums should be [1, 3, 12, 0, 0].
Note:
You must do this in-place without making a copy of the array.
Minimize the total number of operations.
## Analyze
假设给你开一个辅助数组，你会怎么做？
——把非0的元素放到辅助数组中，略过0元素，最后在辅助数组末端添加足够的0即可。
时间复杂度O(n)。
不开辅助数组呢？
——其实还是一样，只不过原数组的空间可以复用。当没有0时，自然无须移动，当至少出现一个0时，当前待移动的非0元素前面必然至少有一个空闲位置可以使用（0位视为空闲）。
## My code
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
void
```
```python
moveZeroes(
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
::iterator pos = nums.begin();
```
```python
for
```
```python
(
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
::iterator num = nums.begin(); num != nums.end(); num++) {
```
```python
if
```
```python
((*num)) {
```
```python
if
```
```python
(num != pos) (*pos) = (*num);
                pos++;
            }
        }
```
```python
for
```
```python
(; pos != nums.end(); pos++ ) (*pos) =
```
```python
0
```
```python
;
    }
};
```

