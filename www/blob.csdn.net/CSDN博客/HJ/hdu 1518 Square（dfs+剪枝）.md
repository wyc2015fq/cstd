# hdu  1518  Square（dfs+剪枝） - HJ - CSDN博客
2017年06月30日 10:47:56[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：293
Problem Description 
Given a set of sticks of various lengths, is it possible to join them end-to-end to form a square?
Input 
The first line of input contains N, the number of test cases. Each test case begins with an integer 4 <= M <= 20, the number of sticks. M integers follow; each gives the length of a stick - an integer between 1 and 10,000.
Output 
For each case, output a line containing “yes” if is is possible to form a square; otherwise output “no”.
Sample Input
3 
4 1 1 1 1 
5 10 20 30 40 50 
8 1 7 2 6 4 4 3 5
Sample Output
yes 
no 
yes
题目大意： 
把所有棍子拼来，看能不能构造出一个正方形？
解题思路： 
应当注意以下几点：
```
1）所有棍子都要用到，不能剩余
2）输入已经保证大于4根棍子了。所以无需判断可能小于3根棍子的情况
3）棍长的总数首先要是4的倍数，才能进行。否则直接输出 “no”
4）当前面前提满足以后，再满足3 根棍子拼好，就完工了。最后一根一定能拼好。
```
用dfs解决，关键在与剪枝！
```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
using namespace std;
int len,n;
struct node{
    int length;         //表示木棒的长度 
    int blag;       //表示木棒是否使用 
}s[25];
int cmp(node a,node b)       //将木棒长度按照降序排列 
{
    return a.length>b.length;
}
int dfs(int l,int count,int pos)
{
    if(count==4)
      return 1;
    for(int i=pos;i<n;i++)
    {
        if(s[i].blag==0)
        {
            if(len==l+s[i].length)      //加上当前木棒的长度刚好为正方形边长 
            {
                s[i].blag=1;
                if(dfs(0,count+1,0))      //如果继续搜索能够构成四条边则返回1 
                  return 1;
                s[i].blag=0;           //否则消除当前木棒的选择标记 
            }
            else if(len>l+s[i].length)      //加上当前木棒的长度仍然不够正方形边长 
            {
                s[i].blag=1;
                l+=s[i].length;
                if(dfs(l,count,0))
                  return 1;
                l-=s[i].length;
                s[i].blag=0;
            } 
        }
    } 
    return 0;
} 
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d",&n);
        len=0;
        for(int i=0;i<n;i++)
        {
            scanf("%d",&s[i].length);
            s[i].blag=0;
            len+=s[i].length;
        }
        if(n<4||len%4)
        {
            printf("no\n");
            continue;
        }
        len/=4;
        sort(s,s+n,cmp);
        if(dfs(0,0,0)) 
          printf("yes\n");
        else
          printf("no\n"); 
    } 
    return 0;
}
```
