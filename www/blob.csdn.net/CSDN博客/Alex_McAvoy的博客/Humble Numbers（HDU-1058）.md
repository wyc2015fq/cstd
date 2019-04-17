# Humble Numbers（HDU-1058） - Alex_McAvoy的博客 - CSDN博客





2018年03月26日 20:42:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：73
个人分类：[HDU																[搜索——暴力搜索及打表](https://blog.csdn.net/u011815404/article/category/7925875)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    A number whose only prime factors are 2,3,5 or 7 is called a humble number. The sequence 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 15, 16, 18, 20, 21, 24, 25, 27, ... shows the first 20 humble numbers. 

    Write a program to find and print the nth element in this sequence.

# Input

    The input consists of one or more test cases. Each test case consists of one integer n with 1 <= n <= 5842. Input is terminated by a value of zero (0) for n.

# Output

    For each test case, print one line saying "The nth humble number is number.". Depending on the value of n, the correct suffix "st", "nd", "rd", or "th" for the ordinal number nth has to be used like it is shown in the sample output.

# Sample Input

**1**

**234111213212223100100058420**

# Sample Output

**The 1st humble number is 1.The 2nd humble number is 2.The 3rd humble number is 3.The 4th humble number is 4.The 11th humble number is 12.The 12th humble number is 14.The 13th humble number is 15.The 21st humble number is 28.The 22nd humble number is 30.The 23rd humble number is 32.The 100th humble number is 450.The 1000th humble number is 385875.The 5842nd humble number is 2000000000.**

————————————————————————————————————————————

思路： 


- 状态方程：取2*num[p2]、3*num[p3]、5*num[p5]、7*num[p7]四者中的最小值。如果当前的值num[n]==四者中的一个，则令pi++。
- 将所有的5842个丑数都求出来，放到一个数组中，然后调用即可。
- 注意输出格式，除了11,12,13外，其余序数词的尾数：1→st，2→nd，3→rd，其余为th。

# Source Program

```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
#define N 5842
using namespace std;

int num[N];
int dp[N];

int min(int a,int b,int c,int d)
{
    int temp1,temp2;

    temp1=a<b?a:b;
    temp2=c<d?c:d;

    return temp1<temp2?temp1:temp2;
}

int main()
{
    int n;
    int i,j;
    int p2,p3,p5,p7;

    num[1]=1;
    p2=p3=p5=p7=1;
    for(i=1;num[i]<2000000000;)//有题设可知，n最大到5842；由所给输出样例可知，n为5842时，丑数为2000000000
    {
        i++;
        num[i]=min(2*num[p2],3*num[p3],5*num[p5],7*num[p7]);//寻找四者中的最小值存在数组中
        /*如果当前的值num[n]==四者中的一个，则令pi++*/
        if(num[i]==2*num[p2])
            p2++;
        if(num[i]==3*num[p3])
            p3++;
        if(num[i]==5*num[p5])
            p5++;
        if(num[i]==7*num[p7])
            p7++;
    }

    while(cin>>n && n)
    {
        if(n%10==1&&n%100!=11)//尾数为1且不是11时
            printf("The %dst humble number is %d.",n,num[n]);
        else if(n%10==2&&n%100!=12)//尾数为2且不是12时
            printf("The %dnd humble number is %d.",n,num[n]);
        else if(n%10==3&&n%100!=13)//尾数为3且不是13时
            printf("The %drd humble number is %d.",n,num[n]);
        else
            printf("The %dth humble number is %d.",n,num[n]);

        printf("\n");
    }
    return 0;
}
```






