# Can you answer these queries?（HDU-4027） - Alex_McAvoy的博客 - CSDN博客





2019年02月16日 19:04:45[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：132
个人分类：[HDU																[数据结构——线段树](https://blog.csdn.net/u011815404/article/category/8669176)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

A lot of battleships of evil are arranged in a line before the battle. Our commander decides to use our secret weapon to eliminate the battleships. Each of the battleships can be marked a value of endurance. For every attack of our secret weapon, it could decrease the endurance of a consecutive part of battleships by make their endurance to the square root of it original value of endurance. During the series of attack of our secret weapon, the commander wants to evaluate the effect of the weapon, so he asks you for help. 

You are asked to answer the queries that the sum of the endurance of a consecutive part of the battleship line. 

Notice that the square root operation should be rounded down to integer.

# Input

The input contains several test cases, terminated by EOF. 

  For each test case, the first line contains a single integer N, denoting there are N battleships of evil in a line. (1 <= N <= 100000) 

  The second line contains N integers Ei, indicating the endurance value of each battleship from the beginning of the line to the end. You can assume that the sum of all endurance value is less than 2 63. 

  The next line contains an integer M, denoting the number of actions and queries. (1 <= M <= 100000) 

  For the following M lines, each line contains three integers T, X and Y. The T=0 denoting the action of the secret weapon, which will decrease the endurance value of the battleships between the X-th and Y-th battleship, inclusive. The T=1 denoting the query of the commander which ask for the sum of the endurance value of the battleship between X-th and Y-th, inclusive. 

# Output

For each test case, print the case number at the first line. Then print one line for each query. And remember follow a blank line after each test case.

# **Sample Input**

**101 2 3 4 5 6 7 8 9 1050 1 101 1 101 1 50 5 81 4 8**

# Sample Output

**Case #1:1976**


题意：每组 n 个数，m 组操作，0 a b 代表对区间 [a,b] 求平方根，1 a b 代表求区间 [a,b] 的和并输出 

思路：

对区间的更新可以转换为对区间范围内所有点的更新，但一定会超时

不过本题的更新是求根，且数据范围在 2^63，其在 7 次开方后归 1，此后继续开方均为 1，因此可以不使用延迟标记，只用一个普通的标记对树节点标记，若这个结点范围内的数为 1，那么就不需要进行区间更新

此外，操作输入处有一天坑。。。先输入的不一定是小值。。因此需要交换一下。。

输出也有一天坑，每一组输出完后有一个换行符。。。

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
    int l,r;//左右区间
    LL sum;//区间和
    int flag;//标记
}tree[N*4];
LL a[N];
LL resSum;//存储结果
void pushUp(int i){//维护子节点
    tree[i].sum=tree[i*2].sum+tree[i*2+1].sum;
    tree[i].flag=tree[i*2].flag&tree[i*2+1].flag;
}

void build(int i,int l,int r){//建树
    tree[i].l=l;
    tree[i].r=r;
    tree[i].flag=0;

    if(l==r){//叶结点
        tree[i].sum=a[l];
        if(a[l]==1)
            tree[i].flag=1;
        return;
    }

    int mid=(l+r)>>1;

    build(i*2,l,mid);//结点左儿子
    build(i*2+1,mid+1,r);//结点右儿子

    pushUp(i);
}

void updateSet(int i,int ql,int qr){//区间修改，整体赋值
    if(tree[i].l>=ql && tree[i].r<=qr)
        if(tree[i].flag==1)
            return;
    if(tree[i].l==tree[i].r){
        tree[i].sum=sqrt(tree[i].sum);
        if(tree[i].sum==1)
            tree[i].flag=1;
        return;
    }

    int mid=(tree[i].l+tree[i].r)/2;
    if(ql<=mid)
        updateSet(i*2,ql,qr);
    if(qr>mid)
        updateSet(i*2+1,ql,qr);

    pushUp(i);
}

void query(int i,int ql,int qr){//区间查询
    if(ql<=tree[i].l && tree[i].r<=qr){
        resSum+=tree[i].sum;
        return;
    }

    int mid=(tree[i].l+tree[i].r)/2;
    if(ql<=mid)
        query(i*2,ql,qr);
    if(qr>mid)
        query(i*2+1,ql,qr);

    pushUp(i);
}

int main(){
    int n;
    int Case=1;
    while(scanf("%d",&n)!=EOF&&(n)){
        for(int i=1;i<=n;i++)
            scanf("%lld",&a[i]);
        build(1,1,n);

        int m;
        scanf("%d",&m);
        printf("Case #%d:\n",Case++);
        while(m--){
            int op;
            int l,r;
            scanf("%d%d%d",&op,&l,&r);

            if(l>r)//天坑。。。
                swap(l,r);

            if(op==1){
                resSum=0;
                query(1,l,r);
                printf("%lld\n",resSum);
            }
            else{
                updateSet(1,l,r);
            }
        }
        printf("%\n");//天坑。。。
    }

    return 0;
}
```






