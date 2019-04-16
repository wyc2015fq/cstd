# HDU1233  还是畅通工程  kruskal算法 - 紫芝的博客 - CSDN博客





2018年05月07日 20:40:36[紫芝](https://me.csdn.net/qq_40507857)阅读数：39







|![](http://acm.hdu.edu.cn/images/banner.jpg)| | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|
||Online Judge|Online Exercise|Online Teaching|Online Contests|Exercise Author||----|----|----|----|----||[F.A.Q](http://acm.hdu.edu.cn/faq.php)[Hand In Hand](http://acm.hdu.edu.cn/friends)[Online Acmers](http://acm.hdu.edu.cn/onlineuser.php)[Forum |](http://acm.hdu.edu.cn/forum)[Discuss](http://acm.hdu.edu.cn/discuss/public/list.php)[Statistical Charts](http://acm.hdu.edu.cn/statistic/submit_graph.php)|[Problem Archive](http://acm.hdu.edu.cn/listproblem.php?vol=1)[Realtime Judge Status](http://acm.hdu.edu.cn/status.php)[Authors Ranklist](http://acm.hdu.edu.cn/ranklist.php)|[C/C++/Java Exams](http://code.hdu.edu.cn/)[ACM Steps](http://acm.hdu.edu.cn/game)[Go to Job](http://acm.hdu.edu.cn/ITJobs/)[Contest LiveCast](http://acm.hdu.edu.cn/livecast)[ICPC@China](http://acm.hdu.edu.cn/region)|[Best Coder beta](http://bestcoder.hdu.edu.cn/)[VIP](http://acm.hdu.edu.cn/vip/contest_list.php) | [STD Contests](http://acm.hdu.edu.cn/contests/contest_list.php)[Virtual Contests](http://code.hdu.edu.cn/vcontest)[DIY |](http://acm.hdu.edu.cn/diy/contest_list.php)[Web-DIYbeta](http://acm.hdu.edu.cn/webcontest)[Recent Contests](http://acm.hdu.edu.cn/recentcontest)|![Author](http://acm.hdu.edu.cn/images/user.png) liuhulin![Mail](http://acm.hdu.edu.cn/images/mail.png) Mail 0[(0)](http://acm.hdu.edu.cn/listmsg.php?type=new)![Control Panel](http://acm.hdu.edu.cn/images/panel.jpg) Control Panel ![Sign Out](http://acm.hdu.edu.cn/images/signout.png) Sign Out||Online Judge|Online Exercise|Online Teaching|Online Contests|Exercise Author|[F.A.Q](http://acm.hdu.edu.cn/faq.php)[Hand In Hand](http://acm.hdu.edu.cn/friends)[Online Acmers](http://acm.hdu.edu.cn/onlineuser.php)[Forum |](http://acm.hdu.edu.cn/forum)[Discuss](http://acm.hdu.edu.cn/discuss/public/list.php)[Statistical Charts](http://acm.hdu.edu.cn/statistic/submit_graph.php)|[Problem Archive](http://acm.hdu.edu.cn/listproblem.php?vol=1)[Realtime Judge Status](http://acm.hdu.edu.cn/status.php)[Authors Ranklist](http://acm.hdu.edu.cn/ranklist.php)|[C/C++/Java Exams](http://code.hdu.edu.cn/)[ACM Steps](http://acm.hdu.edu.cn/game)[Go to Job](http://acm.hdu.edu.cn/ITJobs/)[Contest LiveCast](http://acm.hdu.edu.cn/livecast)[ICPC@China](http://acm.hdu.edu.cn/region)|[Best Coder beta](http://bestcoder.hdu.edu.cn/)[VIP](http://acm.hdu.edu.cn/vip/contest_list.php) | [STD Contests](http://acm.hdu.edu.cn/contests/contest_list.php)[Virtual Contests](http://code.hdu.edu.cn/vcontest)[DIY |](http://acm.hdu.edu.cn/diy/contest_list.php)[Web-DIYbeta](http://acm.hdu.edu.cn/webcontest)[Recent Contests](http://acm.hdu.edu.cn/recentcontest)|![Author](http://acm.hdu.edu.cn/images/user.png) liuhulin![Mail](http://acm.hdu.edu.cn/images/mail.png) Mail 0[(0)](http://acm.hdu.edu.cn/listmsg.php?type=new)![Control Panel](http://acm.hdu.edu.cn/images/panel.jpg) Control Panel ![Sign Out](http://acm.hdu.edu.cn/images/signout.png) Sign Out|
|Online Judge|Online Exercise|Online Teaching|Online Contests|Exercise Author| | | | | | |
|[F.A.Q](http://acm.hdu.edu.cn/faq.php)[Hand In Hand](http://acm.hdu.edu.cn/friends)[Online Acmers](http://acm.hdu.edu.cn/onlineuser.php)[Forum |](http://acm.hdu.edu.cn/forum)[Discuss](http://acm.hdu.edu.cn/discuss/public/list.php)[Statistical Charts](http://acm.hdu.edu.cn/statistic/submit_graph.php)|[Problem Archive](http://acm.hdu.edu.cn/listproblem.php?vol=1)[Realtime Judge Status](http://acm.hdu.edu.cn/status.php)[Authors Ranklist](http://acm.hdu.edu.cn/ranklist.php)|[C/C++/Java Exams](http://code.hdu.edu.cn/)[ACM Steps](http://acm.hdu.edu.cn/game)[Go to Job](http://acm.hdu.edu.cn/ITJobs/)[Contest LiveCast](http://acm.hdu.edu.cn/livecast)[ICPC@China](http://acm.hdu.edu.cn/region)|[Best Coder beta](http://bestcoder.hdu.edu.cn/)[VIP](http://acm.hdu.edu.cn/vip/contest_list.php) | [STD Contests](http://acm.hdu.edu.cn/contests/contest_list.php)[Virtual Contests](http://code.hdu.edu.cn/vcontest)[DIY |](http://acm.hdu.edu.cn/diy/contest_list.php)[Web-DIYbeta](http://acm.hdu.edu.cn/webcontest)[Recent Contests](http://acm.hdu.edu.cn/recentcontest)|![Author](http://acm.hdu.edu.cn/images/user.png) liuhulin![Mail](http://acm.hdu.edu.cn/images/mail.png) Mail 0[(0)](http://acm.hdu.edu.cn/listmsg.php?type=new)![Control Panel](http://acm.hdu.edu.cn/images/panel.jpg) Control Panel ![Sign Out](http://acm.hdu.edu.cn/images/signout.png) Sign Out| | | | | | |
|| | | | | | | | | | |
|# 还是畅通工程**Time Limit: 4000/2000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 55271    Accepted Submission(s): 25114**Problem Description某省调查乡村交通状况，得到的统计表中列出了任意两村庄间的距离。省政府“畅通工程”的目标是使全省任何两个村庄间都可以实现公路交通（但不一定有直接的公路相连，只要能间接通过公路可达即可），并要求铺设的公路总长度为最小。请计算最小的公路总长度。Input测试输入包含若干测试用例。每个测试用例的第1行给出村庄数目N ( < 100 )；随后的N(N-1)/2行对应村庄间的距离，每行给出一对正整数，分别是两个村庄的编号，以及此两村庄间的距离。为简单起见，村庄从1到N编号。当N为0时，输入结束，该用例不被处理。Output对每个测试用例，在1行里输出最小的公路总长度。Sample Input31 2 11 3 22 3 441 2 11 3 41 4 12 3 32 4 23 4 50Sample Output35*Hint*HintHuge input, scanf is recommended.Source[浙大计算机研究生复试上机考试-2006年](http://acm.hdu.edu.cn/search.php?field=problem&key=%D5%E3%B4%F3%BC%C6%CB%E3%BB%FA%D1%D0%BE%BF%C9%FA%B8%B4%CA%D4%C9%CF%BB%FA%BF%BC%CA%D4-2006%C4%EA&source=1&searchmode=source)RecommendJGShining   |   We have carefully selected several similar problems for you:  [1232](http://acm.hdu.edu.cn/showproblem.php?pid=1232)[1875](http://acm.hdu.edu.cn/showproblem.php?pid=1875)[1879](http://acm.hdu.edu.cn/showproblem.php?pid=1879)[1301](http://acm.hdu.edu.cn/showproblem.php?pid=1301)[1162](http://acm.hdu.edu.cn/showproblem.php?pid=1162)[Statistic](http://acm.hdu.edu.cn/statistic.php?pid=1233) | [Submit](http://acm.hdu.edu.cn/submit.php?pid=1233) | [Discuss](http://acm.hdu.edu.cn/discuss/problem/list.php?problemid=1233) | [Note](http://acm.hdu.edu.cn/note/note.php?pid=1233)| | | | | | | | | | |
||[Home](http://acm.hdu.edu.cn/) | [Top](http://acm.hdu.edu.cn/showproblem.php?pid=1233#top)|Hangzhou Dianzi University Online Judge 3.0Copyright © 2005-2018 HDU ACM Team. All Rights Reserved.[Designer & Developer](http://acm.hdu.edu.cn/about_us/developer.php): Wang RongtaoLinLeGaoJieGanLuTotal 0.004000(s) query 5, Server time : 2018-05-07 20:34:59, Gzip enabled|[Administration](http://acm.hdu.edu.cn/admin)||----|----|----||[Home](http://acm.hdu.edu.cn/) | [Top](http://acm.hdu.edu.cn/showproblem.php?pid=1233#top)|Hangzhou Dianzi University Online Judge 3.0Copyright © 2005-2018 HDU ACM Team. All Rights Reserved.[Designer & Developer](http://acm.hdu.edu.cn/about_us/developer.php): Wang RongtaoLinLeGaoJieGanLuTotal 0.004000(s) query 5, Server time : 2018-05-07 20:34:59, Gzip enabled|[Administration](http://acm.hdu.edu.cn/admin)| | | | | | | |
|[Home](http://acm.hdu.edu.cn/) | [Top](http://acm.hdu.edu.cn/showproblem.php?pid=1233#top)|Hangzhou Dianzi University Online Judge 3.0Copyright © 2005-2018 HDU ACM Team. All Rights Reserved.[Designer & Developer](http://acm.hdu.edu.cn/about_us/developer.php): Wang RongtaoLinLeGaoJieGanLuTotal 0.004000(s) query 5, Server time : 2018-05-07 20:34:59, Gzip enabled|[Administration](http://acm.hdu.edu.cn/admin)| | | | | | | | |

[点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1233)

思路：






将所有边放入一个最小堆中，每次取出权重最小的边，看边的两个端点是否属于同一个集合，

不属于同一个集合就加入这条边，否则就舍弃，这种贪心的选择就是kruskal算法




```cpp
//#include<bits/stdc++.h>
#include<iostream>
#include<queue>
#include<cstring>
#include<cstdio>
using namespace std;
const int maxn=120;
struct edge{
int a,b,weight;
bool friend operator < (edge x,edge y)
{
        return x.weight>y.weight;
}
};
int pre[maxn];
int get_parent(int x)
{
    if(pre[x]==-1)  return x;
    return pre[x]=get_parent(pre[x]);
}
bool mix(int x,int y)
{
    int fx=get_parent(x),fy=get_parent(y);
    if(fx==fy)
        return 1;
    pre[fx]=fy;
    return 0;
}
priority_queue<edge> q;
int kruskal()
{
    int cost=0;
    while(!q.empty()){
        edge x=q.top();
        q.pop();
        if(!mix(x.a,x.b))
            cost+=x.weight;
    }
    return cost;
}
int main()
{
	ios::sync_with_stdio(false);
    int n;
    while(scanf("%d",&n)!=EOF&&n)
    {
        memset(pre,-1,sizeof(pre));
        int m=(n*(n-1))/2;
        edge e;
        for(int i=0;i<m;i++)
            {
                cin>>e.a>>e.b>>e.weight;
                q.push(e);
            }
        cout<<kruskal()<<endl;
    }
	return 0;
 }
```




