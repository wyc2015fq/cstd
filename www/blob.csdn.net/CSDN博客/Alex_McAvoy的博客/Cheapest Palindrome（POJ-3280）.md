# Cheapest Palindrome（POJ-3280） - Alex_McAvoy的博客 - CSDN博客





2018年06月07日 14:46:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：57
个人分类：[POJ																[动态规划——区间 DP](https://blog.csdn.net/u011815404/article/category/7813197)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Keeping track of all the cows can be a tricky task so Farmer John has installed a system to automate it. He has installed on each cow an electronic ID tag that the system will read as the cows pass by a scanner. Each ID tag's contents are currently a single string with length M (1 ≤ M ≤ 2,000) characters drawn from an alphabet of N (1 ≤ N ≤ 26) different symbols (namely, the lower-case roman alphabet).

Cows, being the mischievous creatures they are, sometimes try to spoof the system by walking backwards. While a cow whose ID is "abcba" would read the same no matter which direction the she walks, a cow with the ID "abcb" can potentially register as two different IDs ("abcb" and "bcba").

FJ would like to change the cows's ID tags so they read the same no matter which direction the cow walks by. For example, "abcb" can be changed by adding "a" at the end to form "abcba" so that the ID is palindromic (reads the same forwards and backwards). Some other ways to change the ID to be palindromic are include adding the three letters "bcb" to the begining to yield the ID "bcbabcb" or removing the letter "a" to yield the ID "bcb". One can add or remove characters at any location in the string yielding a string longer or shorter than the original string.

Unfortunately as the ID tags are electronic, each character insertion or deletion has a cost (0 ≤ cost ≤ 10,000) which varies depending on exactly which character value to be added or deleted. Given the content of a cow's ID tag and the cost of inserting or deleting each of the alphabet's characters, find the minimum cost to change the ID tag so it satisfies FJ's requirements. An empty ID tag is considered to satisfy the requirements of reading the same forward and backward. Only letters with associated costs can be added to a string.

# **Input**

Line 1: Two space-separated integers: N and M 

Line 2: This line contains exactly M characters which constitute the initial ID string 

Lines 3..N+2: Each line contains three space-separated entities: a character of the input alphabet and two integers which are respectively the cost of adding and deleting that character.

# Output

Line 1: A single line with a single integer that is the minimum cost to change the given name tag.

# Sample Input

**3 4**

**abcba 1000 1100b 350 700c 200 800**

# Sample Output

**900**

———————————————————————————————————————————————————————

题意：给出一由n种字母构成的长度为m的字符串，每种字母在这个字符串加上或者减去都有一个成本，要将字符串变成回文字符串，求最小花费。

思路

区间dp，没思路，又看题解了。。。



用f[i][j]表示将i-j位置的字符变为回文串的最低花费，易得以下递推关系：

    当str[i]=str[j]时：f[i][j]=f[i+1][j-1]

    当str[i+1]到str[j]是回文串时：f[i][j]=f[i+1][j] + min( add[str[i]]，reduce[str[i]] )

    当str[i]到str[j-1]是回文串时：f[i][j]=f[i][j-1] + min( add[str[j]]，reduce[str[j]] )

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 2001
#define MOD 100001
#define E 1e-12
using namespace std;
int f[N][N];
int cost[26];
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        string str;
        cin>>str;
        while(n--)
        {
            int x,y;
            char ch;
            getchar();
            scanf("%c%d%d",&ch,&x,&y);
            cost[ch-'a']=min(x,y);//拼凑回文串，加上和减去字符效果是等价的，因此选择最小花费
        }

        for(int k=1;k<m;k++)
        {
            for(int i=0,j=k;j<m;i++,j++)
            {
                f[i][j]=INF;
                if(str[i]==str[j])
                    f[i][j]=f[i+1][j-1];
                else
                {
                    f[i][j]=min(f[i][j],f[i+1][j]+cost[str[i]-'a']);
                    f[i][j]=min(f[i][j],f[i][j-1]+cost[str[j]-'a']);
                }
            }
        }
        printf("%d\n",f[0][m-1]);
    }
    return 0;
}
```






