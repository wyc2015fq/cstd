# 科学网—讲评seminar-713：坚守阵地 创新求实 - 戴世强的博文




# 讲评seminar-713：坚守阵地 创新求实                           

已有 4312 次阅读2010-9-18 05:33|个人分类:[科海随笔](http://blog.sciencenet.cn/home.php?mod=space&uid=330732&do=blog&classid=141283&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=330732&catid=1)|关键词:seminar报告,刘轶军,快速多极边界元法,计算力学,综合集成创新|[计算力学](misc.php?mod=tag&id=72506), [seminar报告](misc.php?mod=tag&id=79866), [刘轶军](misc.php?mod=tag&id=79867), [快速多极边界元法](misc.php?mod=tag&id=79868), [综合集成创新](misc.php?mod=tag&id=79869)



【按】近两年来，在我所举办的例行seminar和学术报告会上，我时常做一些即兴点评，或在会后写一些随感，尽管相当肤浅，还是受到了年青朋友的欢迎。本学年开始，我将在博客中辟出“讲评seminar”专栏，就刚做过的seminar报告发表一些浅见，与朋友们进行讨论。需要申明的是：

1.我所seminar的题材非常广泛，我不可能全部听懂，因此有些评述很可能是“隔靴搔痒”，甚至言不及义，只是抱着共同学习的态度进行探索，有不当之处请报告人原谅，也请读者随时补充或指正；

2.评述是宏观性的，一定很不全面，着眼点是：我们从所听到的seminar报告中可以学到什么有益的东西，以达到“他山之石可以攻玉”的目的；

3.欢迎进行实质性的讨论。



我所的第713期seminar是本学年的开篇，报告人是来自美国辛辛那提大学机械工程系的刘轶军教授，内容涉及计算力学中的新型边界元法及其应用（见链接）。70分钟的报告相当精彩，接着进行的讨论相当热烈。与会者至少得到如下启迪。



**坚守阵地 持之以恒**



报告人刘轶军于1980年代初期毕业于西北工业大学，从硕士阶段开始就从事边界元法的理论和应用研究。硕士论文研究的课题是我们熟悉的圆薄板大挠度问题，钱伟长先生开创了该领域深入研究的先河，他用边界元法所得到的结果与钱先生的经典结果一致。

在此后的25年间，他持之以恒地坚守边界元法研究的阵地，终于成为该领域里得到公认的专门家，其中的标志是：去年他在剑桥大学出版社出版了专著：“Fast Multipole Boundary Element Method”（《快速多极边界元法》，以下简称此书所论述的方法为FMM-BEM），这本书是国际上第一本关于FMM-BEM的单本专著；他多次在国际会议上做特邀报告，在美国和亚洲多次组织相关的研讨会，并成为《边界元工程分析》杂志的编委。

对于刘轶军来说，最为幸运的是，他出国后不久，就遇到了学术造诣很深的好老师——Rizzo教授，BEM的创始人，在他此后的履历中，他亦步亦趋地紧跟Rizzo教授（直到去辛辛那提大学任职时才放单飞），得到了Rizzo教授的学问和治学理念的真传。

从他的报告我们可以看出，他对FMM-BEM的来龙去脉、精神实质、长处短处了如指掌。20多年来，他深入剖析并发展了这一方法，并将其应用于复合材料结构、电子微器件（MEMS）、生物材料、声波和弹性波传播等五光十色的问题，取得了不少富有创造性的成果。

因此，我们从中得到的主要启迪是：必须在相当长的时间内坚守一个学术方向（对刘教授来说是计算力学中的边界元法），锲而不舍地钻进去；同时，又需要选择符合时代需要的研究方向，如刘教授在不同时期相继主攻的复合材料、MEMS和生物材料等问题。正因为如此，他获得了成功。



**博采众长锐意创新**



大家知道，在计算力学领域里，计算的精度（accuracy）和效率（efficiency）是一对“冤家”，如何找到高精度、高效率的计算方法？这是人们梦寐以求的目标。用有限元法（FEM）解决三维问题，要对体积划分区域，因此尽管用途广泛，计算工作量却很大；采用边界元法（BEM）把原问题化成积分方程，只要在边界面上划分网格进行计算，当然提高了计算效率，但是计算工作量仍相当可观。

于是，快速多极边界元法（FMM-BEM）应运而生。其基本思路是：把边界离散化后的格点进行集群、分类，找出一种各个格点施展影响的“树状结构”，弄清矩-矩、矩-当地、当地-当地等的各类作用细节，从而在保证精度的前提下大大节省了计算工作量。

我认为，这种做法是一种综合集成创新，实际上综合了边界元法的框架和图论演绎的思路，收到了出奇制胜的效果。

说起来容易，做起来难。要把这种新方法应用于复合材料、MEMS等新型问题（如其中出现的CNT——碳纳米管的复杂结构），需要研究者的深度创新，有时还要综合应用分子动力学（MD）的思路和方法。

因此，我们再次看到，锐意创新是成功的关键。



**抓住主线条分缕析**



刘教授的oral presentation的水平可圈可点。要把自己25年的主要工作在一个多小时里向素不相识的听众交待清楚，谈何容易！

我们可从中学到一些表述技巧：

• 有大局观。把要陈述的问题放到计算力学的总体需要中，突出精度-效率这对主要矛盾，为展开主题做了合理铺陈；

• 抓大放小。用简短生动的语言和图文并茂的PPT片子，显示要讲述的方法的要点，不纠缠于细枝末节；

• 有分有合。报告中注意理论与实际的结合，用最吸引人眼球的实例来展示方法的效果，最后收放自如地、水到渠成地得岀报告的结论。

报告中对语言的应用能力也给人以好印象。



在答问环节中，报告人也做得很好，实事求是地回答了诸多问题，特别是回答了我的一个有点刁钻的问题：你认为你是哪个类型的学者？答案是：主要是第二类（参看昨天的博文）。



写于2010年9月17日



【链接】seminar-713的通知和报告内容摘要



**上海市应用数学和力学研究所Seminar 713**



**题目：**Fast Multipole Boundary Element Method and Its Applications in Applied Mechanics （快速多极边界元法及其在应用力学中的应用）

**报告人：**刘轶军教授（美国辛辛那提大学机械工程系）

Professor Liu Yijun (Department of Mechanical Engineering, University of Cincinnati)

**时间：**9月16日（周四）下午1：30

地点：应用数学和力学所会议室



**摘要：**

# Fast Multipole Boundary Element Method And Its 

# Applications In Applied Mechanics



### Yijun Liu

Professor of Mechanical Engineering

University of Cincinnati

Cincinnati, OH 45221-0072, USA

*E-mail*:[Yijun.Liu@uc.edu](mailto:Yijun.Liu@uc.edu)



## Abstract

The boundary element method (BEM) is a numerical method for solving boundary integral equations governing many problems in applied mechanics. With the BEM, only the boundary of a problem domain is discretized with the so-called boundary elements. The advantages of the BEM include easier meshing, smaller systems of equations, higher accuracy, and handling of infinite domains. In recent years, the research on the BEM has been re-energized due to the introduction of the fast multipole method (FMM) in the solutions of the BEM systems of equations. Both the solution time and memory storage requirement for the BEM have been reduced significantly to such a level that it is now practical to solve many large-scale BEM models with millions of equations using desktop PCs. This has also opened up a wide range of applications for the BEM, for example, in large-scale modeling of composites, micro-electromechanical-systems (MEMS), bio-materials, and acoustic and elastic wave problems. In this talk, an overview of the boundary element method and fast multipole method for the analysis of large-scale applied mechanics problems will be provided first. The basic ideas in the BEM and fast multipole method will be discussed and the advantages of the fast multipole BEM will be presented. Then, some recent research results on the fast multipole BEM on modeling nanocomposites, bio-materials, MEMS, Stokes flows, and noise predictions in mechanical systems such as wind turbines will be presented. The potentials of the fast multipole BEM in solving large-scale applied mechanics problems are demonstrated by these results.

（略去报告人简略）


转载本文请联系原作者获取授权，同时请注明本文来自戴世强科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-330732-364132.html](http://blog.sciencenet.cn/blog-330732-364132.html)

上一篇：[老科学家的婚姻爱情（6）郭永怀的故事](blog-330732-364129.html)
下一篇：[力学学科在中国To be, or not to be （1）力学是什么？](blog-330732-364422.html)


