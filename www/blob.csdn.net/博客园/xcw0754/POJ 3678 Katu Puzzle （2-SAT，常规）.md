# POJ 3678 Katu Puzzle （2-SAT，常规） - xcw0754 - 博客园
# [POJ 3678 Katu Puzzle （2-SAT，常规）](https://www.cnblogs.com/xcw0754/p/4636504.html)
题意：给出n个点，每个点上有一个数字可以0或1，然后给出m条限制，要求a和b两个点上的数字满足 a op b = c，op和c都是给定。问是否能够有一组解满足所有限制？（即点上的数字是0是1由你决定）
思路：题意很清晰了，难点在建图。要考虑所有可能的冲突：
当op为and：　　（1）c为0时，其中1个必为0。
　　　　　　　　（2）c为1时，两者必为1。要加两条边，形如 a0->a1。
当op为or：　　　（1）c为0时，两者必为0。要加两条边，形如 a1->a0。
　　　　　　　　（2）c为1时，其中1个必为1。
当op为xor：　　（1）c为0时，两者必定相同。
　　　　　　　　（2）c为1时，两者必定不同。
都是按照冲突来建图就行，有没有解留给DFS去判定。
```
1 #include <iostream>
  2 #include <stdio.h>
  3 #include <string.h>
  4 #include <vector>
  5 #include <stack>
  6 #include <algorithm>
  7 #include <map>
  8 //#include <bits/stdc++.h>
  9 #define LL long long
 10 #define pii pair<int,int>
 11 #define INF 0x7f7f7f7f
 12 using namespace std;
 13 const int N=1000*2+5;
 14 int res[N][N];
 15 vector<int> vect[N*2];
 16 map<string,int> mapp;
 17 
 18 void init()
 19 {
 20     string tmp="AND";
 21     mapp[tmp]=1;
 22     tmp="OR";
 23     mapp[tmp]=2;
 24     tmp="XOR";
 25     mapp[tmp]=3;
 26 }
 27 int s[N*2], col[N*2], c;
 28 bool color(int x)
 29 {
 30     if(col[x^1])    return false;
 31     if(col[x])      return true;
 32     col[x]=2;
 33     s[c++]=x;
 34     for(int i=0; i<vect[x].size(); i++)
 35         if(!color(vect[x][i]))  return false;
 36     return true;
 37 }
 38 
 39 
 40 int cal(int n)
 41 {
 42     memset(col,0,sizeof(col));
 43     memset(s,0,sizeof(s));
 44     for(int i=0; i<n; i+=2)
 45     {
 46         if(!col[i]&&!col[i+1])
 47         {
 48             c=0;
 49             if(!color(i))
 50             {
 51                 while(c)    col[s[--c]]=0;
 52                 if(!color(i+1)) return false;
 53             }
 54         }
 55     }
 56     return true;
 57 }
 58 
 59 int main()
 60 {
 61     freopen("input.txt", "r", stdin);
 62     init();
 63     string op;
 64     int n, m, a, b, c;
 65     while(~scanf("%d%d",&n,&m))
 66     {
 67         memset(res,0xf0,sizeof(res));
 68         for(int i=n*2; i>=0; i--)   vect[i].clear();
 69 
 70         for(int i=0; i<m; i++)
 71         {
 72             scanf("%d%d%d",&a,&b,&c);
 73             cin>>op;
 74             res[a][b]=c;
 75             int t=mapp[op];
 76             //设i*2为0，i*2+1为1
 77             if(t==1)    //and
 78             {
 79                 if(c==0)    //其中必有1个为0
 80                 {
 81                     vect[a*2+1].push_back(b*2);
 82                     vect[b*2+1].push_back(a*2);
 83 
 84                 }
 85                 else        //两者必为1
 86                 {
 87                     vect[a*2].push_back(a*2+1); //指向自己
 88                     vect[b*2].push_back(b*2+1);
 89                 }
 90             }
 91             else if(t==2)   //or
 92             {
 93                 if(c==0)    //两者必为0
 94                 {
 95                     vect[a*2+1].push_back(a*2);
 96                     vect[b*2+1].push_back(b*2);
 97                 }
 98                 else        //其中必有1个为1
 99                 {
100                     vect[a*2].push_back(b*2+1);
101                     vect[b*2].push_back(a*2+1);
102                 }
103             }
104             else        //XOR
105             {
106                 if(c==0)    //两者必定相同
107                 {
108                     vect[a*2].push_back(b*2);
109                     vect[b*2].push_back(a*2);
110                     vect[a*2+1].push_back(b*2+1);
111                     vect[b*2+1].push_back(a*2+1);
112                 }
113                 else        //两者必定不同
114                 {
115                     vect[a*2].push_back(b*2+1);
116                     vect[a*2+1].push_back(b*2);
117                     vect[b*2].push_back(a*2+1);
118                     vect[b*2+1].push_back(a*2);
119                 }
120             }
121         }
122         if(!cal(n<<1))    puts("NO");
123         else    puts("YES");
124     }
125     return 0;
126 }
AC代码
```

