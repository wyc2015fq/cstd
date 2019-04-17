# 剑指offer 面试题：机器人的运动范围 - 别说话写代码的博客 - CSDN博客





2019年03月04日 11:05:11[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：28
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：地上有一个m行和n列的方格。一个机器人从坐标0,0的格子开始移动，每一次只能向左，右，上，下四个方向移动一格，但是不能进入行坐标和列坐标的数位之和大于k的格子。 例如，当k为18时，机器人能够进入方格（35,37），因为3+5+3+7 = 18。但是，它不能进入方格（35,38），因为3+5+3+8 = 19。请问该机器人能够达到多少个格子？

思路：用一个二维数组表示每个位置是否被 访问，然后查看每一个格子是否能被访问，遍历完所有情况就可以

```cpp
class Solution {
public:
    int getsum(int n)    //数字的每一位的和
    {
        int res=0;
        while(n)
        {
            res+=n%10;
            n=n/10;
        }return res;
    }
    bool check(vector<vector<bool>> &num,int threshold,int r,int c,int rows,int cols)   //检查是否满足条件
    {
        if(r>=0&&r<rows &&c>=0 && c<cols && !num[r][c] && getsum(r)+getsum(c)<= threshold)
            return true;
        return false;
    }
    int moving(vector<vector<bool>> &num,int threshold,int r,int c,int rows,int cols)
    {
        int count=0;
        if(check(num,threshold,r,c,rows,cols))   //遍历所有可能的情况
        {
            num[r][c] = true;
            count=1+ moving(num,threshold,r-1,c,rows,cols)
                   + moving(num,threshold,r+1,c,rows,cols)
                   + moving(num,threshold,r,c+1,rows,cols)
                   + moving(num,threshold,r,c-1,rows,cols);
        }return count;
    }
    int movingCount(int threshold, int rows, int cols)
    {
        if(rows<1 || cols<1 || threshold<0) return 0;
        vector<vector<bool>> num(rows,vector<bool> (cols,false));
        return moving(num,threshold,0,0,rows,cols);
        
    }
};
```





