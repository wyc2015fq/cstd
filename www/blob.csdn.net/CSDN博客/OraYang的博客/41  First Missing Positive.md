
# 41. First Missing Positive - OraYang的博客 - CSDN博客

2018年03月05日 11:28:35[OraYang](https://me.csdn.net/u010665216)阅读数：255标签：[Array																](https://so.csdn.net/so/search/s.do?q=Array&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given an unsorted integer array, find the first missing positive integer.
For example,
Given`[1,2,0]`return`3`,
and`[3,4,-1,1]`return`2`.
Your algorithm should run in*O*(*n*) time and uses constant space.

# 思路
本题题目描述的不是很详细，容易误解题意。题目中要求找到数组中未出现的第一个正整数。
对于一个大小与N的数组，我们假设N=5。那么该数组必须是如下形式：
> [1,2,3,4,5]
当我们任意将其中一个数进行替换后得到的数组就是测试数组。将2->0
> [1,0,3,4,5]
关于这个问题我们的思路就是：
> 先排序

> 再遍历找到第一个缺失的正整数
关于这个特殊结构的数组排序。我们可以通过交换法来实现。时间复杂度为O(n),空间复杂度为O(1)。
# 代码
```python
class Solution {
```
```python
public
```
```python
int
```
```python
firstMissingPositive
```
```python
(
```
```python
int
```
```python
[] nums) {
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
;i<nums.length;i++)
        {
```
```python
while
```
```python
(nums[i]>
```
```python
0
```
```python
&&nums[i]<=nums.length&&nums[i]!=nums[nums[i]-
```
```python
1
```
```python
])
                swap(nums,i,nums[i]-
```
```python
1
```
```python
);
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
;i<nums.length;i++)
        {
```
```python
if
```
```python
(i!=(nums[i]-
```
```python
1
```
```python
))
            {
```
```python
return
```
```python
i+
```
```python
1
```
```python
;
            }
        }
```
```python
return
```
```python
nums.length+
```
```python
1
```
```python
;
    }
```
```python
private
```
```python
static
```
```python
void
```
```python
swap
```
```python
(
```
```python
int
```
```python
[] nums,
```
```python
int
```
```python
i,
```
```python
int
```
```python
j)
    {
```
```python
int
```
```python
temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
    }
}
```

