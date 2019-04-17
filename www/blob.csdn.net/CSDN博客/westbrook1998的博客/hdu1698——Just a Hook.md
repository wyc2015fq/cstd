# hdu1698——Just a Hook - westbrook1998的博客 - CSDN博客





2018年08月27日 21:48:45[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：21








> 
In the game of DotA, Pudge’s meat hook is actually the most horrible thing for most of the heroes. The hook is made up of several consecutive metallic sticks which are of the same length.  
![这里写图片描述](https://odzkskevi.qnssl.com/779a6c86f4db19106cba2c46a7dafe46?v=1534830988)

  Now Pudge wants to do some operations on the hook.  

  Let us number the consecutive metallic sticks of the hook from 1 to N. For each operation, Pudge can change the consecutive metallic sticks, numbered from X to Y, into cupreous sticks, silver sticks or golden sticks.  

  The total value of the hook is calculated as the sum of values of N metallic sticks. More precisely, the value for each kind of stick is calculated as follows:  

  For each cupreous stick, the value is 1.  

  For each silver stick, the value is 2.  

  For each golden stick, the value is 3.  

  Pudge wants to know the total value of the hook after performing the operations.  

  You may consider the original hook is made up of cupreous sticks.  

  Input 

  The input consists of several test cases. The first line of the input is the number of the cases. There are no more than 10 cases.  

  For each case, the first line contains an integer N, 1<=N<=100,000, which is the number of the sticks of Pudge’s meat hook and the second line contains an integer Q, 0<=Q<=100,000, which is the number of the operations.  

  Next Q lines, each line contains three integers X, Y, 1<=X<=Y<=N, Z, 1<=Z<=3, which defines an operation: change the sticks numbered from X to Y into the metal kind Z, where Z=1 represents the cupreous kind, Z=2 represents the silver kind and Z=3 represents the golden kind.  

  Output 

  For each case, print a number in a line representing the total value of the hook after the operations. Use the format in the example.  

  Sample Input 

  1 

  10 

  2 

  1 5 2 

  5 9 3 

  Sample Output 

  Case 1: The total value of the hook is 24.
不是很难的一道题 但还是wa到自闭 

最后发现是多组输入然后建树的时候懒惰标记没有初始化为0

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _cle(x,a) memset(x,a,sizeof(x));
#define bug printf("bug\n");
using namespace std;
const int N=100050;
struct node{
    int l;
    int r;
    int v;
    int f;
}tr[N<<2];
void build(int rt,int l,int r){
    tr[rt].l=l;
    tr[rt].r=r;
    tr[rt].f=0;
    if(tr[rt].l==tr[rt].r){
        tr[rt].v=1;
        return;
    }
    int m=(l+r)>>1;
    build(rt<<1,l,m);
    build(rt<<1|1,m+1,r);
    tr[rt].v=tr[rt<<1].v+tr[rt<<1|1].v;
}
void pushdown(int rt){
    tr[rt<<1].f=tr[rt].f;
    tr[rt<<1|1].f=tr[rt].f;
    tr[rt<<1].v=tr[rt].f*(tr[rt<<1].r-tr[rt<<1].l+1);
    tr[rt<<1|1].v=tr[rt].f*(tr[rt<<1|1].r-tr[rt<<1|1].l+1);
    tr[rt].f=0;
}
void update(int rt,int a,int b,int c){
    if(tr[rt].l>b || tr[rt].r<a){
        return;
    }
    if(tr[rt].l>=a && tr[rt].r<=b){
        tr[rt].v=(tr[rt].r-tr[rt].l+1)*c;
        tr[rt].f=c;
        return;
    }
    if(tr[rt].f){
        pushdown(rt);
    }
    int m=(tr[rt].l+tr[rt].r)>>1;
    if(a<=m){
        update(rt<<1,a,b,c);
    }
    if(b>m){
        update(rt<<1|1,a,b,c);
    }
    tr[rt].v=tr[rt<<1].v+tr[rt<<1|1].v;
}
int main(void){
    int t,n,q;
    int a,b,c;
    int cas=1;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        build(1,1,n);
        scanf("%d",&q);
        while(q--){
            scanf("%d%d%d",&a,&b,&c);
            update(1,a,b,c);
        }
        printf("Case %d: The total value of the hook is %d.\n",cas++,tr[1].v);
    }
    return 0;
}
```





