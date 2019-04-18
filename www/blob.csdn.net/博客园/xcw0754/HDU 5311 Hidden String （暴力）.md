# HDU 5311 Hidden String （暴力） - xcw0754 - 博客园
# [HDU 5311 Hidden String （暴力）](https://www.cnblogs.com/xcw0754/p/4676861.html)
题意：今天是BestCoder一周年纪念日. 比赛管理员Soda有一个长度为
  1. 
  2. , , 依次连接之后得到字符串"anniversary".
特别注意：式子中红色符号！！！
思路：其实就是要在一个串中找可能存在的3个连续子串来构成这个"anniversary"，穷举第一个串的大小，再穷举第2个串的大小，剩下的由第3个串来搞定。每个串必须大于等于1的大小才行，而且不能重叠到。
如果用的是string自带的find，还需要特别注意的是OJ上string::npos可能并不是你机器上定义的那样，可能是-1，可能是unsigned的无穷大！总之最好避开这些东西~ 就是这里wa到怕了，因为BC和HDU上的并不一样，所以我在BC上交就过了，而HDU没过。
BC上交的：
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstring>
 4 #include <cstdio>
 5 #include <cmath>
 6 #include <algorithm>
 7 #include <vector>
 8 #include <map>
 9 #include <set>
10 #include <stack>
11 #include <queue>
12 #define LL long long
13 #define pii pair<int,int>
14 #define INF 0x7f7f7f7f
15 using namespace std;
16 const int N=200;
17 
18 
19 string str;
20 string tmp="anniversary";
21 
22 
23 
24 int cal(int len)
25 {
26     if(len<tmp.size())  return false;//不可能
27     if(str.find( tmp )!= string::npos) return true;
28     if(len==tmp.size()) //相同串
29     {
30         if(str.find( tmp )!= string::npos) return true;
31         else    return false;
32     }
33 
34 
35     for(int i=0; i<11; i++)
36     {
37         unsigned int s1=str.find(tmp.substr(0, i));
38         if( s1+tmp.size()>=len || s1==string::npos )  continue;
39 
40         for(int j=1; j+i<11; j++)
41         {
42             unsigned int s2=str.find( tmp.substr(i, j), s1+i  );
43             if( s2==string::npos )  continue;
44             int k=11-j-i;
45             if( str.find( tmp.substr(i+j, k), s2+j )!=string::npos)
46             {
47                 return true;
48             }
49 
50         }
51     }
52     return false;
53 }
54 
55 
56 
57 int main()
58 {
59     //freopen("input.txt", "r", stdin);
60     int n, m, t, p, q;
61 
62     cin>>t;
63     while(t--)
64     {
65         cin>>str;
66         bool tmp=cal(str.size());
67         if(tmp)     puts("YES");
68         else        puts("NO");
69     }
70     return 0;
71 }
AC代码
```
HDU 上的：
```
#include <bits/stdc++.h>
#define LL long long
#define pii pair<int,int>
#define INF 0x7f7f7f7f
using namespace std;
const int N=200;
string str;
string tmp="anniversary";
int cal(int len)
{
    for(int i=1; i+2<=tmp.size(); i++)
    {
        int s1=str.find(tmp.substr(0, i), 0);
        if( s1<0 )  continue;
        for(int j=1; j+i+1<=tmp.size(); j++)
        {
            int s2=str.find( tmp.substr(i, j), s1+i  );
            if( s2<0 )  continue;
            int k=tmp.size()-j-i;
            if(k<1) continue;
            if( str.find( tmp.substr(i+j, k), s2+j )<200 )
                return true;
        }
    }
    return false;
}
int main()
{
    freopen("input.txt", "r", stdin);
    int n, m, t, p, q;
    cin>>t;
    while(t--)
    {
        cin>>str;
        if( cal(   str.size()  ) )     printf("YES\n");
        else                      printf("NO\n");
    }
    return 0;
}
AC代码
```

