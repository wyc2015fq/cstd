# 关于GraphCuts分割算法 - xiahouzuoxin - CSDN博客





2012年12月17日 13:46:03[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：8264









【简介】

Graph Cuts 不等于 graph cut(如 min cut、normalizedcut、RatioCut)， 是最近在计算机视觉中研究和应用比较多的energyminimization（能量函数最小化）优化算法，典型的用于stereo matching, image restortion,texture synthesis等应用。



这个优化算法用来解 markov Random Field. 有[实验](http://people.csail.mit.edu/~billf/tappenIccv.pdf)<[Comparisonof Graph Cuts with Belief Propagation for Stereo, usingIdentical](http://people.csail.mit.edu/~billf/tappenIccv.pdf)[MRFParameters](http://people.csail.mit.edu/~billf/tappenIccv.pdf)>显示，Graph
 Cuts比用BeliefPropagation更好，比用dynamic programming(只用1D求解), gradient decedent,simulated annealing, etc 要好得更多；特别是使用a-b-swap, 和 a-expansion的时候。



【原理】
- [<FastApproximate Energy Minimization via GraphCuts>](http://www.cs.cornell.edu/~rdz/Papers/BVZ-pami01-final.pdf) ([Boykov](http://www.csd.uwo.ca/~yuri/), Veksler and Zabih, PAMI'01)，这篇paper是graph cuts的开山之作，系统介绍了如何构造graph和energy
 term来解stereodisparity, motion等问题，也比较直观的介绍了a-expansion。


- [<GRAPHBASED ALGORITHMS FOR SCENE RECONSTRUCTION FROM TWOOR MORE VIEWS>](http://portal.acm.org/citation.cfm?id=997606) (Kolmogorov'sPhD thesis 04), 这篇论文是[Kolmogorov](http://www.cs.ucl.ac.uk/staff/V.Kolmogorov/)的博士论文，实现了Graph CUT用于立体视觉的匹配，它改善了传统GRAPHCUT计算耗时的缺点，他还因此称为微软一员，专门从事GRAPH
 CUT在图像处理上的应用。


- [<Whatenergy functions can be minimized via graphcuts>](http://www.cs.cornell.edu/~rdz/Papers/KZ-PAMi04.pdf) (Kolmogorov PAMI'04). 本文首先介绍了需要通过graphcut最小化的能量函数的特性。虽然是二进制变量但是很容易推广到其他方面。详细描述了通过graphcut最小化的能量函数。并提供了最小化能量函数的通用构建。最后给出了最小化二进制能量函数的必要条件。


- [<](http://www.csd.uwo.ca/~yuri/Abstracts/iccv01-abs.html)[InteractiveGraph Cuts for Optimal Boundary & RegionSegmentation of Objects in N-D Images](http://www.csd.uwo.ca/~yuri/Abstracts/iccv01-abs.html)[>](http://www.csd.uwo.ca/~yuri/Abstracts/iccv01-abs.html) ([Boykov](http://www.csd.uwo.ca/~yuri/) iccv01) 这篇paper讲怎么用graphcut来做imagesegmentation。





![](http://cafe.postech.ac.kr/img/SEG_fig44.jpg)




【应用】

Graph cuts最主要的应用是图像分割。

在Boykov 和 Kolmogorov 俩人的主页上就有大量的code。包括maxflow/min-cut、stereoalgorithms等算法：

[http://vision.csd.uwo.ca/code/](http://vision.csd.uwo.ca/code/)

[http://www.cs.ucl.ac.uk/staff/V.Kolmogorov/software.html](http://www.cs.ucl.ac.uk/staff/V.Kolmogorov/software.html)



以及olga. Veksler的

[http://www.csd.uwo.ca/faculty/olga/code.html](http://www.csd.uwo.ca/faculty/olga/code.html)





