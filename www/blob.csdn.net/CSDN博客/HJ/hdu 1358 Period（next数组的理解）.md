# hdu  1358 Period（next数组的理解） - HJ - CSDN博客
2017年01月19日 01:29:47[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：193
Problem Description 
For each prefix of a given string S with N characters (each character has an ASCII code between 97 and 126, inclusive), we want to know whether the prefix is a periodic string. That is, for each i (2 <= i <= N) we want to know the largest K > 1 (if there is one) such that the prefix of S with length i can be written as AK , that is A concatenated K times, for some string A. Of course, we also want to know the period K.
Input 
The input file consists of several test cases. Each test case consists of two lines. The first one contains N (2 <= N <= 1 000 000) – the size of the string S. The second line contains the string S. The input file ends with a line, having the number zero on it.
Output 
For each test case, output “Test case #” and the consecutive test case number on a single line; then, for each prefix with length i that has a period K > 1, output the prefix size i and the period K separated by a single space; the prefix sizes must be in increasing order. Print a blank line after each test case.
Sample Input
3 
aaa 
12 
aabaabaabaab 
0
Sample Output
Test case #1 
2 2 
3 3
Test case #2 
2 2 
6 2 
9 3 
12 4
题目大意： 
    一个字符串，从头到某个位置，字符串的前缀最多重复了多少次
解法： 
    判断循环节
```
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
char s[1000005];
int n,nxt[1000005];
void getNext()
{
    nxt[0]=nxt[1]=0;
    int i=1,j=0;
    while(i<=n)
    {
        if(j==0||s[i]==s[j])
          nxt[++i]=++j;
        else
          j=nxt[j];
    }
}
int main()
{
    int t=1;
    while(scanf("%d",&n)&&n)
    {
        scanf("%s",s+1);
        getNext();
        printf("Test case #%d\n",t++);
//      for(int i=1;i<=n+1;i++)
//        printf("%d ",nxt[i]);
//      printf("\n");
        for(int i=2;i<=n;i++)
        {
            int k=i-(nxt[i+1]-1);    //k表示长度
//          printf("%d\n",k);
            if(i!=k&&i%k==0)        //这里注意理解
              printf("%d %d\n",i,i/k);
        } 
        printf("\n");
    } 
    return 0;
}
```
