# 介绍《SICP读书笔记》 - 电影旅行敲代码 - CSDN博客
2018年12月16日 21:49:41[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：78
对SICP - 《Structure and Interpretation of Computer Programs》早有耳闻，但是总认为此书老旧，没有学习的价值，现在回想起来自己很是幼稚。**每个人在知识面前应该保持足够的谦卑**，此书评价这么高说明其中的内容定有学习的价值。回想刘未鹏在自己的博客中曾经提到基于比较的排序与信息论之间存在关系时，发现高德纳前辈在其《计算机程序设计的艺术》一书中早就云淡风轻的进行过介绍，而自叹弗如。而作为菜鸟的自己来说，SICP肯定是自己需要认真研读的书籍！
最近想学习这本书的由来是因为在学习rust时，遇到了其中的`enum type`，不是很理解，在搜索过程中遇到了[CppCon 2016: Ben Deane “Using Types Effectively](https://www.youtube.com/watch?v=ojZbFIQSdl8&index=2&list=FL5670t1B8Ka6tEWSLakhzfw&t=1337s)，但是发现其中的一些概念很陌生，关于编程和编程语言自己没有对应的积累，所以只能老老实实捡起SICP。
自己曾经也尝试学习过一段时间的Haskell，但是并没有坚持下去，一方面是由于自己不是很自律，另一方面**可以说是自己过早学习了自己无法理解的知识**。自己曾“伪勤奋“的读了很长一段时间的龙书，但只理解到了其中一些浅显的道理，当时认为自己理解了，重读时发现自己当初的理解是有偏差的，一大部分原因是由于自己当时没有足够的知识储备去学习这本书，所以读的时候一知半解。这种情况原因有很多，主要原因是在学习某些知识之前偷了懒，没有结合自身的情况来分析是否适合学习这些知识，所以没有意识到有一些先导知识需要补充。最后既没有搞懂相关内容，还有可能对部分知识有了错误的理解。而人很擅长在没有搞懂的地方，用已有的知识强行解释通，而在强行解释的过程中，难免会对内容进行曲解。
![学习知识](https://img-blog.csdnimg.cn/20181216213831258.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1,size_16,color_FFFFFF,t_70)
在自己有了几年编程经验后来读SICP或许是一个不错的选择，由于自己最近有太多的知识需要学习，为了避免读SICP成为另外一个无疾而终的事情，这里效仿[Eli Bendersky](https://eli.thegreenplace.net/2007/06/19/introducing-the-sicp-reading-notes)，做一个SICP的读书笔记。整个学习过程路线如下：
- 尝试做每道习题，并将其放到github上
- 主要使用MIT 6.001 Structure and Interpretation, 1986的教学视频，然后UC的CS61A视频为辅，见https://cs61a.org/和youtube中的CS 61A Departmental
在查找SICP的学习经验时，遇到了很多人推荐《[How to Design Programs: An Introduction to Computing and Programming](https://htdp.org/2003-09-26/Book/)》和《Concepts, Techniques, and Models of Computer Programming》这两本书，先记录在这儿，未来可能做一个关于这两本书的读书笔记。
