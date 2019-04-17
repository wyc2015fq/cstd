# T1294	Charm Bracelet（#Ⅱ- 9 - 2） - Alex_McAvoy的博客 - CSDN博客





2018年06月04日 00:19:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：291








### 【题目描述】

有n个物品，编号为i的物品的重量为w[i]，价值为c[i]，现在要从这些物品中选一些物品装到一个容量为m的背包中，使得背包内物体在总重量不超过m的前提下价值尽量大。

### 【输入】

第1行:两个整数，n(物品数量，n≤3500)和m(背包容量,m≤12880)。

第2..n+1行：：每行二个整数w[i]，c[i]，表示每个物品的重量和价值。

### 【输出】

仅一行，一个数，表示最大总价值。

### 【输入样例】

```
4 6
1 4
2 6
3 12
2 7
```

### 【输出样例】
`23`
### 【源程序】

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 3501
#define MOD 2520
#define E 1e-12
using namespace std;
int n,m;
int w[N],c[N],f[N*5];
void ZeroOnePack(int cost,int weight)
{
    for(int v=m;v>=weight;v--)
        f[v]=max(f[v],f[v-weight]+cost);
}

int main()
{
    cin>>n>>m;
    for(int i=1;i<=n;i++)
        cin>>w[i]>>c[i];
    for(int i=1;i<=n;i++)
        ZeroOnePack(c[i],w[i]);
    cout<<f[m]<<endl;
    return 0;
}
```





