# 【暴力几何】#6 A. Triangle - CD's Coding - CSDN博客





2014年03月31日 21:47:49[糖果天王](https://me.csdn.net/okcd00)阅读数：601
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Johnny has a younger sister Anne, who is very clever and smart. As she came home from the kindergarten, she told his brother about the task that her kindergartener asked her to solve. The task was just to construct a triangle out of four sticks of different
 colours. Naturally, one of the sticks is extra. It is not allowed to break the sticks or use their partial length. Anne has perfectly solved this task, now she is asking Johnny to do the same.


The boy answered that he would cope with it without any difficulty. However, after a while he found out that different tricky things can occur. It can happen that it is impossible to construct a triangle of a positive area, but it is possible to construct a
 degenerate triangle. It can be so, that it is impossible to construct a degenerate triangle even. As Johnny is very lazy, he does not want to consider such a big amount of cases, he asks you to help him.




Input


The first line of the input contains four space-separated positive integer numbers not exceeding 100 — lengthes of the sticks.




Output


Output TRIANGLE if it is possible to construct a non-degenerate triangle. Output SEGMENT if
 the first case cannot take place and it is possible to construct a degenerate triangle. Output IMPOSSIBLE if it is impossible to construct any triangle. Remember
 that you are to use three sticks. It is not allowed to break the sticks or use their partial length.




Sample test(s)




input
4 2 1 3




output
TRIANGLE




input
7 2 2 4




output
SEGMENT




input
3 5 9 1




output
IMPOSSIBLE










官方标签都是：暴力、几何了……我还能说什么……这道题需要知道的就是三角形两边之和大于第三边

贴的是咖喱（ZoeCUR）的C语言解法【反正是我家的，就是我的】，其实没太大差啦，这道题就这样啦~水题可跳



```cpp
#include<stdio.h>
int main()
{
    int a[4];
    for(int i=0;i<4;i++)scanf("%d",&a[i]);
    for(int j=0;j<4;j++)
      { for(int k=j+1;k<4;k++)
     {
                  if(a[k]<a[j])
                  {
                               int temp;
                               temp=a[j];
                               a[j]=a[k];
                               a[k]=temp;
                  }
     }}
     if((a[0]+a[1])<a[2]&&a[0]+a[2]<a[3]&&a[1]+a[2]<a[3])printf("IMPOSSIBLE");
     else if(a[0]+a[1]>a[2]||a[0]+a[2]>a[3]||a[1]+a[2]>a[3])printf("TRIANGLE");
     else printf("SEGMENT");
     return 0;
}
```







