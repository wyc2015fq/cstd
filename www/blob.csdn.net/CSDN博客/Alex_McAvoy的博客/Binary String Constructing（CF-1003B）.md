# Binary String Constructing（CF-1003B） - Alex_McAvoy的博客 - CSDN博客





2018年08月05日 20:00:58[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：61
个人分类：[基础算法——模拟																[CodeForces](https://blog.csdn.net/u011815404/article/category/7923180)](https://blog.csdn.net/u011815404/article/category/7812141)








# Problem Description

You are given three integers aa, bb and xx. Your task is to construct a binary string ss of length n=a+b such that there are exactly aa zeroes, exactly bb ones and exactly x indices i (where 1≤i<n) such that si≠si+1. It is guaranteed that the answer always exists.

For example, for the string "01010" there are four indices ii such that 1≤i<n and si≠si+1 (i=1,2,3,4). For the string "111001" there are two such indices ii (i=3,5).

Recall that binary string is a non-empty sequence of characters where each character is either 0 or 1.

# Input

The first line of the input contains three integers a, b and x (1≤a,b≤100,1≤x<a+b).

# Output

Print only one string s, where ss is any binary string satisfying conditions described above. It is guaranteed that the answer always exists.

# Examples

**Input**

2 2 1

**Output**

1100

**Input**

3 3 3

**Output**

101100

**Input**

5 3 6

**Output**

01010100

# Note

All possible answers for the first example:

1100;

0011.

All possible answers for the second example:

110100;

101100;

110010;

100110;

011001;

001101;

010011;

001011.

————————————————————————————————————————————

题意：给出 a，b，x，要求输出一个含有 a 个 0，b 个 1 的字符串，要求这个字符串串刚好有 x 个 ai，满足 ai != ai+1

思路：模拟，判断奇偶关系，按要求输出即可，需要注意的是 1 和 0 谁放在前的情况。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int main()
{
    int a,b,x;
    while(scanf("%d%d%d",&a,&b,&x)!=EOF)
    {
        if(x%2)
        {
            int num=x/2;//记录如果连续输出01/10有多少位相邻数字不一样的
            if(a>b)
            {
                while(num--)
                    printf("01");
                a-=(x/2),b-=(x/2);//记录剩余的1、0个数
                while(a--)
                    printf("0");
                while(b--)
                    printf("1");
                printf("\n");
            }
            else
            {
                while(num--)
                    printf("10");
                a-=(x/2),b-=(x/2);//记录剩余的1、0个数
                while(b--)
                    printf("1");
                while(a--)
                    printf("0");
                printf("\n");
            }
        }
        else
        {
            int num=x/2;
            if(a>b)
            {
                for(int i=1;i<=num-1;i++)
                    printf("01");

                a=a-(x/2)+1,b=b-(x/2)+1;//记录剩余的1、0个数
                printf("0");
                a--;
                while(b--)
                    printf("1");
                while(a--)
                    printf("0");
                printf("\n");
            }
            else
            {
                for(int i=1;i<=num-1;i++)
                    printf("10");

                a=a-(x/2)+1,b=b-(x/2)+1;//记录剩余的1、0个数

                printf("1");
                b--;
                while(a--)
                    printf("0");
                while(b--)
                    printf("1");
                printf("\n");
            }
        }

    }
    return 0;
}
```






