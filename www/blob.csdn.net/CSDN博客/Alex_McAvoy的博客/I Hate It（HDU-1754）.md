# I Hate It（HDU-1754） - Alex_McAvoy的博客 - CSDN博客





2019年02月16日 14:27:02[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：19
个人分类：[HDU																[数据结构——线段树](https://blog.csdn.net/u011815404/article/category/8669176)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

很多学校流行一种比较的习惯。老师们很喜欢询问，从某某到某某当中，分数最高的是多少。 

这让很多学生很反感。 

不管你喜不喜欢，现在需要你做的是，就是按照老师的要求，写一个程序，模拟老师的询问。当然，老师有时候需要更新某位同学的成绩。

# Input

本题目包含多组测试，请处理到文件结束。 

在每个测试的第一行，有两个正整数 N 和 M ( 0<N<=200000,0<M<5000 )，分别代表学生的数目和操作的数目。 

学生ID编号分别从1编到N。 

第二行包含N个整数，代表这N个学生的初始成绩，其中第i个数代表ID为i的学生的成绩。 

接下来有M行。每一行有一个字符 C (只取'Q'或'U') ，和两个正整数A，B。 

当C为'Q'的时候，表示这是一条询问操作，它询问ID从A到B(包括A,B)的学生当中，成绩最高的是多少。 

当C为'U'的时候，表示这是一条更新操作，要求把ID为A的学生的成绩更改为B。 

# Output

对于每一次询问操作，在一行里面输出最高成绩。

# **Sample Input**

**5 61 2 3 4 5Q 1 5U 3 6Q 3 4Q 4 5U 2 9Q 1 5**

# Sample Output

**5659**


思路：线段树模板题，求最值，要求可单点更新，区间查询

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
#define N 200001
#define LL long long
const int MOD=20091226;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;

int maxx[N*4];
#define lson i*2,l,mid
#define rson i*2+1,mid+1,r

void pushUp(int i){
    maxx[i]=max(maxx[i*2],maxx[i*2+1]);
}
void build(int i,int l,int r){
    if(l==r){
        scanf("%d",&maxx[i]);
        return;
    }

    int mid=(l+r)/2;
    build(lson);
    build(rson);
    pushUp(i);
}

int queryInterval(int ql,int qr,int i,int l,int r){
    if(ql<=l&&r<=qr)
        return maxx[i];

    int mid=(l+r)/2;
    int res=-INF;
    if(ql<=mid)
        res=max(res,queryInterval(ql,qr,lson));
    if(mid<qr)
        res=max(res,queryInterval(ql,qr,rson));

    return res;
}

void updatePoint(int id,int val,int i,int l,int r){
    if(l==r){
        maxx[i]=val;
        return;
    }
    int mid=(l+r)/2;
    if(id<=mid)
        updatePoint(id,val,lson);
    else
        updatePoint(id,val,rson);

    pushUp(i);
}

int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF){
        build(1,1,n);
        while(m--){
            char str[5];
            int x,y;
            scanf("%s%d%d",str,&x,&y);
            if(str[0]=='Q')
                printf("%d\n",queryInterval(x,y,1,1,n));
            else
                updatePoint(x,y,1,1,n);
        }
    }
    return 0;
}
```






