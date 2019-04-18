# 编程之美2015 资格赛 hihocoder 题目2: 回文字符序列 - xcw0754 - 博客园
# [编程之美2015 资格赛 hihocoder 题目2: 回文字符序列](https://www.cnblogs.com/xcw0754/p/4435955.html)
思路：暴力搜，用BFS的方式，生成每一种可能，再对每一种可能进行判断是否回文，进行统计。严重超时！计算一个25个字符的，大概要20多秒！
```
1 #include <iostream>
 2 #include <deque>
 3 #include <string>
 4 #include <stdio.h>
 5 #include <cstring>
 6 using namespace std;
 7 
 8 deque<string> a;
 9 
10 bool isP(string &s )
11 {
12     string::iterator it1=s.begin();
13     string::iterator it2=s.end();
14     while( it1!=it2 && it1!=--it2 )
15     {
16         if( *it1!=*it2 )
17             return false;
18         it1++;
19     }
20     return true;
21 }
22 
23 unsigned long long count()   //判断回文，注意取余
24 {
25     unsigned long long cnt=0;
26     string tmp="";
27     a.pop_front();
28     while( !a.empty() )
29     {
30         tmp =a.front();
31 
32         if( tmp.size()==1 )
33             cnt++;  //单个的情况
34         else if( isP(tmp)==true )
35             cnt++;
36 
37         a.pop_front();
38         if(cnt>100007 )
39             cnt %= 100007;
40     }
41     return cnt;
42 }
43 
44 int betree(string &s, int len)  //生成树
45 {
46     string tmp="";
47     a.push_back(tmp);
48 
49     int i, j, limit =a.size() ;
50 
51     for(i=0; i<len; i++)
52     {
53         for(j=0; j<limit; j++)
54         {
55             a.push_back(a.front());   //不装第i个
56             a.push_back(a.front()+s[i]);   //装第i个
57             a.pop_front();
58         }
59         limit =a.size();
60     }
61     return 0;
62 }
63 
64 int main()
65 {
66     //freopen("input.txt","r",stdin);
67     int t, j=0;
68     string s;
69     cin>>t;
70     while(t--)
71     {
72         a.clear();
73         s="";
74         cin>>s;
75         betree(s, s.size());
76         cout<<"Case #"<< ++j<< ": "<< count()<<endl;
77     }
78     return 0;
79 }
BFS超时
```
另外的思路：肯定可以用DP。参考了别人的代码思路。添加了个SAO函数来记录当前字符在前面所出现的位置，若没出现过则置-1，这样可少扫很多次。
```
1 #include <iostream>
 2 #include <string>
 3 #include <stdio.h>
 4 #include <memory>
 5 #include <string.h>
 6 using namespace std;
 7 long long map[1001][1001];
 8 int pos[26];
 9 int tmp[130];
10 int len;
11 string input;
12 void cal(int start ,int end)
13 {
14     if(start==end)      //自己到自己，即单个字符的情况，是回文，也就是它自己
15     {
16         map[start][end]=1;
17         return ;
18     }
19     if(end-start==1)    //两个字符的情况，判断两者是否相同。若相同，则有3个，否则，只有两个
20     {
21         if(input[start]==input[end])
22             map[start][end]=3;
23         else
24             map[start][end]=2;
25         return ;
26     }
27     map[start][end]+=map[start][end-1]+1;   //先加一个，也就是他自己单独作为回文串。
28 
29 /*
30     for(int i=end-1; i>=start; i--)
31     {
32         if(input[i]==input[end])            //考虑end之前的每一个，对于第i个，如果和end相同，那么结果就是i到end之间的串的回文串的最大数量+1
33             map[start][end] += map[i+1][end-1]+1;       //状态方程
34     }
35 */
36     int temp = pos[end];
37     while( temp>-1 && temp>=start )
38     {
39         map[start][end] += map[temp+1][end-1]+1;       //状态方程
40         temp = pos[temp];
41     }
42 
43     map[start][end]%=100007;
44 }
45 
46 void sao()
47 {
48     for(int i=0; i<26; i++)
49         pos[i] = -1;        // -1说明第1个字符的前面并没有与其相同的字符。
50     for(int i=0; i<130; i++)    //记录最近一个字符出现的位置
51         tmp[i] = -1;
52 
53     for(int i=0; i<len; i++)
54     {
55         pos[i] = tmp[input[i]];
56         tmp[input[i]] = i;       //更新字符input[i]最新出现的位置
57     }
58 }
59 
60 int main()
61 {
62     //freopen("input.txt","r",stdin);
63     int T;
64     int Case=0;
65     cin>>T;
66 
67     while(T--)
68     {
69         memset(map,0,sizeof(map));
70 
71         cin>>input;
72         len = input.size();
73         sao();
74         for(int i=0; i<input.size(); i++)   //从左往右扫
75         {
76             for(int j=i; j>=0; j--)         //从第i个开始往左扫，才能保证在计算大问题之前，所要依靠的小问题已经被解决
77             {
78                 cal(j,i);
79             }
80         }
81         cout<<"Case #"<<++Case<<": "<<map[0][input.size()-1]<<endl;
82     }
83 
84 }
AC了
```

