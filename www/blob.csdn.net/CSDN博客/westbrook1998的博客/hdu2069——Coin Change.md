# hdu2069——Coin Change - westbrook1998的博客 - CSDN博客





2018年07月29日 18:52:01[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45








> 
Suppose there are 5 types of coins: 50-cent, 25-cent, 10-cent, 5-cent, and 1-cent. We want to make changes with these coins for a given amount of money.  

  For example, if we have 11 cents, then we can make changes with one 10-cent coin and one 1-cent coin, or two 5-cent coins and one 1-cent coin, or one 5-cent coin and six 1-cent coins, or eleven 1-cent coins. So there are four ways of making changes for 11 cents with the above coins. Note that we count that there is one way of making change for zero cent.  

  Write a program to find the total number of different ways of making changes for any amount of money in cents. Your program should be able to handle up to 100 coins.  

  Input 

  The input file contains any number of lines, each one consisting of a number ( ≤250 ) for the amount of money in cents. 

  Output 

  For each input line, output a line containing the number of different ways of making changes with the above 5 types of coins. 

  Sample Input 

  11 

  26 

  Sample Output 

  4 

  13
限制总硬币数量的题目 母函数

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=255;
//tmp[i][j]表示用j个硬币凑i元的种类数
int tmp[MAXN][105];
int ans[MAXN][105];
//都有无限个
int a[6]={1,5,10,25,50};
void Init(){
    ans[0][0]=1;
    //枚举硬币种数
    for(int i=0;i<5;i++){
        //枚举要凑成的钱数
        for(int j=0;j<=250;j++){
            //在可用范围内枚举该种硬币数量
            for(int k=0;k*a[i]+j<=250;k++){
                //枚举t控制总硬币数量
                for(int t=0;k+t<=100;t++){
                    tmp[j+k*a[i]][k+t]+=ans[j][t];
                    //printf("%d ",tmp[j+k*a[i]][k+t]);
                }
            }
        }
        memcpy(ans,tmp,sizeof(tmp));
        memset(tmp,0,sizeof(tmp));
    }
}
int main(void){
    Init();
    int n;
    while(~scanf("%d",&n)){
        int res=0;
        //枚举硬币数 累加得到总的种类数
        for(int i=0;i<=100;i++){
            res+=ans[n][i];
        }
        printf("%d\n",res);
    }
    return 0;
}
```





