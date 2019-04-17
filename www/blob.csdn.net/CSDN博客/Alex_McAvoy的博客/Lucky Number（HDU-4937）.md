# Lucky Number（HDU-4937） - Alex_McAvoy的博客 - CSDN博客





2019年01月19日 15:02:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：37
个人分类：[HDU																[数论——方程](https://blog.csdn.net/u011815404/article/category/8796916)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

“Ladies and Gentlemen, It’s show time! ” 

“A thief is a creative artist who takes his prey in style... But a detective is nothing more than a critic, who follows our footsteps...” 

Love_Kid is crazy about Kaito Kid , he think 3(because 3 is the sum of 1 and 2), 4, 5, 6 are his lucky numbers and all others are not. 

Now he finds out a way that he can represent a number through decimal representation in another numeral system to get a number only contain 3, 4, 5, 6. 

For example, given a number 19, you can represent it as 34 with base 5, so we can call 5 is a lucky base for number 19. 

Now he will give you a long number n(1<=n<=1e12), please help him to find out how many lucky bases for that number. 

If there are infinite such base, just print out -1.

# Input

There are multiply test cases. 

The first line contains an integer T(T<=200), indicates the number of cases. 

For every test case, there is a number n indicates the number.

# Output

For each test case, output “Case #k: ”first, k is the case number, from 1 to T , then, output a line with one integer, the answer to the query.

# **Sample Input**

**21019**

# Sample Output

**Case #1: 0Case #2: 1**


题意：t 组数据，每组给出一个十进制数 n，现可将其转为任意进制，但要求转换后的数只由 3、4、5、6 构成的，问满足条件的进制有几种，若有无数个，则输出 -1

思路： 对于任何一种进制 x，均满足 n=A0+A1*x+A2*x^2+A3*x^3+...，因此可对 n 转换后的进制位数进行分情况处理
- 对于一位的情况，很明显有无穷个，即 -1
- 对于两位的情况，有 n=A0+A1*x 的形式，枚举 A0、A1，判断是否有整数解 x，此外，还需判断 x 是否大于 A0、A1 的最大值
- 对于三位的情况，有 n=A0+A1*x+A2*x^2，枚举 A0、A1、A2，判断是否有整数解 x，此外，还需判断 x 是否大于 A0、A1、A2 的最大值
- 对于四位的情况，由于 n 最大为 1E12，此时最多也只有 7000 个数，而 7000^3 接近 1E12，因此直接枚举进制数来判断即可，不必考虑更多的可能

因此，可以枚举进制然后判断是否满足题设

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
LL max(LL a,LL b,LL c){
    return max(a,max(b,c));
}
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL n;
        scanf("%lld",&n);

        LL res=0;
        if(n>=3&&n<=6){//一位的情况
            res=-1;
        }
        else{
            for(LL i=3;i<=6;i++){//两位的情况
                for(LL j=3;j<=6;j++){
                    LL k=(n-j)/i;
                    if(k*i==n-j&&k>max(i,j))
                        res++;
                }
            }

            for(LL i=3;i<=6;i++){//位的情况
                for(LL j=3;j<=6;j++){
                    for(LL k=3;k<=6;k++){
                        LL a=i,b=j,c=k-n;
                        LL delta=b*b-4*a*c;
                        if(delta>0){
                            LL d=sqrt(delta);
                            if(d*d==delta){
                                LL x=(-b+d)/(2*a);
                                if(x>max(i,j,k)&&x*2*a==(-b+d))
                                    res++;
                            }
                        }
                    }
                }
            }

            for(LL i=2;i<=7000;i++){//四位的情况
                LL temp=n,bit;
                LL num=0;
                bool flag=true;
                while(temp){
                    bit=temp%i;
                    temp=temp/i;
                    num++;

                    if(bit<3||bit>6){
                        flag=false;
                        break;
                    }
                }
                if(flag&&num>3)
                    res++;
            }
        }

        printf("Case #%d: %lld\n",Case++,res);
    }

    return 0;
}
```






