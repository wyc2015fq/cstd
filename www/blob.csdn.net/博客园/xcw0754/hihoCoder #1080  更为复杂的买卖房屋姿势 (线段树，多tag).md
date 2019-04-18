# hihoCoder #1080 : 更为复杂的买卖房屋姿势 (线段树，多tag) - xcw0754 - 博客园
# [hihoCoder #1080 : 更为复杂的买卖房屋姿势 (线段树，多tag)](https://www.cnblogs.com/xcw0754/p/4513036.html)
题意：
　　有编号为0~n的n+1个房屋，给出每个房屋的起始价格，随后给出m种修改，每次修改都要进行输出所有房屋的价格总和。修改有两种方式：（1）政府调控，编号L~R全置为同一价格（0）房屋自行涨跌，编号L~R的房屋价格全部涨/跌一部分。
思路：
　　只需要一个tag+浮动的价格就可以解决这个问题了。当tag=1时，表示需要对该节点的孩子进行修改。进行何种修改？看浮动价格flo是否为0，若为0，表示需要设置为同一价格，否则，表示价格需要涨跌。当先进行set后进行add时，按照set的方式下放即可；当先进行add后进行set时，set直接覆盖掉add，依然按照set的方式下放即可。
**不知道错在哪**
```
1 #include <bits/stdc++.h>
  2 using namespace std;
  3 int n, m, e, l, r, d, v;
  4 bool flag;
  5 struct node
  6 {
  7     int sum;    //总和
  8     int flo;    //浮动价格
  9     bool tag;   //lazy tag
 10     node *ll,*rr;
 11 };
 12 
 13 node *create()
 14 {
 15     node *tmp=new(node);
 16     tmp->sum=tmp->flo=tmp->tag=0;
 17     tmp->ll=tmp->rr=0;
 18     return tmp;
 19 }
 20 
 21 node * create_tree(int LL,int RR)
 22 {
 23     node *tmp=create();
 24     if(LL==RR)  //没有必要设置lazy tag
 25     {
 26         int a;
 27         cin>>a;
 28         tmp->sum=a;
 29         return tmp;
 30     }
 31     int mid=((LL+RR)>>1);
 32     tmp->ll= create_tree(LL,mid);
 33     tmp->rr= create_tree(mid+1,RR);
 34     tmp->sum= tmp->ll->sum + tmp->rr->sum;
 35     return tmp;
 36 }
 37 
 38 int dfs(const int ll,const int rr,const int LL,const int RR,const int val,node *t)
 39 {
 40     if(ll==LL&&rr==RR) 
 41     {
 42         t->tag=1;
 43         if(flag==1)    //政府调控
 44         {
 45             t->flo=0;
 46             t->sum=(RR-LL+1)*val;
 47         }
 48         else        //自发涨价
 49         {
 50             t->flo=val;
 51             t->sum+=(RR-LL+1)*val;
 52         }
 53         return t->sum;
 54     }
 55 
 56     int mid=(LL+RR)/2;
 57     if(t->tag)    //下放
 58     {
 59         t->tag=0;
 60         t->ll->tag= t->rr->tag= 1;
 61         if(t->flo==0)//政府调控
 62         {
 63             t->ll->flo= t->rr->flo= 0;
 64             t->ll->sum= t->sum/(RR-LL+1)*(mid-LL+1);
 65             t->rr->sum= t->sum - t->ll->sum;
 66         }
 67         else  //需要改浮动价格的
 68         {
 69             t->ll->flo= t->rr->flo= t->flo;
 70             t->ll->sum+= (mid-LL+1)* t->flo;
 71             t->rr->sum+= (RR-mid)* t->flo;
 72         }
 73     }
 74 
 75     if(ll>mid)
 76         t->sum= t->ll->sum + dfs(ll,rr,mid+1,RR,val,t->rr);
 77     else if(rr<=mid)
 78         t->sum= t->rr->sum + dfs(ll,rr,LL,mid,val,t->ll);
 79     else
 80         t->sum=dfs(ll,mid,LL,mid,val,t->ll) + dfs(mid+1,rr,mid+1,RR,val,t->rr);
 81     return t->sum;
 82 }
 83 
 84 int main()
 85 {
 86     //freopen("input.txt", "r", stdin);
 87     cin>>n>>m;
 88     node *tree=create_tree(1,n+1);    //建树
 89     for(int i=0; i<m; i++)    //修改并输出
 90     {
 91         scanf("%d",&e);
 92         if(e)
 93         {
 94             flag=1;
 95             scanf("%d%d%d",&l,&r,&v);
 96             printf("%d\n",dfs(l+1, r+1, 1, n+1, v, tree));
 97         }
 98         else
 99         {
100             flag=0;
101             scanf("%d%d%d",&l,&r,&d);
102             printf("%d\n",dfs(l+1, r+1, 1, n+1, d, tree));
103         }
104     }
105     return 0;
106 }
WA代码
```

