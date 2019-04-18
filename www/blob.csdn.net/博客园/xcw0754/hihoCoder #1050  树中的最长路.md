# hihoCoder #1050 : 树中的最长路 - xcw0754 - 博客园
# [hihoCoder #1050 : 树中的最长路](https://www.cnblogs.com/xcw0754/p/4458727.html)
题意：
　　求出树上最长路径的长度，并返回。
思路：
　　刚看到数据<=10^5，假如是单分支的树，那么有5万层，就不能递归，那就用桟实现， 那就要将长度信息保存在另开的数组中，很麻烦！！这题专门给递归实现的。DFS到叶子开始返回长度，经过每个节点就计算以该结点为转折点的最长路径，直到根为止，就找到最大路径了。
```
1 #include <iostream>
 2 #include <cstdio>
 3 #include <vector>
 4 using namespace std;
 5 vector< vector<int> > gra;    //树以临接表记录（有向图）
 6 int maxsum;
 7 int DFS(int num)
 8 {
 9     if(gra[num].empty()==true)    //出口
10         return 1;
11     int tmp, big1=0, big2=0;    //多叉树，big1和2分别是两条到本结点的最长路径。
12     for(int i=0; i<gra[num].size(); i++)
13     {
14         tmp=DFS( gra[num][i]);            
15         if(tmp>big1)
16         {
17             big2=big1;
18             big1=tmp;
19         }
20         else if(tmp>big2)
21             big2=tmp;
22     }
23     maxsum=max(maxsum, big1+big2);    //计算以本节点为转折点的最长路径。
24     return big1+1;
25 }
26 
27 int main()
28 {
29     //freopen("input.txt", "r", stdin);
30     int t, i, tmp1, tmp2;
31     cin>>t;
32     gra.resize(t);
33     for(i=1; i<t; i++)
34     {
35         scanf("%d%d", &tmp1, &tmp2);//用c读入会省一半时间
36         gra[--tmp1].push_back(--tmp2);
37     }
38     DFS(0);
39     printf("%d\n", maxsum);
40     return 0;
41 }
AC代码（DFS递归）
```

