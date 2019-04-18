# POJ 2155 Matrix 二维线段树 - weixin_33985507的博客 - CSDN博客
2019年01月08日 03:14:56[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
题目大意：一个二维坐标轴上有很多点，点不是0就是1。然后又两种操作。
C：改变以  （x1， y1）  为左上顶点   （x2,y2 ）为右下顶点 中的每一个点   0变1 1变0
Q：输出   A【x】【y】
然后每次改变都取异或  这样的话就意味着改变偶数次就变回来了  奇数次的话就是做了改变的
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#define maxn 1005
#define lson num<<1,s,mid
#define rson num<<1|1,mid+1,e
using namespace std;
bool tree[maxn<<2][maxn<<2];
int n,ans;
void buildy(int num,int s,int e,int rt)
{
    tree[rt][num]=0;
    if(s==e)return;
    int mid=(s+e)>>1;
    buildy(lson,rt);
    buildy(rson,rt);
}
void buildx(int num,int s,int e)
{
    buildy(1,1,n,num);
    if(s==e)return;
    int mid=(s+e)>>1;
    buildx(lson);
    buildx(rson);
}
void updatey(int num,int s,int e,int l,int r,int rt)
{
    if(l<=s && r>=e)
    {
        tree[rt][num]^=1;
        return ;
    }
    int mid=(s+e)>>1;
    if(r>mid)updatey(rson,l,r,rt);
    if(l<=mid)updatey(lson,l,r,rt);
}
void updatex(int num,int s,int e,int l,int r,int u,int d)
{
    if(l<=s && r>=e)
    {
        updatey(1,1,n,u,d,num);
        return;
    }
    int mid=(s+e)>>1;
    if(r>mid)
    updatex(rson,l,r,u,d);
    if(l<=mid)
    updatex(lson,l,r,u,d);
}
void queryy(int num,int s,int e,int y,int rt)
{
    ans^=tree[rt][num];
    if(s==e)return;
    int mid=(s+e)>>1;
    if(y<=mid)queryy(lson,y,rt);
    else if(y>mid)queryy(rson,y,rt);
}
void queryx(int num,int s,int e,int x,int y)
{
    queryy(1,1,n,y,num);
    if(s==e)
    {
        return;
    }
    int mid=(s+e)>>1;
    if(x>mid)queryx(rson,x,y);
    else if(x<=mid)queryx(lson,x,y);
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--)
    {
        int op;
        scanf("%d%d",&n,&op);
        buildx(1,1,n);
        while(op--)
        {
            char p[10];
            scanf("%s",p);
            if(p[0]=='C')
            {
                int x1,x2,y1,y2;
                scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
                updatex(1,1,n,x1,x2,y1,y2);
            }
            else
            {
                int a,b;
                scanf("%d%d",&a,&b);
                ans=0;
                queryx(1,1,n,a,b);
                printf("%d\n",ans);
            }
        }
        puts("");
    }
    return 0;
}
```
