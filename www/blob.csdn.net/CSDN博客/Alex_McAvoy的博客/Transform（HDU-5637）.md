# Transform（HDU-5637） - Alex_McAvoy的博客 - CSDN博客





2019年01月14日 20:03:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59
个人分类：[HDU																[搜索——广度优先搜索 BFS](https://blog.csdn.net/u011815404/article/category/8115771)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

A list of n integers are given. For an integer x you can do the following operations:

+ let the binary representation of x be ![\overline{b_{31}b_{30}...b_0}](https://private.codecogs.com/gif.latex?%5Coverline%7Bb_%7B31%7Db_%7B30%7D...b_0%7D), you can flip one of the bits.

+ let y be an integer in the list, you can change x to x⊕y, where ⊕ means bitwise exclusive or operation.

There are several integer pairs (S,T). For each pair, you need to answer the minimum operations needed to change S to T.


# **Input**

There are multiple test cases. The first line of input contains an integer T (T≤20), indicating the number of test cases. For each test case:

The first line contains two integer n and m (1≤n≤15,1≤m≤105) -- the number of integers given and the number of queries. The next line contains n integers a1,a2,...,an (1≤ai≤105), separated by a space.

In the next m lines, each contains two integers si and ti (1≤si,ti≤105), denoting a query.

# Output

For each test cases, output an integer ![S=(\sum _{i=1}^mi*zi)\: mod\: (10^9+7)](https://private.codecogs.com/gif.latex?S%3D%28%5Csum%20_%7Bi%3D1%7D%5Emi*zi%29%5C%3A%20mod%5C%3A%20%2810%5E9&plus;7%29), where zi is the answer for i-th query.

# Sample Input

**13 31 2 33 41 23 9**

# Sample Output

**10**

————————————————————————————————————————————————

题意：t 组数据，每组给出 n 个数与 m 组询问，每组询问有 s、t 两个数，对于数 s 现给出两种变换，一种是改变 s 二进制位的某一位，即 0 变 1 或 1 变 0，另一种是让 s 从给出 n 个数当中的任意一个做异或运算，问从 s 到 t 最少要经过几步变换，最后输出每组的组号 i 与该组答案 zi 的和然后模 1E9+7

思路：假设 s^x^y^z^w^...^q=t 是最小操作次数，由于其等价于 0^x^y^z^w^...^q=s^t，因此只需要根据所给的 n 个数将 1E5 范围内的所有步数求出来存到 res[] 数组中，最后根据 s、t 的值直接可以得到 res[s^t] 然后进行计算即可。

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
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 300001
#define LL long long
using namespace std;
int a[N],res[N];
bool vis[N];
void bfs(int n){
    memset(vis,0,sizeof(vis));
    for(int i=1;i<=2e5;i*=2)//在给出的a[i]基础上向后补1、2、4、8...
        a[n++]=i;

    queue<int> Q;
    Q.push(0);
    res[0]=0;
    vis[0]=true;
    while(!Q.empty()){
        int top=Q.front();
        Q.pop();

        for(int i=0;i<n;i++){
            if(vis[a[i]^top]==false){
                Q.push(a[i]^top);//相应异或元素进队
                res[a[i]^top]=res[top]+1;//步数+1
                vis[a[i]^top]=true;//标记
            }
        }
    }
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        int n,m;
        scanf("%d%d",&n,&m);
        for(int i=0;i<n;i++)
            scanf("%d",&a[i]);

        bfs(n);
        LL sum=0;
        int s,t;
        for(int i=1;i<=m;i++){
            scanf("%d%d",&s,&t);
            sum=(sum+(LL)i*res[s^t])%MOD;
        }
        printf("%d\n",(int)sum);
    }
    return 0;
}
```






