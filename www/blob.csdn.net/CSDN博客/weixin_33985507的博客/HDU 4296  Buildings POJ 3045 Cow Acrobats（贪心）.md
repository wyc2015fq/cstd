# HDU 4296   Buildings  POJ 3045 Cow Acrobats（贪心） - weixin_33985507的博客 - CSDN博客
2019年01月08日 08:23:54[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
**题目连接：**
**[http://acm.hdu.edu.cn/showproblem.php?pid=4296](http://acm.hdu.edu.cn/showproblem.php?pid=4296)**
**[http://poj.org/problem?id=3045](http://poj.org/problem?id=3045)**
**题意：有n个砖块，每个砖块有两个属性wi和si，将所有的砖块依次叠放起来，每个砖块得到一个值ti=sigama(W)-si，**
**其中W为在第i块砖上面的砖的总w值之和。要求求一种叠放次序，使得max(ti)最小？**
**思路：假设某一种叠放次序，对于第i，i+1两个砖块来说，他们的次序谁在上谁在下对i以前和i+1以后的都没有****影响。设W为第i块上面的值：（t1、t2分别为上面、下面的ti值）**
（1）第i块在上，则$t_{1}=W-s_{i},t_{2}=W+w_{i}-s_{i+1}$,此时最大值$p=max(t_{1},t_{2})=max(W-s_{i},W+w_{i}-s_{i+1})$
（2）第i+1块在上，则$t_{1}=W-s_{i+1},t_{2}=W+w_{i+1}-s_{i}$，此时最大值$q=max(t_{1},t_{2})=max(W-s_{i+1},W+w_{i+1}-s_{i})$;
若$p<q$，那么i在i+1上较优。此时有$max(W-s_{i},W+w_{i}-s_{i+1})<max(W-s_{i+1},W+w_{i+1}-s_{i})$,两边同时加上$s_{i}+s_{i+1}-W$，即：
则$max(s_{i+1},w_{i}+s_{i})<max(s_{i},w_{i+1}+s_{i+1})$，所以只需：$s_{i}+w_{i}<s_{i+1}+w_{i+1}$。所以只需按照$s_{i}+w_{i}$升序排列，扫描一遍即可。
********************AC代码：********************
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 
 1 #include <iostream>
 2 #include <cstdio>
 3 #include <algorithm>
 4 using namespace std;
 5 
 6 struct node
 7 {
 8     int weight,strength;
 9 };
10 
11 node a[500005];
12 int n;
13 
14 int cmp(node a,node b)
15 {
16     return (a.weight+a.strength)<(b.weight+b.strength);
17 }
18 
19 int main()
20 {
21     while(scanf("%d",&n)!=-1)
22     {
23         int i;
24         for(i=1;i<=n;i++) scanf("%d%d",&a[i].weight,&a[i].strength);
25         sort(a+1,a+n+1,cmp);
26         __int64 x=a[1].weight,pre=-a[1].strength;
27         for(i=2;i<=n;i++)
28         {
29             if(x-a[i].strength>pre) pre=x-a[i].strength;
30             x+=a[i].weight;
31         }
32         printf("%I64d\n",pre);
33     }
34     return 0;
35 }
```

