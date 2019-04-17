# How Many Answers Are Wrong（HDU-3038） - Alex_McAvoy的博客 - CSDN博客





2018年05月23日 18:51:31[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41
个人分类：[HDU																[数据结构——并查集](https://blog.csdn.net/u011815404/article/category/8376617)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

TT and FF are ... friends. Uh... very very good friends -________-b

FF is a bad boy, he is always wooing TT to play the following game with him. This is a very humdrum game. To begin with, TT should write down a sequence of integers-_-!!(bored).

Then, FF can choose a continuous subsequence from it(for example the subsequence from the third to the fifth integer inclusively). After that, FF will ask TT what the sum of the subsequence he chose is. The next, TT will answer FF's question. Then, FF can redo this process. In the end, FF must work out the entire sequence of integers.

Boring~~Boring~~a very very boring game!!! TT doesn't want to play with FF at all. To punish FF, she often tells FF the wrong answers on purpose.

The bad boy is not a fool man. FF detects some answers are incompatible. Of course, these contradictions make it difficult to calculate the sequence.

However, TT is a nice and lovely girl. She doesn't have the heart to be hard on FF. To save time, she guarantees that the answers are all right if there is no logical mistakes indeed.

What's more, if FF finds an answer to be wrong, he will ignore it when judging next answers.

But there will be so many questions that poor FF can't make sure whether the current answer is right or wrong in a moment. So he decides to write a program to help him with this matter. The program will receive a series of questions from FF together with the answers FF has received from TT. The aim of this program is to find how many answers are wrong. Only by ignoring the wrong answers can FF work out the entire sequence of integers. Poor FF has no time to do this job. And now he is asking for your help~(Why asking trouble for himself~~Bad boy)

# **Input**

Line 1: Two integers, N and M (1 <= N <= 200000, 1 <= M <= 40000). Means TT wrote N integers and FF asked her M questions.

Line 2..M+1: Line i+1 contains three integer: Ai, Bi and Si. Means TT answered FF that the sum from Ai to Bi is Si. It's guaranteed that 0 < Ai <= Bi <= N.

You can assume that any sum of subsequence is fit in 32-bit integer. 

# Output

A single line with a integer denotes how many answers are wrong.

# Sample Input

**10 5**

**1 10 1007 10 281 3 324 6 416 6 1**

# Sample Output

**1**


————————————————————————————————————————————————————

题意：给出一些区间[l，r]的和为S，判断后面给出的区间及区间和，是否和前面的冲突，输出冲突的次数。

思路：带权并查集，[A，B] 的区间累加和为 S，即 B 的累加和减去到 (A-1) 的累加和为 S 

需要注意： 

1. 此类问题需要对所有值统计设置相同的初值，但初值的大小一般没有影响。 

2. 对区间**[l, r]**进行记录时，实际上是对 **(l-1, r]**操作，即**l = l - 1**。（即势差是在**l-1**和**r**之间） 

3. 在进行路径压缩时，可和统计类问题相似的**cnt[x] += cnt[fa]**（因为势差是直接累计到根结点的） 

4. 在合并操作中，对我们需要更新**cnt[fb]**（由于fb连接到了fa上），动态更新的公式是**cnt[fb] = cnt[u - 1] - cnt[v] + d**，为了理解这个式子，我们进行如下讨论： 

- 更新**cnt[fb]**的目的是维护被合并的树(fb)相对于合并树(fa)之间的势差。 

- **cnt[fb] - cnt[fa]**两者之间的关系，并不能直接建立，而是通过**cnt[u - 1] - cnt[v]**之间的关系建立。 

- 可知 **cnt[v]**保存结点v与结点fb之间的势差； **cnt[u-1]**保存结点u-1与结点fa之间的势差；**d**是更新信息中结点u-1与结点v之间的势差 

- 所以**cnt[fb]**的值为从结点fb与结点v(**-cnt[v]**)，加上从结点v到结点u(**d**)，最后加上从结点u-1到结点fa(**cnt[u - 1]**)

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#define INF 0x3f3f3f3f
#define N 1000001
#define MOD 2520
#define E 1e-12
using namespace std;
int father[N];
int dis[N];
int Find(int x)
{
    if(father[x]==x)
        return x;
    int temp=father[x];
    father[x]=Find(father[x]);
    dis[x]+=dis[temp];//结点a到根的距离
    return father[x];
}
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        for(int i=1;i<=n;i++)
            father[i]=i;
        memset(dis,0,sizeof(dis));
        int cnt=0;

        while(m--)
        {
            int x,y,s;
            scanf("%d%d%d",&x,&y,&s);
            x=x-1;
            int a=Find(x);
            int b=Find(y);
            if(a!=b)
            {
                father[b]=a;
                dis[b]=dis[x]-dis[y]+s;//结点b到根的距离
            }
            else
            {
                if(dis[y]-dis[x]!=s)//判断是否满足和的关系
                    cnt++;
            }
        }

        printf("%d\n",cnt);
    }
    return 0;
}
```






