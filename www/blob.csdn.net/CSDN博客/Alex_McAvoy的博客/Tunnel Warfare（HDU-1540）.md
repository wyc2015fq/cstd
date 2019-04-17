# Tunnel Warfare（HDU-1540） - Alex_McAvoy的博客 - CSDN博客





2019年02月16日 19:47:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
个人分类：[HDU																[数据结构——线段树](https://blog.csdn.net/u011815404/article/category/8669176)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

During the War of Resistance Against Japan, tunnel warfare was carried out extensively in the vast areas of north China Plain. Generally speaking, villages connected by tunnels lay in a line. Except the two at the ends, every village was directly connected with two neighboring ones. 

Frequently the invaders launched attack on some of the villages and destroyed the parts of tunnels in them. The Eighth Route Army commanders requested the latest connection state of the tunnels and villages. If some villages are severely isolated, restoration of connection must be done immediately! 

# Input

The first line of the input contains two positive integers n and m (n, m ≤ 50,000) indicating the number of villages and events. Each of the next m lines describes an event. 

There are three different events described in different format shown below: 

D x: The x-th village was destroyed. 

Q x: The Army commands requested the number of villages that x-th village was directly or indirectly connected with including itself. 

R: The village destroyed last was rebuilt. 

# Output

Output the answer to each of the Army commanders’ request in order on a separate line. 

# **Sample Input**

**7 9D 3D 6D 5Q 4Q 5RQ 4RQ 4**

# Sample Output

**1024**


题意：n 个村子 m 个操作，D x 代表破坏 x 号村子，Q x 代表询问包括 x 在内的最大连续区间，R 代表修复最后被破坏的村子

思路：线段树区间合并

可以利用线段树来求区间的最大值与最小值来求村子的连续区间

例如对于 5 个村子：12345，有若干个村子被毁了

现给出 Q 3，那么只需要求出来 [1,3] 的 max、[3,5] 的 min，根据两者求出连续区间即可

即：min-max-1

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct Node{
    int l,r;
    int maxx,minn;
} tree[N*4];
int history[N*4];//保存历史记录
int n,m;
void build(int i,int l,int r){//建树
    tree[i].l=l;
    tree[i].r=r;

    if(l==r){
        tree[i].minn=n+1;
        tree[i].maxx=0;
        return;
    }

    int mid=(l+r)>>1;
    build(i*2,l,mid);//结点的左儿子
    build(i*2+1,mid+1,r);//结点的右儿子

    tree[i].maxx=max(tree[i*2].maxx,tree[i*2+1].maxx);
    tree[i].minn=min(tree[i*2].minn,tree[i*2+1].minn);
}
void updateMin(int i,int id,int val){//更新最小值
    if(tree[i].l==tree[i].r){
        tree[i].minn=val;
        return;
    }

    int mid=(tree[i].l+tree[i].r)/2;
    if(id<=mid)
        updateMin(i*2,id,val);
    else
        updateMin(i*2+1,id,val);

    tree[i].minn=min(tree[i*2].minn,tree[i*2+1].minn);
}
void updateMax(int i,int id,int val){//更新最大值
    if(tree[i].l==tree[i].r){
        tree[i].maxx=val;
        return;
    }

    int mid=(tree[i].l+tree[i].r)/2;
    if(id<=mid)
        updateMax(i*2,id,val);
    else
        updateMax(i*2+1,id,val);

    tree[i].maxx=max(tree[i*2].maxx,tree[i*2+1].maxx);
}
int queryMin(int i,int ql,int qr){//查询最小值
    if(ql<=tree[i].l&&qr>=tree[i].r)//当前区间在目标区间内
        return tree[i].minn;

    int mid=(tree[i].l+tree[i].r)/2;

    int res=INF;
    if(ql<=mid)
        res=min(res,queryMin(i*2,ql,qr));
    if(qr>mid)
        res=min(res,queryMin(i*2+1,ql,qr));

    return res;
}
int queryMax(int i,int ql,int qr){//查询最大值
    if(ql<=tree[i].l&&qr>=tree[i].r)//当前区间在目标区间内
        return tree[i].maxx;

    int mid=(tree[i].l+tree[i].r)/2;

    int res=0;
    if(ql<=mid)
        res=max(res,queryMax(i*2,ql,qr));
    if(qr>mid)
        res=max(res,queryMax(i*2+1,ql,qr));

    return res;
}
int main(){

    while(scanf("%d%d",&n,&m)!=EOF){
        build(1,1,n);
        int cnt=0;
        memset(history,0,sizeof(history));
        while(m--){
            char str[5];
            scanf("%s",str);
            if(str[0]=='D'){
                int x;
                scanf("%d",&x);
                updateMax(1,x,x);//把x对应的值更新成x
                updateMin(1,x,x);//把x对应的值更新成x
                history[++cnt]=x;
            }
            else if(str[0]=='Q'){
                int x;
                scanf("%d",&x);
                int maxx=queryMax(1,1,x);
                int minn=queryMin(1,x,n);
                if(maxx==minn)//特判
                    printf("0\n");
                else
                    printf("%d\n",minn-maxx-1);
            }
            else{
                int temp=history[cnt--];
                //若恢复，将对应初始值改回
                updateMin(1,temp,n+1);
                updateMax(1,temp,0);
            }
        }
    }
    return 0;
}
```






