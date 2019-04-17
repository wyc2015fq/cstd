# hdu1978——How many ways - westbrook1998的博客 - CSDN博客





2018年05月25日 16:14:22[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42标签：[动态规划](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
这是一个简单的生存游戏，你控制一个机器人从一个棋盘的起始点(1,1)走到棋盘的终点(n,m)。游戏的规则描述如下：  

  1.机器人一开始在棋盘的起始点并有起始点所标有的能量。  

  2.机器人只能向右或者向下走，并且每走一步消耗一单位能量。  

  3.机器人不能在原地停留。  

  4.当机器人选择了一条可行路径后，当他走到这条路径的终点时，他将只有终点所标记的能量。  
![这里写图片描述](https://odzkskevi.qnssl.com/5a8e6a03898bb4a102b490ef232237bf?v=1527232798)

  如上图，机器人一开始在(1,1)点，并拥有4单位能量，蓝色方块表示他所能到达的点，如果他在这次路径选择中选择的终点是(2,4)  

  点，当他到达(2,4)点时将拥有1单位的能量，并开始下一次路径选择，直到到达(6,6)点。  

  我们的问题是机器人有多少种方式从起点走到终点。这可能是一个很大的数，输出的结果对10000取模。 

  Input 

  第一行输入一个整数T,表示数据的组数。  

  对于每一组数据第一行输入两个整数n,m(1 <= n,m <= 100)。表示棋盘的大小。接下来输入n行,每行m个整数e(0 <= e < 20)。 

  Output 

  对于每一组数据输出方式总数对10000取模的结果. 

  Sample Input 

  1 

  6 6 

  4 5 6 6 4 3 

  2 2 3 1 7 2 

  1 1 4 6 2 7 

  5 8 4 3 9 5 

  7 6 6 2 1 5 

  3 1 1 3 7 2 

  Sample Output 

  3948
稍微有点难度的动态规划，因为数据量比较小，所以可以直接暴力遍历，找出a[i][j]能到的所有位置，更新这些位置的dp，注意边界和0的判断

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=110;
const int MOD=10000;
int dp[MAXN][MAXN];
int a[MAXN][MAXN];
int t,n,m;
int main(void){
    scanf("%d",&t);
    while(t--){
        memset(dp,0,sizeof(dp));
        dp[1][1]=1;
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++){
            for(int j=1;j<=m;j++){
                scanf("%d",&a[i][j]);
            }
        }
        for(int i=1;i<=n;i++){
            for(int j=1;j<=m;j++){
                if(dp[i][j]){
                    int tmp=a[i][j];
                    for(int x=0;x<=tmp;x++){
                        for(int y=0;y<=tmp;y++){
                            if(x+y==0){
                                continue;
                            }
                            if(x+y>tmp){
                                break;      
                            }
                            if(x+i<=n && y+j<=m){
                                dp[i+x][j+y]=(dp[i+x][j+y]+dp[i][j])%MOD;
                            }
                        }
                    }
                }

            }
        }
        printf("%d\n",dp[n][m]%MOD);
    }
    return 0;
}
```





