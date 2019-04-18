# HDU 5316 Magician （线段树，单值修改，微变形） - xcw0754 - 博客园
# [HDU 5316 Magician （线段树，单值修改，微变形）](https://www.cnblogs.com/xcw0754/p/4685680.html)
题意：给一个初始序列A[1,n]，第j个数字代表精灵j的power值，有两种操作：（1）查询区间[L,R]　　（2）修改某个精灵的power值。  
　　但，查询的是区间[L,R]中一个美丽子序列sub[l,r]的和，美丽子序列是从A[L,R]中挑出的一些数字，这些数字按升序排好序，每两个相邻数字(sub[a],sub[b])在A[L,R]中的下标的的奇偶性不同。sub中至少有一个元素。power值可能为负。
思路：
　　单值修改的线段树，比较简单的模板。主要的问题在，奇偶性如何满足？
　　对于一个区间，要从中挑出一些数且满足相邻两数的下标奇偶性不同，那么就有4种选择（奇，奇）（奇，偶）（偶，奇）（偶，偶），“奇偶”代表其开头和结束位置的下标的奇偶性。现在又有问题了，如何合并区间？
　　以奇结尾的就必须以偶开头，这样全部组合一下，继续维护这4个值即可。比如（奇，奇）+（偶，偶），或（奇，偶）+（偶，奇）这些组合都行。该注意的是，因为power还有负值，又必须选1个，所以还可能（奇，奇）被两个区间（奇，奇）（奇，奇）其中的一个更新，（偶，偶）同理。
　　问题又来了，查询的时候，怎么返回？
　　那就绑成一个结构体返回。又要注意的是，可能[L,R]必须拆成两个区间时，合并区间的操作又要再做一次再返回这个结构体。
```
#include <bits/stdc++.h>
#define LL long long
using namespace std;
const int N=101000;
struct node
{
    long long a[2][2];  //1表示奇数，0表示偶数
    bool flag;
    node(){a[0][0]=a[1][1]=a[1][0]=a[0][1]=-1e18;flag=0;}
}point[N*4];
void get_val(int cur)
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            point[cur].a[i][j]=max( point[cur*2].a[i][j], point[cur*2+1].a[i][j]);
            point[cur].a[i][j]=max( point[cur].a[i][j], point[cur*2].a[i][1]+point[cur*2+1].a[0][j] );
            point[cur].a[i][j]=max( point[cur].a[i][j], point[cur*2].a[i][0]+point[cur*2+1].a[1][j] );
        }
    }
}
void create_tree(int l, int r, int cur)  //cur为当前结点下标
{
    if(l==r)
    {
        LL tmp;
        scanf("%lld",&tmp);
        point[cur]=node();
        point[cur].a[l&1][l&1]=tmp;
        return ;
    }
    int mid=(l+r)/2;
    create_tree(l, mid, cur*2);
    create_tree(mid+1, r, cur*2+1);
    get_val(cur);   //以孩子的值来更新自身的值
}
void update(int l,int r,int cur,int a,LL w)
{
    if(l==r)
    {
        point[cur]=node();
        point[cur].a[l&1][l&1]=w;
        return ;
    }
    int mid=(l+r)/2;
    if(a<=mid)    update(l, mid, cur*2, a, w);  //在左边
    else          update(mid+1, r, cur*2+1, a, w);
    get_val(cur);       //回溯时更新自身
}
node query(int l,int r, int L,int R,int cur)
{
    if(l==L && r==R)    return  point[cur];
    int mid=(L+R)/2;
    if(r<=mid)         return query(l,r, L,mid, cur*2);
    else if(l>mid)     return query(l,r, mid+1,R, cur*2+1);
    else
    {
        node tmp=node(), a=node(), b=node();
        a=query(l,mid, L,mid,cur*2);
        b=query(mid+1,r, mid+1,R,cur*2+1);
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<2; j++)
            {
                tmp.a[i][j]=max( a.a[i][j], b.a[i][j]);
                tmp.a[i][j]=max( tmp.a[i][j], a.a[i][1]+b.a[0][j] );
                tmp.a[i][j]=max( tmp.a[i][j], a.a[i][0]+b.a[1][j]);
            }
        }
        return tmp;
    }
}
int main()
{
    //freopen("input.txt", "r", stdin);
    int t, n, q, L, R, op;
    LL W;
    cin>>t;
    while(t--)
    {
        cin>>n>>q;
        create_tree(1, n, 1);
        for(int i=0; i<q; i++)
        {
            scanf("%d",&op);
            if(op==1)  //修改
            {
                scanf("%d %lld", &L, &W);
                update(1, n, 1, L, W);
            }
            else    //查询
            {
                scanf("%d %d", &L, &R);
                node tmp=query( L, R, 1, n, 1);
                LL ans1=max(tmp.a[0][0],tmp.a[1][1]);
                LL ans2=max(tmp.a[1][0],tmp.a[0][1]);
                printf("%lld\n",max(ans1,ans2) );
            }
        }
    }
    return 0;
}
AC代码
```

