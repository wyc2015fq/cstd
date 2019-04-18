# HDU 3697 Selecting courses 选课（贪心） - xcw0754 - 博客园
# [HDU 3697 Selecting courses 选课（贪心）](https://www.cnblogs.com/xcw0754/p/4319429.html)
题意：
　　一个学生要选课，给出一系列课程的可选时间（按分钟计），在同一时刻只能选一门课程（精确的），每隔5分钟才能选一次课，也就是说，从你第一次开始选课起，每过5分钟，要么选课，要么不选，不能隔6分钟再选。在给出的课程的事件Ai~Bi内，Bi起的那分钟是不能够选的了，就是说截止到(Bi-1)分钟59秒还能选，Bi就不能选了。
思路：
　　由于n最大才300，那就可以使用暴力解法。开始时刻可以从0~4分钟这5个时刻开始，因为每5分钟是个周期，比如0分没选，而5分才选了，这和从5分才开始选是一样的。每隔5分钟就检测一下有没有课可以选，如果有的话就立刻选，这么做有个前提，就是得按课程的截止时间来排序，然后往升序的方向开始扫。由于不能重复选课，所以需要一个数组来标记他们是否已经被选过了。每一时刻只能选一门，所以当选到了一门就可以跳到5分钟后了。
代码：
```
1 #include <iostream>
 2 #include <algorithm>
 3 #define N 305
 4 using namespace std;
 5 struct course
 6 {
 7     int a,b;
 8 }courses[N];
 9 bool cmp(course temp1,course temp2)
10 {
11     if(temp1.b==temp2.b)
12         return temp1.a<temp2.a;
13     return temp1.b<temp2.b;
14 }
15 int main()
16 {
17     int n,i,j,num,max,cur;
18     bool vis[N];
19     while(cin>>n,n)
20     {
21         max = 0;
22         for(i=0;i<n;i++)
23             cin>>courses[i].a>>courses[i].b;
24         sort(courses,courses+n,cmp);
25 
26         for(i=0;i<5;i++)    //穷举5个时间
27         {
28             num = 0;
29             memset(vis,0,sizeof(vis));
30             for(j=i; j<courses[n-1].b; j+=5)
31             {
32                 for(int k=0;k<n;k++)
33                 {
34                     if(!vis[k] && j<courses[k].b && j>=courses[k].a )
35                     {
36                         num++;
37                         vis[k] = true;
38                         break;
39                     }
40                 }
41             }
42             if(max<num)
43                 max=num;
44         }
45         cout<<max<<endl;
46     }
47     return 0;
48 }
3697
```

