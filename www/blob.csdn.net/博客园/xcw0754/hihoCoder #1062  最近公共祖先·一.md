# hihoCoder #1062 : 最近公共祖先·一 - xcw0754 - 博客园
# [hihoCoder #1062 : 最近公共祖先·一](https://www.cnblogs.com/xcw0754/p/4459868.html)
题意：
　　提供一棵家族树，询问两点的LCA。
思路：
　　看大神代码得知此思路。用map保存儿子到父亲的映射，查询时，两人共同回溯到根，中间相遇的那个节点就是答案了。先用其中一个来回溯到根，记录路径上的节点为访问过，再用另一个来回溯到根，期间只要第一次碰到已经访问过的节点，就是答案。
```
1 #include <iostream>
 2 #include <map>
 3 #include <cstdio>
 4 #include <string>
 5 using namespace std;
 6 map<string,string> sontofar;
 7 map<string,int> vis;
 8 
 9 void check(string tmp1,string tmp2)
10 {
11     vis.clear();
12     string ind=tmp1;
13     while( !ind.empty() )
14     {
15         vis[ind]=1;
16         ind=sontofar[ind];        //找不到就是空了
17     }
18 
19     ind=tmp2;
20     while( !ind.empty() && vis[ind]==0 )
21         ind=sontofar[ind];
22 
23     if(ind.empty())
24         cout<<"-1"<<endl;
25     else
26         cout<<ind<<endl;
27 }
28 
29 int main()
30 {
31     //freopen("input.txt","r",stdin);
32     int t, p=0;
33     string tmp1, tmp2;
34 
35     cin>>t;
36     while(t--)
37     {
38         cin>>tmp1>>tmp2;
39         sontofar[tmp2]=tmp1;
40     }
41 
42     cin>>t;
43     while(t--)
44     {
45         cin>>tmp1>>tmp2;
46         check(tmp1,tmp2);
47     }
48     return 0;
49 }
AC代码
```

