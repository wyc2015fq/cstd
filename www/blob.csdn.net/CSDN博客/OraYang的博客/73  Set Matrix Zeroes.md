
# 73. Set Matrix Zeroes - OraYang的博客 - CSDN博客

2017年09月23日 15:45:58[OraYang](https://me.csdn.net/u010665216)阅读数：155个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a m x n matrix, if an element is 0, set its entire row and column to 0. Do it in place.
## 思路
本题因为要in place,那么可以利用这样的思路来分析：
> 用每行的第一个位置存储该行的状态(如果存在0，该位置置0）

> 用每列的第一位位置存储该列的状态（如果存在0，则该位置置0）

> 维护一个变量，保存第一行第一列即上述重合的位置，特代表第0列的状态

> 代码

> class
> Solution {
> public
> :
> void
> setZeroes(
> vector
> <
> vector
> <
> int
> >
> >
> & matrix) {
> int
> col0 =
> 1
> , rows = matrix.size(), cols = matrix[
> 0
> ].size();
> for
> (
> int
> i =
> 0
> ; i < rows; i++) {
> if
> (matrix[i][
> 0
> ] ==
> 0
> ) col0 =
> 0
> ;
> for
> (
> int
> j =
> 1
> ; j < cols; j++)
> if
> (matrix[i][j] ==
> 0
> )
                matrix[i][
> 0
> ] = matrix[
> 0
> ][j] =
> 0
> ;
    }
> for
> (
> int
> i = rows -
> 1
> ; i >=
> 0
> ; i--) {
> for
> (
> int
> j = cols -
> 1
> ; j >=
> 1
> ; j--)
> if
> (matrix[i][
> 0
> ] ==
> 0
> || matrix[
> 0
> ][j] ==
> 0
> )
                matrix[i][j] =
> 0
> ;
> if
> (col0 ==
> 0
> ) matrix[i][
> 0
> ] =
> 0
> ;
    }
    }
};

