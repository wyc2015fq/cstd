# HDU4045 Machine scheduling 插版法+第二类stirling数 - 紫芝的博客 - CSDN博客





2018年09月12日 10:13:23[紫芝](https://me.csdn.net/qq_40507857)阅读数：39








# Machine scheduling

****Time Limit: 5000/2000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 2163    Accepted Submission(s): 791****

Problem Description

A Baidu’s engineer needs to analyze and process large amount of data on machines every day. The machines are labeled from 1 to n. On each day, the engineer chooses r machines to process data. He allocates the r machines to no more than m groups ,and if the difference of 2 machines' labels are less than k,they can not work in the same day. Otherwise the two machines will not work properly. That is to say, the machines labeled with 1 and k+1 can work in the same day while those labeled with 1 and k should not work in the same day. Due to some unknown reasons, the engineer should not choose the allocation scheme the same as that on some previous day. otherwise all the machines need to be initialized again. As you know, the initialization will take a long time and a lot of efforts. Can you tell the engineer the maximum days that he can use these machines continuously without re-initialization.

Input

Input end with EOF.

Input will be four integers n,r,k,m.We assume that they are all between 1 and 1000.

Output

Output the maxmium days modulo 1000000007.

Sample Input

5 2 3 2

Sample Output

6

***Hint***

Sample input means you can choose 1 and 4,1 and 5,2 and 5 in the same day. And you can make the machines in the same group or in the different group. So you got 6 schemes. 1 and 4 in same group,1 and 4 in different groups. 1 and 5 in same group,1 and 5 in different groups. 2 and 5 in same group,2 and 5 in different groups. We assume 1 in a group and 4 in b group is the same as 1 in b group and 4 in a group.

Source

[The 36th ACM/ICPC Asia Regional Beijing Site —— Online Contest](http://acm.hdu.edu.cn/search.php?field=problem&key=The+36th+ACM%2FICPC+Asia+Regional+Beijing+Site+%A1%AA%A1%AA+Online+Contest+&source=1&searchmode=source)

题意：

有N个机器，选出R个机器，而且每两个机器的编号差要大于等于K，将这R个机器最多分为M组工作，问最多有多少种方案。

分析：

问题由两部分构成：

1.从n个机器中选出r个满足条件的机器的方案数

2.r个机器最多分为m组的方案数

二者相乘即为答案

第一部分：

先满足每两个机器之间至少有K-1个间隔，也就是还剩下rem=n-((r-1)*k+1)个机器可以随意安排，

把这些多余的插入到R个机器之间(加上两端共R+1个位置)。

问题也就变为rem个相同的球分到R+1个不同的组可以为空这种模型，

不难推出是C(rem+R，R)，网上有说直接用插板法公式。可参考：插板法

第二部分：R个元素分为i个非空集合是第二类斯特林数，对i为1至m求和即可。

```cpp
#include<bits/stdc++.h>
using namespace std;
#define mod 1000000007
typedef long long ll;
const int maxn=1005;
ll s2[maxn][maxn];
int c[maxn<<1][maxn<<1];
void init()
{
    s2[1][1]=1;
    for(int i=2;i<maxn;i++)
        for(int j=1;j<=i;j++){
            s2[i][j]=(s2[i-1][j-1]+j*s2[i-1][j])%mod;
        }

    memset(c,0,sizeof c);
    c[1][1]=1;
    for(int i=2;i<maxn*2;i++)
        for(int j=1;j<=i;j++)
        c[i][j]=(c[i-1][j-1]+c[i-1][j])%mod;
}
int main()
{
    init();
    int n,r,k,m;
    while(scanf("%d%d%d%d",&n,&r,&k,&m)!=EOF){
        ll rem=n-((r-1)*k+1);
        if(rem<=0)//无法满足分割要求
        {
            printf("0\n");
            continue;
        }

        ll cnt=0;
        for(int i=1;i<=m;i++)
        {
            cnt=(cnt+s2[r][i])%mod;
        }
        //printf("%lld\n",c[rem+r+1][r+1]);
        printf("%lld\n",(cnt*c[rem+r+1][r+1])%mod);
    }
    return 0;
}
```





