# poj1258——Agri-Net - westbrook1998的博客 - CSDN博客





2018年06月26日 10:42:22[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：31标签：[最小生成树																[Kruskal](https://so.csdn.net/so/search/s.do?q=Kruskal&t=blog)](https://so.csdn.net/so/search/s.do?q=最小生成树&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Farmer John has been elected mayor of his town! One of his campaign promises was to bring internet connectivity to all farms in the area. He needs your help, of course.  

  Farmer John ordered a high speed connection for his farm and is going to share his connectivity with the other farmers. To minimize cost, he wants to lay the minimum amount of optical fiber to connect his farm to all the other farms.  

  Given a list of how much fiber it takes to connect each pair of farms, you must find the minimum amount of fiber needed to connect them all together. Each farm must connect to some other farm such that a packet can flow from any one farm to any other farm.  

  The distance between any two farms will not exceed 100,000.  

  Input 

  The input includes several cases. For each case, the first line contains the number of farms, N (3 <= N <= 100). The following lines contain the N x N conectivity matrix, where each element shows the distance from on farm to another. Logically, they are N lines of N space-separated integers. Physically, they are limited in length to 80 characters, so some lines continue onto others. Of course, the diagonal will be 0, since the distance from farm i to itself is not interesting for this problem. 

  Output 

  For each case, output a single integer length that is the sum of the minimum length of fiber required to connect the entire set of farms. 

  Sample Input 

  4 

  0 4 9 21 

  4 0 8 17 

  9 8 0 16 

  21 17 16 0 

  Sample Output 

  28
我是真的蠢，看漏了一个several cases 然后改好的时候又把return 0;放while里面了…

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=10050;
#define _cls(x) memset(x,0,sizeof(x))
int n,m;
int u[MAXN],v[MAXN],w[MAXN],p[MAXN],r[MAXN];
void Init(){
    _cls(u);
    _cls(v);
    _cls(w);
    _cls(p);
    _cls(w);
}
int cmp(const int i,const int j){
    return w[i]<w[j];
}
int find(int x){
    return p[x]==x ? x : p[x]=find(p[x]);
}
int Kruskal(){
    int ans=0;
    for(int i=0;i<n;i++){
        p[i]=i;
    }
    for(int i=0;i<m;i++){
        r[i]=i;
    }
    sort(r,r+m,cmp);
    for(int i=0;i<m;i++){
        int e=r[i];
        int x=find(u[e]);
        int y=find(v[e]);
        if(x!=y){
            ans+=w[e];
            p[x]=y;
        }
    }
    return ans;
}
int main(void){
    while(~scanf("%d",&n)){
        Init();
        m=0;
        int we;
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                scanf("%d",&we);
                if(i>=j){
                    continue;
                }
                u[m]=i;
                v[m]=j;
                w[m++]=we;
            }
        }
        int res=Kruskal();
        printf("%d\n",res);
    }
    return 0;
}
```






