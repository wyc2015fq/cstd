# 剑指Offer - 矩阵中的路径 - curryche的博客 - CSDN博客





2018年08月08日 21:16:09[curryche](https://me.csdn.net/whwan11)阅读数：34








## 题目

矩阵中的路径 

时间限制：1秒 空间限制：32768K 热度指数：96421

题目描述 

请设计一个函数，用来判断在一个矩阵中是否存在一条包含某字符串所有字符的路径。路径可以从矩阵中的任意一个格子开始，每一步可以在矩阵中向左，向右，向上，向下移动一个格子。如果一条路径经过了矩阵中的某一个格子，则之后不能再次进入这个格子。 例如 a b c e s f c s a d e e 这样的3 X 4 矩阵中包含一条字符串”bcced”的路径，但是矩阵中不包含”abcb”路径，因为字符串的第一个字符b占据了矩阵中的第一行第二个格子之后，路径不能再次进入该格子。

## 解法

代码

```
class Solution {
public:
    bool hasPath(char* matrix, int rows, int cols, char* str)
    {
        if(matrix==NULL||rows<1||cols<1||str==NULL)
            return false;

        bool* visit=new bool[rows*cols];
        for(int i=0;i<rows*cols;i++)
            visit[i]=false;

        int count=0;
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++)
            {
                count=0;
                if(hasPath_search(matrix,rows,cols,str,i,j,count,visit))
                    return true;
            }
        }
        delete[] visit;
        return false;
    }

    bool hasPath_search(char* matrix, int rows, int cols, char* str,int row,int col,int &count,bool* visit)
    {
        if(str[count]=='\0')
            return true;

        if(matrix==NULL||rows<1||cols<1||str==NULL||row<0||row>=rows||col<0||col>=cols||visit==NULL||count<0)
            return false;


        if(matrix[cols*row+col]==str[count]&&!visit[cols*row+col])
        {
            visit[cols*row+col]=true;
            count++;
            if(hasPath_search(matrix,rows,cols,str,row-1,col,count,visit)||
               hasPath_search(matrix,rows,cols,str,row+1,col,count,visit)||
               hasPath_search(matrix,rows,cols,str,row,col-1,count,visit)||
               hasPath_search(matrix,rows,cols,str,row,col+1,count,visit))
               return true;
            else
            {
                count--;
                visit[cols*row+col]=false;
                return false;
            }

        }
        else
        {
            return false;
        }
    }

};
```



