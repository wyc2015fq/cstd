# Romaji（CF-1008A） - Alex_McAvoy的博客 - CSDN博客





2018年08月23日 19:30:19[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：79








# Problem Description

Vitya has just started learning Berlanese language. It is known that Berlanese uses the Latin alphabet. Vowel letters are "a", "o", "u", "i", and "e". Other letters are consonant.

In Berlanese, there has to be a vowel after every consonant, but there can be any letter after any vowel. The only exception is a consonant "n"; after this letter, there can be any letter (not only a vowel) or there can be no letter at all. For example, the words "harakiri", "yupie", "man", and "nbo" are Berlanese while the words "horse", "king", "my", and "nz" are not.

Help Vitya find out if a word ss is Berlanese.

# Input

The first line of the input contains the string ss consisting of |s||s| (1≤|s|≤1001≤|s|≤100) lowercase Latin letters.

# Output

Print "YES" (without quotes) if there is a vowel after every consonant except "n", otherwise print "NO".

You can print each letter in any case (upper or lower).

# Examples

**Input**

sumimasen

**Output**

YES

**Input**

ninja

**Output**

YES

**Input**

codeforces

**Output**

NO

————————————————————————————————————————————

题意：给出一字符串，判断是否除了 n 以外的辅音字母后面都有一个元音字母

思路：将元音标记一下，逐个枚举即可

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
bool vis[27];
int main()
{
    string str;
    cin>>str;

    vis['a'-'a']=true;
    vis['e'-'a']=true;
    vis['i'-'a']=true;
    vis['o'-'a']=true;
    vis['u'-'a']=true;

    bool flag=false;
    int len=str.length();
    for(int i=0;i<len;i++)
    {
        if(str[i]=='n')
            continue;
        else if(vis[str[i]-'a'])
            continue;
        else
        {
            if(vis[str[i+1]-'a'])
                continue;
            else
                flag=true;
        }
    }

    if(flag)
        cout<<"NO"<<endl;
    else
        cout<<"YES"<<endl;

	return 0;
}
```





