# hdu  3746  Cyclic Nacklace（KMP中next数组的运用） - HJ - CSDN博客
2017年01月18日 13:54:48[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：268
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------字符串---------																[KMP](https://blog.csdn.net/feizaoSYUACM/article/category/7238008)](https://blog.csdn.net/feizaoSYUACM/article/category/7238090)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
CC always becomes very depressed at the end of this month, he has checked his credit card yesterday, without any surprise, there are only 99.9 yuan left. he is too distressed and thinking about how to tide over the last days. Being inspired by the entrepreneurial spirit of “HDU CakeMan”, he wants to sell some little things to make money. Of course, this is not an easy task.
As Christmas is around the corner, Boys are busy in choosing christmas presents to send to their girlfriends. It is believed that chain bracelet is a good choice. However, Things are not always so simple, as is known to everyone, girl’s fond of the colorful decoration to make bracelet appears vivid and lively, meanwhile they want to display their mature side as college students. after CC understands the girls demands, he intends to sell the chain bracelet called CharmBracelet. The CharmBracelet is made up with colorful pearls to show girls’ lively, and the most important thing is that it must be connected by a cyclic chain which means the color of pearls are cyclic connected from the left to right. And the cyclic count must be more than one. If you connect the leftmost pearl and the rightmost pearl of such chain, you can make a CharmBracelet. Just like the pictrue below, this CharmBracelet’s cycle is 9 and its cyclic count is 2:
Now CC has brought in some ordinary bracelet chains, he wants to buy minimum number of pearls to make CharmBracelets so that he can save more money. but when remaking the bracelet, he can only add color pearls to the left end and right end of the chain, that is to say, adding to the middle is forbidden. 
CC is satisfied with his ideas and ask you for help.
Input 
The first line of the input is a single integer T ( 0 < T <= 100 ) which means the number of test cases. 
Each test case contains only one line describe the original ordinary chain to be remade. Each character in the string stands for one pearl and there are 26 kinds of pearls being described by ‘a’ ~’z’ characters. The length of the string Len: ( 3 <= Len <= 100000 ).
Output 
For each case, you are required to output the minimum count of pearls added to make a CharmBracelet.
Sample Input
3 
aaa 
abca 
abcde
Sample Output
0 
2 
5
Author 
possessor WC
Source 
HDU 3rd “Vegetable-Birds Cup” Programming Open Contest
Recommend 
lcy
题目大意：  
    给定一个字符串，问我们还需要添加几个字符可以构成一个由n个循环节组成的字符串。 
解法： 
先求出字符串的最小循环节的长度：
假设字符串的长度为len，那么最小的循环节就是cir = len-next[len] ; 
如果有len%cir == 0,那么这个字符串就是已经是完美的字符串，不用添加任何字符；
如果不是完美的那么需要添加的字符数就是cir - (len-(len/cir)*cir))，相当与需要在最后一个循环节上面添加几个。 
如果cir = 1，说明字符串只有一种字符例如“aaa” ; 
如果m%cir == 0，说明已经不用添加了。 
注意在求next数组的时候有两种情况：
严蔚敏老师在数据结构中的代码是这样的：
```
void getNext()  
{
    int i=0,j=-1;
    nxt[0]=-1;
    while(i<len)
    {
        if(j==-1||s[i]==s[j])
          nxt[++i]=++j;
        else
          j=nxt[j];
    }
}
```
但是如果需要用在KMP的时候（即需要和母串匹配的时候），为了保证k足够大，匹配不成功的时候跳跃性大，即减少中间没有匹配过的误差和时间复杂度，代码应该是这样的：
```
void getNext()
{
    int i=0,j=-1;
    nxt[0]=-1;
    while(i<len)
    {
        if(j==-1||s[i]==s[j])
        {
            i++,j++;
            if(s[i]!=s[j])
              nxt[i]=j;
            else
              nxt[i]=nxt[j];
        }
        else
          j=nxt[j];
    }
}
```
下面给出两种版本放到整个程序中的代码，测试母串abcabca，数据如下，读者可以在纸上模拟一下。
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
char s[100005];
int nxt[100005],len; 
/*
abcabca
-1 0 0 -1 0 0 -1 4
2
*/
//void getNext()
//{
//  int i=0,j=-1;
//  nxt[0]=-1;
//  while(i<len)
//  {
//      if(j==-1||s[i]==s[j])
//      {
//          i++,j++;
//          if(s[i]!=s[j])
//            nxt[i]=j;
//          else
//            nxt[i]=nxt[j];
//      }
//      else
//        j=nxt[j];
//  }
//}
/*
abcabca
-1 0 0 0 1 2 3 4
2
*/
void getNext()  
{
    int i=0,j=-1;
    nxt[0]=-1;
    while(i<len)
    {
        if(j==-1||s[i]==s[j])
          nxt[++i]=++j;
        else
          j=nxt[j];
    }
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%s",s);
        len=strlen(s);
        getNext();
//      for(int i=0;i<=len;i++)
//        printf("%d ",nxt[i]);
//      printf("\n");
        int length=len-nxt[len];
        if(len!=length&&len%length==0)
          printf("0\n");
        else
          printf("%d\n",length-nxt[len]%length);
    }
    return 0;
}
```
