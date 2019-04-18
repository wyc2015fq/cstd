# HDU 1007  Quoit Design最近点对（ 分治法） - xcw0754 - 博客园
# [HDU 1007  Quoit Design最近点对（ 分治法）](https://www.cnblogs.com/xcw0754/p/4881625.html)
题意：
　　给出平面上的n个点，问任意点对之间的最短距离是多少？ 
思路：
　　先将所有点按照x坐标排序，用二分法将n个点一分为二个部分，递归下去直到剩下两或一个点。对于一个部分，左右部分的答案分别都知道，那么可能的答案就是min(left_ans,right_ans) 。注意更小的点对可能一个在左，一个在右。所以还得处理两个边内的紧靠着的部分，如果左边的一个点到达中线的距离已经超过当前最短距离，那么这个点到达右边任意一个点也不会是最短距离了。同时，若一左一右的两个点的y距离已经超过目前最短距离，那么也不可能是最短距离，可能为答案的点对并不太多，大概O(n)对。因此，还得在x距离满足的情况下，y也得满足，所以y也得排序。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 
 4 struct node
 5 { 
 6     double x,y;
 7 }a[100005];  
 8 
 9 int c[100005];  
10 
11 double cmpy(int t1,int t2) { return a[t1].y<a[t2].y;}
12 bool cmp(node t1,node t2) { return t1.x<t2.x; }  
13 double dis(node t1,node t2) {return sqrt((t1.x-t2.x)*(t1.x-t2.x)+(t1.y-t2.y)*(t1.y-t2.y));}  
14 
15 double find(int left,int right)  
16 {  
17     if(left+1==right)    return dis(a[left],a[right]);  
18     if(left==right)        return 2147483.999;  
19 
20     int mid=(left+right)>>1;
21     double aa=find(left,mid);
22     double bb=find(mid+1,right);
23     double ans=min(aa,bb); //当前最小
24 
25     int cnt=0;
26     double x=a[mid].x;
27     for(int i=left; i<=right; i++)
28         if(fabs(a[i].x-x)<ans)    c[cnt++]=i;
29     sort(c,c+cnt,cmpy);        //按y来排序
30 
31 
32     for(int i=0;i<cnt;i++)        //计算x=[mid-ans，mid+ans]
33         for(int j=i+1;j<cnt;j++)    
34         {    
35             if(a[c[j]].y-a[c[i]].y>ans)    break; //两点的距离已超过ans
36             ans=min(ans,dis(a[c[i]],a[c[j]]));    //求最小距离
37         }   
38     return ans;
39 }  
40 
41 int main()  
42 {  
43     int n,i;  
44     while(cin>>n,n)  
45     {  
46         for(i=0;i<n;i++)     
47             scanf("%lf%lf",&a[i].x,&a[i].y);
48         sort(a,a+n,cmp);      //按X排序
49         printf("%.2lf\n",find(0,n-1)/2);
50     }  
51     return 0;  
52 }
AC代码
```

