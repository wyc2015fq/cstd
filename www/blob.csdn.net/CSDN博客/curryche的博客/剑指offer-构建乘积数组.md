# 剑指offer - 构建乘积数组 - curryche的博客 - CSDN博客





2018年09月23日 11:06:10[curryche](https://me.csdn.net/whwan11)阅读数：127







- 题目

构建乘积数组

时间限制：1秒 空间限制：32768K 热度指数：80682

本题知识点： 数组

算法知识视频讲解

题目描述

给定一个数组A[0,1,…,n-1],请构建一个数组B[0,1,…,n-1],其中B中的元素B[i]=A[0]*A[1]*…*A[i-1]*A[i+1]*…*A[n-1]。不能使用除法。- 解法

代码

```
class Solution {
public:
    vector<int> multiply(const vector<int>& A) {
        if(A.size()==0||A.size()==1)
        {
            vector<int> result;
            return result;
        }

        vector<int> result(A.size(),1);
        int tmp;
        
        for(int i=1;i<A.size();i++)
            result[i]=result[i-1]*A[i-1];
        
        tmp=1;
        for(int i=A.size()-1;i>=0;i--)
        {
            result[i]*=tmp;
            tmp*=A[i];
        }
 
        return result;
    }
};
```




