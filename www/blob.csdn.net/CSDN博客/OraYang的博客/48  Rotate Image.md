
# 48. Rotate Image - OraYang的博客 - CSDN博客

2017年09月18日 14:08:39[OraYang](https://me.csdn.net/u010665216)阅读数：494所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
You are given an n x n 2D matrix representing an image.
Rotate the image by 90 degrees (clockwise).
Note:
You have to rotate the image in-place, which means you have to modify the input 2D matrix directly. DO NOT allocate another 2D matrix and do the rotation.
Example 1:
Given input matrix =
[
[1,2,3],
[4,5,6],
[7,8,9]
],
rotate the input matrix in-place such that it becomes:
[
[7,4,1],
[8,5,2],
[9,6,3]
]
Example 2:
Given input matrix =
[
[ 5, 1, 9,11],
[ 2, 4, 8,10],
[13, 3, 6, 7],
[15,14,12,16]
],
rotate the input matrix in-place such that it becomes:
[
[15,13, 2, 5],
[14, 3, 4, 1],
[12, 6, 8, 9],
[16, 7,10,11]
]
## 思路
第一种方法是直接根据题意，做翻转，又外到内，示意图如下：
![这里写图片描述](https://img-blog.csdn.net/20170918112853379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170918112853379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码如下：
[
](https://img-blog.csdn.net/20170918112853379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
class Solution {
public:
    void
```
```python
rotate
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
vector
```
```python
<
```
```python
int
```
```python
>>&
```
```python
matrix
```
```python
) {
```
```python
int
```
```python
layer =
```
```python
matrix
```
```python
.
```
```python
size
```
```python
()/
```
```python
2
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
;i<layer;i++)
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
j=i;j<
```
```python
matrix
```
```python
.
```
```python
size
```
```python
()-i-
```
```python
1
```
```python
;j++)
            {
```
```python
int
```
```python
temp =
```
```python
matrix
```
```python
[i][j];
```
```python
matrix
```
```python
[i][j] =
```
```python
matrix
```
```python
[
```
```python
matrix
```
```python
.
```
```python
size
```
```python
()-j-
```
```python
1
```
```python
][i];
```
```python
matrix
```
```python
[
```
```python
matrix
```
```python
.
```
```python
size
```
```python
()-j-
```
```python
1
```
```python
][i] =
```
```python
matrix
```
```python
[
```
```python
matrix
```
```python
.
```
```python
size
```
```python
()-i-
```
```python
1
```
```python
][
```
```python
matrix
```
```python
.
```
```python
size
```
```python
()-j-
```
```python
1
```
```python
];
```
```python
matrix
```
```python
[
```
```python
matrix
```
```python
.
```
```python
size
```
```python
()-i-
```
```python
1
```
```python
][
```
```python
matrix
```
```python
.
```
```python
size
```
```python
()-j-
```
```python
1
```
```python
] =
```
```python
matrix
```
```python
[j][
```
```python
matrix
```
```python
.
```
```python
size
```
```python
()-i-
```
```python
1
```
```python
];
```
```python
matrix
```
```python
[j][
```
```python
matrix
```
```python
.
```
```python
size
```
```python
()-i-
```
```python
1
```
```python
] = temp;
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
[
](https://img-blog.csdn.net/20170918112853379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)虽然由外到内逐步翻转，思路简单，但是写起来还是很容易写错，而且时间效率比较低，所以，用另外一种方法，对矩阵做两次变换，一次是沿对角线翻转，一次是沿水平线反转，示意图如下：
![这里写图片描述](https://img-blog.csdn.net/20170918140801554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170918140801554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码如下：
[

](https://img-blog.csdn.net/20170918140801554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
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
rotate(
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
&matrix) {
```
```python
int
```
```python
i,j,temp;
```
```python
int
```
```python
n=matrix.size();
```
```python
// 沿着副对角线反转
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
i =
```
```python
0
```
```python
; i < n; ++i) {
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
j =
```
```python
0
```
```python
; j < n - i; ++j) {  
                temp = matrix[i][j];  
                matrix[i][j] = matrix[n -
```
```python
1
```
```python
- j][n -
```
```python
1
```
```python
- i];  
                matrix[n -
```
```python
1
```
```python
- j][n -
```
```python
1
```
```python
- i] = temp;  
            }  
        }
```
```python
// 沿着水平中线反转
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
i =
```
```python
0
```
```python
; i < n /
```
```python
2
```
```python
; ++i){
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
j =
```
```python
0
```
```python
; j < n; ++j) {  
                temp = matrix[i][j];  
                matrix[i][j] = matrix[n -
```
```python
1
```
```python
- i][j];  
                matrix[n -
```
```python
1
```
```python
- i][j] = temp;  
            }  
        }  
    }  
};
```
[            ](https://img-blog.csdn.net/20170918140801554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

[
  ](https://img-blog.csdn.net/20170918112853379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)