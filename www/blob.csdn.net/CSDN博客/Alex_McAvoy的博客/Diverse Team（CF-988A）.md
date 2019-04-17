# Diverse Team（CF-988A） - Alex_McAvoy的博客 - CSDN博客





2018年08月07日 17:22:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：75
个人分类：[基础算法——模拟																[CodeForces](https://blog.csdn.net/u011815404/article/category/7923180)](https://blog.csdn.net/u011815404/article/category/7812141)








# Problem Description

There are nn students in a school class, the rating of the i-th student on Codehorses is aiai. You have to form a team consisting of kk students (1≤k≤n) such that the ratings of all team members are distinct.

If it is impossible to form a suitable team, print "NO" (without quotes). Otherwise print "YES", and then print kk distinct numbers which should be the indices of students in the team you form. If there are multiple answers, print any of them.

# Input

The first line contains two integers n and k (1≤k≤n≤100) — the number of students and the size of the team you have to form.

The second line contains n integers a1,a2,…,an (1≤ai≤100), where aiai is the rating of i-th student.

# Output

If it is impossible to form a suitable team, print "NO" (without quotes). Otherwise print "YES", and then print k distinct integers from 1 to n which should be the indices of students in the team you form. All the ratings of the students in the team should be distinct. You may print the indices in any order. If there are multiple answers, print any of them.

Assume that the students are numbered from 1 to n.

# Examples

**Input**

5 3

15 13 15 15 12

**Output**

YES

1 2 5 

**Input**

5 4

15 13 15 15 12

**Output**

NO

**Input**

4 4

20 10 40 30

**Output**

YES

1 2 3 4 

————————————————————————————————————————————

题意：给出 n 个学生的成绩，要在 n 个学生里选 k 个人组成一队，要求 k 个人的成绩均不相同，如果可以，输出 YES 以及选的人的编号，如果不行，输出 NO

思路：设置一标记数组，记录分数不同的人的编号以及个数，如果小于 k，则输出 NO，如果满足条件，依次输出即可。

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
#define N 10005
#define MOD 123
#define E 1e-6
using namespace std;
int a[N];
int vis[N];
int ans[N];
int main()
{
    int n,k;
    cin>>n>>k;

    int cnt=0;
    for(int i=1;i<=n;i++)
    {
        cin>>a[i];

        if(!vis[a[i]])//如果分数未出现过
        {
            cnt++;//不同的分数+1
            ans[cnt]=i;//记录位置
        }
        vis[a[i]]=1;//标记
    }

    if(cnt<k)//不同分数小于要求数
        cout<<"NO"<<endl;
    else
    {
        cout<<"YES"<<endl;
        for(int i=1;i<=k;i++)
            cout<<ans[i]<<" ";
        cout<<endl;
    }
    return 0;
}
```






