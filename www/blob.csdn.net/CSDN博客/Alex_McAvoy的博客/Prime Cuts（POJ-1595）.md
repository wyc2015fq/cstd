# Prime Cuts（POJ-1595） - Alex_McAvoy的博客 - CSDN博客





2018年08月11日 12:39:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：70
个人分类：[POJ																[数论——素性测试](https://blog.csdn.net/u011815404/article/category/8796920)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

A prime number is a counting number (1, 2, 3, ...) that is evenly divisible only by 1 and itself. In this problem you are to write a program that will cut some number of prime numbers from the list of prime numbers between (and including) 1 and N. Your program will read in a number N; determine the list of prime numbers between 1 and N; and print the C*2 prime numbers from the center of the list if there are an even number of prime numbers or (C*2)-1 prime numbers from the center of the list if there are an odd number of prime numbers in the list.

# **Input**

Each input set will be on a line by itself and will consist of 2 numbers. The first number (1 <= N <= 1000) is the maximum number in the complete list of prime numbers between 1 and N. The second number (1 <= C <= N) defines the C*2 prime numbers to be printed from the center of the list if the length of the list is even; or the (C*2)-1 numbers to be printed from the center of the list if the length of the list is odd.

# Output

For each input set, you should print the number N beginning in column 1 followed by a space, then by the number C, then by a colon (:), and then by the center numbers from the list of prime numbers as defined above. If the size of the center list exceeds the limits of the list of prime numbers between 1 and N, the list of prime numbers between 1 and N (inclusive) should be printed. Each number from the center of the list should be preceded by exactly one blank. Each line of output should be followed by a blank line. Hence, your output should follow the exact format shown in the sample output.

# Sample Input

**21 218 218 18100 7**

# Sample Output

**21 2: 5 7 11**


**18 2: 3 5 7 11**


**18 18: 1 2 3 5 7 11 13 17**


**100 7: 13 17 19 23 29 31 37 41 43 47 53 59 61 67**


题意：给出两个数 n、c，求从 1~N 的素数序列，若素数个数为奇数，则输出序列中心的 2*c-1 个素数，若为偶数，则输出序列中心的 2*c 个素数。若 c 大于素数个数，则输出整个序列。

思路：

打表求素数，再计算序列中心位置，然后判断奇偶输出即可。

坑点很多，最坑的是这个题，1 也算做是素数。。。

输出格式也很坑。。。

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
#define N 1100
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int prime[N],cnt=1;
bool bprime[N];
void make_prime()
{
	memset(bprime,true,sizeof(bprime));
	bprime[0]=false;

    for(int i=2;i<=N;i++)
        for(int j=i+i;j<=N;j+=i)
            bprime[j]=false;

    for(int i=0;i<=N;i++)
        if(bprime[i])
            prime[cnt++]=i;
}
int main()
{
    make_prime();

    int n,c;
    while(scanf("%d%d",&n,&c)!=EOF&&(n+c))
    {
        int pos=0;
        for(int i=1;i<cnt;i++)//求序列中心
        {
            if(prime[i]<=n&&prime[i+1]>n)
            {
                pos=i;
                break;
            }
        }

        printf("%d %d: ",n,c);

        if(pos<c&&pos%2==0)
            c=pos/2;
        else if(pos<c)
            c=(pos+1)/2;


        if(pos%2)
        {
            for(int i=(pos-(c*2-1))/2+1;i<=(pos-(c*2-1))/2+c*2-1;i++)
                printf("%d ",prime[i]);
            printf("\n\n");
        }
        else
        {
            for(int i=(pos-c*2)/2+1;i<=(pos-c*2)/2+c*2;i++)
                printf("%d ",prime[i]);
            printf("\n\n");
        }
    }

    return 0;
}
```






