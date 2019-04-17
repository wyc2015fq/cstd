# Palindromic Twist（CF-1027A） - Alex_McAvoy的博客 - CSDN博客





2018年08月24日 09:51:09[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：56
个人分类：[CodeForces																[字符串处理——回文串相关](https://blog.csdn.net/u011815404/article/category/8650935)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

You are given a string ss consisting of nn lowercase Latin letters. nn is even.

For each position i (1≤i≤n) in string s you are required to change the letter on this position either to the previous letter in alphabetic order or to the next one (letters 'a' and 'z' have only one of these options). Letter in every position must be changed exactly once.

For example, letter 'p' should be changed either to 'o' or to 'q', letter 'a' should be changed to 'b' and letter 'z' should be changed to 'y'.

That way string "codeforces", for example, can be changed to "dpedepqbft" ('c' →→ 'd', 'o' →→ 'p', 'd' →→ 'e', 'e' →→ 'd', 'f' →→ 'e', 'o' →→ 'p', 'r' →→ 'q', 'c' →→ 'b', 'e' →→ 'f', 's' →→ 't').

String ss is called a palindrome if it reads the same from left to right and from right to left. For example, strings "abba" and "zz" are palindromes and strings "abca" and "zy" are not.

Your goal is to check if it's possible to make string ss a palindrome by applying the aforementioned changes to every position. Print "YES" if string ss can be transformed to a palindrome and "NO" otherwise.

Each testcase contains several strings, for each of them you are required to solve the problem separately.

# Input

The first line contains a single integer T (1≤T≤50) — the number of strings in a testcase.

Then 2T lines follow — lines (2i−1) and 2i of them describe the ii-th string. The first line of the pair contains a single integer nn (2≤n≤100, nn is even) — the length of the corresponding string. The second line of the pair contains a string s, consisting of nn lowercase Latin letters.

# Output

Print T lines. The i-th line should contain the answer to the i-th string of the input. Print "YES" if it's possible to make the i-th string a palindrome by applying the aforementioned changes to every position. Print "NO" otherwise.

# Examples

**Input**

5

6

abccba

2

cf

4

adfa

8

abaazaba

2

ml

**Output**

YES

NO

YES

NO

NO


题意：给出一个字符串，每个字母都能变成其相邻字母，但A只能变成B，Z只能变成Y，判断给出的字符串每个字母改变时能否变成回文串

思路：特判一下 A、Z 的情况，其余的情况直接判断对应位是否相差两位即可

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
#define N 5000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        int n;
        string str;
        cin>>n;
        cin>>str;

        int flag=1;
        for(int i=0,j=n-1;i<n/2;i++,j--)
        {
            if(str[i]=='a')
            {
                if(str[j]!='a'&&str[j]!='b'&&str[j]!='c')
                {
                    flag=0;
                    break;
                }
            }

            if(str[i]=='z')
            {
                if(str[j]!='z'&&str[j]!='y'&&str[j]!='x')
                {
                    flag=0;
                    break;
                }
            }

            int temp=abs(str[i]-str[j]);
            if(temp>2||temp==1)
            {
                flag=0;
                break;
            }
        }

        if(flag)
            cout<<"YES"<<endl;
        else
            cout<<"NO"<<endl;
    }

    return 0;
}
```






