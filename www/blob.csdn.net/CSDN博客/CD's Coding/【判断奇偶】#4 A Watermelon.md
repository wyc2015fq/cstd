# 【判断奇偶】#4 A. Watermelon - CD's Coding - CSDN博客





2014年03月31日 21:23:45[糖果天王](https://me.csdn.net/okcd00)阅读数：580标签：[codeforces																[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)












One hot summer day Pete and his friend Billy decided to buy a watermelon. They chose the biggest and the ripest one, in their opinion. After that the watermelon was weighed, and the scales showed *w* kilos.
 They rushed home, dying of thirst, and decided to divide the berry, however they faced a hard problem.


Pete and Billy are great fans of even numbers, that's why they want to divide the watermelon in such a way that each of the two parts weighs even number of kilos, at the same time it is not obligatory that the parts are equal. The boys are extremely tired and
 want to start their meal as soon as possible, that's why you should help them and find out, if they can divide the watermelon in the way they want. For sure, each of them should get a part of positive weight.




Input


The first (and the only) input line contains integer number *w* (1 ≤ *w* ≤ 100)
 — the weight of the watermelon bought by the boys.




Output


Print YES, if the boys can divide the watermelon into two parts, each of them weighing even number of kilos; and NO in
 the opposite case.




Sample test(s)




input
8




output
YES






Note


For example, the boys can divide the watermelon into two parts of 2 and 6 kilos respectively (another variant — two parts of 4 and 4 kilos).





题目的意思是两个熊孩子分西瓜，但是他们都喜欢偶数，所以他们要偶数重量的西瓜……

所以……除以二咯~ 记得特殊情况2虽然是偶数但是不能别分成2个偶数就行了~ 大 水 题 ^_^

没有什么太多想说的，很多题只需要理解题意就好其实……



```cpp
#include<stdio.h>  
int main(void)  
{
    int w;
    scanf("%d",&w);
    if(w==2)
    printf("NO");
    else if(w%2)
              printf("NO");
         else printf("YES");
    return 0;
}
```








