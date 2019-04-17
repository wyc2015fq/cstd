# leetcode-79. Word Search 单词搜索 - 别说话写代码的博客 - CSDN博客





2019年02月22日 22:40:43[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：11标签：[Word Search																[单词搜索																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=单词搜索&t=blog)



|Given a 2D board and a word, find if the word exists in the grid.The word can be constructed from letters of sequentially adjacent cell, where "adjacent" cells are those horizontally or vertically neighboring. The same letter cell may not be used more than once.**Example:**board =[  ['A','B','C','E'],  ['S','F','C','S'],  ['A','D','E','E']]Given word = "**ABCCED**", return **true**.Given word = "**SEE**", return **true**.Given word = "**ABCB**", return **false**.|给定一个二维网格和一个单词，找出该单词是否存在于网格中。单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。同一个单元格内的字母不允许被重复使用。**示例:**board =[  ['A','B','C','E'],  ['S','F','C','S'],  ['A','D','E','E']]给定 word = "**ABCCED**", 返回 **true**.给定 word = "**SEE**", 返回 **true**.给定 word = "**ABCB**", 返回 **false**.|
|----|----|

思路：贪心算法。从每个位置开始都向上下左右 走，然后递归走 判断是否每个 位置元素是否 相等。这里如果 借助新的辅助空间的话基本上会超时。所以用一个临时 变量记录当前遍历到的值，遍历完之后恢复。

```cpp
class Solution {
public:
    bool search(vector<vector<char>> &board,string word,int i,int x,int y)
    {
        if(i == word.size()) return true;
        int m=board.size(),n=board[0].size();
        if(x>=m || y>=n || x<0 || y<0 || board[x][y]!=word[i]) return false;
        char c = board[x][y];
        board[x][y] = '.';
        bool res = search(board,word,i+1,x-1,y) 
                || search(board,word,i+1,x+1,y)
                || search(board,word,i+1,x,y-1)
                || search(board,word,i+1,x,y+1);
        board[x][y] = c;
        return res;        
    }
    bool exist(vector<vector<char>>& board, string word) {
        if(!board.size() || !board[0].size()) return false;
        int m=board.size(),n=board[0].size();
        for(int i=0;i<m;++i)
        {
            for(int j=0;j<n;++j)
                if(search(board,word,0,i,j)) return true;
        }
        return false;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Word Search&t=blog)




