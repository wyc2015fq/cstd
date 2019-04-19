# hdu 2058  The sum problem（公式） - HJ - CSDN博客
2016年08月12日 20:53:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：288
Problem Description 
Given a sequence 1,2,3,……N, your job is to calculate all the possible sub-sequences that the sum of the sub-sequence is M.
Input 
Input contains multiple test cases. each case contains two integers N, M( 1 <= N, M <= 1000000000).input ends with N = M = 0.
Output 
For each test case, print all the possible sub-sequence that its sum is M.The format is show in the sample below.print a blank line after each test case.
Sample Input 
20 10 
50 30 
0 0
Sample Output 
[1,4] 
[10,10]
[4,8] 
[6,9] 
[9,11] 
[30,30]
解析：这个题目如果按一般思路来遍历肯定会超时，注意利用等差数列求和公式约束循环的范围是解决本题关键，详细解析代码如下：
```
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
int main()
{
    int n,m,sum,i,j;
    while(cin>>n>>m)
    {
        if(n==0&&m==0)  break;
        for(int i=sqrt(m*2);i>0;i--)     //Sn=a1*n+n*(n-1)/2,当a1=1的时候，n最大，此时 Sn*2=n*(n+1),所以n<sqrt(Sn*2) 
        {
            int sum=(m-(i*i+i)/2);          //a1*n+n*(n-1)/2=Sn
            if(sum%i==0)
              printf("[%d,%d]\n",sum/i+1,sum/i+i);    //sum=(a1-1)*i 
        }
        printf("\n"); 
    }
    return 0;
}
```
