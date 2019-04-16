# HDU1102  Constructing Roads 最小生成树 - 紫芝的博客 - CSDN博客





2018年05月06日 22:36:59[紫芝](https://me.csdn.net/qq_40507857)阅读数：33










[点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1102)

|![](http://acm.hdu.edu.cn/images/banner.jpg)| | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|
||Online Judge|Online Exercise|Online Teaching|Online Contests|Exercise Author||----|----|----|----|----||[F.A.Q](http://acm.hdu.edu.cn/faq.php)[Hand In Hand](http://acm.hdu.edu.cn/friends)[Online Acmers](http://acm.hdu.edu.cn/onlineuser.php)[Forum |](http://acm.hdu.edu.cn/forum)[Discuss](http://acm.hdu.edu.cn/discuss/public/list.php)[Statistical Charts](http://acm.hdu.edu.cn/statistic/submit_graph.php)|[Problem Archive](http://acm.hdu.edu.cn/listproblem.php?vol=1)[Realtime Judge Status](http://acm.hdu.edu.cn/status.php)[Authors Ranklist](http://acm.hdu.edu.cn/ranklist.php)|[C/C++/Java Exams](http://code.hdu.edu.cn/)[ACM Steps](http://acm.hdu.edu.cn/game)[Go to Job](http://acm.hdu.edu.cn/ITJobs/)[Contest LiveCast](http://acm.hdu.edu.cn/livecast)[ICPC@China](http://acm.hdu.edu.cn/region)|[Best Coder beta](http://bestcoder.hdu.edu.cn/)[VIP](http://acm.hdu.edu.cn/vip/contest_list.php) | [STD Contests](http://acm.hdu.edu.cn/contests/contest_list.php)[Virtual Contests](http://code.hdu.edu.cn/vcontest)[DIY |](http://acm.hdu.edu.cn/diy/contest_list.php)[Web-DIYbeta](http://acm.hdu.edu.cn/webcontest)[Recent Contests](http://acm.hdu.edu.cn/recentcontest)|![Author](http://acm.hdu.edu.cn/images/user.png) liuhulin![Mail](http://acm.hdu.edu.cn/images/mail.png) Mail 0[(0)](http://acm.hdu.edu.cn/listmsg.php?type=new)![Control Panel](http://acm.hdu.edu.cn/images/panel.jpg) Control Panel ![Sign Out](http://acm.hdu.edu.cn/images/signout.png) Sign Out||Online Judge|Online Exercise|Online Teaching|Online Contests|Exercise Author|[F.A.Q](http://acm.hdu.edu.cn/faq.php)[Hand In Hand](http://acm.hdu.edu.cn/friends)[Online Acmers](http://acm.hdu.edu.cn/onlineuser.php)[Forum |](http://acm.hdu.edu.cn/forum)[Discuss](http://acm.hdu.edu.cn/discuss/public/list.php)[Statistical Charts](http://acm.hdu.edu.cn/statistic/submit_graph.php)|[Problem Archive](http://acm.hdu.edu.cn/listproblem.php?vol=1)[Realtime Judge Status](http://acm.hdu.edu.cn/status.php)[Authors Ranklist](http://acm.hdu.edu.cn/ranklist.php)|[C/C++/Java Exams](http://code.hdu.edu.cn/)[ACM Steps](http://acm.hdu.edu.cn/game)[Go to Job](http://acm.hdu.edu.cn/ITJobs/)[Contest LiveCast](http://acm.hdu.edu.cn/livecast)[ICPC@China](http://acm.hdu.edu.cn/region)|[Best Coder beta](http://bestcoder.hdu.edu.cn/)[VIP](http://acm.hdu.edu.cn/vip/contest_list.php) | [STD Contests](http://acm.hdu.edu.cn/contests/contest_list.php)[Virtual Contests](http://code.hdu.edu.cn/vcontest)[DIY |](http://acm.hdu.edu.cn/diy/contest_list.php)[Web-DIYbeta](http://acm.hdu.edu.cn/webcontest)[Recent Contests](http://acm.hdu.edu.cn/recentcontest)|![Author](http://acm.hdu.edu.cn/images/user.png) liuhulin![Mail](http://acm.hdu.edu.cn/images/mail.png) Mail 0[(0)](http://acm.hdu.edu.cn/listmsg.php?type=new)![Control Panel](http://acm.hdu.edu.cn/images/panel.jpg) Control Panel ![Sign Out](http://acm.hdu.edu.cn/images/signout.png) Sign Out|
|Online Judge|Online Exercise|Online Teaching|Online Contests|Exercise Author| | | | | | |
|[F.A.Q](http://acm.hdu.edu.cn/faq.php)[Hand In Hand](http://acm.hdu.edu.cn/friends)[Online Acmers](http://acm.hdu.edu.cn/onlineuser.php)[Forum |](http://acm.hdu.edu.cn/forum)[Discuss](http://acm.hdu.edu.cn/discuss/public/list.php)[Statistical Charts](http://acm.hdu.edu.cn/statistic/submit_graph.php)|[Problem Archive](http://acm.hdu.edu.cn/listproblem.php?vol=1)[Realtime Judge Status](http://acm.hdu.edu.cn/status.php)[Authors Ranklist](http://acm.hdu.edu.cn/ranklist.php)|[C/C++/Java Exams](http://code.hdu.edu.cn/)[ACM Steps](http://acm.hdu.edu.cn/game)[Go to Job](http://acm.hdu.edu.cn/ITJobs/)[Contest LiveCast](http://acm.hdu.edu.cn/livecast)[ICPC@China](http://acm.hdu.edu.cn/region)|[Best Coder beta](http://bestcoder.hdu.edu.cn/)[VIP](http://acm.hdu.edu.cn/vip/contest_list.php) | [STD Contests](http://acm.hdu.edu.cn/contests/contest_list.php)[Virtual Contests](http://code.hdu.edu.cn/vcontest)[DIY |](http://acm.hdu.edu.cn/diy/contest_list.php)[Web-DIYbeta](http://acm.hdu.edu.cn/webcontest)[Recent Contests](http://acm.hdu.edu.cn/recentcontest)|![Author](http://acm.hdu.edu.cn/images/user.png) liuhulin![Mail](http://acm.hdu.edu.cn/images/mail.png) Mail 0[(0)](http://acm.hdu.edu.cn/listmsg.php?type=new)![Control Panel](http://acm.hdu.edu.cn/images/panel.jpg) Control Panel ![Sign Out](http://acm.hdu.edu.cn/images/signout.png) Sign Out| | | | | | |
|| | | | | | | | | | |
|# Constructing Roads**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 26460    Accepted Submission(s): 10130**Problem DescriptionThere are N villages, which are numbered from 1 to N, and you should build some roads such that every two villages can connect to each other. We say two village A and B are connected, if and only if there is a road between A and B, or there exists a village C such that there is a road between A and C, and C and B are connected. We know that there are already some roads between some villages and your job is the build some roads such that all the villages are connect and the length of all the roads built is minimum.InputThe first line is an integer N (3 <= N <= 100), which is the number of villages. Then come N lines, the i-th of which contains N integers, and the j-th of these N integers is the distance (the distance should be an integer within [1, 1000]) between village i and village j.Then there is an integer Q (0 <= Q <= N * (N + 1) / 2). Then come Q lines, each line contains two integers a and b (1 <= a < b <= N), which means the road between village a and village b has been built.OutputYou should output a line contains an integer, which is the length of all the roads to be built such that all the villages are connected, and this value is minimum. Sample Input30 990 692990 0 179692 179 011 2Sample Output179Source[kicc](http://acm.hdu.edu.cn/search.php?field=problem&key=kicc&source=1&searchmode=source)RecommendEddy   |   We have carefully selected several similar problems for you:  [1233](http://acm.hdu.edu.cn/showproblem.php?pid=1233)[1301](http://acm.hdu.edu.cn/showproblem.php?pid=1301)[1162](http://acm.hdu.edu.cn/showproblem.php?pid=1162)[1232](http://acm.hdu.edu.cn/showproblem.php?pid=1232)[1875](http://acm.hdu.edu.cn/showproblem.php?pid=1875)[Statistic](http://acm.hdu.edu.cn/statistic.php?pid=1102) | [Submit](http://acm.hdu.edu.cn/submit.php?pid=1102) | [Discuss](http://acm.hdu.edu.cn/discuss/problem/list.php?problemid=1102) | [Note](http://acm.hdu.edu.cn/note/note.php?pid=1102)| | | | | | | | | | |
||[Home](http://acm.hdu.edu.cn/) | [Top](http://acm.hdu.edu.cn/showproblem.php?pid=1102#top)|Hangzhou Dianzi University Online Judge 3.0Copyright © 2005-2018 HDU ACM Team. All Rights Reserved.[Designer & Developer](http://acm.hdu.edu.cn/about_us/developer.php): Wang RongtaoLinLeGaoJieGanLuTotal 0.004000(s) query 5, Server time : 2018-05-06 22:29:14, Gzip enabled|[Administration](http://acm.hdu.edu.cn/admin)||----|----|----||[Home](http://acm.hdu.edu.cn/) | [Top](http://acm.hdu.edu.cn/showproblem.php?pid=1102#top)|Hangzhou Dianzi University Online Judge 3.0Copyright © 2005-2018 HDU ACM Team. All Rights Reserved.[Designer & Developer](http://acm.hdu.edu.cn/about_us/developer.php): Wang RongtaoLinLeGaoJieGanLuTotal 0.004000(s) query 5, Server time : 2018-05-06 22:29:14, Gzip enabled|[Administration](http://acm.hdu.edu.cn/admin)| | | | | | | |
|[Home](http://acm.hdu.edu.cn/) | [Top](http://acm.hdu.edu.cn/showproblem.php?pid=1102#top)|Hangzhou Dianzi University Online Judge 3.0Copyright © 2005-2018 HDU ACM Team. All Rights Reserved.[Designer & Developer](http://acm.hdu.edu.cn/about_us/developer.php): Wang RongtaoLinLeGaoJieGanLuTotal 0.004000(s) query 5, Server time : 2018-05-06 22:29:14, Gzip enabled|[Administration](http://acm.hdu.edu.cn/admin)| | | | | | | | |





## 分析：

题目中已经给出一些道路，这些道路将村庄将村庄构成了部分连通集
要求的是使全部村庄连通的最小道路和，实际为最小生成树问题




### 1.kruskal算法

已经给出一些边，可能不是最小生成树的边，只需要选择边长度和最小即可
在给出的边的集合中，已经构成部分连通集，接下来将所有边放入一个最小堆中，
每次取出权重最小的边，看边的两个端点是否属于同一个集合，不属于就加入这条边，
否则就舍弃，这种贪心的选择就是kruskal算法





```cpp
//kruskal算法
#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
const int maxn=120;
struct Edge
{
    int v,u,weight;
    bool friend operator < (Edge a,Edge b)
    {
        return a.weight>b.weight;
    }
};
//并查集模板
int pre[maxn];
int get_parent(int x)
{
    if(-1==pre[x])   return x;
    return pre[x]=get_parent(pre[x]);
}
bool mix (int x,int y)
{
    int fx=get_parent(x),fy=get_parent(y);
    if(fx==fy)
        return 1;
    pre[fx]=fy;
    return 0;
}

priority_queue<Edge> E;
int kruskal()
{
    int cost=0;
    while(!E.empty()){
        Edge now=E.top();
        E.pop();
        if(!mix(now.v,now.u)){//不在同一个集合中
            cost+=now.weight;
        }
    }
    return cost;
}
int main()
{
    int n;
    while(cin>>n){
            while(!E.empty())   E.pop();
    memset(pre,-1,sizeof(pre));
            Edge e;
        for(int i=1;i<=n;i++){
                e.v=i;
            for(int j=1;j<=n;j++)
        {
            e.u=j;
        cin>>e.weight;
        if(i<j) E.push(e);
        }
    }
    int q,a,b;
    cin>>q;
    while(q--){
        cin>>a>>b;
        mix(a,b);
    }
    int ans=kruskal();
    cout<<ans<<endl;
    }
    return 0;
}
```



### 2.Prime算法



       由于题目中给定了一些边，处理的办法是将这些边权值设置为0.定义一个visit[i]数组记录顶点i是否在树中。




```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
#define MAX_SIZE 105
int parent[MAX_SIZE];            //记录父节点
int E[MAX_SIZE][MAX_SIZE];       //边集合
int Prim(int N);
bool visit[MAX_SIZE];            //判断该顶点是否在集合里面
int main(){
	int Q, N, i, j, v, w, ans;
	while (scanf("%d",&N)!=EOF){
		memset(visit, 0, sizeof(visit));    //初始化
		for (i = 1; i <= N; i++)
		for (j = 1; j <= N; j++)
			scanf("%d",&E[i][j]);            //输入边
		scanf("%d", &Q);
		for (i = 0; i < Q; i++){
			scanf("%d%d",&v,&w);
			E[v][w] = E[w][v] = 0;     //已建好将权值设为0
		}
		//-----------------------
		ans = Prim(N);
		printf("%d\n",ans);
	}
	return 0;
}
int Prim(int N){
	int i, j, k,pos,lmin,ans;
	parent[1] =-1;         //设置根节点
	visit[1] = 1;          //表示顶点 1已加入树中
	int LowCost[MAX_SIZE];
	for (i = 1; i <= N; i++)
		LowCost[i] = INT_MAX;      //初始化
	ans = 0;
	j = 1;
	for (i = 2; i <= N; i++){
		/*j表示上一次加入集合的顶点*/
		for (k = 1; k <= N; k++){
			if (!visit[k] && LowCost[k] > E[k][j]){   //更新j的邻接点
				parent[k] = j;           //将k父节点暂定为 j
				LowCost[k] = E[k][j];
			}
		}
		lmin = INT_MAX, pos = 0;
		for (k = 1; k <= N; k++){       //找出不在树中且离树距离最小的点
			if (!visit[k]&&lmin > LowCost[k]){
				pos = k;
				lmin = LowCost[k];
			}
		}
		visit[pos] = 1;      //加入树中
		ans += E[pos][parent[pos]];  //加入边
		j = pos;       //更新j
	}
	return ans;
}
```






