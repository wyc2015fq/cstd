# hdu 1394 Minimum Inversion Number（线段树） - HJ - CSDN博客
2018年08月09日 16:25:17[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：38
个人分类：[====ACM====																[--------数据结构--------																[线段树																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308065)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=1394](http://acm.hdu.edu.cn/showproblem.php?pid=1394)
题目大意：
求一个可循环数组中最小逆序数和的值
解题思路：
对于给出的数组，求出逆序数，再根据每次将首位数移到尾部，第i个数逆序数的变化为2*a[i]-n+1，循环中每次更新最小值即可。
最重要的就是如何求出逆序数？有很多种方法，归并排序、树状数组，当然，也还有线段树。
做法：
建一颗空数，每次向树中插入a[i]之前，求出[a[i]+1,n]有多少个数，即在前i-1个位置中，有多少个数比a[i]大。累加即是逆序数。
```
#include<iostream>
using namespace std;
#define lson l,m,rt<<1
#define rson m+1,r,rt<<1|1
#define N 100005
int sum[N<<2],a[N];
void PushUp(int rt)
{
    sum[rt]=sum[rt<<1]+sum[rt<<1|1];
}
void build(int l,int r,int rt)  //初始化建一颗树 
{
    sum[rt]=0;
    if(l==r)
      return ;
    int m=(l+r)>>1;
    build(lson);
    build(rson);
    PushUp(rt);
}
void update(int c,int l,int r,int rt)  //往c中插入一个值 
{
    sum[rt]++;
    if(l==r)
      return;
    int m=(l+r)>>1;
    if(c<=m)
      update(c,lson);
    else
      update(c,rson);
    PushUp(rt);
}
int query(int L,int R,int l,int r,int rt)  //查询[L,R]中的总个数 
{
    if(L<=l&&R>=r)
      return sum[rt];
    int m=(l+r)>>1;
    int res=0;
    if(L<=m)
      res+=query(L,R,lson);
    if(R>m)
      res+=query(L,R,rson);
    return res;
}
int main()
{
    int n;
    while(cin>>n)
    {
        for(int i=0;i<n;i++)
          cin>>a[i];
        build(0,n-1,1);
        int sum=0;
        for(int i=0;i<n;i++)
        {
            sum+=query(a[i],n-1,0,n-1,1);
            update(a[i],0,n-1,1);
        }
        int ans=min(0x3f3f3f3f,sum);
        for(int i=0;i<n;i++)
        {
            sum-=2*a[i]-n+1;
            ans=min(ans,sum);
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
