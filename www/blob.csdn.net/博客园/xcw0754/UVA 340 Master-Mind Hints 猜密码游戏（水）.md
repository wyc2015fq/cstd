# UVA 340 Master-Mind Hints 猜密码游戏（水） - xcw0754 - 博客园
# [UVA 340 Master-Mind Hints 猜密码游戏（水）](https://www.cnblogs.com/xcw0754/p/4443831.html)
题意:
　　给一串密码（第一行），接着再给你很多行猜测，针对每行猜测，输出两个数字，分表代表：同一列上匹配的个数，不同列上匹配的个数。注：匹配指的是一次，一旦配对，不能再与其他配对。
思路：
　　每接受一行猜测就匹配，扫一遍就知道哪些是同列匹配的，统计出来，作为第一个输出的数字。扫的过程中将同列匹配的guess列置为零，顺便将不匹配的secret列插进哈希可重复的set中。接着再扫一遍guess数组，把非0的数字逐个在set中的找，找到了就删掉set中对应的数字，并统计个数。结果就出来了。
```
1 #include <iostream>
 2 #include <unordered_set>
 3 #include <cstdio>
 4 using namespace std;
 5 const int N=1005;
 6 int secret[N];  //密码
 7 int guess[N];
 8 int main()
 9 {
10     freopen("input.txt", "r", stdin);
11     int n, j=0;
12     int pair_1,pair_2;
13     unordered_multiset<int> mul_set;
14     while(cin>>n,n!=0)
15     {
16         for(int i=0; i<n; i++)
17             cin>>secret[i];
18 
19         cout<<"Game "<<++j<<":"<<endl;
20         while(1)
21         {
22             for(int i=0; i<n; i++)  //输入
23                 cin>>guess[i];
24             if(!guess[0]&&!guess[1])  break;           //结束例子
25 
26             pair_1=0;
27             pair_2=0;
28             for(int i=0; i<n; i++)  //扫一遍，处理相同的
29             {
30                 if(secret[i]==guess[i])
31                 {
32                     pair_1++;
33                     guess[i]=0;
34                 }
35                 else
36                     mul_set.insert(secret[i]);
37             }
38 
39             unordered_multiset<int>::iterator it;
40             for(int i=0; i<n; i++)  //再扫一遍
41             {
42                 if(guess[i])
43                 {
44                     it=mul_set.find(guess[i]);
45                     if(it!=mul_set.end())   //找到了
46                     {
47                         pair_2++;
48                         mul_set.erase(it);
49                     }
50                 }
51             }
52             mul_set.clear();
53             printf("    (%d,%d)\n", pair_1,pair_2);
54         }
55     }
56     return 0;
57 }
AC代码
```

