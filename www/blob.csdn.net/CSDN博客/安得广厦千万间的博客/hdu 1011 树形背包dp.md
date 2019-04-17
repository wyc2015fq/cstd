# hdu 1011 树形背包dp - 安得广厦千万间的博客 - CSDN博客





2018年05月26日 13:02:59[独-](https://me.csdn.net/qq_41713256)阅读数：41








题目网址：http://acm.hdu.edu.cn/showproblem.php?pid=1011



题意：



带领m个士兵，存在n个房间；每个房间有两个参数：放置的bugs数以及存在的价值。

每个士兵可以抵抗20个bugs，只有消灭房间中的所有bugs，才可以得到该房间的价值。

要到达下一个房间，必须先将当前房间的bugs全部消除；并且走过的房间就不再到达。

问有m个士兵的情况下，怎样使最后得到的头领的可能性最大，最大为多少？



dp【n】【m】表示以n为根节点的子树用m个士兵能得到的最大的价值。

得到状态方程为 dp【n】【m】=max（dp【n】【m】，dp【n】【m-j】+dp【son（n）】【j】）；

要求父节点的值就要先得到子节点的值，因此需要回溯。

用图来理解这个状态方程（每一层循环的顺序都很关键）

![](https://img-blog.csdn.net/20180526124953186)


我们要求dp【p】【...】的值，首先dp【u】【...】的值我们已经知道了。于是我们对这两组数据做01背包，因此循环的顺序很关键。从大到小遍历p，从小到大遍历u。

for(int k=m;k>=num;k--)    //num为攻打p房间需要的最少士兵
         for(int j=1;j<=k-num;j++)


             dp[p][k]=max(dp[p][k],dp[p][k-j]+dp[u][j]);

最后对每一个子节点做同样的操作。还有就是要建立无向树。

一开始以为这样做会T，但是后来仔细一想背包的复杂度为O（100*100），最多有100个房间，所以没毛病。

最后细节就是 m为0的时候直接输出0，不写就会Wa，不知道为什么。。。。感觉m为0时用dfs的结果应该也是0.。。。

AC代码：


#include<iostream>
#include<cstring>
#include<string.h>
#include<cstdio>
#include<stack>
#include<vector>
#include<algorithm>
#include<map>
#include<cmath>
**using namespace** std**;typedef long long** ll**;const int** mod**=**1e9**;int** dp**[**110**][**110**];int** w**[**110**],**v**[**110**];**
vector**<int>**edge**[**110**];int** n**,**m**;void** dfs**(int** u**,int** p**){    int** num**=(**w**[**u**]+**19**)/**20**;    for(int** i**=**num**;**i**<=**m**;**i**++)** dp**[**u**][**i**]=**v**[**u**];    for(int** i**=**0**;**i**<**edge**[**u**].**size**();**i**++)    {        int** v**=**edge**[**u**][**i**];        if(**v**==**p**) continue;**
        dfs**(**v**,**u**);        for(int** k**=**m**;**k**>=**num**;**k**--)            for(int** j**=**1**;**j**<=**k**-**num**;**j**++)**
             dp**[**u**][**k**]=**max**(**dp**[**u**][**k**],**dp**[**u**][**k**-**j**]+**dp**[**v**][**j**]);    }}int main(){    while(~**scanf**(**"%d%d"**,&**n**,&**m**))    {**
        memset**(**dp**,**0**,sizeof(**dp**));        if(**n**==-**1**&&**m**==-**1**) break;        for(int** i**=**1**;**i**<=**n**;**i**++)        {**
            scanf**(**"%d%d"**,&**w**[**i**],&**v**[**i**]);**
            edge**[**i**].**clear**();        }        for(int** i**=**1**;**i**<**n**;**i**++)        {            int** x**,**y**;**
            scanf**(**"%d%d"**,&**x**,&**y**);**
            edge**[**x**].**push_back**(**y**);**
            edge**[**y**].**push_back**(**x**);        }        if(**m**==**0**)        {**
            printf**(**"0\n"**); continue;        }**
        dfs**(**1**,**0**);**
        printf**(**"%d\n"**,**dp**[**1**][**m**]);    }    return** 0**;}**



