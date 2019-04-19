# codeforces 858-C. Did you mean...（暴力模拟） - HJ - CSDN博客
2018年08月14日 08:16:18[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：111
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/858/C](http://codeforces.com/problemset/problem/858/C)
解题思路：根据题目大意设置两个变量进行模拟即可。
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
string s;
char a[]={'a','e','i','o','u'};
int Judge(char c)
{
    for(int i=0;i<5;i++)
      if(c==a[i])
        return 0;
    return 1;
}
int main()
{
    while(cin>>s)
    {
        int cnt=0,num=0;  //cnt代表辅音种类数  num代表当前的相同辅音连续的个数 
        cnt=num=Judge(s[0]); 
        printf("%c",s[0]);
        for(int i=1;i<s.length();i++)
        {
            if(!Judge(s[i]))
              cnt=num=0;
            else
            {
                num++;
                if(s[i]!=s[i-1])
                  cnt++;
            }
            if((cnt>1&&num>cnt)||cnt>2)
              printf(" "),cnt=num=1;
            printf("%c",s[i]);
        }
        printf("\n");
    }
    return 0;
}
```
