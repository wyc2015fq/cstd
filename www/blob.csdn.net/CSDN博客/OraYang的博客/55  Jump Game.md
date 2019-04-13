
# 55. Jump Game - OraYang的博客 - CSDN博客

2017年09月20日 09:57:39[OraYang](https://me.csdn.net/u010665216)阅读数：253个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given an array of non-negative integers, you are initially positioned at the first index of the array.
Each element in the array represents your maximum jump length at that position.
Determine if you are able to reach the last index.
For example:
A = [2,3,1,1,4], return true.
A = [3,2,1,0,4], return false.
## 思路
本题是典型的dp问题，找到状态子方程即可：即当前位置的能跳跃的最大距离global = max(global,nums[i]+i)
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
bool
```
```python
canJump(
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
global=
```
```python
0
```
```python
,i;
```
```python
for
```
```python
(i=
```
```python
0
```
```python
;i<=global&&global<nums.size()-
```
```python
1
```
```python
;i++)
            global = max(global,nums[i]+i);
```
```python
if
```
```python
(i>global)
```
```python
return
```
```python
false
```
```python
;
```
```python
return
```
```python
true
```
```python
;
    }
};
```

