# Tree（HDU-5060） - Alex_McAvoy的博客 - CSDN博客





2019年01月14日 21:36:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：38
个人分类：[HDU																[数据结构——并查集](https://blog.csdn.net/u011815404/article/category/8376617)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

There is a tree(the tree is a connected graph which contains n points and n−1 edges),the points are labeled from 1 to n,which edge has a weight from 0 to 1,for every point i∈[1,n],you should find the number of the points which are closest to it,the clostest points can contain i itself.

# **Input**

the first line contains a number T,means T test cases.

for each test case,the first line is a nubmer n,means the number of the points,next n-1 lines,each line contains three numbers u,v,w,which shows an edge and its weight.

T≤50,n≤105,u,v∈[1,n],w∈[0,1]

# Output

for each test case,you need to print the answer to each point.

in consideration of the large output,imagine ansi is the answer to point i,you only need to output,ans1 xor ans2 xor ans3.. ansn.

# Sample Input

**131 2 02 3 1**

# Sample Output

**1**

# **Hint**

**in the sample.**
- **ans_1=2**
- **ans_2=2**
- **ans_3=1**

**2~xor~2~xor~1=1,so you need to output 1.**


————————————————————————————————————————————————

题意：给出 t 组数据，每组数据给出一个整数 n 代表有 n 个点 n-1 条边，然后依次给出 n-1 条边及其权值（仅有 0、1），要统计每个点距离他最近的点的个数（包括其自身），最后将每个点距离他最近的个数异或后输出

思路：由于要统计距离每个点最近的点的个数，且边的权值仅有 0、1，因此可以考虑当边权为 0 时，使用并查集将点连接，边权为 1 时，不进行任何操作，这样将所有的点可划分为多个连通块，然后统计每个点所在的连通块中的点的个数即可，最后将所有点的值异或输出。

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
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1000001
#define LL long long
using namespace std;
int father[N];
int num[N];
int Find(int x){
    while(father[x]!=x)
        x=father[x];
    return x;
}
void Union(int x,int y){
    x=Find(x);
    y=Find(y);
    if(x!=y)
        father[x]=y;

    num[y]+=num[x];
}
int main(){
    int t;
    scanf("%d",&t);

    while(t--){
        int n;
        scanf("%d",&n);

        memset(num,0,sizeof(num));
        for(int i=1;i<=n;i++)
            father[i]=i;

        int m=n-1;
        for(int i=0;i<m;i++){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            if(w==0)
                Union(x,y);
        }

        for(int i=1;i<=n;i++){//枚举每个点
            father[i]=Find(i);//寻找每个点的父节点
            num[father[i]]++;//统计子节点个数（包括其自身）
        }
        for(int i=1;i<=n;i++)//统计父节点外的点的个数
            num[i]=num[father[i]];

        int res=0;
        for(int i=1;i<=n;i++)//将所有点的值异或
            res^=num[father[i]];

        printf("%d\n",res);
    }

    return 0;
}
```






