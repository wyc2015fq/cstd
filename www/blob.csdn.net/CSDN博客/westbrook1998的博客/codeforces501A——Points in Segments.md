# codeforces501A——Points in Segments - westbrook1998的博客 - CSDN博客





2018年08月01日 15:24:05[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：75标签：[暴力](https://so.csdn.net/so/search/s.do?q=暴力&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
You are given a set of n segments on the axis Ox, each segment has integer endpoints between 1 and m inclusive. Segments may intersect, overlap or even coincide with each other. Each segment is characterized by two integers li and ri (1≤li≤ri≤m) — coordinates of the left and of the right endpoints. 

  Consider all integer points between 1 and m inclusive. Your task is to print all such points that don’t belong to any segment. The point x belongs to the segment [l;r] if and only if l≤x≤r. 

  Input 

  The first line of the input contains two integers n and m (1≤n,m≤100) — the number of segments and the upper bound for coordinates. 

  The next n lines contain two integers each li and ri (1≤li≤ri≤m) — the endpoints of the i-th segment. Segments may intersect, overlap or even coincide with each other. Note, it is possible that li=ri, i.e. a segment can degenerate to a point. 

  Output 

  In the first line print one integer k — the number of points that don’t belong to any segment. 

  In the second line print exactly k integers in any order — the points that don’t belong to any segment. All points you print should be distinct. 

  If there are no such points at all, print a single integer 0 in the first line and either leave the second line empty or do not print it at all. 

  Examples 

  Input 

  3 5 

  2 2 

  1 2 

  5 5 

  Output 

  2 

  3 4  

  Input 

  1 7 

  1 7 

  Output 

  0 

  Note 

  In the first example the point 1 belongs to the second segment, the point 2 belongs to the first and the second segments and the point 5 belongs to the third segment. The points 3 and 4 do not belong to any segment. 

  In the second example all the points from 1 to 7 belong to the first segment.
暴力 

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=105;
int t[MAXN];
int n,m;
int a,b;
int main(void){
    scanf("%d%d",&n,&m);
    for(int i=0;i<n;i++){
        scanf("%d%d",&a,&b);
        for(int j=a;j<=b;j++){
            t[j]=1;
        }
    }
    int num=0;
    for(int i=1;i<=m;i++){
        if(!t[i]){
            num++;
        }
    }
    printf("%d\n",num);
    for(int i=1;i<=m;i++){
        if(!t[i]){
            printf("%d ",i);
        }
    }
    printf("\n");
    return 0;
}
```





