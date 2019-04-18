# hihoCoder #1079 : 离散化 (线段树，数据离散化) - xcw0754 - 博客园
# [hihoCoder #1079 : 离散化 (线段树，数据离散化)](https://www.cnblogs.com/xcw0754/p/4512173.html)
题意：有一块宣传栏，高一定，给出长度，再给出多张海报的张贴位置，问还能见到几张海报（哪怕有一点被看到）？假设海报的高于宣传栏同高。
思路：问题转成“给出x轴上长为L的一条线段，再用n条线段进行覆盖上去，最后还能看到及条线”。长度是0~L，即长度是L，进行离散化的时候，应该用1~L，每个数字表示一个单位长。还有就是按照提示所给的信息实现即可。步骤如下：
（1）保存n个数据，做成pair，并将所有出现过的数字在另外找地方排序，去掉重复的，再将数据紧缩化处理，那么大小在1~max。再将紧缩化的数据与原数据做成映射表。
（2）建立线段树，原始n个数据进行映射后对线段树插入更新。注意上限的问题，因为已经离散化了。每个节点上有个值val，表示处于此段区间的是编号为val的海报。
（3）DFS搜索线段树，遇到tag=1的节点就进行标记，然后返回，其子树都没用处的，已经被覆盖。可能会有重复的，比如一段为[2,3]，一段为[3,5]，因为不能放在一起，所以分开放的时候会有重复的val，要去重。
```
1 #include <bits/stdc++.h>
  2 using namespace std;
  3 const int N=100010;
  4 int n,l,rr,ll,cnt=0;
  5 bool vis[N];
  6 unordered_map<int,int>  mapp;
  7 pair<int,int>   pai[N];
  8 struct node
  9 {
 10     int val;
 11     bool tag;
 12     node *ll,*rr;
 13 };
 14 
 15 node *create()
 16 {
 17     node *tmp=new(node);
 18     tmp->val=tmp->tag=0;
 19     tmp->ll=tmp->rr=0;
 20     return tmp;
 21 }
 22 
 23 void update(int l,int r,int LL,int RR,int num,node *t)  //其实可以不用递归，因为不用回溯
 24 {
 25     if(l==LL&&r==RR)//找到了对应区间
 26     {
 27         t->tag=1;
 28         t->val=num;
 29         return ;
 30     }
 31     if( LL!=RR && !t->ll ) //有孩子，但是还没有建立
 32     {
 33         t->ll=create();
 34         t->rr=create();
 35         t->ll->tag= t->rr->tag= 1;
 36         t->ll->val= t->rr->val= t->val;
 37     }
 38 
 39     int mid=((LL+RR)>>1);
 40     if(t->tag)//需要对此节点下推
 41     {
 42         if(t->ll)//前提是有孩子
 43         {
 44             t->ll->tag= t->rr->tag= 1;
 45             t->ll->val= t->rr->val= t->val;
 46         }
 47         t->tag=0;
 48     }
 49 
 50     if(l>mid)   update(l,r,mid+1,RR,num,t->rr);//在右边
 51     else if(r<=mid) update(l,r,LL,mid,num,t->ll);//在左边
 52     else
 53     {
 54         update(l,mid,LL,mid,num,t->ll);
 55         update(mid+1,r,mid+1,RR,num,t->rr);
 56     }
 57 }
 58 
 59 void DFS(int LL,int RR,node *t) //进行深搜，搜到tag为1的就有用，可能存在重复
 60 {
 61     if(t->tag)
 62     {
 63         vis[t->val]=1;//为了去重
 64         return;
 65     }
 66     int mid=((LL+RR)>>1);
 67     DFS(LL,mid,t->ll);
 68     DFS(mid+1,RR,t->rr);
 69 }
 70 
 71 int main()
 72 {
 73     //freopen("input.txt", "r", stdin);
 74 
 75     cin>>n>>l;
 76     vector<int> vect;   //辅助
 77     for(int i=0; i<n; i++)
 78     {
 79         scanf("%d%d",&ll,&rr);
 80         pai[i]=make_pair(ll,rr);
 81         vect.push_back(ll);
 82         vect.push_back(rr);
 83     }
 84 
 85     sort(vect.begin(),vect.end());
 86     vect.push_back(-1);     //仅仅为了下面的for不会溢出。
 87     int up=0;
 88     for(int i=0;i<vect.size()-1; i++)   //紧缩化处理
 89     {
 90         if(vect[i]!=vect[i+1])
 91             mapp[vect[i]]=++up; //做个哈希表
 92     }
 93     vect.clear();
 94 
 95     node *tree=create();    //建树根节点
 96     tree->tag=1;
 97 
 98     for(int i=0; i<n; i++)  //根据pair进行修改树。设置tag，在需要修改其儿子时在进行下推。
 99     {
100         int L=mapp[pai[i].first];
101         int R=mapp[pai[i].second];
102         update(L,R-1,1,up-1,i+1,tree);      //插入修改
103     }
104 
105     DFS(1,up,tree);//深搜
106     for(int i=1; i<=n; i++)    if(vis[i])  cnt++;
107     cout<<cnt<<endl;
108 
109 
110     return 0;
111 }
AC代码
```

