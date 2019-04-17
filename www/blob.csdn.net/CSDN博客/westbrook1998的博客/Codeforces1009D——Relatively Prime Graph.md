# Codeforces1009D——Relatively Prime Graph - westbrook1998的博客 - CSDN博客





2018年08月19日 19:23:32[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：34








> 
Let’s call an undirected graph G=(V,E) relatively prime if and only if for each edge (v,u)∈E  GCD(v,u)=1 (the greatest common divisor of v and u is 1). If there is no edge between some pair of vertices v and u then the value of GCD(v,u) doesn’t matter. The vertices are numbered from 1 to |V|. 

  Construct a relatively prime graph with n vertices and m edges such that it is connected and it contains neither self-loops nor multiple edges. 

  If there exists no valid graph with the given number of vertices and edges then output “Impossible”. 

  If there are multiple answers then print any of them. 

  Input 

  The only line contains two integers n and m (1≤n,m≤105) — the number of vertices and the number of edges. 

  Output 

  If there exists no valid graph with the given number of vertices and edges then output “Impossible”. 

  Otherwise print the answer in the following format: 

  The first line should contain the word “Possible”. 

  The i-th of the next m lines should contain the i-th edge (vi,ui) of the resulting graph (1≤vi,ui≤n,vi≠ui). For each pair (v,u) there can be no more pairs (v,u) or (u,v). The vertices are numbered from 1 to n. 

  If there are multiple answers then print any of them. 

  Examples 

  Input 

  5 6 

  Output 

  Possible 

  2 5 

  3 2 

  5 1 

  3 4 

  4 1 

  5 4 

  Input 

  6 12 

  Output 

  Impossible 

  Note 

  Here is the representation of the graph from the first example: ![这里写图片描述](https://odzkskevi.qnssl.com/63576f4634487698d2035fb9addb537c?v=1534186259)
一开始以为是什么很难的图论问题…结果看了题解发现是暴力？？？

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
vector<pair<int,int>> res;
int main(void){
    int n,m;
    scanf("%d%d",&n,&m);
    bool flag=false;
    for(int i=1;i<n;i++){
        if(flag){
            break;
        }
        for(int j=i+1;j<=n;j++){
            if(res.size()==m){
                flag=true;
                break;
            }
            if(__gcd(i,j)==1){
                res.push_back(make_pair(i,j));
            }
        }
    }
    int l=res.size();
    if(m<n-1 || l<m){
        printf("Impossible\n");
    }
    else{
        printf("Possible\n");
        for(int i=0;i<l;i++){
            printf("%d %d\n",res[i].first,res[i].second);
        }
    }
    return 0;
}
```





