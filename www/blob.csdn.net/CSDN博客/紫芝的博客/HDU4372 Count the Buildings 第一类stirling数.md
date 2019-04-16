# HDU4372 Count the Buildings 第一类stirling数 - 紫芝的博客 - CSDN博客





2018年09月13日 09:49:13[紫芝](https://me.csdn.net/qq_40507857)阅读数：24
个人分类：[数论																[stirling数](https://blog.csdn.net/qq_40507857/article/category/8053760)](https://blog.csdn.net/qq_40507857/article/category/7647322)








# Count the Buildings

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/65536 K (Java/Others)Total Submission(s): 2935    Accepted Submission(s): 960****

Problem Description

There are N buildings standing in a straight line in the City, numbered from 1 to N. The heights of all the buildings are distinct and between 1 and N. You can see F buildings when you standing in front of the first building and looking forward, and B buildings when you are behind the last building and looking backward. A building can be seen if the building is higher than any building between you and it.

Now, given N, F, B, your task is to figure out how many ways all the buildings can be.

Input

First line of the input is a single integer T (T<=100000), indicating there are T test cases followed.

Next T lines, each line consists of three integer N, F, B, (0<N, F, B<=2000) described above.

Output

For each case, you should output the number of ways mod 1000000007(1e9+7).

Sample Input

2

3 2 2

3 2 1

Sample Output

2 1

Source

[2012 Multi-University Training Contest 8](http://acm.hdu.edu.cn/search.php?field=problem&key=2012+Multi-University+Training+Contest+8&source=1&searchmode=source)

### 题意：

**N座高楼，高度均不同且为1~N中的数，从前向后看能看到F个，从后向前看能看到B个，问有多少种可能的排列数。**

**0 < N, F, B <= 2000**

### 分析：

**首先我们知道一个结论：n的环排列的个数与n-1个元素的排列的个数相等，因为P(n,n)/n=(n-1)!。**

可以肯定，无论从最左边还是从最右边看，最高的那个楼一定是可以看到的.

**假设最高的楼的位置固定，最高楼的编号为n，那么我们为了满足条件，可以在楼n的左边分x-1组，右边分y-1组，且用每组最高的那个元素代表这一组，那么楼n的左边，从左到右，****组与组之间最高的元素一定是单调递增的****，且每组中的最高元素一定排在该组的最左边，每组中的其它元素可以任意排列（相当于这个组中所有元素的环排列）。右边反之亦然。**

然后，可以这样考虑这个问题，最高的那个楼左边一定有x-1个组，右边一定有y-1个组，且每组是一个环排列，这就引出了第一类Stirling数（n个人分成k组，每组内再按特定顺序围圈的分组方法的数目）。

我们可以先把n-1个元素分成x-1+y-1组，然后每组内部做环排列。再在所有组中选取x-1组放到楼n的左边。所以答案是

### **ans(n, f, b) = C[f + b - 2][f - 1] * S[n - 1][f + b - 2];**

**计算组合数的方法1：杨辉三角形**

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int mod = 1e9+7;
const int maxn=2007;
LL s1[maxn][maxn];
LL c[maxn][maxn];
void init()
{
    s1[1][1]=1;
    for(int i=2;i<maxn;i++)
        for(int j=1;j<=i;j++)
        s1[i][j]=(s1[i-1][j-1]+ ((i-1)*s1[i-1][j])%mod )%mod;

    memset(c,0,sizeof c);
    c[0][0]=1;
    for(int i=1;i<maxn;i++)
        for(int j=0;j<=i;j++)
        c[i][j]=(c[i-1][j-1]+c[i-1][j])%mod;
}
int main()
{
    init();
    /*
    for(int i=0;i<50;i++)
    {
        for(int j=0;j<=i;j++)
            printf("%lld ",c[i][j]);
        printf("\n");
    }
    */
    int T;
    scanf("%d",&T);
    while(T--){
        int n,f,b;
        scanf("%d%d%d",&n,&f,&b);
        if(f+b-1>n){
            printf("0\n");
            continue;
        }
        LL ans=(c[f+b-2][f-1]*s1[n-1][f+b-2])%mod;
        printf("%I64d\n",ans);
    }
}
```

**计算组合数的方法2：阶乘的逆元**

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int maxn=2007,mod=1e9+7;
LL s1[maxn][maxn];//第一类斯特林数
LL fact[maxn];//阶乘
LL inv[maxn];//逆元
LL ifact[maxn];//阶乘的逆元
void init()
{
    s1[1][1]=1;
    for(int i=2;i<maxn;i++)
        for(int j=1;j<=i;j++)
        s1[i][j]=(s1[i-1][j-1]+(i-1)*s1[i-1][j]%mod)%mod;

    fact[0]=1;
    for(int i=1;i<maxn;i++)
        fact[i]=i*fact[i-1]%mod;

    inv[1]=1;
    for(int i=2;i<maxn;i++)
        inv[i]=(mod-mod/i)*inv[mod%i]%mod;

    ifact[0]=1;
    for(int i=1;i<maxn;i++)
        ifact[i]=ifact[i-1]*inv[i]%mod;
}
LL c(LL m,LL n)
{
    if(n<0) return 0;
    return fact[m]*ifact[n]%mod*ifact[m-n]%mod;
}
int main()
{
    init();
    int T;
    scanf("%d",&T);
    while(T--){
        int n,f,b;
        scanf("%d%d%d",&n,&f,&b);
        if(f+b-1>n)
        {
            printf("0\n");
            continue;
        }
        LL ans=s1[n-1][f+b-2]*c(f+b-2,f-1)%mod;
        printf("%lld\n",ans);
    }
}
```






