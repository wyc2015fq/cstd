# ZCMU-1540: 第k大数：二分+思路 - 安得广厦千万间的博客 - CSDN博客





2018年06月07日 15:55:39[独-](https://me.csdn.net/qq_41713256)阅读数：107








链接：[点击打开链接](http://acm.zcmu.edu.cn/JudgeOnline/problem.php?id=1540)

## 1540: 第k大数
Time Limit: 10 Sec  Memory Limit: 128 MB
Submit: 657  Solved: 95
[[Submit](http://acm.zcmu.edu.cn/JudgeOnline/submitpage.php?id=1540)][[Status](http://acm.zcmu.edu.cn/JudgeOnline/problemstatus.php?id=1540)][[Web Board](http://acm.zcmu.edu.cn/JudgeOnline/bbs.php?pid=1540)]
## Description


有两个序列a，b，它们的长度分别为n和m，那么将两个序列中的元素对应相乘后得到的n*m个元素从大到小排列后的第k个元素是什么？


## Input


输入的第一行为一个正整数T (T<=10)，代表一共有T组测试数据。

每组测试数据的第一行有三个正整数n，m和k(1<=n, m<=100000,1<=k<=n*m)，分别代表a序列的长度，b序列的长度，以及所求元素的下标。第二行为n个正整数代表序列a。第三行为m个正整数代表序列b。序列中所有元素的大小满足[1,100000]。


## Output


对于每组测试数据，输出一行包含一个整数代表第k大的元素是多少。


## Sample Input

3

3 2 3

1 2 3

1 2

2 2 1

1 1

1 1

2 2 4

1 1

1 1

## Sample Output

3

1

1

要求找到第k大的数，也就是从大到小排的第k个数，转化一下，就是找到最大的val使得大于等于val的个数大于等于k。这样子基本是就是用二分了，直接二分答案，先排序，确定二分的边界。【 a【0】*b【0】，a【n-1】*b【m-1】 】。

然后就是找大于等于val的有多少个了，这里有一种操作，对a数组我们从后往前找，b数组从前往后找。

a数组：n-1   n-2    n-3 ....  i+1  i   i-1  ...  0

b数组   0  1  2  3 .... j-1   j   j+1 ....   m-1

当a【i】*b【j】>=val （当位于a【i】时 j为第一个使得a【i】*b【j】>=val的数，我们就break，因为j后面的数肯定都比val打）的时候那么个数等于m-j （注意a【i】*b【j-1】<val）

下一轮 a【i-1】* b【j-1】肯定也是小于val的那么我们只要从j这个位置开始查找。

复杂度为 O（m+n）。

这里有人会想，二分出来的答案会不会是 不能用某个a和某个b得到。其实是不会的。

之前说了，找到最大的val使得大于等于val的个数大于等于k。那么大于val+1这个值的数肯定是小于k的。那么val这个值至少存在一个才能使得大于等于k。   

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int inf=1e9;
const int maxn=1e5+5;
int n,m;
LL a[maxn],b[maxn];
LL judge(LL val)
{
    LL ans=0;
    int j=0;
    for(int i=n-1;i>=0;i--)
    {
        for(;j<m;j++)
            if(a[i]*b[j]>=val) break;
        if(j==m) break;
        ans+=(m-j);
    }
    return ans;
}
int main()
{
    int T;
    LL k;
    cin>>T;
    while ( T-- )
    {
        cin>>n>>m>>k;
        for(int i=0;i<n;i++) cin>>a[i];
        for(int i=0;i<m;i++) cin>>b[i];
        sort(a,a+n);
        sort(b,b+m);
        LL l=a[0]*b[0],r=a[n-1]*b[m-1];
        LL mid,ans;
        while(l<=r)
        {
            mid=(l+r)>>1;
           // cout<<mid<<' '<<judge(mid)<<endl;
            if(judge(mid)>=k)
            {
                ans=mid;
                l=mid+1;
            }
            else r=mid-1;
        }
        cout<<ans<<endl;
    }
    return 0;
}
```







