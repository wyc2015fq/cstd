# The magic apple tree（HDU-3290） - Alex_McAvoy的博客 - CSDN博客





2018年03月19日 19:25:39[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：131








# Problem Description

    Sailormoon girls all like eating many kinds of fruit, such as banana, grape, apple and so on.

    One day, when they was walking on a orchard, they found a magic apple tree.The magic apple tree have many nodes,but there is only one root. Each notes has its label. It is labeled from 1.On the first day,only each leaf nodes(has no children nodes) have apples. Any other nodes have no apples. The number of apples that each leaf nodes have is just the label of this node.When all the immediate children of a node each has apples,this node will grow some apple on the next day. If a node has K immediate children node,the number of apple that this node grow on next day is just the number of apples that the (K + 1) / 2th smaller node has.The Xth smaller node means there are X – 1 nodes’number of apples is less than this node’s number of apple.

    Now you task is to calculate the number of apples that the root has at last.

# Input

    There are multiple test cases.<br>Each case contains a positive integer N, it means this tree has N nodes, labeled 1, 2, ... N(0 < N <= 20000).<br>The following N lines describe the children of all nodes in order of their labels. The (X + 1)th line in each test case starts with a number p (0 <= p <N), it means the Xth node has p immediate children nodes.then followed by p positive integer, means the label of immediate child node

# Output

    Print the number of apples that the root grow at last.

# Sample Input

**7    2 2 3    2 5 4    2 6 7    0    0    0    0        12    3 2 3 4    0    2 5 6    3 7 8 9    3 10 11 12    0    0    0    0    0    0    0**

# Sample Output

**4   6**

————————————————————————————————————————————

题目大意：

给出一个有N(0<N<=20000)个节点的苹果树，这个树只有1个root(根节点)，每个节点都有1个编号，label从1开始一直到N。苹果树的每个节点按照如下规则生长苹果：


- 叶子节点生长出的苹果数量等于叶子节点的编号。
- 某父亲节点有K个儿子节点，直到它的K个儿子节点都生长出苹果，父亲节点才开始生长苹果。父亲节点长出的苹果数量等于它的  所有儿子中苹果数量第(k+1)/2小的  儿子节点的苹果数量。

求：根节点生长出的苹果数量。

思路：
- 直接递归一下，先求出所有儿子的苹果树，再排序
- 内联+引用=输入外挂

# Source Program

```cpp
#include<bits/stdc++.h>
using namespace std;
#define N 20001

int root,n;
int father[N];//每一个父节点的子节点数
int vis[N];//根节点
vector<int>con[N];//记录每个父节点的子节点
char c;

int dfs(int node)
{
    int i;
    if(father[node]==0)//叶节点直接返回
        return node;
    vector<int>node_am;  //记录苹果数
    for(i=0;i<father[node];i++)//保存每个子节点苹果树
        node_am.push_back(dfs(con[node][i]));
    sort(node_am.begin(),node_am.end());
    return node_am[(father[node]+1)/2-1];//因为从0开始，所以-1
}
inline void scan(int &x)
{
    while(c=getchar(),c<'0'||c>'9');

    x=c-'0';
    while(c=getchar(),c>='0'&&c<='9')
    x=x*10+c-'0';
}
int main()
{
    int a;
    int i,j;

    while(scanf("%d",&n)!=EOF)
    {
        /*清零工作*/
        memset(vis,0,sizeof(vis));
        memset(father,0,sizeof(father));
        for(i=1;i<=n;i++)
            con[i].clear();

        for(i=1;i<=n;i++)//枚举结点
        {
            scan(father[i]);//输入所连接节点的数量

            for(j=0;j<father[i];j++)
            {
                scan(a);//输入父节点连接的节点
                con[i].push_back(a);//压入其父亲节点
                vis[a]++;//非根节点不为0
            }
        }

        for(i=1;i<=n;i++)//寻找父节点
            if(vis[i]==0)
            {
                root=i;
                break;
            }
        printf("%d\n",dfs(root));
    }
    return 0;
}
```





