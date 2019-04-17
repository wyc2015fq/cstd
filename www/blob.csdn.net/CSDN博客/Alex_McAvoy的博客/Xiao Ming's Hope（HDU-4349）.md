# Xiao Ming's Hope（HDU-4349） - Alex_McAvoy的博客 - CSDN博客





2018年08月12日 21:35:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：75








> 
# Problem Description

Xiao Ming likes counting numbers very much, especially he is fond of counting odd numbers. Maybe he thinks it is the best way to show he is alone without a girl friend. The day 2011.11.11 comes. Seeing classmates walking with their girl friends, he coundn't help running into his classroom, and then opened his maths book preparing to count odd numbers. He looked at his book, then he found a question "C(n,0)+C(n,1)+C(n,2)+...+C(n,n)=?". Of course, Xiao Ming knew the answer, but he didn't care about that , What he wanted to know was that how many odd numbers there were? Then he began to count odd numbers. When n is equal to 1, C(1,0)=C(1,1)=1, there are 2 odd numbers. When n is equal to 2, C(2,0)=C(2,2)=1, there are 2 odd numbers...... Suddenly, he found a girl was watching him counting odd numbers. In order to show his gifts on maths, he wrote several big numbers what n would be equal to, but he found it was impossible to finished his tasks, then he sent a piece of information to you, and wanted you a excellent programmer to help him, he really didn't want to let her down. Can you help him?

# **Input**

Each line contains a integer n(1<=n<=10^8)

# Output

A single line with the number of odd numbers of C(n,0),C(n,1),C(n,2)...C(n,n).

# Sample Input

**1211**

# Sample Output

**228**


题意：求 C(n,0),C(n,1),C(n,2)...C(n,n) 中的奇数个数

思路：实质为求 C(n,m)%2，也就是将 n 转化为二进制后 1 的个数，设个数为 t，则结果为 2 的 t 次方

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int Quick_Pow(int a,int b)
{
    int res=1;
    while(b)
    {
        if(b&1)
            res*=a;
        a*=a;
        b/=2;
    }
    return res;
}
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        int sum=0;
        while(n)
        {
            if(n%2)
                sum++;
            n/=2;
        }
        int res=Quick_Pow(2,sum);
        printf("%d\n",res);
    }
    return 0;
}
```





