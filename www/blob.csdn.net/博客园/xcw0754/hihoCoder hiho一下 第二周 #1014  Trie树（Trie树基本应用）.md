# hihoCoder hiho一下 第二周 #1014 : Trie树（Trie树基本应用） - xcw0754 - 博客园
# [hihoCoder hiho一下 第二周 #1014 : Trie树（Trie树基本应用）](https://www.cnblogs.com/xcw0754/p/4443062.html)
思路：
　　完全看题目中的介绍就行了。还有里面的input写道：不保证是英文单词，也有可能是火星文单词哦。比赛结束后的提交是不用考虑26个字母之外的，都会AC，如果考虑128种可能的话，爆了内存。步骤就是，在插单词的同时记录该结点之后的单词数，查词就查最后一个字母所在结点上的单词数。
```
1 #include <iostream>
 2 #include <cstring>
 3 #include <vector>
 4 #include <stdio.h>
 5 using namespace std;
 6 const int N=26;
 7 char dict[15];
 8 int n, m;
 9 struct node
10 {
11     int num;    //以本节点开头的单词个数
12     node *child[N];  //孩子
13 }tree_gen;
14 
15 int check_dict(char *p)
16 {
17     node *node_p=&tree_gen;  //指向树根
18     int ans=0;
19     while(*p!='\0')
20     {
21         if(node_p->child[*p-'a']!=0)
22         {
23             node_p=node_p->child[*p-'a'];
24             ans=node_p->num;
25         }
26         else
27             return 0;
28         p++;
29     }
30     return ans;
31 }
32 
33 int insert_tree(char *p)
34 {
35     node *node_p=&tree_gen;  //指向树根
36     node_p->num++;           //访问这个节点
37     while(*p!='\0')
38     {
39         if( node_p->child[*p-'a']==0 )  //还没有这叉，就要建
40         {
41             node *new_node=new(node);   //创建新节点
42             for(int i=0; i<N; i++)
43                 new_node->child[i]=0;
44             new_node->num=0;
45             node_p->child[*p-'a']=new_node; //连接工作
46             node_p=new_node;
47         }
48         else                        //已有这叉，继续往下
49             node_p=node_p->child[*p-'a'];
50         node_p->num++;      //访问这个节点
51         p++;
52     }
53     return 0;
54 }
55 
56 int main()
57 {
58     //freopen("e:input.txt", "r", stdin);
59     tree_gen.num=0;  //树根的初始化
60     for(int i=0; i<N; i++)  tree_gen.child[i]=0;
61 
62     cin>>n;getchar();
63     for(int i=0; i<n; i++)      //接受字典,顺便插入，顺便记录num
64     {
65         gets(dict);
66         insert_tree(dict);
67     }
68     cin>>m;getchar();
69     for(int i=0; i<m; i++)      //查询以xxx开头的单词个数
70     {
71         gets(dict);
72         cout<<check_dict(dict)<<endl;
73     }
74     return 0;
75 }
AC代码
```
还有一种不是以链表实现的树，而是用数组来做的。小Ho做的这代码可以过一些小一点的数据。
```
1 #include <cstdio>
 2 #include <cstring>
 3 
 4 const int MAXL = 10 + 2;
 5 const int MAXN = 1000 + 10;
 6 const int MAXM = 100000 + 10;
 7 
 8 int next[MAXN * MAXL][26], count[MAXN * MAXL];
 9 
10 int main() {
11     int n;
12     while (scanf("%d", &n) != EOF) {
13         int top = 1; memset(next, 0, sizeof(next));
14         char str[MAXL];
15         for (int i = 0; i < n; i++) {
16             scanf("%s", str);
17             int p = 1;
18             for (int j = 0; str[j] != '\0'; j ++) {
19                 if (next[p][str[j] - 'a'] == 0) next[p][str[j] - 'a'] = ++top;
20                 p = next[p][str[j] - 'a'];
21                 count[p] ++;
22             }
23         }
24         int m;
25         scanf("%d", &m);
26         while (m --) {
27             scanf("%s", str);
28             int p = 1;
29             for (int j = 0; str[j] != '\0'; j++) {
30                 p = next[p][str[j] - 'a'];
31             }
32             printf("%d\n", count[p]);
33         }
34     }
35 }
Not AC代码
```

