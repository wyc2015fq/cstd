# Codeforces1005D——Polycarp and Div 3 - westbrook1998的博客 - CSDN博客





2018年08月08日 22:49:59[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：41








> 
Polycarp likes numbers that are divisible by 3. 

  He has a huge number s. Polycarp wants to cut from it the maximum number of numbers that are divisible by 3. To do this, he makes an arbitrary number of vertical cuts between pairs of adjacent digits. As a result, after m such cuts, there will be m+1 parts in total. Polycarp analyzes each of the obtained numbers and finds the number of those that are divisible by 3. 

  For example, if the original number is s=3121, then Polycarp can cut it into three parts with two cuts: 3|1|21. As a result, he will get two numbers that are divisible by 3. 

  Polycarp can make an arbitrary number of vertical cuts, where each cut is made between a pair of adjacent digits. The resulting numbers cannot contain extra leading zeroes (that is, the number can begin with 0 if and only if this number is exactly one character ‘0’). For example, 007, 01 and 00099 are not valid numbers, but 90, 0 and 10001 are valid. 

  What is the maximum number of numbers divisible by 3 that Polycarp can obtain? 

  Input 

  The first line of the input contains a positive integer s. The number of digits of the number s is between 1 and 2⋅105, inclusive. The first (leftmost) digit is not equal to 0. 

  Output 

  Print the maximum number of numbers divisible by 3 that Polycarp can get by making vertical cuts in the given number s. 

  Examples 

  Input 

  3121 

  Output 

  2 

  Input 

  6 

  Output 

  1 

  Input 

  1000000000000000000000000000000000 

  Output 

  33 

  Input 

  201920181 

  Output 

  4 

  Note 

  In the first example, an example set of optimal cuts on the number is 3|1|21. 

  In the second example, you do not need to make any cuts. The specified number 6 forms one number that is divisible by 3. 

  In the third example, cuts must be made between each pair of digits. As a result, Polycarp gets one digit 1 and 33 digits 0. Each of the 33 digits 0 forms a number that is divisible by 3. 

  In the fourth example, an example set of optimal cuts is 2|0|1|9|201|81. The numbers 0, 9, 201 and 81 are divisible by 3.
同学搞出来的dp 看的我一愣一愣的

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=200020;
int dp[MAXN];
char s[MAXN];
int main(void){
    scanf("%s",s);
    int len=strlen(s);
    if((s[0]-'0')%3==0){
        dp[1]=1;
    }
    else{
        dp[1]=0;
    }
    //dp时主要是要考虑当前判断的位置是否与前面组成3的倍数
    for(int i=2;i<=len;i++){
        int f=0;
        dp[i]=dp[i-1];
        int sum=0;
        int j;
        for(j=i;j>=1;j--){
            sum+=s[j-1]-'0';
            if(sum%3==0){
                f=1;
                //跳出时则记录当前的j 也就是从i往左到j这段是3的倍数
                break;
            }
            //如果dp[j-1]<dp[i-1] 则没有必要再继续往左走
            //因为最多加上i往左到j这一段也就是和dp[i-1]相等 没必要
            else if(dp[j-1]<dp[i-1]){
                break;
            }
        }
        if(f){
            //那这里更新的就应该以dp[j-1]为基础了
            dp[i]=dp[j-1]+1;
            printf("dp//%d\n",dp[4]);
        }
    }
    printf("%d\n",dp[len]);
    return 0;
}
```





