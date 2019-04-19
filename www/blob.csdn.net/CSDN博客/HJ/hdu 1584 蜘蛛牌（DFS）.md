# hdu  1584  蜘蛛牌（DFS） - HJ - CSDN博客
2016年08月08日 22:00:04[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：516
个人分类：[====ACM====																[-----------搜索-----------																[DFS																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6317663)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
蜘蛛牌
Problem Description 
蜘蛛牌是windows xp操作系统自带的一款纸牌游戏，游戏规则是这样的：只能将牌拖到比她大一的牌上面（A最小，K最大），如果拖动的牌上有按顺序排好的牌时，那么这些牌也跟着一起移动，游戏的目的是将所有的牌按同一花色从小到大排好，为了简单起见，我们的游戏只有同一花色的10张牌，从A到10，且随机的在一行上展开，编号从1到10，把第i号上的牌移到第j号牌上，移动距离为abs(i-j)，现在你要做的是求出完成游戏的最小移动距离。
Input 
第一个输入数据是T，表示数据的组数。 
每组数据有一行，10个输入数据，数据的范围是[1,10]，分别表示A到10，我们保证每组数据都是合法的。
Output 
对应每组数据输出最小移动距离。
Sample Input 
1 
1 2 3 4 5 6 7 8 9 10
Sample Output 
9
解析：要求最小移动距离，需要遍历，这个时候就需要用到搜索解决，但是要注意的是，要求一点点剪枝的能力，每次搜索耗费的步数需要进行对比，因为求的是最小值，所以只要当前移动耗费的步数大于以前移动的最小值就提前跳出，重新进行下一次搜索。
技巧：用数组值表示卡牌所在的位置，数组下标表示卡牌（比较难理解，需要好好推敲）
另外，在给最小值min赋初值的时候尽量把数据INF开大一点
```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
#define INF  10000000
int a[11],vis[11],ans;
void dfs(int x,int sum)   //x代表已经移动了几张牌，sum代表目前移动耗费的步数 
{
    if(sum>=ans)        //剪枝，只要移动耗费的步数大于目前的最小值，则提前跳出搜索 
      return ;
    if(x==9)           //只要移动9次就可以了，10是固定不变的 
    {
        ans=sum;
        return ;
    }
    for(int i=1;i<10;i++)
    {
        if(!vis[i])
        {
            vis[i]=1;       //对用过的数字进行标记 
            for(int j=i+1;i<=10;j++)      //用来确定i牌要移到什么位置 
            {
                if(!vis[j])   //比如要移1了，如果2,3,4,5都已经被移动过了 那么这几张牌必定叠放在6的下面，所以要移到6的位置
                {
                    dfs(x+1,sum+abs(a[j]-a[i]));   //对下一张牌继续搜索 
                    break;
                }
            }       
            vis[i]=0;    //回溯清除标记 
        }
    }
}
int main()
{
    int t,x;
    cin>>t;
    while(t--)
    {
        for(int i=0;i<10;i++)
        {
            cin>>x;
            a[x]=i;         //牌面为i的牌所在的位置
            vis[i]=0;
        }
        ans=INF;     //赋初值 
        dfs(0,0);
        cout<<ans<<endl;
    }
    return 0;
}
```
