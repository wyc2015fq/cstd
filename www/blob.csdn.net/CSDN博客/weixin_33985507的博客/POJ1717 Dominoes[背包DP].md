# POJ1717 Dominoes[背包DP] - weixin_33985507的博客 - CSDN博客
2016年11月09日 20:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
Dominoes
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 6731||**Accepted:** 2234|
Description
A domino is a flat, thumbsized tile, the face of which is divided into two squares, each left blank or bearing from one to six dots. There is a row of dominoes laid out on a table: 
![](http://poj.org/images/1717_1.jpg)
The number of dots in the top line is 6+1+1+1=9 and the number of dots in the bottom line is 1+5+3+2=11. The gap between the top line and the bottom line is 2. The gap is the absolute value of difference between two sums. 
Each domino can be turned by 180 degrees keeping its face always upwards. 
What is the smallest number of turns needed to minimise the gap between the top line and the bottom line? 
For the figure above it is sufficient to turn the last domino in the row in order to decrease the gap to 0. In this case the answer is 1. 
Write a program that: computes the smallest number of turns needed to minimise the gap between the top line and the bottom line.
Input
The first line of the input contains an integer n, 1 <= n <= 1000. This is the number of dominoes laid out on the table. 
Each of the next n lines contains two integers a, b separated by a single space, 0 <= a, b <= 6. The integers a and b written in the line i + 1 of the input file, 1 <= i <= 1000, are the numbers of dots on the i-th domino in the row, respectively, in the top line and in the bottom one. 
Output
Output the smallest number of turns needed to minimise the gap between the top line and the bottom line.
Sample Input
4
6 1
1 5
1 3
1 2
Sample Output
1
Source
[CEOI 1997](http://poj.org/searchproblem?field=source&key=CEOI+1997)
题意：
多米诺骨牌有上下2个方块组成，每个方块中有1~6个点。现有排成行的
上方块中点数之和记为S1，下方块中点数之和记为S2，它们的差为|S1-S2|。例如在图8-1中，S1=6+1+1+1=9，S2=1+5+3+2=11，|S1-S2|=2。每个多米诺骨牌可以旋转180°，使得上下两个方块互换位置。
编程用最少的旋转次数使多米诺骨牌上下2行点数之差达到最小。
很普通的01背包
f[i][j+s]表示前i个差值为s的最少次数
滚动数组+for(int j=-i*6;j<=i*6;j++)轻松200ms快了两三倍
```
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=1005,INF=1e9;
inline int read(){
    char c=getchar();int x=0,f=1;
    while(c<'0'||c>'9'){if(c=='-')f=-1;c=getchar();}
    while(c>='0'&&c<='9'){x=x*10+c-'0';c=getchar();}
    return x*f;
}
int n,w[N],s;
int f[2][6*N*2];
void dp(){
    memset(f,127,sizeof(f));
    f[0][s]=0;
    for(int i=1;i<=n;i++){
        for(int j=-i*6;j<=i*6;j++){
            int p=i&1;
            j+=s;
            f[p][j]=INF;
            if(j-w[i]>=0) f[p][j]=min(f[p][j],f[p^1][j-w[i]]);
            if(j+w[i]>=0&&f[p^1][j+w[i]]<INF) f[p][j]=min(f[p][j],f[p^1][j+w[i]]+1);
            j-=s;
        }
    }
}
int main(){
    n=read();s=n*6;
    for(int i=1;i<=n;i++) w[i]=read()-read();
    dp();
    int p=n&1;
    for(int d=0;d<=n;d++){
        if(f[p][s+d]!=INF){printf("%d",f[p][s+d]);break;}
        if(f[p][s-d]!=INF){printf("%d",f[p][s-d]);break;} 
    }
}
```
