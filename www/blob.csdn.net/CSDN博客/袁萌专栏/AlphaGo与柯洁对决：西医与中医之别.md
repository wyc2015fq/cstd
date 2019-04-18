# AlphaGo与柯洁对决：西医与中医之别 - 袁萌专栏 - CSDN博客
2017年05月25日 04:39:13[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1989
AlphaGo与柯洁对决：西医与中医之别
在老外眼中，围棋（Go）是一种神秘而又古老的智力游戏，其“中医式“理论涉及许多奇奇怪怪的概念。围棋的西医理论何在？
2016年，围棋的“西医理论”出现了，论文发表在《自然》杂志上，使用数学的蒙特卡洛方法从概率意义上研究围棋博弈过程。对此，中国人看起来也是怪怪的。
此文原文是“Mastering the game of
with deep neural networks and tree search”，其摘要如下（论文作者：[D
 Silver](http://xueshu.baidu.com/s?wd=author%3A%28David%20Silver%29%20&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&sc_f_para=sc_hilight%3Dperson)，[A Huang](http://xueshu.baidu.com/s?wd=author%3A%28Aja%20Huang%29%20&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&sc_f_para=sc_hilight%3Dperson)，[CJ Maddison](http://xueshu.baidu.com/s?wd=author%3A%28Chris%20J.%20Maddison%29%20&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&sc_f_para=sc_hilight%3Dperson)，[A Guez](http://xueshu.baidu.com/s?wd=author%3A%28Arthur%20Guez%29%20&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&sc_f_para=sc_hilight%3Dperson)，[L Sifre](http://xueshu.baidu.com/s?wd=author%3A%28Laurent%20Sifre%29%20&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&sc_f_para=sc_hilight%3Dperson)）：
The game of Go has longbeen viewed as the most challenging of classic games for artificialintelligence owing
 to its enormous search space and the difficulty ofevaluating board positions and moves. Here we introduce a new approach tocomputer Go that uses ‘value networks’ to evaluate board positions and ‘policynetworks’ to select moves. These deep neural networks are
 trained by a novelcombination of supervised learning from human expert games, and reinforcementlearning from games of self-play. Without any lookahead search, the neuralnetworks play Go at the level of state-of-the-art Monte Carlo tree searchprograms that
 simulate thousands of random games of self-play. We alsointroduce a new search algorithm that combines Monte Carlo simulation withvalue and policy networks. Using this search algorithm, our program AlphaGoachieved a 99.8% winning rate against other Go programs,
 and defeated the humanEuropean Go champion by 5 games to 0. This is the first time that a computerprogram has defeated a human professional player in the full-sized game of Go,a feat previously thought to be at least a decade away.
蒙特卡洛方法，另文介绍。
说明：西医也不是万能的，柯洁战胜AlphaGo完全是有可能的。
袁萌5月25日
