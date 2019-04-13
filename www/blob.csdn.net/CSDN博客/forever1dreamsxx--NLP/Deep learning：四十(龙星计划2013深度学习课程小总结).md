
# Deep learning：四十(龙星计划2013深度学习课程小总结) - forever1dreamsxx--NLP - CSDN博客


2014年01月27日 15:20:34[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：1465


转载地址：[http://www.cnblogs.com/tornadomeet/p/3189772.html](http://www.cnblogs.com/tornadomeet/p/3189772.html)
头脑一热，坐几十个小时的硬座北上去天津大学去听了门4天的深度学习课程，课程预先的计划内容见：[http://cs.tju.edu.cn/web/courseIntro.html](http://cs.tju.edu.cn/web/courseIntro.html)。上课老师为微软研究院的大牛——邓力，群（qq群介绍见：[Deep
 learning高质量交流群](http://www.cnblogs.com/tornadomeet/archive/2013/05/02/3055678.html)）里面有人戏称邓力（拼音简称DL）老师是天生注定能够在DL（Deep learning）领域有所成就的，它的个人主页见：[http://research.microsoft.com/en-us/people/deng/](http://research.microsoft.com/en-us/people/deng/)。这次我花费这么大的力气参加这个课程，是期望能达到3个目的的：一、把DL中一些常见基础算法弄明白，特别是跟能量模型等相关的算法。二、站在更上一层的角度来理解DL，知道下一步该学哪些知识。三、和群友交流下科研心得，多认识几个朋友。实践表明，第二和第三个目的基本达到，但关于第一个有点小失望。
下面简单回忆下邓老师这4天大概讲了些什么内容（没做太多笔记，记忆有些模糊）。
**Day1：**其课件主要见[Lecture-Slides-DayOne](http://research.microsoft.com/en-us/people/deng/final-day1-july2.pptx).由于邓老师讲课时喜欢插入一些他和其它DL大牛（比如Hinton）之间的故事，所以这个课件没有讲完（课件本身内容页太多），大概讲了前面100页，后面几天还有不少内容是基于这个课件的。在这一天的课程中，他列出了一些DL研究小组的网页，并给出了一些常见的学习工具。主要是从语音处理领域介绍DL，Dynamic
 model，以及两者的结合；具体内容有：DL是分布式的表示；感知机介绍，SVM其实本质也是感知机；为什么BP算法在90年代末期被扔掉的原因；比较详细的介绍了RBM以及RBM的训练过程，对比了RBM和GMM模型；介绍了DBN模型，以及DBN在生成数据和识别上的使用方法；DNN模型（用generative method pre-training后的深度网络）；从优化角度和过拟合角度介绍了为什么pre-training会有效果；RBM等价于一个无穷层的DBN；微软在语音识别领域的历史进展；DBN（动态贝叶斯模型）在语音处理上的使用；DL和动态贝叶斯的结合。
**Day2：**课件见[Lecture-Slides-DayTwo](http://research.microsoft.com/en-us/people/deng/final-day2-july3.pptx)，课件内容非常少。因为这一天放了一天的video，video内容都是coursera上的课程，其中一门课是Hintion的[Neural
 Networks for Machine Learning](https://class.coursera.org/neuralnets-2012-001/auth/auth_redirector?type=login&subtype=normal)，另一门课程是从生物角度介绍神经科学的：[Computational Neuroscience](https://class.coursera.org/compneuro-001/auth/auth_redirector?type=login&subtype=normal)。可能每个人基础不同以及所期望的目的不同吧，反正本人对那一天的课程比较失望。因为那些课程都是网上公开的，课件和视频网上都可以看得到，而邓老师却用了一天的时间在课程上放这些。比较好的方法应该是列出它要放的视频的重点（几页ppt就可以搞定），然后大致提一下其内容，让学员回去后对照看即可（比较大家千里迢迢跑来听课是想多学点东西）。这一天的内容中邓老师是想从生物神经的角度来推导出DL，因为DL的数学理论太差，这点经常遭他人抨击。而邓老师的观点是，DL其实是生物神经模型通过N步简化而来的，具有一定的生物基础，所以有一定的道理。这就是那一天的主要内容，交叉放生物神经模型和工程神经网络的视频教程。听说现场有些搞科研的老师听完当天的课程后收获非常大，因为受到邓老师很多观点的启发，有了很多自己新的想法。另外，在第二天的课程中，邓老师比较客观的介绍了他自己认为DL能够适用的领域（比如语音，图像，文本等），以及DL不擅长的领域（比如说Malware
 detection，movie recommender, speaker/language detection），理由是一些有感知机制的AI比较适合用DL，而一些Data Matching不太适合。
**Day3：**课件虽为[Lecture-Slides-DayThree](http://research.microsoft.com/en-us/people/deng/final-day3-july4.pptx)，但是由于第二天有人反应上课方式和内容不太好，所以把邓老师的计划弄乱了，因此这一天的内容是想到哪方面还没介绍就讲哪方面的知识。主要内容好像是CNN；目标识别；dropout；SGD；Hopfield能量模型；RNN的介绍以及一些常见的RNN
 learning方法，包括long short term memory, Hessian Free optimization, Echo State Network等，主要还是用Hintion的NN课件。
**Day4：**当天的内容更没有预先的计划，像打补丁似的想到哪就讲到哪，课件主要用的是第一天没有讲完的内容和hintion的课件，还有一些是ICML2013的一些keynote，比如Vincent Vanhoucke的。主要讲的内容有Deep AutoEncoder；DL在IR中的应用，重点讲了邓老师自己提出的模型DSN；DL在NLP领域的应用；DL在语音处理中的acoustic
 model中的应用。下午的安排主要是邓老师回答同学的提问，问题是前一天通过已发邮件给胡老师（胡老师个人主页：[http://cs.tju.edu.cn/faculty/huqinghua/](http://cs.tju.edu.cn/faculty/huqinghua/)）整理过的，在邓老师回答这些问题前请科大讯飞和中科院自动化所的人分享了一些Deep learning领域的心得，其中一个报告毫无内容，另一个没太听懂。同学提的问题比较多，从邓老师的回答过程中也收获了不少。
**总结：**邓老师还是相当厉害的，从他上课激动的语言可以看出，他对DL的未来还是比较乐观的，他并不是盲目推崇DL，比如他也比较客观的分析了哪些领域适合DL，哪些领域不适合DL。这次上课虽然没有收获太多的DL基本算法细节，但是也知道了后续一段时间如果想继续DL的，该看哪方面的知识。比如说至少知道了coursera是个很好的自学平台。另外和群友一起参加小讨论，逛校园，聚餐，整个过程还是相当愉快的，认识了不少朋友。
10号到学校后花了3天多时间把Hition的课程[Neural Networks for Machine Learning](https://class.coursera.org/neuralnets-2012-001/auth/auth_redirector?type=login&subtype=normal)大概学了一遍。共16节课。感觉这个课程还是比较不错的（很早以前有看过其中的一节RBM，完全不知所云，所以第一印象是他的课程不好），有很多工程经验性的东西在里面，因为课程主要是从工程角度上讲解的，所以如果你对所将内容有过实际项目和实验经验的话，收获会很大。这么课的基本内容是：从什么是机器学习，为什么要使用机器学习到怎样常见的NN算法在机器学习中的应用；从感知机的学习到BP算法；从NN在预测下一个单词到图片目标识别的应用；从梯度下降法到常见的NN优化算法；从Hopfield
 Nets到RNN网络以及其训练方法，从模型泛化到模型平均；从BM到RBM以及从RBM到DBN,DBM；从SBN到Week-Sleep算法；从PCA到Deep AutoEncoder；从joint model到Hierachy part model；还有各种DL在语音，图像，文本上的应用，以及DL的未来都给出了一定的介绍。课程覆盖的内容相当广泛；相信随着自己今后具体实践时回过图来参考这些课件，会很有帮助。
总的来说说，”北伐”的这几天，收获还是不错的。感谢龙星计划和天津大学的组织者，感谢邓老师这几天辛苦的付出！


作者：tornadomeet 出处：http://www.cnblogs.com/tornadomeet 欢迎转载或分享，但请务必声明文章出处。      （新浪微博：tornadomeet,欢迎交流！）


