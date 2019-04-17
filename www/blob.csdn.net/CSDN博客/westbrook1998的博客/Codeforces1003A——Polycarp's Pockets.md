# Codeforces1003A——Polycarp's Pockets - westbrook1998的博客 - CSDN博客





2018年08月09日 18:38:37[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33








> 
Polycarp has n coins, the value of the i-th coin is ai. Polycarp wants to distribute all the coins between his pockets, but he cannot put two coins with the same value into the same pocket. 

  For example, if Polycarp has got six coins represented as an array a=[1,2,4,3,3,2], he can distribute the coins into two pockets as follows: [1,2,3],[2,3,4]. 

  Polycarp wants to distribute all the coins with the minimum number of used pockets. Help him to do that. 

  Input 

  The first line of the input contains one integer n (1≤n≤100) — the number of coins. 

  The second line of the input contains n integers a1,a2,…,an (1≤ai≤100) — values of coins. 

  Output 

  Print only one integer — the minimum number of pockets Polycarp needs to distribute all the coins so no two coins with the same value are put into the same pocket. 

  Examples 

  Input 

  6 

  1 2 4 3 3 2 

  Output 

  2 

  Input 

  1 

  100 

  Output 

  1
暑假练习第二天 

这题思路就是找出出现次数最多的 

代码：
```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=105;
int a[MAXN];
int main(void){
    int n;
    int t;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&t);
        a[t]++;
    }
    int Max=0;
    for(int i=1;i<=100;i++){
        if(a[i]>Max){
            Max=a[i];
        }
    }
    printf("%d\n",Max);
    return 0;
}
```






