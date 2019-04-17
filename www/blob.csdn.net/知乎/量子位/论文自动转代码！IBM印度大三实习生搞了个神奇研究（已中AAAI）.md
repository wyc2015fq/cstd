# 论文自动转代码！IBM印度大三实习生搞了个神奇研究（已中AAAI） - 知乎
# 



李林 岳排槐 发自 凹非寺

量子位 出品 | 公众号 QbitAI

深度学习的论文越来越多了~

多到什么程度？Google scholar的数据显示，2016年以来，人工智能领域新增的论文已经超过3.5万篇。arXiv上，AI相关的论文每天都不下百篇。

刚刚结束不久的计算机视觉会议ICCV上，发表了621篇论文；2018年的ICLR，有1004篇论文正在匿名开放评审；NIPS 2017共收到3240篇论文投稿。

研究成果极大丰富了，但离应用到产品中，还差一大步：把论文转化成代码。毕竟，作者顺便提供源码的是少数。

怎么办？

IBM印度研究院最近公布了一项新研究：DLPaper2Code，顾名思义，这个程序能够用深度学习技术，将论文转化成代码。
![](https://pic1.zhimg.com/v2-07f22b104a687d5ef754cd2f3ebaff4c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='343'></svg>)



**△** 论文转代码的过程

这么神奇？！该不是看到一篇假研究？

坦白讲，相关论文**DLPaper2Code: Auto-generation of Code from Deep Learning Research Papers**已经被AAAI 2018接收为会议论文。

AAAI是人工智能顶会之一，明年就是第32届了，2月2-7日在美国路易斯安那州的新奥尔良召开。百度、京东是大会的黄金赞助商。
![](https://pic1.zhimg.com/v2-93492f69c584bcccb59302f8ede9a90c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='148'></svg>)



深度学习论文自动转代码

## 这篇论文中指出，由于大部分深度学习论文都会用流程图来表示神经网络模型的设计模式，因此，在论文转换成代码的过程中，DLPaper2Code首先提取、理解论文中描述的深度学习设计流程图和表格，将它们转化成抽象的计算图。

然后，它会把抽取的计算图转换成Keras和Caffe框架下的可执行源代码。

IBM的这项研究，总共生成了5000篇arXiv深度学习论文的Caffe和Keras代码，还创建了一个网站，供同行们众包编辑这些代码。不过，这个网站的地址还没有公布，目前只能看到截图：
![](https://pic3.zhimg.com/v2-e69b93743cab5337f012156111836086_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='434'></svg>)



在提取流程图的过程中，IBM的研究员们遇到了一些障碍：他们需要让程序提取论文中所有图表之后，再进行一次分类，找出包含深度学习模型设计的那些，去掉那些和模型相关性不大的描述性图片和展示结果的表格。

但是，论文中介绍深度学习模型设计的图千奇百怪，表格的结构也各不相同。

怎样让程序自动找出有用的图表呢？IBM研究员们人肉处理了论文中的3万张图，将深度学习模型设计图分成了5大类：
![](https://pic3.zhimg.com/v2-635c5e6f04f3b8847e5225369af19092_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='159'></svg>)



**△** 5大类深度学习模型设计图

**1.** 神经元分布图；

**2.** 2D Box：将每个隐藏层表示为一个2D方块；

**3.** Stacked2D Box：将神经网络每一层表示为堆叠的2D方块，表示层的深度；

**4.** 3D Box：将每个隐藏层表示为一个3D立方体结构；

**5.** 表示整个流程的Pipeline plot。

而表格，主要包括横排表示模型设计流程和纵列表示模型设计流程两类。

在此基础上，他们构建了一个细粒度的分类器，来把图表分到上面提到的5类图2类表之中，然后就可以使用OCR等工具将图表中的内容提取出来。
![](https://pic3.zhimg.com/v2-df1f90577d2ebc83aa1689273475379e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='466'></svg>)



**△** 从图中提取内容的过程

图表内容提取出来之后，就可以根据这些信息构建计算图并生成源代码了。
![](https://pic2.zhimg.com/v2-2cc379bff39499864d4a80482034a789_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='315'></svg>)



**△** 池化2D层对应的计算图、Caffe（Protobuf）和Keras（Python）代码

自动生成的代码究竟怎么样呢？

为了对DLPaper2Code进行评估，IBM研究员们创建了一个包含21.6万份设计可视化图的模拟数据集，在这些数据集上的实验显示，上面讲的模型，在流程图内容提取上准确率可达93%。

AI自动化畅想曲

## 上面这个研究。来自IBM印度研究院。

共有五位署名作者：Akshay Sethi、Anush Sankaran、Naveen Panwar、Shreya Khare、Senthil Mani。其中第一作者Akshay Sethi，明年才会本科毕业。目前是IBM印度研究院的实习生。

嗯，实习生又开挂了。中外概莫能外~

但这么一篇清新脱俗的研究，真能在实践中应用么？

在reddit上，不少人还是对这篇论文有点心生疑虑。比方有人觉得这个论文很有意思，但是细细读下来，还是有些地方比较奇怪。但也有人觉得虽然标题有点唬人，但这项研究感觉还是不错。当然也有人直言：浪费时间。

更多的结论还有待时间考验，但归根结底，这些都是AI自动化方向的一种探索。让AI自己搞定AI，让软件自己编写软件，一直都是研究人员追逐的目标。

比方今年5月，Google发布了AutoML。对，就是跟今天正式推出的TensorFlow Lite同天发布的AutoML。

AutoML就是要让神经网络去设计神经网络。谷歌希望能借AutoML来促进深度学习开发者规模的扩张，让设计神经网络的人，从供不应求的PhD，变成成千上万的普通工程师。
![](https://pic1.zhimg.com/v2-4ce76fd6f9b5ec706ac5f4a1ef08fe28_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='292'></svg>)



也是在今年，MIT学者开发出一套系统，能够自动给代码打补丁。

听起来，以后码农越来越好干了呢。其实不是。

要知道，微软和剑桥联合开发了一个系统：DeepCoder。就能够通过搜索一系列代码建立一个完整的程序，可达到编程比赛的水平。而且，这个系统还能通过自我训练能够变得更聪明。未来程序员的饭碗也不是很铁了。

还记得量子位昨天发的那篇报道么？《深度学习蚕食软件，十年内大部分软件工作不再涉及编程》。

前端也别笑。有家公司正在开发和完善一套“pix2code”的深度学习系统。正这个系统能够根据输入的图形用户界面（GUI）截图直接输出对应的代码，从而省去前端手动编写代码的过程。不信？传送门在此。
![](https://pic3.zhimg.com/v2-5786b0c9dfd4a8e006e53ee895756022_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='97'></svg>)



嗯，一个新时代真的来了。

论文获取
![](https://pic3.zhimg.com/v2-77daea94d0bca6e750f25d79fbdbe656_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='308'></svg>)



想要下载这篇论文，可以在量子位微信公众号（ID：QbitAI）后台对话界面，回复：“**p2c**”三个字符即可。

—完—

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




