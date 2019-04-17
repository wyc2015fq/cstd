# [CareerCup] 7.1 Basketball Shooting Game 投篮游戏 - Grandyang - 博客园







# [[CareerCup] 7.1 Basketball Shooting Game 投篮游戏](https://www.cnblogs.com/grandyang/p/4765721.html)







7.1 You have a basketball hoop and someone says that you can play one of two games.
 Game 1: You get one shot to make the hoop.
 Game 2: You get three shots and you have to make two of three shots.
 If p is the probability of making a particular shot, for which values of p should you pick one game or the other?



这道题提出了两种投篮游戏，可以任意选择一种，第一种是投一次篮，第二种是投三次中两个即可，现给出投篮命中率为p，问p的取值和选择哪个游戏的关系。

这题是典型的我大天朝高中数学中的概率问题，当时做的题可比这个要难的多。对于Game1，只投一次篮，那么获胜概率就是p，对于Game2，要获胜的情况有两种，一种是3个全命中，概率为p3，另一种是命中其中两个球，三个球中选两个的情况有3种，则命中率是3p2(1-p),那么我们先求Game1>Game2的情况，即p > 3p2(1-p)，接的p < 0.5, 而当p = 0, 0.5 或 1的时候，，两边相等。那么结论就出来了，当命中率小于0.5时，选Game1，当命中率为0,0.5或1时，选哪个都行。












