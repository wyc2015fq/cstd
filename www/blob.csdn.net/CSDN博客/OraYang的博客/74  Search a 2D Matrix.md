
# 74. Search a 2D Matrix - OraYang的博客 - CSDN博客

2017年09月25日 09:28:09[OraYang](https://me.csdn.net/u010665216)阅读数：195所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Write an efficient algorithm that searches for a value in an m x n matrix. This matrix has the following properties:
Integers in each row are sorted from left to right.
The first integer of each row is greater than the last integer of the previous row.
For example,
Consider the following matrix:
[
[1,   3,  5,  7],
[10, 11, 16, 20],
[23, 30, 34, 50]
]
Given target = 3, return true.
## 思路
本题矩阵的各种限制条件就是说明这个矩阵是有序的，查找target就是用二分法：
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
searchMatrix(
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
& matrix,
```
```python
int
```
```python
target) {
```
```python
if
```
```python
(matrix.empty())
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
int
```
```python
m=matrix.size(),n=matrix[
```
```python
0
```
```python
].size();
```
```python
int
```
```python
first =
```
```python
0
```
```python
,end =m*n-
```
```python
1
```
```python
,mid = (m*n-
```
```python
1
```
```python
) /
```
```python
2
```
```python
;
```
```python
while
```
```python
(first<=end)
        {
```
```python
if
```
```python
(target>matrix[m-
```
```python
1
```
```python
][n-
```
```python
1
```
```python
]||target<matrix[
```
```python
0
```
```python
][
```
```python
0
```
```python
])
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
else
```
```python
if
```
```python
(target>matrix[mid/n][mid-(mid/n)*n])
            {
                first = mid+
```
```python
1
```
```python
;
                mid = (first+end) /
```
```python
2
```
```python
;
            }
```
```python
else
```
```python
if
```
```python
(target<matrix[mid/n][mid-(mid/n)*n])
            {
                end = mid -
```
```python
1
```
```python
;
                mid = (first+end) /
```
```python
2
```
```python
;
            }
```
```python
else
```
```python
if
```
```python
(target==matrix[mid/n][mid-(mid/n)*n])
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
```
```python
return
```
```python
false
```
```python
;    
    }
};
```

