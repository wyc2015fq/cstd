# 数位dp模板（C++） - westbrook1998的博客 - CSDN博客





2018年11月14日 23:23:12[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：122








记忆化搜索

bzoj1026 找出区间内满足相邻两位数差大于等于2的数的个数

数位dp的过程就是先分解记录数位，然后从最高位开始dfs，注意前导零和limit的限制，然后一般带上的还有pos当前位置和pre上一个数，然后根据具体题目再带上其他状态，dp[i][j]在这里表示高一位为j的i位数满足条件的个数，第二维也要根据具体题目设计状态
```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll l,r;
int a[20];
//dp[i][j]表示高一位为j的i位数满足条件的个数
ll dp[20][20];
//pos:当前枚举数位 pre:上一位数 status:是否有前导零（0为有） limit:是否有上限限制
int dfs(int pos,int pre,int status,bool limit){
    if(pos<0){
        return 1;
    }
    if(status && !limit && dp[pos][pre]!=-1){
        return dp[pos][pre];
    }
    //是否有上限限制
    int up=limit?a[pos]:9;
    ll ans=0;
    if(status==0){
        for(int i=0;i<=up;i++){
            ans+=dfs(pos-1,i,i,limit&&(i==up));
        }
    }else{
        for(int i=0;i<=up;i++){
            if(abs(i-pre)<2){
                continue;
            }
            ans+=dfs(pos-1,i,1,limit&&(i==up));
        }
    }
    if(status && !limit){
        dp[pos][pre]=ans;
    }
    return ans;
}
int solve(ll x){
    int len=0;
    while(x){
        a[len++]=x%10;
        x/=10;
    }
    memset(dp,-1,sizeof(dp));
    return dfs(len-1,0,0,true);
}
int main(void){
    scanf("%lld%lld",&l,&r);
    int ans=solve(r)-solve(l-1);
    printf("%d\n",ans);
    return 0;
}
```




