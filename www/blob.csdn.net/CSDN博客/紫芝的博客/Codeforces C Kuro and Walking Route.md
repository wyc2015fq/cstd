# Codeforces C. Kuro and Walking Route - 紫芝的博客 - CSDN博客





2018年06月07日 20:53:16[紫芝](https://me.csdn.net/qq_40507857)阅读数：56
个人分类：[DFS](https://blog.csdn.net/qq_40507857/article/category/7566193)








- 

Kuro is living in a country called Uberland, consisting of n

 towns, numbered from 1 to n, and n−1 bidirectional roads connecting these towns. It is possible to reach each town from any other. Each road connects two towns a and b. Kuro loves walking and he is planning to take a walking marathon, in which he will choose a pair of towns (u,v) (u≠v) and walk from u using the shortest path to v (note that (u,v) is considered to be different from (v,u)
).

Oddly, there are 2 special towns in Uberland named Flowrisa (denoted with the index x
) and Beetopia (denoted with the index y). Flowrisa is a town where there are many strong-scent flowers, and Beetopia is another town where many bees live. In particular, Kuro will avoid any pair of towns (u,v) if on the path from u to v
, he reaches Beetopia after he reached Flowrisa, since the bees will be attracted with the flower smell on Kuro’s body and sting him.

Kuro wants to know how many pair of city (u,v)


 he can take as his route. Since he’s not really bright, he asked you to help him with this problem.



Input

The first line contains three integers n

, x and y (1≤n≤3⋅105,1≤x,y≤n, x≠y
) - the number of towns, index of the town Flowrisa and index of the town Beetopia, respectively.

n−1
 lines follow, each line contains two integers a and b (1≤a,b≤n, a≠b), describes a road connecting two towns a and b

.

It is guaranteed that from each town, we can reach every other town in the city using the given roads. That is, the given map of towns and roads is a tree.



Output

A single integer resembles the number of pair of towns (u,v)



 that Kuro can use as his walking route.



Examples



input
Copy

3 1 3
1 2
2 3



output
Copy

5


input
Copy

3 1 3
1 2
1 3



output
Copy

4




Note

On the first example, Kuro can choose these pairs:
- (1,2)

: his route would be 1→2
- ,
- (2,3): his route would be 
2→3,(3,2): his route would be 3→2,(2,1): his route would be 2→1,(3,1): his route would be 3→2→1- .

Kuro can't choose pair (1,3)
 since his walking route would be 1→2→3, in which Kuro visits town 1 (Flowrisa) and then visits town 3 (Beetopia), which is not allowed (note that pair (3,1)
 is still allowed because although Kuro visited Flowrisa and Beetopia, he did not visit them in that order).

On the second example, Kuro can choose the following pairs:
- (1,2)
: his route would be 1→2,(2,1): his route would be 2→1,(3,2): his route would be 3→1→2,(3,1)
: his route would be 3→1

题意



    给出n个点，有n-1个边将他们链接。给出x,y，当某一路径中出现x....y时，此路不通。路径（u,v）和（v,u）是不同的。




思路

    n个点而只有n-1条边，所以不会出现环路径。点对数为n*（n-1）。此时我们可以先找出哪一些路径是不符合的，dfs一遍从x到y的路径并记录，这样就可以提取出（x,y）这段路中间的点。再分别从x，y为起点跑一遍dfs,可以统计出x,y两侧的点的个数cntx、cnty。

    画一张简图很好理解，答案就是总点对数减去x、y两侧点数的乘积，即为n*(n-1)-cntx*cnty。

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn = 300005;
vector<int> G[maxn];
int n,x,y,u,v,son[maxn];
void dfs(int x,int fa,int ban){
  son[x] = 1;
  int size = G[x].size();
  for(int i = 0;i < size; i++){
    int k = G[x][i];
    if(k != fa && k != ban){
      dfs(k,x,ban);
      son[x] += son[k];
    }
  }
}
int main()
{
  scanf("%d%d%d",&n,&x,&y);
  for(int i = 1;i < n; i++){
    scanf("%d %d",&u,&v);
    G[u].push_back(v);
    G[v].push_back(u);
  }
  dfs(x,0,y);
  dfs(y,0,x);
  int mid = son[x] + son[y] - n;
  long long int A = son[x] - mid,B = son[y] - mid,m = n;
  long long int ans = m*(m - 1) - A*B;
  printf("%I64d\n",ans);


  return 0;
}
```




