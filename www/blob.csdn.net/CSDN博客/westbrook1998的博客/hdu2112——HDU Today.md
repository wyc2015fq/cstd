# hdu2112——HDU Today - westbrook1998的博客 - CSDN博客





2018年08月03日 17:13:49[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：59








> 
经过锦囊相助，海东集团终于度过了危机，从此，HDU的发展就一直顺风顺水，到了2050年，集团已经相当规模了，据说进入了钱江肉丝经济开发区500强。这时候，XHD夫妇也退居了二线，并在风景秀美的诸暨市浬浦镇陶姚村买了个房子，开始安度晚年了。  

  这样住了一段时间，徐总对当地的交通还是不太了解。有时很郁闷，想去一个地方又不知道应该乘什么公交车，在什么地方转车，在什么地方下车（其实徐总自己有车，却一定要与民同乐，这就是徐总的性格）。  

  徐总经常会问蹩脚的英文问路：“Can you help me?”。看着他那迷茫而又无助的眼神，热心的你能帮帮他吗？  

  请帮助他用最短的时间到达目的地（假设每一路公交车都只在起点站和终点站停，而且随时都会开）。  

  Input 

  输入数据有多组，每组的第一行是公交车的总数N(0<=N<=10000)；  

  第二行有徐总的所在地start，他的目的地end；  

  接着有n行，每行有站名s，站名e，以及从s到e的时间整数`t(0<t<100)`(每个地名是一个长度不超过30的字符串)。  

  note：一组数据中地名数不会超过150个。  

  如果N==-1，表示输入结束。  

  Output 

  如果徐总能到达目的地，输出最短的时间；否则，输出“-1”。  

  Sample Input 

  6 

  xiasha westlake 

  xiasha station 60 

  xiasha ShoppingCenterofHangZhou 30 

  station westlake 20 

  ShoppingCenterofHangZhou supermarket 10 

  xiasha supermarket 50 

  supermarket westlake 10 

  -1 

  Sample Output 

  50 

  Hint: 

  The best route is: 

  xiasha->ShoppingCenterofHangZhou->supermarket->westlake 

  虽然偶尔会迷路，但是因为有了你的帮助 
**和**从此还是过上了幸福的生活。 

  ――全剧终――
最短路的题 

直接套dijkstra模板 

不过要注意几个点 

地点的名词是字符串 所以要用一个`map<string,int>` 转成数字来表示，再输入的时候判断是否有在map里，然后算出地点数量 

第一次wa忘记考虑到不了的情况 要输出-1 

第二次wa忘记考虑起点和终点一样的情况 要输出0 

第n++次wa忘记考虑起点或者终点不在输入的路线中的情况，所以要在输入起点和终点的时候也判断加入map
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#include <map>
using namespace std;
//边数
int m;
//节点数
int n;
string s,e;
map<string,int> sea;
const int MAXN=180;
const int INF=0x3f3f3f3f;
int costmap[MAXN][MAXN];
int lowcost[MAXN];
bool vis[MAXN];
void Dijkstra(int s){
    //初始化
    for(int i=1;i<n;i++){
        lowcost[i]=INF;
        vis[i]=false;
    }
    //注意这个
    lowcost[s]=0;
    for(int i=1;i<n;i++){
        int k=-1;
        int Min=INF;
        //找出未访问的最小lowcost和位置
        for(int j=1;j<n;j++){
            if(!vis[j] && lowcost[j]<Min){
                Min=lowcost[j];
                k=j;
            }
        }
        if(k==-1){
            break;
        }
        vis[k]=true;
        for(int j=1;j<n;j++){
            //走k这条路更快
            if(!vis[j] && lowcost[k]+costmap[k][j]<lowcost[j]){
                lowcost[j]=lowcost[k]+costmap[k][j];
            }
        }
    }
}
int main(void){
    while(~scanf("%d",&m)){
        if(m==-1){
            break;
        }
        sea.clear();
        cin >> s >> e;
        n=1;
        if(sea.find(s)==sea.end()){
            sea[s]=n++;
        }
        if(sea.find(e)==sea.end()){
            sea[e]=n++;
        }
        //cout << s << e << endl;
        string st,et;
        int cost;
        memset(costmap,INF,sizeof(costmap));
        while(m--){
            cin >> st >> et >> cost;
            if(sea.find(st)==sea.end()){
                sea[st]=n++;
            }
            if(sea.find(et)==sea.end()){
                sea[et]=n++;
            }
            costmap[sea[st]][sea[et]]=cost;
            costmap[sea[et]][sea[st]]=cost;
        }
        // for(int i=1;i<=n;i++){
        //     for(int j=1;j<=n;j++){
        //         printf("%d ",costmap[i][j]);
        //     }
        //     printf("\n");
        // }
        Dijkstra(sea[s]);
        // for(int i=1;i<n;i++){
        //     printf("%d ",lowcost[i]);
        // }
        // printf("\n");
        if(s.compare(e)==0){
            printf("0\n");
        }
        else{
            if(lowcost[sea[e]]!=INF){
                printf("%d\n",lowcost[sea[e]]);
            }
            else{
                printf("-1\n");
            }
        }
    }
    return 0;
}
```






