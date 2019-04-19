# hdu  4300  Clairewd’s message（KMP） - HJ - CSDN博客
2017年01月19日 01:24:11[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：291
个人分类：[====ACM====																[--------字符串---------																[KMP																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238008)](https://blog.csdn.net/feizaoSYUACM/article/category/7238090)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Clairewd is a member of FBI. After several years concealing in BUPT, she intercepted some important messages and she was preparing for sending it to ykwd. They had agreed that each letter of these messages would be transfered to another one according to a conversion table. 
Unfortunately, GFW(someone’s name, not what you just think about) has detected their action. He also got their conversion table by some unknown methods before. Clairewd was so clever and vigilant that when she realized that somebody was monitoring their action, she just stopped transmitting messages. 
But GFW knows that Clairewd would always firstly send the ciphertext and then plaintext(Note that they won’t overlap each other). But he doesn’t know how to separate the text because he has no idea about the whole message. However, he thinks that recovering the shortest possible text is not a hard task for you. 
Now GFW will give you the intercepted text and the conversion table. You should help him work out this problem.
Input 
The first line contains only one integer T, which is the number of test cases. 
Each test case contains two lines. The first line of each test case is the conversion table S. S[i] is the ith latin letter’s cryptographic letter. The second line is the intercepted text which has n letters that you should recover. It is possible that the text is complete. 
Hint 
Range of test data: 
T<= 100 ; 
n<= 100000;
Output 
For each test case, output one line contains the shorest possible complete text.
Sample Input
2 
abcdefghijklmnopqrstuvwxyz 
abcdab 
qwertyuiopasdfghjklzxcvbnm 
qwertabcde
Sample Output
abcdabcd 
qwertabcde
Author 
BUPT
Source 
2012 Multi-University Training Contest 1
题目大意： 
    给定一个翻译表，即第i个字母用哪个字母表示 
再给一个串，里面前面为密文，后面为明文，密文一定是完整的，但明文不完整或可能没有 
求这个完整的前面密文后面明文的串 
把给定的串全部按表翻译为密文，
解法： 
      题目要求求最短长度， 从B的一半开始， 当找到第一个满足 B[i]== len- i条件的，i即为第一个明文的位置  
      然后分段输出即可。
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
char a[100005],b[100005],s[30];
int nxt[100005];
void getNext()
{
    int i=0,j=-1;
    nxt[0]=-1;
    while(a[i])
    {
        if(j==-1||a[i]==a[j])
          nxt[++i]=++j;
        else
          j=nxt[j];
    }
} 
int KMP()
{
    int i=0,j=0,len1=strlen(b),len2=strlen(a);
    getNext();
    while(i<len1&&j<len2)         //以b为母串a为模式串匹配 
    {
        if(j==-1||b[i]==a[j])
        {
            i++,j++;
            if(i==len1)
              return j;
        }
        else
          j=nxt[j];
    }
    return 0;
}
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        cin>>s>>a;
        int len=strlen(a);
        strcpy(b,a+(len+1)/2);
        cout<<a;
        for(int i=0;a[i];i++)
          for(int j=0;j<26;j++)
            if(a[i]==s[j])
            {
                a[i]='a'+j;
                break;
            }
        int cnt=KMP();
        for(int i=cnt;i<len-cnt;i++)
          cout<<a[i];
        cout<<endl;
    } 
    return 0;
}
```
