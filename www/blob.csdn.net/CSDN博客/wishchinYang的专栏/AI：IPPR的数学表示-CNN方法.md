# AI：IPPR的数学表示-CNN方法 - wishchinYang的专栏 - CSDN博客
2017年07月08日 00:15:09[wishchin](https://me.csdn.net/wishchin)阅读数：997
**前言**：   
随着超量类别PR和高精度的需求，人工特征方法局限性凸显出来，固定的特征hash压缩映射因其压缩损失。表现为特定的特征hash方法，在海量的同类数据集上近邻特性变差，而在不同类别的数据上面隔离性又出现问题。
  
       既然人工构建的特征hash函数并不能满足每一个场景的需求，每个经验都有局限，且**特征提取的压缩映射必然导致压缩损失**，为何不略过此环节，使用数据来完成此过程。越多的数据可生成越精确的分类结果，这就引出了一站式图像处理PR方法——CNN方法。IPPR又从框架分治法回到一站式框架方法。
 
        从2012年AlexNet横空出世，获取制高点之后，图像处理PR进入了CNN方法的汪洋大海，而后以横扫之势统治了整个IPPR界。后来随着类别的增加，和准确度要求的提高，模式函数的参数更多，CNN走向了更大、更深，且在这条路上越走越远。
        接上一篇文章：[AI：PR的数学表示-传统方法PR](http://blog.csdn.net/wishchin/article/details/74780073)
**CNN能干什么**：图像处理的模式识别。此外呢？没有了？
**ANN的全链接**和局部链接：   
原始ANN使用了全链接方法，一般使用一个隐含层的三层ANN网络。参考文章： [ANN：神经网络堆叠进化故事](http://blog.csdn.net/wishchin/article/details/45067177)。
![](https://img-blog.csdn.net/20170709123644843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170709123842895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   
      这显示了判别式识别的一般结构，左图为ANN的三层结构，右图为SVM的一般结构，但结构上的相似性不能掩盖模型的本质区别。
 
        ANN结构的隐含层参数是经验性的，专家性的，一般是试出来的，隐含层的语义是无法分析的，而SVM的隐含层则有明确的意义，每一个节点表示核函数，代表了高维空间中对应的一个支持向量。
        
 模式识别第94页，类似的人们研究发现，任意一个从x到y的非线性映射，都存在一个适当结构的三层前馈神经网络能够以任意的精度来逼近它。人们分别从Kolmogorov定理的角度和函数的傅里叶展开思想来研究这一性质（[Approximation
 by superposition of a sigmoid function](https://link.springer.com/article/10.1007/BF02551274) - SPringer和Elervier才是阻碍人类进步的铁栏杆）。
        
 这就从理论上证明了，任一的模式识别方法，即可以表示为压缩hash函数的方法，都可以使用一个三层前馈神经网络实现，且只要找到一个合适的结构，必能达到想要的精度。
          
 这对于向量机来说，足够多、且足够准的核函数，能否达到识别的任意精度呢？那是可能的，但是在复杂度上，一定超过基于分段线性方法的SV决策树空间划分方法，乘法总是比加减法要慢一些。
**可解释性**：   
固定函数模型的可解释性：不管是逻辑斯特递归、LDA模型，马尔科夫模型，还是SVM，其模型本身的意义，训练过程和训练方法都是可解释的。比如LDA模型假设词汇分布符合隐式狄利克雷模型，使用拟合方法拟合参数，使用残差优化方法进行模型训练。SVM的模型
 则可以可以把训练明确地转化为一个凸优化问题，使用梯度下降法直接进行训练。     
**ANN**：其结构是经验性的，即使当前广泛流行的CNN网络ALexNet和GoogleNet，其结构（多少层 ,每一层多少个神经元 ）都是人造的，且专家也不能明确给出为何使用这种结构，只能告诉你，在我的认知范围里/实验效果里，这种结构对于这个问题是有效的和高效的。
 此外，参考上一篇文章 AI：[](http://blog.csdn.net/wishchin/article/details/74780073)传统PR的数学表示 
 ，对SIFT+VQ场景特征的分析，场景特征提取方法类似与一个两层隐含层的网络，SIFT相当于CNN网络输入到第二层和第二层到第三层的局部链接，而VQ编码相当于第三层到后一层的全链接。而每一层网络，都有其明确的语义即函数解释，并可以通过对函数的分析证明其有效性和高效性，且可以从规则和实验的角度对有效性进行佐证。
 CNN：形式的固定同时也决定了受到限制，没有通用高效的算法适应每一个数据集，且每一种特定的压缩算法都有其最合适的应用范围和压缩损失，即使是广受好评的SIFT特征。所以为何不使用数据驱动来完成类似于特征提取层的功能，使用数据驱动来完成特征提取的功能，使用不受形式限制的局部链接来完成SIFT层的功能，即使不能进行明确的语义解释。
      
 理论上，只是经过ImageNet预训练的AlexNet网络，在未经微调网络的情况下，在对特征提取的准确率上仍然超过了广泛使用的SIFT特征和HOG特征。
特征的层次：CNN已经模糊了特征的限制，例如SIFT+VQ，可以把SIFT算子结果称为特征，也可以把SIFT×VQ复合函数算子结果作为特征。而对于CNN来说，特征的定义不是那么明晰，下图把前四层定义为特征提取，而把后两层定义为模式函数，是可行的，但并非唯一的，依然使用传统思维来分析CNN对图像的处理。
![](https://img-blog.csdn.net/20170712120156311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        
 若使用SIFT+VQ+SVM进行场景检测，可以把特征提取和模式方法串联，从图像到分类结果，最后得到一个类似于CNN结构的4+3层的7层前向网络，但这个7层网络每一层的映射都有明确的解释。
**纤维丛理论**：   
流形：流形是局部具有欧式特性的拓扑空间。希望完全读完马天的《拓扑流形学》![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)
拓扑流形：最容易定义的流形是**拓扑流形**（参考：[流形的详细讲解](http://blog.csdn.net/u010555688/article/details/38981813)），它局部看起来象一些“普通”的[欧几里得空间](http://zh.wikipedia.org/wiki/%E6%AC%A7%E5%87%A0%E9%87%8C%E5%BE%97%E7%A9%BA%E9%97%B4)**R***n*。形式化地讲，一个拓扑流形是一个[局部同胚](http://zh.wikipedia.org/w/index.php?title=%E5%B1%80%E9%83%A8%E5%90%8C%E8%83%9A&action=edit&redlink=1)于一个欧几里得空间的[拓扑空间](http://zh.wikipedia.org/wiki/%E6%8B%93%E6%89%91%E7%A9%BA%E9%97%B4)。这表示每个点有一个[邻域](http://zh.wikipedia.org/wiki/%E9%82%BB%E5%9F%9F)，它有一个[同胚](http://zh.wikipedia.org/wiki/%E5%90%8C%E8%83%9A)（[连续](http://zh.wikipedia.org/wiki/%E8%BF%9E%E7%BB%AD%E5%87%BD%E6%95%B0)双射其逆也连续）将它映射到**R***n*。这些同胚是流形的坐标图。
微分流形：很容易定义拓扑流形，但是很难在它们上面工作。对于多数应用，拓扑流形的一种，微分流形比较好用。如果流形上的局部坐标图以某种形式相容，就可以在该流形上讨论方向，切空间，和可微函数。特别是，可以在微分流形上应用“[](http://zh.wikipedia.org/wiki/%E5%BE%AE%E7%A7%AF%E5%88%86%E5%AD%A6)微积分”。
基底流形：单类样本集。根据流形的定义和结构，我们可以假设每一个类别的样本定义分布在一个特定维度的流形上，且不同类别的流形在此维度必定不会交叉，即是可分离的。这个样本集合作为模式函数的定义域存在，那么我们称之为基底流形。
嵌入：如果多类样本集可以以不交叉的流形的形式嵌入到N维向量空间，则样本集被称为是N维可分离的，则对于每一类样本集合，都是N维可嵌入的。
        线性可分：若是两类样本集可以嵌入到M维向量空间且可以被一个M-1维的线性界面分开，则两类样本集是M维线性可分的。
![](https://img-blog.csdn.net/20170710125559142?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170709133440167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
           
 上图右图中的样本分布在三维欧式空间中，但可视化之后可以明显看出样本分布在一个二维流形上面，这个二维曲面可视为样本的基底流形，且这个样本集合是三维可嵌入的。
          
 上图左图中的样本分布在不明维度的欧氏空间中，且不是二维可嵌入的，各种点集产生了交叉。从各个类别在二维平面的投影大致可以看出，相同的样本分布相对集中，除去噪音干扰，在特定维度是可分离的。上图右图中的样本分布在三维欧式空间中，但可视化之后可以明显看出样本分布在一个二维流形上面，这个二维曲面可视为样本的基底流形，且这个样本集合是三维可嵌入的。
![](https://img-blog.csdn.net/20170709133000265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       
 两类样本集合在二维欧式空间中不是线性可分的，在特定的三维空间中是线性可分的。若是两类样本集可以嵌入到M维向量空间且可以被一个M-1维的线性界面分开，则两类样本集是M维线性可分的。
纤维丛：
             一个纤维丛由四元组（E,B,π，F)组成，其中E,B,F是拓扑空间
             而π： E → B是一个 连续满射，满足下面给出的局部平凡条件。
             B称为丛的基空间，E称为总空间，而F称为纤维。
             映射π称为投影映射.下面我们假定基空间B是连通的。
             我们要求对于B中的每个x,存在一个x的开邻域U，使得π^-1(U)是同胚于积空间U × F的，并满足π 转过去就变成到第一个因子的投影。也就是一下的图可交换：
                   其中Proj1 :U × F → U是自然投影
                   而φ： π^-1(U) → U × F是一个同胚。所有{(U,φ）}的集合称为丛的局部平凡化。
              对于B中每个x,原象 π^-1(x) 和F同胚并称为x上的纤维.一个纤维丛（E,B,π，F)经常记为
            以引入一个空间的短恰当序列。
            注意每个纤维从π： E → B 都是一个开映射，因为积空间的投影是开映射。所以B 有由映射π决定的商拓扑.
        一个光滑纤维丛是一个在光滑流形的范畴内的纤维丛。也就是，E,B,F都必须是光滑流形而所有上面用到的函数都必须是光滑映射。这是纤维丛研究和使用的通常环境。
       参考：[《鬼火燎原》-29-洗澡水中的小女孩](http://blog.sciencenet.cn/blog-677221-742938.html)
![](https://img-blog.csdn.net/20170710105446910?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**模式函数的流形**：模式函数是一个从N维可分离的流形嵌入空间到1维离散个点的映射。
**        中国人群的故乡模式**：过年回故乡是大多数中国人必须要做的事情。通过这一个可以有趣地表述模式函数纤维丛感念。模式纤维丛的基底流形比较复杂，可以在各种维度提取各种特征，比如口音、行业等等各种元素提取特征，寻找流形可嵌入的维度。
![](https://img-blog.csdn.net/20170710123043672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
           散落在各地的人群样本混有各种背景，但故乡模式有一个较为明显的特征，即是祖籍所在地。每一个红点表示一个故乡模式值域（祖籍省会城市）的元素，散落在各地的每一个人都是故乡模式的定义。如果深处探索，会发现特定祖籍的人群总是聚集在一起，或者分布在相似的行业，即分布在特定的流形上。
**CNN方法——一站式方法**
     
     CNN方法最有效的思想是引入了二维结构，可以搭建从直觉到网络设计的直接连接。CNN模型的典型结构.LeNet.AlexNet。
**经典的LeNet结构.6层结构**
![](https://img-blog.csdn.net/20170710145608075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
          
 若对于简单的IPPR问题，若从0开始，一般可以使用类似于LeNet简单的六层网络，使用Caffe可以仅配置参数就可以构建简单的CNN，一般的六层网络是这样设置的：
          
 InPut——>Conv层——>Pooling层——>Conv层——>
 Pooling层/ ReLU整流层+pooling层——>全链接层——>softMax层——>输出类别概率。
          
 卷积核实现特征提取的各种不变性，同时可以完成梯度计算、主方向确定等功能。当然一般来说使用越大的卷积核越能学习全局的特征，且若要学到较为全局的特征，需要使用至少两个卷积层。
**经典的AlexNet结构.8/13层结构**
![](https://img-blog.csdn.net/20170710145624243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        
 图片来自：[我看AlexNet](http://www.jianshu.com/p/58168fec534d?from=androidqq)
**CNN的二维乘法递进结构     **
![](https://img-blog.csdn.net/20170710151330877?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   
        图片来自：[卷积神经网络](http://blog.csdn.NET/stdcoutzyx/article/details/41596663)
** 压缩映射结构分析**
![](https://img-blog.csdn.net/20170710151644473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
      
      上图的左边使用全链接压缩映射  fF( X ) 把1000*1000的输入图像压缩为1*1，为下一层的一个节点数据。上图的右边为局部链接
 fL( X ) ，把一个10*10的输入图像块压缩到1*1，成为下一层的节点数据，作为下一层网络输入，图中有4块10*10的数据，压缩传输到4个下层节点上。
fF(
 X )  和 fL(
 X )
 函数具体形式可根据计算机硬件的不同进行不同的设计，其中最常用的是卷积核-Polling。若某一层的神经元个数为N*N，全链接的卷积核则为N*N，矩阵元素值对应网络参数的值。
       
 CNN控制链接方式和卷积核参数的方法有两种，一种是特定节点参数置为0，一种是直接断开特定节点前向或后项连接。
** CNN的乘法级联**
       
 卷积核的级联结构直接构成了乘法级联结构——一个压缩映射。如下图所示，整个压缩映射 F(X—>Y)={ X = 32*32的所有图像，Y∈{ 1, 2，...n2
 } ，例如下图中的n2为100（表示100个类别）  }
![](https://img-blog.csdn.net/20170710153446733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
            
 传统PR方法： 使用全局特征+编码器+多类分类器的CNN网络。
**CNN迎接多类的挑战**
      
   以ImageNet为例，模式识别的定义域为{ X = ImageNet的图片  }，不管有多少张图片，这些图像分布在 2维欧式空间（也称为2维张量空间）的 L1*L2（L1维向量空间和L2维向量空间乘积，但两个向量空间不再一个欧式空间内）的长宽的图片集，而值域Y={
 1860个类的标记 } 为0维欧式空间甚至向量空间中的离散个点上，即把定义域中浩瀚的二维图片使用模式函数  f( x ) 压缩到 0维向量空间中的1860个离散点上。
特定类型的传统PR方法模式函数的形式是固定的，在理论上产生了特定的“局限性”
 的，分类准确度可以使用PAC学习理论的方法计算出来。特定函数形式的模式识别准确度、泛化误差都受到模型本身VC维的限制。
![](https://img-blog.csdn.net/20170712120156311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       
 SIFT对一定程度内的缩放、平移、旋转、视角改变、亮度调整等畸变，都具有不变性，是当时最重要的图像特征提取方法之一。然而SIFT这类算法提取的特征有本身的局限性，在ImageNet ILSVRC比赛的最好结果的错误率也有26%以上，而且常年难以产生突破。
    
     那么放开形式的限制，使用不受限制的网络来代替特定形式的网络。比如使用不受SIFT函数形式限制的局部链接层取代上图中的SIFT特征提取层，使用数据驱动来完成类似的特征函数设计的功能，得到参数化的“特征提取层”网络，即使参数是无法通用语义解析的。
使用不受限制的全链接层取代稀疏编码和SVM分类器层，使用更多的数据来训练调整连接层的参数，获得“模式识别层”网络，避开编码层和分类器层的模型本身限制，即使参数仅是由训练得到，也是无法进行通用语义分析的。
**总结**
       
 深度学习是图像语义处理突破语法限制，即舍弃“可解释性”的语法，即舍弃特定范畴的函数形式，直接使用语义进行函数参数学习，使用难以具体形式化的参数网络代替特定形式的模式函数。在IP和PR层突破函数形式的限制，获取更精确的模式函数，是多类别和高精度的必然。
