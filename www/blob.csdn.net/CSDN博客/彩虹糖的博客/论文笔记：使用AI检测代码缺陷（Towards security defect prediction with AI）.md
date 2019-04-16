# 论文笔记：使用AI检测代码缺陷（Towards security defect prediction with AI） - 彩虹糖的博客 - CSDN博客





2018年12月08日 15:13:15[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：687








# Towards security defect prediction with AI



## 文章基本概况

作者：Carson D. Sestili，William S. Snavely， Nathan M. VanHoudnos。三位作者均来自卡内基梅隆大学。

时间：2018年8月提交到了arXiv。

原文链接：[Towards security defect prediction with AI](https://arxiv.org/abs/1808.09897)

源码链接：[sa-bAbl](https://github.com/cmu-sei/sa-bAbI)

源码运行指南：[用于代码缺陷检测的数据集生成](https://blog.csdn.net/caozixuan98724/article/details/84646592)



## 论文摘要与笔记



### 背景与目的

伴随着人工智能的兴起，人工智能这项技术手段，被用于各行各业。作为这项技术最先的使用者——程序猿，肯定会想到使用这项技术来方便自己检查代码漏洞。虽然现代编译器在检测代码漏洞方面已经十分先进，但仍然存在很多问题，论文中提到，传统的编译器都是静态的，尽管其拥有很高的准确率，但是召回率很低，这是因为代码中存在很多的控制结构，或者像random函数这样的不确定性因素，在这个时候，传统的编译器就会失效，不能准确做出类似“数组越界”等的警告，因此，作者想要将人工智能，这个目前最流行最好用的技术用在代码缺陷检测这个领域。

然而理想很美好，现实很骨感，将AI用于代码缺陷的检测存在种种困难。其中，最大的困难在于数据集，在代码缺陷检测领域，我们缺乏大量的，有标注的数据集（这个工作工作量实在是太大了）。在论文中，作者提到了这两个工作，一个是[Juliet test suite](https://samate.nist.gov/SRD/testsuite.php)，这个代码缺陷的数据集存在的问题是数据量太小，而且错误太复杂，现有的人工智能手段还没有发展到可以处理这么小还这么复杂的数据集的程度（后面实验有提到，把论文中的方法用到该数据集，模型无法收敛），另一个是[End-to-End Prediction of Buffer Overruns from Raw Source Code via Neural Memory Networks](https://arxiv.org/abs/1703.02458)，这篇论文的贡献有两个，一个是生成的缓冲区溢出代码缺陷集，一个是使用memory network证明了使用AI检测代码漏洞是可行的，但是这篇论文生成的代码是伪代码，并且没有控制结构。因此，在作者的这篇论文中，作者首先提出方法生成有控制结构的真实的C代码，并且自动生成标注，然后使用memory network验证其在自己生成的数据集上的表现。



### 方法介绍

**一. 数据集的生成**

关于数据集的生成，作者引用了Facebook在对话系统设计中的研究成果——[TOWARDS AI-COMPLETE QUESTION ANSWERING : A SET OF PREREQUISITE TOY TASKS](https://arxiv.org/pdf/1502.05698.pdf)，感觉实际的方法并不复杂，原文中是在生成对话系统中基本的问答环节，比如问位置，问时间。可以把这个过程想象成一个文字冒险游戏的过程，每一段话其实都是由对应的模板生成的，比如下面这样的模板

![](https://img-blog.csdnimg.cn/20181208104950840.png)

中间的人物，对象都是可以随机生成的，而问题的答案，是可以设计一个确定的程序，推断出来的，通过这种方式，我们就可以生成大量的问答对话，交给AI去训练。

本论文中数据集生成的方法，与之类似。我们看一个例子：

![](https://img-blog.csdnimg.cn/20181208105255630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

外面的OTHER不重要，重要的是main中的部分，BODY代表代码中绝对不含有缓冲区溢出缺陷的部分，而对于可能出现缓冲区溢出的部分，有四个标签：

1. BUFWRITE_COND_SAFE：这一行涉及的变量有经过控制结构，并且是安全的。

2. BUFWRITE_COND_UNSAFE：这一行涉及的变量有经过控制结构，并且是不安全的。

3. BUFWRITE_COND_SAFE：这一行涉及的变量没有经过控制结构，并且是安全的。

4. BUFWRITE_COND_SAFE：这一行涉及的变量没有经过控制结构，并且是不安全的。

注意标签中带“COND”的，代表其涉及的变量经过了控制结构，因此这里的代码有可能出现缓冲区溢出，也有可能不出现。

根据规则生成代码，我们需要模板，其实模板中主要包含这么几个方面的内容

1. 控制语句（if/else）

2. 循环语句（for/while）

3. rand()随机数生成函数

4. 变量声明

5. 变量赋值

6. 变量加一(i++)

7. 数组声明

8. 数组元素赋值

下图展示了源码中的部分模板：

![](https://img-blog.csdnimg.cn/20181208111539309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

根据这个模板，我们也可以比较容易的写出代码去标注是否存在缓冲区溢出，具体详情可以参考源代码。

源代码也有分析笔记：[用于代码缺陷检测的数据集生成](https://www.zybuluo.com/yinnner/note/1361724?tdsourcetag=s_pctim_aiomsg)

**二. Memory Network**

1. 数据预处理

 (1) 将文件以行为单位切分，再把行切分成token序列。

 (2) 每行最后再加一个特殊token代表行号

 (3) 将每一个独特的token映射成一个整数

 (4) 将上述所有的数据变成一个F*N*J的数组，F代表文件数，N代表每行允许的最长token序列长度，J代表每个文件允许的最大行数，到不了最长token的，最大行数的地方全部填充0.

2. 神经网络的结构

这里看的朦朦胧胧的。

![](https://img-blog.csdnimg.cn/20181208114420642.png)

![](https://img-blog.csdnimg.cn/20181208114437241.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181208114452196.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

简单理解一下，首先这个网络的输入就是前面预处理得到的3维数组和对应的标注。后面呢，作者把token序列转化成了向量，然后把这些东西丢到了我看不懂的神经网络中，做了分类。

关于memory network可以看看其他的博文，目前我还不大理解。

[记忆网络：memory network](https://blog.csdn.net/irving_zhang/article/details/79094416)



### 实验结果

在实验结果部分，作者主要讨论了三个问题：静态分析器和记忆网络在生成数据集上表现如何？静态分析器和神经网络在生成数据集上对比如何？与静态分析器相比，记忆网络的独特性与优势在哪里？

（1）静态分析器和记忆网络在生成数据集上表现如何？

一句话总结，与静态分析器相比，记忆网络在召回率上优势明显，在准确率上惨不忍睹。

下面一张图分别展示了召回率和准确率：

![](https://img-blog.csdnimg.cn/20181208115434136.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

实验结果其实很好理解，静态分析器的缺陷是死板，好处也是死板。连静态编译器都说你错了，你一定是错了。但是神经网络是一个学习的过程，很多东西很难解释，其对于代码的缺陷可以说是一知半解，很难再准确率上和静态分析器抗衡。

（2）静态分析器和神经网络在生成数据集上对比如何？

在这里，作者主要表达了静态分析器不需要训练，而神经网络需要大量标注数据的意思。

对于神经网络，作者对比了不同大小的训练集得到的训练结果在完整测试集和完备子集上的表现，结果如下图：

![](https://img-blog.csdnimg.cn/20181208144336764.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

可以看到，伴随着训练集的扩大，预测不仅越来越准确，训练的模型收敛的也越来越可靠，这证明了数据集的大小对这一方法的重要性。

（3）与静态分析器相比，记忆网络的独特性与优势在哪里？

这一部分不仅分析了优势，更是分析了劣势啊。

首先，记忆网络不仅能判断代码是否有缓冲区溢出的缺陷，并且还可以给出是一定存在缺陷还是在特定输入条件上存在缺陷的判断，这是静态分析器做不到的。这说明记忆网络学习的其实是代码的逻辑结构，基于对if，while等的判断来决定代码是否存在缺陷，存在什么样的缺陷。

但是这种记忆网络无法泛化。用这种方法训练的结果只能用在相同的方法生成数据集上，用在其他的代码，比如真实代码环境中，效果很差。之后，作者简要分析了一下原因，比如把token序列映射成一个个整数的不合理性等等。

**展望**

根据作者目前的研究，作者给出了三个发展方向：1.继续发展静态分析器，想办法提高它的召回率。2. 提升AI系统，让它至少能完美解决当前生成的数据集的代码缺陷。3. 丰富代码缺陷的种类，改进生成数据集的手段，产生更多更接近现实的生成代码。

针对后两个方向，作者提到可以改进代码的表达手段，还有改进神经网络结构，让这个网络可以直接接受整数作为训练参数。

作者希望通过这样的迭代，不断提升AI检测代码漏洞的能力。

### 读后感受

其实在展望里面作者已经把问题说的比较明白了，代码缺陷检测确实是一个比较难做的事情，它一定需要大量标注的数据，但是标注数据的难度和工作量确实很大，因此人们把目光转向生成数据集，但是生成数据集的问题同样很大。一个是代码太简单，基于规则产生的代码还是太单一了，训练很容易产生过拟合，根本不能泛化，一个是漏洞的种类有限。在这里讨论这个事情的时候，一方面我们通过自己的知识去丰富这个数据库，加规则，加一些新漏洞的产生方式，也可以提高生成代码的质量，另一方面，现在也有生成代码的神经网络，不知道可不可以同样用神经网络去学习有缺陷的代码然后去自动产生有缺陷的代码，不知道这样生成的代码是否是可控的。另一方面，关于代码的表达，把token映射成整数确实比较落后，在论文中也给出了一些可以考虑的方向，比如code2vec。不过对于这个方向的改进我也是比较存疑的，感觉似乎难度也比较大，简单看了一下那篇code2vec的摘要，感觉和上面我们要处理的问题关系不是太大。

最后总结，这篇论文针对目前使用AI做代码缺陷检测的问题，用对话系统生成数据集的方式生成了简单的含有缓冲区溢出漏洞的代码集，并把其用于神经网络的训练，虽然不能解决实际问题，但是，是在AI代码缺陷检测这个领域做出的一个有意义的尝试。



