# 剑指Offer - 二维数组中的查找 - curryche的博客 - CSDN博客





2018年07月31日 21:29:17[curryche](https://me.csdn.net/whwan11)阅读数：46标签：[剑指offer](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)









## 题目

二维数组中的查找 

时间限制：1秒 空间限制：32768K 热度指数：779869 

本题知识点： 查找
题目描述 

在一个二维数组中（每个一维数组的长度相同），每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。

## 解法

代码

```cpp
class Solution {
public:
    bool Find(int target, vector<vector<int> > array)
    {

        //if(array[0][0]>target||array.empty()||array[0].empty())
        //{
           // return false;
       // }

        int col,row;

        col=array[0].size()-1;
        row=0;

        while(col>=0&&row<array.size())
        {
            if(array[row][col]==target)
                return true;
            else if(array[row][col]<target)
            {
                row++;
            }
            else
            {
                col--;
            }
        } 
        return false;
    }
};
```




