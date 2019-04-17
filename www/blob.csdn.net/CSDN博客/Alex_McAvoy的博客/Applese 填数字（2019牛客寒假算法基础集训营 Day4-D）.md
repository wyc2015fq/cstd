# Applese 填数字（2019牛客寒假算法基础集训营 Day4-D） - Alex_McAvoy的博客 - CSDN博客





2019年02月02日 16:42:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：172








> 
# 【题目描述】

精通程序设计的 Applese 叒写了一个游戏。

在这个游戏中，有一个 n×m 的方格，每个格子只能填入 0∼9 这 10 个数字中的一个。要求每两个相邻的格子的数字的和是个素数。

现在已经往格子里面填入了一些数字，没有填入数字的格子显示成 ?，它想知道有多少方案把这些格子填满。

# 【输入描述】

第一行两个整数 n, m。表示 n 行 m 列的格子。

接下来 n 行，每行 m 个字符表示这个方格。

数据保证已经填写数字的格子满足要求。

1≤n,m≤6

# 【输出描述】

输出一个整数表示方案数。

# 【样例】

示例1

输入

2 2

1?

23

输出

2


思路：轮廓线 DP

用 dp[x][sta] 表示第 x 个格子，其所在轮廓线上所有数字的十进制状态为 sta 的方案数，其中 sta 的最高位存其前一行的格子，最低位存其前一列的格子，如下图，状态转移就是一个带溢出的十进制左移并补上填的数字

![](https://img-blog.csdnimg.cn/20190202164135325.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# 【源代码】

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
#define N 1000001
#define LL long long
using namespace std;
bool isprime[]={0,0,1,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0};//isprime[i]代表从0-18的第i个数是否为素数
LL dp[N][2];
char G[10][10];
int main(){
    int n,m;
    scanf("%d%d",&n,&m);
    for(int i=0;i<n;i++)
        scanf("%s", G[i]);

    int num=1;//记录状态总数
    for(int i=1;i<m;i++)
        num*=10;

    dp[0][0]=1;
    int sta=0;

    for(int i=0;i<n;i++){//枚举行
        for(int j=0;j<m;j++){//枚举列
            for(int x=0;x<num*10;x++){//枚举状态
                if(dp[x][sta]){
                    for(int dec=0;dec<=9;dec++){//枚举0-9数字
                        if( (G[i][j]=='?'||G[i][j]==dec+'0') && (!i||isprime[dec+x/num]) && (!j||isprime[dec+x%10]) ){//最高位存其前一行的格子，最低位存其前一列的格子，
                            dp[x%num*10+dec][sta^1]+=dp[x][sta];
                        }
                    }
                    dp[x][sta]=0;//滚动掉数组第一维
                }
            }
            sta=!sta;
        }
    }
    
    LL res=0;
    for(int i=0;i<num*10;i++)
        res+=dp[i][sta];
    printf("%lld\n",res);

    return 0;
}
```





