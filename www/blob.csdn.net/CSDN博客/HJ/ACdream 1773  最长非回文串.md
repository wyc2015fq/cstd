# ACdream  1773   最长非回文串 - HJ - CSDN博客
2016年08月19日 20:19:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：284
Problem Description
回文串大家都知道是什么吧~
现在给你一串字符串
请求出该字符串最长的连续非回文子串的长度是多少。
Input 
长度不超过10^6。题目保证字符串只由小写字母a-z组成。
Output 
一个整数，表示最长非回文串的长度。若不存在输出-1。
Sample Input 
AAABA
Sample Output 
5
比较坑的一个题目，没有之一！！！
解析：把输入的字符串做以下两种情况处理：
（1）如果是回文字符串，则输出该字符串长度减一的数值；
（2）如果不是回文字符串，则直接输出该字符串长度的数值；
但是注意，在处理这两种情况之前，需要排除所有字符都一样的字符串情况出现
代码如下：
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cstring>
using namespace std;
char s[1000005];
int main()
{
    while(cin>>s)
    {
        int len,blag=0,flag=0;
        len=strlen(s);
        for(int i=0;i<len;i++)
            if(s[0]!=s[i])      //首先判断每个字符都一样的字符串情况是否存在
            {
               blag=1;
               break;
            }
        if(blag==0)
          cout<<-1<<endl;
        else
        {
            for(int i=0,j=len-1;i<len/2;i++,j--)   //一个循环开两个下标，分别从两边向中间靠拢，减少循环次数
              if(s[i]!=s[j])       //然后判断该字符串是否为回文串
              {
                 flag=1;
                 break;
              }
            if(flag==1)        //如果不是回文串，则直接输出字符串长度的数值
              cout<<len<<endl;
            else
              cout<<len-1<<endl;   //如果是回文串，则输出字符串长度减一的数值
        }       
    }
    return 0;
}
```
