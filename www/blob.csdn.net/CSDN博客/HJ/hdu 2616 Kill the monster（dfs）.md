# hdu  2616 Kill the monster（dfs） - HJ - CSDN博客
2017年06月30日 12:01:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：174
个人分类：[====ACM====																[-----------搜索-----------																[DFS																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6317663)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
There is a mountain near yifenfei’s hometown. On the mountain lived a big monster. As a hero in hometown, yifenfei wants to kill it. 
Now we know yifenfei have n spells, and the monster have m HP, when HP <= 0 meaning monster be killed. Yifenfei’s spells have different effect if used in different time. now tell you each spells’s effects , expressed (A ,M). A show the spell can cost A HP to monster in the common time. M show that when the monster’s HP <= M, using this spell can get double effect.
Input 
The input contains multiple test cases. 
Each test case include, first two integers n, m (2< n< 10, 1< m<10^7), express how many spells yifenfei has. 
Next n line , each line express one spell. (Ai, Mi).(0< Ai,Mi<=m).
Output 
For each test case output one integer that how many spells yifenfei should use at least. If yifenfei can not kill the monster output -1.
Sample Input
3 100 
10 20 
45 89 
5  40
3 100 
10 20 
45 90 
5 40
3 100 
10 20 
45 84 
5 40
Sample Output
3 
2 
-1
题目大意： 
有一个血量为m的怪物，现在由n种攻击技能，每种技能最多只能使用一次，且每种技能在怪物血量不高于该技能对应的b值的时候，技能攻击能够加倍（也就是a*2），问最少需要使用多少种技能？
解题方法： 
dfs，把使用标记blag，攻击力a，规定血量值b封装到一个结构体中，每次dfs就不断比较一下并更新ans的值。
```cpp
#include<iostream>
using namespace std;
#define maxn 10005
#define INF 0x3f3f3f3f
int n,ans;              //ans表示杀死怪物最少需要使用的技能数目，如果不能杀死怪物，值为INF 
struct node{
    int a,b;
    int blag;
}s[maxn];
void dfs(int k,int cur)
{
    if(k<=0)              //只要怪物死了之后，就不断比较更新ans的值 
      ans=ans<cur?ans:cur-1;
    for(int i=0;i<n;i++)
    {
        if(s[i].blag)
          continue;
        s[i].blag=1;
        if(k<=s[i].b)
          dfs(k-s[i].a*2,cur+1);
        else
          dfs(k-s[i].a,cur+1);
        s[i].blag=0;
    }
}
int main()
{
    int m;
    while(cin>>n>>m)
    {
        for(int i=0;i<n;i++)
        {
            cin>>s[i].a>>s[i].b;
            s[i].blag=0;
        }
        ans=INF;
        dfs(m,1);
        if(ans==INF)
          cout<<-1<<endl;
        else
          cout<<ans<<endl;
    }
    return 0;
}
```
