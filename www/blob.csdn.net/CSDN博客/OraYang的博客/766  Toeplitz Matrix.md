
# 766. Toeplitz Matrix - OraYang的博客 - CSDN博客

2018年03月14日 13:23:05[OraYang](https://me.csdn.net/u010665216)阅读数：243所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
A matrix is*Toeplitz*if every diagonal from top-left to bottom-right has the same element.
Now given an`M x N`matrix, return`True`if and only if the matrix is*Toeplitz*.
**Example 1:**
**Input:**matrix = [[1,2,3,4],[5,1,2,3],[9,5,1,2]]**Output:**True**Explanation:**1234
5123
9512
In the above grid, the diagonals are "[9]", "[5, 5]", "[1, 1, 1]", "[2, 2, 2]", "[3, 3]", "[4]", and in each diagonal all elements are the same, so the answer is True.**Example 2:**
**Input:**matrix = [[1,2],[2,2]]**Output:**False**Explanation:**The diagonal "[1, 2]" has different elements.**Note:**
matrixwill be a 2D array of integers.
matrixwill have a number of rows and columns in range[1, 20].
matrix[i][j]will be integers in range[0, 99].


# 思路
本题我们先举个例子，加入存在一个3*3的Toeplitz矩阵，那么该矩阵如下所示：
|1|2|3|
|---|---|---|
|5|1|2|
|9|5|1|
我们发现，数字1对应的坐标如下所示：
> (0,0)、（1,1）、(2,2)
数字2对应的坐标：
> (0,1)、（1,2）
数字5对应的坐标：
> (1,0)、（2,1）
很明显我们发现凡是数字相同的坐标都是x与y坐标自增1的
所以我们只要遍历完整个数组，并检查当前坐标（x,y）与（x+1,y+1)的关系即可。一旦发现不想等直接返回false，否则一直循环直至退出，返回true。
# 代码
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
isToeplitzMatrix(
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
& matrix) {
```
```python
int
```
```python
n = matrix.size();
```
```python
int
```
```python
m = matrix[
```
```python
0
```
```python
].size();
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
;i<n-
```
```python
1
```
```python
;i++)
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
j=
```
```python
0
```
```python
;j<m-
```
```python
1
```
```python
;j++)
            {
```
```python
if
```
```python
(matrix[i][j]!=matrix[i+
```
```python
1
```
```python
][j+
```
```python
1
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
            }
        }
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

