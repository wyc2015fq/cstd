# NBUT  1670  字符串（strstr函数的处理） - HJ - CSDN博客
2016年08月19日 19:24:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：228
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description
给两个字符串p和s，如果字符串s包含了字符串p，输出字符串p在字符串s的起始位置（如果含有多个p，输出最小一个），不包含则输出-1。
Input 
第一行 T（表示测试数据），每个测试数据有两行，第一行s（字符长不超过1000000），第二行p（字符长不超过10000）。
Output 
输出p在s中最早出现的位置，如果p没有在s中出现过，输出-1.
Sample Input
2 
ababcabcacbab 
abcac 
ababcabcacbab 
abcba
Sample Output
6 
-1
这个题目比较坑，一定要用到字符串处理函数strstr，用KMP都会超时！！！而且还要用Scanf作输入不能用cin。
关于strstr函数的知识可以上百度搜搜。
代码如下：
```cpp
#include<stdio.h>
#include<string.h>
char a[1000005],b[10005];
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%s %s",a,b);
        int len1=strlen(a);
        int len2=strlen(b);
        char *tt;
        tt=strstr(a,b);     //strstr可以从前串中寻找出子串第一个字符在前串中的位置，时间复杂度为O(n)
        if(tt==NULL)          //注意子串为空以及前串中不存在子串的情况
           printf("-1\n");
        else
        {   
            int len3=strlen(tt);
            printf("%d\n",len1-len3+1);
        }
    }
    return 0;
}
```
