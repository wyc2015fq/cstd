
# 75. Sort Colors - OraYang的博客 - CSDN博客

2017年09月25日 10:14:49[OraYang](https://me.csdn.net/u010665216)阅读数：170个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given an array with n objects colored red, white or blue, sort them so that objects of the same color are adjacent, with the colors in the order red, white and blue.
Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue respectively.
Note:
You are not suppose to use the library’s sort function for this problem.
click to show follow up.
Follow up:
A rather straight forward solution is a two-pass algorithm using counting sort.
First, iterate the array counting number of 0’s, 1’s, and 2’s, then overwrite array with total number of 0’s, then 1’s and followed by 2’s.
Could you come up with an one-pass algorithm using only constant space?
## 思路
本题因为数组数值比较特殊，只有三种0，1，2；那么只要遍历数组，遇到0将其插入数组头部，遇到2将其插入到数组尾部。剩下的1就处在0，2之间。
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
void
```
```python
sortColors(
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
count =
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
i=
```
```python
0
```
```python
;i<nums.size()-count;i++)
        {
```
```python
if
```
```python
(nums[i]==
```
```python
0
```
```python
&&i!=
```
```python
0
```
```python
)
            {
                nums.erase(nums.begin()+i);
                nums.insert(nums.begin(),
```
```python
0
```
```python
);
            }
```
```python
else
```
```python
if
```
```python
(nums[i]==
```
```python
2
```
```python
)
            {
                count++;
                nums.erase(nums.begin()+i);
                nums.insert(nums.end(),
```
```python
2
```
```python
);
                i--;
            }
        }
```
```python
return
```
```python
;
    }
};
```

