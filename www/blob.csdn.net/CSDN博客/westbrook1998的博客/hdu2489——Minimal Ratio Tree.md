# hdu2489——Minimal Ratio Tree - westbrook1998的博客 - CSDN博客





2019年02月11日 23:53:49[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：25








题意：给定一个n个点的完全图，选出其中m个点的一颗树，使得这棵树的边权和/点权和最小

n很小(<=15)，所以可以用dfs来枚举所有m大小的顶点子集，然后对这个顶点集跑一下最小生成树，因为是完全图所以肯定能求出最小生成树，然后不断更新答案

犯了好几个傻逼错误，dfs枚举也写错，调试的时候想把dfs输出看看结果输出了1.4g的文件。。。怪不得超时，然后最后输出也写错，应该是m而不是n，错了几发pe

代码：

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N=20;
const int INF=0x3f3f3f3f;
const double DINF=0x3f3f3f3f;
const double eps=1e-8;
//暴力dfs出m个点然后求最小比例树
int n,m;
int a[N];
int cost[N][N];
vector<int> sel;
int low[N];
bool vis[N];
bool used[N];
int Prim(vector<int> &s){
    int ans=0;
    for(int i=0;i<m;i++){
        vis[s[i]]=false;
        low[s[i]]=cost[s[0]][s[i]];
    }
    vis[s[0]]=true;
    for(int i=1;i<m;i++){
        int Min=INF;
        int k=-1;
        for(int j=0;j<m;j++){
            if(!vis[s[j]] && Min>low[s[j]]){
                Min=low[s[j]];
                k=j;
            }
        }
        if(k==-1){
            return -1;
        }
        ans+=Min;
        vis[s[k]]=true;
        for(int j=0;j<m;j++){
            if(!vis[s[j]] && low[s[j]]>cost[s[k]][s[j]]){
                low[s[j]]=cost[s[k]][s[j]];
            }
        }
    }
    return ans;
}
double res;
//选择的点集 已选个数 已选总点权 当前下标
void dfs(vector<int> &s,int num,int sum,int idx){
    if(n-m<idx+1-num){
        return;
    }
    if(m==num){
        int es=Prim(s);
        if(es==-1){
            return;
        }
        // printf("%lf %d %d\n",es*1.0/sum-res,sum,num);
        if(es*1.0/sum<res){
            res=es*1.0/sum;
            sel=s;
        }
        return;
    }
    for(int i=idx+1;i<n;i++){
        if(used[i]){
            continue;
        }
        used[i]=true;
        s.push_back(i);
        //sb错误
        // dfs(s,num+1,sum+a[i],idx+1);
        dfs(s,num+1,sum+a[i],i);
        s.pop_back();
        used[i]=false;
    }
}
int main(void){
    // freopen("in.txt","r",stdin);
    // freopen("out.txt","w",stdout);
    while(~scanf("%d%d",&n,&m)){
        if(n==0 && m==0){
            break;
        }
        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
        }
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                scanf("%d",&cost[i][j]);
            }
        }
        vector<int> s;
        sel.clear();
        res=DINF;
        dfs(s,0,0,-1);
        sort(sel.begin(),sel.end());
        int siz=sel.size();
        for(int i=0;i<siz;i++){
            printf("%d",sel[i]+1);
            //sb错误 写成n-1
            if(i!=siz-1){
                printf(" ");
            }
        }
        printf("\n");
    }
    return 0;
}
```



