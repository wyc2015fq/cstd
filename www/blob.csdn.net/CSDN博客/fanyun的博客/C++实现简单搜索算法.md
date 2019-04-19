# C++实现简单搜索算法 - fanyun的博客 - CSDN博客
2018年09月28日 20:23:30[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：477
1、简单搜索算法要求
       给一个N行M列的矩阵，值分别为0和1，每次你可以选择将一个变成相反状态，同时，它周围的四个数也会变为相反状态。 
问：最少翻转多少次，可以将所有值都变成0 
多个解，输出翻转次数最少的（若有次数相同解，输出字典序小的） 
若无解，输出”IMPOSSIBLE”
2、思路
      对于每个点，只能有两种操作，翻或不翻，若暴力所有可能性，需要2^(M＊N)次操作，显然不可行 。所以有了这个法子。 先枚举第一行的所有可能性(2^M)，搜索或位运算均可 。然后，对坐标(i, j)来说，如果(i-1, j)不为0，那么(i, j)必然需要翻转。 重复进行上操作由2至N 。此时，最后一行也已翻转完毕，如果最后一行全为0，得出结果 第一行的所有结果中取最小值。
3、代码实现
```cpp
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <queue>
#include <string.h>
using namespace std;
int g[17][17];         //保存初始状态　
int f[17][17] = {};
int ans[17][17] = {};
int mmin = 0x1f1ffff;
bool judge(int n,int m)//判断最后一行是否全为0
{
    for(int i=1;i<=m;i++)
    {
        int t = f[n][i]+f[n][i-1]+f[n][i+1]+f[n-1][i];
        if((g[n][i]+t)&1)
            return false;
        }
    return true;
}
void dfs(int n,int m,int k,int num)
{
    if(num > mmin)//剪枝
        return;
    if(k > n)
    {
        if(judge(n, m) && mmin>num)//判断是否符合条件
        {
            memcpy(ans, f, sizeof(f));
            mmin = num;
        }
        return;
    }
    int t = 0;
    for(int i=1;i<=m;i++)
    {
        if((g[k-1][i]+f[k-2][i]+f[k-1][i-1]+f[k-1][i+1]+f[k-1][i])&1)//上一行是否为1，即是否需要翻转
        {
            f[k][i] = 1;
            t++;
        }
        else
            f[k][i] = 0;
    }
    dfs(n, m, k+1, num+t);
}
//n,m行列数   k当前列  num第一行翻转的次数
void todfs(int n, int m, int k, int num)
{
    if(k > m)
    {
        dfs(n, m, 2, num); //对第一行每种情况进行搜索
        return;
    }
    f[1][k] = 0;      //不翻转
    todfs(n, m, k+1, num);
    f[1][k] = 1;      //翻转，num＋1
    todfs(n, m, k+1, num+1);
}
int main()
{
    int n,m;
    cin>>n>>m;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)//下标从1开始，便于边界处理
            cin>>g[i][j];
    todfs(n, m, 1, 0);     //递归遍历第一行所有情况
    if(mmin == 0x1f1ffff)
        cout<<"IMPOSSIBLE"<<endl;
    else
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=m;j++)
                cout<<ans[i][j]<<" ";
            cout<<endl;
        }
    return 0;
}
```
