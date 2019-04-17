# 搞机器学习/AI有什么必备的数学基础？｜经验之谈+资源大全 - 知乎
# 



> 雷刚 发自 凹非寺
量子位 报道 | 公众号 QbitAI



![](https://pic2.zhimg.com/v2-399da395383f7bd58446b39524fd97e5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='356'></svg>)



今天是开学第一天！心里只有学习的量子位，发现Hacker News上又有高分话题，而且还跟学习有关！

这次讨论的主题是：

**想搞机器学习/AI需要怎样的数学基础，有什么推荐的入门资料/课程？**

凡事总须研究，才会明白。看到这个话题，心里只有学习的量子位（×2）仔细看了半夜，这些讨论写着许多字、说了许多话，不过满篇可以分类为三个部分：
- 中肯建议
- 教材书籍
- 视频课程

独学习不如众学习。量子位就把学后感整理如下，一起遨游知识的海洋吧~



![](https://pic2.zhimg.com/v2-05250e29b42ddf01ab824011a88f2289_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='373'></svg>)



## 中肯建议

这部分内容的主要贡献者包括：**mindcrime**、**tlb**、**jules**、**rocqua**、**srean**、**leecarraher**、**irchans**、**KirinDave**、**wadams19**、**pramalin**等

首先

多变量微积分、线性代数、概率论、信息论，这几门必须基础扎实。精通图论也挺有用的。

大部分机器学习是对数据的模型拟合。为了拟合模型，需要把一些误差当成真实参数的函数，并对其进行最小化。最小化的算法基于梯度下降，也就是依赖于导数，这就是一种微积分运算。

如果你在做贝叶斯推理，你也需要用到微积分，因为贝叶斯定律将后验分布作为一个积分。

搞机器学习你只需要微积分1和2，微积分3里的旋度和散度、斯托克斯定理之类的，学物理用得上，机器学习不用这些。另外，你可能还需要一些微积分4中的基本函数分析。

（量子位注：此处微积分1234指的美国大学课程体系）



![](https://pic2.zhimg.com/v2-8010f312b6bb5db4ff6c812bfd73daf1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='358'></svg>)



微积分的本质之一，是反映了事物的变化，对于变化的平顺性提供了一个很好的描述。一个处于最佳状态的系统，在某个确定的点上不再增减变化。

机器学习中的许多问题都是优化问题：在给定一些约束的条件下，怎样选择参数才能让错误最小化？通常这非常困难（NP-hard），但如果你把问题设计为“平滑”，那么就能通过微积分来获得很好的代数解。

多变量微积分也是需要的，在尝试最小化“错误”时，通常会通过每次更改许多、许多参数来实现。这意味着你需要知道如何在高位空间进行平滑变换。

而微积分的中的积分，用来“测量”物体大小。大部分概率是用来描述非常笼统的比例。“这块有多大”的问题类似于“这件事发生的可能性有多大”。解决问题的办法，就是用数量庞大的小块集合在一起，形成一个复杂的整体。

所以从根本上讲，机器学习取决于如何测量一件事（积分）并且知道这件事如何变化（导数）。从某种程度上说，这两件事就是你在微积分中学到的。



![](https://pic3.zhimg.com/v2-559255d5517592290524f971f819150a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='271'></svg>)



我数学背景还不错，但想要搞明白K-L散度时，还得重新研究一下。在机器学习领域，几乎我遇到的信息论问题都是最小化K-L散度，这些看维基百科都能搞懂。你还得能理解具有概率输出的模型，比方生成模型和强化学习等。

如果你要阅读学术期刊，至少下面这些知识点应该懂一点：
- 统计学核心。你得熟悉统计学家如何处理数据，这常常用到
- 微积分。你不需要成为解题达人，但得明白多个变量进行微分和积分的过程
- 线性代数。一切的基础，比统计还重要
- 数值计算的方法。我不断的查看资料，以搞懂大家为什么那样做
- 计算理论以及相关研究。熟悉这些能让你发现错误，找到改进的方向
- 我的下一个挑战是非参数统计。许多研究者跟我说这一领域会得到很多收获，许多方法能极大的改进机器学习

还有人觉得，机器学习中最需要数学的地方，莫过于理解反向传播时。反向传播几乎都是偏导数/链式法则什么的。还有很多机器学习涉及一些微积分的凸优化。

但是

我们得分清“**应用**”和“**研究**”之间的区别。并不是每个人都在做最前沿的研究。有人下载一个DL4J，看几个教程，就能搭建一个基本的网络来解决问题，这个过程中也创造了价值。

机器学习虽然还没来到完全不需要关心底层细节的时间节点，但我们确实已经可以合法获取很多现成的工具，而不需要动手推导反向传播的方程式。

讲真，大多数情况下在工作中应用已知的方法，并不要求搞懂背后的数学，只需要了解基本的统计数据和概率论，能解释结果就好了。所以，如果你只是简单的使用别人做好的工具来解决问题，真的不需要什么数学背景。

一个本科生就能学会漂亮的解决问题，而不需要深入研究底层的数学细节，就权当做是工程问题的最佳实践。大多数实际工作中，并不用演算低级别的架构或公式，通常都是从已经选好的框架中，把想用的东西跳出来而已。

另一方面，如果你面临的问题不能用现成的方法搞定，这时候数学背景就派上用场了。如果你想在框架里应用一个全新或者小众的架构，就得搞明白之后才能写出来。

在应用和研究机器学习技术之间，有很大的不同。总的来说，单纯在应用这一端，并没有太多严格的数学背景要求。



![](https://pic4.zhimg.com/v2-d521884e0fdb40ed86f71087832d45af_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='424'></svg>)



需要多少数学，取决于你要在机器学习/AI领域扎多深。

如果只是应付工作，那你走运了，现成就能用的东西原来越多了。例如DataBot、H2O、Scikit-learn、Keras（加TensorFlow）……可能唯一必备的数学技能就是统计学。无论你选择了哪种解决方案，采用了何种自动调整和选择的算法，都得需要一些统计数据才能说明你的模型有效果。

想进一步提升自己，还可以花更多时间学习特征提取、数据工程，好好研究一下上面提到的几个工具包，特别是其中的模型。

如果你想研发新的技术和算法，天空才是你的极限，不过还是得统计数据。

那些已经大量使用的机器学习和AI框架，其实只是顶着一个数学的帽子，你完全可以把它们当成可靠的黑盒系统来用，没必要理解模型的生成过程和设置。很多工具可以告诉你哪些算法对你的数据最有意义，甚至能帮你找出最有效的那种。

虽然这说起来令人沮丧，但真的已经不是非得有博士学位才能干这行了。

不过，即便你能干的事情跟博士科学家差不多，也不意味着有人会雇你。雇主还是会看重数学、计算机科学或相关领域的博士学位。但这些可能更多出于其他方面因素的考量，而不是搞机器学习/AI的必要条件。

了解数学能让你更好的理解工作，减少愚蠢犯错的可能。



![](https://pic3.zhimg.com/v2-76f59a92a8a9599201880ac95f65727e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='322'></svg>)



上面讲到的工具，建议试试完全自动化的黑盒机器学习管道，比方说TPOT。尽早上手，以及可以推荐给你的产品经理朋友。

TPOT即Tree-based Pipeline Optimization Tool，这是一个基于遗传算法自动选择、优化机器学习模型和参数的工具。通常模型有非常大量的参数需要调整和优化，这类工具可以节省找到最优参数组合的时间。

不过，许多机器学习从业人员对这类自动化机器学习管道非常警惕。如果没有理解这些工具的基本统计/数学假设，可能会遇到很多坑；而这种一刀切的解决方案，也可能会给出误导性的结果。另外使用这类工具，也让解释原因和结果的工作变得更加困难，一个“黑盒子”很难得到价值认同。
- TPOT的GitHub地址：

[https://github.com/rhiever/tpot](https://link.zhihu.com/?target=https%3A//github.com/rhiever/tpot)

到底应该怎么开始学习？
- 建议一：有两种方法来学习机器学习/AI：1）阅读所有资料，然后开始解决问题 2）先开始解决问题，然后根据需要学习相关的数学知识。第二种方法更好。
- 建议二：首先在Coursera上看吴恩达机器学习和深度学习的课程。选择你感兴趣的领域和问题。接着阅读机器学习/AI在这一领域如何应用的论文。然后动手重现你已经搞明白并且感兴趣的论文。
- 建议三：这个**学习计划**我觉得非常有用，很好的列出了所需课程和时间框架，地址在此：

[https://www.analyticsvidhya.com/blog/2017/01/the-most-comprehensive-data-science-learning-plan-for-2017/](https://link.zhihu.com/?target=https%3A//www.analyticsvidhya.com/blog/2017/01/the-most-comprehensive-data-science-learning-plan-for-2017/)

## 教材书籍

接下来开始分享资源。先从书籍讲起。

这部分内容的主要贡献者包括：**CuriouslyC**、**rdudekul**、**kgwgk**、**charlescearl**、**ChadyWady**等。



![](https://pic4.zhimg.com/v2-9ba1e6e53d15f96386b02c6f21d4e317_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='404'></svg>)



我认为最好从**David MacKay**的《Information Theory, Inference and Learning Algorithms（信息论、推理与学习算法）》入手，内容可能有点老旧了，但仍然是这个领域最平易近人的书籍之一。
- 在线版本：

[http://www.inference.org.uk/itprnn/book.pdf](https://link.zhihu.com/?target=http%3A//www.inference.org.uk/itprnn/book.pdf)

另一本推荐的旧书是**E. T. Jaynes**的《Probability Theory: the Logic of Science（概率论：科学的逻辑）》。
- 在线版本

[http://www.med.mcgill.ca/epidemiology/hanley/bios601/GaussianModel/JaynesProbabilityTheory.pdf](https://link.zhihu.com/?target=http%3A//www.med.mcgill.ca/epidemiology/hanley/bios601/GaussianModel/JaynesProbabilityTheory.pdf)

以及**Tibshirani**的《Elements of Statistical Learning（统计学习元素）》。
- 在线版本：

[https://web.stanford.edu/~hastie/ElemStatLearn/](https://link.zhihu.com/?target=https%3A//web.stanford.edu/~hastie/ElemStatLearn/)

**Andrew Gelman**的《Bayesian Data Analysis（贝叶斯数据分析）》也很好。
- 在线版本：

[http://hbanaszak.mjr.uw.edu.pl/TempTxt/(Chapman%20&%20Hall_CRC%20Texts%20in%20Statistical%20Science)](https://link.zhihu.com/?target=http%3A//hbanaszak.mjr.uw.edu.pl/TempTxt/%28Chapman%2520%26%2520Hall_CRC%2520Texts%2520in%2520Statistical%2520Science%29)%20Andrew%20Gelman,%20John%20B.%20Carlin,%20Hal%20S.%20Stern,%20David%20B.%20Dunson,%20Aki%20Vehtari,%20Donald%20B.%20Rubin-Bayesian%20Data%20Analysis-Chapman%20and%20Hall_CRC%20(2014).pdf



![](https://pic3.zhimg.com/v2-0bee8e95b44a060a0ea9d75721a47b2e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='397'></svg>)



想了解这个领域最新的额进展，建议阅读**Ian Goodfellow**和**Yoshua Bengio**的《Deep Learning》。
- 在线版本：

[http://www.deeplearningbook.org/](https://link.zhihu.com/?target=http%3A//www.deeplearningbook.org/)

推荐一本我本科时候用的统计学教材：《Probability & Statistics for Engineers & Scientists》。
- 在线版本：

[https://www.amazon.com/Probability-Statistics-Engineers-Scientists-MyStatLab/dp/0134468910](https://link.zhihu.com/?target=https%3A//www.amazon.com/Probability-Statistics-Engineers-Scientists-MyStatLab/dp/0134468910)

再推荐一些**网上免费的数学参考书**：
- 在线数学教材汇总

[http://people.math.gatech.edu/~cain/textbooks/onlinebooks.html](https://link.zhihu.com/?target=http%3A//people.math.gatech.edu/~cain/textbooks/onlinebooks.html)
- 免费数学教材

[http://www.openculture.com/free-math-textbooks](https://link.zhihu.com/?target=http%3A//www.openculture.com/free-math-textbooks)
- 开放教材图书馆

[https://open.umn.edu/opentextbooks/SearchResults.aspx?subjectAreaId=7](https://link.zhihu.com/?target=https%3A//open.umn.edu/opentextbooks/SearchResults.aspx%3FsubjectAreaId%3D7)
- MIT在线教材

[https://ocw.mit.edu/courses/online-textbooks/#mathematics](https://link.zhihu.com/?target=https%3A//ocw.mit.edu/courses/online-textbooks/%23mathematics)
- 美国数学研究所认证教材

[https://aimath.org/textbooks/approved-textbooks/](https://link.zhihu.com/?target=https%3A//aimath.org/textbooks/approved-textbooks/)

AI是一个非常广阔的领域，每个细分领域都有不同的数学背景要求。掌握所有的数学知识是不可能的，所以你得想清楚对什么感兴趣。



![](https://pic2.zhimg.com/v2-911948b97d51a335b7dbcc934a405509_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='427'></svg>)



同时推荐一本**Russell和Norvig**的好书，覆盖了人工智能中很多不同的主题。无论你本科学到了什么，这本书都可以提供了一深入了解AI的良好起点。

这本书是《Artificial Intelligence: A Modern Approach》，中文版名称《人工智能：一种现代方法》。
- 英文版地址在此：

[http://aima.cs.berkeley.edu/](https://link.zhihu.com/?target=http%3A//aima.cs.berkeley.edu/)

另外附送一份**Michael I. Jordan**之前开出的书单。
- 地址在此：

[https://news.ycombinator.com/item?id=1055389](https://link.zhihu.com/?target=https%3A//news.ycombinator.com/item%3Fid%3D1055389)

**上述在线内容都是合法的。合法的。合法的。**



![](https://pic1.zhimg.com/v2-92a8f812aa1ef6bc441343444dc4fec0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='321'></svg>)



## 视频课程

再来就是视频课程汇总。主要贡献者包括：**72mena**、**leecarraher**、**mindcrime**、**rdrey**等。

YouTube以及[http://Videolectures.net](https://link.zhihu.com/?target=http%3A//Videolectures.net)上有很多高质量的数学**视频**教学内容。
- 之前提到的**David MacKay**，合辑在此：

[http://videolectures.net/david_mackay/](https://link.zhihu.com/?target=http%3A//videolectures.net/david_mackay/)
- **Leonard教授**合辑在此：

[https://www.youtube.com/user/professorleonard57](https://link.zhihu.com/?target=https%3A//www.youtube.com/user/professorleonard57)



![](https://pic1.zhimg.com/v2-ca0f1183d554aed607e657a5b6abc18c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='225'></svg>)


- **Gilbert Strang**合辑在此：

[https://www.youtube.com/results?search_query=gilbert+strang](https://link.zhihu.com/?target=https%3A//www.youtube.com/results%3Fsearch_query%3Dgilbert%2Bstrang)
- **3Blue1Brown**合辑在此：

[https://www.youtube.com/channel/UCYO_jab_esuFRV4b17AJtAw](https://link.zhihu.com/?target=https%3A//www.youtube.com/channel/UCYO_jab_esuFRV4b17AJtAw)
- 还有画风清奇的**Siraj Raval**讲人工智能中的数学：

[https://www.youtube.com/watch?v=xRJCOz3AfYY&list=PL2-dafEMk2A7mu0bSksCGMJEmeddU_H4D](https://link.zhihu.com/?target=https%3A//www.youtube.com/watch%3Fv%3DxRJCOz3AfYY%26list%3DPL2-dafEMk2A7mu0bSksCGMJEmeddU_H4D)



![](https://pic2.zhimg.com/v2-27046fb6f1a82cbe7160b24800165dc1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='307'></svg>)



我的建议是，先快速看一遍**Jeremy Howard**的讲座，这里面有很多机器学习/AI的应用案例，而且只需要一点点的数学背景就能看懂。
- 地址在此：

[http://course.fast.ai/](https://link.zhihu.com/?target=http%3A//course.fast.ai/)

接下来可以去Coursera上**吴恩达**的新课程，比原来的课程更容易接近，但仍然会有一些方程式让你不知所措，不过你肯定能实现出来。地址在此：
- 老课程《机器学习》：

[https://www.coursera.org/learn/machine-learning](https://link.zhihu.com/?target=https%3A//www.coursera.org/learn/machine-learning)
- 新课程《深度学习》：

[https://www.coursera.org/specializations/deep-learning](https://link.zhihu.com/?target=https%3A//www.coursera.org/specializations/deep-learning)



![](https://pic2.zhimg.com/v2-fc32d37d859da2dc66354323b8383d2d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='638' height='359'></svg>)



再推荐一个统计学课程，德州大学奥斯汀分校统计和数据科学系**Michael J. Mahometa**主讲的《数据分析基础》。
- 地址在此：

[https://courses.edx.org/courses/course-v1:UTAustinX+UT.7.11x+3T2016/course/](https://link.zhihu.com/?target=https%3A//courses.edx.org/courses/course-v1%3AUTAustinX%2BUT.7.11x%2B3T2016/course/)

小建议，在这个课程中讲师使用了R语言，我觉得还是Python更好。

另外，Coursera上有一系列的统计课程还不错。不过贝叶斯统计这门课有点难，建议买一本书或者补充点其他课外资料。我推荐这本书：《Bayes’ Rule: A Tutorial Introduction to Bayesian Analysis》
- 亚马逊有售：

[https://www.amazon.com/Bayes-Rule-Tutorial-Introduction-Bayesian/dp/0956372848](https://link.zhihu.com/?target=https%3A//www.amazon.com/Bayes-Rule-Tutorial-Introduction-Bayesian/dp/0956372848)

## OMT

想搞机器学习/AI但数学不好的孩子，或者还有？快把这篇发给他。

救救孩子……

二零一七年九月。



![](https://pic3.zhimg.com/v2-18304d3419be62aedd721abeacad6f6a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='305'></svg>)



传送门：肉身前往Hacker News观摩，请点击左下角“阅读原文”。

课外阅读

量子位在知乎上也发现一个质量很高的讨论。主要是王乃岩谈“如何判断一个面试者的深度学习水平？”，贾扬清等也参与了回答……

在量子位微信公众号（QbitAI）对话界面，回复：“**naiyan**”这六个字母，即刻前往观摩。

如果你有更好的推荐，欢迎留言，让更多朋友看到~

— 完 —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)

վ'ᴗ' ի 追踪AI技术和产品新动态


