# HDU 1009 FatMouse' Trade肥老鼠的交易（AC代码） 贪心法 - xcw0754 - 博客园
# [HDU 1009 FatMouse' Trade肥老鼠的交易（AC代码） 贪心法](https://www.cnblogs.com/xcw0754/p/4223455.html)
题意：
　　一只老鼠用猫粮来换豆子，每个房间的兑换率不同，所以得尽量从兑换率高的房间先兑换。肥老鼠准备M磅猫粮去跟猫交易，让猫在warehouse中帮他指路，以找到好吃的。warehouse有N个房间，第i个房间包含J[i]磅豆子，且要求F[i]磅猫粮。肥老鼠不必交易房间里的所有豆子，相反，当他以F[i]*a% 磅猫粮交换，就可以拿到J[i]*a%磅豆子，这里a是一个实数。现在，他准备把作业分配给你：他能获得最大的豆子数是多少。输入：非负整数M和N，紧接着有N行，每行有2个正数，分别是J[i]和F[i]。最后一个例子会给出两个-1作为结束。所有整数不超过1000。
输出：对于每个输入例子，打印出一个实数（可换得的豆子数），精确到小数点后3位。
思路：
　　为防止猫粮太多，那么兑换的话就需要每个房间计算一下兑换率，再搜索兑换率高的，这样下去复杂度为n的平方。所以用另外的方法，先根据兑换率降序排序，nlogn，再从头一个个兑换过去，也只是n次循环，所以复杂度为nlogn。
```
1 #include <algorithm>  
 2 #include <iostream>
 3 using namespace std;
 4 struct node
 5 {
 6     int f,j;
 7 }trade[1001];
 8 bool cmp(node a,node b)
 9 {
10     return (double)a.f/a.j>(double)b.f/b.j;
11 }
12 double cal(int m,int n)
13 {
14     int i=0;
15     double mount=0;
16     while(m)
17     {
18         if(i>=n)    return mount;    //豆子太少，猫粮太多
19         if(m>=trade[i].j)//能交换整房间的豆子
20         {
21             m-=trade[i].j;
22             mount+=trade[i++].f;
23         }
24         else    //不足交换整房间的豆子
25         {
26             mount+=((double)trade[i].f/trade[i].j)*m;
27             m=0;
28         }
29     }
30     return mount;
31 }
32 void main()
33 {
34     int m,n,i;
35     while(scanf("%d %d",&m,&n)&&m!=-1&&n!=-1)
36     {
37         for(i=0;i<n;i++)
38             cin>>trade[i].f>>trade[i].j;
39         sort(trade,trade+n,cmp);    //按兑换率排序，降序
40         printf("%.3f\n",cal(m,n));
41     }
42 }
1009
```

