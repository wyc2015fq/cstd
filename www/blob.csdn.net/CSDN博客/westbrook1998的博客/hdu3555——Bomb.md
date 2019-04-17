# hdu3555——Bomb - westbrook1998的博客 - CSDN博客





2018年08月17日 18:22:04[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：40标签：[dp																[数位dp](https://so.csdn.net/so/search/s.do?q=数位dp&t=blog)](https://so.csdn.net/so/search/s.do?q=dp&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
The counter-terrorists found a time bomb in the dust. But this time the terrorists improve on the time bomb. The number sequence of the time bomb counts from 1 to N. If the current number sequence includes the sub-sequence “49”, the power of the blast would add one point.  

  Now the counter-terrorist knows the number N. They want to know the final points of the power. Can you help them?  

  Input 

  The first line of input consists of an integer T (1 <= T <= 10000), indicating the number of test cases. For each test case, there will be an integer N (1 <= N <= 2^63-1) as the description.  

  The input terminates by end of file marker.  

  Output 

  For each test case, output an integer indicating the final points of the power. 

  Sample Input 

  3 

  1 

  50 

  500 

  Sample Output 

  0 

  1 

  15 

  Hint 

  From 1 to 500, the numbers that include the sub-sequence “49” are “49”,”149”,”249”,”349”,”449”,”490”,”491”,”492”,”493”,”494”,”495”,”496”,”497”,”498”,”499”, 

  so the answer is 15.
数位dp入门题，挺神奇的，不过还没学到要点 

代码里面最后注释的那个判断不知道为什么不能放进else里面，wa了 求路过的大佬解答一下

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int dig[25];
//dp[i][0]表示i位数不含49的个数
//dp[i][1]表示i位数不含49且最高位为9的个数
//dp[i][2]表示i位数含有49的个数
long long dp[25][3];
long long n;
void init(){
    //虽然不知道这里为什么要1
    dp[0][0]=1;
    for(int i=1;i<=20;i++){
        //i位数不含49的个数就等于i-1位数不含49的个数*10（因为第i位可以放0-9）
        //再减去i-1位数不含49但最高位为9的情况，（这种情况第i位加个4即可）
        dp[i][0]=dp[i-1][0]*10-dp[i-1][1];
        //i位数不含49且最高位为9的个数就等于i-1位不含49的在第i位加一个9
        dp[i][1]=dp[i-1][0];
        //i位数含49的个数就等于i-1位数含49的个数*10（因为第i位可以放0-9）
        //再加上i-1位数不含49但最高位为9的情况，（这种情况第i位加个4即可）
        //与第一种情况类似
        dp[i][2]=dp[i-1][2]*10+dp[i-1][1];
    }
}
long long solve(long long num){
    memset(dig,0,sizeof(dig));
    int k=0;
    while(num){
        dig[++k]=num%10;
        num/=10;
    }
    //dig[k+1]=0;
    long long ans=0;
    //标志是否第i位和第i+1是否是49
    //也就是dig[i]==9 dig[i+1]==4
    bool flag=false;
    //每一位的dp加起来
    for(int i=k;i>=1;i--){
        //后面i-1位已经包含49，第i位可以有dig[i]种选择
        ans+=dp[i-1][2]*dig[i];
        if(flag){
            //第i位和第i+1是49,那么i-1位那些不含49的也算
            ans+=dp[i-1][0]*dig[i];
        }
        else{
            //第i位大于4,那么就可以选4,后面选第i-1位是9的
            if(dig[i]>4){
                ans+=dp[i-1][1];
            }

        }
        //这个判断如果放在上面else里就会WA  ？？？
        //因为这个标志
        //第i位和前面第i+1位可以凑成49，标记
        if(dig[i]==9 && dig[i+1]==4){
            flag=true;
        }
    }
    return ans;
}
int main(void){
    int t;
    long long n;
    init();
    scanf("%d",&t);
    while(t--){
        scanf("%lld",&n);
        printf("%lld\n",solve(n+1));
    }
    return 0;
}
```






