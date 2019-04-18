# hihoCode r#1077 : RMQ问题再临-线段树 - xcw0754 - 博客园
# [hihoCode r#1077 : RMQ问题再临-线段树](https://www.cnblogs.com/xcw0754/p/4506838.html)
思路：
　　两种实现方法：　（1）用链表（2）用数组。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 int n, q, L, R, op, P, W;
 4 
 5 struct node
 6 {
 7     int v;
 8     node *ll,*rr;
 9 };
10 
11 node* create()  //创建节点
12 {
13     node *tmp=new(node);
14     tmp->v=-1;
15     tmp->ll=tmp->rr=0;
16     return tmp;
17 }
18 
19 node* init_tree(int l,int r)    //初始化树
20 {
21     node *tmp=create();
22     if(l==r)
23     {
24         scanf("%d",&tmp->v);
25         return tmp;
26     }
27     tmp->ll=init_tree(l,(r+l)/2);
28     tmp->rr=init_tree((r+l)/2+1,r);
29     tmp->v= tmp->ll->v < tmp->rr->v?tmp->ll->v:tmp->rr->v;
30     return tmp;
31 }
32 
33 int query(int l,int r,int LL,int RR,node *t)    //查询(欲查询的左，右，区间下限，上限，根)
34 {
35     if(l==LL&&r==RR)    return  t->v;
36     int mid=((LL+RR)>>1);
37     if(l>mid)     return query(l, r, mid+1, RR, t->rr);
38     if(r<=mid)    return query(l, r, LL, mid, t->ll);
39     return min( query(l,mid,LL,mid,t->ll),query(mid+1,r,mid+1,RR,t->rr) );
40 }
41 
42 void update(int LL,int RR,node *t)   //修改，深搜，搜到后一直往回改。
43 {
44     if(LL==RR)
45     {
46         t->v=W;
47         return ;
48     }
49     int mid=((LL+RR)>>1);
50     int tmp;
51     if(P>mid)    update(mid+1,RR,t->rr);  //要改的在右边
52     else    update(LL,mid,t->ll);
53     t->v=min(t->ll->v,t->rr->v);
54 }
55 
56 int main()
57 {
58     freopen("input.txt", "r", stdin);
59     cin>>n;
60     node *tree=init_tree(1,n);
61     cin>>q;
62     for(int i=0; i<q; i++)
63     {
64         scanf("%d",&op);
65         if(op)  //修改
66         {
67             scanf("%d%d",&P,&W);
68             update(1,n,tree);
69         }
70         else    //查询
71         {
72             scanf("%d%d",&L,&R);
73             printf("%d\n",query(L,R,1,n,tree) );
74         }
75     }
76     return 0;
77 }
AC代码（链实现）
```

