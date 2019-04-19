# lintcode&九章算法——Microsoft 面试题 | 我能赢 ？ 待解决 - wjheha的博客 - CSDN博客
2017年11月30日 09:14:34[wjheha](https://me.csdn.net/wjheha)阅读数：84
***题目描述***
有两个人玩一个游戏，给定一个最大可取整数 maxChoosableInteger，两个人轮流从1~maxChoosableInteger 中取一个数，取过的数不可再取，若其中一方取过以后，
所有取过的数加起来的和大于等于desiredTotal，那么这个人获胜。
现在给你maxChoosableInteger和desiredTotal，问先手是否必胜，假定游戏双方均采取最优策略。
你可以假定给出的 maxChoosableInteger不超过20,desiredTotal不超过300。
***样例***
输入: 
maxChoosableInteger = 10 
desiredTotal = 11
输出: 
false
***样例说明***
无论先手怎么取数，先手都会输掉游戏。
先手可以取1到10中的任何一个。如果先手取1，那么后手可以取2到10中的任何一个。
后手如果取10，那么就可以赢得游戏，因为此时和为1+10=11=desiredTotal。
假如先手取其他的数，那么后手仍然能赢得游戏，只要使和大于等于11即可。
***思路***：
