# coderforces-755-C PolandBall and Forest（简单并查集） - HJ - CSDN博客
2017年01月31日 20:14:39[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：385
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
PolandBall lives in a forest with his family. There are some trees in the forest. Trees are undirected acyclic graphs with k vertices and k - 1 edges, where k is some integer. Note that one vertex is a valid tree.
There is exactly one relative living in each vertex of each tree, they have unique ids from 1 to n. For each Ball i we know the id of its most distant relative living on the same tree. If there are several such vertices, we only know the value of the one with smallest id among those.
How many trees are there in the forest? 
Input
The first line contains single integer n (1 ≤ n ≤ 104) — the number of Balls living in the forest.
The second line contains a sequence p1, p2, …, pn of length n, where (1 ≤ pi ≤ n) holds and pi denotes the most distant from Ball i relative living on the same tree. If there are several most distant relatives living on the same tree, pi is the id of one with the smallest id.
It’s guaranteed that the sequence p corresponds to some valid forest.
Hacking: To hack someone, you should provide a correct forest as a test. The sequence p will be calculated according to the forest and given to the solution you try to hack as input. Use the following format:
In the first line, output the integer n (1 ≤ n ≤ 104) — the number of Balls and the integer m (0 ≤ m < n) — the total number of edges in the forest. Then m lines should follow. The i-th of them should contain two integers ai and bi and represent an edge between vertices in which relatives ai and bi live. For example, the first sample is written as follows:
5 3 
1 2 
3 4 
4 5
Output
You should output the number of trees in the forest where PolandBall lives. 
Interaction
From the technical side, this problem is interactive. However, it should not affect you (except hacking) since there is no interaction. 
Examples 
Input
5 
2 1 5 3 3
Output
2
Input
1 
1
Output
1
Note
In the first sample testcase, possible forest is: 1-2 3-4-5.
There are 2 trees overall.
In the second sample testcase, the only possible graph is one vertex and no edges. Therefore, there is only one tree.
看数据就OK了，判断有多少个独立团，用并查集实现
```
#include<iostream>
using namespace std;
int parent[100005];
int find(int x)          //查找 
{
    return x==parent[x]?x:find(parent[x]);
}
void Union(int a,int b)    //合并 
{
    int x,y;
    x=find(parent[a]);    //寻找a的祖先 
    y=find(parent[b]);    //寻找b的祖先 
    if(x!=y)
      parent[x]=y;     //如果两个点的祖先不一样，则把b的祖先作为a的祖先的子结点 
}
int main()
{
    int n;
    while(cin>>n)
    {
        int a,sum=0;
        for(int i=1;i<=n;i++)
          parent[i]=i;           //赋初值 
        for(int i=1;i<=n;i++)
        {
            cin>>a;
            Union(i,a);           //合并操作 
        }
        for(int i=1;i<=n;i++)
        {
            if(parent[i]==i)         //遍历一次查找所有独立的树的个数 
              sum++; 
        }
        cout<<sum<<endl; 
    }
    return 0;
}
```
