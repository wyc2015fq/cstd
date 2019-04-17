# Go Deeper（HDU-3715） - Alex_McAvoy的博客 - CSDN博客





2018年12月18日 14:51:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：65








> 
# Problem Description

Here is a procedure's pseudocode:

go(int dep, int n, int m)

begin

output the value of dep.

if dep < m and x[a[dep]] + x[b[dep]] != c[dep] then go(dep + 1, n, m)

end

In this code n is an integer. a, b, c and x are 4 arrays of integers. The index of array always starts from 0. Array a and b consist of non-negative integers smaller than n. Array x consists of only 0 and 1. Array c consists of only 0, 1 and 2. The lengths of array a, b and c are m while the length of array x is n. Given the elements of array a, b, and c, when we call the procedure go(0, n, m) what is the maximal possible value the procedure may output?

# **Input**

There are multiple test cases. The first line of input is an integer T (0 < T ≤ 100), indicating the number of test cases. Then T test cases follow. Each case starts with a line of 2 integers n and m (0 < n ≤ 200, 0 < m ≤ 10000). Then m lines of 3 integers follow. The i-th(1 ≤ i ≤ m) line of them are ai-1 ,bi-1 and ci-1 (0 ≤ ai-1, bi-1 < n, 0 ≤ ci-1 ≤ 2).

# Output

For each test case, output the result in a single line.

# Sample Input

**32 10 1 02 10 0 02 20 1 01 1 2**

# Sample Output

**112**


题意：给出一个递归程序：

go(int dep, int n, int m)

begin

output the value of dep.

if dep < m and x[a[dep]] + x[b[dep]]!= c[dep] then go(dep + 1, n, m)

end

其中，dep<m，数组 a、b、c 大小均是从 0 到 m，数组 x 大小是从 0 到 n-1，数组 a、b 取值是从 0 到 n-1，数组 c 的取值为 0、1、2，数组 x 的取值为 0、1，现在给出 a、b、c 对应下标的所有值，但不知道 x 的取值，现在可以指定数组 x 的值，求 dep 最大能到多少

思路：x 数组只能为 0、1，可以看做一个 2-SAT 问题，由于要求 dep 的最大值，对其进行二分即可

设当前 dep=mid，对下标 0 到 mid-1 来说，有：
- x[a[0]]+x[b[0]] != c[0]
- x[a[1]]+x[b[1]] != c[1]
-         ...
- x[a[mid-1]]+x[b[mid-1]] !=c[mid-1]

然后设置 x 的值，看能否满足前 mid 个条件，有：
- x[a]+x[b]=0：(a,0,b,1)、(b,0,a,1)
- x[a]+x[b]=1：(a,1,b,1)、(a,0,b,0)、(b,1,a,1)、(b,0,a,0)
- x[a]+x[b]=2：(a,1,b,0)、(b,1,a,0)

然后对 dep 进行二分即可

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
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
using namespace std;
bool vis[N*2];
int Stack[N*2],top;
vector<int> G[N*2];
void init(int n){
    memset(vis,false,sizeof(vis));
    for(int i=0;i<2*n;i++)
        G[i].clear();
}
void addOrClause(int x,int xVal,int y,int yVal){
    x=x*2+xVal;
    y=y*2+yVal;
    G[x^1].push_back(y);
    G[y^1].push_back(x);
}
void addAndClause(int x,int xval,int y,int yval) {
    x=x*2+xval;
    y=y*2+yval;
    G[x].push_back(y);
}
bool dfs(int x){
    if(vis[x^1])
        return false;

    if(vis[x])
        return true;
    vis[x]=true;

    Stack[top++]=x;

    for(int i=0;i<G[x].size();i++)
        if(!dfs(G[x][i]))
            return false;

    return true;
}
bool twoSAT(int n){
    for(int i=0;i<2*n;i+=2){
        if(!vis[i] && !vis[i+1]){
            top=0;

            if(!dfs(i)){
                while(top>0)
                    vis[Stack[--top]]=false;

                if(!dfs(i+1))
                    return false;
            }
        }
    }
    return true;
}
int n,m;
int a[N],b[N],c[N];
bool judge(int mid){
    init(n);
    for(int i=0;i<mid;i++){
        if(c[i]==0){//x[a]+x[b]=0
            addAndClause(a[i],0,b[i],1);
            addAndClause(b[i],0,a[i],1);
        }
        else if(c[i]==1){//x[a]+x[b]=1
            addAndClause(a[i],0,b[i],0);
            addAndClause(a[i],1,b[i],1);
            addAndClause(b[i],0,a[i],0);
            addAndClause(b[i],1,a[i],1);
        }
        else if(c[i]==2){//x[a]+x[b]=2
            addAndClause(a[i],1,b[i],0);
            addAndClause(b[i],1,a[i],0);
        }
    }
    return twoSAT(n);
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&m);
        for(int i=0;i<m;i++)
            scanf("%d%d%d",&a[i],&b[i],&c[i]);

        int left=0,right=m;
        while(left<right){
            int mid=left+(right-left+1)/2;
            if(judge(mid))
                left=mid;
            else
                right=mid-1;
        }
        printf("%d\n",left);
    }
    return 0;
}
```





