# PAT 1013 Battle Over Cities - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1013 Battle Over Cities （25 分)








It is vitally important to have all the cities connected by highways in a war. If a city is occupied by the enemy, all the highways from/toward that city are closed. We must know immediately if we need to repair any other highways to keep the rest of the cities connected. Given the map of cities which have all the remaining highways marked, you are supposed to tell the number of highways need to be repaired, quickly.

For example, if we have 3 cities and 2 highways connecting 

### Input Specification:

Each input file contains one test case. Each case starts with a line containing 3 numbers 

### Output Specification:

For each of the 

### Sample Input:

```
3 2 3
1 2
1 3
1 2 3
```

### Sample Output:

```
1
0
0
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define maxnum 100005
int n,m,k;
int mp[1005][1005];
int temp[1005][1005];
int vis[1005] = {0};

int findnotvis(){
    for(int i=1;i <= n;i++){
        if(!vis[i]) return i;
    }
    return -1;
}


void dfs(int x){
    vis[x] = 1;
    for(int i=1;i <= n;i++){
        if(!vis[i]&&mp[x][i]){
            dfs(i);
        }
    }
}



int main(){
    cin >> n >> m >> k;
    memset(mp,0, sizeof(mp));
    for(int i=0;i < m;i++){
        int x,y;scanf("%d%d",&x,&y);
        mp[x][y] = mp[y][x] = 1;
    }

    while(k--){
        int num;cin >> num;
        vis[num] = 1; //只要把点置为访问过的就可以了，不需要删去边
        int cnt = 0;
        for(int i=1;i <= n;i++){ //这个写法很好，学习了。
            if(!vis[i]){
                dfs(i);
                cnt++;
            }
        }
        cout << cnt-1 << endl;
        memset(vis,0, sizeof(vis));
    }

    return 0;
}
```

果然用cin输入数据会超时。。



其实这是考察图的问题，删除图的一个节点，是其他节点成为连通图，至少需要添加多少条线

添加最少的路线，就是连通分量数-1(例：n个互相独立的连通分量组成一个连通图，只需要连n-1条线就可以)

这道题最重要就是求除去图的一个节点后 剩余的连通分量的数目

*利用邻接矩阵v存储路线，用visit数组表示城市是否被遍历过*

*对于每个被占领的城市，将其表示为遍历过的状态true即可*

*利用深度优先遍历dfs计算连通分量数目*

```
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<string>
#include<cstring>
#include<queue>
#include<cmath>
#include<set>
#include<map>
#define ll long long
#define lson l,m,rt<<1
#define rson m+1,r,rt<<1|1
using namespace std;
const int N=1010;
int n,m,a[N*N],b[N*N],pre[N];
 
int f(int x){
    if(pre[x]==x)return x;
    pre[x]=f(pre[x]);
    return pre[x];
}
 
void link(int x,int y){
    int r1=f(x),r2=f(y);
    if(r1!=r2){
        pre[r2]=r1;
    }
}
void solve(int city){
    for(int i=1;i<=n;i++){
        pre[i]=i;
    }
    for(int i=0;i<m;i++){
        if(a[i]==city||b[i]==city)continue;
        link(a[i],b[i]);
    }
    int ans=0;
    for(int i=1;i<=n;i++){   //查找堆的个数
        if(i==city)continue;
        int ri=f(i);  
        if(ri==i)ans++; *//是个堆顶*
} printf("%d\n",ans-1); } int main(){ int k; scanf("%d%d%d",&n,&m,&k); for(int i=0;i<m;i++){ scanf("%d%d",&a[i],&b[i]); } int x; for(int i=1;i<=k;i++){ scanf("%d",&x); solve(x); } }
```

用并查集也挺好的思想


` `













