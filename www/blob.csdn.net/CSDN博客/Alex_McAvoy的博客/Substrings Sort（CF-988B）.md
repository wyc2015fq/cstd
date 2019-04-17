# Substrings Sort（CF-988B） - Alex_McAvoy的博客 - CSDN博客





2018年08月07日 17:29:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：67
个人分类：[CodeForces																[STL 的使用——算法](https://blog.csdn.net/u011815404/article/category/8793970)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

You are given n strings. Each string consists of lowercase English letters. Rearrange (reorder) the given strings in such a way that for every string, all strings that are placed before it are its substrings.

String a is a substring of string b if it is possible to choose several consecutive letters in b in such a way that they form a. For example, string "for" is contained as a substring in strings "codeforces", "for" and "therefore", but is not contained as a substring in strings "four", "fofo" and "rof"

# Input

The first line contains an integer n (1≤n≤100) — the number of strings.

The next n lines contain the given strings. The number of letters in each string is from 1 to 100, inclusive. Each string consists of lowercase English letters.

Some strings might be equal

# Output

If it is impossible to reorder n given strings in required order, print "NO" (without quotes).

Otherwise print "YES" (without quotes) and nn given strings in required order.

# Examples

**Input**

5

a

aba

abacaba

ba

aba

**Output**

YES

a

ba

aba

aba

abacaba

**Input**

5

a

abacaba

ba

aba

abab

**Output**

NO

**Input**

3

qwerty

qwerty

qwerty

**Output**

YES

qwerty

qwerty

qwerty


题意：给出 n 个字符串，按长度排序后要求前一个字符串是后一个字符串的子串，如果满足条件，输出 YES 以及排好序后的字符串，否则输出 NO

思路：先排序，然后用 find() 来寻找子串即可，注意 find() 返回的是子串的首地址

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
string str[N];
int cmp(string a,string b)
{
    return a.length()<b.length();
}
int main()
{
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)
        cin>>str[i];

    sort(str+1,str+1+n,cmp);//按字符串长度排序

    bool flag=true;
    for(int i=1;i<=n-1;i++)
        if(str[i+1].find(str[i])>n)//寻找第i+1的子串，如果大于n，说明不存在
            flag=false;

    if(flag)
    {
        cout<<"YES"<<endl;;
        for(int i=1;i<=n;i++)
            cout<<str[i]<<endl;
    }
    else
        cout<<"NO"<<endl;


    return 0;
}
```






