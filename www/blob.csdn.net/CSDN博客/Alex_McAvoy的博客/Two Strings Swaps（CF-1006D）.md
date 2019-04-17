# Two Strings Swaps（CF-1006D） - Alex_McAvoy的博客 - CSDN博客





2018年08月06日 12:02:29[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：95








> 
# Problem Description

You are given two strings a and b consisting of lowercase English letters, both of length n. The characters of both strings have indices from 1 to n, inclusive.

You are allowed to do the following changes:

Choose any index i (1≤i≤n) and swap characters ai and bi;

Choose any index i (1≤i≤n) and swap characters ai and an−i+1;

Choose any index i (1≤i≤n) and swap characters bi and bn−i+1.

Note that if nn is odd, you are formally allowed to swap a⌈n2⌉ with a⌈n2⌉ (and the same with the string b) but this move is useless. Also you can swap two equal characters but this operation is useless as well.

You have to make these strings equal by applying any number of changes described above, in any order. But it is obvious that it may be impossible to make two strings equal by these swaps.

In one preprocess move you can replace a character in aa with another character. In other words, in a single preprocess move you can choose any index i (1≤i≤n), any character cc and set ai:=c.

Your task is to find the minimum number of preprocess moves to apply in such a way that after them you can make strings aa and bb equal by applying some number of changes described in the list above.

Note that the number of changes you make after the preprocess moves does not matter. Also note that you cannot apply preprocess moves to the string bb or make any preprocess moves after the first change is made.

# Input

The first line of the input contains one integer nn (1≤n≤10^5) — the length of strings a and b.

The second line contains the string a consisting of exactly n lowercase English letters.

The third line contains the string b consisting of exactly n lowercase English letters.

# Output

Print a single integer — the minimum number of preprocess moves to apply before changes, so that it is possible to make the string a equal to string b with a sequence of changes from the list above.

# Examples

**Input**

7

abacaba

bacabaa

**Output**

4 

**Input**

5

zcabd

dbacz

**Output**

0


题意：给两个长度为 n 的字符串a与串b，可以交换 ai 与 an-i+1，交换 ai 与 bi，交换 bi 与 bn-i+1，现要使得通过以上3种操作使得字符串相同，但大多数情况不可能，因此要进行预处理，改变串a，使得改变后可以满足条件，从而相同。

思路：

改变字符串的操作，实质是对4个位置进行交换，即 ai、an−i+1、bi、bn−i+1 这几个字符的位置随意交换。 

因此只要保证这4个位置只有2种字符，每种字符有2个或者只有1种字符即可。

先统计4个位置的字符，再进行分类讨论：

![](https://img-blog.csdn.net/20180806112343869?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

思路来源：[点击这里](https://blog.csdn.net/C20190102/article/details/81128440)

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
#define MOD 123
#define E 1e-6
using namespace std;
int n;
int num[27];
char str1[N],str2[N];
int main()
{
    scanf("%d",&n);
    scanf("%s%s",str1+1,str2+1);

    int ans=0;
    if(n&1)//中间的两个字符
        ans+=(str1[n/2+1]!=str2[n/2+1]);

    for(int i=1;i<=n/2;i++)
    {
        memset(num,0,sizeof(num));
        num[str1[i]-'a']++;
        num[str2[i]-'a']++;
        num[str1[n-i+1]-'a']++;
        num[str2[n-i+1]-'a']++;

        int kind=0;
        bool flag=false;
        for(int j=0;j<26;j++)
        {
            if(num[j])
            {
                kind++;//统计字符种类
                if(num[j]&1)
                    flag=true;
            }
        }
        if(kind==2&&flag)
            ans++;
        else if(kind==3)//看串1中两字符是否相等
            ans+=(str1[i]==str1[n-i+1])+1;
        else if(kind==4)
            ans+=2;
    }

    cout<<ans<<endl;

    return 0;
}
```





