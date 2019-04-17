# Halloween treats（HDU-1808） - Alex_McAvoy的博客 - CSDN博客





2018年08月12日 21:25:12[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：84








> 
# Problem Description

Every year there is the same problem at Halloween: Each neighbour is only willing to give a certain total number of sweets on that day, no matter how many children call on him, so it may happen that a child will get nothing if it is too late. To avoid conflicts, the children have decided they will put all sweets together and then divide them evenly among themselves. From last year's experience of Halloween they know how many sweets they get from each neighbour. Since they care more about justice than about the number of sweets they get, they want to select a subset of the neighbours to visit, so that in sharing every child receives the same number of sweets. They will not be satisfied if they have any sweets left which cannot be divided.

Your job is to help the children and present a solution.

# **Input**

The input contains several test cases.

The first line of each test case contains two integers c and n (1 ≤ c ≤ n ≤ 100000), the number of children and the number of neighbours, respectively. The next line contains n space separated integers a1 , ... , an (1 ≤ ai ≤ 100000 ), where ai represents the number of sweets the children get if they visit neighbour i.

The last test case is followed by two zeros.

# Output

For each test case output one line with the indices of the neighbours the children should select (here, index i corresponds to neighbour i who gives a total number of ai sweets). If there is no solution where each child gets at least one sweet print "no sweets" instead. Note that if there are several solutions where each child gets at least one sweet, you may print any of them.

# Sample Input

**4 51 2 3 7 53 67 11 2 5 13 170 0**

# Sample Output

**3 52 3 4**


题意：给两个整数 c、n，再给出 n 个整数，要求从中找到若干个连续的数，使其和为 c 的倍数，输出这些数的序号

思路：抽屉原理经典应用，套模板即可，模板：[点击这里](https://blog.csdn.net/u011815404/article/details/81357109)

# Source Program

```cpp
#include<cstdio>
#include<cstring>
#define N 1000001
#define LL long long
LL a[N],drawer[N];
LL sum[N];
int main()
{
    LL c,n;
    while(scanf("%lld%lld",&c,&n)!=EOF&&c&&n)
    {
        memset(drawer,-1,sizeof(drawer));
        drawer[0]=0;
        sum[0]=0;
        
        for(LL i=1;i<=n;i++)
        {
            scanf("%lld",&a[i]);
            sum[i]=sum[i-1]+a[i];
        }
        
        for(LL i=1;i<=n;i++)
        {
            if(drawer[sum[i]%c]!=-1)
            {
                for(LL j=drawer[sum[i]%c]+1;j<i;j++)
                    printf("%lld ",j);
                printf("%lld\n",i);
                break;
            }
            drawer[sum[i]%c]=i;
        }
    }
    return 0;
}
```





