# Codeforces1073C——Vasya and Robot - westbrook1998的博客 - CSDN博客





2018年10月27日 14:13:43[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：211








```
#include <bits/stdc++.h>
using namespace std;
const int N=1e6+5;
const int INF=0x3f3f3f3f;
int n,x,y;
char s[N];
int pre[N][2];
bool check(int m){
    for(int i=1;i+m-1<=n;i++){
        //xx yy是整个序列减去中间修改部分剩下的序列所对应的x和y
        int xx=pre[n][0]-pre[i+m-1][0]+pre[i-1][0];
        int yy=pre[n][1]-pre[i+m-1][1]+pre[i-1][1];
        //tx ty是要修改的区间修改后应该对应的x和y
        int tx=x-xx;
        int ty=y-yy;
        //如果区间长度m大于等于|tx|+|ty|而且m减去|tx|+|ty|剩下的是偶数的话，就可以把这些剩下的相互低效，然后其他就一直往一个方向走即达到目标
        if(abs(tx)+abs(ty)<=m &&(m-abs(tx)-abs(ty))%2==0){
            return true;
        }
    }
    return false;
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d",&n);
    scanf("%s",s+1);
    scanf("%d%d",&x,&y);
    //预处理前缀和 pre[i][0]代表x pre[i][1]代表y  比如RU 到R的位置x=1 y=0 到U的时候x=1 y=1
    pre[0][0]=0;
    pre[0][1]=0;
    for(int i=1;i<=n;i++){
        pre[i][0]=pre[i-1][0];
        pre[i][1]=pre[i-1][1];
        if(s[i]=='U'){
            pre[i][1]++;
        }
        else if(s[i]=='D'){
            pre[i][1]--;
        }
        else if(s[i]=='L'){
            pre[i][0]--;
        }
        else{
            pre[i][0]++;
        }
    }
    //二分答案(区间长度)
    int l=0,r=n;
    int m;
    int ans=INF;
    while(l<=r){
        m=(l+r)>>1;
        if(check(m)){
            r=m-1;
            //ans=min(ans,m);
            ans=m;
        }
        else{
            l=m+1;
        }
    }
    if(ans==INF){
        printf("-1\n");
        return 0;
    }
    printf("%d\n",ans);
    return 0;
}
```



