# HDU3480 Division DP四边形不等式优化 - 紫芝的博客 - CSDN博客





2018年08月19日 21:08:26[紫芝](https://me.csdn.net/qq_40507857)阅读数：59








# Division

****Time Limit: 10000/5000 MS (Java/Others)    Memory Limit: 999999/400000 K (Java/Others)Total Submission(s): 5868    Accepted Submission(s): 2332****

**Problem Description**

Little D is really interested in the theorem of sets recently. There’s a problem that confused him a long time.  

Let T be a set of integers. Let the MIN be the minimum integer in T and MAX be the maximum, then the cost of set T if defined as (MAX – MIN)^2. Now given an integer set S, we want to find out M subsets S1, S2, …, SM of S, such that and the total cost of each subset is minimal.

![](http://acm.hdu.edu.cn/data/images/C295-1003-1.jpg)

**Input**

The input contains multiple test cases.

In the first line of the input there’s an integer T which is the number of test cases. Then the description of T test cases will be given. 

For any test case, the first line contains two integers N (≤ 10,000) and M (≤ 5,000). N is the number of elements in S (may be duplicated). M is the number of subsets that we want to get. In the next line, there will be N integers giving set S.

**Output**

For each test case, output one line containing exactly one integer, the minimal total cost. Take a look at the sample output for format.

**Sample Input**

```
2 
3 2 
1 2 4 
4 2 
4 7 10 1
```

**Sample Output**

```
Case 1: 1 

Case 2: 18
```

***Hint***

The answer will fit into a 32-bit signed integer.

**Source**

[2010 ACM-ICPC Multi-University Training Contest（5）——Host by BJTU](http://acm.hdu.edu.cn/search.php?field=problem&key=2010+ACM-ICPC+Multi-University+Training+Contest%A3%A85%A3%A9%A1%AA%A1%AAHost+by+BJTU&source=1&searchmode=source)

**Recommend**

zhengfeng   |   We have carefully selected several similar problems for you:  [3478](http://acm.hdu.edu.cn/showproblem.php?pid=3478)[3485](http://acm.hdu.edu.cn/showproblem.php?pid=3485)[3487](http://acm.hdu.edu.cn/showproblem.php?pid=3487)[3486](http://acm.hdu.edu.cn/showproblem.php?pid=3486)[3484](http://acm.hdu.edu.cn/showproblem.php?pid=3484)

**题意：**
将n个数，分成m个子集，使得每个子集的最大值与最小值的平方差之和最小

**分析：**

[DP四边形不等式优化](https://blog.csdn.net/qq_40507857/article/details/81635667)

dp[i][j]=dp[i-1][k-1]+w[k][j]，由于w[i+1][j]-w[i][j]=(a[i]-a[i+1])*(2*a[j]-a[i+1]-a[i])，这个表达式是随着j的增加单调递减的，所以就有w[i+1][j+1]-w[i][j+1]<=w[i+1][j]-w[i][j]，也就是w[i][j]+w[i+1][j+1]<=w[i][j+1]+w[i+1][j]，满足四边形不等式优化的条件，我们就可以放心地使用四边形不等式优化dp了。

对于dp[i][j]的决策s[i][j]，有s[i][j-1]<=s[i][j]<=s[i+1][j]，在dp[i][j]求出来之前要先求出dp[i][j-1]、dp[i+1][j]，可以按照长度递增来递推，具体见代码，还有注意初始化。

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e4+10;
#define inf 0x3f3f3f3f
int dp[maxn>>1][maxn];//dp[i][j]表示在【i，j】区间上的最优值
int s[maxn>>1][maxn];//s[i][j]表示dp[i][j]取最优值时的下标
int a[maxn];//n个数字
int n,m;//n个数，分成m个集合
int ans;//答案
void solve()
{
    memset(dp,inf,sizeof(dp));
    for(int i=0;i<=m;i++)
    {
        dp[i][i]=0;
        s[i][i]=i;
    }
    for(int i=m+1;i<=n;i++)
        s[m+1][i]=i;
    for(int len=1;len<n;len++)//枚举区间长度
    {
        for(int i=1;i<=m;i++)//枚举区间起点
        {
            int j=i+len;//枚举区间终点
            if(j>n) break;//越界
            for(int k=s[i][j-1];k<=s[i+1][j];k++)//枚举区间断点
            {
                int temp=dp[i-1][k-1]+(a[k]-a[j])*(a[k]-a[j]);
                if(dp[i][j]>temp)//更新操作
                    dp[i][j]=temp,s[i][j]=k;
            }
        }
    }
    ans=dp[m][n];//n个数，分成m个集合的最优值
}
int main()
{
    int T,Case=1;
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);
        sort(a+1,a+n+1);
        solve();
        printf("Case %d: %d\n",Case++,ans);
    }
    return 0;
}
```





