
# 463. Island Perimeter - OraYang的博客 - CSDN博客

2017年08月17日 09:34:40[OraYang](https://me.csdn.net/u010665216)阅读数：87标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
You are given a map in form of a two-dimensional integer grid where 1 represents land and 0 represents water. Grid cells are connected horizontally/vertically (not diagonally). The grid is completely surrounded by water, and there is exactly one island (i.e.,
 one or more connected land cells). The island doesn't have "lakes" (water inside that isn't connected to the water around the island). One cell is a square with side length 1. The grid is rectangular, width and height don't exceed 100. Determine the perimeter
 of the island.
Example:
[[0,1,0,0],
 [1,1,1,0],
 [0,1,0,0],
 [1,1,0,0]]
Answer: 16
Explanation: The perimeter is the 16 yellow stripes in the image below:思路：本题思路就是遍历数组，计算出岛屿个数islands及与当前岛屿右相临与下相临的岛屿个数neighbours，最后返回4*islandas-2*neighbours
代码：

```python
class Solution {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int islands =0,neighbours = 0;
        for(int i =0;i<grid.size();i++)
            for(int j =0;j<grid[0].size();j++){
                if(grid[i][j]==1){
                    islands++;
                    if(j<grid[0].size()-1&&grid[i][j+1])
                        neighbours++;
                    if(i<grid.size()-1&&grid[i+1][j])
                        neighbours++;
                }            
            }
        return 4*islands - 2*neighbours;
    }
};
```


