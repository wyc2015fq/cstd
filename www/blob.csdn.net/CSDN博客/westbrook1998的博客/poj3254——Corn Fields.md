# poj3254——Corn Fields - westbrook1998的博客 - CSDN博客





2018年08月25日 18:33:30[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：27标签：[状压dp																[dp](https://so.csdn.net/so/search/s.do?q=dp&t=blog)](https://so.csdn.net/so/search/s.do?q=状压dp&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Farmer John has purchased a lush new rectangular pasture composed of M by N (1 ≤ M ≤ 12; 1 ≤ N ≤ 12) square parcels. He wants to grow some yummy corn for the cows on a number of squares. Regrettably, some of the squares are infertile and can’t be planted. Canny FJ knows that the cows dislike eating close to each other, so when choosing which squares to plant, he avoids choosing squares that are adjacent; no two chosen squares share an edge. He has not yet made the final choice as to which squares to plant. 

  Being a very open-minded man, Farmer John wants to consider all possible options for how to choose the squares for planting. He is so open-minded that he considers choosing no squares as a valid option! Please help Farmer John determine the number of ways he can choose the squares to plant. 

  Input 

  Line 1: Two space-separated integers: M and N  

  Lines 2.. M+1: Line i+1 describes row i of the pasture with N space-separated integers indicating whether a square is fertile (1 for fertile, 0 for infertile) 

  Output 

  Line 1: One integer: the number of ways that FJ can choose the squares modulo 100,000,000. 

  Sample Input 

  2 3 

  1 1 1 

  0 1 0 

  Sample Output 

  9 

  Hint 

  Number the squares as follows:  

  1 2 3 

    4 

  There are four ways to plant only on one squares (1, 2, 3, or 4), three ways to plant on two squares (13, 14, or 34), 1 way to plant on three squares (134), and one way to plant on no squares. 4+3+1+1=9.
状压dp的入门题

代码：

```cpp
#include <cstdio>
#include <algorithm>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int MOD=100000000;
const int N=1<<13;
//可放的位置状态
int map[N];
//每一行的状态（用一个二进制数表示一整行）
int st[N];
//表示第i行j状态下能放牛的情况种数
int dp[N][N];
//判断是否跟右边相邻的冲突
bool check(int x){
    return (x&(x<<1));
}
//地的状态是以不可放的地二进制标记为1来表示
//因此地和放的状态进行按位与 会有00 01 10 11
//其中00表示地可以放但没放 01表示地可以放而且放了
//10表示地不可以放而且没放，这三种按位与都返回0
//即冲突返回false
bool judge(int i,int x){
    return (map[i]&st[x]);
}
int main(void){
    //freopen("data.txt","r",stdin);
    int n,m,x;
    scanf("%d%d",&n,&m);
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            scanf("%d",&x);
            //计算出地的状态
            if(x==0){
                map[i]+=(1<<j);
            }
        }
    }
    int k=0;
    //一行有m个格子，2进制数可以有2^m个,计算出符合条件（不相邻的）
    //每一行的暂时可用状态（未考虑地的状态，只考虑相邻）
    for(int i=0;i<(1<<m);i++){
        if(!check(i)){
            st[k++]=i;
            //000 001 010 100 101
        }
    }
    //枚举第一行可能的状态
    for(int i=0;i<k;i++){
        //与地的状态进行判断
        if(!judge(0,i)){
            //符合条件，初始化dp，第一行的这种情况有1种摆放方式
            dp[0][i]=1;
        }
    }
    //dp
    for(int i=1;i<n;i++){
        for(int j=0;j<k;j++){
            //单纯判断第i行能否按状态j放
            //判断为真则不能按状态j放！
            if(judge(i,j)){
                continue;
            }
            //当前的j状态不能满足这一行
            //相邻包括上下两行的相邻
            //判断上一行能否按状态f放
            for(int f=0;f<k;f++){
                //遍历所有状态，满足的都不用管
                if(judge(i-1,f)){
                    continue;
                }
                //来到这里就找到一个满足这一行的状态j和满足上一行的状态f
                //判断是否冲突，因为如果冲突，必存在位数1&1=1
                //这一行j状态的情况数就要加上上一行f状态的情况数
                if(!(st[j]&st[f])){
                    //这一行放状态j，上一行可以有状态f1,f2...
                    //将上一行的状态的dp全部加起来
                    dp[i][j]+=dp[i-1][f];
                }
            }
        }
    }
    int ans=0;
    for(int i=0;i<k;i++){
        ans=(ans+dp[n-1][i])%MOD;
    }
    printf("%d\n",ans);
    // for(int i=0;i<n;i++){
    //     printf("%d\n",map[i]);
    // }
    // printf("---------\n");
    // for(int i=0;i<k;i++){
    //     printf("%d\n",st[i]);
    // }
    return 0;
}
```






