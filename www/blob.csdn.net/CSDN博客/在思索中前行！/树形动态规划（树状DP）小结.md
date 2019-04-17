# 树形动态规划（树状DP）小结 - 在思索中前行！ - CSDN博客





2015年04月29日 23:10:03[_Tham](https://me.csdn.net/txl16211)阅读数：16317标签：[树形DP																[树形动态规划																[树形DP小结																[树形DP典型习题训练](https://so.csdn.net/so/search/s.do?q=树形DP典型习题训练&t=blog)
个人分类：[NOIP 动态规划																[数据结构](https://blog.csdn.net/txl16211/article/category/2409063)](https://blog.csdn.net/txl16211/article/category/2239693)





## 树状动态规划定义

之所以这样命名树规，是因为树形DP的这一特殊性：没有环，dfs是不会重复，而且具有明显而又严格的层数关系。利用这一特性，我们可以很清晰地根据题目写出一个在树（型结构）上的记忆化搜索的程序。而深搜的特点，就是“不撞南墙不回头”。这一点在之后的文章中会详细的介绍。
       首先是扫盲，介绍几条名词的专业解释以显示我的高端（大部分人可以略过，因为学习到树规的人一下应该都懂……）：

### **动态规划： **
　问题可以分解成若干相互联系的阶段，在每一个阶段都要做出决策，全部过程的决策是一个决策序列。要使整个活动的总体效果达到最优的问题，称为多阶段决策问题。动态规划就是解决多阶段决策最优化问题的一种思想方法。

阶段：

　　将所给问题的过程，按时间或空间（树归中是空间，即层数）特征分解成若干相互联系的阶段，以便按次序去求每阶段的解。

状态：

　　各阶段开始时的客观条件叫做状态。

决策：

　　当各段的状态取定以后，就可以做出不同的决定，从而确定下一阶段的状态，这种决定称为决策。 （即孩子节点和父亲节点的关系）

策略：

　　由开始到终点的全过程中，由每段决策组成的决策序列称为全过程策略，简称策略。

状态转移方程：

　　前一阶段的终点就是后一阶段的起点，前一阶段的决策选择导出了后一阶段的状态，这种关系描述了由k阶段到k+1阶段（在树中是孩子节点和父亲节点）状态的演变规律，称为状态转移方程。

目标函数与最优化概念：

　　目标函数是衡量多阶段决策过程优劣的准则。最优化概念是在一定条件下找到一个途径，经过按题目具体性质所确定的运算以后，使全过程的总效益达到最优。
树的特点与性质：

1、 有n个点，n-1条边的无向图，任意两顶点间可达

2、 无向图中任意两个点间有且只有一条路

3、 一个点至多有一个前趋，但可以有多个后继

4、 无向图中没有环；

废话说完了，下面是正文：

**拿到一道树规题，我们有以下3个步骤需要执行：**

判断是否是一道树规题：

即判断数据结构是否是一棵树，然后是否符合动态规划的要求。如果是，那么执行以下步骤，如果不是，那么换台。

建树：通过数据量和题目要求，选择合适的树的存储方式。

如果节点数小于5000，那么我们可以用邻接矩阵存储，如果更大可以用邻接表来存储(注意边要开到2*n，因为是双向的。这是血与泪的教训)。如果是二叉树或者是需要多叉转二叉，那么我们可以用两个一维数组brother[]，child[]来存储（这一点下面会仔细数的）。
写出树规方程：通过观察孩子和父亲之间的关系建立方程。我们通常认为，树形DP的写法有两种：

a.根到叶子: 不过这种动态规划在实际的问题中运用的不多。本文只有最后一题提到。

b.叶子到根: 既根的子节点传递有用的信息给根，完后根得出最优解的过程。这类的习题比较的多。

注意：这两种写法一般情况下是不能相互转化的。但是有时可以同时使用具体往后看。


以下即将分析的题目的目录及题目特点：

1、加分二叉树：区间动规+树的遍历；

2、二叉苹果树：二叉树上的动规；

3、最大利润：多叉树上的动规；

4、选课：多叉树转二叉；

5、选课（输出方案）：多叉转二叉+记录路径；

6、软件安装：判断环+缩点+多叉转二叉；

【4、5、6属于依赖问题的变形】
基本的知识掌握和步骤了，我们就通过习题来感受一下树规的魅力，先来看这样一道题：

### 1、加分二叉树
【问题描述】

设一个n个节点的二叉树tree的中序遍历为（l,2,3,…,n），其中数字1,2,3,…,n为节点编号。每个节点都有一个分数（均为正整数），记第i个节点的分数为di，tree及它的每个子树都有一个加分，任一棵子树subtree（也包含tree本身）的加分计算方法如下：

subtree的左子树的加分× subtree的右子树的加分＋subtree的根的分数

若某个子树为空，规定其加分为1，叶子的加分就是叶节点本身的分数。不考虑它的空子树。

试求一棵符合中序遍历为（1,2,3,…,n）且加分最高的二叉树tree。要求输出；

（1）tree的最高加分

（2）tree的前序遍历

【输入格式】

第1行：一个整数n（n＜30），为节点个数。

第2行：n个用空格隔开的整数，为每个节点的分数（分数＜100）。

【输出格式】
第1行：一个整数，为最高加分（结果不会超过4,000,000,000）。

第2行：n个用空格隔开的整数，为该树的前序遍历。

【算法&思路】：

看到这个问题，我们首先应该想到的是这道题是否属于动态规划，而这里我们发现，结合问题，如果整棵树的权值最大，必然有左子树的权值最大，右子树的权值也最大，符合最优性原理。所以是动态规划。

而却不是一道树规的题目。因为我们可以用区间动规的模型解决掉：直接定义一个f[i][j]表示从i到j的最大值，则f[i][j]=max(f[i][k-1]*f[k+1][j]+a[k]),枚举k即可。接下来是如何建树的问题，只有把树建好了，才能输出其前序遍历。于是，我们看到了两个关键词：二叉树，中序遍历。有了这两个关键词，加上区间动规，这棵树就能建起来了。根据二叉树的特性来建树（这里不再具体讨论树的详细的构造了，中序遍历和前序遍历不懂得自己百度）。所以这颗树的前序遍历，只需要边动规边记录下root[i][j]=k表示i到j的根为k即可确定树的构造。



```cpp
#include<iostream>
#include<iomanip>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<memory>
#include<algorithm>
#include<string>
#include<climits>
#include<queue>
#include<vector>
#include<cstdlib>
#include<map>
using namespace std;

const int ee=50,e=-999999999;
int n;
int a[ee]={0},f[ee][ee],root[ee][ee]={0};//f(i,j)中序遍历为i,i+1,…,j的二叉树的最大加分

//**若根节点的下表是k，则左端点的是k-1，右端点是k+1；
void front(int x,int y)
{
    if(root[x][y]!=0)
        cout<<root[x][y]<<' ';
    if(root[x][root[x][y]-1]!=0)    front(x,root[x][y]-1);
    if(root[root[x][y]+1][y]!=0)    front(root[x][y]+1,y);
}

int main()
{
    //memset 赋初值不能为1 memset(f,1,sizeof(f));
    cin>>n;

    for(int i=0;i<=n;i++)
    {
        for(int j=0;j<=n;j++)
            f[i][j]=1;
    }
    for(int i=1;i<=n;i++)
    {
        cin>>a[i];
        f[i][i]=a[i];
        root[i][i]=i;
    }
    //区间长度
    for(int len=1;len<=n;len++)
    {
        //区间起点
        for(int i=1;i<=n;i++)
        {
            //终点
            int j=i+len;
            if(j<=n)
            {
                int temp=e;
                //因为是中序排列
                for(int k=i;k<=j;k++)
                {
                    if(temp < (f[i][k-1]*f[k+1][j]+a[k]))
                    {
                        temp=f[i][k-1]*f[k+1][j]+a[k];
                        root[i][j]=k;
                    }
                }
                f[i][j]=temp;
            }
        }
    }
    cout<<f[1][n];

    //前序遍历
    cout<<endl;
    front(1,n);

return 0;
}
```
【小结】：拿到一道题目，首先我们要做的是看清题目，判断这是一道考察什么算法的题目。只有建立在正确思路基础下的算法，才是有意义的，正确的算法，也是事半功倍的算法。而此题是批着 树形 外观的 非树形动态规划题。而真正的树形动态规划是在树上做动态规划。

### **2、二叉苹果树**
【题目描述】：

有一棵苹果树，如果树枝有分叉，一定是分2叉（就是说没有只有1个儿子的结点）这棵树共有N个结点（叶子点或者树枝分叉点），编号为1-N,树根编号一定是1。我们用一根树枝两端连接的结点的编号来描述一根树枝的位置。现在这颗树枝条太多了，需要剪枝。但是一些树枝上长有苹果。

给定需要保留的树枝数量，求出最多能留住多少苹果。

【输入格式】：

第1行2个数，N和Q(1<=Q<= N,1<N<=100)。

N表示树的结点数，Q表示要保留的树枝数量。接下来N-1行描述树枝的信息。

每行3个整数，前两个是它连接的结点的编号。第3个数是这根树枝上苹果的数量。

每根树枝上的苹果不超过30000个。

【输出格式】：

剩余苹果的最大数量。

input

5 2

1 3 1

1 4 10

2 3 20

3 5 20

output

21

【算法&思路】：首先，可以肯定的是，这是一道有关树规的题目，父节点和子节点存在着相互关联的阶段关系。

第一步完成。


再执行第二步：我们观察到题目数据量不大,所以有两种选择：邻接矩阵和邻接表。因为邻接矩阵的代码简单，思路清晰，所以建议能写邻接矩阵的时候就不要写邻接表了。我们设ma[x][y]为边的值，因为树是双向的，所以要再记录ma[y][x]。

　　设tree[v,1]为节点v的左子树，tree[v,2]为节点v的右子树,然后我们再递归建树（因为树是递归定义的，所以很多时候建树都要考虑递归）。

　　建树的问题解决的了，我们就要列状态转移方程了。根据求什么设什么的原则，我们定义f[i][j]表示以i为节点的根保留k条边的最大值，那么f[v][k]=max(f[v][k],(f[tree[v][1]][i]+f[tree[v][2]][k-i-1]+num[v])),我们枚举i就可以了。正如我开头提到的。因为树是递归定义的所以我们可以用记忆化搜索的形式（dfs）来具体实现。而树本身严格分层，而且没有环。所以是不会重复的。

F[1][Q+1]就是答案。因为题目中给的是边的权值，而我们在处理时将每条边的权值全赋给其所连的父节点和子节点中的子节点（将关于边的问题转化为关于点的问题），所以最后是Q+1，表示点的数目。




```cpp
#include<iostream>
#include<iomanip>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<memory>
#include<algorithm>
#include<string>
#include<climits>
#include<queue>
#include<vector>
#include<cstdlib>
#include<map>
using namespace std;

const int ee=105;
int n,q;
int tree[ee][5]={0},ma[ee][ee]={0},num[ee]={0},f[ee][ee]={0};

void preproccess()
{
    for(int i=0;i<=n;i++)
        for(int j=0;j<=n;j++)
        {
            ma[i][j]=-1;
            ma[j][i]=-1;
        }
}

void maketree(int v);

void build(int x,int y,int lor)//lor means left or right
{
    num[y]=ma[x][y];
    tree[x][lor]=y;
    ma[x][y]=-1;ma[y][x]=-1;
    maketree(y);
}

void maketree(int v)
{
    int lr=0;
    for(int i=0;i<=n;i++)
        if(ma[v][i]>=0)//如果分叉了，那么记录
        {
            lr++;      //1 or 2 表示左支还是右支；
            build(v,i,lr);//存入并递归
            if(lr==2)    return;
        }
}

void dfs(int v,int k)
{
    if(k==0)    f[v][k]=0;
    else if(tree[v][1]==0 && tree[v][2]==0) f[v][k]=num[v];
    else
    {
        f[v][k]=0;
        for(int i=0;i<k;i++)
        {
            if(f[tree[v][1]][i]==0)    dfs(tree[v][1],i);
            if(f[tree[v][2]][k-i-1]==0)    dfs(tree[v][2],k-i-1);
            f[v][k]=max(f[v][k],(f[tree[v][1]][i]+f[tree[v][2]][k-i-1]+num[v]));
        }
    }
}

int main()
{
    cin>>n>>q;
    preproccess();

    for(int i=0;i<n;i++)
    {
        int x,y,xy;
        scanf("%d%d%d",&x,&y,&xy);
        ma[x][y]=xy;
        ma[y][x]=xy;
    }

    //建树；
    maketree(1);

    dfs(1,q+1);

    cout<<f[1][q+1];

return 0;
}
```
【小结】：在树的存储结构上，我们一般选的都是二叉树，因为二叉树可以用静态数组来存储，并且状态转移也很好写（根节点只和左子节点和右子节点有关系）。
可如果是多叉怎么办？  往下看。

### 3、最大利润
【题目描述】

政府邀请了你在火车站开饭店，但不允许同时在两个相连接的火车站开。任意两个火车站有且只有一条路径，每个火车站最多有50个和它相连接的火车站。

告诉你每个火车站的利润，问你可以获得的最大利润为多少。

最佳投资方案是在1，2，5，6这4个火车站开饭店可以获得利润为90

【输入格式】

第一行输入整数N(<=100000)，表示有N个火车站，分别用1，2。。。，N来编号。接下来N行，每行一个整数表示每个站点的利润，接下来N-1行描述火车站网络，每行两个整数，表示相连接的两个站点。

【输出格式】

输出一个整数表示可以获得的最大利润。

【样例输入】

6 10

20

25

40

30

30

4 5

1 3

3 4

2 3

6 4

【样例输出】

90

【算法&思路】：

按照上一题的步骤，我们再来分析一遍：一、是否是动态规划。这时可能很多人已经吐槽了：闭着眼都知道是动态规划，不然你粘出来干什么？？呵呵，没错，确实是。但是为什么是呢？？首先，这是棵树，是一棵多叉树。其次，当我们尝试着把他向动态规划上靠时，我们发现当前节点只与其孩子节点的孩子节点（这里没打错，因为隔一个火车站）有关系。所以综上所述，是动规，还是一个树规，一个不折不扣的树规！

接下来，第二步建树。看范围和题目发现，这是一个有着n（<100000）的多叉树，所以只能用邻接表存储了。没有根，我们一般通常指定1为根。

第三步：F[i]表示i这条根要，G[i]表示不要（也可以用f[i][1,0]来表示）。然后以此枚举i的孩子：如果i要了那么i的孩子就不能要，如果i不要i的孩子就可要可不要（取最大值）即可。最后输出max（f[1],g[1]）;





```cpp
#include<iostream>
#include<iomanip>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<memory>
#include<algorithm>
#include<string>
#include<climits>
#include<queue>
#include<vector>
#include<cstdlib>
#include<map>
using namespace std;

const int e=100020;
int f[e]={0},g[e]={0},a[e]={0},link[e]={0};//f[i]表示这条根要，g【i】表示不要；
int n,t=0;

struct qq
{
    int y,next;
}ee[2*e];

void insert(int startt,int endd)    //临界表存储树
{
    ee[++t].y=endd; ee[t].next=link[startt];
    link[startt]=t;
}

void dfs(int root,int father)
{
    int tempf=0,tempg=0;
    for(int i=link[root];i;i=ee[i].next)
    {
        if(ee[i].y!=father)
        {
            dfs(ee[i].y,root);
            f[root]+=g[ee[i].y];
            g[root]+=max(f[ee[i].y],g[ee[i].y]);
        }
    }
    f[root]+=a[root];
}

int main()
{
    cin>>n;
    for(int i=1;i<=n;i++)
        scanf("%d",&a[i]);

    for(int i=1;i<n;i++)
    {
        int xx,yy;
        scanf("%d%d",&xx,&yy);
        insert(xx,yy);
        insert(yy,xx);
    }

    dfs(1,1);

    cout<<max(f[1],g[1]);

return 0;
}
```
【小结】：无论是多叉树还是二叉树，只要我们把树以正确的形式建立起来，那么我们再根据建树的形式和题目要求，找出孩子和父亲之间的关系，那么状态转移方程很容易就求解出来了。多叉其实也不是很难。对么？呵呵，那么再看下面一道题：

### 4、选课
【题目描述】

学校实行学分制。每门的必修课都有固定的学分，同时还必须获得相应的选修课程学分。学校开设了N（N<300）门的选修课程，每个学生可选课程的数量M是给定的。学生选修了这M门课并考核通过就能获得相应的学分。

在选修课程中，有些课程可以直接选修，有些课程需要一定的基础知识，必须在选了其它的一些课程的基础上才能选修。例如《Frontpage》必须在选修了《Windows操作基础》之后才能选修。我们称《Windows操作基础》是《Frontpage》的先修课。每门课的直接先修课最多只有一门。两门课也可能存在相同的先修课。每门课都有一个课号，依次为1，2，3，…。

你的任务是为自己确定一个选课方案，使得你能得到的学分最多，并且必须满足先修课优先的原则。假定课程之间不存在时间上的冲突。

【输入格式 】Input Format

输入文件的第一行包括两个整数N、M（中间用一个空格隔开），其中1≤N≤300,1≤M≤N。

以下N行每行代表一门课。课号依次为1，2，…，N。每行有两个数（用一个空格隔开），第一个数为这门课先修课的课号（若不存在先修课则该项为0），第二个数为这门课的学分。学分是不超过10的正整数。

【输出格式】 Output Format

只有一个数：实际所选课程的学分总数。

【算法&思路】：

继续照着三步的方法判断：一，题目大致一看，有点像有依赖的背包问题，于是你扭头就走，关掉了我的《树规》，打开了崔神犇的《背包九讲》。然后你哭了，因为有依赖的背包问题只限定于一个物品只依赖于一个物品，而没有间接的依赖关系。有依赖的背包问题的模型，根本解决不了。崔神告诉你，这属于树规的问题，不属于他背包的范围了。好了，回过来，我们接着分析。发现这是一棵树，还是一棵多叉树，嗯，很好，确定是树规了。

然后第二步，建树，一看数据范围邻接矩阵；

第三步动规方程：f[i][j]表示以i为节点的根的选j门课的最大值，然后有两种情况： i不修，则i的孩子一定不修，所以为0；i修，则i的孩子们可修可不修（在这里其实可以将其转化为将j-1个对i的孩子们进行资源分配的问题，也属于背包问题）；答案是f[1][m]。问题圆满解决，一气呵成。

但……

身为追求完美的苦*程序猿的我们，不可以将它更简单一点呢？

多叉转二叉。

因为之前我们说过“在树的存储结构上，我们一般选的都是二叉树，因为二叉树可以用静态数组来存储，并且状态转移也很好写（根节点只和左子节点和右子节点有关系）。”所以转换成二叉树无疑是一种不错的选择。

我们开两个一维数组，b[i](brother)&c[i](child)分别表示节点i的孩子和兄弟，以左孩子和右兄弟的二叉树的形式存储这样，根节点之和两个节点有关系了，状态转移的关系少了，代码自然也就好写了。

我们依旧f[i][j]表示以i为节点的根的选j门课的最大值，那么两种情况：1.根节点不选修则f[i][j]=f[b[i]][j];2.根节点选修f[i][j]=f[c[i]][k]+f[b[i]][j-k-1]+a[i]（k表示左孩子学了k种课程）;取二者的最大值即可。





```cpp
#include<iostream>
#include<iomanip>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<memory>
#include<algorithm>
#include<string>
#include<climits>
#include<queue>
#include<vector>
#include<cstdlib>
#include<map>
using namespace std;

const int e=320;
int n,m;
int c[e]={0},b[e]={0},s[e]={0},f[e][e]={0};//c[]:means child ; b[]:means brother

void maketree()//多叉转二叉
{
    cin>>n>>m;
    for(int i=1;i<=n;i++)
    {
        int ta,tb;
        scanf("%d%d",&ta,&tb);
        s[i]=tb;
        if(ta==0) ta=n+1;
        b[i]=c[ta];
        c[ta]=i;
    }
}

void dfs(int x,int y)
{
    if(f[x][y]>=0)    return;
    if(x==0 || y==0) {  f[x][y]=0;return;}
    dfs(b[x],y);
    //max()f[b[x]][y];
    for(int k=0;k<y;k++)
    {
        dfs(b[x],k); //不取根节点
        dfs(c[x],y-k-1);//取根节点
        f[x][y]=max(f[x][y] , max(f[b[x]][y] , f[b[x]][k]+f[c[x]][y-k-1]+s[x]));
    }
    //cout<<x<<' '<<y<<' '<<f[x][y]<<endl;
    return;
}

int main()
{

    memset(f,-1,sizeof(f));

    maketree();
    dfs(c[n+1],m);

    cout<<f[c[n+1]][m]<<endl;

return 0;
}
```
【小结】：当题目中的数据结构是多叉树的时候，我们有两种选择：直接在多叉树上动规，或者转化为二叉树后动规。毫无疑问，二叉树上的动规是简洁的。但是，并不是说所有的多叉树都需要转化，一般情况下，当根节点与孩子节点有着必然的关系时才会转化。这需要我们多做题目，增加对树规的感觉才能游刃有余。

### 5、选课（输出方案）
【题目描述】同上。

【输入格式】同上。

【输出格式】 Output Format

第一行只有一个数，即实际所选课程的学分总数。以下N行每行有一个数，表示学生所选课程的课号。n行学生选课的课号按从小到大的顺序输出。

【算法&思路】：拿到这道题目，首先我们必然要和上一道题目做一下对比。对比后我们发现，这道题目和上第一道题目完全一样，除了问题比上一题多一问：输出方案。所以，我们可以把这道题目分成两部分：求总数和输出方案。而求总数的问题我们在上一题中已经很好的解决了，所以这道题目重点是考察的是树的路径记录的问题。

既然数是递归定义的，所以我们依旧使用递归的形式来记录路径：使用一个bool数组ans来进行递归，


分两种情况：取(1)和不取(0)。

然后，我们继续利用已经求得的f[i][j]的值来思考如何找到路径：

首先定义一个path()函数。如果f[i][j]=f[b[i]][j],那么节点i必然没有取，让ans[i]=0;否则，节点i一定取到了。（为什么呢？其实，这是依照第一问的dfs来思考的，第一问的dfs是这样定义的，所以我们就这样考虑了。）然后依照上一问，if(f[x][y]==f[b[x]][k-1]+f[c[x]][y-k]+s[x])，那么我们在i节点后选的一定是以上的方案，在这时让ans[i]=1,继续深搜path()即可。最后从1到n依次输出取到的点即可。


```cpp
#include<iostream>
#include<iomanip>
#include<cmath>
#include<algorithm>
#include<cstring>
#include<string>
#include<cstdio>
#include<cstdlib>
#include<map>
#include<vector>
#include<queue>
#include<memory>
#include<climits>
using namespace std;

const int e=520;
int c[e]={0},b[e]={0},f[e][e]={0},s[e]={0};
bool ans[e]={0};
int n,m;

void maketree()
{
    cin>>n>>m;
    for(int i=1;i<=n;i++)
    {
        int ta,tb;
        scanf("%d%d",&ta,&tb);
        s[i]=tb;
        b[i]=c[ta];
        c[ta]=i;
    }
}

void dfs(int x,int y)//以x为节点，取y个
{
    if(f[x][y]>=0)    return;
    if(x==0 || y==0)
    {
        f[x][y]=0;
        return;
    }
    else    dfs(b[x],y);
    int tm=f[b[x]][y],tp=0;

    for(int k=1;k<=y;k++)
    {
        dfs(b[x],k-1);
        dfs(c[x],y-k);
        tp=max(tp,f[b[x]][k-1]+f[c[x]][y-k]+s[x]);
    }

    if(tp>tm)    f[x][y]=tp;
    else    f[x][y]=tm;

    return;
}

void path(int x,int y)
{
    if(x==0 || y==0)
        return;
    if(f[x][y]==f[b[x]][y])    path(b[x],y);
    else
    {
        for(int k=1;k<=y;k++)
        {
            if(f[x][y]==f[b[x]][k-1]+f[c[x]][y-k]+s[x])
            {
                path(b[x],k-1);
                path(c[x],y-k);
                ans[x]=1;
                return;
            }
        }
    }
}

int main()
{
    memset(f,-1,sizeof(f));

    maketree();
    dfs(c[0],m);

    cout<<f[c[0]][m]<<endl;
    path(c[0],m);

    for(int i=1;i<=n;i++)
        if(ans[i])    cout<<i<<endl;

    return 0;
}
```
【小结】：路径输出的问题，在不同的题目中有不同的解法，比如说边求值边记录等等，而在树规中，利用动规和树的特点，求解完后再原路返回找一遍，是一种比较容易想到且容易操作的性价比比较高的算法。


### 6、软件安装
【题目描述】：

现在我们的手头有N个软件，对于一个软件i，它要占用Wi的磁盘空间，它的价值为Vi。我们希望从中选择一些软件安装到一台磁盘容量为M的计算机上，使得这些软件的价值尽可能大（即Vi的和最大）。

但是现在有个问题：软件之间存在依赖关系，即软件i只有在安装了软件j（包括软件j的直接或间接依赖）的情况下才能正确工作（软件i依赖软件j）。幸运的是，一个软件最多依赖另外一个软件。如果一个软件不能正常工作，那么他能够发挥的作用为0。

我们现在知道了软件之间的依赖关系：软件i依赖Di。现在请你设计出一种方案，安装价值尽量大的软件。一个软件只能被安装一次，如果一个软件没有依赖则Di=0，这是只要这个软件安装了，它就能正常工作。

【输入格式】：

第1行：N,M （0<=N<=100,0<=M<=500）

第2行：W1,W2, … Wi, … ,Wn

第3行：V1,V2, … Vi, … ,Vn

第4行：D1,D2, … Di, … ,Dn

【输出格式】：

一个整数，代表最大价值。

【样例】

3 10

5 5 6

2 3 4

0 1 1


输出：

5

【算法&思路】：同样，这道题目类似与第4题，是一个依赖的问题，毫无疑问是一道动态规划，但是它确实是树规么？我们来想这样一组数据，1依赖2，2依赖3,3依赖1。这样符合题目要求，但有形成了环，所以不是一棵树了。但是根据题目，这样特殊的情况，要么全要，要么全就不要。所以，事实上我们可以将这个环看成一个点再来动规，即缩点。如何判断是否是一个环呢，依照数据范围，我们想到了floyed（弗洛里德），这是在这种数据范围内性价比最高的方式。最后树规。于是一个比较清晰的步骤就出来了：判环，缩点，树规。

接下来是细节：首先存树，毫无疑问，是邻接矩阵。

做floyed。如果两点之间mapp[i][j]中有另一条路径相连，即mapp[i][k]=1 && mapp[k][j]=1(1表示两点是通的);那么mapp[i][j]也是通的且是环。

缩点。这个是最麻烦的，麻烦在于我们要把缩的点当成一个新点来判断，而且要判断某个点是否在某个环里。我们用染色法来判断，用所占的空间w控制颜色的对应，有以下三种情况：

1、点i所在的环之前没有判断过，是新环。那么，我们将这个新环放到数组最后，即新加一个点，然后让这两个点的空间标记为负值tmpw，且tmpw+tmpn（新点的下标）等于原来的点数，这样，我们就可以通过某个点的空间迅速找到他所在的新点。像钥匙一样一一对应；

2、点i所在的环之前已经判断过了，是旧环（已合成新点），且i是环的一部分。那么我们就把i也加到这个新点里面，即体积，价值相加即可；

3、点j所在的环是旧环，但是i不是环的一部分（例如1依赖2，2依赖3,3依赖1。4也依赖1，那么，4所在的是个环，但4不属于环的一部分）。那么，把j的父亲转到新点上d[j]= n-w[d[j]]。

以上缩点的工作做完之后，剩下的就是一棵树。就可以在这上面动规了：先将其转换成一棵左孩子右兄弟的二叉树，之后记忆化。i的孩子不取f[b[x]][k]=dfs(b[x],k);还是取：

f[c[x]][y-i]=dfs(c[x],y-i); 

f[b[x]][i]=dfs(b[x],i);                                                                                                                                         

f[x][k]=max(f[x][k],v[x]+f[c[x]][y-i]+f[b[x]][i]);

最后答案是f[c[0]][m]。


```cpp
#include<iostream>
#include<iomanip>
#include<cstring>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<memory>
#include<climits>
#include<vector>
#include<map>
#include<queue>
#include<algorithm>
using namespace std;

const int e=505;
int n,m,tmpw=0,tmpn;
int w[e]={0},v[e]={0},b[e]={0},c[e]={0},f[e][5*e]={0},d[e]={0};
bool mapp[e][e]={0};

void floride()
{
    for(int i=1;i<=n;i++)//弗洛里德判断是否有环；
        for(int j=1;j<=n;j++)
            for(int k=1;k<=n;k++)
                if(mapp[k][i]==1 && mapp[i][j]==1)
                    mapp[k][j]=1;
}


void merge()//合点
{
    tmpn=n;
    for(int i=1;i<=tmpn;i++)
        for(int j=1;j<=tmpn;j++)
        {
            if(mapp[i][j]==1 && mapp[j][i]==1 && i!=j && w[i]>0 && w[j]>0)//如果是新环；
            {
                tmpn++;
                v[tmpn]=v[i]+v[j];
                w[tmpn]=w[i]+w[j];
                tmpw--;    w[i]=tmpw;    w[j]=tmpw;    //tmpw+tmpn永远等于最开始的n
            }

            //如果j依赖的点被合并(是旧环),且j在环里
            if(w[d[j]]<0 && w[j]>0 && mapp[j][d[j]]==1 && mapp[j][d[j]]==1)
            {
                w[n-w[d[j]]]+=w[j];
                v[n-w[d[j]]]+=v[j];
                w[j]=w[d[j]];
            }

            //如果j依赖的点在环里，但是j不在环里
            if(w[d[j]]<0 && w[j]>0)
                if((mapp[j][d[j]]==1 && mapp[d[j]][j]==0) || (mapp[j][d[j]]==0 && mapp[d[j]][j]==1))
                    d[j]=n-w[d[j]];
        }
}

int  dfs(int x,int k)
{
    if(f[x][k]>0)    return(f[x][k]);
    if(x==0 || k<=0)    return(0);
    //不取x
    f[b[x]][k]=dfs(b[x],k);
    f[x][k]=f[b[x]][k];
        int y=k-w[x];
        for(int i=0;i<=y;i++)
        {
            f[c[x]][y-i]=dfs(c[x],y-i);
            f[b[x]][i]=dfs(b[x],i);
            f[x][k]=max(f[x][k],v[x]+f[c[x]][y-i]+f[b[x]][i]);
        }
    return(f[x][k]);
}



int main()
{
    cin>>n>>m;
    for(int i=1;i<=n;i++)
        scanf("%d",&w[i]);
    for(int i=1;i<=n;i++)
        scanf("%d",&v[i]);
    for(int i=1;i<=n;i++)
    {
        int a;
        scanf("%d",&a);
        d[i]=a;
        mapp[a][i]=1;
    }

    floride();
    merge();

    //多叉转二叉
    for(int i=1;i<=tmpn;i++)
        if(w[i]>0)
        {
            b[i]=c[d[i]];
            c[d[i]]=i;
        }
    cout<<dfs(c[0],m);

    return 0;
}
```
【小结】：依赖问题的变化很多，比如基本树规，记录路径，有环等等。但是他们都有一些共同的特点，比如说记忆化的方程差不多。根据题目，我们应该会判断给的数据是否是图，是树，能想出特殊情况。树毕竟是图的一种特殊形式，而二叉树又是树的一种特殊形式。如果能将一个问题由复杂向简单转换，那么我们不仅思路会清晰很多，代码量也会少很多。下面一道题目，就是我们所说的很少见的根节点向叶子节点动规的问题。


【总结】：树规是动态规划的一种，它将树和动态规划很巧妙地结合在了一起。做树规题目，不仅仅锻炼了我们的代码能力，而且加深了我们对动态规划的理解。再次强调，树的递归定义使树规多以记忆化的形式来写，而由于树的严格分层，使动规的阶段自然就清晰了起来，多找一找父节点与子节点的关系，就是很可能是两个阶段之间的联系。

## 树形动态规划练习《蓝桥杯 结点选择》



问题描述


有一棵 n 个节点的树，树上每个节点都有一个正整数权值。如果一个点被选择了，那么在树上和它相邻的点都不能被选择。求选出的点的权值和最大是多少？


输入格式


第一行包含一个整数 n 。


接下来的一行包含 n 个正整数，第 i 个正整数代表点 i 的权值。


接下来一共 n-1 行，每行描述树上的一条边。


输出格式


输出一个整数，代表选出的点的权值和的最大值。

样例输入


5

1 2 3 4 5

1 2

1 3

2 4

2 5

样例输出


12

样例说明


选择3、4、5号点，权值和为 3+4+5 = 12 。


数据规模与约定


对于20%的数据， n <= 20。


对于50%的数据， n <= 1000。


对于100%的数据， n <= 100000。


权值均为不超过1000的正整数。

解题过程


　　刚学习完树形动态规划的原理，所以乍一看就知道此题应该用树形动态规划解决。分两步：1、建树。2、动态规划。


　　刚开始选择的存储结构是二维数组，既每一行表示树的一层，每一列表示该层（行）的所有节点；记录下树的最大层数，从最后一层开始改变每个节点的状态，最后从根节点中获取最优解。

```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define M 100010 //数组最大长度

int fu[M],hz[M][M],shu[M][M],pow[M],f[M][2];
//父节点数组; 孩子数组hz[i][0]第i个节点的孩子数,hz[i][j](j>0)表示i节点的第j个孩子
//树二维数组,shu[i][0]表示第i层节点数,shu[i][j](j>0)表示第i层的第j个节点；
//pow[]权值数组,p[i]表示第i个节点的权值
//f[i][1]保留节点i时最大权值，f[i][0]不保留节点i时的最大权值

int main()
{
    int n,i,j,u,v;
	memset(fu,0,sizeof(fu));
    memset(hz,0,sizeof(hz));
    memset(shu,0,sizeof(shu));
    memset(f,0,sizeof(f));
    scanf("%d",&n);
    for(i=1;i<=n;i++)scanf("%d",&pow[i]);
    for(i=1;i<n;i++)
    {
        scanf("%d%d",&u,&v);
        fu[v]=u;
        hz[u][0]++;
        hz[u][hz[u][0]]=v;
    }
    //建树
    int x,maxlev=-1,s;
    for(i=1;i<=n;i++)
    {
        x=fu[i];
        s=1;
        while(x!=0){  s++;x=fu[x];  }
        shu[s][0]++;
        shu[s][shu[s][0]]=i;
        if(s>maxlev)maxlev=s;
    }
	//动态规划
    int now,k,a,b;
    for(i=maxlev;i>0;i--)
    {
        for(j=1;j<=shu[i][0];j++)
        {
            now=shu[i][j];
            if(hz[now][0]==0)
            {
                f[now][0]=0;
                f[now][1]=pow[now];
            }
        	else
            {
                for(k=1;k<=hz[now][0];k++)
                {
                    a=f[hz[now][k]][0];
                    b=f[hz[now][k]][1];
                    f[now][1]+=a;
                    if(b>a)a=b;
                    f[now][0]+=a;
                }

        	}
        }
    }
    int sum=0;
    for(i=1;i<=shu[1][0];i++)
    {
        now=shu[1][i];
        a=f[now][0];b=f[now][1];
        if(b>a)a=b;
            sum+=a;
    }

	printf("%d\n",sum);
    return 0;
}
```
按理说这个算法是可行的，但是再提交答案时，居然发生运行错误，我看了看内存使用率非常大，返回题目看了数据规模，节点数n<=100000,也就意味着要用二维数组存储树的话，二维数组至少定义为shu[100000][100000],占用了非常大的控件资源。再者，题目给n个顶点，n-1条边，也就意味着树没有孤立点，并且有且仅有一个根节点，可见每一层的节点很多时候是远少于100000的，所以应该改用动态存储结构。

树的存储结构

《1》、双亲表示法

假设以一组连续空间存储树的节点，同时在每个节点中附设一个指示器指示其双亲节点在链表中的位置，其形式说明如下：

```cpp
#define MAX_TREE_SIZE 100

typedef struct PTNode{//节点结构

	TElemType data;
    int parent;//双亲位置

}PTNode;

typedef struct{    //树结构

    PTNode nodes[MAX_TREE_SIZE];
    int r,n; //根节点位置和节点数

}PTree;
```

这种存储结构利用了每个节点（除根节点以外）只有唯一双亲的性质。PARENT(T,x)操作可以在常数时间内实现。反复调用PARENT操作，直到遇见无双亲的节点时，便找到了树的根，这个就是ROOT(x)的过程。但是，在这种表示法中，求节点的孩子时需要遍历整个结构。

《2》、孩子表示法

这里主要给出一种类似于邻接表的表示法。把每个节点的孩子节点排列起来，看成是一个线性表，且以单链表作为存储结构，则n个节点有n个孩子链表（叶子节点的孩子链表为空表）。而n个头指针又组成一个线性表，为了便于查找，可采用顺序存储结构。这种存储结构可形式地说明如下：




```cpp
#define MAX_TREE_SIZE 100

typedef struct CTNode{    //孩子节点

    int child;
    struct CTNode *next;

}*ChildPtr;

typedef struct{

    TElemType data;
    ChildPtr firstchild;    //孩子链表头指针

}CTBox;

typedef struct{

    CTBox nodes[ MAX_TREE_SIZE ];
    int n,r;    //节点数和根节点位置

}CTree;
```
与双亲表示法相反，孩子表示法便于那些涉及孩子操作的实现，却不适合用于PARENT(T，x)的操作。我们可以把双亲表示法和孩子表示法合起来，既将双亲表示和孩子链表和在一起。

《3》、孩子兄弟表示法

又称二叉树表示法，或二叉树表示法。既以二叉树表作树的存储结构。链表中节点的两个链域分别指向该节点的第一个孩子节点和下一个兄弟节点，分别命名为firstchild域和nextsibling域。存储结构形式说明如下：

```cpp
#define MAX_TREE_SIZE 100

typedef struct CSNode{

    ElemType data;
    struct CSNode *firstchild,*nextsibling;

}CSNode,*CSTree;
```


利用这种结构便于实现各种树的操作。

符合题目要求的结果采用了树存储结构中的《孩子表示法》，当然有些改进。



```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>

#define M 100100　　//最大长度

using namespace std;

//孩子节点结构
typedef struct Node
{
        int vex;
        Node* next;

}Child;

Child* head[M];//链表头数组

int f[M][2],pow[M],visit[M];

//pow[]权值数组,p[i]表示第i个节点的权值
//f[i][1]保留节点i时最大权值，f[i][0]不保留节点i时的最大权值
//visit[i]==1表示i点被访问过，visit[i]==0表示节点i未被访问过

//添加边（对称的）
void addADJ(int u,int v)
{
        Child *p,*q;
        p=(Child*)malloc(sizeof(Child));
        p->vex=v;
        p->next=head[u];
        head[u]=p;
        q=(Child*)malloc(sizeof(Child));
        q->vex=u;
        q->next=head[v];
        head[v]=q;
}

//动态规划获取结果

void GetResul(int v)
{
        visit[v]=1;
        Child *p;
        for(p=head[v];p!=NULL;p=p->next)
        {
                if(visit[p->vex]==0)
                {
                         GetResul(p->vex);
                         f[v][1] = f[v][1]+f[p->vex][0];
                         f[v][0]+=max(f[p->vex][0],f[p->vex][1]);
                }
        }
        f[v][1]+=pow[v];
}

int main()
{
        int i,j,u,v,n;

        memset(head,NULL,sizeof(head));
        memset(f,0,sizeof(f));
        memset(visit,0,sizeof(visit));

        scanf("%d",&n);
        for(i=1;i<=n;i++)
        {
                scanf("%d",&pow[i]);
        }
        for(i=1;i<n;i++)
        {
                scanf("%d%d",&u,&v);
                addADJ(u,v);
        }

        GetResul(1);//从节点1开始进行动态规划
        printf("%d\n",max(f[1][0],f[1][1]));//结果输出

        return 0;
}
```](https://so.csdn.net/so/search/s.do?q=树形DP小结&t=blog)](https://so.csdn.net/so/search/s.do?q=树形动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=树形DP&t=blog)




