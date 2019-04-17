# The hat（CF-1020D） - Alex_McAvoy的博客 - CSDN博客





2018年08月24日 10:07:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：85
个人分类：[CodeForces																[查找——二分查找](https://blog.csdn.net/u011815404/article/category/8116900)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

This is an interactive problem.

Imur Ishakov decided to organize a club for people who love to play the famous game «The hat». The club was visited by n students, where n is even. Imur arranged them all in a circle and held a draw to break the students in pairs, but something went wrong. The participants are numbered so that participant i and participant i + 1 (1 ≤ i ≤ n - 1) are adjacent, as well as participant n and participant 1. Each student was given a piece of paper with a number in such a way, that for every two adjacent students, these numbers differ exactly by one. The plan was to form students with the same numbers in a pair, but it turned out that not all numbers appeared exactly twice.

As you know, the most convenient is to explain the words to the partner when he is sitting exactly across you. Students with numbers i and  sit across each other. Imur is wondering if there are two people sitting across each other with the same numbers given. Help him to find such pair of people if it exists.

You can ask questions of form «which number was received by student i?», and the goal is to determine whether the desired pair exists in no more than 60 questions.

# Input

At the beginning the even integer n (2 ≤ n ≤ 100 000) is given — the total number of students.

You are allowed to ask no more than 60 questions.

# Output

To ask the question about the student i (1 ≤ i ≤ n), you should print «? i». Then from standard output you can read the number ai received by student i ( - 109 ≤ ai ≤ 109).

When you find the desired pair, you should print «! i», where i is any student who belongs to the pair (1 ≤ i ≤ n). If you determined that such pair doesn't exist, you should output «! -1». In both cases you should immediately terminate the program.

The query that contains your answer is not counted towards the limit of 60 queries.

Please make sure to flush the standard output after each command. For example, in C++ use function fflush(stdout), in Java call System.out.flush(), in Pascal use flush(output) and stdout.flush() for Python language.

Hacking

Use the following format for hacking:

In the first line, print one even integer n (2 ≤ n ≤ 100 000) — the total number of students.

In the second line print n integers ai ( - 109 ≤ ai ≤ 109) separated by spaces, where ai is the number to give to i-th student. Any two adjacent elements, including n and 1, must differ by 1 or  - 1.

The hacked solution will not have direct access to the sequence ai.

# Examples

**Input**

8

2

2

**Output**



? 4

? 8

! 4

**Input**

6

1

2

3 

2

1

0

**Output**



? 1

? 2

? 3

? 4

? 5

? 6

! -1

————————————————————————————————————————————

题意：n 个人编号 1 到 n 坐一圈，编号 i 的对面是编号 i+n/2 的人，每个人手上有一个数字 ai，任意相邻两人数字差为 1 或 -1，现在有询问方式《? x》，机器会回答 x 编号的人手上的数字，如果任意一个人跟其对面人手上数字相同，并回答《! ans》，ans为满足条件的人，如果没有这样的人，回答《! -1》

思路来源：[点击这里](https://blog.csdn.net/kzn2683331518/article/details/81605059)

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
#define N 1001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int n;
int ans[N];
int across(int x)
{
    return (x+n/2-1)%n+1;//-1取模再+1，结果范围在1到n
}

int query(int x)
{
    printf("? %d\n? %d\n",x,across(x));
    fflush(stdout);
    scanf("%d%d",&ans[1],&ans[2]);
    if(ans[1]-ans[2]==0)//询问到答案，输出并退出程序
    {
        printf("! %d\n",x);
        fflush(stdout);
        exit(0);
    }
    return ans[1]>ans[2]?1:-1;
}
int main()
{
    scanf("%d",&n);
    if(n%4)//n不是4的倍数无解
    {
        printf("! -1\n");
        fflush(stdout);
        return 0;
    }


    int left=1,right=1+n/2;
    int left_d=query(left);
    int right_d=-left_d;
    while(left+1<right)
    {
        int mid=(left+right)/2;
        int mid_d=query(mid);
        if(left_d<0)
        {
            if(mid_d<0)//mid_d与left_d同号则更新left
                left=mid;
            else
                right=mid;
        }
        else if(right_d<0)//mid_d与right_d同号则更新right
        {
            if(mid_d<0)
                right=mid;
            else
                left=mid;
        }
    }
    query(right);
    query(left);


    return 0;
}
```






