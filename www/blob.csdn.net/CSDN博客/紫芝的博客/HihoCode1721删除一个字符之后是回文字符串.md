# HihoCode1721删除一个字符之后是回文字符串 - 紫芝的博客 - CSDN博客





2018年11月21日 13:37:50[紫芝](https://me.csdn.net/qq_40507857)阅读数：90标签：[回文字符串](https://so.csdn.net/so/search/s.do?q=回文字符串&t=blog)








**#1721 : 回文字符串2**

Time Limit:10000ms

Case Time Limit:1000ms

Memory Limit:256MB
**Description**

愚人节那天，小Ho在小Hi的一个回文字符串中添加了一个字符。你能帮助小Hi找到被添加的是第几个字符吗？

**Input**

一个只包含小写字母的字符串S。

对于70%的数据，|S| ≤ 1000

对于100%的数据，|S| ≤ 500000

**Output**

输出一个整数K，表示删除第K(从1开始计数)个字符后，S会变成一个回文字符串。

数据保证有解。如果有多个解，输出其中K最小的。

 Sample Input 

aaba
 Sample Output 

1
```cpp
#include<stdio.h>
#include<algorithm>
#include<string.h>
#include<iostream>
#include<math.h>
#include<queue>
#include<map>
#include<vector>
using namespace std;
#define inf 0x3f3f3f3f
#define ll long long
char s[510000];
int main()
{
    while(~scanf("%s",s))
    {
    int k=strlen(s);
    int i=0,j=k-1;
   while(i<j)
   {
     if(s[i]!=s[j]) //从两端找到不同的字符
        break;
     i++;
     j--;
   }
   if(i==j) //说明是aca或aaa这种情况 ，加了字符后依旧为回文串
   {
       char a=s[i];
       for(;i>=0;i--)
        if(s[i]!=a) break;//如aaa，我们要找到第一个a
       printf("%d\n",i+2);
       continue;
   }
   if(s[i+1]==s[j])//aagoga
   {
       char a=s[i];
       for(;i>=0;i--)
        if(s[i]!=a) break;
       printf("%d\n",i+2);
   }
   else//agogaa或acca
       printf("%d\n",j+1);
    }
}
```





