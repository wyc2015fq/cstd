# hdu  2594  Simpsons' Hidden Talents（KMP） - HJ - CSDN博客
2017年01月19日 01:36:03[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：180
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------字符串---------																[KMP](https://blog.csdn.net/feizaoSYUACM/article/category/7238008)](https://blog.csdn.net/feizaoSYUACM/article/category/7238090)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Homer: Marge, I just figured out a way to discover some of the talents we weren’t aware we had. 
Marge: Yeah, what is it? 
Homer: Take me for example. I want to find out if I have a talent in politics, OK? 
Marge: OK. 
Homer: So I take some politician’s name, say Clinton, and try to find the length of the longest prefix 
in Clinton’s name that is a suffix in my name. That’s how close I am to being a politician like Clinton 
Marge: Why on earth choose the longest prefix that is a suffix??? 
Homer: Well, our talents are deeply hidden within ourselves, Marge. 
Marge: So how close are you? 
Homer: 0! 
Marge: I’m not surprised. 
Homer: But you know, you must have some real math talent hidden deep in you. 
Marge: How come? 
Homer: Riemann and Marjorie gives 3!!! 
Marge: Who the heck is Riemann? 
Homer: Never mind. 
Write a program that, when given strings s1 and s2, finds the longest prefix of s1 that is a suffix of s2.
Input 
Input consists of two lines. The first line contains s1 and the second line contains s2. You may assume all letters are in lowercase.
Output 
Output consists of a single line that contains the longest string that is a prefix of s1 and a suffix of s2, followed by the length of that prefix. If the longest such string is the empty string, then the output should be 0. 
The lengths of s1 and s2 will be at most 50000.
Sample Input
clinton 
homer 
riemann 
marjorie
Sample Output
0 
rie 3
题目大意： 
  给定两个字符串s1,s2,求最长的s1前缀ss使得ss为s2的最长后缀,输出该字符串和其长度。
解法一：KMP匹配
```cpp
#include<iostream>
#include<cstring>
using namespace std;
char a[100005],b[50005];
int nxt[100005],len1,len2;
void getNext()
{
    int i=0,j=-1;
    nxt[0]=-1;
    while(i<len1)
    {
        if(j==-1||a[i]==a[j])
          nxt[++i]=++j;
        else
          j=nxt[j]; 
    } 
} 
void KMP()
{
    getNext();
    int i=0,j=0;
    while(i<len2)
    {
        if(j==-1||a[j]==b[i])
        {
            i++;
            j++;
        }
        else
          j=nxt[j];
    }
    if(!j)
      cout<<0<<endl;
    else
    {
        for(int i=0;i<j;i++)
          cout<<a[i];
        cout<<" "<<j<<endl;
    }
}
int main()
{
    while(cin>>a>>b)
    {
        len1=strlen(a);
        len2=strlen(b);
        KMP(); 
    }
    return 0;
}
```
解法二：把两个串链接起来，求next数组即可。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
char a[100005],b[50005];
int nxt[100005];
void getNext()
{
    int i=0,j=-1,len=strlen(a);
    nxt[0]=-1;
    while(i<len)           //strlen(a)要先求出来，不能直接放到括号里面作为判断边界，否则会超时 
    {
        if(j==-1||a[i]==a[j])
          nxt[++i]=++j;
        else
          j=nxt[j];
    }
}
int main()
{
    while(~scanf("%s%s",a,b))
    {
        int lena=strlen(a),lenb=strlen(b),len=lena+lenb;
        strcat(a,b);                         //把串a,b连接起来放到a后面 
        getNext();
        while(nxt[len]>lena||nxt[len]>lenb)
          len=nxt[len];
        len=nxt[len];
        for(int i=0;i<len;i++)
          printf("%c",a[i]);
        if(len)
          printf(" ");
        printf("%d\n",len);
    }
    return 0;
}
```
