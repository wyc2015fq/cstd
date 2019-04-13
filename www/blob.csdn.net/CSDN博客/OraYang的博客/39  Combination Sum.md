
# 39. Combination Sum - OraYang的博客 - CSDN博客

2017年09月17日 14:31:31[OraYang](https://me.csdn.net/u010665216)阅读数：122个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a set of candidate numbers (C) (without duplicates) and a target number (T), find all unique combinations in C where the candidate numbers sums to T.
The same repeated number may be chosen from C unlimited number of times.
Note:
All numbers (including target) will be positive integers.
The solution set must not contain duplicate combinations.
For example, given candidate set [2, 3, 6, 7] and target 7,
A solution set is:
[
[7],
[2, 2, 3]
]
## 思路
本题是典型的回溯法，基本思路就是排序，回溯，但是要注意，数值可以被使用多次，所以回溯时要注意代码中i不变
> backtrack(candidates,cur,i,target-candidates[i]);

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
res;
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
combinationSum(
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
& candidates,
```
```python
int
```
```python
target) {
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
cur;
        sort(candidates.begin(),candidates.end());
        backtrack(candidates,cur,
```
```python
0
```
```python
,target);
```
```python
return
```
```python
res;
    }
```
```python
private
```
```python
:
```
```python
void
```
```python
backtrack(
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
candidates,
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
cur,
```
```python
int
```
```python
start ,
```
```python
int
```
```python
target)
    {
```
```python
if
```
```python
(target==
```
```python
0
```
```python
)
        {
            res.push_back(cur);
        }
```
```python
else
```
```python
{
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
i=start;i<candidates.size()&&candidates[i]<=target;i++)
            {
                cur.push_back(candidates[i]);
                backtrack(candidates,cur,i,target-candidates[i]);
                cur.pop_back();
            }
        }
    }
};
```

