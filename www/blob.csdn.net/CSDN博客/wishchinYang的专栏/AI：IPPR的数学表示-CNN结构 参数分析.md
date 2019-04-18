# AI：IPPR的数学表示-CNN结构/参数分析 - wishchinYang的专栏 - CSDN博客
2017年07月12日 12:15:02[wishchin](https://me.csdn.net/wishchin)阅读数：1325
**前言**：**CNN迎接多类的挑战**
特定类型的传统PR方法特征提取的方法是固定的，模式函数的形式是固定的，在理论上产生了特定的“局限性” 的，分类准确度可以使用PAC学习理论的方法计算出来。特定函数形式的模式识别准确度、泛化误差都受到模型本身VC维的限制。
![](https://img-blog.csdn.net/20170712120156311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
使用不受限制的多层网络取代可以有明确语法形式的传统网络，可以突破特征提取和模式函数的固有限制，也导致了模式识别的黑盒方法——不停的实验，使用更多的数据直至接近遍历，提高训练测试技巧，直到评测结果达到最优。随着类别的增加，和更高准确率的要求，压缩映射函数必须有更小的损失，意味着压缩函数变得更加复杂，形式化的网络和参数化的网络都面临着参数暴涨的必然之路，表现在网络结构上为网络变得更大，每层网络的神经元个数变多。
模式识别第94页，类似的人们研究发现，任意一个从x到y的非线性映射，都存在一个适当结构的三层前馈神经网络能够以任意的精度来逼近它。人们分别从Kolmogorov定理的角度和函数的傅里叶展开思想来研究这一性质（[](https://link.springer.com/article/10.1007/BF02551274) Approximation by superposition of a sigmoid function - SPringer和Elervier这两个出版社对科学论文的过分商业化才是阻碍人类科学进步的铁栏杆 ）。
           可以以任意精度逼近任意函数的三层网络理论上可以描述任意压缩映射，但即使是传统的模式识别方法已经使用了多层网络。三层网描述多类别模式函数需要更多的节点，仅使用两次函数复合达到多类识别和高精度，每一个函数将会极度复杂，全连接层将变得异常复杂，训练更加困难。
           固定的特征提取方法和模式识别方法没有广泛通用到各个场景的形式，但分层的思想是通用的经验之谈，逐层递进也是复杂度分解的有效方式。使用更多层次的映射，即使用相对简单函数进行多次复合来拟合能适用多类和高精度的模式函数，称为DNN方法的一般途径。
弱遍历
           此外，根据乘法组合原理，分层可以有效的降低网络总参数个数。每一个从定义域边缘定义到类别的特定值映射都可被视为一个界面函数，这个函数有且仅有一个纤维组成，从图像到模式的模式映射被所有的纤维包裹在一个超度量空间里，模式函数必须隐式地在所有界面纤维“里面”，模式函数复杂度等同于纤维丛的复杂度，而多类和高精度意味着界面纤维更多，更靠近定义域定义的个数，所以模式函数的负责度更靠近于定义域的个数。最少的描述每个纤维可以用一个二维张量转换矩阵描述，纤维的个数等同于矩阵的个数，多层网络可以得到保证完成相同表示矩阵复合可能的情况下，降低总的网络参数。使用4*3个矩阵复合12个矩阵比直接使用12个矩阵参数降低了5=X×Y-（X+Y）个，层数越多可以降低的参数越多。
         愈来愈多的类别和要求更高的精度要求网络越来越大，越变越胖，而随着参数暴涨，网络训练更加困难，分治法又有了用武之地。为使参数变少，使训练变得相应简单，网络逐渐变深，变得更长。
**逐渐变深/变长的ANN-CNN**
20年前的LeNet网络在手写识别领域效果斐然，而对于更复杂的图像分类如ImageNet级别的数据集，效果就不是那么好了。更深更大的2012年崛起的CNN-AlexNet，以8层的深度取得了很好的效果，力压各种传统分类器。
        6层网络的LeNet只可以在手写数字识别上发挥奇效，在Minist数据集上取得了很好的结果，而面对类别更多的自然图像处理变得更加困难。理论上不断变胖的LeNet也可以在自然图像上取得奇效(三层网都可以做到)，但多类的模式函数使网络映射函数非线性复杂度增加，局部极值点的势需要更多参数的吉布斯采样，使训练过程变得极度困难。8层网络的ALexNet在2012年获得ImageNet-ILSVRC-2012的冠军，把组合随机森林方法远远地甩在了身后。8层网络的ALexNet比LeNet更长，同时也比原始LeNet更胖。
         来看看最近几年的CNN变的多胖多长。
[](http://www.jianshu.com/p/ce609f9b5910)![](https://img-blog.csdn.net/20170716112657947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
          20年前的LeNet只有6层，而2012年不可一世的ALexNet网络层数为8层，终于可以在ImageNet数据集上展现风采，在ILSVRC上取得冠军。
         此后紧随着AlexNet取得成功 ,  VGGNet、Google Inception Net 和 ResNet，这3种网络依照顺序先后出现，排列查看，网络的深度和复杂度也依次递进。
       它们*分别获得了ILSVRC（ImageNet Large Scale Visual Recognition Challenge）比赛分类项目的2012年冠军（AlexNet, top-5错误率16.4%，使用额外数据可达到15.3%，8层神经网络）、2014年亚军（VGGNet，top-5错误率7.3%，19层神经网络），2014年冠军（InceptionNet，top-5错误率6.7%，22层神经网络）和2015年的冠军（ResNet，top-5错误率3.57%，152层神经网络）*。
        此处参考：[CNN浅析和历年ImageNet冠军模型解析](http://www.cnblogs.com/payton/articles/6732130.html)   http://www.cnblogs.com/payton/articles/6732130.html    
**CNN网络参数-有多胖？**
         参考文章：[LeNet参数详解](http://www.jianshu.com/p/ce609f9b5910) ,此文简单地计算一下参数总数。
         参考链接：[Deep Learning（深度学习）学习笔记整理系列之LeNet-5卷积参数个人理解](http://blog.csdn.net/qiaofangjie/article/details/16826849)
        LeNet-5共**有6层**，不包含输入，每层都包含可训练参数；每个层有**多个Feature Map**，每个FeatureMap通过一种卷积滤波器提取输入的一种特征，然后每个FeatureMap有**多个神经元。可训练参数大约有30万个。**
**![](https://img-blog.csdn.net/20170717110817120?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**![](https://img-blog.csdn.net/20170717110741300?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**1. C1**层**是一个卷积层**
          输入图片：32*32         卷积核大小：5*5       卷积核种类：6       输出featuremap大小：28*28 （32-5+1）
          神经元数量：28*28*6       可训练参数：（5*5+1）*6（每个滤波器5*5=25个unit参数和一个bias参数，一共6个滤波器）
           连接数：（5*5+1）*6*28*28
**2. S2层是一个下采样层**
           输入：28*28 采样区域：2*2        采样方式：4个输入相加，乘以一个可训练参数，再加上一个可训练偏置。结果通过sigmoid
           采样种类：6            输出featureMap大小：14*14（28/2）
           神经元数量：14*14*6 可训练参数：2*6（和的权+偏置）
           连接数：（2*2+1）*6*14*14                        S2中每个特征图的大小是C1中特征图大小的1/4
**3. C3层也是一个卷积层**
           输入：S2中所有6个或者几个特征map组合          卷积核大小：5*5          卷积核种类：16           输出featureMap大小：10*10
            C3中的每个特征map是连接到S2中的所有6个或者几个特征map的，表示本层的特征map是上一层提取到的特征map的不同组合
            存在的一个方式是：C3的前6个特征图以S2中3个相邻的特征图子集为输入。接下来6个特征图以S2中4个相邻特征图子集为输入。然后的3个以不相邻的4个特征图子集为输入。最后一个将S2中所有特征图为输入。
            则：可训练参数：6*（3*25+1）+6*（4*25+1）+3*（4*25+1）+（25*6+1）=1516
            连接数：10*10*1516=151600
**4. S4层是一个下采样层**
            输入：10*10     采样区域：2*2  采样方式：4个输入相加，乘以一个可训练参数，再加上一个可训练偏置。结果通过sigmoid
            采样种类：16   输出featureMap大小：5*5（10/2）     神经元数量：5*5*16=400
            可训练参数：2*16=32（和的权+偏置）    连接数：16*（2*2+1）*5*5=2000
            S4中每个特征图的大小是C3中特征图大小的1/4
**5. C5层是一个卷积层**
            输入：S4层的全部16个单元特征map（与s4全相连）       卷积核大小：5*5         卷积核种类：120       输出featureMap大小：1*1（5-5+1）
            可训练参数/连接：120*（16*5*5+1）=48120
**6. F6层全连接层**
          输入：c5 120维向量     计算方式：计算输入向量和权重向量之间的点积，再加上一个偏置，结果通过sigmoid函数
          可训练参数:84*(120+1)=10164；
    F6层有84个单元（之所以选这个数字的原因来自于输出层的设计），与C5层全相连。有**10164个**可训练参数。如同经典神经网络，F6层计算输入向量和权重向量之间的点积，再加上一个偏置。然后将其传递给sigmoid函数产生单元i的一个状态。
**AlexNet等更复杂的网络参数分析**
        段落摘抄：[深度 | ImageNet 历届冠军架构最新评析：哪个深度学习网络最适合](http://www.sohu.com/a/120017287_473283)你     
**AlexNet**包含了6亿3000万个连接，6000万/60M个参数（是LeNet的200倍）和65万个神经元，拥有5个卷积层，其中3个卷积层后面连接了最大池化层，最后还有3个全连接层。AlexNet以显著的优势赢得了竞争激烈的ILSVRC 2012比赛，top-5的错误率降低至了16.4%，相比第二名的成绩26.2%错误率有了巨大的提升。
![](https://img-blog.csdn.net/20170717111039233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170819231105275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
           图片： [http://www.jianshu.com/p/58168fec534d?from=androidqq](http://www.jianshu.com/p/58168fec534d?from=androidqq)   参数众多的全链接层，浪费！        
         AlexNet可以说是神经网络在低谷期后的第一次发声，确立了深度学习（深度卷积网络）在计算机视觉的统治地位，同时也推动了深度学习在语音识别、自然语言处理、强化学习等领域的拓展。
**VGGNet**是牛津大学计算机视觉组（Visual Geometry Group）和Google DeepMind公司的研究员一起研发的的深度卷积神经网络。VGGNet探索了卷积神经网络的深度与其性能之间的关系，通过反复堆叠3*3的小型卷积核和2*2的最大池化层，VGGNet成功地构筑了16~19层深的卷积神经网络。
          而VGG参数个数进一步增加，接近130M个，相对于ALexNet，没有变宽，体型瘦了不少。
![](https://img-blog.csdn.net/20170716120026076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170716120037974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Google Inception Net**首次出现在ILSVRC 2014的比赛中（和VGGNet同年），就以较大优势取得了第一名。那届比赛中的Inception Net通常被称为Inception V1，它最大的特点是控制了计算量和参数量的同时，获得了非常好的分类性能——top-5错误率6.67%，只有AlexNet的一半不到。
        Inception V1有22层深，比AlexNet的8层或者VGGNet的19层还要更深。但其计算量只有15亿次浮点运算，同时只有500万的参数量，仅为AlexNet参数量（6000万）的1/12，却可以达到远胜于AlexNet的准确率，可以说是非常优秀并且非常实用的模型。
        此时，网络变深的乘法因式分解起了作用，直接把参数降低到ALexNet的1/12，网络变得瘦长。
**残差网络ResNet**，[Deep Residual Learning for Image Recognition](https://arxiv.org/abs/1512.03385)中提出一种residual learning的框架，能够大大简化模型网络的训练时间，使得在可接受时间内，模型能够更深( 152甚至尝试了1000 )，因此能够获得更好的全局表征，该方法在ILSVRC2015上取得最好的成绩。
残差连接（Residual connections ）允许模型中存在shortcuts，可以让研究学者成功地训练更深的神经网络（表达更多的 边缘参数，能够获得更好的表现），这样也能明显地简化深度网络模块。
          文章翻译：[图像识别的深度残差学习](https://www.leiphone.com/news/201606/BhcC5LV32tdot6DD.html?viewType=weixin)
**Google Inception V4**研究了Inception模块结合Residual Connection能不能有改进？发现ResNet的结构可以极大地加速训练，同时性能也有提升，得到一个Inception-ResNet v2网络，同时还设计了一个更深更优化的Inception v4模型，能达到与Inception-ResNet v2相媲美的性能。
       下图显示了Google Inception 网络的改进对比：
![](https://img-blog.csdn.net/20170716202915255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        下图为出类拔萃的众多CNN网络的参数分布和层数概况。
![](https://img-blog.csdn.net/20170716201227304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       2016年之后，CNN方法的改进呢？海康威视不是拿到了场景分类的冠军吗?
**AI总的路线**
       CNN广泛用于图像识别和图像检测，取得了显著的成果，并应用于工程领域，催生了价值几十亿美元的单个企业，产生了巨大的社会价值。是AI研究、工程化及社会价值实现的先行者。
       AI模型的线路图为：模型结构的探索和模型参数的优化。在CNN以及各个领域，总有一个相对明确的线路图，即结构的复杂化以及适应性的增强，这样逐步导致一个整体构架复杂而局部稠密且特定链接稀疏的模型。
       这个模型应用于不同的系统会产生特定快速的局部结构选择，而选择部分结构之后稠密部分能带来接近最优的结果，在处理各种场景时能够相对快速的取得明显的效果，即智能。
