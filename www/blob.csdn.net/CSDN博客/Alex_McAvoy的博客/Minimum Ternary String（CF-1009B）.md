# Minimum Ternary String（CF-1009B） - Alex_McAvoy的博客 - CSDN博客





2018年08月23日 20:16:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45
个人分类：[CodeForces																[字符串处理——模拟与暴力](https://blog.csdn.net/u011815404/article/category/8650811)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

You are given a ternary string (it is a string which consists only of characters '0', '1' and '2').

You can swap any two adjacent (consecutive) characters '0' and '1' (i.e. replace "01" with "10" or vice versa) or any two adjacent (consecutive) characters '1' and '2' (i.e. replace "12" with "21" or vice versa).

For example, for string "010210" we can perform the following moves:

"010210" →→ "100210";

"010210" →→ "001210";

"010210" →→ "010120";

"010210" →→ "010201".

Note than you cannot swap "02" → "20" and vice versa. You cannot perform any other operations with the given string excluding described above.

You task is to obtain the minimum possible (lexicographically) string by using these swaps arbitrary number of times (possibly, zero).

String aa is lexicographically less than string bb (if strings aa and bb have the same length) if there exists some position i (1≤i≤|a|, where |s||s| is the length of the string ss) such that for every j<i holds aj=bj, and ai<bi.

# Input

The first line of the input contains the string ss consisting only of characters '0', '1' and '2', its length is between 1 and 105 (inclusive).

# Output

Print a single string — the minimum possible (lexicographically) string you can obtain by using the swaps described above arbitrary number of times (possibly, zero).

# Examples

**Input**

100210

**Output**

001120

**Input**

11222121

**Output**

11112222

**Input**

20

**Output**

20


题意：给出一串只有 0、1、2 的数串，其中，0 与 1 可互相交换，1 与 2 可互相交换，求交换后最小的值

思路：无论 1 在哪，总能被换到 2 的前面去，但 0 无法换到 2 的前面，因此先统计第一个 2 前面 0 的个数，输出，然后再统计整个数串中 1 点个数，输出，最后再将第一个 2 后面的所有数字去掉 1 后输出

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
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
int main()
{

    string str;
    cin>>str;

    LL len=str.length();
    LL num_1=0;
    bool flag=true;
    for(LL i=0;i<len;i++)
    {
        if(str[i]=='1')
            num_1++;
        if(str[i]=='1'||str[i]=='2')
            flag=false;
    }
    LL pos;
    LL num_0=0;
    for(LL i=0;i<len;i++)
    {
        if(str[i]=='0')
            num_0++;

        if(str[i]=='2')
        {
            pos=i;
            break;
        }
    }

    if(flag)
    {
        cout<<str<<endl;
        return 0;
    }

    while(num_0--)
        cout<<'0';
    while(num_1--)
        cout<<'1';

    for(LL i=pos;i<len;i++)
    {
        if(str[i]=='1')
            continue;
        else
            cout<<str[i];
    }
    cout<<endl;

    return 0;
}
```






