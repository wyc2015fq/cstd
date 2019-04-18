# Weighted-Entropy-based Quantization for Deep Neural Networks - AI之路 - CSDN博客





2017年09月01日 08:13:59[AI之路](https://me.csdn.net/u014380165)阅读数：1912








论文：Weighted-Entropy-based Quantization for Deep Neural Networks 
[论文链接](http://openaccess.thecvf.com/content_cvpr_2017/papers/Park_Weighted-Entropy-Based_Quantization_for_CVPR_2017_paper.pdf)

**这是CVPR2017的一篇文文章，是用基于weight entropy做深度学习模型quantization的模型压缩和加速算法。**

深度学习模型压缩和加速是近年来的一个研究热点，也是将深度学习模型部署到移动端的必经之路，一方面要压缩模型的大小，减少存储，另一方面要减少模型测试的时间。**其中一个比较有效的方式就是通过降低计算精度来达到模型压缩和加速的目的**。我们知道一般深度学习模型的数值精度都是32位浮点型，那么如果变成8位或者4位存储会怎么样？显然能达到压缩和加速目的，不过不可避免地会影响模型的准确率。因此现在的研究方向都是在不大影响准确率的前提下尽可能加速和压缩模型。

**quantization是深度学习模型压缩的其中一类方法，也就是通过降低计算精度来达到，那么什么是quantization呢**？可以看下面的Figure1。比如说你一个训练好的模型的权重分布像Figure1的左上角那样，**大部分值是0或接近0，这些值小的权重虽然数量多，但是对模型的输出影响并不大；小部分权重的值很大，对模型输出结果影响也比较大，但是数量较少，也就是比较稀疏**。那么linear quantization就像Figure1右上角的图那样（这里都分成24个level），将所有的权重分成24份，然后将值在每一份内的权重用同一个值表示，这种线性的方法会使得quantization后的权重分布比较均匀，并没有考虑原权重的分布。左下角的log quantization将权重接近0的部分分成太多部分了，作者认为这是不必要的，因为值较小的权重对结果的影响较小，没必要分这么多部分。右下角的weighted quantization则是本文提出的quantization方法，是基于weight entropy来对weights和activations做quantization得到的。为什么要这么做呢？来源于作者观察到的一个现象：**the impact of each weight/activation value on the final result should also be considered during the quantization. 而且将quantization的重心放在那些值既不是很大也不是很小的权重上。**

**上面所说的权重不仅仅包含类似卷积层，全连接层等参数，也包含激活层（activation）的参数，这也是你在文中看到的作者既对weight做quantization，也有activation quantization，前者主要是卷积层和全连接层的参数，后者是激活层参数，只不过激活层参数的分布不完全像Figure1那样，因为激活层的输出都是非负的（ReLU）。**

![这里写图片描述](https://img-blog.csdn.net/20170901080807967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**另外我们常看到的binary-weight（比如XNOR-Net）其实是quantization的一种特殊形式，也就是weight只用二值来表示，相当于只有1个bit。**

**那么本文的weighted-entropy based quantization和普通的quantization算法有什么区别呢？our approach is multi-bit quantization, in which weights and activations can be quantized by any number of bits depending on the target accuracy. 也就是说本文的quantization算法更加灵活，可以根据需求决定参数采用多少位的表示方式。另外还有一个特点是：quantization of the full network without modifications to the existing networks.**

接下来先介绍weight entropy的概念，然后分别介绍基于weight entropy的weight quantization和activation quantization。

**Weight Entropy**

weight entropy是本文的核心，公式如下，S就是weight entropy。这里C0，…，CN-1表示把权重分成N个cluster（你可以理解为N个集合或N个簇），因此**Pn就表示有多少比例的权重的值在Cn这一个cluster所包含的范围内**。In表示Cn这一个cluster里所有权重的平均重要性。**i(n,m)表示第n份中的第m个权重的重要性，计算公式也很简单，就是该权重的值的平方**。因为值大的权重分布比较稀疏，值小的权重的分布比较密集，所以值较大的Cn会有较大的In和较小的Pn。

![这里写图片描述](https://img-blog.csdn.net/20170901080935503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Weight Quantization**

明白了weight entropy，就可以看看weight quantization的具体过程了，如下Algorithm1。首先是OPTSEARCH函数，输入是N和w，表示要将权重分成多少N个cluster（N个集合或N个簇）和权重。Nw表示权重的个数。第2,3行表示计算每个权重的重要性，第4行表示对计算得到的重要性进行排序。**第5行表示初始化一系列分割边界，怎么理解呢？举个例子，假如s=[1,2,3,4]，N=2，那么就可以得到初始化的分割边界：c0=0，c1=2，c2=4，也就是将s分成了C0={1,2}和C1={3,4}两份**。然后第6行到第11行是条件语句，里面的第一个循环是遍历所有cluster，第8行表示当我的分割边界ck在ck-1到ck+1这个范围变动的时候，我能得到的最大S是多少，这个S就是前面说的weight entropy。在经过上面的循环操作得到新的分割边界后就进入第12到第15行了，依然是遍历每个cluster，然后第13行是计算每个cluster的平均重要性，**第14行表示根据该cluster的平均重要性Ik计算出Ck这个cluster的representative weight rk，也就是前面Figure1中右下角图的纵坐标！第15行的bk是根据边界重要性s[ck]计算出权重的边界，也就是Figure1中右下角图的横坐标**！最后这个OPTSEARCH函数返回的就是r0到rN-1和b0到bN，分别表示每个cluster的值和边界。第18行是QUANTIZE函数表示输入一个权重wn，就能得到wn所属的cluster的值rk。

![这里写图片描述](https://img-blog.csdn.net/20170901081023658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因此weight quantization的最终效果是什么样呢？答案是：**Our method groups many near-zero values into a large cluster by considering their lower importance. Large, but infrequent values are also grouped into a cluster that covers a wide range of weight values.**

**Activation Quantization**

activation quantization和weight quantization不一样，因为模型的weight在训练完成后就是定值了，在用模型测试图像的时候weight是不会变化的，但是activation不一样，用训练好的模型测试图像的时候其各个激活层的输出都是变化的，和训练时候是不一样的。因此activation quantization不能用weight quantization那种基于聚类的方式。

**在文中作者采用基于对数quantization（LogQuan）方式做activation quantization。作者通过对原来的LogQuan方法做修改，并且改进了原来的求解最优LogQuant参数的方法，得到了Algorithm2**。对LogQuan的改进主要是采用更小的log bases和offsets（分别对应Algorithm2中的step和fsr）。

![这里写图片描述](https://img-blog.csdn.net/20170901081116661?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可能Algorithm看起来有点复杂，建议看看LogQuan这篇文章：[https://arxiv.org/pdf/1603.01025.pdf](https://arxiv.org/pdf/1603.01025.pdf)。我这里也只是简单说下我的理解：激活层的输出是大于等于0的，因此当对激活层输出做划分的时候，等于0的为第一个level，剩下的根据log值分为几个level。比如现在要对激活层（activation）做一个3-bit的quantization，那么第一个level就是0，第二个level是

![这里写图片描述](https://img-blog.csdn.net/20170901081153188?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

第三个level是

![这里写图片描述](https://img-blog.csdn.net/20170901081203061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

依次类推。

**综上，实际对模型做基于weight entropy的quantization是同时对weight和activation做quantization的。**

**实验结果：**
**Figure2是3种不同的网络应用weight ectropy quantization的准确率对比**。横坐标的(1,f)表示weight的bitwidth是1，activation的bitwidth是f（表示不变）；如果横坐标是（3,4）则表示weight的bitwidth是3，activation的bitwidth是4，也就是weight的bitwidth减少了（1-3/32）。显然当两者的值都大时，说明quantization后的weight和activation的精度都比较高，相应的准确率也会高一些。

![这里写图片描述](https://img-blog.csdn.net/20170901081234207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Figure3是关于本文的weight extropy quantization和XNOR-Net（Figure3中用X表示），DoReFa-Net（Figure3中用D表示）的对比。**

![这里写图片描述](https://img-blog.csdn.net/20170901081313102?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

前面介绍的都是所有层都采用统一的bitwidth压缩，但作者发现不同层采用不同的bitwidth压缩对结果影响较大，如Table2。4列的名称含义：monotonically decreasing (DEC), monotonically increasing (INC), concave (CONCAVE), and convex (CONVEX). **从实验结果可以看出当采用CONVEX方式的bitwidth，也就是中间层采用较少的bit的方式可以得到最高的准确率。而如果在前面几层就采用比较少的bit，得到的结果就很差（比如INC）。**

![这里写图片描述](https://img-blog.csdn.net/20170901081338185?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



