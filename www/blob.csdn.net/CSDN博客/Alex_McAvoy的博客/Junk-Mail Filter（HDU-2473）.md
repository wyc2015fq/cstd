# Junk-Mail Filter（HDU-2473） - Alex_McAvoy的博客 - CSDN博客





2018年05月22日 13:51:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：88
个人分类：[HDU																[数据结构——并查集](https://blog.csdn.net/u011815404/article/category/8376617)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Recognizing junk mails is a tough task. The method used here consists of two steps:

    1) Extract the common characteristics from the incoming email.

    2) Use a filter matching the set of common characteristics extracted to determine whether the email is a spam.

We want to extract the set of common characteristics from the N sample junk emails available at the moment, and thus having a handy data-analyzing tool would be helpful. The tool should support the following kinds of operations:

    a) “M X Y”, meaning that we think that the characteristics of spam X and Y are the same. Note that the relationship defined here is transitive, so relationships (other than the one between X and Y) need to be created if they are not present at the moment.

    b) “S X”, meaning that we think spam X had been misidentified. Your tool should remove all relationships that spam X has when this command is received; after that, spam X will become an isolated node in the relationship graph.

Initially no relationships exist between any pair of the junk emails, so the number of distinct characteristics at that time is N.

Please help us keep track of any necessary information to solve our problem.

# Input

There are multiple test cases in the input file.

Each test case starts with two integers, N and M (1 ≤ N ≤ 10, 1 ≤ M ≤ 10), the number of email samples and the number of operations. M lines follow, each line is one of the two formats described above.

Two successive test cases are separated by a blank line. A case with N = 0 and M = 0 indicates the end of the input file, and should not be processed by your program.

# Output

For each test case, please print a single integer, the number of distinct common characteristics, to the console. Follow the format as indicated in the sample below.

# Sample Input

**5 6M 0 1M 1 2M 1 3S 1M 1 2S 33 1M 1 20 0 **

# Sample Output

**Case #1: 3**

**Case #2: 2**


————————————————————————————————————————————————————————

题意：已知有n个点，进行两个操作，一是合并，二是将某个点从集合中删除，求进行所有操作后集合的个数。

思路：合并好说，删除有些麻烦，由于在合并操作中所有点都会因路经压缩而链接到根节点上，所以可以将每一个点都设立一个虚拟父节点，这样根节点就是我们设立的虚拟节点，如果删除某点，那么可以修改当前节点的父节点来导致当前点的孤立，这样就完成了删点 。

# Source Program

```cpp
#include<bits/stdc++.h>
#define N 1200000
using namespace std;
int father[N];
int vis[N];
int cnt;

int Find(int x)
{
    if(x!=father[x])
        return father[x] = Find(father[x]);
    return x;
}

void Union(int x, int y)
{
    x=Find(x);
    y=Find(y);
    if(x!=y)
        father[y]=x;
}

void del(int x)
{
    father[x]=cnt;
    cnt++;
}

int main ()
{
    int n,m;
    int k=1;
    while(scanf("%d%d",&n,&m)!=EOF&&(n||m))
    {
        cnt=n+n;
        for(int i=0;i<n;i++)//添加虚拟父节点
            father[i]=i+n;
        for(int i=n;i<=n+n+m;i++)//最多可删除m个节点
            father[i]=i;

        while(m--)
        {
            int a,b,c;
            char ch[2];
            scanf("%s",&ch);
            if(ch[0]=='M')//合并
            {
                scanf("%d%d",&a,&b);
                Union(a,b);
            }
            else//删除
            {
                scanf("%d",&c);
                del(c);
            }
        }
        int sum=0;
        memset(vis,0,sizeof(vis));
        for(int i=0;i<n;i++)//枚举所有点，查看是否还在集合内
        {
            int x=Find(i);
            if(vis[x]==0)//若某点不在集合中
            {
                sum++;
                vis[x]=1;
            }
        }
        printf("Case #%d: %d\n",k++,sum);
    }
    return 0;
}
```






