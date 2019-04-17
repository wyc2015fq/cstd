# 处女座与cf（2019牛客寒假算法基础集训营 Day2-B） - Alex_McAvoy的博客 - CSDN博客





2019年01月25日 19:38:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：124
个人分类：[牛客																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/8639591)








> 
# 【题目描述】

众所周知，处女座经常通过打cf来调节自己的心情。今天处女座又参加了一场cf的比赛，他知道了所有的提交记录，他想知道自己的得分和排在第几名。你知道处女座的cf账号是cnz

Codeforces规则如下：

1.比赛一共2小时

2.比赛有5题，A题500分，B题1000分，C题1500分，D题2000分，E题2500分。

3.得分规则如下：

在第 0 分钟完成某一题可以得到全部的分数，每过一分钟每题的分值会衰减 1/250，比如在第 3 分钟完成 A 题，能够得到500-2*3=494 分

4.如果一道题是的返回结果 WA 或者 TLE 被称为错误的提交，CE 视为无效的提交，AC，WA 和 TLE 都视为有效的提交。如果一道题你最后通过了，你会得到这道题衰减之后的分值再减去你错误提交次数*50，就是每次错误的提交会有 50 分的罚时。

5.如果你通过了一道题，你的得分不会低于该题分值的 30%。比如你在第 50 分钟通过了 A，你有 7 次错误的提交，你的得分为 max(500*0.3,500-2*50(得分衰减)-7*50(错误提交的罚时))=150分。

6.由于 hack 机制的存在，你每进行一次提交，对于这一题之前的有效提交 (AC,WA,TLE) 都视为错误的提交。

7.一个人只有提交 (AC,WA,TLE,CE) 过代码，才被视为参加比赛。

处女座又了解到一些信息：本场比赛没有任何选手 hack 别人，并且没有任何的提交 fst（即只要是某题的最后一次提交通过，就视为通过这道题）

# 【输入描述】

第一行两个整数 n 和 m，n 为报名比赛的人数，m 为提交的个数

接下来 n 行，每行一个字符串，表示报名比赛的人的昵称。（字符串只包含小写字母，且长度小于 20）

接下来 m 行，每行的格式为 Time，Submiter，Problem，Verdict。

Time 为提交的时间，是 1 到 120 中的一个正整数（包含 1 和 120）,保证 Time 按顺序给出

Submiter 为提交者昵称

Problem 为题目名称，是 ’A’,’B’,’C’,’D’,’E’ 中的一个字母。

Verdict为返回的结果，为 ”AC”,”WA”,”TLE”,”CE” 中的一个。

2<=n<=500

1<=m<=10000

# 【输出描述】

如果处女座参加了比赛,输出两行:

第一行为处女座的得分

第二行格式x/y,其中x为处女座的排名，y为参加比赛的总人数。如果分数相同那么排名并列。

如果处女座没有参加比赛，输出”-1”

# 【样例】

示例1

输入

3 7

cnz

cuber

moon

3 cnz A AC

5 cuber A AC

6 cnz B CE

10 cuber C AC

20 cnz B AC

30 cuber D TLE

100 cnz E AC

输出

2914

1/2


思路：模拟，具体见代码

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
#define N 1001
#define LL long long
using namespace std;
int n,m;
map<string,int> mp;
bool vis[N];
int submitTime[N][N];//submitTime[i][j]第i个人的第j个题的有效提交次数
int submitScore[N][N];//submitScore[i][j]第i个人的第j个题的分数
int grade[5]={500,1000,1500,2000,2500};//5个题的得分
int score[N];//score[i]为第i个的最终得分
int main(){

    cin>>n>>m;
    for(int i=1;i<=n;i++){
        string str;
        cin>>str;
        mp[str]=i;
    }

    for(int i=1;i<=m;i++){
        int time;
        string submiter,problem,verdict;
        cin>>time>>submiter>>problem>>verdict;

        int num=(int)(problem[0]-'A');//问题的序号
        int order=mp[submiter];//当前提交者的id
        vis[order]=true;

        if(verdict=="AC"){
            submitScore[order][num]=max(grade[num]/10*3,grade[num]-time*(grade[num]/250)-50*submitTime[order][num]);//计算分数
            submitTime[order][num]++;//提交次数+1
        }
        else if(verdict=="WA"||verdict=="TLE"){
            submitScore[order][num]=0;//分数为0
            submitTime[order][num]++;//提交次数+1
        }
        else if(verdict=="CE"){
            submitScore[order][num]=0;//分数为0
        }
    }

    for(int i=1;i<=n;i++)//统计每个人的最终得分
        for(int j=0;j<5;j++)
            score[i]+=submitScore[i][j];


    int people=0;
    for(int i=1;i<=n;i++)//记录参加总人数
        if(vis[i])
            people++;

    int id=mp["cnz"];//记录处女座的序号
    if(!vis[id])
        printf("-1\n");
    else{
        int Rank=1;//记录处女座排名
        for(int i=1;i<=n;i++)
            if(score[i]>score[id])
                Rank++;

        printf("%d\n",score[id]);
        printf("%d/%d\n",Rank,people);
    }

    return 0;
}
```






