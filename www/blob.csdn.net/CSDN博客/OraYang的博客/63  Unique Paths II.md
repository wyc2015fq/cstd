
# 63. Unique Paths II - OraYang的博客 - CSDN博客

2017年09月22日 10:08:08[OraYang](https://me.csdn.net/u010665216)阅读数：203个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Follow up for “Unique Paths”:
Now consider if some obstacles are added to the grids. How many unique paths would there be?
An obstacle and empty space is marked as 1 and 0 respectively in the grid.
For example,
There is one obstacle in the middle of a 3x3 grid as illustrated below.
[
[0,0,0],
[0,1,0],
[0,0,0]
]
The total number of unique paths is 2.
Note: m and n will be at most 100.
## 思路
本题和上一篇博客[Unique Paths](http://blog.csdn.net/u010665216/article/details/78059036)很类似，唯一区别就是，当遇到当前位置为1，需要将res[j]清0.
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
uniquePathsWithObstacles(
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
& obstacleGrid) {
```
```python
int
```
```python
n = obstacleGrid[
```
```python
0
```
```python
].size(),m = obstacleGrid.size();
```
```python
if
```
```python
(m==
```
```python
0
```
```python
||m==
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
0
```
```python
;
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
res(n,
```
```python
0
```
```python
);
        res[
```
```python
0
```
```python
] =
```
```python
1
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
;i<m;i++)
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
j=
```
```python
0
```
```python
;j<n;j++)
            {
```
```python
if
```
```python
(obstacleGrid[i][j]==
```
```python
1
```
```python
)
                    res[j] =
```
```python
0
```
```python
;
```
```python
else
```
```python
{
```
```python
if
```
```python
(j>=
```
```python
1
```
```python
)
                       res[j] += res[j-
```
```python
1
```
```python
]; 
                }
            }
```
```python
return
```
```python
res[n-
```
```python
1
```
```python
];
    }
};
```

