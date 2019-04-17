# hdu1213——How Many Tables - westbrook1998的博客 - CSDN博客





2018年07月27日 16:34:27[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：23标签：[并查集](https://so.csdn.net/so/search/s.do?q=并查集&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Today is Ignatius’ birthday. He invites a lot of friends. Now it’s dinner time. Ignatius wants to know how many tables he needs at least. You have to notice that not all the friends know each other, and all the friends do not want to stay with strangers.  

  One important rule for this problem is that if I tell you A knows B, and B knows C, that means A, B, C know each other, so they can stay in one table.  

  For example: If I tell you A knows B, B knows C, and D knows E, so A, B, C can stay in one table, and D, E have to stay in the other one. So Ignatius needs 2 tables at least.  

  Input 

  The input starts with an integer T(1<=T<=25) which indicate the number of test cases. Then T test cases follow. Each test case starts with two integers N and M(1<=N,M<=1000). N indicates the number of friends, the friends are marked from 1 to N. Then M lines follow. Each line consists of two integers A and B(A!=B), that means friend A and friend B know each other. There will be a blank line between two cases.  

  Output 

  For each test case, just output how many tables Ignatius needs at least. Do NOT print any blanks.  

  Sample Input 

  2 

  5 3 

  1 2 

  2 3 

  4 5 

  5 1 

  2 5 

  Sample Output 

  2 

  4
明明是一道并查集的水题 还是卡了很久 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=10050;
int p[MAXN];
int find(int x){
    int t=x;
    while(t!=p[t]){
        t=p[t];
    }
    return p[x]=t;
}
void unit(int a,int b){
    int pa=find(a);
    int pb=find(b);
    if(pa!=pb){
        p[pa]=pb;
    }
}
int main(void){
    int t;
    int n,m;
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++){
            p[i]=i;
        }
        int cnt=0;
        int a,b;
        while(m--){
            scanf("%d%d",&a,&b);
            unit(a,b);
        }
        for(int i=1;i<=n;i++){
            if(p[i]==i){
                cnt++;
            }
        }
        printf("%d\n",cnt);
    }
    return 0;
}
```





