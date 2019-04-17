# 【强化学习】MCTS (Monte Carlo Tree Search) - zkq_1986的博客 - CSDN博客





2017年08月17日 14:11:05[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：2618








MCTS

(Monte Carlo Tree Search)



# **1 MCTS基本概念**

## **1.1 Monte Carlo**

Monte Carlo是指，随机、大量地从某个分布中生成采样数据，以此计算某一特定目标值。

【举例】

For example, consider a circle inscribed in a [unit square](https://en.wikipedia.org/wiki/Unit_square). Given that the circle and the square have a ratio of areas
 that is π/4, the value of [π](https://en.wikipedia.org/wiki/Pi) can be approximated using a Monte Carlo method:

·Draw a square, then [inscribe](https://en.wikipedia.org/wiki/Inscribed_figure) a circle within it

·[Uniformly](https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)) scatter objects of uniform size over the square

·Count the number of objects inside the circle and the total number of objects

·The ratio of the inside-count and the total-sample-count is an estimate of the ratio of the two areas, which is π/4.
 Multiply the result by 4 to estimate π



![](https://upload.wikimedia.org/wikipedia/commons/thumb/8/84/Pi_30K.gif/220px-Pi_30K.gif)

Monte Carlo method applied to approximating the value of π. After placing 30,000 random points, the
 estimate for π is within 0.07% of the actual value.





## 1.2 Monte Carlo Tree Search

Each round of Monte Carlo tree search consists of four steps:[[4]](#cite_note-chaslot2008-4)

· ·Selection: start from root R and select successive child nodes down to a leaf node L. The section below says more about a way of choosing child nodes that lets the game tree expand towards most promising moves, which is the
 essence of Monte Carlo tree search.

· ·Expansion: unless L ends the game with a win/loss for either player, create one (or more) child nodes and choose node Cfrom one of them.

· ·Simulation: play a random playout from node C. This step is sometimes also called playout or rollout.

· ·Backpropagation: use the result of the playout to update information in the nodes on the path from C to R.

Sample steps from one round are shown in the figure below. Each tree node stores the number of won/played playouts.

![](https://upload.wikimedia.org/wikipedia/commons/thumb/b/b3/MCTS_%28English%29.svg/808px-MCTS_%28English%29.svg.png)


Steps of Monte Carlo tree search













