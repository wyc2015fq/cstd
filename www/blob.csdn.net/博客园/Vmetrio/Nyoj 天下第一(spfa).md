# Nyoj  天下第一(spfa) - Vmetrio - 博客园







# [Nyoj  天下第一(spfa)](https://www.cnblogs.com/wangmengmeng/p/5317141.html)




- 描述

AC_Grazy一直对江湖羡慕不已,向往着大碗吃肉大碗喝酒的豪情，但是“人在江湖漂,怎能



不挨刀",”人在江湖身不由己",如果自己的武功太差,在江湖会死的很惨，但是AC_Grazy没有



武功秘籍练不了绝世武功.有道是“山重水复疑无路,柳暗花明又一村”,在AC_Grazy家里面



竟然藏着一本书,书名竟然叫做【超级外挂】,竟然能在各种武功之间进行转化,据说是他爷



爷的爷爷的...爷爷传下来的...



闲着无事便拿来看看,只看一眼便再也停不下了,只见上面写着“纵横武林打遍天下无敌手武功心法秘籍收录”.



翻开第一篇一看竟然是【降龙十八掌】...



心法只是一个修练武功的途径,重要的是真气的多少,于是他便想利用外挂让武功之间进行转



化，来让真气无限增加，但是这个心法只能按照顺序转化，我们分别用 1号和2号来代替两种功法 当然转化会有一定的转化率f



比如1 0.5 2 便是把 1的一半真气转化给2 ,为了简化问题，我们每次都从1号秘籍开始进行转化,如果其中一个秘籍转化断了，那么以后的功法就不能转换。


- 输入
输入：首先输入一个数 T(T<=20)表示T组数据

然后输入两个数n(2<=n<=500)和m(1=<m<=2000)分别表

示有n种秘籍，随后的m行分别输入

秘籍u(n>=u>0) 转化率 f (0<f<=10)秘籍 v.(0<v<=n)- 输出
输出：如果可以无限增加真气输出Yes否则输出No.- 样例输入
2
3 3
1 2 2
2 2 3
3 2 1
4 3
1 2 2
3 2 4
4 2 3- 样例输出
Yes
No- 上传者
[ACM_王亚龙](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=ACM_%E7%8E%8B%E4%BA%9A%E9%BE%99)




            第一次写spfa....判断负环....



代码:

```
1 #include <vector>
 2 #include <map>
 3 #include <set>
 4 #include <algorithm>
 5 #include <iostream>
 6 #include <cstdio>
 7 #include <cmath>
 8 #include <cstdlib>
 9 #include <string>
10 #include <cstring>
11 #include <queue>
12 using namespace std;
13 #define INF 0x3f3f3f3f
14 #define MAX 555
15 
16 double dis[MAX],mp[MAX][MAX];
17 int vis[MAX],num[MAX];
18 int n,m;
19 vector<int> list[MAX];
20 
21 int spfa(int s)
22 {
23     queue<int> q;
24     while(!q.empty())
25         q.pop();
26     q.push(s);
27     dis[s]=1.0;
28     vis[s]=1;
29     num[s]++;
30     while(!q.empty()){
31         s=q.front();
32         q.pop();  
33         vis[s]=0;
34         for(int i=0; i<list[s].size(); i++){
35             int p=list[s][i];
36             if(dis[p]<dis[s]*mp[s][p]){
37                 dis[p]=dis[s]*mp[s][p];
38                 if(!vis[p]){
39                     vis[p]=1;
40                     q.push(p);
41                     num[p]++;
42                     if(num[p]==n)
43                         return 0;
44                 }
45             }
46         }
47     }
48     return 1;
49 }
50 
51 void init()
52 {
53     memset(dis,0,sizeof(dis));
54     memset(vis,0,sizeof(vis));
55     memset(mp,0,sizeof(mp));
56     memset(num,0,sizeof(num));
57 }
58 
59 int main()
60 {
61     int t,u,v;
62     double f;
63     scanf("%d",&t);
64     while(t--){
65         init();
66         scanf("%d%d",&n,&m);
67         for(int i=0; i<m; i++){
68             scanf("%d%lf%d",&u,&f,&v);
69             mp[u][v]=max(mp[u][v],f);
70             list[u].push_back(v);
71         }
72         if(spfa(1))
73             printf("No\n");
74         else
75             printf("Yes\n");
76     }
77 }
```














