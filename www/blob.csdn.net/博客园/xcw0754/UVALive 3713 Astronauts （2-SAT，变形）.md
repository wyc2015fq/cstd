# UVALive 3713 Astronauts （2-SAT，变形） - xcw0754 - 博客园
# [UVALive 3713 Astronauts （2-SAT，变形）](https://www.cnblogs.com/xcw0754/p/4636160.html)
题意：
　　有A,B,C三种任务，每个人必获得1个任务，大于等于平均年龄的可以选择A和C，小于平均年龄的可以选择B和C。这些人有一些是互相讨厌的，必须不能执行同任务，问能否安排他们工作？若行，输出任意一组解。
思路：
　　依然是 2-SAT，只不过换了个样子，建图时不同而已。这里每个人依然有2人选择，也有冲突的出现，问题在如何找出冲突。
　　首先，无论是哪两人，只要互相讨厌，去抢C，必定冲突。其次，如果是同龄人（同大于等于，或同小于），那么抢他们那个年龄段的任务也会冲突。所以共计2种，每种2条边，即我选的时候，你不能选；你选的时候，我不能选。
　　在建好图后，进行DFS进行尝试一组可行解，所有尝试都不行时，就No Solution。有解时，如果2*i为true，那么输出时要判断这个人的年龄，给他分配对应年龄段的任务；如果i*2+1为true，那么这个人是选C的，不关年龄事直接输出。
```
1 #include <iostream>
  2 #include <stdio.h>
  3 #include <string.h>
  4 #include <vector>
  5 #include <stack>
  6 #include <algorithm>
  7 
  8 #include <bits/stdc++.h>
  9 #define LL long long
 10 #define pii pair<int,int>
 11 #define INF 0x7f7f7f7f
 12 using namespace std;
 13 const int N=100000*2+5;
 14 int age[N], n, m, col[N],s[N],c;
 15 double even;
 16 vector<int> vect[N];
 17 
 18 bool isolder(int x)
 19 {
 20     return x>=even? 1: 0;
 21 }
 22 void add_edge(int i,int j)  //建图
 23 {
 24     //x*2+1的表示选择c，x*2表示选择非c
 25     vect[i*2+1].push_back(j*2);         //无论哪两人，抢c必有冲突
 26     vect[j*2+1].push_back(i*2);
 27 
 28     if(isolder(age[i])==isolder(age[j]))  //抢A或B有冲突
 29     {
 30         vect[i*2].push_back(j*2+1);
 31         vect[j*2].push_back(i*2+1);
 32     }
 33 }
 34 
 35 bool color(int x)
 36 {
 37     if(col[x^1])    return false;
 38     if(col[x])      return true;
 39     col[x]=1;
 40     s[c++]=x;
 41     for(int i=0; i<vect[x].size(); i++)
 42     {
 43         int t=vect[x][i];
 44         if(!color(t)) return false;
 45     }
 46     return true;
 47 }
 48 
 49 bool cal(int n)
 50 {
 51     memset(col,0,sizeof(col));
 52     memset(s,0,sizeof(s));
 53     for(int i=0; i<n; i+=2)
 54     {
 55         if(!col[i]&&!col[i+1])
 56         {
 57             c=0;
 58             if(!color(i))
 59             {
 60                 while(c)    col[s[--c]]=0;
 61                 if(!color(i+1))    return false;
 62             }
 63         }
 64     }
 65     return true;
 66 }
 67 
 68 
 69 
 70 int main()
 71 {
 72     freopen("input.txt", "r", stdin);
 73     int a,b;
 74     while(scanf("%d%d",&n,&m),n+m)
 75     {
 76         for(int i=n*2; i>=0; i--)   vect[i].clear();
 77         even=0.0;
 78         for(int i=0; i<n; i++)
 79         {
 80             scanf("%d",&age[i]);
 81             even+=age[i];
 82         }
 83         even/=n;
 84 
 85         for(int i=0; i<m; i++)
 86         {
 87             scanf("%d%d",&a,&b);
 88             add_edge(--a,--b);
 89         }
 90 
 91         if(!cal(n<<1))    puts("No solution.");
 92         else
 93         {
 94             for(int i=0; i<n; i++)  //随便输出一组合法解。
 95             {
 96                 if(col[i*2])
 97                 {
 98                     if(isolder(age[i]) )      puts("A");
 99                     else    puts("B");
100                 }
101                 else puts("C");
102             }
103         }
104     }
105     return 0;
106 }
AC代码
```

