# NBUT 1118  Marisa's Affair （排序统计，水） - xcw0754 - 博客园
# [NBUT 1118  Marisa's Affair （排序统计，水）](https://www.cnblogs.com/xcw0754/p/4538772.html)
题意：
　　每行给出一个人名和一个int值，人名可重复出现。要求对同一个人名统计int值，最大的先输出，若相同，则按照人名出现次数，若再相同，则按照人名字典序。
思路：
　　输入完全部进行排序，写个比较函数传进去sort即可。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 
 4 struct node
 5 {
 6     char nam[22];
 7     int lik;
 8     int tim;
 9 }a[5500];
10 
11 map<string, int> mapp2;//名字映射为int比较好处理
12 
13 int cmp(node ll,node rr)
14 {
15     if(ll.lik<rr.lik)
16         return 0;
17     else if(ll.lik>rr.lik)
18         return 1;
19 
20     if(ll.tim<rr.tim)
21         return 0;
22     else if(ll.tim>rr.tim)
23         return 1;
24 
25 
26     if(strcmp(ll.nam, rr.nam)>0)
27         return 0;
28     else    return 1;
29 }
30 
31 int main()
32 {
33     //freopen("input.txt", "r", stdin);
34 
35     //按照可能性排序，再按照时间排序，再按字典序排序。
36 
37     int n, like;
38     char s[22];
39     string temp;
40     map<string,int>::iterator it;
41 
42     while(cin>>n)
43     {
44         memset(a,0,sizeof(a));
45         mapp2.clear();
46 
47         int k=0;
48         for(int i=0; i<n; i++)
49         {
50             scanf("%s%d",s,&like);
51             temp=s;
52             it=mapp2.find(temp);
53 
54             if(it==mapp2.end())      //新来的
55             {
56                 mapp2[temp]=k;
57 
58                 a[k].lik=like;
59                 a[k].tim=1;
60                 strcpy(a[k].nam, s);
61                 k++;
62             }
63             else
64             {
65                 a[it->second].lik+=like;
66                 a[it->second].tim++;
67             }
68         }
69         sort(a,a+k,cmp);
70         printf("%d\n",k);
71         for(int i=0; i<k; i++)
72             printf("%s %d %d\n", a[i].nam, a[i].lik, a[i].tim);
73     }
74     return 0;
75 }
AC代码
```

