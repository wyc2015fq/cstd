# HYSBZ  1208  宠物收养所 （Splay树） - xcw0754 - 博客园
# [HYSBZ  1208  宠物收养所 （Splay树）](https://www.cnblogs.com/xcw0754/p/4749303.html)
题意：一家宠物收养所负责处理领养者与遗弃宠物业务，有人来领宠物，则领一只最理想的。若没有宠物了，领养者们就得等到宠物来，宠物一来立刻送给其中一个等待者。如果有两个理想的选择，则选择那个值较小的。收养所时刻只存在人or宠物or没有存在任何！
思路：
　　这题主要是树中可能存在人或者宠物，但两者不可能同时存在，因为一旦存在，必须有对象被配对并从树中删除。 所有函数独自写起来都是很简单的，但是合起来使用时就得小心了，得配合一点。
　　要时刻注意树中是否还有节点，若没有，则可能要树要换种类了。
```
1 #include <bits/stdc++.h>
  2 #define pii pair<int,int>
  3 #define INF 0x3f7f7f7f
  4 #define LL long long
  5 using namespace std;
  6 const int N=40020;
  7 const int mod=1000000;
  8 
  9 struct node
 10 {
 11     int key, pre, ch[2];
 12 }nod[N];
 13 int node_cnt, root, flag;
 14 
 15 int create_node(int v,int far)
 16 {
 17     nod[node_cnt].ch[0]=nod[node_cnt].ch[1]=0;
 18     nod[node_cnt].pre=far;
 19     nod[node_cnt].key=v;
 20     return node_cnt++;
 21 }
 22 
 23 void Rotate(int t,int d)
 24 {
 25     int son=nod[t].ch[d];
 26     int far=nod[t].pre;
 27     int gra=nod[far].pre;
 28     nod[t].pre=gra;
 29     nod[far].pre=t;
 30     nod[son].pre=far;
 31     nod[t].ch[d]=far;
 32     nod[far].ch[d^1]=son;
 33     nod[gra].ch[ nod[gra].ch[1]==far ]=t;
 34 }
 35 
 36 
 37 int Insert(int t,int v)
 38 {
 39     if(t==0)    return  root=create_node(v, 0); //树中还没有节点。
 40     if( v < nod[t].key )
 41     {
 42         if(nod[t].ch[0])  return Insert(nod[t].ch[0], v);
 43         else              return nod[t].ch[0]=create_node(v, t);
 44     }
 45     else
 46     {
 47         if(nod[t].ch[1])  return Insert(nod[t].ch[1], v);
 48         else              return nod[t].ch[1]=create_node(v, t);
 49     }
 50 }
 51 
 52 void Splay(int t, int goal)
 53 {
 54     while(nod[t].pre!=goal)
 55     {
 56         int f=nod[t].pre, g=nod[f].pre;
 57         if(g==goal)    Rotate( t, nod[f].ch[0]==t );
 58         else
 59         {
 60             int d1=nod[f].ch[0]==t, d2=nod[g].ch[0]==f;
 61             if(d1==d2)    Rotate(f, d1),Rotate(t, d1);
 62             else          Rotate(t, d1),Rotate(t, d2);
 63         }
 64     }
 65     if(goal==0) root=t; //随时更新根！
 66 }
 67 
 68 int Find(int t,int v)   //在子树t中找到值为v的点，返回点号
 69 {
 70     while(t)
 71     {
 72         if(v==nod[t].key)   return t;
 73         if(v<nod[t].key)    t=nod[t].ch[0];
 74         else                t=nod[t].ch[1];
 75     }
 76     return 0;   //找不到
 77 }
 78 
 79 int Find_pre(int t,int v)
 80 {
 81     int val=-1;
 82     while(t)
 83     {
 84         if(nod[t].key<v)    val=max(val, nod[t].key);
 85         if(v<nod[t].key)    t=nod[t].ch[0];
 86         else                t=nod[t].ch[1];
 87     }
 88     if(val<0)   return INF;
 89     return val;
 90 }
 91 
 92 int Find_bac(int t,int v)
 93 {
 94     int val=INF;
 95     while(t)
 96     {
 97         if(nod[t].key>v)    val=min(val, nod[t].key);
 98         if(v<nod[t].key)    t=nod[t].ch[0];
 99         else                t=nod[t].ch[1];
100     }
101     return val;
102 }
103 
104 void Delete(int v)  //在树中删除值为v的任意一个点
105 {
106     Splay(Find(root, v), 0);   //将目标旋转到根
107     int L=nod[root].ch[0];
108     int R=nod[root].ch[1];
109     if(L==0 && R==0)    root=0;
110     else if(R==0)    //没有右子树
111     {
112         nod[L].pre=0;
113         root=L;
114     }
115     else if(L==0)    //没有左子树
116     {
117         nod[R].pre=0;
118         root=R;
119     }
120     else            //有前有后
121     {
122         while(nod[R].ch[0]) R=nod[R].ch[0]; //找到后继
123         Splay(R, root);
124         nod[R].ch[0]=L;
125         nod[R].pre=0;
126         nod[L].pre=R;
127         root=R;
128     }
129 }
130 
131 int main()
132 {
133     //freopen("input.txt", "r", stdin);
134     int n, ans, a, b, contain;
135     while(cin>>n)
136     {
137         flag=node_cnt=7;
138         contain=root=ans=0;
139         while(n--)
140         {
141             scanf("%d%d",&a,&b);
142             if(a==flag || contain==0)    // 树为空 or 树中是同种类 则 插入
143             {
144                 flag=a;
145                 contain++;
146                 Splay(Insert(root, b), 0);
147             }
148             else
149             {
150                 if(Find(root, b))    Delete(b); //刚好有值为b的。
151                 else
152                 {
153                     int L=Find_pre(root, b);
154                     int R=Find_bac(root, b);
155                     if( abs(L-b)<=abs(R-b) )      //L和R不可能同时为INF。
156                     {
157                         ans=(ans+abs(L-b))%mod;
158                         Delete(L);
159                     }
160                     else
161                     {
162                         ans=(ans+abs(R-b))%mod;
163                         Delete(R);
164                     }
165                 }
166                 contain--;
167             }
168         }
169         printf("%d\n", ans );
170     }
171     return 0;
172 }
AC代码
```

