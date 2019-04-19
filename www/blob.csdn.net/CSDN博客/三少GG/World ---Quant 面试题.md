# World ---Quant 面试题 - 三少GG - CSDN博客
2012年12月03日 22:13:06[三少GG](https://me.csdn.net/scut1135)阅读数：7639
### [http://www.docin.com.sixxs.org/p-6473837.html](http://www.docin.com.sixxs.org/p-6473837.html)
### [World Quant 面试题-4](http://blog.csdn.net.sixxs.org/maqingli20/article/details/7232095)
Question:
一条东西向长街，你站在街中间，街北是一排门，你有一把钥匙，请写出一种策略，要求X/N在最坏情况下最少，X为你到达正确的门时所走的总路程，N为正确的门距原点的距离，可以假设门与门之间距离为1。
Definition:
1. There are 2*m houses in all 
2. X is total distance traveled when open the rightdoor
3. N is distance from right door to middle point
4. Regard distance from middle point to the two nearbyhouses as 1, though it should be 0.5. This does not affect the result.
Solution #1: The first idea.
First gothrough left m houses by order. If the right house is not found, go to theright side. The worst case occurs whenright first house matches with key. Worst limit is: (1 + 2*m) .
Solution #2: An intuitional try.
Can theworst case improve if we first go left, but turn right at the middle point,then go through m houses at right, afterwards, check the remaining houses. Let m= 2*k, the value of X/N is: 1, for first left k houses; 1 + m/N for right mhouses; 1 + 3*m/N
 for remaining left half.
Theworst limit occurs at the remaining left half, but smaller than 7.
Solution #3: Find the best.
Inspiredby solution #2, we believe such a strategy can bring improvement, how can weget to the optimum? Assume that this strategy get best if it turned at left Phouses, and P = l*m. Noticed in the last solution the worst limit is 7, to makeit easy for analysis,
 we only take care of ‘m’ values that are big enough(becauseif m = 1, the limit will be 3, such analysis is meaningless).
Followthe same way, we can see optimum is reached when l = [1/m] 1/2,namely P = [m] 1/2.
Conclusion:
For very big value of m, thepossible best worst limit may be (1 + 2[m] 1/2). To achieve such alimit, first check left [m] ½ houses, then go right and check allhouses, go through the remaining houses in the end!
[http://zhiqiang.org.sixxs.org/blog/science/worldquant-written-test-2007.html](http://zhiqiang.org.sixxs.org/blog/science/worldquant-written-test-2007.html)
[http://zhiqiang.org.sixxs.org/blog/tag/%E9%9D%A2%E8%AF%95%E9%A2%98](http://zhiqiang.org.sixxs.org/blog/tag/%E9%9D%A2%E8%AF%95%E9%A2%98)
今年找工作并且常在水木混的人对WorldQuant这个公司应该不陌生，因为它在各求职版周期性发帖，标题是“美国著名对冲基金！ 超百万收入！！！”，而且中英文轮流上，让人不注意也难。
WorldQuant的笔试以难度注明，考试时间也超长，5个小时以上，绝对是智力和体力的双重挑战。
今年这次校园招聘马上就要开始了。先贴一个网上找到的去年的WorldQuant笔试题（好像去年才进入中国，所以也只有这么一次的样本）。大家先热身一下。
300层楼，3个一样的小球，设计一个策略，得到小球摔碎的临界层数，并且要求最坏情况下所试次数最少。
> 
经典的扔鸡蛋问题，只不过现在有三个鸡蛋。解题思路一样的，都是动态规划。
记F(n, k)为n层楼，k个球时所需要的最少尝试次数，则
F(n, k) = min ( F(n-r, k) + 1, F(r-1, k-1) + 1), r = 1, 2, …, n;
F(n, 1) = n;
一百个眼镜，摆成一个圈，全部正面向上，第一个人将每个翻动一次，一共翻了100次；第二个人从no.2开始隔一个翻一次，也翻100次；第3个人从no.3开始隔两个翻一次，翻100次，问100个人之后，多少眼镜正面向上
![](https://img-my.csdn.net/uploads/201212/03/1354543616_1828.jpg)
一个蛋糕，切成连续的n块，有m个豆，问如果每小块上放一种豆，并且要求相邻的2块上的豆不一样，有多少种方法。
> 
？？？
一条东西向长街，你站在街中间，街北是一排门，你有一把钥匙，请写出一种策略，要求X/N在最坏情况下最少，X为你到达正确的门时所走的总路程，N为正确的门距原点的距离，可以假设门与门之间距离为1。
> 
动态规划？
都不怎么会做 ![:(](http://zhiqiang.org.sixxs.org/blog/wp-includes/images/smilies/sad.gif)
