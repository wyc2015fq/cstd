# Diverse Substring（CF-1037A） - Alex_McAvoy的博客 - CSDN博客





2018年10月31日 20:45:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：152








# Problem Description

You are given a string s, consisting of nn lowercase Latin letters.

A substring of string s is a continuous segment of letters from ss. For example, "defor" is a substring of "codeforces" and "fors" is not.

The length of the substring is the number of letters in it.

Let's call some string of length nn diverse if and only if there is no letter to appear strictly more than n2n2 times. For example, strings "abc" and "iltlml" are diverse and strings "aab" and "zz" are not.

Your task is to find any diverse substring of string s or report that there is none. Note that it is not required to maximize or minimize the length of the resulting substring.

# Input

The first line contains a single integer nn (1≤n≤1000) — the length of string s.

The second line is the string s, consisting of exactly n lowercase Latin letters.

# Output

Print "NO" if there is no diverse substring in the string s.

Otherwise the first line should contain "YES". The second line should contain any diverse substring of string s.

# Examples

**Input**

10

codeforces

**Output**

YES

code

**Input**

5

aaaaa

**Output**

NO

————————————————————————————————————————————

题意：这给出一个长度为 n 的字串，要求判断其有没有任何字母个数不超过 n/2 的子串，有就输出YES以及任意一个子串，没有就输出 NO

思路：一开始想复杂了，其实直接暴力判断相邻两个字符是否一样即可，相同就输出 NO，否则就输出这个子串

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
#define N 20001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
int main(){
    int n;
    string str;
    cin>>n;
    cin>>str;
    for(int i=1;i<n;i++){
        if(str[i]!=str[i-1]){
            cout<<"YES"<<endl;
            cout<<str[i-1]<<str[i]<<endl;
            return 0;
        }
    }
    cout<<"NO"<<endl;
}
```





