# 【HDOJ 1242 Rescue】bfs+优先队列 - qq_37385726的博客 - CSDN博客





2018年05月26日 23:02:04[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：29标签：[ACM																[hdoj 1242																[bfs																[优先队列](https://so.csdn.net/so/search/s.do?q=优先队列&t=blog)
个人分类：[ACM																[ACM 图论](https://blog.csdn.net/qq_37385726/article/category/7693052)](https://blog.csdn.net/qq_37385726/article/category/7683299)





![](https://img-blog.csdn.net/2018052623000517?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


![](https://img-blog.csdn.net/20180526230024238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





```cpp
#include<iostream>
using namespace std;
#include<queue>

int dir[4][2]={{0,1},{1,0},{0,-1},{-1,0}};
char maze[201][201];
int n,m;

struct Node
{
	int x,y,t;
	Node(int xx,int yy,int tt);
	bool operator < (Node b) const
	{
		return b.t<t;
	}
};

Node::Node(int xx,int yy,int tt)
{
	x=xx;
	y=yy;
	t=tt;
}

void bfs(int sx,int sy)
{
	priority_queue<Node> pq;
	Node temp(sx,sy,0);
	pq.push(temp);
	while(pq.size())
	{
		Node top=pq.top();
		pq.pop();
		maze[top.x][top.y]='#';
		for(int i=0;i<4;i++)
		{
			temp.x=top.x+dir[i][0];
			temp.y=top.y+dir[i][1];
			if(temp.x<0||temp.y<0||temp.x>=n||temp.y>=m)
			{
				continue;
			}
			if(maze[temp.x][temp.y]=='#')
			{
				continue;
			}
			if(maze[temp.x][temp.y]=='r')
			{
				cout<<top.t+1<<endl;
				return;
			}
			if(maze[temp.x][temp.y]=='.')
			{
				temp.t=top.t+1;
			}
			if(maze[temp.x][temp.y]=='x')
			{
				temp.t=top.t+2;
			}
			pq.push(temp);
		}
	}
	cout<<"Poor ANGEL has to stay in the prison all his life."<<endl;
}

int main(void)
{
	while(~scanf("%d %d",&n,&m))
	{
		int sx,sy;
		for(int i=0;i<n;i++)
		{
			for(int k=0;k<m;k++)
			{
				cin>>maze[i][k];
				if(maze[i][k]=='a')
				{
					sx=i;
					sy=k;
				}
			}
		}
		bfs(sx,sy);
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)](https://so.csdn.net/so/search/s.do?q=hdoj 1242&t=blog)](https://so.csdn.net/so/search/s.do?q=ACM&t=blog)




