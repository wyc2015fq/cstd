# 剑指offer - 最小的K个数 - curryche的博客 - CSDN博客





2018年09月01日 12:08:27[curryche](https://me.csdn.net/whwan11)阅读数：21标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

最小的K个数 

时间限制：1秒 空间限制：32768K 热度指数：288073 

本题知识点： 数组
题目描述 

输入n个整数，找出其中最小的K个数。例如输入4,5,1,6,2,7,3,8这8个数字，则最小的4个数字是1,2,3,4,。

## 解法

代码 

可以改变数组时的做法，基于快排的partition函数，时间复杂度为O(n),当不能改变数组时可以采用基于multiset来做，代码还没写完，待更。

```cpp
class Solution {
public:
    vector<int> GetLeastNumbers_Solution(vector<int> input, int k) {
        vector<int> result;
        if(input.size()==0||k<=0||k>input.size())
            return result;
        int start,end,index;
        start=0;
        end=input.size()-1;
        index=partition(&input,start,end);
        while(index!=k-1)
        {
            if(index>k-1)
                index=partition(&input,start,index-1);
            else
                index=partition(&input,index+1,end);
        }
        for(int i=0;i<k;i++)
            result.push_back(input[i]);
        return result;
    }

    int partition(vector<int>* input,int start,int end)
    {
        int pivot=(*input)[start];
        int i,j;
        i=start;
        j=end;
        while(i<j)
        {
            while(i<j&&(*input)[j]>=pivot)
                j--;
            if(j>i)
                (*input)[i++]=(*input)[j];

            while(i<j&&(*input)[i]<=pivot)
                i++;
            if(j>i)
                (*input)[j--]=(*input)[i];
        }
        (*input)[i]=pivot;
        return i;
    }
};
```





