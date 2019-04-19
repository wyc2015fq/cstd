# hdu  3308  LCIS  （线段树区间合并） - HJ - CSDN博客
2017年01月26日 22:54:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：271
Problem Description 
Given n integers. 
You have two operations: 
U A B: replace the Ath number by B. (index counting from 0) 
Q A B: output the length of the longest consecutive increasing subsequence (LCIS) in [a, b].
Input 
T in the first line, indicating the case number. 
Each case starts with two integers n , m(0< n,m<=105). 
The next line has n integers(0<=val<=105). 
The next m lines each has an operation: 
U A B(0<=A,n , 0<=B=105) 
OR 
Q A B(0<=A<=B< n).
Output 
For each Q, output the answer.
Sample Input
1 
10 10 
7 7 3 3 5 9 9 8 1 8  
Q 6 6 
U 3 4 
Q 0 1 
Q 0 5 
Q 4 7 
Q 3 5 
Q 0 2 
Q 4 6 
U 6 10 
Q 0 9
Sample Output
1 
1 
4 
2 
3 
1 
2 
5
题目大意： 
求出查询一栏中给定的区间内最长连续递增序列的长度
解题思路： 
因为同时要更新数据，因此需要建树，进行基础的区间合并，没有坑点，适合作为模板。
```
#include<iostream>
using namespace std;
#define imax 1000005
int n,m,s[imax];
struct node{
    int l,r,len;      //左右边界与连续的长度 
    int ln,rn;       //左右边界的值 
    int ls,rs,ms;    //左右最大的LICS和区间最大的LICS 
}f[imax*3];
void pushup(int t)    //区间合并处理 
{
    f[t].ls=f[t*2].ls;
    f[t].rs=f[t*2+1].rs;
    f[t].ln=f[t*2].ln;
    f[t].rn=f[t*2+1].rn;
    f[t].ms=max(f[t*2].ms,f[t*2+1].ms);    //更新当前节点的区间最大LICS 
    if(f[t*2].rn<f[t*2+1].ln)   //如果左子树的右边界小于右子树的左边界值，要合并左子树的右边界和右子树的左边界进行计算 
    {
        if(f[t*2].ls==f[t*2].len)    //如果左子树的LICS长度等于左子树区间长度 
          f[t].ls+=f[t*2+1].ls;           //那么就可以把右子树的左子树最大的LICS值累加到左子树的ls值中 
        if(f[t*2+1].rs==f[t*2+1].len)   //右子树亦然 
          f[t].rs+=f[t*2].rs;
        f[t].ms=max(f[t].ms,f[t*2].rs+f[t*2+1].ls);   //然后再更新一次当前节点的区间最大LICS 
    }
}
void build(int t,int l,int r)
{
    f[t].l=l;
    f[t].r=r;
    f[t].len=r-l+1;
    if(l==r)
    {
        f[t].ln=f[t].rn=s[l];
        f[t].ls=f[t].rs=f[t].ms=1;   //建树赋值 
        return ;
    }
    int mid=(l+r)/2;
    build(t*2,l,mid);
    build(t*2+1,mid+1,r);
    pushup(t); 
} 
void insert(int t,int num,int c)
{
    if(f[t].l==f[t].r)
    {
        f[t].ln=f[t].rn=c;
        return ;
    } 
    int mid=(f[t].l+f[t].r)/2;
    if(num<=mid)  
      insert(t*2,num,c);
    else
      insert(t*2+1,num,c);
    pushup(t);
}
int query(int t,int l,int r)
{
    if(f[t].l>=l&&f[t].r<=r)
      return f[t].ms;
    int mid=(f[t].l+f[t].r)/2,ans=0;
    if(l<=mid)         //如果区间包含当前节点左子树的一部分值 
      ans=max(ans,query(t*2,l,r));    //深入当前节点的左子树查询 
    if(r>mid)          //如果区间包含当前节点右子树的一部分值 
      ans=max(ans,query(t*2+1,l,r));    //深入当前节点的右子树查询 
    if(f[t*2].rn<f[t*2+1].ln)     //如果左子树的右边界小于右子树的左边界值，
      ans=max(ans,min(mid-l+1,f[t*2].rs)+min(r-mid,f[t*2+1].ls));     //更新一次ans的值 
    return ans;
}
int main()
{
    int t,l,r,a,b;
    char ss[10];
    cin>>t;
    while(t--)
    {
        cin>>n>>m;
        for(int i=1;i<=n;i++)
          cin>>s[i];
        build(1,1,n);
        while(m--)
        {
            cin>>ss>>a>>b;
            if(ss[0]=='Q')
              cout<<query(1,a+1,b+1)<<endl;
            else
              insert(1,a+1,b);
        } 
    }
    return 0; 
}
```
