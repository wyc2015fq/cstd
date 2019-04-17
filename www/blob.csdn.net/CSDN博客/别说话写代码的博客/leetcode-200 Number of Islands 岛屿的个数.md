# leetcode-200. Number of Islands 岛屿的个数 - 别说话写代码的博客 - CSDN博客





2019年03月15日 22:19:29[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：13标签：[Number of Islands																[岛屿的个数																[leetcode																[leetcode 200](https://so.csdn.net/so/search/s.do?q=leetcode 200&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Given a 2d grid map of `'1'`s (land) and `'0'`s (water), count the number of islands. An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically. You may assume all four edges of the grid are all surrounded by water.**Example 1:****Input:**11110110101100000000**Output:** 1**Example 2:****Input:**11000110000010000011**Output: **3|给定一个由 `'1'`（陆地）和 `'0'`（水）组成的的二维网格，计算岛屿的数量。一个岛被水包围，并且它是通过水平方向或垂直方向上相邻的陆地连接而成的。你可以假设网格的四个边均被水包围。**示例 1:****输入:**11110110101100000000**输出:** 1**示例 2:****输入:**11000110000010000011**输出: **3|
|----|----|

思路：其实就是求连通域的个数，直接遍历每个格子，如果有1，res++， 然后将1的4个方向连通域为1的全部置为0.相当于去掉当前连通域。最终 res就是连通域的 个数。

```cpp
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if(grid.empty() || grid[0].empty()) return 0;
        int m=grid.size(),n=grid[0].size(),res=0;
        
        for(int i=0;i<m;++i)
        {
            for(int j=0;j<n;++j)
            {
                if(grid[i][j]=='1')
                {
                    res++;
                    islands(grid,i,j);
                }
            }
        }
        return res;
    }
private:
    void islands(vector<vector<char>> &grid,int r,int c)
    {
        if(r<0 || r>=grid.size() || c<0 || c>=grid[0].size() || grid[r][c]=='0')
            return ;
        grid[r][c]='0';
        islands(grid,r-1,c);
        islands(grid,r,c-1);
        islands(grid,r+1,c);
        islands(grid,r,c+1);
    }
};
```](https://so.csdn.net/so/search/s.do?q=岛屿的个数&t=blog)](https://so.csdn.net/so/search/s.do?q=Number of Islands&t=blog)




