# UVA 11020 Efficient Solutions （BST，Splay树） - xcw0754 - 博客园
# [UVA 11020 Efficient Solutions （BST，Splay树）](https://www.cnblogs.com/xcw0754/p/4750638.html)
题意：给n个坐标。一个坐标(x,y)若有无存在的坐标满足**x1<x && y1<=y**  或  **x1<=x && y1<y** 时，此坐标(x,y)是就是有优势的。在给每一个坐标之后，立刻输出当前有优势的坐标有多少个。
思路：Set可以做，但是我用Splay树实现也不难。观察题意中的不等式发现，一个点(x,y)的左下方不能有点（相当于跟(0,0)组成的矩形中不能有其他点，除了(x,y)之外），但是若有相同的点的存在，这些相同点都是优势点。
　　用Splay维护剩下的全部优势点，对于新来的一个点，如果它本来就被其他已经存在的优势点所淘汰，则不必插入树了。若没有被淘汰，插入它后，它有可能会淘汰其他点，如果我们根据x坐标来排序的话，有可能被淘汰的点就在新插入的点的右边，只要右边有点(x1,y1)满足y1>=y就会淘汰掉该点。
```
1 #include <bits/stdc++.h>
  2 #define pii pair<int,int>
  3 #define INF 0x3f7f7f7f
  4 #define LL long long
  5 using namespace std;
  6 const int N=30200;
  7 const int mod=10007;
  8 
  9 struct node
 10 {
 11     int key, y, pre, flip, ch[2], son[2];
 12 }nod[N];
 13 int  n, node_cnt, root;
 14 
 15 int create_node(int v,int y,int far)    //以x来排序即可。
 16 {
 17     nod[node_cnt].key=v;
 18     nod[node_cnt].y=y;
 19     nod[node_cnt].pre=far;
 20     nod[node_cnt].flip=0;
 21     nod[node_cnt].ch[0]=0;
 22     nod[node_cnt].ch[1]=0;
 23     nod[node_cnt].son[0]=0;
 24     nod[node_cnt].son[1]=0;
 25     return node_cnt++;
 26 }
 27 
 28 void Rotate(int t,int d)
 29 {
 30     int son=nod[t].ch[d];
 31     int far=nod[t].pre;
 32     int gra=nod[far].pre;
 33 
 34     nod[son].pre=far;
 35     nod[far].pre=t;
 36     nod[t].pre=gra;
 37 
 38     nod[t].ch[d]=far;
 39     nod[far].ch[d^1]=son;
 40     nod[gra].ch[ nod[gra].ch[1]==far ]=t;
 41 
 42     nod[far].son[d^1]=nod[t].son[d];
 43     nod[t].son[d]+=nod[far].son[d]+1;
 44 }
 45 
 46 int Insert(int t,int v,int y)
 47 {
 48     if(t==0)    return root=create_node(v, y, 0);
 49     if( v==nod[t].key )     //x相同，必有人被淘汰
 50     {
 51         if(nod[t].y<y)      return 0;           //此人无效
 52         if(nod[t].ch[1])    return Insert(nod[t].ch[1], v, y);            //插在其前面
 53         else                return nod[t].ch[1]=create_node(v, y, t);
 54     }
 55     else if( v<nod[t].key )
 56     {
 57         if(nod[t].ch[0])    return Insert(nod[t].ch[0], v, y);
 58         else                return nod[t].ch[0]=create_node(v, y, t);
 59     }
 60     else
 61     {
 62         if(nod[t].ch[1])    return Insert(nod[t].ch[1], v, y);
 63         else                return nod[t].ch[1]=create_node(v, y, t);
 64     }
 65 }
 66 
 67 void Splay(int t,int goal)
 68 {
 69     while(nod[t].pre!=goal)
 70     {
 71         int f=nod[t].pre, g=nod[f].pre;
 72         if(g==goal)    Rotate(t, nod[f].ch[0]==t);
 73         else
 74         {
 75             int d1=nod[f].ch[0]==t, d2=nod[g].ch[0]==f;
 76             if(d1==d2)  Rotate(f, d1),Rotate(t, d1);
 77             else        Rotate(t, d1),Rotate(t, d2);
 78         }
 79     }
 80     if(!goal)   root=t;
 81 }
 82 
 83 
 84 int Find_del_node(int t,int y)
 85 {
 86     if(t==0)            return 0;   //无后继了
 87     if(nod[t].y>=y )    return t;
 88     int q;
 89     if(nod[t].ch[0] && (q=Find_del_node(nod[t].ch[0], y)) )    return q;
 90     if(nod[t].ch[1] && (q=Find_del_node(nod[t].ch[1], y)) )    return q;
 91     return 0;
 92 }
 93 
 94 int Find_bac(int t,int d)   //这个可以找前驱也可以找后继。d为1则前驱。
 95 {
 96     if(t==0)            return 0;
 97     if(nod[t].ch[d]==0) return t;
 98     else    return Find_bac(nod[t].ch[d], d);
 99 }
100 
101 void Delete(int t)  //专门删除根节点
102 {
103     int L=nod[t].ch[0], R=nod[t].ch[1];
104     if(R==0)    //无后继
105     {
106         root=L;
107         nod[root].pre=0;
108     }
109     else        //找后继
110     {
111         R=Find_bac(R, 0);
112         Splay( R, root);
113         nod[R].ch[0]=L;
114         nod[R].pre=0;
115         nod[R].son[0]=nod[L].son[0]+1+nod[L].son[1];
116         nod[L].pre=R;
117         root=R;
118     }
119 }
120 
121 
122 int main()
123 {
124     //freopen("input.txt", "r", stdin);
125     int t, k, a, b, ans, Case=0;
126     cin>>t;
127     while(t--)
128     {
129         printf("Case #%d:\n", ++Case);
130         node_cnt=3;
131         root=0;
132         scanf("%d",&n);
133         for(int i=0; i<n; i++)
134         {
135             scanf("%d%d", &a, &b);
136             int ok=Insert(root, a, b);
137             if(ok!=0)       //碰到x相同的，而且y又比它大，那么直接无效。但是并不是一定是优势点。
138             {
139                 Splay(ok, 0);                           //插入成功，先伸展到根。
140                 int r=Find_bac(nod[root].ch[0], 1);      //找前驱
141                 if( r && nod[r].key<a && b>=nod[r].y)   Delete(root);    //新点为非优势点
142                 else
143                 {
144                     while( (k=Find_del_node(nod[root].ch[1], b))!=0  ) //从右边开始，找到一个删一个。
145                     {
146                         Splay(k, 0);
147                         Delete(root);
148                         Splay(ok, 0);
149                     }
150                 }
151             }
152             printf("%d\n", nod[root].son[0]+1+nod[root].son[1]);    //输出树上有多少点
153         }
154         if(t)   printf("\n");
155     }
156     return 0;
157 }
AC代码
```

