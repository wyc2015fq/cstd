# POJ  2096  Collecting Bugs （概率DP & 期望 ） - weixin_33985507的博客 - CSDN博客
2019年01月08日 01:54:36[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
Collecting Bugs
|**Time Limit:** 10000MS||**Memory Limit:** 64000K|
|----|----|----|
|**Total Submissions:** 1485||**Accepted:** 647|
|**Case Time Limit:** 2000MS||Special Judge|
Description
Ivan is fond of collecting. Unlike other people who collect post stamps, coins or other material stuff, he collects software bugs. When Ivan gets a new program, he classifies all possible bugs into n categories. Each day he discovers exactly one bug in the program and adds information about it and its category into a spreadsheet. When he finds bugs in all bug categories, he calls the program disgusting, publishes this spreadsheet on his home page, and forgets completely about the program. 
Two companies, Macrosoft and Microhard are in tight competition. Microhard wants to decrease sales of one Macrosoft program. They hire Ivan to prove that the program in question is disgusting. However, Ivan has a complicated problem. This new program has s subcomponents, and finding bugs of all types in each subcomponent would take too long before the target could be reached. So Ivan and Microhard agreed to use a simpler criteria --- Ivan should find at least one bug in each subsystem and at least one bug of each category. 
Macrosoft knows about these plans and it wants to estimate the time that is required for Ivan to call its program disgusting. It's important because the company releases a new version soon, so it can correct its plans and release it quicker. Nobody would be interested in Ivan's opinion about the reliability of the obsolete version. 
A bug found in the program can be of any category with equal probability. Similarly, the bug can be found in any given subsystem with equal probability. Any particular bug cannot belong to two different categories or happen simultaneously in two different subsystems. The number of bugs in the program is almost infinite, so the probability of finding a new bug of some category in some subsystem does not reduce after finding any number of bugs of that category in that subsystem. 
Find an average time (in days of Ivan's work) required to name the program disgusting.
Input
Input file contains two integer numbers, n and s (0 < n, s <= 1 000).
Output
Output the expectation of the Ivan's working days needed to call the program disgusting, accurate to 4 digits after the decimal point.
Sample Input
1 2
Sample Output
3.0000
Source
[Northeastern Europe 2004](http://poj.org/searchproblem?field=source&key=Northeastern+Europe+2004), Northern Subregion
/**
    dp求期望的题。
    题意：一个软件有s个子系统，会产生n种bug。
    某人一天发现一个bug，这个bug属于某种bug，发生在某个子系统中。
    求找到所有的n种bug，且每个子系统都找到bug，这样所要的天数的期望。
    需要注意的是：bug的数量是无穷大的，所以发现一个bug，出现在某个子系统的概率是1/s，
    属于某种类型的概率是1/n。
    解法：
    dp[i][j]表示已经找到i种bug，并存在于j个子系统中，要达到目标状态的天数的期望。
    显然，dp[n][s]=0，因为已经达到目标了。而dp[0][0]就是我们要求的答案。
    dp[i][j]状态可以转化成以下四种：
        dp[i][j]    发现一个bug属于已经找到的i种bug和j个子系统中
        dp[i+1][j]  发现一个bug属于新的一种bug，但属于已经找到的j种子系统
        dp[i][j+1]  发现一个bug属于已经找到的i种bug，但属于新的子系统
        dp[i+1][j+1]发现一个bug属于新的一种bug和新的一个子系统
    以上四种的概率分别为：
    p1 =     i*j / (n*s)
    p2 = (n-i)*j / (n*s)
    p3 = i*(s-j) / (n*s)
    p4 = (n-i)*(s-j) / (n*s)
    又有：期望可以分解成多个子期望的加权和，权为子期望发生的概率，即 E(aA+bB+...) = aE(A) + bE(B) +...
    所以：
    dp[i,j] = p1*dp[i,j] + p2*dp[i+1,j] + p3*dp[i,j+1] + p4*dp[i+1,j+1] + 1;
    整理得：
    dp[i,j] = ( 1 + p2*dp[i+1,j] + p3*dp[i,j+1] + p4*dp[i+1,j+1] )/( 1-p1 )
            = ( n*s + (n-i)*j*dp[i+1,j] + i*(s-j)*dp[i,j+1] + (n-i)*(s-j)*dp[i+1,j+1] )/( n*s - i*j )
**/
```
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
const int N=1010;
double dp[N][N];
int main(){
    //freopen("input.txt","r",stdin);
    int n,s;
    while(~scanf("%d%d",&n,&s)){
        dp[n][s]=0;
        for(int i=n;i>=0;i--)
            for(int j=s;j>=0;j--){
                if(i==n && j==s)
                    continue;
                dp[i][j]=(i*(s-j)*dp[i][j+1]+(n-i)*j*dp[i+1][j]+(n-i)*(s-j)*dp[i+1][j+1]+n*s)/(n*s-i*j);
            }
        printf("%.4f\n",dp[0][0]);
    }
    return 0;
}
```
