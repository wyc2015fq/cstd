# Nyoj   网络的可靠性(水题) - Vmetrio - 博客园







# [Nyoj   网络的可靠性(水题)](https://www.cnblogs.com/wangmengmeng/p/5303337.html)





描述

A公司是全球依靠的互联网解决方案提供商，也是2010年世博会的高级赞助商。它将提供先进的网络协作技术，展示其”智能+互联“的生活概念，同时为参观者提供高品质的个人体验和互动，以”信息通信，尽情城市梦想”为主题贯穿。借助奇幻的剧场大屏幕和特效，展现信息通信技术的应用前景，通过生动形象的故事，向观众展示沟通无限制的未来社会前景。

为此，A公司为世博园的N个区域建立了视频通信系统，其中每个区域建立一个基站，编号依次为1，2，3...,N。通过基站之间的通信线路为各区域的参观者提供视频服务。

已知在各基站之间已铺设了一些光纤通讯线路，这些线路覆盖了所有的区域，即任意两个区域都可以进行视频传递。但为了节约成本开支，目前只铺设了N-1条线路，同时为了减轻各基站的信息传递负载，每个基站最多有三条光纤通讯线路与之连接。

但在通信系统试运行期间，A公司发现当某个基站发生故障时，会导致其它区域之间无法进行信息传递。为了提高该通信网络的可靠性，A公司准备在基站之间再新铺设一些光纤线路，使得任意一个基站故障后，其它基站之间仍然可以通讯。

由于铺设线路的成本昂贵，A公司希望新增设的光纤线路越少越好。A公司请求Dr. Kong来完成这个任务


- 输入
有多组测试数据，以EOF为结束标志。
第一行： N 表示有N个基站
接下来有N-1行：X Y 表示第X个基站与第Y个基站直连
1<=N<=10000- 输出
输出一个整数，表示至少需新铺设的光纤线路数- 样例输入
8
1 3
3 2
5 3
5 4 
5 6
2 7
2 8- 样例输出
3- 来源
[第三届河南省程序设计大赛](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%E7%AC%AC%E4%B8%89%E5%B1%8A%E6%B2%B3%E5%8D%97%E7%9C%81%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E5%A4%A7%E8%B5%9B)


判断度为 1 的点的个数...(sum+1)/2...画图就可以理解.....



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
16 int a[10010];
17 
18 int main()
19 {
20     int n,x,y;
21     while(~scanf("%d",&n)){
22         memset(a,0,sizeof(a));
23         for(int i=1; i<n; i++){
24             scanf("%d%d",&x,&y);
25             a[x]++;
26             a[y]++;
27         }
28         int sum=0;
29         for(int i=1; i<=n; i++){
30             if(a[i]==1){
31                 sum++;
32             }
33         }
34         printf("%d\n",(sum+1)/2);
35     }
36 }
```














