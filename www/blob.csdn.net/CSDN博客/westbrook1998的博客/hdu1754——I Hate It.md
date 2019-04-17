# hdu1754——I Hate It - westbrook1998的博客 - CSDN博客





2018年08月27日 21:19:18[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：21标签：[线段树](https://so.csdn.net/so/search/s.do?q=线段树&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
很多学校流行一种比较的习惯。老师们很喜欢询问，从某某到某某当中，分数最高的是多少。  

  这让很多学生很反感。  

  不管你喜不喜欢，现在需要你做的是，就是按照老师的要求，写一个程序，模拟老师的询问。当然，老师有时候需要更新某位同学的成绩。 

  Input 

  本题目包含多组测试，请处理到文件结束。  

  在每个测试的第一行，有两个正整数 N 和 M ( 0
```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=200020;
struct node {
    int Max;
    int l;
    int r;
}tree[4*N];
int n, m ;
void build(int k,int ll,int rr){
    tree[k].l=ll;
    tree[k].r=rr;
    if(tree[k].l==tree[k].r){
        scanf("%d",&tree[k].Max);
        return;
    }
    int m=(ll+rr)/2;
    build(k*2,ll,m);
    build(k*2+1,m+1,rr);
    tree[k].Max=max(tree[k*2].Max,tree[k*2+1].Max);
}
inline int ask(int k,int a,int b){
    if(tree[k].l>b|| tree[k].r<a){
        return 0;
    }
    if(tree[k].l>=a && tree[k].r<=b){
        return tree[k].Max;
    }
    int _a=ask(k*2, a,b);
    int _b=ask(k*2+1,a,b);
    return max(_a,_b);
}

inline int change_point(int k,int x,int y){
    if(x<tree[k].l || x>tree[k].r){
        return tree[k].Max;
    }
    if(tree[k].l==x && tree[k].r==x){
        return tree[k].Max=y;
    }
    int _a=change_point(k*2,x,y);

    int _b=change_point(k*2+1,x,y);
    return tree[k].Max=max(_a,_b);
}

int main ()
{
    char c ;
    int i ;
    int x, y ;
    while (~scanf ("%d%d", &n, &m))
    {
        build (1, 1, n) ;
        for (i = 1 ; i <= m ; ++i)
        {
            getchar () ;
            scanf ("%c%d%d", &c, &x, &y) ;
            if (c == 'Q')
            {
                printf ("%d\n", ask (1, x, y)) ;
            }
            else
            {
                change_point (1, x, y) ;
            }
        }
    }
    return 0 ;
}
```





