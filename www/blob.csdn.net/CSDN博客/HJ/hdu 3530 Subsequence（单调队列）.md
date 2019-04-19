# hdu  3530 Subsequence（单调队列） - HJ - CSDN博客
2017年08月16日 17:20:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：203
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------数据结构--------																[单调队列&单调栈](https://blog.csdn.net/feizaoSYUACM/article/category/7238094)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=3530](http://acm.hdu.edu.cn/showproblem.php?pid=3530)
题目是要求一段区间内最大值与最小值的差不低于m不超过k，那么维护一个递增的单调队列和一个递减的单调队列。凡是差值超过k的就往前挪动，满足条件之后每次得到的一个区间，如果符合差值不低于m的条件，更新区间长度的ans值就行了。
```cpp
#include<iostream>
using namespace std;
#define maxn 100010
int n,m,k,q_max[maxn],q_min[maxn],a[maxn];
int main()
{
    while(cin>>n>>m>>k)
    {
        for(int i=1;i<=n;i++)
          cin>>a[i];
        int l1=0,r1=0,l2=0,r2=0,ans=0,pos=0;
        for(int i=1;i<=n;i++)
        {
            while(r1>l1&&a[q_max[r1-1]]<=a[i])          //维护一个递减的单调队列 
              r1--;            
            q_max[r1++]=i;
            while(r2>l2&&a[q_min[r2-1]]>=a[i])          //维护一个递增的单调队列 
              r2--;                    
            q_min[r2++]=i;
            while(r1>l1&&r2>l2&&a[q_max[l1]]-a[q_min[l2]]>k)    //如果max-min>k 每次更新位置较小的那一个 
            {
                if(q_max[l1]<q_min[l2])
                  pos=q_max[l1++];
                else
                  pos=q_min[l2++];
            }
            if(r1>l1&&r2>l2&&a[q_max[l1]]-a[q_min[l2]]>=m)     //比较每次符合max-min>=m条件的区间大小 更新ans值 
              ans=max(ans,i-pos);
        }   
        cout<<ans<<endl;
    }
    return 0;
}
```
