# hdu  1686  Oulipo（KMP） - HJ - CSDN博客
2017年01月18日 13:45:54[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：263
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------字符串---------																[KMP](https://blog.csdn.net/feizaoSYUACM/article/category/7238008)](https://blog.csdn.net/feizaoSYUACM/article/category/7238090)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
The French author Georges Perec (1936–1982) once wrote a book, La disparition, without the letter ‘e’. He was a member of the Oulipo group. A quote from the book:
Tout avait Pair normal, mais tout s’affirmait faux. Tout avait Fair normal, d’abord, puis surgissait l’inhumain, l’affolant. Il aurait voulu savoir où s’articulait l’association qui l’unissait au roman : stir son tapis, assaillant à tout instant son imagination, l’intuition d’un tabou, la vision d’un mal obscur, d’un quoi vacant, d’un non-dit : la vision, l’avision d’un oubli commandant tout, où s’abolissait la raison : tout avait l’air normal mais…
Perec would probably have scored high (or rather, low) in the following contest. People are asked to write a perhaps even meaningful text on some subject with as few occurrences of a given “word” as possible. Our task is to provide the jury with a program that counts these occurrences, in order to obtain a ranking of the competitors. These competitors often write very long texts with nonsense meaning; a sequence of 500,000 consecutive ‘T’s is not unusual. And they never use spaces.
So we want to quickly find out how often a word, i.e., a given string, occurs in a text. More formally: given the alphabet {‘A’, ‘B’, ‘C’, …, ‘Z’} and two finite strings over that alphabet, a word W and a text T, count the number of occurrences of W in T. All the consecutive characters of W must exactly match consecutive characters of T. Occurrences may overlap.
Input 
The first line of the input file contains a single number: the number of test cases to follow. Each test case has the following format:
One line with the word W, a string over {‘A’, ‘B’, ‘C’, …, ‘Z’}, with 1 ≤ |W| ≤ 10,000 (here |W| denotes the length of the string W). 
One line with the text T, a string over {‘A’, ‘B’, ‘C’, …, ‘Z’}, with |W| ≤ |T| ≤ 1,000,000.
Output 
For every test case in the input file, the output should contain a single number, on a single line: the number of occurrences of the word W in the text T.
Sample Input
3 
BAPC 
BAPC 
AZA 
AZAZAZA 
VERDI 
AVERDXIVYERDIAN
Sample Output
1 
3 
0
Source 
华东区大学生程序设计邀请赛_热身赛
Recommend 
lcy
统计匹配串出现次数，可以前后重叠，所以直接计数即可。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
int nxt[100005],n,m,cnt;
char a[10000005],b[100005];
void getNext()
{
    memset(nxt,0,sizeof(nxt));
    for(int i=1;i<m;i++)
    {
        int j=nxt[i];
        while(j&&b[i]!=b[j])
          j=nxt[j];
        nxt[i+1]=(b[i]==b[j])?j+1:0;
    }   
}
void KMP()
{
    n=strlen(a);
    m=strlen(b);
    getNext();
    for(int i=0,j=0;i<n;i++)
    {
        while(j&&a[i]!=b[j])
          j=nxt[j];
        if(a[i]==b[j])
          j++;
        if(j==m)     //只要出现匹配成功，不必担心前后两次匹配成功的子串是否有重叠部分 
          cnt++;
    }
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%s%s",b,a);
        cnt=0;
        KMP();
        printf("%d\n",cnt);
    }
    return 0;
}
```
