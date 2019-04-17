# 剑指offer - 数字在排序数组中出现的次数 - curryche的博客 - CSDN博客





2018年09月04日 19:45:34[curryche](https://me.csdn.net/whwan11)阅读数：18标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

数字在排序数组中出现的次数 

时间限制：1秒 空间限制：32768K 热度指数：158826 

本题知识点： 数组
题目描述 

统计一个数字在排序数组中出现的次数。

## 解法

代码

```cpp
class Solution {
public:
    int GetNumberOfK(vector<int> data ,int k) {
        if(data.size()==0)
            return 0;
        if(k<data[0]||k>data.back())
            return 0;

        int first_pos,last_pos;
        first_pos=getFirstOfK(&data,k);
        last_pos=getLastOfK(&data,k);

        if(first_pos>-1&&last_pos>-1)
            return last_pos-first_pos+1;
        else
            return 0;

    }

    int getFirstOfK(vector<int>* data,int k)
    {
        int i,j,middle;
        i=0;
        j=(*data).size()-1;
        while(i<j)
        {
            middle=(i+j)/2;
            if((*data)[middle]==k)
            {
                if(middle==0||(middle>0&&(*data)[middle-1]!=k))
                    return middle;
                else
                    j=middle-1;
            }

            if((*data)[middle]<k)
                i=middle+1;
            if((*data)[middle]>k)
                j=middle-1;
        }
        if((*data)[i]==k)
            return i;
        else
            return -1;
    }

    int getLastOfK(vector<int>* data,int k)
    {
        int i,j,middle;
        i=0;
        j=(*data).size()-1;
        while(i<j)
        {
            middle=(i+j)/2;
            if((*data)[middle]==k)
            {
                if(middle==(*data).size()-1||(middle<(*data).size()-1&&(*data)[middle+1]!=k))
                    return middle;
                else
                    i=middle+1;
            }
            if((*data)[middle]<k)
                i=middle+1;
            if((*data)[middle]>k)
                j=middle-1;
        }
        if((*data)[i]==k)
            return i;
        else
            return -1;
    }
};
```





