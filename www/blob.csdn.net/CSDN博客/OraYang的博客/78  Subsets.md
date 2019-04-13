
# 78. Subsets - OraYang的博客 - CSDN博客

2017年09月27日 08:43:19[OraYang](https://me.csdn.net/u010665216)阅读数：372个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a set of distinct integers, nums, return all possible subsets.
Note: The solution set must not contain duplicate subsets.
For example,
If nums = [1,2,3], a solution is:
[
[3],
[1],
[2],
[1,2,3],
[1,3],
[2,3],
[1,2],
[]
]
## 思路
本题，也是个组合问题，本质上解法与我前面一篇博客：[Combinations](http://blog.csdn.net/u010665216/article/details/78083652)解法一致，唯一区别就是，combination是指定组合个数，而本题是所有可能的组合方式，所以再写个for循环指定k的大小即可，核心思路还是递归
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
vector
```
```python
<
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
>
```
```python
subsets(
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
size = nums.size();
```
```python
vector
```
```python
<
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
>
```
```python
res;
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
tmp;
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
;i<=size;i++)
        {
            helper(nums, i,
```
```python
0
```
```python
, tmp, res);
            tmp.clear();
        }
```
```python
return
```
```python
res;   
    }
```
```python
void
```
```python
helper(
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
nums,
```
```python
int
```
```python
k,
```
```python
int
```
```python
level,
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
& tmp,
```
```python
vector
```
```python
<
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
>
```
```python
& res) {
```
```python
if
```
```python
(tmp.size() == k) res.push_back(tmp);
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
i = level; i < nums.size(); ++i) {
            tmp.push_back(nums[i]);
            helper(nums, k, i +
```
```python
1
```
```python
, tmp, res);
            tmp.pop_back();
        }
    }
};
```

