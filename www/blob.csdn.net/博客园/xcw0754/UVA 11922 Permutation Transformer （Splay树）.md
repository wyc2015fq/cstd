# UVA 11922  Permutation Transformer （Splay树） - xcw0754 - 博客园
# [UVA 11922  Permutation Transformer （Splay树）](https://www.cnblogs.com/xcw0754/p/4750280.html)
题意：
　　给一个序列，是从1~n共n个的自然数，接下来又m个区间，对于每个区间[a,b]，从第a个到第b个从序列中分离出来，翻转后接到尾部。输出最后的序列。
思路：
　　这次添加了Split和Merge两个基本操作，还有个比较困难的翻转操作。翻转操作只需要将需要翻转的序列独立成树，给根加上翻转标记之后再直接插到另外由前后两棵树组成的树上。但是在做一些操作的时候可能会遇到已经标记了翻转的子树，比如splay时，如果不顾flip标记，直接带flip标记的点伸展到根，会就会跟其他没有标记的节点混合了，而一个点如果带flip标记，其实标记的是它的两个孩子是需要翻转，此时如果来个无标记的孩子替换了某个孩子，就会造成错误。所以必须在splay之前完成翻转。
```
1 #include <bits/stdc++.h>
  2 #define pii pair<int,int>
  3 #define INF 0x3f7f7f7f
  4 #define LL long long
  5 using namespace std;
  6 const int N=100200;
  7 const int mod=10007;
  8 
  9 struct node
 10 {
 11     int key, pre, flip, ch[2], son[2];
 12 }nod[N];
 13 int  n, m, a, b, node_cnt, root;
 14 
 15 int create_node(int v,int far)
 16 {
 17     nod[node_cnt].key=v;
 18     nod[node_cnt].pre=far;
 19     nod[node_cnt].flip=0;
 20     nod[node_cnt].ch[0]=0;
 21     nod[node_cnt].ch[1]=0;
 22     nod[node_cnt].son[0]=0;
 23     nod[node_cnt].son[1]=0;
 24     return node_cnt++;
 25 }
 26 
 27 void push_down(int t)
 28 {
 29     if(nod[t].flip)
 30     {
 31         swap(nod[t].ch[0],  nod[t].ch[1]);
 32         swap(nod[t].son[0], nod[t].son[1]);
 33         nod[t].flip=0;
 34         int L=nod[t].ch[0], R=nod[t].ch[1];
 35         if(L)   nod[L].flip=!nod[L].flip;
 36         if(R)   nod[R].flip=!nod[R].flip;
 37     }
 38 }
 39 
 40 void Rotate(int t,int d)
 41 {
 42     int son=nod[t].ch[d];
 43     int far=nod[t].pre;
 44     int gra=nod[far].pre;
 45 
 46     nod[son].pre=far;
 47     nod[far].pre=t;
 48     nod[t].pre=gra;
 49 
 50     nod[t].ch[d]=far;
 51     nod[far].ch[d^1]=son;
 52     nod[gra].ch[ nod[gra].ch[1]==far ]=t;
 53 
 54     nod[far].son[d^1]=nod[t].son[d];
 55     nod[t].son[d]+=nod[far].son[d]+1;
 56 }
 57 
 58 int Insert(int t,int v)
 59 {
 60     if(t==0)    return root=create_node(v, 0);
 61     if( v<nod[t].key )
 62     {
 63         if(nod[t].ch[0])    return Insert(nod[t].ch[0], v);
 64         else                return nod[t].ch[0]=create_node(v, t);
 65     }
 66     else
 67     {
 68         if(nod[t].ch[1])    return Insert(nod[t].ch[1], v);
 69         else                return nod[t].ch[1]=create_node(v, t);
 70     }
 71 }
 72 
 73 void Splay(int t,int goal)
 74 {
 75     while(nod[t].pre!=goal)
 76     {
 77         int f=nod[t].pre, g=nod[f].pre;
 78         if(g==goal)    Rotate(t, nod[f].ch[0]==t);
 79         else
 80         {
 81             int d1=nod[f].ch[0]==t, d2=nod[g].ch[0]==f;
 82             if(d1==d2)  Rotate(f, d1),Rotate(t, d1);
 83             else        Rotate(t, d1),Rotate(t, d2);
 84         }
 85     }
 86     if(!goal)   root=t;
 87 }
 88 
 89 int Find(int t,int k)   //找第k个元素，必须能找到。这里不处理出错。
 90 {
 91     push_down(t);               //找的时候顺便往下推。
 92     if( nod[t].son[0]+1==k )    return t;
 93     if( k<nod[t].son[0]+1 )     return Find(nod[t].ch[0], k);
 94     else    return Find(nod[t].ch[1], k-nod[t].son[0]-1);
 95 }
 96 
 97 void Split(int t, int k, int &L, int &R)    //在以t为根的树中将[1,k]分离出来，变成两棵树L和R。
 98 {
 99     Splay( Find(t, k), 0);   //查找第k个元素，并伸展到根
100     L=root;
101     R=nod[L].ch[1];
102     nod[L].son[1]=0;
103     nod[L].ch[1]=0;
104     nod[R].pre=0;
105 }
106 
107 int Merge(int L,int R)
108 {
109     int k=nod[L].son[0]+nod[L].son[1]+1;
110     Splay( Find(L, k), 0 );         //在用Splay时，必须先往下推。
111     L=root;
112     nod[L].ch[1]=R;
113     nod[R].pre=L;
114     nod[L].son[1]=nod[R].son[0]+1+nod[R].son[1];
115     return L;
116 }
117 
118 void cal(int a,int b)   //从树中取出[a,b]然后旋转，插到尾部
119 {
120     int left=0, mid=0, right=0;
121     if(a!=1 && b!=n)    //三段
122     {
123         Split(root,  a-1, left, right);
124         Split(right, b-a+1, mid, right );
125         nod[mid].flip^=1;
126         root=Merge(Merge( left, right ) , mid );
127     }
128     else if(a==1)       //中后段
129     {
130         Split(root, b, mid, right );
131         nod[mid].flip^=1;
132         root=Merge( right, mid );
133     }
134     else if(b==n)       //前中段
135     {
136         Split(root, a-1, left, mid);
137         nod[mid].flip^=1;
138         root=Merge(left, mid );
139     }
140 }
141 
142 void print(int t)    //深搜输出
143 {
144     push_down(t);   //要往下推
145     if(nod[t].ch[0])    print(nod[t].ch[0]);
146     printf("%d\n", nod[t].key);
147     if(nod[t].ch[1])    print(nod[t].ch[1]);
148 }
149 
150 int main()
151 {
152     //freopen("input.txt", "r", stdin);
153     node_cnt=3;root=0;
154     cin>>n>>m;
155     for(int i=1; i<=n; i++)    Splay(Insert(root, i), 0);
156 
157     for(int i=0; i<m; i++)
158     {
159         scanf("%d%d",&a,&b);
160         if(a==1 && b==n)  nod[root].flip^=1;    //整个区间都翻转
161         cal(a,b);
162     }
163     print(root);
164     return 0;
165 }
AC代码
```

