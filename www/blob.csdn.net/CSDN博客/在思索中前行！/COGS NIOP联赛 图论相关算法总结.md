# COGS NIOP联赛 图论相关算法总结 - 在思索中前行！ - CSDN博客





2014年12月01日 18:40:36[_Tham](https://me.csdn.net/txl16211)阅读数：629







- 
最小生成树
- 
Kruskal+ufs
int ufs(int x){return f[x]== x ? x : f[x]= ufs(f[x]);}intKruskal(){int w =0;for(int i=0; i<n; i++)
        f[i]= i;
    sort(e, e+n);for(int i=0; i<n; i++){int x = ufs(e[i].u), y = ufs(e[i].v);if(x != y){
            f[x]= y;
            w += e[i].w;}}return w;}



- 
Prim
intPrim(){int w =0;
    priority_queue<pair<int,int>> q;bool l[N]={0};
    l[1]=1; q.push(make_pair(0,1));for(int k=1; k<n; k++){int u = q.top().second; q.pop();for(int i=0; i<G[u].size(); i++)if(!l[G[u][i]])
                q.push(make_pair(-c[u][i], G[u][i]));while(!q.empty()&& l[q.top().second])
            q.pop();
        l[q.top().second]=1;
        w +=-q.top().first;
        q.pop();}return w;}




- 
最短路径
- 
Dijkstra+priority_queue
voidDijkstra(int s){
    priority_queue<pair<int,int>> q;bool l[N]={0}; l[s]=1;
    fill_n(f, n, INF); f[s]=0;
    q.push(make_pair(-f[s], s));while(!q.empty()){int u = q.front().second; q.pop();for(int i=0; i<G[u].size(); i++){int v = G[u][i];if(f[v]> f[u]+ c[u][i]){
                f[v]= f[u]+ c[u][i];if(!l[v]){
                    l[v]=1;
                    q.push(make_pair(-f[v], v));}}}}}



- 
Bellman-Ford (SPFA)
voidBellmanFord(int s){// SPFA
    queue<int> q;bool l[N]={0}; l[s]=1;
    fill_n(f, n, INF); f[s]=0;
    q.push(s);while(!q.empty()){int u = q.front(); q.pop();
        l[u]=0;for(int i=0; i<G[u].size(); i++){int v = G[u][i];if(f[v]> f[u]+ c[u][i]){
                f[v]= f[u]+ c[u][i];if(!l[v]){
                    l[v]=1;
                    q.push(v);}}}}}



- 
Floyd
voidFloyd(){for(int k=0; k<n; k++)for(int i=0; i<n; i++)for(int j=0; j<n; j++)
                f[i][j]= min(f[i][j], f[i][k]+ f[k][j]);}




- 
二分图
- 
ufs 验证

- 
Hungary
bool DFS(int u){for(int i=0; i<G[u].size(); i++){int v = G[u][i];if(!l[v]){
            l[v]=1;if(!f[v]|| DFS(f[v])){
                f[v]= u;returntrue;}}}returnfalse;}intHungary(){int w =0;for(int i=0; i<n; i++){
        fill_n(l, l+n,0);if(DFS(i))
            w++;}return w;}




- 
连通分量
- 
Tarjan
stack<int> s;voidTarjan(int u){
    dfn[u]= low[u]=++time;
    l[u]=1;
    s.push(u);for(int i=0; i<G[u].size(); i++){int v = G[u][i];if(!dfn[v]){Tarjan(v);
            low[u]= min(low[u], low[v]);}elseif(l[v])
            low[u]= min(low[u], dfn[v]);}if(dfn[u]== low[u]){
        w++;do{int v;
            l[v = s.top()]=0;
            f[v]= w;
            s.pop();}while(u != v);}}void SCC(){
    fill_n(dfn, n,0);for(int i=0; i<n; i++)if(!dfn(i))Tarjan(i);}




- 
*网络流
- 
最大流：Edmonds-Karp

- 
费用流：Bellman-Ford 找增广路，或者用贪心求解





