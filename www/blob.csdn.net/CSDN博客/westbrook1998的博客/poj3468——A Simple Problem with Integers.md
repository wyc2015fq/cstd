# poj3468——A Simple Problem with Integers - westbrook1998的博客 - CSDN博客





2018年08月27日 21:20:50[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：23








> 
You have N integers, A1, A2, … , AN. You need to deal with two kinds of operations. One type of operation is to add some given number to each number in a given interval. The other is to ask for the sum of numbers in a given interval. 

  Input 

  The first line contains two numbers N and Q. 1 ≤ N,Q ≤ 100000. 

  The second line contains N numbers, the initial values of A1, A2, … , AN. -1000000000 ≤ Ai ≤ 1000000000. 

  Each of the next Q lines represents an operation. 

  “C a b c” means adding c to each of Aa, Aa+1, … , Ab. -10000 ≤ c ≤ 10000. 

  “Q a b” means querying the sum of Aa, Aa+1, … , Ab. 

  Output 

  You need to answer all Q commands in order. One answer in a line. 

  Sample Input 

  10 5 

  1 2 3 4 5 6 7 8 9 10 

  Q 4 4 

  Q 1 10 

  Q 2 4 

  C 3 6 3 

  Q 2 4 

  Sample Output 

  4 

  55 

  9 

  15 

  Hint 

  The sums may exceed the range of 32-bit integers.
区间修改的线段树裸题

代码：

```
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int N=100000;
struct node{
    int l,r;
    long long s;
    long long f;
}tree[4*N];
int n,q;
long long ans;
inline void build(int k,int l,int r){
    tree[k].l=l;
    tree[k].r=r;
    if(tree[k].l==tree[k].r){
        scanf("%lld",&tree[k].s);
        return;
    }
    int m=l+(r-l)/2;
    build(k*2,l,m);
    build(k*2+1,m+1,r);
    tree[k].s=tree[k*2].s+tree[k*2+1].s;
}
inline void down(int k){
    tree[k*2].f+=tree[k].f;
    tree[k*2+1].f+=tree[k].f;
    tree[k*2].s+=tree[k].f*(tree[k*2].r-tree[k*2].l+1);
    tree[k*2+1].s+=tree[k].f*(tree[k*2+1].r-tree[k*2+1].l+1);
    tree[k].f=0;
}
inline void ask(int k,int a,int b){
    //整个区间覆盖
    if(tree[k].l>=a && tree[k].r <=b){
        ans+=tree[k].s;
        return;
    }
    if(tree[k].f){
        down(k);
    }
    int m=tree[k].l+(tree[k].r-tree[k].l)/2;
    if(a<=m){
        ask(k*2,a,b);
    }
    if(b>m){
        ask(k*2+1,a,b);
    }
}
inline void update(int k,int a,int b,int c){
    if(tree[k].l>b || tree[k].r<a){
        return;
    }
    if(tree[k].l>=a && tree[k].r<=b){
        //只更新这个节点，然后更新懒惰标记
        tree[k].s+=(tree[k].r-tree[k].l+1)*c;
        tree[k].f+=c;
        return;
    }
    if(tree[k].f){
        down(k);
    }
    int m=tree[k].l+(tree[k].r-tree[k].l)/2;
    if(a<=m){
        update(k*2,a,b,c);
    }
    if(b>m){
        update(k*2+1,a,b,c);
    }
    tree[k].s=tree[k*2].s+tree[k*2+1].s;
}
int main(void){
    while(~scanf("%d%d",&n,&q)){
        build(1,1,n);
        char t[2];
        int a,b,c;
        while(q--){
            scanf("%s%d%d",t,&a,&b);
            if(t[0]=='C'){
                scanf("%d",&c);
                update(1,a,b,c);
                // for(int i=1;i<=30;i++){
                //     printf("%d %d %d %d\n",tree[i].l,tree[i].r,tree[i].s,tree[i].f);
                // }
            }
            else{
                ans=0;
                ask(1,a,b);
                printf("%lld\n",ans);
            }
        }
    }
    return 0;
}
```





