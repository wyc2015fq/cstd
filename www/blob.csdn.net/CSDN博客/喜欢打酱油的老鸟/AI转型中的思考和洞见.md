
# AI转型中的思考和洞见 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月10日 08:03:34[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：820标签：[AI 																](https://so.csdn.net/so/search/s.do?q=AI &t=blog)[转型																](https://so.csdn.net/so/search/s.do?q=转型&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=AI &t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://blog.csdn.net/chancein007/article/details/74943322](https://blog.csdn.net/chancein007/article/details/74943322)
![](https://img-blog.csdn.net/20170711180431146?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlaW4wMDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## AI（人工智能）时代已经来临
应公司卓越技术委员会（TEC）的要求，本人一直利用工作之余的时间在学习深度学习和Keras。下面结合我自己获得的信息，给大家分享一下我对AI转型中的一些思考和洞见，我们知道AI（Artificial Intelligence）其实是一个很广泛的概念，根据AI的英文定义，Artificial Intelligence implies a machine that can reason。 翻译成中文的意思就是，人工智能就是一个机器，其能进行逻辑推理。其一般具备下面5种能力：
**Reasoning:***the ability to solve problems through logical deduction*
**Knowledge:***the ability to represent knowledge about the world*
**Planning**:*the ability to set and achieve goal*s
**Communication:***the ability to understand written and spoken language.*
**Perception:***the ability to deduce things about the world from visual images, sounds and other sensory inputs.*
从上面的英文来看，其实人工智能就是研究一种机器，其能够进行自我的逻辑推理，而且能够把现实世界通过知识描述和表达出来，同时还能自我设置目标并达成自己设置的目标；另外，不但能理解书面和口头的语言，还能通过对认知世界中的图形，声音以及其他途径感知的信息进行归纳推演等。
Oh，我的神啊！！！ 这不就是活脱脱的一个人吗？一个高级人类吗？ 在10年前，我们肯定感觉这是可望而不可及的，但是随着alpha go在韩国大败围棋高手之后，其人工智能才慢慢的被巨鳄资本家，卓有远见的政客，企业界，风投界，甚至平民百姓所了解并震惊。

第一次工业革命，蒸汽机改变了世界和人类的生活。第二次工业革命，电力改变了人类的生活方式；紧接着互联网和通信改变了人类的生活方式，正如吴恩达所说的，人工智能也将彻底改变人们的生活方式。
谁曾想到，2012年随着Hinton大师把深度学习应用到了一年一度的ImageNet图像识别竞赛中的并显著提高了图像识别的精度以来，目前为止，通过人工智能算法中的深度学习算法，其图像识别率已经超过96%，而且还超过了人类。
这个看似遥不可及的东西，正在悄悄的改变一些人职业命运并有提前终止了一些以前认为是铁饭碗的职业，比如，富士康正在把一些工厂迁移到美国，因为流水组装线上的很多重复性劳动都能被机器人自动代替，又比如，自动驾驶技术流行和普及以后，路上的交通事故就会显著的减少，保险公司的车险收入也会显著减少，司机也许就会失业；如果人工智能用于广泛应用于癌症的早期诊断和治疗的话，可能就不会出现很多叹息和可惜，很多医生就会失业；如果大家看过微软的最新的应用深度学习实现的同步翻译的话，就会被其技术深深的震撼，其不但能同步中英文翻译，而且还能模仿被翻译人的语音语调。但是大家不要害怕，我们应该拥抱这份变化，因为还有很多的事情，是人工智能替代不了，因为人工智能只能在特定的领域才能进行自我的学习，但是如果是跨知识领域或者涉及到人的一些情感艺术以及创造性方面，人工智能暂时还是无法代替人类的。

## 人工智能 VS 机器学习 VS 深度学习
我们今天经常有听别的同事或者朋友或者公众号提起，人工智能，机器学习，深度学习，那么他们之间到底是什么样的关系呢？
其实机器学习是人工智能的一种实现方法，而且在许多的应用领域应用的非常成功，所以现在比较流行； 说到机器学习，就不得不提深度学习，深度学习是机器学习中的一个组成分支，深度学习是机器学习研究中的一个新的领域，其动机在于建立、模拟人脑进行分析学习的神经网络，它模仿人脑的机制来解释数据，例如图像，声音和文本。其关系如下：
![](https://img-blog.csdn.net/20170720124032228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlaW4wMDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 传统机器学习的经典算法
机器学习分为无监督学习和有监督学习。简单的说，有监督学习，就是不但提供给训练算法的训练数据，而且还告诉其每个训练数据对应的结果或者说标签。让训练有对比，就好比考试中的选择题，有标准答案一样；而无监督学习就是有训练样本，但是每个训练样本不提供对应的标签。靠训练算法自己来进行学习和分类。有点自学成才的味道。那传统的机器学习里面有哪些主要的经典算法呢？请参考下面的图片。
![](https://img-blog.csdn.net/20170720125403960?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlaW4wMDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 机器学习和深度学习的本质是什么？
机器学习和深度学习的本质是什么呢？ 其实就是给定一个输入（比如一张图片），找到一个函数，从而得到一个精确或者准备的结果（识别图像，原来是一只猫）。所以学好数学，包括线性代数，微积分，概率论，是基础。因为我们需要找到一个最佳的模型，也就是函数，从而根据用户提供的问题，我们能够告诉用户想要的答案。
![](https://img-blog.csdn.net/20170720125749706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlaW4wMDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 神经网络 与 深度学习
提到深度学习，就不得不提神经网络，神经网络是深度学习的基础.那什么是神经网络呢？神经网络其实就模仿生物学的中的神经网络，生物学家在研究人类的大脑的时候发现，人的大脑中的神经系统有一个个神经元组成（触突），神经元之间是有选择的进行记忆和遗忘；借鉴这个发现，我们用一套简单的模型模拟了神经网络，也就是人工神经网络（英文：artificial neural network，缩写ANN）入下图示意：
![](https://img-blog.csdn.net/20170725131601968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlaW4wMDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于神经网络，我们需要知道下面几个基本概念：
输入层（Input layer），众多神经元（Neuron）接受大量非线形输入消息。输入的消息称为输入向量。
输出层（Output layer），消息在神经元链接中传输、分析、权衡，形成输出结果。输出的消息称为输出向量。
隐藏层（Hidden layer），简称“隐层”，是输入层和输出层之间众多神经元和链接组成的各个层面。隐层可以有多层，习惯上会用一层。隐层的节点（神经元）数目不定，但数目越多神经网络的非线性越显著，从而神经网络的强健性（robustness）（控制系统在一定结构、大小等的参数摄动下，维持某些性能的特性。）更显著。习惯上会选输入节点1.2至1.5倍的节点。
典型的神经网络具有以下三个部分：
结构（Architecture） 结构指定了网络中的变量和它们的拓扑关系。例如，神经网络中的变量可以是神经元连接的权重（weights）和神经元的激励值（activities of the neurons）。
激励函数（Activity Rule）大部分神经网络模型具有一个短时间尺度的动力学规则，来定义神经元如何根据其他神经元的活动来改变自己的激励值。一般激励函数依赖于网络中的权重（即该网络的参数）。
学习规则（Learning Rule）学习规则指定了网络中的权重如何随着时间推进而调整。这一般被看做是一种长时间尺度的动力学规则。一般情况下，学习规则依赖于神经元的激励值。它也可能依赖于监督者提供的目标值和当前权重的值。
其数学模型，可以用下面的公式表示，
![](https://img-blog.csdn.net/20170725132639167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlaW4wMDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
深度学习就是利用人工神经网络的模型，通过提供的训练数据（结果数据，可选），找出一个最佳的权值，从而找出一个最佳的函数，对于任意的输入，都有近似乎的期望结果；这个就是深度学习模型的本质。
那为什么叫深度学习呢？其实深度学习就是相对人工神经网络的层数而言的，隐含层越多，其深度越深（Deep），虽然其训练的时间成本以及对计算性能的要求越高，但是其精度越高。如下图，所示意，一种方式是不增加其层数，
但是大量增加每层的节点数（Fat +Short），另外一种方式（深度学习），主要增加其层数，但是适度选择其每层的神经元的个数（Thin +Tall）；通过实验结果分析，增加层数的精度和效果远远好于增加其每层的神经元的个数；
加上现代的计算机和网络并行能力大大提高，所以深度学习的模型成为了不二的选择。根据最新的研究，苹果为了能够产生大批量图片数据来训练神经网络，已经使用了人工合成的技术来以假乱真，生成训练图像。果然人类的智慧真是无穷的啊！！！
![](https://img-blog.csdn.net/20170725132741682?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlaW4wMDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 人工智能三大从业领域
说到人工智能，就不得不提人工智能的三大法宝： 大数据，计算能力和算法。 因为我们知道，人工智能其实是通过不断的吃数据，自动的处理数据变聪明的，而且数据训练的越多，其准备度越高。无论是个人企业都逃离不了上面的三大区域。 随着工业的现代化，人类的分工也越来越密切，一个人试图掌握一切知识的能力所付出的成本也越来越高。如果你觉得你对数据的处理比较在行的话，就可以在大数据的获取和分类方面钻研的更深，其实也是投入了人工智能行业的怀抱的；如果你或者贵公司是做硬件设备的，或者有大型分布式处理的设计和使用经验的话，在人工智能的计算方面进行拓展也是非常不错的；如果你觉得你的算法和数学功底很好的话，那么可以在算法方面进行深入的造诣。同时，肯定还有一部分人需要结合上面的三大法宝进行应用开发和参数调优，实际应用AI去解决生产生活的一些问题，这也是一个不错的转型的方向。

## 深度学习的开源框架推荐
深度学习有很多的开源框架，有TenseFlow, Theano,微软的CNTK等等。但是笔者首推Keras，特别是对于入门的用户来说，为什么呢？因为学习曲线低，容易学习，容易上手，而且易于扩展，是快速实现原型的开源的基于Python的深度学习的好框架。
![](https://img-blog.csdn.net/20170720124756655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlaW4wMDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 人工智能的开发语言推荐
对于人工智能以及大数据，当前最流行的应该是Python语言，因为Python语言是面向对象的，而且其原生的血统里面就是用来进行科学计算的，而人工智能的学习和应用过程中往往有牵涉到了大量复杂的数学公式和模型，所以首选语言选Python是不错的，而且Python当前也有很多的开源框架和大公司在背后的支撑和使用,可以帮助用户快速建立模型；另外Python也是一种胶水语言，如果感觉某部分算法的性能比较慢的话，可以十分的方便用C或者C++语言替换。
另外，如果是从事数据分析领域，R语言也是一种非常不错可以用于人工智能领域的语言，R是用于统计分析、绘图的语言和操作环境。R是属于GNU系统的一个自由、免费、源代码开放的软件，它是一个用于统计计算和统计制图的优秀工具。
如果大家在研究生或者博士阶段对Matlab比较熟悉的话，其实也可以用Matlab来进行建模。 所以，如果大家上面三门一门都没有掌握的话，建议选用Python，如果已经有R语言的基础或者Matblab的基础的话，如果用R语言或者MatLab也是一个不错的选择。
![](https://img-blog.csdn.net/20170720191737694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlaW4wMDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 一点学习心得分享
![](https://img-blog.csdn.net/20170817150329593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlaW4wMDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
作为程序员的我们，作为聪明程序员的我们，作为容易接受和学习新鲜技术的程序员的我们，作为站在时代信息前锋的程序员的我们，还等什么？即使不转行，也要学习和了解一下未来可能会颠覆世界的人工智能技术。
我们也许在我们以前犹豫的时候错过了宝贵的移动互联网的成长或者创业机会，也许因为犹豫我们错过了大数据带来的成长或者创业机会，也许由于犹豫我们错过了云浪潮和物联网带来的成长或者创业机会，现在人工智能，就近在咫尺，想抓住吗？能抓住吗？ 光有一颗欲望的心是远远不够的。还需要一些策略和思维：

首先第一步我感觉是要去感知和学习什么是人工智能，深度学习等。只有自己入门了之后，才能知道自己是否真的适合并喜欢这个行业，然后再决定自己是否继续深入下去，还是作为一个吃瓜的群众，了解一下即可。下面是笔者在最近2个月总结的关于深度学习和Keras框架学习的经验，仅供入门参考，不喜勿喷！！！

@ 因为深度学习这门课，需要有一定的数学功底，但是一上来就让大家学习积分，线性代数，概率统计，最优化方法有点让人望而生畏。所以笔者建议，大家可以先到网上找一些简单的视频来看看，比如莫烦的Keras,每个视频讲的很短。而且还有视频和代码，大家可以过一遍，并安装视频把代码敲一遍，哪怕不理解。如果英语好的，可以看斯坦福的人工智能或者深度学习的视频。
@ 在看视频的时候，必须找到一个深度学习的开发框架自己倒腾倒腾，笔者感觉Keras是一个不错的选择，为什么呢？简单。 而且运行Keras的时候，最好在非window机器上安装，比如Ubuntu或者Mac等。否则一旦安装不顺利，则会让人感到很沮丧。
@ 这个时候，可以学学Python。不一定要学的很精通，但是需要知道其中的一些基本用法和概念，比如，数组，元组的处理，方法的调用，类的实例化，而且特别注意Python的语法是非常强大和灵活的，其可以一次性返回多个不同的值。
@学些了Python之后，把高等代数中的微积分，线性代数或者概率论中的一些基本概率了解一下。
@ 如果英文好的话，看一下下面这本入门的书籍，如果英文不好，找它的中文翻译版本。
http://neuralnetworksanddeeplearning.com/
@看了之后，在看台湾大学李宏毅老师的PPT，PPT做的非常的好。其中反向传播这个章节还有视频讲解。
@等把上面的PPT看完之后，在回过头来看看莫烦的Keras视频并把其提供的例子尝试运行一遍。

最后，
祝所有看到这篇文章的朋友们了解人工智能成功！！！！！！！！
祝所有强烈意愿转型的程序员朋友们转型成功    ！！！！！！！ !

## 参考资料
https://morvanzhou.github.io/tutorials/machine-learning/ML-intro/2-2-CNN/
http://neuralnetworksanddeeplearning.com/
https://keras.io/
https://cs.stanford.edu/people/karpathy/convnetjs/demo/mnist.html
http://speech.ee.ntu.edu.tw/~tlkagk/courses.html
https://www.youtube.com/watch?v=a2VlPaHeFgE
https://www.youtube.com/watch?v=cdgQpa1pUUE
https://zh.wikipedia.org/wiki/%E4%BA%BA%E5%B7%A5%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C\#.E4.BA.BA.E5.B7.A5.E7.A5.9E.E7.BB.8F.E5.85.83.E7.BD.91.E7.BB.9C.E6.A8.A1.E5.9E.8B

