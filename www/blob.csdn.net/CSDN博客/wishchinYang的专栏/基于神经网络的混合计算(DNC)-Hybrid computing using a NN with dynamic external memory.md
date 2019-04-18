# 基于神经网络的混合计算(DNC)-Hybrid computing using a NN with dynamic external memory - wishchinYang的专栏 - CSDN博客
2016年11月12日 13:50:26[wishchin](https://me.csdn.net/wishchin)阅读数：2022
个人分类：[AI/ES																[ANN/DNN/纤维丛																[TuringMachine																[时序/变长分析](https://blog.csdn.net/wishchin/article/category/6034695)](https://blog.csdn.net/wishchin/article/category/6972922)](https://blog.csdn.net/wishchin/article/category/3109825)](https://blog.csdn.net/wishchin/article/category/1730787)
前言：
DNC可以称为NTM的进一步发展，希望先看看这篇译文，关于NTM的译文：[人工机器-NTM-Neutral Turing Machine](http://blog.csdn.net/wishchin/article/details/53084685)
基于神经网络的混合计算
Hybrid computing using a neural network with dynamic external memory
原文：Nature：[doi: 10.1038/nature20101](http://www.nature.com/nature/journal/v538/n7626/full/nature20101.html)
异义祠：memory matrix ：存储矩阵，内存以矩阵方式编码，亦成为记忆矩阵。
               the neural Turing machine：神经图灵机[16]。看做是DNC的早期版本。
               differentiable attention mechanisms：可微注意力机制。
               The read vector：结合操作符和数据结构 的操作。
使用神经网络和动态外部存储器进行混合计算
Hybrid computing using a neural network with dynamic external memory
### 1. 摘要
- ANN非常擅长感知处理、 序列学习、 增强学习，而由于外部存储器的缺失，在表达变量、数据结构和存储长时间数据上能力有限。
- 在此我们介绍一种机器学习模型称为可微神经计算机 (DNC) ，包含一个可以读取和写入外部存储器的神经网络，类似于传统计算机的随机存储器。正如传统计算机，可以用内存来表达和操纵复杂的数据结构，并且，类似于一个神经网络，依然可以从数据中进行学习。
- 当使用监督学习进行训练时，我们可以确定，DNC 可以成功地解答用来模仿自然语言中的推理和判断的综合问题。我们可以得到，它可以进行任务学习，例如查找随机图中指定点之间的最短路径和推断的缺失环节，之后再将这种能力泛化，用于交通线路图、家谱等特定的图。
- 使用强化学习训练后，DNC 能够完成移动拼图这个益智游戏，其中游戏目标可以使用序列符号进行表示。
- 综上所述，我们的成果展示了 DNC 拥有解决复杂、结构化任务的能力，这些任务是没有外部可读写的存储器的神经网络难以胜任的。
### 2. 前言
- 现代计算机普遍使用计算和数据分离的计算体系，计算和输入输出分离。这包含两个便利：分层的存储结构带来价格和存储的折中。但是变量的读取和生成需要运算器对地址进行操作，不好之处就是，在内存动态增长的网络中，网络不能进行随机动态进行存储操作。
- 最近的在信号处理、序列学习、强化学习、认知科学和神经科学有很大突破，但在表达变量和数据结构时受到限制。此文旨在通过提供一个结合神经网络和外部存储器的结构，结合神经网络和计算处理的优势，方法是聚焦于最小化备忘录memoranda/内存和长时间存储器的接口。整个系统是可微的，因此可以使用随机梯度下降法进行端到端的训练，允许网络学习如何 在有目的行为中操作和组织内存。
**3.系统概览**
- DNC 是一种耦合到外部存储矩阵的神经网络（只要内存不被占用完全，网络的行为与内存块的大小独立|应该是使用了分布表进行去位置相关|，因此我们认为内存是“外部的”）。如果内存可以被认为是 DNC 的 RAM，网络则可以被称为控制器，CPU可微的操作是通过梯度下降法直接进行学习。DNC的早期结构，**神经图灵机**，拥有相似的结构，但使用了更受限的内存存取方法。
- DNC 架构不同于最近提出的Memory networks和Pointer networks的神经记忆框架，其区别在于DNC内存有选择性地可以写入和读取，允许迭代修改内存内容。
- 相比传统计算机使用唯一编址内存，DNC使用可微注意/分析机制[2,16-18]定义指派内存第N行或者“位置”，在N*W的矩阵M中（这样直接定义内存有问题啊），这些分派，这里我们成为权值，表示此处位置涉及到读或者写的程度/度量？。读向量r通过对记忆矩阵M的一个读权值操作wr返回( 记忆位置的权值累加和 )：
- 
![](https://img-blog.csdn.net/20161108155157255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 类比，写操作符使用一个写权值wW首先擦除向量e，然后加和一个向量v：
-                         M[ i, j ] <—— M[ i, j ]![](https://img-blog.csdn.net/20161108155610209?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 决定 和应用权值的单元叫做读写头。头的操作可由表1进行阐述。
![](https://img-blog.csdn.net/20161108155937852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
表1 DNC的结构
**a**,A recurrent controller network receives input from an external data source and produces output.**b, c**, The controller also outputs vectors that parameterize one write head (green) and multiple read heads (two in this case, blue and pink). (A reduced selection of parameters is shown.) The write head defines a write and an erase vector that are used to edit the N × W memory matrix, whose elements’ magnitudes and signs are indicated by box area and shading, respectively. Additionally, a write key is used for content lookup to find previously written locations to edit. The write key can contribute to defining a weighting that selectively focuses the write operation over the rows, or locations, in the memory matrix. The read heads can use gates called read modes to switch between content lookup using a read key (‘C’) and reading out locations either forwards (‘F’) or backwards (‘B’) in the order they were written. d, The usage vector records which locations have been used so far, and a temporal link matrix records the order in which locations were written; here, we represent the order locations were written to using directed arrows.
**a**,一个DNC结构从额外的数据源接受数据输入并产生输出;
**b,c**,控制器可以写/输出向量（参数化一个写磁头-绿色）且并联一个读磁头（上图中有两个，蓝色和粉色）。
写磁头定义了一个写和擦除向量（用于编辑N*M内存块），其元素的量级和符号通过块区域和shading唯一表示。另外，一个写键用来查找内容去寻找先前写过的位置（待编辑）。写键可以用于定义一个权值（有选择的）确定于写操作在矩阵块的行或者位置。
读磁头可以使用门（被称作读模式）来进行 使用一个读键（“C”）进行内容查找，和读出位置后（使用F键进行前向搜索或者“B”键进行后项）写入。
**d**.使用标记位置向量 记录目前已使用位置，一个缓存链接矩阵记录被写入的顺序；图中，我们使用有向箭头表示写入的顺序。
**4 EXPERIMENT SETTINGS**
~~      We evaluate the proposed approach on the task of English-to-French translation. We use the bilingual, parallel corpora provided by ACL WMT ’14.3 As a comparison, we also report the performance of an RNN Encoder–Decoder which was proposed recently by Cho et al. (2014a). We use the same training procedures and the same dataset for both models.4~~
~~          不要再翻译了，可能不小心nature会找上门来。~~
## References
1. Krizhevsky, A., Sutskever, I. & Hinton, G. E. Imagenet classification with deep convolutional neural networks. In Advances in Neural Information Processing Systems Vol. 25 (eds Pereira, F. et al.) 1097–1105 (Curran Associates, 2012).
2. Graves, A. Generating sequences with recurrent neural networks. Preprint at http://arxiv.org/abs/1308.0850 (2013).
3. Sutskever, I., Vinyals, O. & Le, Q. V. Sequence to sequence learning with neural networks. In Advances in Neural Information Processing Systems Vol. 27 (eds Ghahramani, Z. et al.) 3104–3112 (Curran Associates, 2014).
4. Mnih, V. et al. Human-level control through deep reinforcement learning.Nature 518, 529–533 (2015).
5. Gallistel, C. R. & King, A. P. Memory and the Computational Brain: Why Cognitive Science Will Transform Neuroscience (John Wiley & Sons, 2011).
6. Marcus, G. F. The Algebraic Mind: Integrating Connectionism and Cognitive Science (MIT Press, 2001).
7. Kriete, T., Noelle, D. C., Cohen, J. D. & O’Reilly, R. C. Indirection and symbol-like processing in the prefrontal cortex and basal ganglia. Proc. Natl Acad. Sci. USA110, 16390–16395 (2013).
8. Hinton, G. E. Learning distributed representations of concepts. In Proc. Eighth Annual Conference of the Cognitive Science Society Vol. 1, 1–12 (Lawrence Erlbaum Associates, 1986).
9. Bottou, L. From machine learning to machine reasoning. Mach. Learn. 94, 133–149 (2014).
10. Fusi, S., Drew, P. J. & Abbott, L. F. Cascade models of synaptically stored memories. Neuron 45, 599–611 (2005).
11. Ganguli, S., Huh, D. & Sompolinsky, H. Memory traces in dynamical systems. Proc. Natl Acad. Sci. USA 105, 18970–18975 (2008).
12. Kanerva, P. Sparse Distributed Memory (MIT press, 1988).
13. Amari, S.-i. Characteristics of sparsely encoded associative memory. Neural Netw. 2, 451–457 (1989).
14. Weston, J., Chopra, S. & Bordes, A. Memory networks. Preprint at http://arxiv.org/abs/1410.3916 (2014).
15. Vinyals, O., Fortunato, M. & Jaitly, N. Pointer networks. In Advances in Neural Information Processing Systems Vol. 28 (eds Cortes, C et al.) 2692–2700(Curran Associates, 2015).
16. Graves, A., Wayne, G. & Danihelka, I. Neural Turing machines. Preprint at http://arxiv.org/abs/1410.5401 (2014).
17. Bahdanau, D., Cho, K. & Bengio, Y. Neural machine translation by jointly learning to align and translate. Preprint at http://arxiv.org/abs/1409.0473 (2014).
18. Gregor, K., Danihelka, I., Graves, A., Rezende, D. J. & Wierstra, D. DRAW: a recurrent neural network for image generation. In Proc. 32nd International Conference on Machine Learning (eds Bach, F. & Blei, D.) 1462–1471 (JMLR, 2015).
19. Hintzman, D. L. MINERVA 2: a simulation model of human memory. Behav. Res. Methods Instrum. Comput. 16, 96–101 (1984).
20. Kumar, A. et al. Ask me anything: dynamic memory networks for natural language processing. Preprint at http://arxiv.org/abs/1506.07285 (2015).
21. Sukhbaatar, S. et al. End-to-end memory networks. In Advances in Neural Information Processing Systems Vol. 28 (eds Cortes, C et al.) 2431–2439(Curran Associates, 2015).
22. Magee, J. C. & Johnston, D. A synaptically controlled, associative signal for Hebbian plasticity in hippocampal neurons. Science 275, 209–213 (1997).
23. Johnston, S. T., Shtrahman, M., Parylak, S., Gonc¸ alves, J. T. & Gage, F. H. Paradox of pattern separation and adult neurogenesis: a dual role for new neurons balancing memory resolution and robustness. Neurobiol. Learn. Mem. 129, 60–68 (2016).
24. O’Reilly, R. C. & McClelland, J. L. Hippocampal conjunctive encoding, storage, and recall: avoiding a trade-off. Hippocampus 4, 661–682 (1994).
25. Howard, M. W. & Kahana, M. J. A distributed representation of temporal context. J. Math. Psychol. 46, 269–299 (2002).
26. Weston, J., Bordes, A., Chopra, S. & Mikolov, T. Towards AI-complete question answering: a set of prerequisite toy tasks. Preprint at http://arxiv.org/abs/1502.05698 (2015).
27. Hochreiter, S. & Schmidhuber, J. Long short-term memory. Neural Comput. 9,1735–1780 (1997).
28. Bengio, Y., Louradour, J., Collobert, R. & Weston, J. Curriculum learning. In Proc.26th International Conference on Machine Learning (eds Bottou, L. & Littman, M.)41–48 (ACM, 2009).
29. Zaremba, W. & Sutskever, I. Learning to execute. Preprint at http://arxiv.org/abs/1410.4615 (2014).
30. Winograd, T. Procedures as a Representation for Data in a Computer Program for Understanding Natural Language. Report No. MAC-TR-84 (DTIC, MIT Project MAC, 1971).
31. Epstein, R., Lanza, R. P. & Skinner, B. F. Symbolic communication between two pigeons (Columba livia domestica). Science 207, 543–545 (1980).
32. McClelland, J. L., McNaughton, B. L. & O’Reilly, R. C. Why there are complementary learning systems in the hippocampus and neocortex: insights from the successes and failures of connectionist models of learning and memory. Psychol. Rev. 102, 419–457 (1995).
33. Kumaran, D., Hassabis, D. & McClelland, J. L. What learning systems do intelligent agents need? Complementary learning systems theory updated. Trends Cogn. Sci. 20, 512–534 (2016).
34. McClelland, J. L. & Goddard, N. H. Considerations arising from a complementary learning systems perspective on hippocampus and neocortex. Hippocampus 6, 654–665 (1996).
35. Lake, B. M., Salakhutdinov, R. & Tenenbaum, J. B. Human-level concept learning through probabilistic program induction. Science 350, 1332–1338 (2015).
36. Rezende, D. J., Mohamed, S., Danihelka, I., Gregor, K. & Wierstra, D. One-shot generalization in deep generative models. In Proc. 33nd International Conference on Machine Learning (eds Balcan, M. F. & Weinberger, K. Q.) 1521–1529 (JMLR, 2016).
37. Santoro, A., Bartunov, S., Botvinick, M., Wierstra, D. & Lillicrap, T. Meta-learning with memory-augmented neural networks. In Proc. 33nd International Conference on Machine Learning (eds Balcan, M. F. & Weinberger, K. Q.)1842–1850 (JMLR, 2016).
38. Oliva, A. & Torralba, A. The role of context in object recognition. Trends Cogn.Sci. 11, 520–527 (2007).
39. Hermann, K. M. et al. Teaching machines to read and comprehend. In Advances in Neural Information Processing Systems Vol. 28 (eds Cortes, C. et al.)1693–1701 (Curran Associates, 2015).
40. O’Keefe, J. & Nadel, L. The Hippocampus as a Cognitive Map (Oxford Univ. Press,1978).
