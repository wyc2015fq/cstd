# codeforces 570-C. Replacement（预处理+规律） - HJ - CSDN博客
2017年07月11日 11:07:26[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：215
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Daniel has a string s, consisting of lowercase English letters and period signs (characters ‘.’). Let’s define the operation of replacement as the following sequence of steps: find a substring “..” (two consecutive periods) in string s, of all occurrences of the substring let’s choose the first one, and replace this substring with string “.”. In other words, during the replacement operation, the first two consecutive periods are replaced by one. If string s contains no two consecutive periods, then nothing happens.
Let’s define f(s) as the minimum number of operations of replacement to perform, so that the string does not have any two consecutive periods left.
You need to process m queries, the i-th results in that the character at position xi (1 ≤ xi ≤ n) of string s is assigned value ci. After each operation you have to calculate and output the value of f(s).
Help Daniel to process all queries. 
Input
The first line contains two integers n and m (1 ≤ n, m ≤ 300 000) the length of the string and the number of queries.
The second line contains string s, consisting of n lowercase English letters and period signs.
The following m lines contain the descriptions of queries. The i-th line contains integer xi and ci (1 ≤ xi ≤ n, ci — a lowercas English letter or a period sign), describing the query of assigning symbol ci to position xi. 
Output
Print m numbers, one per line, the i-th of these numbers must be equal to the value of f(s) after performing the i-th assignment. 
Examples 
Input
10 3 
.b..bz…. 
1 h 
3 c 
9 f
Output
4 
3 
1
Input
4 4 
.cc. 
2 . 
3 . 
2 a 
1 a
Output
1 
3 
1 
1
Note
Note to the first sample test (replaced periods are enclosed in square brackets).
The original string is “.b..bz….”.
```
after the first query f(hb..bz....) = 4    ("hb[..]bz...."  →  "hb.bz[..].."  →  "hb.bz[..]."  →  "hb.bz[..]"  →  "hb.bz.")
after the second query f(hbс.bz....) = 3    ("hbс.bz[..].."  →  "hbс.bz[..]."  →  "hbс.bz[..]"  →  "hbс.bz.")
after the third query f(hbс.bz..f.) = 1    ("hbс.bz[..]f."  →  "hbс.bz.f.")
```
Note to the second sample test.
The original string is “.cc.”.
```
after the first query: f(..c.) = 1    ("[..]c."  →  ".c.")
after the second query: f(....) = 3    ("[..].."  →  "[..]."  →  "[..]"  →  ".")
after the third query: f(.a..) = 1    (".a[..]"  →  ".a.")
after the fourth query: f(aa..) = 1    ("aa[..]"  →  "aa.")
```
题目大意： 
    一个字符串s只包含小写字母和’.’，现在有一种化简方法就是把两个连续的’.’化为一个’.’。但是现在有一种操作，即把位置a上的字符替换成b，这种操作有m组。问m组操作每一组操作之后，至少需要多少步化简才能化简到不能化简？（操作前后牵连，即第二组操作是在第一组操作之后的基础上）
解题思路： 
    每次发现两个连续的‘.’的时候，就需要多操作一次，因此，先预处理一下总共需要的操作数，每次改变字符的时候判断是‘.’改成字母还是字母改成‘.’,判断相邻之间能否构成两个连续的‘.’即可。
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
#define maxn 1000005
int cnt,x;
char s[maxn],c;
void add(int i,int p)
{
    if(s[i-1]=='.')
      cnt+=p;
    if(s[i+1]=='.')
      cnt+=p;
}
int main()
{
    int n,m;
    while(~scanf("%d%d",&n,&m))
    {
        s[0]='\0';
        scanf("%s",s);
        cnt=0;
        for(int i=1;s[i]!='\0';i++)
          if(s[i]==s[i-1]&&s[i]=='.')
            cnt++;
        for(int i=0;i<m;i++)
        {
            scanf("%d %c",&x,&c);
            if(s[x-1]=='.')       
            {
                if(c!='.')       //用字母替代'.' 
                  add(x-1,-1);    //需要减的话只需要改成-1即可 
            }
            else
            {   
                if(c=='.')      //用'.'替代字母 
                  add(x-1,1);       //此时需要加 
            }
            printf("%d\n",cnt);
            s[x-1]=c;           //每次计算完之后需要改动这个字符 
        }
    }
    return 0;
}
```
