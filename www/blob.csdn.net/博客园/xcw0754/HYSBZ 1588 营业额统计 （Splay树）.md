# HYSBZ  1588 营业额统计 （Splay树） - xcw0754 - 博客园
# [HYSBZ  1588 营业额统计 （Splay树）](https://www.cnblogs.com/xcw0754/p/4744321.html)
题意：给出一个公司每一天的营业额，求每天的最小波动值之和。该天的最小波动值= min { 绝对值| 该天以前某一天的营业额-该天的营业额 | }。第一天的最小波动值就是其自己。
思路：Splay伸展树的入门题，仅有splay，insert，rotate这三个主要的函数而已。
　　将一个数字（营业额）插入树中，并把其splay到根位置，然后其前驱和后继中离它较近的一个用来求最小波动值。注意可能没有前驱/后继。对第一个数特别处理。
　　注意：此题的数据有缺陷，读入营业额之前先将变量清零。　
```
1 #include <bits/stdc++.h>
  2 #define pii pair<int,int>
  3 #define INF 0x7f7f7f7f
  4 #define LL long long
  5 using namespace std;
  6 const int N=1000002;
  7 int root, node_cnt;
  8 struct node
  9 {
 10     int pre, val, ch[2];
 11 }nod[N];
 12 
 13 int create_node(int v,int far)  //返回节点下标
 14 {
 15     nod[node_cnt].val=v;
 16     nod[node_cnt].pre=far;
 17     nod[node_cnt].ch[0]=0;
 18     nod[node_cnt].ch[1]=0;
 19     return node_cnt++;
 20 }
 21 
 22 void Rotate(int t, int d)       //d为方向,0是左旋，1是右
 23 {
 24     int far=nod[t].pre;
 25     int son=nod[t].ch[d];       //far的孩子
 26     int gra=nod[far].pre;       //far的父亲
 27 
 28     nod[son].pre=far;
 29     nod[t].pre=gra;
 30     nod[far].pre=t;
 31 
 32     nod[far].ch[d^1]=son;
 33     nod[t].ch[d]=far;
 34     nod[gra].ch[nod[gra].ch[1]==far]=t;
 35 }
 36 
 37 void Splay(int t,int goal)   //将t转为goal的任一孩子
 38 {
 39     while( nod[t].pre!=goal  )     //t还不是根
 40     {
 41         int f=nod[t].pre, g=nod[f].pre;
 42         if( g==goal )    Rotate( t, nod[f].ch[0]==t );    //父亲就是根s，旋1次
 43         else
 44         {
 45             int d1=(nod[f].ch[0]==t), d2=(nod[g].ch[0]==f);
 46             if( d1==d2 )    //两次同向旋转
 47             {
 48                 Rotate( f, d1);
 49                 Rotate( t, d1);
 50             }
 51             else            //两次反向旋转
 52             {
 53                 Rotate( t, d1);
 54                 Rotate( t, d2);
 55             }
 56         }
 57     }
 58     if(!goal)    root=t;        //时刻更新树根
 59 }
 60 
 61 int Insert(int t, int v)
 62 {
 63     if(v==nod[t].val)   return -1;
 64     int q=-1;
 65     if( v>nod[t].val )  //右边
 66     {
 67         if( nod[t].ch[1]==0 )   q=(nod[t].ch[1]=create_node(v, t));
 68         else                    q=Insert(nod[t].ch[1], v);
 69     }
 70     else                //左边
 71     {
 72         if( nod[t].ch[0]==0 )  q=(nod[t].ch[0]=create_node(v, t));
 73         else                   q=Insert(nod[t].ch[0], v);
 74     }
 75     return q;
 76 }
 77 int get_pre(int t, int d)   //求前驱和后继的，d=1代表求前驱
 78 {
 79     if(nod[t].ch[d])    return get_pre(nod[t].ch[d], d);
 80     return nod[t].val;
 81 }
 82 
 83 void init()
 84 {
 85     root=node_cnt=0;
 86     create_node(INF,0);       //0号点是不要的
 87 }
 88 int main()
 89 {
 90     freopen("input.txt", "r", stdin);
 91     int n;
 92     while(cin>>n)
 93     {
 94         init();
 95         int ans=0;
 96         for(int i=0,a=0; i<n; i++,a=0)
 97         {
 98             scanf("%d", &a);
 99             int t=Insert(root, a);
100             if(t<0)     continue;          //已经存在此数字
101             Splay(t, 0);                   //将t伸展一下到根
102             if(i==0)  ans=a;               //第一个数字
103             else if( nod[t].ch[0] && nod[t].ch[1] )
104                 ans+=min( abs(a-get_pre(nod[t].ch[0], 1)), abs(a-get_pre( nod[t].ch[1], 0)));
105             else if( nod[t].ch[0] )    ans+=abs( a-get_pre( nod[t].ch[0], 1) );
106             else if( nod[t].ch[1] )    ans+=abs( a-get_pre( nod[t].ch[1], 0) );
107 
108         }
109         cout<<ans<<endl;
110     }
111     return 0;
112 }
AC代码
```

