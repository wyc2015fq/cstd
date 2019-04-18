# POJ 3590 The shuffle Problem [置换群 DP] - weixin_33985507的博客 - CSDN博客
2017年02月28日 19:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
[传送门](http://poj.org/problem?id=3590)
$1A$太爽了
从此$Candy?$完全理解了这种$DP$做法
和bzoj1025类似，不过是求最大的公倍数，并输出一个字典序最小的方案
依旧枚举质因子和次数，不足的划分成1
输出方案从循环长度小的到大的输出
```
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<cmath>
using namespace std;
const int N=105;
typedef long long ll;
inline int read(){
    char c=getchar();int x=0,f=1;
    while(c<'0'||c>'9'){if(c=='-')f=-1; c=getchar();}
    while(c>='0'&&c<='9'){x=x*10+c-'0'; c=getchar();}
    return x*f;
}
int n,m;
int p[N];
bool notp[N];
void sieve(int n){
    for(int i=2;i<=n;i++){
        if(!notp[i]) p[++m]=i;
        for(int j=1;j<=m&&i*p[j]<=n;j++){
            notp[i*p[j]]=1;
            if(i%p[j]==0) break;
        }
    }
}
int f[N][N],g[N][N];
int a[N],tot;
void getPath(int i,int j){
    if(i==0) return;
    if(g[i][j]!=0) a[++tot]=g[i][j];
    getPath(i-1,j-g[i][j]);
}
void dp(){
    f[0][0]=1;
    for(int i=1;i<=m;i++)
        for(int j=0;j<=n;j++){
            f[i][j]=f[i-1][j];
            g[i][j]=0;
            for(int k=p[i];k<=j;k*=p[i])
                if(f[i-1][j-k]*k>f[i][j]){
                    f[i][j]=f[i-1][j-k]*k;
                    g[i][j]=k;
                }
        }
    int ans=1,now=1;
    for(int i=0;i<=n;i++) if(f[m][i]>f[m][ans]) ans=i;
    printf("%d ",f[m][ans]);
    for(;now<=n-ans;now++) printf("%d ",now);
    //puts("totot");
    tot=0;
    getPath(m,ans);
    sort(a+1,a+1+tot);
    //for(int i=1;i<=tot;i++) printf("a %d %d\n",i,a[i]);
    for(int i=1;i<=tot;i++){
        int _=now;
        for(int j=1;j<a[i];j++) printf("%d ",++now);
        printf("%d ",_);
        now++;
    }
    puts("");
}
int main(){
    freopen("in","r",stdin);
    sieve(100);
    int T=read();
    while(T--){
        n=read();
        dp();
    }
}
```
