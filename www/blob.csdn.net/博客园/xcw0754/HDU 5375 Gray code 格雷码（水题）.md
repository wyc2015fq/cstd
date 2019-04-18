# HDU 5375 Gray code 格雷码（水题） - xcw0754 - 博客园
# [HDU 5375 Gray code 格雷码（水题）](https://www.cnblogs.com/xcw0754/p/4722637.html)
题意：给一个二进制数（包含3种符号:'0'  '1'  '?'  ，问号可随意 ），要求将其转成格雷码，给一个序列a，若转成的格雷码第i位为1，则得分+a[i]。求填充问号使得得分最多。
思路：如果了解格雷码的转换，相信能很快看出一些端倪。偷别人的图：
![](https://images0.cnblogs.com/blog2015/641737/201508/112307015985856.png)
　　分析一下：所给的二进制数要转成格雷码，只与所给二进制有关。即不需要利用已经某些转换好的格雷码字。
　　接下来分析5个位的串 ：
　　（1）00?00　　仅有1个问号，只会与后面那些连续且非问号的串转成格雷码有关
　　（2）00??0　　有连续的1个问号，这才需要用到dp啊，因为所有问号有很多组合可能，但是他们满足：第i位只与前1位有关。所以仅需记录此位为0的结果，和为1的结果。
　　（3）0?0?0　　//问号不连续，按第1种处理。
　　没有问号的串没有什么DP可言，直接转。
```
#include <bits/stdc++.h>
#define LL long long
using namespace std;
const int N=200010;
char str[N];
int a[N];
int dp[N][2];   //结果只有两种，第i位为0/1
int get_sum(int pos, int cur)//选个大的前缀，但并不需要关心其究竟是多少
{
    int ans1=dp[pos-1][0]+ (0^cur)*a[pos];  //选0试试
    int ans2=dp[pos-1][1]+ (1^cur)*a[pos];  //选1试试
    return max(ans1, ans2);
}
int cal(int n)
{
    memset(dp, 0, sizeof(dp));
    if(str[0]=='1' || str[0]=='?')        dp[0][1]= a[0];
    for(int i=1; i<n; i++)
    {
        int t=str[i]-'0';         //当前
        int p=str[i-1]-'0';     //前一位
        if( str[i]=='?' && str[i-1]=='?' )
        {
            dp[i][0]=get_sum(i, 0) ;
            dp[i][1]=get_sum(i, 1) ;
        }
        if( str[i]=='?' && str[i-1]!='?' )
        {
            dp[i][0]= dp[i-1][p] +( p^0) *a[i];  //前面是固定的，没啥好选。
            dp[i][1]= dp[i-1][p] +( p^1) *a[i];
        }
        if( str[i]=='0' || str[i]=='1')
        {
            if(str[i-1]=='?')    //前面是问号，取大者即可。
                dp[i][t]=max( dp[i-1][0]+ (0^t)*a[i], dp[i-1][1]+ (1^t)*a[i] );
            else
                dp[i][t]= dp[i-1][p]+ (p^t)*a[i];
        }
    }
    return max(dp[n-1][0], dp[n-1][1]);
}
int main()
{
    freopen("input.txt", "r", stdin);
    int t, tmp, j=0;
    char c;
    cin>>t;
    while(t--)
    {
        scanf("%s", str);
        int len=strlen(str);
        for(int i=0; i<len; i++)    scanf("%d", &a[i]);
        printf("Case #%d: %d\n", ++j,  cal(len) );
    }
    return 0;
}
AC代码
```

