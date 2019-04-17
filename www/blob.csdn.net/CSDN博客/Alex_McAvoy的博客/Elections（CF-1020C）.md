# Elections（CF-1020C） - Alex_McAvoy的博客 - CSDN博客





2018年08月14日 16:21:48[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：118








# Problem Description

As you know, majority of students and teachers of Summer Informatics School live in Berland for the most part of the year. Since corruption there is quite widespread, the following story is not uncommon.

Elections are coming. You know the number of voters and the number of parties — nn and mm respectively. For each voter you know the party he is going to vote for. However, he can easily change his vote given a certain amount of money. In particular, if you give i-th voter cici bytecoins you can ask him to vote for any other party you choose.

The United Party of Berland has decided to perform a statistical study — you need to calculate the minimum number of bytecoins the Party needs to spend to ensure its victory. In order for a party to win the elections, it needs to receive strictly more votes than any other party.

# Input

The first line of input contains two integers n and m (1≤n,m≤3000) — the number of voters and the number of parties respectively.

Each of the following n lines contains two integers pipi and cici (1≤pi≤m, 1≤ci≤109) — the index of this voter's preferred party and the number of bytecoins needed for him to reconsider his decision.

The United Party of Berland has the index 1.

# Output

Print a single number — the minimum number of bytecoins needed for The United Party of Berland to win the elections.

# Examples

**Input**

1 2

1 100

**Output**

0

**Input**

5 5

2 100

3 200

4 300

5 400

5 900

**Output**

500

**Input**

5 5

2 100

3 200

4 300

5 800

5 900

**Output**

600

————————————————————————————————————————————

题意：n 个人投票，每个人都有一个选好的序号，若想让其换序号就必须支付他所给的价格，先想让1号的票最多，求最少支付的钱数

思路：纯考虑贪心的话，情况太多，因此可以只考虑1号的获胜的状态，是以几票来获取胜利的，然后枚举所有获胜情况，取最优的即可

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
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
struct Node{
    int num;
    int value;
}a[N];
int bucket[N];
int ticket[N];
int cmp(Node x,Node y)
{
    return x.value<y.value;
}
int main()
{
    int n,m;
    cin>>n>>m;
    for(int i=1;i<=n;i++)
    {
        scanf("%d%d",&a[i].num,&a[i].value);
        bucket[a[i].num]++;//最开始的投票情况
    }

    sort(a+1,a+n+1,cmp);
    long long minn=999999999999,sum=0,ans;
    for(int i=bucket[1];i<=n;i++)//枚举获胜状态
    {
        sum=0;
        ans=0;
        for(int j=2;j<=m;j++)
        {
            if(bucket[j]>=i)
            {
                ticket[j]=bucket[j]-i+1;//要赢此选手需要在其处买的票数
                sum+=ticket[j];//记录要买的总票
            }
            else ticket[j]=0;
        }
        if(i-bucket[1]<sum) 
            continue;
        sum=i-bucket[1]-sum; //记录要在比1号低的选手买的票数
        for(int j=1;j<=n;j++)
        {
            if(ticket[a[j].num]!=0)//比1号高
            {
                ticket[a[j].num]--;
                ans+=a[j].value;
            }
            else
            {
                if(sum!=0&&a[j].num!=1)//低且需要买票
                {
                    sum--;
                    ans+=a[j].value;
                }
            }
        }
        minn=min(minn,ans);//取最优
    }
    cout<<minn<<endl;
    return 0;
}
```





