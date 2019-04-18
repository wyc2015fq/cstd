# HDU 5285 wyh2000 and pupil (二分图着色) - xcw0754 - 博客园
# [HDU 5285 wyh2000 and pupil (二分图着色)](https://www.cnblogs.com/xcw0754/p/4657727.html)
题意：
　　共有n个小学生，编号为1−n。将所有小学生分成2组，每组都至少有1个人。但是有些小学生之间并不认识，而且如果a不认识b，那么b也不认识a。Wyh2000希望每组中的小学生都互相认识。而且第一组的人要尽可能多。请你帮wyh2000求出第一组和第二组的人数是多少。如果找不到分组方案，则输出"Poor wyh"。
思路：
　　二分图着色。给的就是无向图，每次都累加人多的颜色即可。若不能着色，必定不能分成2组。如果全部都是1个颜色，那么要让其中1人过第2组。我勒个去，就因为赭色时颜色号码开小了而错。
```
1 //#include <bits/stdc++.h>
  2 #include <cstdio>
  3 #include <cstring>
  4 #include <iostream>
  5 #include <map>
  6 #include <set>
  7 #include <deque>
  8 #include <algorithm>
  9 #include <vector>
 10 #define LL long long
 11 #define pii pair<int,int>
 12 #define INF 0x7f7f7f7f
 13 using namespace std;
 14 const int N=100000+100;
 15 
 16 vector<int> vect[N];
 17 int col[N];
 18 set<int> mapp;
 19 
 20 int color(int u, int f)
 21 {
 22     mapp.clear();
 23     deque<int> que;
 24     que.push_back(u);
 25     col[u]=f;
 26     mapp.insert(u);
 27     while(!que.empty())
 28     {
 29         int x=que.front();
 30         que.pop_front();
 31 
 32         for(int i=0; i<vect[x].size(); i++)
 33         {
 34             int tmp=vect[x][i];
 35             if(col[tmp]==col[x]) return 0;  //冲突
 36             if(col[tmp]==0)   //没色
 37             {
 38                 mapp.insert(tmp);//一个连同分量装进去
 39                 col[tmp]=1000000-col[x];
 40                 que.push_back(tmp);
 41             }
 42         }
 43     }
 44     return 1;
 45 }
 46 
 47 
 48 int cal(int n)
 49 {
 50     memset(col,0,sizeof(col));
 51     int big=0, small=0, k=0;
 52     for(int i=1; i<=n; i++)
 53     {
 54         if(!col[i])
 55         {
 56             if(!color(i, ++k)) return 0;
 57 
 58             //统计人数
 59             int a=0, b=0;
 60             set<int>::iterator it=mapp.begin();
 61             for(int j=0; j<mapp.size(); j++)
 62             {
 63                 if(col[*it]==k)   a++;
 64                 if(col[*it]==1000000-k) b++;
 65                 it++;
 66             }
 67 
 68             if(a<b) swap(a, b);
 69             big+=a;
 70             small+=b;
 71         }
 72     }
 73     if(!small)  return big-1;
 74     else return big;
 75 }
 76 
 77 
 78 
 79 int main()
 80 {
 81     freopen("input.txt", "r", stdin);
 82     int t, n, m, a, b;
 83     cin>>t;
 84     while(t--)
 85     {
 86         scanf("%d%d",&n,&m);
 87         for(int i=0; i<=n; i++)    vect[i].clear();
 88 
 89         for(int i=0; i<m; i++)
 90         {
 91             scanf("%d%d",&a,&b);
 92             vect[a].push_back(b);
 93             vect[b].push_back(a);
 94         }
 95         if(n<=1)
 96         {
 97              puts("Poor wyh");
 98              continue;
 99         }
100         else if(n==2)
101         {
102              puts("1 1");
103              continue;
104         }
105 
106         int tmp=cal(n);
107         if(tmp) printf("%d %d\n",tmp, n-tmp);
108         else  puts("Poor wyh");
109 
110 
111     }
112     return 0;
113 }
AC代码
```

