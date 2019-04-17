# hdu6300——Triangle Partition - westbrook1998的博客 - CSDN博客





2018年07月30日 13:04:22[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：35标签：[贪心																[排序](https://so.csdn.net/so/search/s.do?q=排序&t=blog)](https://so.csdn.net/so/search/s.do?q=贪心&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Chiaki has 3n points p1,p2,…,p3n. It is guaranteed that no three points are collinear.  

  Chiaki would like to construct n disjoint triangles where each vertex comes from the 3n points.  

  Input 

  There are multiple test cases. The first line of input contains an integer T, indicating the number of test cases. For each test case:  

  The first line contains an integer n (1≤n≤1000) – the number of triangle to construct.  

  Each of the next 3n lines contains two integers xi and yi (−109≤xi,yi≤109).  

  It is guaranteed that the sum of all n does not exceed 10000.  

  Output 

  For each test case, output n lines contain three integers ai,bi,ci (1≤ai,bi,ci≤3n) each denoting the indices of points the i-th triangle use. If there are multiple solutions, you can output any of them.  

  Sample Input 

  1 

  1 

  1 2 

  2 3 

  3 5 

  Sample Output 

  1 2 3
给一些点 保证不三点共线 求连成的多个三角形的点的id顺序 

结构体 然后排序一下

代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=1050;
struct node{
    int id;
    int x;
    int y;
}points[3*MAXN];
bool cmp(node a,node b){
    if(a.x!=b.x){
        return a.x<b.x;
    }
    else{
        return a.y<b.y;
    }
}
int main(void){
    int t,n;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        for(int i=0;i<3*n;i++){
            scanf("%d%d",&points[i].x,&points[i].y);
            points[i].id=i+1;
        }
        sort(points,points+3*n,cmp);
        for(int i=0;i<n;i++){
            printf("%d %d %d\n",points[3*i+0].id,points[3*i+1].id,points[3*i+2].id);
        }
    }
    return 0;
}
```






