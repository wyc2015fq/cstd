# Backward Digit Sums（POJ-3187） - Alex_McAvoy的博客 - CSDN博客





2018年07月21日 15:56:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：49








# Problem Description

FJ and his cows enjoy playing a mental game. They write down the numbers from 1 to N (1 <= N <= 10) in a certain order and then sum adjacent numbers to produce a new list with one fewer number. They repeat this until only a single number is left. For example, one instance of the game (when N=4) might go like this: 



    3   1   2   4

      4   3   6

        7   9

         16

Behind FJ's back, the cows have started playing a more difficult game, in which they try to determine the starting sequence from only the final total and the number N. Unfortunately, the game is a bit above FJ's mental arithmetic capabilities. 

Write a program to help FJ play the game and keep up with the cows.

# **Input**

Line 1: Two space-separated integers: N and the final sum.

# Output

Line 1: An ordering of the integers 1..N that leads to the given sum. If there are multiple solutions, choose the one that is lexicographically least, i.e., that puts smaller numbers first.

# Sample Input

**4 16**

# Sample Output

**3 1 2 4**

————————————————————————————————————————————————————

思路：刚做过的洛谷原题。具体思路：[点击这里](https://blog.csdn.net/u011815404/article/details/80904045)

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 21
#define MOD 123
#define E 1e-6
using namespace std;
int n,sum;
int triangle[N][N];
int a[N],vis[N];
bool flag;
void dfs(int step,int cnt)
{
    if(flag)
        return;
    if(cnt>sum)//当前和大于sum，剪枝
        return;
    if(step==n+1&&cnt==sum)//达到最后一层并找到答案
    {
        flag=true;
        for(int i=1;i<=n;i++)
            cout<<a[i]<<" ";
    }
    for(int i=1;i<=n;i++)
        if(!vis[i])
        {
            a[step]=i;
            vis[i]=1;
            cnt+=triangle[n][step]*a[step];//加上i*系数
 
            dfs(step+1,cnt);
 
            cnt-=triangle[n][step]*a[step];//减去i*系数
            vis[i]=0;
        }
}
int main()
{
    cin>>n>>sum;
 
    /*构造存储答案系数的杨辉三角*/
    triangle[1][1]=1;
    for(int i=2;i<=n;i++)
        for(int j=1;j<=i;j++)
            triangle[i][j]=triangle[i-1][j-1]+triangle[i-1][j];
 
    dfs(1,0);//从1开始搜索
 
    return 0;
}
```





