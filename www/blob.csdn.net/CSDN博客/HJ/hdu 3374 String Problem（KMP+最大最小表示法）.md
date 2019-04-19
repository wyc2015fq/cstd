# hdu  3374  String Problem（KMP+最大最小表示法） - HJ - CSDN博客
2017年01月18日 01:49:49[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：382
Problem Description 
Give you a string with length N, you can generate N strings by left shifts. For example let consider the string “SKYLONG”, we can generate seven strings: 
String Rank 
SKYLONG 1 
KYLONGS 2 
YLONGSK 3 
LONGSKY 4 
ONGSKYL 5 
NGSKYLO 6 
GSKYLON 7 
and lexicographically first of them is GSKYLON, lexicographically last is YLONGSK, both of them appear only once. 
  Your task is easy, calculate the lexicographically fisrt string’s Rank (if there are multiple answers, choose the smallest one), its times, lexicographically last string’s Rank (if there are multiple answers, choose the smallest one), and its times also.
Input 
  Each line contains one line the string S with length N (N <= 1000000) formed by lower case letters.
Output 
Output four integers separated by one space, lexicographically fisrt string’s Rank (if there are multiple answers, choose the smallest one), the string’s times in the N generated strings, lexicographically last string’s Rank (if there are multiple answers, choose the smallest one), and its times also.
Sample Input
abcder 
aaaaaa 
ababab
Sample Output
1 1 6 1 
1 6 1 6 
1 3 2 3
Author 
WhereIsHeroFrom
题目大意： 
    给一个串，要求输出这个串的最大和最小表示，以及数量（同构串个数）。
什么是最大最小表示？就是将字符串不断旋转，得到字典序最大或者最小的。
而同构串个数就是利用KMP求出来的next数组来求循环节
```cpp
#include<iostream>
#include<cstdio>
#include<cstring> 
using namespace std;
const int N=10000005;
char s[N];
int nxt[N],len;
void Getnext()             //KMP算法中next数组求循环节 
{
    nxt[0]=-1;
    int i=0,j=-1;
    while(i<len)
    {
        if(j==-1||s[i]==s[j])
        {
            i++;j++;
            if(s[i]==s[j])
              nxt[i]=nxt[j];
            else
              nxt[i]=j; 
        }
        else
          j=nxt[j];
    }
}
int Get_min_max(int x)       //循环字符同构串最大最小表示法 
{
    int i=0,j=1,k=0;
    while(i<len&&j<len&&k<len)
    {
        int t=s[(i+k)%len]-s[(j+k)%len];
        if(!t)  k++;
        else
        {
            if(x)              //x==1 即最大表示 
              if(t>0) j+=k+1;
              else  i+=k+1;
            else              //否则x==0 即最小表示 
              if(t>0)  i+=k+1;
              else j+=k+1;
            if(i==j)  j++;
            k=0; 
        }
    }
    return min(i,j);
}
int main()
{
    while(scanf("%s",s)!=EOF)
    {
        len=strlen(s);
        Getnext();
        int min=Get_min_max(0)+1;
        int max=Get_min_max(1)+1;
        int k=len-nxt[len];
        int ans=len%k?1:len/k;
        printf("%d %d %d %d\n",min,ans,max,ans);
    }
    return 0;   
}
```
