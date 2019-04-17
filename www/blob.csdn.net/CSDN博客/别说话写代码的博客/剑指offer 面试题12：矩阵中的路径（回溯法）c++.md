# 剑指offer 面试题12：矩阵中的路径（回溯法）c++ - 别说话写代码的博客 - CSDN博客





2018年11月29日 21:26:26[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：34








题目：请设计一个函数，用来判断在一个矩阵中是否存在一条包含某字符串所有字符的路径。路径可以从矩阵中的任意一个格子开始，每一步可以在矩阵中向左，向右，向上，向下移动一个格子。如果一条路径经过了矩阵中的某一个格子，则该路径不能再进入该格子。例如 a b c e s f c s a d e e 矩阵中包含一条字符串"bcced"的路径，但是矩阵中不包含"abcb"路径，因为字符串的第一个字符b占据了矩阵中的第一行第二个格子之后，路径不能再次进入该格子。

思路：（回溯法）从每个位置开始，判断上下左右路径中字符串是否一致，可以用一个数据记录是否被访问过

```cpp
class Solution {
public:
    bool haspathcore(char *matrix,int rows,int cols,int row,int col,char *str,
                     int length,bool *visited)
    {
        if(str[length] == '\0') return true;//查找到最后一个结束
        bool haspath = false;
        if(row>=0&&rows>row &&col>=0&&cols>col &&    //不在四个边界
          matrix[row*cols+col]==str[length] &&    //元素相等
           !visited[row*cols + col])   //未被访问过
        {
            ++length;
            visited[row*cols + col] = true;
            haspath = haspathcore(matrix,rows,cols,row-1,col,str,length,visited)
                || haspathcore(matrix,rows,cols,row+1,col,str,length,visited)
                || haspathcore(matrix,rows,cols,row,col-1,str,length,visited)
                || haspathcore(matrix,rows,cols,row,col+1,str,length,visited);
            if(!haspath)
            {
                --length;
                visited[row*cols + col] = false;
            }
        } return haspath;
    }
    bool hasPath(char* matrix, int rows, int cols, char* str)
    {
        if(matrix == nullptr || rows<1 || cols<1 || str==nullptr) return false;
        bool *path = new bool[rows * cols];  //记录是否被访问过
        memset(path,0,rows*cols); //全部置为0
        int pathlength =0;
        for(int i=0;i<rows;++i)
            for(int j=0;j<cols;++j)
                if(haspathcore(matrix,rows,cols,i,j,str,pathlength,path))
                    return true;
        return false;
    }
};
```





