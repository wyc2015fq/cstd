# HDU 6187 Destroy Walls 最小生成树 - 紫芝的博客 - CSDN博客





2018年05月13日 10:42:12[紫芝](https://me.csdn.net/qq_40507857)阅读数：85








# Destroy Walls
****Time Limit: 8000/4000 MS (Java/Others)    Memory Limit: 132768/132768 K (Java/Others)Total Submission(s): 575    Accepted Submission(s): 249****

Problem Description

Long times ago, there are beautiful historic walls in the city. These walls divide the city into many parts of area.  

Since it was not convenient, the new king wants to destroy some of these walls, so he can arrive anywhere from his castle. We assume that his castle locates at 0.62√0.63√. 

There are  towers in the city, which numbered from 1 to n. The ith's location is . Also, there are m walls connecting the towers. Specifically, the ith wall connects the tower  and the tower (including the endpoint). The cost of destroying the ith wall is . 

Now the king asks you to help him to divide the city. Firstly, the king wants to destroy as less walls as possible, and in addition, he wants to make the cost least. 

The walls only intersect at the endpoint. It is guaranteed that no walls connects the same tower and no 2 walls connects the same pair of towers. Thait is to say, the given graph formed by the walls and towers doesn't contain any multiple edges or self-loops.

Initially, you should tell the king how many walls he should destroy at least to achieve his goal, and the minimal cost under this condition.




Input

There are several test cases.For each test case:

The first line contains 2 integer n, m.

Then next n lines describe the coordinates of the points.

Each line contains 2 integers . then m lines follow, the ith line contains 3 integers 10^5

31000001200000,1010000


Output

For each test case outout one line with 2 integers sperate by a space, indicate how many walls the king should destroy at least to achieve his goal, and the minimal cost under this condition. 




Sample Input


4 4

-1 -1

-1 1

1 1

1 -1

1 2 1

2 3 2

3 4 1

4 1 2




Sample Output


1 1


Source

[2017ACM/ICPC广西邀请赛-重现赛（感谢广西大学）](http://acm.hdu.edu.cn/search.php?field=problem&key=2017ACM%2FICPC%B9%E3%CE%F7%D1%FB%C7%EB%C8%FC-%D6%D8%CF%D6%C8%FC%A3%A8%B8%D0%D0%BB%B9%E3%CE%F7%B4%F3%D1%A7%A3%A9&source=1&searchmode=source)




Recommend

liuyiding   |   We have carefully selected several similar problems for you:  [6275](http://acm.hdu.edu.cn/showproblem.php?pid=6275)[6274](http://acm.hdu.edu.cn/showproblem.php?pid=6274)[6273](http://acm.hdu.edu.cn/showproblem.php?pid=6273)[6272](http://acm.hdu.edu.cn/showproblem.php?pid=6272)[6271](http://acm.hdu.edu.cn/showproblem.php?pid=6271)



[点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=6187)

## 题意：

给n个点的坐标，m条边（包括起点和终点，权值），
要求去掉重边和环，输出去掉边的个数num，和权值和的最小值min



## 思路：

最小生成树的简单变形：“最大生成树”，所需费用为cost,边数为cnt


预处理所有边的权值和sum，min=sum-cost,num=m-cnt

最小生成树的kruskal算法：


将m条边放入优先队列，每次取出权值最大的边，
如果这条边的两个顶点在同一个集合，舍弃这条边；
如果这条边的两个顶点不在同一个集合，cost加这条边的权值，cnt++



## hint：

完全没有用到这n个点的坐标，竟然AC啦，
只能说明数据太水，我太菜,大佬们多指教

```cpp
#include<cstdio>
#include<queue>
#include<cstring>
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn=1e5+10;
struct edge
{
    int u,v,w;//起点，终点，权值
    bool friend operator <(edge a,edge b)//重载小于运算符
    {
        return a.w<b.w;//每次让权值最大的边出队
    }
};
priority_queue<edge> q;
int pre[maxn];//每个点的前驱节点
int cnt;//最小生成树所需边的个数
int get_parent(int x)//查找x的前驱节点
{
    if(pre[x]==-1)  return x;
    return pre[x]=get_parent(pre[x]);//递归查询
}
bool mix(int x,int y)//判断x，y是否在同一个集合
{
    int fx=get_parent(x),fy=get_parent(y);
    if(fx==fy)//x，y在同一个集合
        return 1;
    pre[fx]=fy;//x,y不在同一个集合
 return 0;
}
ll kruskal()
{
    ll cost=0;//建树的花费
    while(!q.empty()){//队列非空
        edge x=q.top();
        q.pop();
        if(!mix(x.u,x.v))//若两点不在同一个集合
            cost+=x.w,cnt++;//建边
    }
    return cost;
}
int main()
{
    int n,m,x,y;
    while(scanf("%d%d",&n,&m)!=EOF){
            memset(pre,-1,sizeof(pre));//初始化每个点的前驱
        for(int i=0;i<n;i++)
            scanf("%d%d",&x,&y);
            ll sum=0;cnt=0;
        for(int i=0;i<m;i++)
        {
            edge x;
            scanf("%d%d%d",&x.u,&x.v,&x.w);
            sum+=x.w;//预处理边权和
            q.push(x);//入队
        }
        sum-=kruskal();
        printf("%d %d\n",m-cnt,sum);
        //cout<<m-cnt<<" "<<sum<<endl;
    }
    return 0;
}
```




