# hihoCoder #1078 : 线段树的区间修改 - xcw0754 - 博客园
# [hihoCoder #1078 : 线段树的区间修改](https://www.cnblogs.com/xcw0754/p/4506910.html)
|G++|213ms|12MB|
题意:给一个序列，询问时要返回给定区间的和，修改时整个区间都改成一个值。
思路：
　　按提示说的做。线段树，再设置lazy_tag，必要时再修改
　　查询时遇到tag就将自身分配成两半分别给左右孩子（如果有的话），然后为左右孩子置tag，取消自身的tag。当查询的节点刚好有tag，不用管了（用不到其孩子），直接返回即可。
　　修改时遇到tag按照查询的那种方法，用不到孩子就不用改，用到的时候必须将tag往下推给孩子。注意此题是要返回区间和，所以如果答案有多个区间的，必须return多个区间的和。
```
1 #include <bits/stdc++.h>
  2 using namespace std;
  3 const int N=100010;
  4 int n, q, L, R, op, a, b, W;
  5 struct node
  6 {
  7     int v;
  8     bool flag;
  9     node *ll,*rr;
 10 
 11 };
 12 
 13 node* create()  //创建节点
 14 {
 15     node *tmp=new(node);
 16     tmp->flag=tmp->v=0;
 17     tmp->ll=tmp->rr=0;
 18     return tmp;
 19 }
 20 
 21 node* init_tree(int l,int r)    //初始化树
 22 {
 23     node *tmp=create();
 24     if(l==r)
 25     {
 26         scanf("%d",&tmp->v);
 27         return tmp;
 28     }
 29     tmp->ll=init_tree(l,(r+l)>>1);
 30     tmp->rr=init_tree(((r+l)>>1)+1,r);
 31     tmp->v =tmp->ll->v + tmp->rr->v;
 32     return tmp;
 33 }
 34 
 35 int query(int l,int r,int LL,int RR,node *t) //查询(欲查询的左，右，区间下限，上限，根)
 36 {
 37     if(l==LL&&r==RR)    return t->v;
 38     int mid=((RR+LL)>>1);
 39     if(t->flag)
 40     {
 41         if(t->ll)
 42         {
 43             t->ll->flag=t->rr->flag=1;
 44             t->rr->v=t->v/(RR-LL+1)*(RR-mid);
 45             t->ll->v=t->v-t->rr->v;
 46         }
 47         t->flag=0;
 48     }
 49     if(l>mid)    return query(l,r,mid+1,RR,t->rr);
 50     if(r<=mid)    return query(l,r,LL,mid,t->ll);
 51     return query(l,mid,LL,mid,t->ll)+query(mid+1,r,mid+1,RR,t->rr);
 52 }
 53 void update(int l,int r,int LL,int RR,node *t)   //修改。
 54 {
 55     if(l==LL&&r==RR)
 56     {
 57         t->flag=1;
 58         t->v=(r-l+1)*W;
 59         //cout<<"****"<<t->v<<endl;
 60         return ;
 61     }
 62     //修改的过程中也可能遇到flag=1的情况，要进行改；
 63     int mid=((LL+RR)>>1);
 64     if(t->flag)
 65     {
 66         if(t->ll)
 67         {
 68             t->rr->flag=t->ll->flag=1;
 69             t->rr->v= t->v/(RR-LL+1)*(RR-mid);
 70             t->ll->v= t->v - t->rr->v;
 71         }
 72         t->flag=0;
 73     }
 74     if(l>mid)    update(l, r, mid+1, RR, t->rr);  //要改的在右边
 75     else if(r<=mid)    update(l, r, LL, mid, t->ll);
 76     else
 77     {
 78         update(l, mid, LL, mid, t->ll);
 79         update(mid+1, r, mid+1, RR, t->rr);
 80     }
 81     t->v= t->ll->v + t->rr->v;
 82 }
 83 int main()
 84 {
 85     //freopen("input.txt", "r", stdin);
 86     cin>>n;
 87     node *tree=init_tree(1,n);
 88     //cout<<query(2,n,1,n,tree)<<endl;
 89     cin>>q;
 90     for(int i=0; i<q; i++)
 91     {
 92         scanf("%d",&op);
 93         if(op)  //修改
 94         {
 95             scanf("%d%d%d",&a,&b,&W);
 96             update(a,b,1,n,tree);
 97         }
 98         else    //查询
 99         {
100             scanf("%d%d",&L,&R);
101             printf("%d\n",query(L,R,1,n,tree) );
102         }
103     }
104     return 0;
105 }
AC代码
```

