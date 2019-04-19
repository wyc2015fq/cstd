# POJ  2247  Humble Numbers（dp） - HJ - CSDN博客
2016年08月11日 13:23:35[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：251
Description 
A number whose only prime factors are 2,3,5 or 7 is called a humble number. The sequence 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 15, 16, 18, 20, 21, 24, 25, 27, … shows the first 20 humble numbers. 
Write a program to find and print the nth element in this sequence.  
Input 
The input consists of one or more test cases. Each test case consists of one integer n with 1 <= n <= 5842. Input is terminated by a value of zero (0) for n. 
Output 
For each test case, print one line saying “The nth humble number is number.”. Depending on the value of n, the correct suffix “st”, “nd”, “rd”, or “th” for the ordinal number nth has to be used like it is shown in the sample output.
Sample Input 
1 
2 
3 
4 
11 
12 
13 
21 
22 
23 
100 
1000 
5842 
0
Sample Output 
The 1st humble number is 1. 
The 2nd humble number is 2. 
The 3rd humble number is 3. 
The 4th humble number is 4. 
The 11th humble number is 12. 
The 12th humble number is 14. 
The 13th humble number is 15. 
The 21st humble number is 28. 
The 22nd humble number is 30. 
The 23rd humble number is 32. 
The 100th humble number is 450. 
The 1000th humble number is 385875. 
The 5842nd humble number is 2000000000.
解析：本题主要的问题就是数据打表，因为每个数都是能被2、3、5、7其中至少一个数整除，并且不含有其他素因子，因此初始条件从2、3、5、7开始，累乘赋值。
思路：这个题目最dt的就是打表还要带排序，因此方法思路很重要，用dp记录当前值，每一次取累乘2、3、5、7四个数的中最小值，层层赋值，达到排序的目的，详细的请看如下代码：
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    long long dp[6000],s[4]={2,3,5,7},i,j,k;   //把2,3,5,7放到数组里方便相乘 
    dp[1]=1;       //初始值为1 
    for(i=2;i<=5842;i++)
    {
        dp[i]=2000000001;    //赋边界值方便比较 ，下面的循环得到的值给dp数组是按照从小到大的顺序
        for(j=0;j<4;j++)
          for(k=i-1;k>=1;k--)
          {
              if(dp[k]*s[j]<=dp[i-1]) break;     //跳出条件：如果相乘不比数组dp里的前一个数大则直接跳出 
              if(dp[k]*s[j]<dp[i])
                dp[i]=dp[k]*s[j];     //把乘以2,3,5,7的大小分别作比较，把较小的先赋值，起到一个给dp排序的作用 
          }
    }
    int n;
    while(cin>>n)
    {
        if(n==0) break;
        if(n%10==1&&n%100!=11)                        //以下均为控制输出格式 
          printf("The %dst humble number is ",n);
        else if(n%10==2&&n%100!=12)
          printf("The %dnd humble number is ",n);
        else if(n%10==3&&n%100!=13)
          printf("The %drd humble number is ",n);
        else
          printf("The %dth humble number is ",n);
        cout<<dp[n]<<"."<<endl;
    }
    return 0;
}
```
