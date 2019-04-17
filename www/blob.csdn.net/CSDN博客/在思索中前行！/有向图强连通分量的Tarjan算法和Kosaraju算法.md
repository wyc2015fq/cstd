# 有向图强连通分量的Tarjan算法和Kosaraju算法 - 在思索中前行！ - CSDN博客





2014年08月16日 20:31:58[_Tham](https://me.csdn.net/txl16211)阅读数：1272








### **[有向图强连通分量]**

在有向图G中，如果两个顶点间至少存在一条路径，称两个顶点**强连通**(strongly connected)。如果有向图G的每两个顶点都强连通，称G是一个**强连通图**。非强连通图有向图的极大强连通子图，称为**强连通分量**(strongly connected components)。

下图中，子图{1,2,3,4}为一个强连通分量，因为顶点1,2,3,4两两可达。{5},{6}也分别是两个强连通分量。

![image](https://www.byvoid.com/upload/wp/2009/04/image1.png)

直接根据定义，用双向遍历取交集的方法求强连通分量，时间复杂度为O(N^2+M)。更好的方法是Kosaraju算法或Tarjan算法，两者的时间复杂度都是O(N+M)。本文介绍的是Tarjan算法。

### **[Tarjan算法]**

Tarjan算法是基于对图深度优先搜索的算法，每个强连通分量为搜索树中的一棵子树。搜索时，把当前搜索树中未处理的节点加入一个堆栈，回溯时可以判断栈顶到栈中的节点是否为一个强连通分量。

定义DFN(u)为节点u搜索的次序编号(时间戳)，Low(u)为u或u的子树能够追溯到的最早的栈中节点的次序号。由定义可以得出，

```cpp
Low(u)=Min
{
    DFN(u),
    Low(v),(u,v)为树枝边，u为v的父节点
    DFN(v),(u,v)为指向栈中节点的后向边(非横叉边)
}
```

当DFN(u)=Low(u)时，以u为根的搜索子树上所有节点是一个强连通分量。

算法伪代码如下

```cpp
tarjan(u)
{
    DFN[u]=Low[u]=++Index                      // 为节点u设定次序编号和Low初值
    Stack.push(u)                              // 将节点u压入栈中
    for each (u, v) in E                       // 枚举每一条边
        if (v is not visted)               // 如果节点v未被访问过
            tarjan(v)                  // 继续向下找
            Low[u] = min(Low[u], Low[v])
        else if (v in S)                   // 如果节点v还在栈内
            Low[u] = min(Low[u], DFN[v])
    if (DFN[u] == Low[u])                      // 如果节点u是强连通分量的根
        repeat
            v = S.pop                  // 将v退栈，为该强连通分量中一个顶点
            print v
        until (u== v)
}
```

接下来是对算法流程的演示。

从节点1开始DFS，把遍历到的节点加入栈中。搜索到节点u=6时，DFN[6]=LOW[6]，找到了一个强连通分量。退栈到u=v为止，{6}为一个强连通分量。

![image](https://www.byvoid.com/upload/wp/2009/04/image2.png)

返回节点5，发现DFN[5]=LOW[5]，退栈后{5}为一个强连通分量。

![image](https://www.byvoid.com/upload/wp/2009/04/image3.png)

返回节点3，继续搜索到节点4，把4加入堆栈。发现节点4向节点1有后向边，节点1还在栈中，所以LOW[4]=1。节点6已经出栈，(4,6)是横叉边，返回3，(3,4)为树枝边，所以LOW[3]=LOW[4]=1。

![image](https://www.byvoid.com/upload/wp/2009/04/image4.png)

继续回到节点1，最后访问节点2。访问边(2,4)，4还在栈中，所以LOW[2]=DFN[4]=5。返回1后，发现DFN[1]=LOW[1]，把栈中节点全部取出，组成一个连通分量{1,3,4,2}。

![image](https://www.byvoid.com/upload/wp/2009/04/image5.png)

至此，算法结束。经过该算法，求出了图中全部的三个强连通分量{1,3,4,2},{5},{6}。

可以发现，运行Tarjan算法的过程中，每个顶点都被访问了一次，且只进出了一次堆栈，每条边也只被访问了一次，所以该算法的时间复杂度为O(N+M)。

求有向图的强连通分量还有一个强有力的算法，为Kosaraju算法。Kosaraju是基于对有向图及其逆图两次DFS的方法，其时间复杂度也是O(N+M)。与Trajan算法相比，Kosaraju算法可能会稍微更直观一些。但是Tarjan只用对原图进行一次DFS，不用建立逆图，更简洁。在实际的测试中，Tarjan算法的运行效率也比Kosaraju算法高30%左右。此外，该Tarjan算法与[求无向图的双连通分量(割点、桥)的Tarjan算法](https://www.byvoid.com/blog/biconnect/)也有着很深的联系。学习该Tarjan算法，也有助于深入理解求双连通分量的Tarjan算法，两者可以类比、组合理解。

求有向图的强连通分量的Tarjan算法是以其发明者[Robert Tarjan](http://en.wikipedia.org/wiki/Robert_Tarjan)命名的。Robert Tarjan还发明了求[双连通分量](https://www.byvoid.com/blog/biconnect/)的Tarjan算法，以及求最近公共祖先的离线Tarjan算法，在此对Tarjan表示崇高的敬意。

附：tarjan算法的C++程序



```cpp
void tarjan(int i)
{
    int j;
    DFN[i]=LOW[i]=++Dindex;
    instack[i]=true;
    Stap[++Stop]=i;
    for (edge *e=V[i];e;e=e->next)
    {
        j=e->t;
        if (!DFN[j])
        {
            tarjan(j);
            if (LOW[j]<LOW[i])
                LOW[i]=LOW[j];
        }
        else if (instack[j] && DFN[j]<LOW[i])
            LOW[i]=DFN[j];
    }
    if (DFN[i]==LOW[i])
    {
        Bcnt++;
        do
        {
            j=Stap[Stop--];
            instack[j]=false;
            Belong[j]=Bcnt;
        }
        while (j!=i);
    }
}
void solve()
{
    int i;
    Stop=Bcnt=Dindex=0;
    memset(DFN,0,sizeof(DFN));
    for (i=1;i<=N;i++)
        if (!DFN[i])
            tarjan(i);
}
```

完整算法实现：





```cpp
#include<iostream>
using namespace std;
int time=1,low[1000],dfn[1000];

int stack[1000],top=0;

bool instack[1000]={false};

struct LIST
{
	int v;
	LIST *next;
};

LIST *head[1000]={NULL};

int min(int a,int b)
{
	if(a<b)
		return a;
	return b;
}


void tarjan(int v)		/*tarjan求强连通分支*/
{
	dfn[v]=low[v]=time++;		/*标记点v的DFS遍历序号*/
	stack[top++]=v;				/*将点v入栈*/
	instack[v]=true;			/*标记点v已经在栈中*/
	for(LIST *p=head[v];p!=NULL;p=p->next)		/*遍历V能直接到达的点*/
		if(!dfn[p->v])	/*如果v的邻接点没有入过栈*/
		{
			tarjan(p->v);
			low[v]=min(low[v],low[p->v]);	/*如果v能直接到达的这个点没在栈中,v的最早祖先为他们中的较小值*/
		}
		else if(instack[p->v])	/*如果在栈中*/
			low[v]=min(low[v],dfn[p->v]);	/*如果在栈中，则v的最早祖先是他的序号和那个点的序号较小的*/

	if(dfn[v]==low[v])		/*如果dfn[v]和low[v]相等，则说明v点是其所属强连通分支DFS遍历起点，这个强连通分支说有点都在v点之上*/
	{
		cout<<"{ ";
		do
		{
			v=stack[--top];
			instack[v]=false;
			cout<<v<<' ';
		}while(dfn[v]!=low[v]);
		cout<<"}"<<endl;		
	}
}

int main()
{
	int i,j,n,m;
	cin>>n;

	memset(dfn,0,sizeof(char)*4000);
	for(i=1;i<=n;i++)
	{	
		cout<<i<<"的邻接点数量:";
		cin>>m;
		cout<<"输入每个邻接点编号";
		LIST *rear=head[i];
		for(j=0;j<m;j++)		/*创建邻接表*/
		{
			if(!j)
			{
				rear=new LIST;
				head[i]=rear;
			}
			else
			{
				rear->next=new LIST;
				rear=rear->next;
			}
			rear->next=NULL;
			cin>>rear->v;
		}
	}
	
	for(i=1;i<=n;i++)
		if(!dfn[i])		/*如果i没有入过栈*/
			tarjan(i);


	return 0;
}
```



## 强连通分量 – Kosaraju Algorithm


强连通分量（Strongly Connected Components），简称SCC。是指在给定的一张图的一个子图这个子图满足对于其中的任意一对点均存在这样两条路径。


如果我们把强连通分量缩成一个点，这时候，原图则会变成有向无环图。


图是有向无环图当且仅当该图中没有点集合元素个数大于1的强连通分量。且任意一个强连通分量都至少包含一个有向环。下面我们通过一张图片来理解一下强连通分量以及缩点：

![SCC](http://www.ivy-end.com/wp-content/uploads/2013/11/SCC.png)




对于统计给定的图中强连通分量的个数，我们可以应用并查集在时间内得到求解。


如果不仅需要统计强连通分量的个数，还要将强连通分量缩点，则需要用到今天介绍的Kosaraju Algorithm。它的具体步骤如下：
- 
对原图进行DFS并将出栈顺序进行逆序，得到的顺序就是拓扑序列。
- 
将原图的每一条边反向，得到反图。
- 
按照第一步生成的拓扑序列的顺序再对反图进行DFS染色，染成同色的就是一个强连通分量。


这个算法比较容易理解，也是最通用的算法。它主要是同时运用了原图和反图。


该算法具有一个性质：如果我们把求出来的每个强连通分量缩成一个点，并且用求出每个强连通分量的顺序来标记收缩后的结点，那么这个顺序就是强连通分量缩点后所形成的有向无环图的拓扑序列。


代码如下：

```cpp
#include <iostream>
#include <cstring>
#include <stack>

using namespace std;

const int MAX = 10240;

int N, M, nCnt = 0;
int pMap[MAX][MAX], pColor[MAX];
stack<int> S;	// 储存拓扑序列

void dfs1(int x);	// 原图DFS
void dfs2(int x);	// 反图DFS
void Kosaraju();

int main()
{
	cin >> N >> M;
	memset(pMap, 0, sizeof(pMap));
	for(int i = 1; i <= M; i++)
	{
		int s, e;
		cin >> s >> e;
		pMap[s][e] = 1;		// 有向图
	}
	Kosaraju();
	return 0;
}

void Kosaraju()
{
	memset(pColor, 0, sizeof(pColor));
	for(int i = 1; i <= N; i++)	// DFS原图求出拓扑序列
	{
		if(!pColor[i])
		{ dfs1(i); }
	}

	memset(pColor, 0, sizeof(pColor));
	while(!S.empty())	// 按照拓扑序列DFS反图
	{
		int x = S.top(); S.pop();
		if(!pColor[x])
		{
			nCnt++;		// 找到一个强连通分量
			dfs2(x);
		}
	}
	cout << "The number of SCC is " << nCnt << endl;
}

void dfs1(int x)
{
	pColor[x] = 1;	// 染色
	for(int i = 1; i <= N; i++)
	{
		if(pMap[x][i] == 1 && !pColor[i])
		{ dfs1(i); }
	}
	S.push(x);	// 加入拓扑序列
}

void dfs2(int x)
{
	pColor[x] = nCnt;	// 属于第几个强连通分量
	for(int i = 1; i <= N; i++)
	{
		if(pMap[i][x] == 1 && !pColor[i])	// 原邻接矩阵的对称矩阵为反图，也可以自己建立一个反图RMap[][],然后根据RMap[x][i]进行搜索染色
		{ dfs2(i); }
	}
}
```

回顾一下Kosaraju的主要步骤：
- 对G求解Reverse
 Post-Order，即上文中的”伪拓扑排序“
- 对G进行转置得到GR
- 按照第一步得到的集合中顶点出现的顺序，对GR调用DFS得到若干颗搜索树
- 每一颗搜索树就代表了一个强连通分量
这个算法的想法很巧妙，为了突出回向边，而对图进行转置，然后对转置的图按照之前得到的顶点(拓扑)序列进行DFS调用。



算法的正确性进行证明：

**有向图的这样一个性质，一个图和他的transpose graph(边全部反向)具有相同的强连通分量！**


证明的目标，就是最后一步 ---每一颗搜索树代表的就是一个强连通分量

证明：设在图GR中，调用DFS(s)能够到达顶点v，那么顶点s和v是强连通的。

两个顶点如果是强连通的，那么彼此之间都有一条路径可达，因为DFS(s)能够达到顶点v，因此从s到v的路径必然存在。现在关键就是需要证明在GR中从v到s也是存在一条路径的，也就是要证明在G中存在s到v的一条路径。

而之所以DFS(s)能够在DFS(v)之前被调用，是因为在对G获取ReversePost-Order序列时，s出现在v之前，这也就意味着，v是在s之前加入该序列的(因为该序列使用栈作为数据结构，先加入的反而会在序列的后面)。因此根据DFS调用的递归性质，DFS(v)应该在DFS(s)之前返回，而有两种情形满足该条件：
- DFS(v) START -> DFS(v) END -> DFS(s) START -> DFS(s)
 END
- DFS(s) START -> DFS(v) START -> DFS(v) END -> DFS(s)
 END

是因为而根据目前的已知条件，GR中存在一条s到v的路径，即意味着G中存在一条v到s的路径，而在第一种情形下，调用DFS(v)却没能在它返回前递归调用DFS(s)，这是和G中存在v到s的路径相矛盾的，因此不可取。故情形二为唯一符合逻辑的调用过程。而根据DFS(s)
 START ->DFS(v) START可以推导出从s到v存在一条路径。

所以从s到v以及v到s都有路径可达，证明完毕。


复杂度分析：

根据上面总结的Kosaraju算法关键步骤，不难得出，该算法需要对图进行两次DFS，以及一次图的转置。所以复杂度为O(V+E)。

求出来的**每个强连通分量收缩成一个点，并且用求出每个强连通分量的顺序来标记收缩后的节点，那么这个顺序其 实就是强连通分量收缩成点后形成的有向无环图的拓扑序列**。为什么呢？首先，应该明确搜索后的图一定是有向无环图呢？废话，如果还有环，那么环上的顶点对应
 的所有原来图上的顶点构成一个强连通分量，而不是构成环上那么多点对应的独自的强连通分量了。然后就是为什么是拓扑序列，我们在改进分析的时候，不是先选 的树不会连通到其他树上（对于反图GT来说），也就是后选的树没有连通到先选的树，也即先出现的强连通分量收缩的点只能指向后出现的强连通分量收缩的点。那么拓扑序列不是理所当然的吗？这就是Kosaraju算法的一个隐藏性质。







