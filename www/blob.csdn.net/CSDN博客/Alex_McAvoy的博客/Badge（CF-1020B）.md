# Badge（CF-1020B） - Alex_McAvoy的博客 - CSDN博客





2018年08月13日 17:01:58[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：165
个人分类：[CodeForces																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

In Summer Informatics School, if a student doesn't behave well, teachers make a hole in his badge. And today one of the teachers caught a group of n students doing yet another trick.

Let's assume that all these students are numbered from 11 to nn. The teacher came to student aa and put a hole in his badge. The student, however, claimed that the main culprit is some other student papa.

After that, the teacher came to student papa and made a hole in his badge as well. The student in reply said that the main culprit was student ppappa.

This process went on for a while, but, since the number of students was finite, eventually the teacher came to the student, who already had a hole in his badge.

After that, the teacher put a second hole in the student's badge and decided that he is done with this process, and went to the sauna.

You don't know the first student who was caught by the teacher. However, you know all the numbers pi. Your task is to find out for every student a, who would be the student with two holes in the badge if the first caught student was a.

# Input

The first line of the input contains the only integer n (1≤n≤1000) — the number of the naughty students.

The second line contains n integers p1, ..., pn (1≤pi≤n), where pipi indicates the student who was reported to the teacher by student i.

# Output

For every student aa from 1 to n print which student would receive two holes in the badge, if aa was the first student caught by the teacher.

# Examples

**Input**

3

2 3 2

**Output**

2 2 3 

**Input**

3

1 2 3

**Output**

1 2 3 

————————————————————————————————————————————

题意：n 个学生，每个学生都说是另一个学生做的坏事，老师从第 i 个学生的话开始向下寻找，直到第二次找到这个学生为止

思路：模拟水题，一个指着另一个，顺着走一遍即可，当一个人第二次出现时，停止输出即可

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
#define N 1000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int p[N];
int bucket[N];
int main()
{
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)
        cin>>p[i];

    for(int i=1;i<=n;i++)
    {
        int temp=i;
        memset(bucket,0,sizeof(bucket));

        while(1)
        {
            bucket[temp]++;
            temp=p[temp];

            if(bucket[temp]==2)
                break;
        }

        cout<<temp<<" ";
    }
    cout<<endl;

    return 0;
}
```






