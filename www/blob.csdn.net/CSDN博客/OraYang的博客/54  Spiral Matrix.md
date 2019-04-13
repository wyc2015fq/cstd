
# 54. Spiral Matrix - OraYang的博客 - CSDN博客

2017年09月19日 22:39:28[OraYang](https://me.csdn.net/u010665216)阅读数：212所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a matrix of m x n elements (m rows, n columns), return all elements of the matrix in spiral order.
For example,
Given the following matrix:
[
[ 1, 2, 3 ],
[ 4, 5, 6 ],
[ 7, 8, 9 ]
]
You should return [1,2,3,6,9,8,7,4,5].
## 思路
本题思路很简单,但是就是写起来复杂，是真的复杂，很容易写错，主要思路如下:
> 建立四个变量分别为
> int rowbegin = 0,rowend=matrix.size()-1,colbegin=0,colend = matrix[0].size()-1;

> 然后按照从左往右，从上至下，从右往左，从下至上的顺序，不断循环；

> 满足条件，跳出循环，并返回

> 代码

> class
> Solution {
> public
> :
> vector
> <
> int
> >
> spiralOrder(
> vector
> <
> vector
> <
> int
> >
> >
> & matrix) {
> vector
> <
> int
> >
> res;
> if
> (matrix.empty())
> return
> res;
> int
> rowbegin =
> 0
> ,rowend=matrix.size()-
> 1
> ,colbegin=
> 0
> ,colend = matrix[
> 0
> ].size()-
> 1
> ;
> while
> (rowbegin<=rowend&&colbegin<=colend)
> //大循环
> {
> for
> (
> int
> i=colbegin;i<=colend;i++)
> //从左往右
> {
                res.push_back(matrix[rowbegin][i]);
            }
            rowbegin++;
> for
> (
> int
> i=rowbegin;i<=rowend;i++)
> //从上至下
> {
                res.push_back(matrix[i][colend]);
            }
            colend--;
> if
> (rowbegin<=rowend)
            {
> for
> (
> int
> i=colend;i>=colbegin;i--)
> //从右往左
> {
                    res.push_back(matrix[rowend][i]);
                }
            }
            rowend--;
> if
> (colbegin<=colend)
            {
> for
> (
> int
> i=rowend;i>=rowbegin;i--)
> //从下往上
> {
                    res.push_back(matrix[i][colbegin]);
                }
            }
            colbegin++;
        }
> return
> res;
    }
};

