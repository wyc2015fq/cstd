# Nyoj  星际之门（一）(Cayley定理) - Vmetrio - 博客园







# [Nyoj  星际之门（一）(Cayley定理)](https://www.cnblogs.com/wangmengmeng/p/5303567.html)





描述

公元3000年，子虚帝国统领着N个星系，原先它们是靠近光束飞船来进行旅行的，近来，X博士发明了星际之门，它利用虫洞技术，一条虫洞可以连通任意的两个星系，使人们不必再待待便可立刻到达目的地。

帝国皇帝认为这种发明很给力，决定用星际之门把自己统治的各个星系连结在一起。

可以证明，修建N-1条虫洞就可以把这N个星系连结起来。

现在，问题来了，皇帝想知道有多少种修建方案可以把这N个星系用N-1条虫洞连结起来？




- 输入
第一行输入一个整数T,表示测试数据的组数(T<=100)
每组测试数据只有一行，该行只有一个整数N，表示有N个星系。(2<=N<=1000000)- 输出
对于每组测试数据输出一个整数，表示满足题意的修建的方案的个数。输出结果可能很大，请输出修建方案数对10003取余之后的结果。- 样例输入
2
3
4- 样例输出
3
16- 来源
[[张云聪]原创](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%5B%E5%BC%A0%E4%BA%91%E8%81%AA%5D%E5%8E%9F%E5%88%9B)- 上传者
[张云聪](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E5%BC%A0%E4%BA%91%E8%81%AA)


**定义：**

> 
有n个标志节点的树的数目等于nn−2(仅是cayley在组合数学中的应用)


#### 简单证明：

> 
 1.首先我们假设n为4，即有3个节点

2.这样的话我们就有k个子树，此时k=3


![](http://img.blog.csdn.net/20140622224210296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2VzaHVhaTE5OTQwNzIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)(图1)



3.选中其中一个节点C(1n),然后x 再选中不含该节点的一个子树C(1k−1),让这颗子树的根连接到该节点上，这样的话子树就减少了一棵

![](http://img.blog.csdn.net/20140622224222218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2VzaHVhaTE5OTQwNzIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)(图2)

![](http://img.blog.csdn.net/20140622224235015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2VzaHVhaTE5OTQwNzIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)(图3)
等。。。



4.重复操作直到k=1，k从n变成1总共执行了n-1次，所以根据乘法原理，构造出的有确定根节点的树有ans=nn−1∗(n−1)!

5.但是对于一棵树来说，它又n-1条边，每条边被选中先后的顺序有(n−1)!种，但是对于树来说，边的先后关系是无关紧要的，所以ans=ans(n−1)!=nn−1


![](http://img.blog.csdn.net/20140622224323406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2VzaHVhaTE5OTQwNzIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)(图4)

![](http://img.blog.csdn.net/20140622224333203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2VzaHVhaTE5OTQwNzIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)(图5)
6.对于每个树来说，构造树时有确定根节点，每一个树可以将该树中的n个节点均做为根节点，于是乎ans=ansn=nn−2

![](http://img.blog.csdn.net/20140622224340062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2VzaHVhaTE5OTQwNzIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)(图6)

![](http://img.blog.csdn.net/20140622224347750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2VzaHVhaTE5OTQwNzIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)(图7)

![](http://img.blog.csdn.net/20140622224355921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2VzaHVhaTE5OTQwNzIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




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
14 #define MAX 1000000 
15 
16 int main()
17 {
18     int n,m;
19     scanf("%d",&n);
20     while(n--){
21         scanf("%d",&m);
22         int ans=1;
23         for(int i=0; i<m-2; i++){
24             ans*=m;
25             ans%=10003;
26         }
27         printf("%d\n",ans);
28     }
29 }
```









数学-cayley定理   转载链接:http://blog.csdn.net/keshuai19940722/article/details/33417525












