# hdu 1862 EXCEL排序 - HJ - CSDN博客
2017年03月06日 23:52:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：191
Excel可以对一组纪录按任意指定列排序。现请你编写程序实现类似功能。 
Input 
    测试输入包含若干测试用例。每个测试用例的第1行包含两个整数 N (<=100000) 和 C，其中 N 是纪录的条数，C 是指定排序的列号。以下有 N 行，每行包含一条学生纪录。每条学生纪录由学号（6位数字，同组测试中没有重复的学号）、姓名（不超过8位且不包含空格的字符串）、成绩（闭区间0,100内的整数）组成，每个项目间用1个空格隔开。当读到 N=0 时，全部输入结束，相应的结果不要输出。
Output 
    对每个测试用例，首先输出1行“Case i:”，其中 i 是测试用例的编号（从1开始）。随后在 N 行中输出按要求排序后的结果，即：当 C=1 时，按学号递增排序；当 C=2时，按姓名的非递减字典序排序；当 C=3 
    时，按成绩的非递减排序。当若干学生具有相同姓名或者相同成绩时，则按他们的学号递增排序。
Sample Input
```
3 1
000007 James 85
000010 Amy 90
000001 Zoe 60
4 2
000007 James 85
000010 Amy 90
000001 Zoe 60
000002 James 98
4 3
000007 James 85
000010 Amy 90
000001 Zoe 60
000002 James 90
0 0
```
Sample Output
```
Case 1:
000001 Zoe 60
000007 James 85
000010 Amy 90
Case 2:
000010 Amy 90
000002 James 98
000007 James 85
000001 Zoe 60
Case 3:
000001 Zoe 60
000007 James 85
000002 James 90
000010 Amy 90
```
排序的时候注意：学号排名之前，需要化成整数，存入结构体中。
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdio>
using namespace std;
struct node{
    char a[15],b[15];
    int n,sum;
}s[100005];
int cmp1(node x,node y)
{
    return x.sum<y.sum;
}
int cmp2(node x,node y)
{
    int blag=strcmp(x.b,y.b);
    if(blag==0)
      return x.sum<y.sum;
    return strcmp(x.b,y.b)<0;
}
int cmp3(node x,node y)
{
    if(x.n==y.n)
      return x.sum<y.sum;
    return x.n<y.n;
}
int main()
{
    int c,m,tt=0,j;
    while(cin>>m>>c)
    {
        if(c==0&&m==0)
          break;
        for(int i=0;i<m;i++)
        { 
            cin>>s[i].a>>s[i].b>>s[i].n;
            for(j=0;s[i].a[j]!='\0';j++)
              if(s[i].a[j]!='0')
                break;
            s[i].sum=0;
            for(;s[i].a[j]!='\0';j++)
              s[i].sum=s[i].sum*10+s[i].a[j]-'0';
        }
        if(c==1)
          sort(s,s+m,cmp1);
        else if(c==2)
          sort(s,s+m,cmp2);
        else
          sort(s,s+m,cmp3);
        printf("Case %d:\n",++tt);
        for(int i=0;i<m;i++)
          printf("%s %s %d\n",s[i].a,s[i].b,s[i].n);
    }
    return 0;
}
```
