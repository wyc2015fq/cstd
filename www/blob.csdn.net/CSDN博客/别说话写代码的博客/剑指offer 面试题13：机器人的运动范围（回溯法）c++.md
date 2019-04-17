# 剑指offer 面试题13：机器人的运动范围（回溯法）c++ - 别说话写代码的博客 - CSDN博客





2018年11月29日 21:49:31[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：42








题目：地上有一个m行和n列的方格。一个机器人从坐标0,0的格子开始移动，每一次只能向左，右，上，下四个方向移动一格，但是不能进入行坐标和列坐标的数位之和大于k的格子。 例如，当k为18时，机器人能够进入方格（35,37），因为3+5+3+7 = 18。但是，它不能进入方格（35,38），因为3+5+3+8 = 19。请问该机器人能够达到多少个格子？

思路：（回溯法）可以 前后左右遍历路径，查看可否访问。我们可以用 一个数组来记录访问路径

```cpp
class Solution {
public:
    int digitsum(int n)
    {
        int sum =0;
        while(n)
        {
            sum += n%10;
            n=n/10;
        }return sum;
    }
    bool checking(int threshold,int rows,int cols,int row,int col,bool* visited)
    {
        if(rows>row&&row>=0 &&cols>col&&col>=0 &&!visited[row*cols + col] 
          && digitsum(col)+digitsum(row)<= threshold)
            return true;
        return false;
    }
    int moving(int threshold,int rows,int cols,int row,int col,bool* visited)
    {
        int count = 0;
        if(checking(threshold,rows,cols,row,col,visited))
        {
            visited[row*cols + col] = true;
            count = 1 + moving(threshold,rows,cols,row-1,col,visited)
                + moving(threshold,rows,cols,row+1,col,visited)
                + moving(threshold,rows,cols,row,col-1,visited)
                + moving(threshold,rows,cols,row,col+1,visited);
        }return count;
    }
    int movingCount(int threshold, int rows, int cols)
    {
        if(threshold<0 || rows<1 || cols<1) return 0;
        bool *visited = new bool[rows*cols];
        memset(visited,0,rows*cols);
        return moving(threshold,rows,cols,0,0,visited);
    }
};
```





