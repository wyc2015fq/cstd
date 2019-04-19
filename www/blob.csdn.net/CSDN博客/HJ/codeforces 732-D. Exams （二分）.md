# codeforces 732-D. Exams  （二分） - HJ - CSDN博客
2018年08月11日 20:18:11[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：54
个人分类：[====ACM====																[-----------搜索-----------																[二分&三分																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238038)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/732/D](http://codeforces.com/problemset/problem/732/D)
题目大意：
一个人有m门科目需要考试，每一门科目需要a[i]（1<=i<=m）的复习时间，并且有份n天的考试安排表，其中d[i]（1<=i<=n）假如为0，就代表这一天没有任何科目的考试。试求这个人最少在第几天顺利通过所有考试？
解题思路：
想了好久的贪心，排序各种做法，总是能被一些数据推翻，最后才想到用二分，二分查找出最小天数，其中对于二分查找的mid进行判断即可。
```cpp
#include<iostream>
#include<algorithm>
#include<vector>
#include<cstring>
#include<cstdio>
using namespace std;
typedef long long ll;
#define mod 1000000007
const int maxn=1e5+10;
int n,m,d[maxn],a[maxn];
bool vis[maxn];
bool Judge(int x)   //判断第x天是否满足 
{
    memset(vis,false,sizeof(vis));
    int cnt=x-1,p=0;
    for(int i=x;i>0;i--)
    {
        cnt=min(cnt,i-1);
        if(d[i]&&!vis[d[i]]&&a[d[i]]<=cnt)
          cnt-=a[d[i]]+1,vis[d[i]]=true,p++;
    }
    return p==m;
}
int search()    //二分查找满足所有科目都能复习完并且考试通过的最小的天数 
{
    int l=1,r=n;
    while(l<=r)
    {
        int mid=(l+r)/2;
        if(Judge(mid))
          r=mid-1;
        else
          l=mid+1;
    }
    return l>n?-1:l;
}
int main()
{
    while(cin>>n>>m)
    {
        for(int i=1;i<=n;i++)
          cin>>d[i];
        for(int i=1;i<=m;i++)
          cin>>a[i];
        cout<<search()<<endl;
    }
    return 0;
}
```
