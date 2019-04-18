# HDU 2222 Keywords Search （AC自动机） - xcw0754 - 博客园
# [HDU 2222 Keywords Search （AC自动机）](https://www.cnblogs.com/xcw0754/p/4536870.html)
题意：
　　给一堆敏感单词，再给一篇文章，求此文章中出现几次敏感词。（多模式串匹配）
思路：
　　现在是多个模式串来匹配一篇文章，文章一般较长，如果每个串都来一次KMP的话，复杂度还是很高的，不如将文章拿来匹配这些敏感词，主要就是对这些敏感词进行处理，使得形成一棵类似于Trie树。
　　纯AC自动机的题目。以关键字建立trie树，设置好fail指针，就可以进行求出现次数了。 有几点需要注意：
　　（1）fail[t]表示的是一个点u，从root->u经过的串是root->t经过的串的后缀，比如u="asd"，t="qasd"，fail[t]=u，那么后缀是完全一样的。
　　（2）query时要注意统计全部符合条件的串，由于只是在每个模式串的最后一个节点设置tag，所以如果只顺着文章essay来走的话，可能漏统计了很多。比如有串"asd"和"sd"，而essay="asd"，那么你统计到的只是1个串，是错误的。正确做法是，根据essay串，每走到一个点t时（非root），顺着该点的fail[t]点，再fail[fail[t]]...逐个点统计tag，直到fail[x]=root为止。
```
1 #include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #include <cmath>
  6 #include <map>
  7 #include <algorithm>
  8 #include <vector>
  9 #include <iostream>
 10 #define pii pair<int,int>
 11 #define INF 0x7f3f3f3f
 12 #define LL long long
 13 #define ULL unsigned long long
 14 using namespace std;
 15 const double PI  = acos(-1.0);
 16 const int N=1000100;
 17 
 18 char essay[N];
 19 
 20 struct Trie
 21 {
 22     static const int NN=5e5+10;    //节点数
 23     int next[NN][26], fail[NN], tag[NN];    //tag标记每个模式串尾节点
 24     int root, node_cnt;
 25     int newnode()    //创建点
 26     {
 27         for(int i=0; i<26; i++)
 28             next[node_cnt][i]=-1;
 29         tag[node_cnt]=0;    //刚创建时，默认非叶子
 30         return node_cnt++;
 31     }
 32     void init()
 33     {
 34         node_cnt=0;
 35         root=newnode(); //root是虚拟点
 36     }
 37     void insert(char s[])
 38     {
 39         int len=strlen(s);
 40         int now=root;
 41         for(int i=0; i<len; i++)
 42         {
 43             if(next[now][s[i]-'a']==-1)
 44                 next[now][s[i]-'a']=newnode();
 45             now=next[now][s[i]-'a'];
 46         }
 47         tag[now]++; //尾节点:可能有多个相同模式串！
 48     }
 49 
 50     void buildAC()   //创建AC自动机：设置fail指针
 51     {
 52         queue<int> que;         //普通队列
 53         fail[root]=root;
 54         for(int i=0; i<26; i++)  //先将所有模式串头节点进队
 55         {
 56             if(next[root][i]==-1)
 57                 next[root][i]=root;
 58             else
 59             {
 60                 fail[next[root][i]]=root;
 61                 que.push(next[root][i]);
 62             }
 63         }
 64 
 65         while(!que.empty())
 66         {
 67             int now=que.front();que.pop();
 68             for(int i=0; i<26; i++)
 69             {
 70                 if( next[now][i]==-1)
 71                     next[now][i]=next[fail[now]][i];
 72                 else
 73                 {
 74                     fail[next[now][i]]=next[fail[now]][i];
 75                     que.push(next[now][i]);
 76                 }
 77             }
 78         }
 79     }
 80 
 81     int query(char s[])
 82     {
 83         int len=strlen(s);
 84         int now=root;
 85         int ans=0;
 86         for(int i=0; i<len; i++)
 87         {
 88             now=next[now][s[i]-'a'];    //取next
 89             int tmp=now;
 90             while( tmp!=root )    //顺着走完它，继续统计
 91             {
 92                 ans+=tag[tmp];
 93                 tag[tmp]=0;       //统计过的置0
 94                 tmp=fail[tmp];
 95             }
 96         }
 97         return ans;
 98     }
 99 }AC;
100 
101 int main()
102 {
103     //freopen("input.txt","r",stdin);
104     int t, n;cin>>t;
105     while(t--)
106     {
107         scanf("%d",&n);
108         AC.init();
109         for(int i=0; i<n; i++)  //构建字典树
110         {
111             scanf("%s",essay);
112             AC.insert(essay);
113         }
114         AC.buildAC();
115         scanf("%s",essay);
116         printf("%d\n", AC.query(essay));
117     }
118 
119     return 0;
120 }
AC代码
```

