# CSU  1120  病毒（LCIS） - HJ - CSDN博客
2017年08月24日 08:56:08[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：312
[http://acm.csu.edu.cn/csuoj/problemset/problem?pid=1120](http://acm.csu.edu.cn/csuoj/problemset/problem?pid=1120)
求最长公共上升子序列。
```cpp
#include<iostream>
#include<cstdio> 
#include<algorithm>   
using namespace std;  
typedef long long LL;  
typedef pair<int, int> PII;  
#define maxn 1005  
#define INF 0x3f3f3f3f 
int dp[maxn][maxn];  
int a[maxn],b[maxn];  
int main() {  
    int t;    
    scanf("%d", &t);  
    while(t--) 
    {  
        int m, n;  
        scanf("%d", &m);  
        memset(dp, 0, sizeof(dp)); 
        for(int i=1;i<=m;i++) 
          scanf("%d",&a[i]);  
        scanf("%d", &n);  
        for(int i=1;i<=n;i++)
          scanf("%d",&b[i]);  
        int ans=0;  
        for(int i=1;i<=m;i++)
        {  
            int k=0;  
            for(int j=1;j<=n;j++)
            {  
                if(a[i]==b[j])  
                  dp[i][j]=k+1;  
                else  
                  dp[i][j]=dp[i - 1][j];   
                if(a[i]>b[j])
                  k=max(k, dp[i][j]);  
                ans=max(ans, dp[i][j]);  
            }  
        }  
        printf("%d\n", ans);  
    }  
    return 0;  
}
```
