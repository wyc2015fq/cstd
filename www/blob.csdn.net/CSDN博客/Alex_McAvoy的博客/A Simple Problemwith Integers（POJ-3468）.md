# A Simple Problemwith Integers（POJ-3468） - Alex_McAvoy的博客 - CSDN博客





2019年02月16日 15:14:01[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：22








> 
# Problem Description

You have N integers, A1, A2, ... , AN. You need to deal with two kinds of operations. One type of operation is to add some given number to each number in a given interval. The other is to ask for the sum of numbers in a given interval.

# Input

The first line contains two numbers N and Q. 1 ≤ N,Q ≤ 100000.

The second line contains N numbers, the initial values of A1, A2, ... , AN. -1000000000 ≤ Ai ≤ 1000000000.

Each of the next Q lines represents an operation.

"C a b c" means adding c to each of Aa, Aa+1, ... , Ab. -10000 ≤ c ≤ 10000.

"Q a b" means querying the sum of Aa, Aa+1, ... , Ab.

# Output

You need to answer all Q commands in order. One answer in a line.

# **Sample Input**

**10 51 2 3 4 5 6 7 8 9 10Q 4 4Q 1 10Q 2 4C 3 6 3Q 2 4**

# Sample Output

**455915**


题意：n 个数 m 个操作，Q a b 表示输出 [a,b] 区间内的和，C a b c 表示将 [a,b] 区间内所有的值都加上 c

思路：线段树模板题，区间更新+区间查询，注意使用 long long

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
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;

LL sum[N*4];
LL add[N*4];
#define lson i*2,l,mid
#define rson i*2+1,mid+1,r

void pushUp(int i){//收集子节点信息
    sum[i]=sum[i*2]+sum[i*2+1];
}
/*

void pushUp(int i,int num){//收集子节点信息
    sum[i]=sum[i*2]+sum[i*2+1];
    sum[i]+=add[i*2]*(num+1)/2+add[i*2+1]*(num-(num+1)/2);
}

*/
void pushDown(int i,int num){//将i节点的延迟标记下传，更新sum[i]
    if(add[i]){
        //sum[i]+=add[i]*num;

        /**/
        sum[i*2]+=add[i]*(num-(num/2));
        sum[i*2+1]+=add[i]*(num/2);
        /**/

        add[i*2]+=add[i];
        add[i*2+1]+=add[i];
        add[i]=0;
    }
}
void build(int i,int l,int r){//建树
    add[i]=0;

    if(l==r){
        scanf("%lld",&sum[i]);
        return;
    }

    int mid=(l+r)/2;
    build(lson);
    build(rson);

    //pushUp(i,r-l+1);
    pushUp(i);
}
void updateInterval(int ql,int qr,int val,int i,int l,int r){//区间更新，使区间[ql,qr]整体+val
    if(ql<=l&&r<=qr){
        add[i]+=val;
        /**/
        sum[i]+=(LL)val*(r-l+1);
        /**/
        return;
    }
    pushDown(i,r-l+1);//标记下传

    int mid=(l+r)/2;
    if(ql<=mid)
        updateInterval(ql,qr,val,lson);
    if(mid<qr)
        updateInterval(ql,qr,val,rson);

    pushUp(i);
    //pushUp(i,r-l+1);
}
LL queryInterval(int ql,int qr,int i,int l,int r){//区间查询，在区间[l,r]内查询区间[ql,qr]间的目标值
    if(ql<=l&&r<=qr){
        //pushDown(i,r-l+1);
        return sum[i];
    }

    pushDown(i,r-l+1);

    int mid=(l+r)/2;
    LL res=0;
    if(ql<=mid)
        res+=queryInterval(ql,qr,lson);
    if(mid<qr)
        res+=queryInterval(ql,qr,rson);

    return res;
}
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        build(1,1,n);
        while(m--){
            char str[5];
            scanf("%s",str);
            if(str[0]=='Q'){
                int x,y;
                scanf("%d%d",&x,&y);
                printf("%lld\n",queryInterval(x,y,1,1,n));
            }
            else{
                int x,y,val;
                scanf("%d%d%d",&x,&y,&val);
                updateInterval(x,y,val,1,1,n);
            }
        }
    }
    return 0;
}
```





