# POJ 图论题目大集合 - HJ - CSDN博客
2017年02月02日 12:42:48[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：1016
POJ图论分类2009-07-28 23:13POJ 2449 Remmarguts’ Date(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2449](http://acm.pku.edu.cn/JudgeOnline/problem?id=2449)
题意：经典问题：K短路 
解法：dijkstra+A*(rec)，方法很多 
相关：[http://acm.pku.edu.cn/JudgeOnline/showcontest?contest_id=1144](http://acm.pku.edu.cn/JudgeOnline/showcontest?contest_id=1144)
该题亦放在搜索推荐题中
POJ 3013 - Big Christmas Tree(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3013](http://acm.pku.edu.cn/JudgeOnline/problem?id=3013)
题意：最简单最短路，但此题要过，需要较好的程序速度和，还要注意精度 
解法：Dijkstra
POJ 3463 - Sightseeing(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3463](http://acm.pku.edu.cn/JudgeOnline/problem?id=3463)
题意：最短路和比最短路大1的路的数量 
解法：需要真正理解dijkstra
POJ 3613 - Cow Relays(较难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3613](http://acm.pku.edu.cn/JudgeOnline/problem?id=3613)
题意：求经过N条边的最短路 
解法：floyd + 倍增，贪心
POJ 3621 - Sightseeing Cows(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3621](http://acm.pku.edu.cn/JudgeOnline/problem?id=3621)
题意：求一个环路，欢乐值 / 总路径最大 
解法：参数搜索 + 最短路(ms 原始的bellman tle, 用spfa才过)
POJ 3635 - full tank?(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3635](http://acm.pku.edu.cn/JudgeOnline/problem?id=3635)
题意：最短路变形 
解法：广搜 
相关：[http://hi.baidu.com/hnu_reason/blog/item/086e3dccfc8cb21600e9286b.html](http://hi.baidu.com/hnu_reason/blog/item/086e3dccfc8cb21600e9286b.html)
生成树问题 
基本的生成树就不放上来了
POJ 1639 - Picnic Planning(较难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1639](http://acm.pku.edu.cn/JudgeOnline/problem?id=1639)
题意：顶点度数有限制的最小生成树 
解法：贪心 + prim/kruskal
POJ 1679 - The Unique MST(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1679](http://acm.pku.edu.cn/JudgeOnline/problem?id=1679)
题意：判断MST是否唯一 
解法：prim就行，不过还是易错的题
POJ 2728 - Desert King(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2728](http://acm.pku.edu.cn/JudgeOnline/problem?id=2728)
题意：所谓最优比率生成树 
解法：参数搜索 + prim
POJ 3164 - Command Network(难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3164](http://acm.pku.edu.cn/JudgeOnline/problem?id=3164)
题意：最小树形图 
解法：刘朱算法，这个考到的可能性比较小吧？
POJ 3522 - Slim Span(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3522](http://acm.pku.edu.cn/JudgeOnline/problem?id=3522)
题意：求一颗生成树，让最大边最小边差值最小 
解法：kruskal活用
连通性，度数，拓扑问题 
此类问题主要牵扯到DFS，缩点等技巧
POJ 1236 - Network of Schools(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1236](http://acm.pku.edu.cn/JudgeOnline/problem?id=1236)
题意：问添加多少边可成为完全连通图 
解法：缩点，看度数
POJ 1659 - Frogs’ Neighborhood(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1659](http://acm.pku.edu.cn/JudgeOnline/problem?id=1659)
题意：根据度序列构造图 
解法：贪心，详细证明参见havel定理
POJ 2553 - The Bottom of a Graph(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2553](http://acm.pku.edu.cn/JudgeOnline/problem?id=2553)
POJ 2186 - Popular Cows(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2186](http://acm.pku.edu.cn/JudgeOnline/problem?id=2186)
题意：强连通分量缩点图出度为0的点
POJ 2762 - Going from u to v or from v to u?(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2762](http://acm.pku.edu.cn/JudgeOnline/problem?id=2762)
题意：单向连通图判定 
解法：缩点 + dp找最长链
POJ 2914 - Minimum Cut(难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2914](http://acm.pku.edu.cn/JudgeOnline/problem?id=2914)
题意：无向图最小割 
解法：Stoer-Wagner算法，用网络流加枚举判定会挂
POJ 2942 - Knights of the Round Table(难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2942](http://acm.pku.edu.cn/JudgeOnline/problem?id=2942)
题意：求双联通分量(或称块)中是否含奇圈 
解法：求出双连通分量后做黑白染色进行二分图图判定 
相关：[http://hi.baidu.com/zfy0701/blog/item/57ada7ed104ce9d2b31cb104.html](http://hi.baidu.com/zfy0701/blog/item/57ada7ed104ce9d2b31cb104.html)
POJ 3177 - Redundant Paths(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3177](http://acm.pku.edu.cn/JudgeOnline/problem?id=3177)
POJ 3352 - Road Construction(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3352](http://acm.pku.edu.cn/JudgeOnline/problem?id=3352)
题意：添加多少条边可成为双向连通图 
解法：把割边分开的不同分量缩点构树，看入度 
建议对比下1236，有向图添加多少条边变成强连通图
POJ 3249 - Test for Job(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3249](http://acm.pku.edu.cn/JudgeOnline/problem?id=3249)
解法：bfs / dfs + dp
POJ 3592 - Instantaneous Transference(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3592](http://acm.pku.edu.cn/JudgeOnline/problem?id=3592)
解法：缩点，最长路，少人做的水题，注意细节
POJ 3687 - Labeling Balls(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3687](http://acm.pku.edu.cn/JudgeOnline/problem?id=3687)
解法：拓扑排序
POJ 3694 - Network(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3694](http://acm.pku.edu.cn/JudgeOnline/problem?id=3694)
解法：双连通分量+并查集
2-SAT问题 
此类问题理解合取式的含义就不难
POJ 2723 - Get Luffy Out(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2723](http://acm.pku.edu.cn/JudgeOnline/problem?id=2723)
POJ 2749 - Building roads(较难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2749](http://acm.pku.edu.cn/JudgeOnline/problem?id=2749)
解法：二分 + 2-SAT判定
POJ 3207 - Ikki’s Story IV - Panda’s Trick(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3207](http://acm.pku.edu.cn/JudgeOnline/problem?id=3207)
解法：简单的2-sat，不过其他方法更快
POJ 3648- Wedding(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3648](http://acm.pku.edu.cn/JudgeOnline/problem?id=3648)
解法：用2-sat做会比较有意思，但是暴搜照样0ms
POJ 3678 - Katu Puzzle(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3678](http://acm.pku.edu.cn/JudgeOnline/problem?id=3678)
解法：直接按合取式构图验证就行了
POJ 3683 - Priest John’s Busiest Day(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3683](http://acm.pku.edu.cn/JudgeOnline/problem?id=3683)
解法：n^2枚举点之间的相容性构图，求解2-SAT
最大流问题 
变形很多，最小割最大流定理的理解是关键
POJ 1149 - PIGS(较难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1149](http://acm.pku.edu.cn/JudgeOnline/problem?id=1149)
绝对经典的构图题
POJ 1273 - Drainage Ditches(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1273](http://acm.pku.edu.cn/JudgeOnline/problem?id=1273)
最大流入门
POJ 1459 - Power Network(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1459](http://acm.pku.edu.cn/JudgeOnline/problem?id=1459)
基本构图
POJ 1637 - Sightseeing tour(Crazy) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1637](http://acm.pku.edu.cn/JudgeOnline/problem?id=1637)
题意：求混合图的欧拉迹是否存在 
解法：无向边任意定向，构图，详建黑书P324
POJ 1815 - Friendship(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1815](http://acm.pku.edu.cn/JudgeOnline/problem?id=1815)
题意：求最小点割 
解法：拆点转换为边割 
相关：[http://hi.baidu.com/zfy0701/blog/item/a521f230b06dea9fa9018e0e.html](http://hi.baidu.com/zfy0701/blog/item/a521f230b06dea9fa9018e0e.html)
POJ 1966 - Cable TV Network(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1966](http://acm.pku.edu.cn/JudgeOnline/problem?id=1966)
题意：去掉多少点让图不连通 
解法：任定一源点，枚举汇点求点割集(转换到求边割)，求其中最小的点割
POJ 2112 - Optimal Milking(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2112](http://acm.pku.edu.cn/JudgeOnline/problem?id=2112)
二分枚举，最大流
POJ 2391 - Ombrophobic Bovines(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2391](http://acm.pku.edu.cn/JudgeOnline/problem?id=2391)
题意：floyd, 拆点，二分枚举 
相关：[http://hi.baidu.com/zfy0701/blog/item/3e0006c4f73f0eaf8226acff.html](http://hi.baidu.com/zfy0701/blog/item/3e0006c4f73f0eaf8226acff.html)
POJ 2396 - Budget(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2396](http://acm.pku.edu.cn/JudgeOnline/problem?id=2396)
题意：有源汇的上下界可行流 
解法：用矩阵-网络流模型构图，然后拆边 
相关：[http://hi.baidu.com/zfy0701/blog/item/6449d82a64e15e3e5343c1ba.html](http://hi.baidu.com/zfy0701/blog/item/6449d82a64e15e3e5343c1ba.html)
，最小割模型在竞赛中的应用
POJ 2455 - Secret Milking Machine(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2455](http://acm.pku.edu.cn/JudgeOnline/problem?id=2455)
二分枚举，一般来说需要写对边容量的更新操作而不是每次全部重新构图
POJ 2699 - The Maximum Number of Strong Kings(较难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2699](http://acm.pku.edu.cn/JudgeOnline/problem?id=2699)
解法：枚举人数 + 最大流(感谢xpcnq_71大牛的建图的提示)
POJ 2987 - Firing(较难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2987](http://acm.pku.edu.cn/JudgeOnline/problem?id=2987)
题意：最大权闭包 
解法：先边权放大，第一问总量-最大流，第二问求最小割 
相关：[http://wywcgs.spaces.live.com/blog/cns!4D861A02A3382142!1109.entry?&_c02_owner=1](http://wywcgs.spaces.live.com/blog/cns!4D861A02A3382142!1109.entry?&_c02_owner=1)
Profit(中等) 
[http://www.vijos.cn/Problem_Show.asp?id=1352](http://www.vijos.cn/Problem_Show.asp?id=1352)
最大权闭包图的特殊情况 
ZOJ 2071 - Technology Trader 也是此类型，懒了没做 
[http://acm.zju.edu.cn/show_problem.php?pid=2071](http://acm.zju.edu.cn/show_problem.php?pid=2071)
POJ 3084 - Panic Room(中等，好题) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3084](http://acm.pku.edu.cn/JudgeOnline/problem?id=3084)
题意：略 
解法：根据最小割建模
POJ 3155 - Hard Life(很挑战一题) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3155](http://acm.pku.edu.cn/JudgeOnline/problem?id=3155)
题意：最大密度子图 
解法：参数搜索 + 最大权闭合图，A.V.Goldberg的论文(nb解法) 
最小割模型在信息学竞赛中的应用 一文中也有讲
POJ 3189 - Steady Cow Assignment(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3189](http://acm.pku.edu.cn/JudgeOnline/problem?id=3189)
题意：寻找最小的区间完成匹配 
解法：这题充分说明SAP的强大，纯暴力可过。更好的方法是在枚举区间的过程中不断删边和加边继续网络流过程
POJ 3204 - Ikki’s Story I - Road Reconstruction(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3204](http://acm.pku.edu.cn/JudgeOnline/problem?id=3204)
ZOJ 2532 - Internship(基础) 
[http://acm.zju.edu.cn/show_problem.php?pid=2532](http://acm.zju.edu.cn/show_problem.php?pid=2532)
题意：确定边是否是某个割中的边 
解法：两边dfs求割, 或暴力枚举(需要写取消某条增广路的操作(但数据弱，也许不取消也能混过))
POJ 3308 - Paratroopers(较难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3308](http://acm.pku.edu.cn/JudgeOnline/problem?id=3308)
POJ 2125 - Destroying The Graph(难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2125](http://acm.pku.edu.cn/JudgeOnline/problem?id=2125)
题意：最小点权覆盖
POJ 3469 - Dual Core CPU(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3469](http://acm.pku.edu.cn/JudgeOnline/problem?id=3469)
题意：最小割
POJ 3498 - March of the Penguins(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3498](http://acm.pku.edu.cn/JudgeOnline/problem?id=3498)
题意：满足点容量限制的网络流 
解法：拆点把点容量转换为边容量，枚举汇点
ZOJ 2587 - Unique Attack(较难) 
[http://acm.zju.edu.cn/show_problem.php?pid=2587](http://acm.zju.edu.cn/show_problem.php?pid=2587)
题意：确定最小割是否是唯一的 
解法：得理解dfs求最小割算法的本质
SPOJ 839 - Optimal Marks(难) 
[http://www.spoj.pl/problems/OPTM/](http://www.spoj.pl/problems/OPTM/)
题意：略 
解法：很经典哦，见amber的集训队论文，根据标号的每一位求最小割
SGU 326 - Perspective(中等) 
[http://acm.sgu.ru/problem.php?c0&problem=326](http://acm.sgu.ru/problem.php?c0&problem=326)
比较经典的构图法
费用流问题 
可以KM解的就不放在这里，另外，感觉除非很特殊的图，一般用连续增广路的算法就够了
POJ 2175 - Evacuation Plan(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2175](http://acm.pku.edu.cn/JudgeOnline/problem?id=2175)
题意：判断是否给定解是最优解，比较阴的一题 
解法：根据给出的计划构造流，然后消且只消一次负圈
POJ 3422 - Kaka’s Matrix Travels(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3422](http://acm.pku.edu.cn/JudgeOnline/problem?id=3422)
题意：略 
解法：拆点
POJ 3680 - Intervals(较难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3680](http://acm.pku.edu.cn/JudgeOnline/problem?id=3680)
题意：略，这题还是蛮经典 
解法：discuss中比较详细
SPOJ 371 - Boxes(简单) 
[http://www.spoj.pl/problems/BOXES/](http://www.spoj.pl/problems/BOXES/)
题意：略 
解法：费用流，但似乎有比网络流更好的做法
SGU 185 - Two shortest(中等) 
[http://acm.sgu.ru/problem.php?c0&problem=185](http://acm.sgu.ru/problem.php?c0&problem=185)
题意：求两条不想交的最短路径 
解法：费用流，也可以最短路 + 最大流。
匹配问题 
正确理解KM算法是很重要的
这里我还要说几句：最正确解最小权匹配的办法是用一个很大的数-当前边权值，而不是直接对边权取反(这样只能处理左右点相等的完全二分图，即K(n, n)
以上有可能还是说的有点问题，以后补充
POJ 1486 - Sorting Slides(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1486](http://acm.pku.edu.cn/JudgeOnline/problem?id=1486)
题意：二分图的必须边 
解法：需正真理解最大匹配算法，详见[http://hi.baidu.com/kevin0602/blog/item/1d5be63b5bec9bec14cecb44.html](http://hi.baidu.com/kevin0602/blog/item/1d5be63b5bec9bec14cecb44.html)
POJ 1904 - King’s Quest(中等，好题) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1904](http://acm.pku.edu.cn/JudgeOnline/problem?id=1904)
题意：求二分图所有可能的匹配边 
解法：虽然最终不是用匹配算法，但需要理解匹配的思想转换成强连通分量问题。
POJ 2060 -Taxi Cab Scheme(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2060](http://acm.pku.edu.cn/JudgeOnline/problem?id=2060)
题意：最小路径覆盖
POJ 2594 -Treasure Exploration(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2594](http://acm.pku.edu.cn/JudgeOnline/problem?id=2594)
题意：可相交最小路径覆盖 
解法：先传递闭包转化下
POJ 3041 - Asteroids(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3041](http://acm.pku.edu.cn/JudgeOnline/problem?id=3041)
POJ 2226 - Muddy Fields(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2226](http://acm.pku.edu.cn/JudgeOnline/problem?id=2226)
题意：行列的覆盖 
解法：最小点集覆盖 = 最大匹配
POJ 2195 - Going Home(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2195](http://acm.pku.edu.cn/JudgeOnline/problem?id=2195)
题意：最小权值匹配 
解法：KM算法
POJ 2400 - Supervisor, Supervisee(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2400](http://acm.pku.edu.cn/JudgeOnline/problem?id=2400)
题意：输出所有最小权匹配 
解法：KM, 然后回溯解，汗，输入的两个矩阵居然是反过来的
POJ 2516 -Minimum Cost(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2516](http://acm.pku.edu.cn/JudgeOnline/problem?id=2516)
题意：最小权值匹配或最小费用流 
解法：拆点 + KM算法(只有正确的才能过)，费用流(ms错的可能也能过)
POJ 3686 - The Windy’s(较难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=3686](http://acm.pku.edu.cn/JudgeOnline/problem?id=3686)
题意：最小权值匹配 
解法：拆点,然后尽管用KM算法去水吧，数据其实弱得不得了 O(50 * 50 * 2500) -> 16ms 
相关：[http://hi.baidu.com/kevin0602/blog/item/2829dc01d7143b087bec2c97.html](http://hi.baidu.com/kevin0602/blog/item/2829dc01d7143b087bec2c97.html)
SPOJ 412 - K-path cover(较难) 
[https://www.spoj.pl/problems/COVER/](https://www.spoj.pl/problems/COVER/)
题意：略 
解法：很牛叉的一道匹配 
相关：[http://hi.baidu.com/roba/blog/item/c842fdfac10d24dcb48f31d7.html](http://hi.baidu.com/roba/blog/item/c842fdfac10d24dcb48f31d7.html)
SGU 206. Roads(较难) 
[http://acm.sgu.ru/problem.php?c0&problem=206](http://acm.sgu.ru/problem.php?c0&problem=206)
解法：经典题目，也可以使用spoj 412那题的优化
NP问题 
一般是搜索或dp解的
POJ 1419 - Graph Coloring(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1419](http://acm.pku.edu.cn/JudgeOnline/problem?id=1419)
题意：图的着色 
解法：搜索，可惜题目的数据真是太弱了
POJ 2989 - All Friends(难) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=2989](http://acm.pku.edu.cn/JudgeOnline/problem?id=2989)
题意：极大团数量 
解法：开始狂tle, 后来找了论文：Finding All Cliques of an Undirected Graph（Coen Bron & Joep Kerboscht）
ZOJ 1492 - Maximum Clique(基础) 
[http://acm.zju.edu.cn/show_problem.php?pid=1492](http://acm.zju.edu.cn/show_problem.php?pid=1492)
题意：图的最大团 
解法：搜索，如果要求速度，可参考下相应论文
其他 
不能成大类的
POJ 1470 - Closest Common Ancestors(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1470](http://acm.pku.edu.cn/JudgeOnline/problem?id=1470)
题意：LCA问题 
解法：tarjan或RMQ，另外输入很恶心
POJ 1985 - Cow Marathon(基础) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1985](http://acm.pku.edu.cn/JudgeOnline/problem?id=1985)
题意：树上的最长路径 
解法：dp
POJ 1986 - Distance Queries(中等) 
[http://acm.pku.edu.cn/JudgeOnline/problem?id=1986](http://acm.pku.edu.cn/JudgeOnline/problem?id=1986)
题意：LCA 
解法：tarjan或RMQ   
