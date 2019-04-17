# Reversing Encryption（CF-999B） - Alex_McAvoy的博客 - CSDN博客





2018年08月07日 17:36:08[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：43
个人分类：[CodeForces																[数论——素性测试](https://blog.csdn.net/u011815404/article/category/8796920)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

A string ss of length nn can be encrypted by the following algorithm:

iterate over all divisors of nn in decreasing order (i.e. from n to 1),

for each divisor dd, reverse the substring s[1…d] (i.e. the substring which starts at position 1 and ends at position d).

For example, the above algorithm applied to the string s="codeforces" leads to the following changes: "codeforces" → "secrofedoc" → "orcesfedoc" → "rocesfedoc" → "rocesfedoc" (obviously, the last reverse operation doesn't change the string because d=1).

You are given the encrypted string t. Your task is to decrypt this string, i.e., to find a string ss such that the above algorithm results in string tt. It can be proven that this string ss always exists and is unique.

# Input

The first line of input consists of a single integer n (1≤n≤100) — the length of the string t. The second line of input consists of the string t. The length of tt is n, and it consists only of lowercase Latin letters.

# Output

Print a string ss such that the above algorithm results in t.

# Examples

**Input**

10

rocesfedoc

**Output**

codeforces

**Input**

16

plmaetwoxesisiht

**Output**

thisisexampletwo

**Input**

1

z

**Output**

z


题意：给出一个长度为 n 的字符串，要求进行解密。对于长度为 n 的字符串的加密规则是，翻转从1到他的一个因子的位置的所有字符

思路：先打表记录 n 的因子，一共翻转其因子个数的轮数，对于每轮，交换当前因子的个数除以2

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
#define N 1000005
#define MOD 1e9+7
#define E 1e-6
using namespace std;
string str;
int prime[N];
int main()
{
    int n;
    cin>>n;
    cin>>str;

    if(n==1)
        cout<<str<<endl;
    else if(n==2)
    {
        swap(str[0],str[1]);
        cout<<str<<endl;
    }
    else if(n==3)
    {
        swap(str[0],str[2]);
        cout<<str<<endl;
    }
    else
    {
        int cnt=0;
        for(int i=1;i<=n;i++)//记录因子
            if(n%i==0)
                prime[cnt++]=i;

        for(int i=0;i<cnt;i++)//总共交换的次数为因子个数
            for(int j=0;j<prime[i]/2;j++)//每轮交换共交换 因子/2 次
                swap(str[j],str[prime[i]-j-1]);

        cout<<str<<endl;
    }

    return 0;
}
```






