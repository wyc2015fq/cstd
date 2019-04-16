# hdu 1874 Dijkstra算法模板 - 紫芝的博客 - CSDN博客





2018年04月23日 21:49:00[紫芝](https://me.csdn.net/qq_40507857)阅读数：25
个人分类：[解题模板](https://blog.csdn.net/qq_40507857/article/category/7565904)









                单源最短路径 迪杰斯特拉算法 
1.初始化地图，map[i][j]记录城镇 i，j之间最短的道路长度，
若无道路连通 ，则为极大值 
2.从起始城镇开始，用广度优先搜索思想，嵌入松弛处理算法，
用dis[i]记录起始城镇到城镇 i 的最短路径长度；

3.答案位于dis[t]，即终点t城镇到起始城镇的最小距离

```cpp
#include<stdio.h>
#include<queue>
#define inf 10000000
using namespace std;
int main(){
	int n,m;
	int map[200][200];
	//map[i][j]记录城镇 i 到 j 间最短连通道路长度
	int s,t;//记录起始村庄和终点村庄的编号
	int dis[200];
	//记录 i 号村庄到起始村庄的最短路径长度
	int x,y,d;
	int i,j;
	
	while(scanf("%d%d",&n,&m)!=EOF){
		for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		map[i][j]=inf;
	//所有路径初始化为最大值
	while(m--){
		scanf("%d%d%d",&x,&y,&d);
		if(map[x][y]>d)
		map[x][y]=d,map[y][x]=d;
	} 
		scanf("%d%d",&s,&t);
		
		queue<int> q;
		q.push(s);//起点入队
		for(i=0;i<n;i++) 
		dis[i]=inf;
		//所有城镇到起始位置的距离初始化为最大值
		dis[s]=0;//起始城镇到其自身的距离为0
		
		//所有与起始城镇连接都遍历完了，结束遍历 
		while(!q.empty()) {
			x=q.front();
			q.pop();
			for(i=0;i<n;i++){
				if(map[x][i]!=inf){
					//从城镇 x 到城镇 i 的路径（到起始城镇）较短 
					if(dis[i]>dis[x]+map[x][i])
					{
						dis[i]=dis[x]+map[x][i];
						//跟新 i 城镇到起始城镇的最短距离
						q.push(i); 
					}
				}
			} 
		}
		if(dis[t]==inf)
		printf("-1\n");
		else
		printf("%d\n",dis[t]);
	} 
	return 0;
}
```




