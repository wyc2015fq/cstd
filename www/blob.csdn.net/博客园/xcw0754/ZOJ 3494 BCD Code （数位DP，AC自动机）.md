# ZOJ  3494  BCD Code （数位DP，AC自动机） - xcw0754 - 博客园
# [ZOJ  3494  BCD Code （数位DP，AC自动机）](https://www.cnblogs.com/xcw0754/p/4857334.html)
题意：
　　将一个整数表示成4个bit的bcd码就成了一个01串，如果该串中出现了部分病毒串，则是危险的。给出n个病毒串（n<=100,长度<21），问区间[L,R]中有几个数字是不含病毒串的（结果需要取模）？（0<L<=R<=10200）
思路：
　　区间非常大，怎样暴力统计都是不科学的。首先确定状态，按传统，一维必定是位数，二维就是压缩的状态了，如果长度为20个bit的话，200*104万的数组是不行的。类似多模式串匹配问题，病毒串可以构建成AC自动机，那么每个点可以代表一个独立状态，而n<=100，所以最多20n个节点，是可以的。转移的话可以根据新考虑的数位是多少，然后在AC自动机上面走4步（BCD码是4bit）到达另一个状态（点），如果经过了病毒串的末尾节点，表示该数出现病毒串，就不能转移。这个可以在AC自动机创建完成后，预处理出来就行了。而对于每个询问[L,R]，L仍然是需要减1的，大数减1比较简单。注意点是，AC自动机上的tag需要特殊处理，如果有病毒串"asdf"和串"sd"，而碰到原串为"asdd"，别忘了还有"sd"。这只需要在构建fail指针的时候处理一下。
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
 16 const int N=210;
 17 const LL mod=1000000009;
 18 
 19 struct Trie{
 20     static const int NN=2100;    //节点数
 21     static const int CC=2;    //孩子数
 22     int next[NN][CC], fail[NN];
 23     bool tag[NN];
 24     int root, node_cnt;
 25     int newnode(){
 26         for(int i=0; i<CC; i++)    next[node_cnt][i]=-1;
 27         tag[node_cnt]=false;    //刚创建时，默认非叶子
 28         return node_cnt++;
 29     }
 30     void init(){
 31         node_cnt=0;
 32         root=newnode(); //root是虚拟点
 33     }
 34     void insert(char s[]){
 35         int len=strlen(s);
 36         int now=root;
 37         for(int i=0; i<len; i++){
 38             if(next[now][s[i]-'0']==-1)
 39                 next[now][s[i]-'0']=newnode();
 40             now=next[now][s[i]-'0'];
 41         }
 42         tag[now]=true; //尾节点:可能有多个相同模式串！
 43     }
 44     void buildAC(){
 45         fail[root]=root; queue<int> que;
 46         for(int i=0; i<CC; i++){
 47             if(next[root][i]==-1)
 48                 next[root][i]=root;
 49             else{
 50                 fail[next[root][i]]=root;
 51                 que.push(next[root][i]);
 52             }
 53         }
 54         while(!que.empty()){
 55             int now=que.front();que.pop();
 56             if(tag[fail[now]]==true) tag[now]=true;   //注意
 57             for(int i=0; i<CC; i++){
 58                 if( next[now][i]==-1)
 59                     next[now][i]=next[fail[now]][i];
 60                 else{
 61                     fail[next[now][i]]=next[fail[now]][i];
 62                     que.push(next[now][i]);
 63                 }
 64             }
 65         }
 66     }
 67 }AC;
 68 
 69 LL f[N][N*10];
 70 int bcd[N*10][10], len;
 71 char bit[N];
 72 
 73 LL dfs(int i,int s,int sum,bool e)  //s是节点编号
 74 {
 75     if(i==0)         return 1;
 76     if(!e&&~f[i][s]) return f[i][s];
 77 
 78     LL ans=0;
 79     if(sum==0)  //处理前缀0
 80     {
 81         ans+=dfs(i-1, s, 0, e&&bit[i]=='0');
 82         ans%=mod;
 83     }
 84 
 85     int d= sum>0? 0: 1;     //起
 86     int u= e? bit[i]-'0': 9;//终
 87     for(; d<=u; d++)
 88     {
 89         if(bcd[s][d]!=-1)
 90         {
 91             ans+=dfs(i-1, bcd[s][d], sum+d, e&&d==u);
 92             ans%=mod;
 93         }
 94     }
 95     if(!e&&sum) f[i][s]=ans;    //没有前导零
 96     return ans;
 97 }
 98 
 99 
100 LL cal()
101 {
102     reverse(bit+1, bit+len+1);
103     if(len==1&&bit[len]=='0')   return 1;
104     return dfs(len, 0, 0, true);
105 }
106 
107 int changeto(int s,int t)
108 {
109     if(AC.tag[s])   return -1;  //已经是病毒串
110     int now=s;
111     for(int i=3; i>=0; i--)
112     {
113         if( AC.tag[AC.next[now][(t>>i)&1]]==1 ) return -1; //病毒串
114         now=AC.next[now][(t>>i)&1];
115     }
116     return now;
117 }
118 void pre_cal()  //预处理转移
119 {
120     for(int i=0; i<AC.node_cnt; i++)
121         for(int j=0; j<10; j++)
122             bcd[i][j]=changeto(i,j);
123 }
124 int main()
125 {
126     freopen("input.txt","r",stdin);
127     int t, n;cin>>t;
128     LL  ans[2];
129     while( t-- )
130     {
131         memset(bcd, -1, sizeof(bcd));
132         memset(f, -1, sizeof(f));
133         AC.init();
134         scanf("%d",&n);
135         for(int i=0; i<n; i++)
136         {
137             scanf("%s",bit);
138             AC.insert(bit);
139         }
140         AC.buildAC();  //AC自动机
141         pre_cal();     //预处理转移
142 
143         for(int j=0; j<2; j++)
144         {
145             scanf("%s", bit+1);
146             len=strlen(bit+1);
147             if(j==0)
148             {
149                 for(int i=len; i>0; i--)    //注意逆序
150                 {
151                     if( bit[i]>'0' ){bit[i]--;break;}
152                     else            bit[i]='9';
153                 }
154             }
155             ans[j]=cal();
156         }
157         printf("%lld\n",(ans[1]+mod-ans[0])%mod);
158     }
159     return 0;
160 }
AC代码
```

