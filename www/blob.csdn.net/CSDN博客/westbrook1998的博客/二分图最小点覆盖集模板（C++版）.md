# 二分图最小点覆盖集模板（C++版） - westbrook1998的博客 - CSDN博客





2018年11月04日 10:26:59[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：155








如果只是求最小覆盖数，直接求最大匹配就好，如果还要求覆盖点集，就还要再抄抄模板

这里是一道经典的最小覆盖点集的题目，所以左右点集都是确定好的，和上份模板最大匹配不太一样

```
#include <bits/stdc++.h>
using namespace std;
const int N=100050;
const int M=100050;
int n,m;
char s[N];
struct Edge{
    int v,next;
}edge[M];
int cnt,head[N];
void init(){
    cnt=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int u,int v){
    edge[cnt]=Edge{v,head[u]};
    head[u]=cnt++;
}
//Left表示右边点集的左边匹配点
int Left[N],Right[N];
//S T分别表示左右点集是否已匹配
bool S[N],T[N];
//X Y表示左右两边的最小覆盖点集
vector<int> X,Y;
bool dfs(int u){
    S[u]=true;
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        if(!T[v]){
            T[v]=true;
            if(Left[v]==-1 || dfs(Left[v])){
                Left[v]=u;
                Right[u]=v;
                return true;
            }
        }
    }
    return false;
}
void solve(){
    memset(S,0,sizeof(S));
    memset(T,0,sizeof(T));
    //左边没有匹配的点再增广下去
    for(int i=1;i<=n;i++){
        if(Right[i]==-1){
            dfs(i);
        }
    }
    //此时最小点覆盖集即是左边没标记的点
    for(int u=1;u<=n;u++){
        if(!S[u]){
            X.push_back(u);
        }
    }
    //和右边已标记的点
    for(int v=1;v<=m;v++){
        if(T[v]){
            Y.push_back(v);
        }
    }
}
int main(void){
    scanf("%d%d",&n,&m);
    init();
    for(int i=1;i<=n;i++){
        scanf("%s",s+1);
        for(int j=1;j<=m;j++){
            if(s[j]=='*'){
                addEdge(i,j);
            }
        }
    }
    solve();
    //最小点覆盖集，补集即是最大独立集
    printf("%d",X.size());
    for(int i=0;i<X.size();i++){
        printf(" %d",X[i]);
    }
    printf("\n");
    printf("%d",Y.size());
    for(int i=0;i<Y.size();i++){
        printf(" %d",Y[i]);
    }
    printf("\n");
    return 0;
}
```



