# lintcode&九章算法——Google 面试题 | 多余的连接 ？ 待解决 - wjheha的博客 - CSDN博客
2017年11月18日 15:33:55[wjheha](https://me.csdn.net/wjheha)阅读数：152
1 题目描述
a. 给定一个无向图，这个图是在一棵树的基础上加上一条边构成的。问哪条边可以删掉使图重新变成一棵树？如果有多个答案那么输出输入的边中最后出现的那条。
b. 输入输出 
Input: [[1,2], [1,3], [2,3]] 
Output: [2,3] 
Explanation: The given undirected graph will be like this:
1 
/ \ 
2 - 3
Input: [[1,2], [2,3], [3,4], [1,4], [1,5]] 
Output: [1,4] 
Explanation: The given undirected graph will be like this:
5 - 1 - 2 
      |   | 
     4 - 3
c. 注意给的边有顺序，当两个点已经有共同的根结点时候，这条边应该被删除
d. 输入保证[u,v] u
