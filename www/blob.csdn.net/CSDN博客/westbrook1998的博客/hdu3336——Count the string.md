# hdu3336——Count the string - westbrook1998的博客 - CSDN博客





2019年02月02日 00:08:54[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：17








理解了好久才勉强理解。。。

题目求的是给定字符串s的所有前缀在字符串里出现的次数总和（特么匹配次数不能算重复的。。。然后样例也不说清楚。。。全世界好像就我在这里被卡住。。。）

定义一个dp数组，`dp[i]`表示前i个字符里前缀出现的总次数，所以题目求的就是dp[n]

然后递推关系就是`dp[i]=dp[i-1]+1+(nex[i]==0?0:1)`

这个递推关系前部分很好理解，就是前i-1个字符的状态再加1，就是加上i这个字符形成的一个新的前缀，这暂时肯定只出现1次，所以加1

后部分根据nex数组来判断，因为nex数组表示的就是前i个字符的最大前缀后缀相同的长度，所以如果nex[i]不等于0，说明这前i个字符加上这个第i个字符就有了一个后缀等于前面的前缀，那么也就是说前面的那个前缀又多出现了一次，那就得加1，否则加0，为什么只加1个呢，难道不会出现aaa…aaa这种情况，很多个前缀都出现多了一次，这是因为前面的情况已经在前面的递推中累加了，所以只考虑第i位即可
```
#include <bits/stdc++.h>
using namespace std;
const int N=200050;
const int MOD=10007;
int nex[N],dp[N];
int t,n;
char s[N];
void getNext(char p[]){
    int n=strlen(p);
    int i=0;
    int j=-1;
    nex[0]=-1;
    while(i<n){
        if(j==-1 || p[i]==p[j]){
            nex[++i]=++j;
        }else{
            j=nex[j];
        }
    }
}
int main(void){
    scanf("%d",&t);
    while(t--){
        int cnt=0;
        memset(dp,0,sizeof(dp));
        scanf("%d",&n);
        scanf("%s",s+1);
        getNext(s+1);
        //坑点：字符串匹配个数不能算重叠
        //dp[i]表示前i个字符里前缀出现的总次数
        for(int i=1;i<=n;i++){
            dp[i]=(dp[i-1]+1)%MOD;
            if(nex[i]){
                dp[i]++;
            }
        }
        printf("%d\n",dp[n]%MOD);
    }
    return 0;
}
```




