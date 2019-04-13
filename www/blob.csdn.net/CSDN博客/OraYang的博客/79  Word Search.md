
# 79. Word Search - OraYang的博客 - CSDN博客

2017年10月30日 15:12:09[OraYang](https://me.csdn.net/u010665216)阅读数：476所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a 2D board and a word, find if the word exists in the grid.
The word can be constructed from letters of sequentially adjacent cell, where “adjacent” cells are those horizontally or vertically neighboring. The same letter cell may not be used more than once.
For example,
Given board =
> [

> [‘A’,’B’,’C’,’E’],

> [‘S’,’F’,’C’,’S’],

> [‘A’,’D’,’E’,’E’]

> ]

> word = “ABCCED”, -> returns true,

> word = “SEE”, -> returns true,

> word = “ABCB”, -> returns false.

> 思路

> 本题是典型的深度优先遍历，外层循环遍历整个二维数组，内层循环对每个节点上下左右做遍历

> 外层循环：

> //外层循环
> for
> (
> int
> i=
> 0
> ;i<board.
> size
> ();i++)
        {
> for
> (
> int
> j=
> 0
> ;j<board[
> 0
> ].
> size
> ();j++)
            {
> if
> (search(board,word,
> 0
> ,i,j,used))
> return
> true;
            }
        }
> 内层循环：

> //DFS
> bool res = search(board,word,
> index
> +
> 1
> ,i-
> 1
> ,j,used)   
                || search(board,word,
> index
> +
> 1
> ,i+
> 1
> ,j,used)  
                || search(board,word,
> index
> +
> 1
> ,i,j-
> 1
> ,used)   
                || search(board,word,
> index
> +
> 1
> ,i,j+
> 1
> ,used);
> 代码

> class
> Solution {
> public
> :
> bool
> exist(
> vector
> <
> vector
> <
> char
> >
> >
> & board,
> string
> word) {
> if
> (word.empty()||word.size()==
> 0
> )
> return
> false
> ;
> if
> (board.empty()||board.size()==
> 0
> ||board[
> 0
> ].size()==
> 0
> )
> return
> false
> ;
> vector
> <
> vector
> <
> bool
> >
> >
> used(board.size(),
> vector
> <
> bool
> >
> (board[
> 0
> ].size()));
> for
> (
> int
> i=
> 0
> ;i<board.size();i++)
        {
> for
> (
> int
> j=
> 0
> ;j<board[
> 0
> ].size();j++)
            {
> if
> (search(board,word,
> 0
> ,i,j,used))
> return
> true
> ;
            }
        }
> return
> false
> ; 
    }
> private
> :
> bool
> search(
> vector
> <
> vector
> <
> char
> >
> >
> board,
> string
> word,
> int
> index,
> int
> i,
> int
> j,
> vector
> <
> vector
> <
> bool
> >
> >
> used)
    {
> if
> (index==word.size())
> return
> true
> ;
> if
> (i<
> 0
> ||j<
> 0
> ||i>=board.size()|| j>=board[
> 0
> ].size() || used[i][j] || board[i][j]!=word[index])
> return
> false
> ; 
        used[i][j] =
> true
> ;
> bool
> res = search(board,word,index+
> 1
> ,i-
> 1
> ,j,used)   
                || search(board,word,index+
> 1
> ,i+
> 1
> ,j,used)  
                || search(board,word,index+
> 1
> ,i,j-
> 1
> ,used)   
                || search(board,word,index+
> 1
> ,i,j+
> 1
> ,used);  
        used[i][j] =
> false
> ;
> return
> res; 
    }
};

