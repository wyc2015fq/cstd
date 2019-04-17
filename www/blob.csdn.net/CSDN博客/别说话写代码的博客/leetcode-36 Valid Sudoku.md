# leetcode-36. Valid Sudoku - 别说话写代码的博客 - CSDN博客





2019年01月07日 16:08:03[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：15标签：[Valid Sudoku																[leetcode																[有效的数独](https://so.csdn.net/so/search/s.do?q=有效的数独&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Determine if a 9x9 Sudoku board is valid. Only the filled cells need to be validated **according to the following rules**:- Each row must contain the digits `1-9` without repetition.- Each column must contain the digits `1-9` without repetition.- Each of the 9 `3x3` sub-boxes of the grid must contain the digits `1-9` without repetition.![](https://upload.wikimedia.org/wikipedia/commons/thumb/f/ff/Sudoku-by-L2G-20050714.svg/250px-Sudoku-by-L2G-20050714.svg.png)			A partially filled sudoku which is valid.The Sudoku board could be partially filled, where empty cells are filled with the character `'.'`.**Example 1:****Input:**[  ["5","3",".",".","7",".",".",".","."],  ["6",".",".","1","9","5",".",".","."],  [".","9","8",".",".",".",".","6","."],  ["8",".",".",".","6",".",".",".","3"],  ["4",".",".","8",".","3",".",".","1"],  ["7",".",".",".","2",".",".",".","6"],  [".","6",".",".",".",".","2","8","."],  [".",".",".","4","1","9",".",".","5"],  [".",".",".",".","8",".",".","7","9"]]**Output:** true**Example 2:****Input:**[  ["8","3",".",".","7",".",".",".","."],  ["6",".",".","1","9","5",".",".","."],  [".","9","8",".",".",".",".","6","."],  ["8",".",".",".","6",".",".",".","3"],  ["4",".",".","8",".","3",".",".","1"],  ["7",".",".",".","2",".",".",".","6"],  [".","6",".",".",".",".","2","8","."],  [".",".",".","4","1","9",".",".","5"],  [".",".",".",".","8",".",".","7","9"]]**Output:** false**Explanation:** Same as Example 1, except with the **5** in the top left corner being     modified to **8**. Since there are two 8's in the top left 3x3 sub-box, it is invalid.**Note:**- A Sudoku board (partially filled) could be valid but is not necessarily solvable.- Only the filled cells need to be validated according to the mentioned rules.- The given board contain only digits `1-9` and the character `'.'`.- The given board size is always `9x9`.|判断一个 9x9 的数独是否有效。只需要**根据以下规则**，验证已经填入的数字是否有效即可。- 数字 `1-9` 在每一行只能出现一次。- 数字 `1-9` 在每一列只能出现一次。- 数字 `1-9` 在每一个以粗实线分隔的 `3x3` 宫内只能出现一次。![](https://upload.wikimedia.org/wikipedia/commons/thumb/f/ff/Sudoku-by-L2G-20050714.svg/250px-Sudoku-by-L2G-20050714.svg.png)上图是一个部分填充的有效的数独。数独部分空格内已填入了数字，空白格用 `'.'` 表示。**示例 1:****输入:**[  ["5","3",".",".","7",".",".",".","."],  ["6",".",".","1","9","5",".",".","."],  [".","9","8",".",".",".",".","6","."],  ["8",".",".",".","6",".",".",".","3"],  ["4",".",".","8",".","3",".",".","1"],  ["7",".",".",".","2",".",".",".","6"],  [".","6",".",".",".",".","2","8","."],  [".",".",".","4","1","9",".",".","5"],  [".",".",".",".","8",".",".","7","9"]]**输出:** true**示例 2:****输入:**[  ["8","3",".",".","7",".",".",".","."],  ["6",".",".","1","9","5",".",".","."],  [".","9","8",".",".",".",".","6","."],  ["8",".",".",".","6",".",".",".","3"],  ["4",".",".","8",".","3",".",".","1"],  ["7",".",".",".","2",".",".",".","6"],  [".","6",".",".",".",".","2","8","."],  [".",".",".","4","1","9",".",".","5"],  [".",".",".",".","8",".",".","7","9"]]**输出:** false**解释:** 除了第一行的第一个数字从** 5** 改为 **8 **以外，空格内其他数字均与 示例1 相同。     但由于位于左上角的 3x3 宫内有两个 8 存在, 因此这个数独是无效的。**说明:**- 一个有效的数独（部分已被填充）不一定是可解的。- 只需要根据以上规则，验证已经填入的数字是否有效即可。- 给定数独序列只包含数字 `1-9` 和字符 `'.'` 。- 给定数独永远是 `9x9` 形式的。|
|----|----|

思路：数独每行每列每个格子 都不重复。定义三个标记的存储空间。遍历每一个元素的时候判断它在行列或格子是否重复。重复则返回false。这里需要一个将格子位置转换：[3*(i/3)+j/3][n]，表示 将原始数组i和j 坐标转化为 第几个格子的坐标。

注意c++定义二维m行n列的bool数组可以一句话搞定vector<vector<bool>> rowflag(m,vector<bool>(n,false));

```cpp
class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        int m=board.size(),n=board[0].size();
        if(m==0 || n==0) return false;
        vector<vector<bool>> rowflag(m,vector<bool>(n,false));
        vector<vector<bool>> colflag(m,vector<bool>(n,false));
        vector<vector<bool>> cellflag(m,vector<bool>(n,false));
        for(int i=0;i<m;++i)
        {
            for(int j=0;j<n;++j)
            {
                if(board[i][j]<='9' && board[i][j]>='1')
                {
                    int tmp = board[i][j]-'1';
                    if(rowflag[i][tmp] || colflag[tmp][j] ||
                      cellflag[3*(i/3)+j/3][tmp])
                        return false;
                    rowflag[i][tmp]=true;
                    colflag[tmp][j]=true;
                    cellflag[3*(i/3)+j/3][tmp] = true;
                }
            }
        }
        return true;
    }
};
```

```python
class Solution:
    def isValidSudoku(self, board):
        """
        :type board: List[List[str]]
        :rtype: bool
        """
        row= [[0]*9 for _ in range(9)]
        col= [[0]*9 for _ in range(9)]
        cell= [[0]*9 for _ in range(9)]
        for i in range(9):
            for j in range(9):
                if(board[i][j]<='9' and board[i][j]>='1'):
                    n = int(board[i][j])-1
                    pos = 3*int(i/3)+int(j/3)
                    if row[i][n]!=0 or col[n][j]!=0 or cell[pos][n]!=0:
                        return False
                    row[i][n]=1
                    col[n][j]=1
                    cell[pos][n]=1
        return True
```](https://so.csdn.net/so/search/s.do?q= Valid Sudoku&t=blog)




