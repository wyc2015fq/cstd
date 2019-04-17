# Garland（CF-767C） - Alex_McAvoy的博客 - CSDN博客





2018年10月06日 16:53:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：44
个人分类：[CodeForces																[动态规划——树形 DP](https://blog.csdn.net/u011815404/article/category/8104361)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Once at New Year Dima had a dream in which he was presented a fairy garland. A garland is a set of lamps, some pairs of which are connected by wires. Dima remembered that each two lamps in the garland were connected directly or indirectly via some wires. Furthermore, the number of wires was exactly one less than the number of lamps.

There was something unusual about the garland. Each lamp had its own brightness which depended on the temperature of the lamp. Temperatures could be positive, negative or zero. Dima has two friends, so he decided to share the garland with them. He wants to cut two different wires so that the garland breaks up into three parts. Each part of the garland should shine equally, i. e. the sums of lamps' temperatures should be equal in each of the parts. Of course, each of the parts should be non-empty, i. e. each part should contain at least one lamp.

![](https://img-blog.csdn.net/2018100616500725?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Help Dima to find a suitable way to cut the garland, or determine that this is impossible.

While examining the garland, Dima lifted it up holding by one of the lamps. Thus, each of the lamps, except the one he is holding by, is now hanging on some wire. So, you should print two lamp ids as the answer which denote that Dima should cut the wires these

 lamps are hanging on. Of course, the lamp Dima is holding the garland by can't be included in the answer.

# Input

The first line contains single integer n (3 ≤ n ≤ 106) —

 the number of lamps in the garland.

Then n lines follow. The i-th

 of them contain the information about the i-th lamp: the number lamp ai,

 it is hanging on (and 0, if is there is no such lamp), and its temperature ti ( - 100 ≤ ti ≤ 100).

 The lamps are numbered from 1 to n.

# Output

If there is no solution, print -1.

Otherwise print two integers — the indexes of the lamps which mean Dima should cut the wires they are hanging on. If there are multiple answers, print any of them.

# Examples

**Input**

6

2 4

0 5

4 2

2 1

1 1

4 2

**Output**

1 4

**Input**

6

2 4

0 6

4 2

2 1

1 1

4 2

**Output**

-1

# Note

The garland and cuts scheme for the first example:

![](https://img-blog.csdn.net/20181006165115431?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

————————————————————————————————————————————

题意：给定一颗二叉树，问从哪两个地方剪可以使余下的三部分的节点和都相等

思路：由于总和固定，因此每一颗子树的点权是固定的，设每一部分的大小为 W，从下往上更新，遇到等于 W 的子树就切，次数重置成 0，这样就可以一边遍历子树，一边算次数，需要注意一条链的情况与三的倍数但不能分成几个相等的部分的情况。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1000001
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
vector<int> son[N];
int pre[N],temp[N];
int cnt[N],num[2];
int sum,root;
int k,t;
int treeDP(int x){
    int y=son[x].size();//记录儿子数
    if(y==0){//如果儿子数为0
        cnt[x]=temp[x];
        if(temp[x]==t&&x!=root)
		{
			num[k++]=x;
			return 0;
		}
		return temp[x];
    }
    for(int i=0;i<y;i++)//统计，递归向下寻找
        cnt[x]+=treeDP(son[x][i]);

    cnt[x]+=temp[x];
    if(cnt[x]==t&&x!=root){
        num[k++]=x;
        return 0;
    }
    return cnt[x];
}
int main()
{
    int n;
    cin>>n;
    for(int i=1;i<=n;i++){
        int x,y;
        cin>>x>>y;
        son[x].push_back(i);//存储关系
        sum+=y;//记录总和
        pre[i]=x;//记录前驱
        temp[i]=y;
        if(x==0)//寻找根节点
            root=i;
    }

    if(sum%3){//如果不能整除
        cout<<-1<<endl;
        return 0;
    }

    t=sum/3;
    treeDP(root);
    if(num[0]==0||num[1]==0)//切的位置是根节点
        cout<<-1<<endl;
    else
        cout<<num[0]<<" "<<num[1]<<endl;

	return 0;
}
```






