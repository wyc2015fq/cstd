# HDU 5384 Danganronpa （Trie树） - xcw0754 - 博客园
# [HDU 5384 Danganronpa （Trie树）](https://www.cnblogs.com/xcw0754/p/4727897.html)
题意：给出两个集合S和T，集合中每个元素是个字符串，而T集合中任一元素都是个子弹，可以打S中的任一怪物，如果子弹是怪物的子串，那么才有伤害值1，若在怪物中出现多次，次数为该子弹打到该怪物的伤害值。每个子弹可以射不同怪物分别一次。求用完所有子弹，每个怪物受到的伤害值。
思路：先将所有子弹插到Trie树中。穷举每个怪物，将其所有的子串在Trie树中找，统计出现的次数，并输出。
　　（1）插子弹时在结点中记录以该结点为结束的有多少个子弹。
　　（2）每个怪物只需要截取size个str[i...end]在树中查找。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=100010;
 5 
 6 int n, m;
 7 string str[N];
 8 char s[N];
 9 
10 struct node
11 {
12     int num;    //以本节点结尾的单词个数
13     node *child[26];  //孩子
14 } pos[N*50];
15 node *tree_gen;
16 
17 int node_cnt;
18 node * create_node()
19 {
20     pos[node_cnt].num=0;
21     for(int i=0; i<26; i++) pos[node_cnt].child[i]=0;
22     return &pos[node_cnt++];
23 }
24 
25 void insert_tree(char *p)
26 {
27     node *node_p=tree_gen;  //指向树根
28 
29     while(*p!='\0')
30     {
31         if( node_p->child[*p-'a']==0 )  //还没有这叉，就要建
32         {
33             node *new_node=create_node();   //创建新节点
34             node_p->child[*p-'a']=new_node; //连接
35             node_p=new_node;
36         }
37         else                        //已有这叉，继续往下
38             node_p=node_p->child[*p-'a'];
39 
40         if( *(p+1)=='\0' )        node_p->num++;      //以此单词为结尾的
41         p++;
42     }
43 }
44 
45 int check(string t)
46 {
47     int ans=0;
48     node *node_p=tree_gen;  //指向树根
49     int r=0;
50     while( node_p!=0 && r<t.size() )
51     {
52         if(node_p->child[ t[r]-'a' ])
53             node_p=node_p->child[ t[r]-'a' ];
54         else break;
55         r++;
56         ans+=node_p->num;
57     }
58     return ans;
59 }
60 
61 int cal(string &t)
62 {
63     int ans=0;
64     for(int i=0; i<t.size(); i++)
65     {
66         ans+=check(t.substr(i));
67     }
68     return ans;
69 }
70 
71 
72 int main()
73 {
74     //freopen("input.txt", "r", stdin);
75     int t;
76     cin>>t;
77     while(t--)
78     {
79         scanf("%d%d", &n, &m);
80         node_cnt=0;
81         tree_gen=create_node();
82 
83         for(int i=0; i<n; i++)
84         {
85             cin>>str[i];
86         }
87         for(int i=0; i<m; i++)
88         {
89             scanf("%s",s);
90             insert_tree(s);
91         }
92         for(int i=0; i<n; i++)
93         {
94             printf("%d\n", cal(str[i]));
95         }
96     }
97     return 0;
98 }
AC代码
```

