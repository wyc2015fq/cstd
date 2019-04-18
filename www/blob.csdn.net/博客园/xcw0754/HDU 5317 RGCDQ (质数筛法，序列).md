# HDU 5317 RGCDQ (质数筛法，序列) - xcw0754 - 博客园
# [HDU 5317 RGCDQ (质数筛法，序列)](https://www.cnblogs.com/xcw0754/p/4684135.html)
题意：从1~1000,000的每个自然数质因子分解，不同因子的个数作为其f 值，比如12=2*2*3，则f(12)=2。将100万个数转成他们的f值后变成新的序列seq。接下来T个例子，每个例子一个询问区间seq[L,R]。问该子序列中任意两个不同下标的数，他们的GCD值最大为多少？
思路：
（1）质因子分解，用筛法先将质数先全部出来。
（2）对每个自然数求其f值，即将其质因子分解后的不同因子个数，作为seq序列。
（3）扫一遍seq，用另一个数组记录下标i之前有多少个1、2、3、4...。因为seq序列中最大的数不超过10，所以可以统计，以方便能在O(1)内得到一个区间内有多少个值为x的元素。
（4）对于每个询问L和R，将所有元素装入新的序列中，准备求GCD。注意：相同数字不必超过2个，即新序列中的元素顶多出现2次。
```
#include <bits/stdc++.h>
using namespace std;
const int N=1000005;
vector<int> prime;
bool seq[N];
int cnt[N];
int fval[N][10];
void pre_cal()
{
    memset(seq,1,sizeof(seq));
    memset(cnt,0,sizeof(cnt));
    memset(fval,0,sizeof(fval));
    for(int i=2; i*i<N; i++)    //所有质数都是false;
    {
        if(!seq[i]) continue;
        for(int j=i*i; j<N; j+=i)    seq[j]=0;
    }
    for(int i=2; i<N; i++)    if(seq[i])    prime.push_back(i);
    for(int i=2; i<N; i++)  //求f值
    {
        int t=i;
        for(int j=0; j<prime.size()&& prime[j]<i && t!=1; j++)
        {
            if( seq[t] )    //若是质数，不用再求了
            {
                cnt[i]++;
                break;
            }
            if( t%prime[j]==0  )    cnt[i]++;          //是因子
            while(  t%prime[j]==0 ) t/=prime[j];    //去掉该因子
        }
    }
    for(int i=1; i<N; i++)
    {
        for(int j=1; j<10; j++)    fval[i][j]=fval[i-1][j];
        fval[i][cnt[i]]++;
    }
}
vector<int> val;
int cal(int L,int R)//处理询问
{
    val.clear();
    for(int i=9; i>1; i--)
    {
        if(  fval[R][i]-fval[L-1][i]>1 )
        {
            val.push_back(i);
            val.push_back(i);
        }
        else if( fval[R][i]-fval[L-1][i]==1 )
            val.push_back(i);
    }
    int ans=1;
    for(int i=0; i<val.size(); i++)
    {
        for(int j=i+1; j<val.size(); j++)
            ans=max(ans,__gcd(val[i],val[j]));
    }
    return ans;
}
int main()
{
    freopen("input.txt", "r", stdin);
    pre_cal();
    int t, L, R;
    cin>>t;
    while(t--)
    {
        scanf("%d%d",&L,&R);
        printf("%d\n",cal(L,R));
    }
    return 0;
}
AC代码
```

