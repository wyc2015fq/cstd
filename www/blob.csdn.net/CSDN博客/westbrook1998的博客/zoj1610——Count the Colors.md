# zoj1610——Count the Colors - westbrook1998的博客 - CSDN博客





2018年08月27日 21:50:01[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：32标签：[线段树](https://so.csdn.net/so/search/s.do?q=线段树&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Painting some colored segments on a line, some previously painted segments may be covered by some the subsequent ones. 

  Your task is counting the segments of different colors you can see at last. 

  Input 

  The first line of each data set contains exactly one integer n, 1 <= n <= 8000, equal to the number of colored segments. 

  Each of the following n lines consists of exactly 3 nonnegative integers separated by single spaces: 

  x1 x2 c 

  x1 and x2 indicate the left endpoint and right endpoint of the segment, c indicates the color of the segment. 

  All the numbers are in the range [0, 8000], and they are all integers. 

  Input may contain several data set, process to the end of file. 

  Output 

  Each line of the output should contain a color index that can be seen from the top, following the count of the segments of this color, they should be printed according to the color index. 

  If some color can’t be seen, you shouldn’t print it. 

  Print a blank line after every dataset. 

  Sample Input 

  5 

  0 4 4 

  0 3 1 

  3 4 2 

  0 2 2 

  0 2 3 

  4 

  0 1 1 

  3 4 1 

  1 3 2 

  1 3 1 

  6 

  0 1 0 

  1 2 1 

  2 3 1 

  1 2 0 

  2 3 0 

  1 2 1 

  Sample Output 

  1 1 

  2 1 

  3 1 

  1 1 

  0 2 

  1 1
对线段染色 然后问最后能看多几种颜色 分别有多少段

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=8050;
int n;
int last;
int cnt[N];
struct node{
    int l;
    int r;
    int f;
}tr[N<<2];
void build(int rt,int l,int r){
    tr[rt].l=l;
    tr[rt].r=r;
    //多组数据 重新建树要清楚懒惰标记
    //标记也表示颜色
    tr[rt].f=-1;
    if(l==r){
        return;
    }
    int m=(l+r)>>1;
    build(rt<<1,l,m);
    build(rt<<1|1,m+1,r);
}
void pushdown(int rt){
    tr[rt<<1].f=tr[rt<<1|1].f=tr[rt].f;
    tr[rt].f=-1;
}
void update(int L,int R,int f,int l,int r,int rt){
    //染色区域大于节点区域
    if(L<=l && R>=r){
        tr[rt].f=f;
        return;
    }
    if(tr[rt].f!=-1){
        pushdown(rt);
    }
    int m=(l+r)>>1;
    if(L<=m){
        update(L,R,f,l,m,rt<<1);
    }
    if(R>m){
        update(L,R,f,m+1,r,rt<<1|1);
    }
}
void query(int l,int r,int rt){
    if(l==r){
        if(tr[rt].f!=-1 && tr[rt].f!=last){
            cnt[tr[rt].f]++;
        }
        last=tr[rt].f;
        return;
    }
    if(tr[rt].f!=-1){
        pushdown(rt);
    }
    int m=(l+r)>>1;
    query(l,m,rt<<1);
    query(m+1,r,rt<<1|1);
}
int main(void){
    //freopen("data.txt","r",stdin);
    while(~scanf("%d",&n)){
        memset(cnt,0,sizeof(cnt));
        build(1,1,N);
        int a,b,c;
        for(int i=0;i<n;i++){
            scanf("%d%d%d",&a,&b,&c);
            //左端点加1 比如1-2 3-4这两个区间是不连续的
            update(a+1,b,c,1,N,1);
        }
        //记录下一层节点的颜色
        last=-1;
        query(1,N,1);
        for(int i=0;i<N;i++){
            if(cnt[i]){
                printf("%d %d\n",i,cnt[i]);
            }
        }
        printf("\n");
    }
    return 0;
}
```





