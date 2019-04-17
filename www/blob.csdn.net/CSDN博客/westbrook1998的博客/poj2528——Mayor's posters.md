# poj2528——Mayor's posters - westbrook1998的博客 - CSDN博客





2018年08月27日 21:46:46[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：16








> 
The citizens of Bytetown, AB, could not stand that the candidates in the mayoral election campaign have been placing their electoral posters at all places at their whim. The city council has finally decided to build an electoral wall for placing the posters and introduce the following rules:  

  Every candidate can place exactly one poster on the wall.  

  All posters are of the same height equal to the height of the wall; the width of a poster can be any integer number of bytes (byte is the unit of length in Bytetown).  

  The wall is divided into segments and the width of each segment is one byte.  

  Each poster must completely cover a contiguous number of wall segments. 

  They have built a wall 10000000 bytes long (such that there is enough place for all candidates). When the electoral campaign was restarted, the candidates were placing their posters on the wall and their posters differed widely in width. Moreover, the candidates started placing their posters on wall segments already occupied by other posters. Everyone in Bytetown was curious whose posters will be visible (entirely or in part) on the last day before elections.  

  Your task is to find the number of visible posters when all the posters are placed given the information about posters’ size, their place and order of placement on the electoral wall.  

  Input 

  The first line of input contains a number c giving the number of cases that follow. The first line of data for a single case contains number 1 <= n <= 10000. The subsequent n lines describe the posters in the order in which they were placed. The i-th line among the n lines contains two integer numbers l i and ri which are the number of the wall segment occupied by the left end and the right end of the i-th poster, respectively. We know that for each 1 <= i <= n, 1 <= l i <= ri <= 10000000. After the i-th poster is placed, it entirely covers all wall segments numbered l i, l i+1 ,… , ri. 

  Output 

  For each input data set print the number of visible posters after all the posters are placed.  

  The picture below illustrates the case of the sample input.  

  Sample Input 
![这里写图片描述](https://odzkskevi.qnssl.com/85d8df2191db8df82ba259ebe2bfe93e?v=1535167593)

  1 

  5 

  1 4 

  2 6 

  8 10 

  3 4 

  7 10 

  Sample Output 

  4
贴海报的问题 线段树 

对着题解抄抄写写总算做出来 

因为坐标的范围很大 但是线段不多 所以要离散化….又学了新东西 

然后还有一个问题就是这种区间以段为节点的和以点为节点的不同 比如1-2 3-4这实际上是不连续的 这种情况一般是更新是左端点+1 这里就直接抄了题解的离散化方法了 

然后就维护一个标记就可以了
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <map>
#define bug printf("bug\n");
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=100050;
int n;
int ans;
//离散化后的坐标
int x[N<<1];
//原始端点坐标
struct Node{
    int l;
    int r;
}t[N];
int vis[N<<2];
int has[N<<2];
//标记下传
void pushdown(int rt){
    //染色问题直接将子节点染成和父节点一样的颜色
    vis[rt<<1]=vis[rt<<1|1]=vis[rt];
    //清空标记
    vis[rt]=-1;
}
//染色区间左端点 染色区间右端点 颜色标号 根节点左端点 根节点右端点 根节点编号
void update(int L,int R,int c,int l,int r,int rt){
    //染色区间大于根节点
    if(L<=l && R>=r){
        vis[rt]=c;
        return;
    }
    if(vis[rt]!=-1){
        pushdown(rt);
    }
    int m=(l+r)>>1;
    if(L<=m){
        update(L,R,c,l,m,rt<<1);
    }
    if(R>m){
        update(L,R,c,m+1,r,rt<<1|1);
    }
}
void query(int l,int r,int rt){

    if(vis[rt]!=-1){
        if(!has[vis[rt]]){
            ans++;
        }
        has[vis[rt]]=1;
        return;
    }
    //因为是段的线段树 离散化后3 3这个区间是没有意义的 直接返回
    //如果是点的线段树 3 3就表示3这个点
    if(l==r){
        return;
    }
    int m=(l+r)>>1;
    query(l,m,rt<<1);
    query(m+1,r,rt<<1|1);
}
int main(void){
    int T;
    scanf("%d",&T);
    while(T--){
        scanf("%d",&n);
        int k=0;
        for(int i=0;i<n;i++){
            scanf("%d%d",&t[i].l,&t[i].r);
            x[k++]=t[i].l;
            x[k++]=t[i].r;
        }
        sort(x,x+k);
        //去重
        int m=1;
        for(int i=1;i<k;i++){
            if(x[i]!=x[i-1]){
                x[m++]=x[i];
            }
        }
        //处理段的离散化的一个问题
        //比如1-10 1-4 5-10 离散化之后是1-4 1-2 3-4 看起来第二和第三段会覆盖第一段，但实际上不是
        for(int i=m-1;i>=1;i--){
            if(x[i]!=x[i-1]+1){
                x[m++]=x[i-1]+1;
            }
        }
        sort(x,x+m);
        _clr(vis,-1);
        //二分找到离散化后的坐标
        for(int i=0;i<n;i++){
            int l=lower_bound(x,x+m,t[i].l)-x;
            int r=lower_bound(x,x+m,t[i].r)-x;
            update(l,r,i,0,m,1);
        }
        _clr(has,0);
        ans=0;
        //整个区间查询
        query(0,m,1);
        printf("%d\n",ans);
    }
    return 0;
}
```






