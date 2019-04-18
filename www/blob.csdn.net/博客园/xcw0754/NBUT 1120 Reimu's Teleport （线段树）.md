# NBUT 1120 Reimu's Teleport （线段树） - xcw0754 - 博客园
# [NBUT 1120 Reimu's Teleport （线段树）](https://www.cnblogs.com/xcw0754/p/4538835.html)
题意：
　　有n个格子，一开始全部面向top。接下来的每次修改F a b ，如果 a>b则将a~b之间的格子全面置为向右，否则置为向左。对于每个询问Q输出向左、top、右的数量。
思路：
　　普通线段树。每个结点中保存三个值，分别对应3个方向的数量。要设置一下lazy_tag以减少时间消耗，因为有可能Q远少于F，那么一直在更改，大多可能不需要改到，暂时不更新，询问时要用到再更新。每次一改就整个区间都是一个方向，所以当有tag时可以往下推各占多少。
```
1 #include<bits/stdc++.h>
  2 using namespace std;
  3 int ans[3];//答案
  4 struct node
  5 {
  6     bool tag;
  7     int num[3];
  8     node *ll,*rr;
  9     node()
 10     {
 11         tag=false;
 12         num[0]=num[1]=num[2]=0;
 13         ll=rr=0;
 14     }
 15 };
 16 
 17 node * built(int LL, int RR)
 18 {
 19     node *tmp=new(node);
 20     if(LL==RR)
 21     {
 22         tmp->num[1]=1;
 23         return tmp;
 24     }
 25     int mid=(LL+RR)>>1;
 26     tmp->ll=built(LL,mid);
 27     tmp->rr=built(mid+1,RR);
 28     tmp->num[1]+=tmp->ll->num[1]+tmp->rr->num[1];   //开始只需要更新向上的
 29     return tmp;
 30 }
 31 
 32 void modify( node *t,int LL,int RR,int mid)//专门修改统计数量
 33 {
 34     if(t->num[0])
 35     {
 36         t->ll->num[0]=mid+1-LL;
 37         t->ll->num[1]=t->ll->num[2]=0;
 38         t->rr->num[0]=RR-mid;
 39         t->rr->num[1]=t->rr->num[2]=0;
 40     }
 41     if(t->num[1])
 42     {
 43         t->ll->num[1]=mid+1-LL;
 44         t->ll->num[0]=t->ll->num[2]=0;
 45         t->rr->num[1]=RR-mid;
 46         t->rr->num[0]=t->rr->num[2]=0;
 47     }
 48     if(t->num[2])
 49     {
 50         t->ll->num[2]=mid+1-LL;
 51         t->ll->num[0]=t->ll->num[1]=0;
 52         t->rr->num[2]=RR-mid;
 53         t->rr->num[0]=t->rr->num[1]=0;
 54     }
 55 }
 56 
 57 void update(int ll,int rr,int LL,int RR,int c,node *t)
 58 {
 59     if(ll==LL&&rr==RR)
 60     {
 61         t->tag=true;
 62         if(c==0)
 63         {
 64             t->num[0]=RR-LL+1;
 65             t->num[1]=t->num[2]=0;
 66         }
 67         else
 68         {
 69             t->num[2]=RR-LL+1;
 70             t->num[0]=t->num[1]=0;
 71         }
 72         return;
 73     }
 74     int mid=(LL+RR)>>1;
 75 
 76     if(t->tag)
 77     {
 78         t->tag=false;
 79         t->ll->tag=t->rr->tag=true;
 80         modify(t,LL,RR,mid);
 81     }
 82     if(ll>mid)  //右边
 83         update(ll,rr,mid+1,RR,c,t->rr);
 84     else if(rr<=mid)    //左边
 85         update(ll,rr,LL,mid,c,t->ll);
 86     else
 87     {
 88         update(ll, mid, LL, mid, c,t->ll);
 89         update(mid+1,rr,mid+1,RR,c,t->rr);
 90     }
 91     t->num[0]=t->ll->num[0]+t->rr->num[0];
 92     t->num[1]=t->ll->num[1]+t->rr->num[1];
 93     t->num[2]=t->ll->num[2]+t->rr->num[2];
 94 }
 95 
 96 
 97 
 98 void query(int ll,int rr,int LL,int RR,node* t)
 99 {
100     if(ll==LL && rr==RR)
101     {
102         ans[0]+=t->num[0];
103         ans[1]+=t->num[1];
104         ans[2]+=t->num[2];
105         return;
106     }
107     int mid=((LL+RR)>>1);
108     if(t->tag)
109     {
110         t->tag=false;
111         t->ll->tag= t->rr->tag= true;
112         modify(t, LL, RR, mid);
113     }
114 
115     if(ll>mid)          query(ll,rr,mid+1,RR,t->rr);
116     else if(rr<=mid)    query(ll,rr,LL,mid,t->ll);
117     else
118     {
119         query(ll, mid, LL, mid, t->ll);
120         query(mid+1,rr,mid+1,RR,t->rr);
121     }
122 
123 }
124 
125 
126 int main()
127 {
128     //freopen("e://input.txt","r",stdin);
129     int a, t, e, r;
130     char ch;
131 
132     while(cin>>a>>t)
133     {
134         node *tree=built(1,t); //建树
135 
136         for(int i=0; i<a; i++)
137         {
138             cin>>ch;
139             scanf("%d %d", &e, &r);
140             if(ch=='F')  //修改
141             {
142                 if(e>r) update(r, e, 1, t, 0, tree);
143                 else    update(e, r, 1, t, 2, tree);
144             }
145             else        //Q 查询
146             {
147                 memset(ans,0,sizeof(ans));
148                 query(e,r,1,t,tree);
149                 printf("%d %d %d\n",ans[0],ans[1],ans[2]);
150             }
151         }
152     }
153     return 0;
154 }
AC代码
```

