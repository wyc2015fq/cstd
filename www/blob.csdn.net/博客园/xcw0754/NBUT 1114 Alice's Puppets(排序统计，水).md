# NBUT 1114  Alice's Puppets(排序统计，水) - xcw0754 - 博客园
# [NBUT 1114  Alice's Puppets(排序统计，水)](https://www.cnblogs.com/xcw0754/p/4538716.html)
题意：给一棵人名树，按层输出，同层则按名字的字典序输出。
思路：首先对每个人名做索引，确定其在哪一层，按层装进一个set，再按层输出就自动排好序了。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 vector< set<string> > ord;    //每层一个set，自动按照字典序排好了
 4 map<string,int>   mapp;    //作哈希用
 5 
 6 int main()
 7 {
 8     //freopen("input.txt", "r", stdin);
 9     int n;
10     string son,far;
11     set<string> tmp;
12     while(cin>>n)
13     {
14         ord.clear();
15         for(int i=0; i<=n; i++)    ord.push_back(tmp);  //初始化
16         mapp.clear();
17         mapp["Alice"]=0;
18         int k=0;
19         for(int i=0; i<n; i++)
20         {
21             cin>>son>>far;
22             k=mapp[far];
23             mapp[son]=++k;//son的层数是far结点的层数+1
24             ord[k].insert(son);
25         }
26 
27         for(int i=1; i<=n; i++)
28         {
29             if(ord[i].empty()==true)    break;
30             else
31             {
32                 for(set<string>::iterator it=ord[i].begin(); it!=ord[i].end(); it++)
33                     cout<<*it<<" "<<i<<endl;
34             }
35         }
36     }
37 
38     return 0;
39 }
AC代码
```

