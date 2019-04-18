# HYSBZ  1503  郁闷的出纳员 （Splay树） - xcw0754 - 博客园
# [HYSBZ  1503  郁闷的出纳员 （Splay树）](https://www.cnblogs.com/xcw0754/p/4747509.html)
题意：
　　作为一名出纳员，我的任务之一便是统计每位员工的工资。但是我们的老板反复无常，经常调整员工的工资。如果他心情好，就可能把每位员工的工资加上一个相同的量。反之，如果心情不好，就可能把他们的工资扣除一个相同的量。
　　工资的频繁调整很让员工反感，尤其是集体扣除工资的时候，一旦某位员工发现自己的工资已经低于了合同规定的工资下界，他就会立刻气愤地离开公司，并且再也不会回来了。
　　每位员工的工资下界都是统一规定的。每当一个人离开公司，我就要从电脑中把他的工资档案删去，同样，每当公司招聘了一位新员工，我就得为他新建一个工资档案。
　　老板经常到我这边来询问工资情况，他并不问具体某位员工的工资情况，而是问现在工资第k多的员工拿多少工资。
![](https://images0.cnblogs.com/blog2015/641737/201508/211159591911460.jpg)
思路：
　　（1）插入比较简单，只要允许重复值的存在就行了。但是下面的操作需要维护一个变化量add，所以插入之前要先减去add再插入。若不这样做，由于每次加工资只是针对当前的员工，而之前的加减工资并不没有针对新员工，所以新员工并不应该享受到老员工的加减工资福利。
　　（2）集体加工资，可以用一个全局变量统计当前所有员工的工资变化量add。
　　（3）扣工资同加工资一样，用同一个变量统计。但是有人可能会因为此次扣工资而离开公司，所以要及时清理掉这些人。方法是，如果当前员工有人的工资刚好等于min，那么将其伸展到根，再删除其左子树即可（注意，可能有多个人的工资等于min，那么你删除时，要保证根的左子树中并不存在工资为min的人，即需要将最左的min伸展到根）。否则，插入一个工资为min的点，再将其伸展到根，同样删除左子树，然后再删除自己。注意，所有人的工资得加上所维护的工资变化量。
　　（4）查询第k多比较简单，只要维护左子树的节点数量以及右子树的节点数量。如果在左子树中，则k要减去工资大于本节点的人数，再往下找。如果在右子树中，仍然用的是k来找。
 　　还有一点很神奇的地方就是，每次插入新元素都是插到叶子节点，但是插入过程我们不需要更新插入路径上面的点的左右孩子数量，因为一插入完毕之后立刻就splay到根了，这样子相当于插入之前的树自己在维护孩子数量而已，而新节点从叶子伸展到根自然会更新孩子数量了。所以只需要在rotate中维护孩子数量即可。
```
1 #include <bits/stdc++.h>
  2 #define pii pair<int,int>
  3 #define INF 0x7f7f7f7f
  4 #define LL long long
  5 using namespace std;
  6 const int N=1000002;
  7 int root, node_cnt, ans, add, del;
  8 struct node
  9 {
 10     int pre, val;
 11     int son[2]; //子树中有多少个节点。
 12     int ch[2];
 13 }nod[N];
 14 
 15 
 16 int create_node(int v,int far)  //返回节点下标
 17 {
 18     nod[node_cnt].val=v;
 19     nod[node_cnt].pre=far;
 20     nod[node_cnt].ch[0]=0;
 21     nod[node_cnt].ch[1]=0;
 22     nod[node_cnt].son[0]=0;
 23     nod[node_cnt].son[1]=0;
 24     return node_cnt++;
 25 }
 26 
 27 void init()
 28 {
 29     add=root=node_cnt=0;
 30     create_node(INF, 0);       //0号点是不要的
 31 }
 32 
 33 void Rotate(int t, int d)       //d为方向,0是左旋，1是右
 34 {
 35     int far=nod[t].pre;
 36     int son=nod[t].ch[d];       //far的孩子
 37     int gra=nod[far].pre;       //far的父亲
 38 
 39     nod[son].pre=far;
 40     nod[t].pre=gra;
 41     nod[far].pre=t;
 42 
 43     nod[far].ch[d^1]=son;
 44     nod[t].ch[d]=far;
 45     nod[gra].ch[nod[gra].ch[1]==far]=t;
 46 
 47     //子树中的节点要更新
 48     nod[far].son[d^1]=nod[t].son[d];
 49     nod[t].son[d]+=1+nod[far].son[d];   //别忘了还有far也是个节点
 50 }
 51 
 52 void Splay(int t,int goal)   //将t转为goal的任一孩子
 53 {
 54     while( nod[t].pre!=goal  )     //t还不是根
 55     {
 56         int f=nod[t].pre, g=nod[f].pre;
 57         if( g==goal )    Rotate( t, nod[f].ch[0]==t );    //父亲就是根s，旋1次
 58         else
 59         {
 60             int d1=(nod[f].ch[0]==t), d2=(nod[g].ch[0]==f);
 61             if( d1==d2 )    //两次同向旋转
 62             {
 63                 Rotate( f, d1);
 64                 Rotate( t, d1);
 65             }
 66             else            //两次反向旋转
 67             {
 68                 Rotate( t, d1);
 69                 Rotate( t, d2);
 70             }
 71         }
 72     }
 73     if(!goal)    root=t;        //时刻更新树根
 74 }
 75 
 76 int Insert(int t, int v)
 77 {
 78     int q=-1;       
 79     if( v>nod[t].val )  //右边
 80     {
 81         if( nod[t].ch[1]==0 )   q=(nod[t].ch[1]=create_node(v, t));
 82         else                    q=Insert(nod[t].ch[1], v);
 83     }
 84     else                //左边，相等时插左边
 85     {
 86         if( nod[t].ch[0]==0 )  q=(nod[t].ch[0]=create_node(v, t));
 87         else                   q=Insert(nod[t].ch[0], v);
 88     }
 89     return q;
 90 }
 91 
 92 
 93 int Find(int t,int v)   //找到值为v的，若没有，则返回-1
 94 {
 95     while( t )
 96     {
 97         if(nod[t].val==v)
 98         {
 99             int r=Find(nod[t].ch[0], v);   //找到最左边的那一个，即保证t的左子树中没有等于v的点。
100             if(r==-1)   return t;
101             else t=r;
102         }
103         if( nod[t].val<v )              //左边
104             t=nod[t].ch[0];
105         else
106             t=nod[t].ch[1];
107     }
108     return -1;
109 }
110 
111 void Delete(int t, int limit)  //将所有工资低于限额的，删去该子树。
112 {
113     //先找找看有没有等于这个值的。
114     int r=Find(root, limit-add);
115     if(r==-1)   //没有找到，则插入这样的值，Splay到顶，然后删去此点的左子树
116     {
117         Splay( Insert( root, limit-add ) , 0);
118         del+=nod[root].son[0];
119         int right=nod[root].ch[1];        //再删去此节点（即根）
120         if(right==0)    init();           //全部删完，没有员工
121         else    nod[right].pre=0, root=right;
122     }
123     else                    //找到了最左端的一个。
124     {
125         Splay(r, 0);        //旋转到顶，删去左子树。
126         del+=nod[root].son[0];
127         nod[root].son[0]=0;
128     }
129 }
130 
131 int Query(int t,int k)  //查找第k多
132 {
133     if( nod[t].son[0]+nod[t].son[1]+1<k )   return -1;  //整棵树都还没有k个
134     while( nod[t].son[1]!=k-1 )
135     {
136         if(nod[t].son[1]>k-1)    t=nod[t].ch[1];   //在右孩子中
137         else                                       //在左孩子中
138         {
139             k-=nod[t].son[1]+1;
140             t=nod[t].ch[0];
141         }
142     }
143     Splay(t, 0);
144     return add+nod[t].val;
145 }
146 
147 int main()
148 {
149     //freopen("input.txt", "r", stdin);
150     int n, limit, t;char c;
151     while(cin>>n>>limit)
152     {
153         init();
154         del=0;      //离开员工的人数
155         for(int i=0,a=0; i<n; i++,a=0)
156         {
157             while( !isalpha(c=getchar()))  ;
158             scanf("%d", &a);
159             if(c=='I' && a>=limit)       Splay( Insert(root, a-add), 0);   //插完就伸展。新员工要减掉个add。再伸展到根。
160             else if(c=='A')  add+=a;                           //全体加工资
161             else if(c=='S')  add-=a,Delete(root, limit);       //全体扣工资，有人可能因为此次扣工资而离开公司。
162             else if(c=='F')  printf("%d\n", Query(root, a));
163         }
164         printf("%d\n", del);
165     }
166     return 0;
167 }
168 
169 AC代码
AC代码
```

