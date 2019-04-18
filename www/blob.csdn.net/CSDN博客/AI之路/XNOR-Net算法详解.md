# XNOR-Net算法详解 - AI之路 - CSDN博客





2017年08月31日 08:27:51[AI之路](https://me.csdn.net/u014380165)阅读数：9860








论文：XNOR-Net: ImageNet Classification Using Binary Convolutional Neural Networks 

链接：[https://arxiv.org/abs/1603.05279](https://arxiv.org/abs/1603.05279)

代码地址：[http://allenai.org/plato/xnornet](http://allenai.org/plato/xnornet)
**模型压缩和加速**是深度学习算法应用在移动端必须要解决的问题，也是近年来的研究热点，这篇**ECCV2016**的文章就是做这样的事。在这篇文章中作者主要提到两种**二值化网络**：**Binary-Weight-Networks和XNOR-Networks**。Figure1简单列出了这两种网络和标准的卷积网络的差别，根据实验结果，这里主网络应该采用的是AlexNet。**Binary-Weight-Networks通过对权重W做二值化操作，达到减少模型存储空间的目的，准确率影响并不明显（不过后面在ResNet-18上的实验对准确率的影响还比较大）。XNOR-Networks通过同时对权重W和输入I做二值化操作，达到既减少模型存储空间，又加速模型的目的，当然准确率影响也比较明显。**

![这里写图片描述](https://img-blog.csdn.net/20170831081650651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

接下来依次介绍这两种二值化网络，另外这里说的权重是指网络中的卷积层参数和全连接层参数，因为全连接层可以用卷积层代替，所以接下来我都用卷积层来介绍二值化操作。

**Binary-Weight-Networks**
**首先Binary-weights的目的是将权重W的值都用二值表示，也就是W的值要么是-1，要么是1。这个替代过程贯穿整个forward和backward过程，但是在更新参数时候还是采用原来的权重W，主要是因为更新参数需要的精度比较高。**

接下来详细介绍怎么实现和公式推导，公式部分虽然多，但是很简单。一个卷积层的操作可以用I*W表示，I表示输入，维度是c*win*hin，W表示卷积核（或者叫权重），维度是c*w*h。那么当我用二值卷积核B以及一个尺度参数a代替原来的卷积核W，那么就可以得到下面这个式子：

![这里写图片描述](https://img-blog.csdn.net/20170831081806201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里有个圆圈里面带加号的符号表示没有乘法的卷积计算。这里：

![这里写图片描述](https://img-blog.csdn.net/20170831081818326?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意这里a默认是个正数，a和B是相对的，因为如果a和B都取相反数的话，二者相乘的结果不变。前面的B和W表示某一层的卷积操作的写法，因为每一层卷积都包含多个卷积核，因此如果具体到某一层的某个卷积操作，则可以用下面这个式子表示：

![这里写图片描述](https://img-blog.csdn.net/20170831081845377?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下标lk表示第l层的第k个卷积核。这里

![这里写图片描述](https://img-blog.csdn.net/20170831081853990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

既然我们希望用一个尺度参数a和二值矩阵B来代替原来的权重W，那么肯定希望前者能尽可能等于后者，于是就有了下面的优化目标函数，也就是希望下面这个式子中的J越小越好，这种情况下的a和B就是我们需要的。另外这里将矩阵W和B变换成向量，也就是W和B的维度是1*n，这里n=c*w*h，这主要是为了后面推导的方便。

![这里写图片描述](https://img-blog.csdn.net/20170831081912577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因此接下来就是要求a和B的最优值，使得满足上面那个优化目标。**那么这个优化具体要怎么实现呢？**来看作者的推导。上面那个优化函数展开后可以写成下面这个形式。应该比较容易理解。

![这里写图片描述](https://img-blog.csdn.net/20170831081946800?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因为B是一个1*n的向量，里面的值要么是-1，要么是1，所以：

![这里写图片描述](https://img-blog.csdn.net/20170831081956925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

也就是一个常量。同时因为W是已知的，所以：

![这里写图片描述](https://img-blog.csdn.net/20170831082013401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

也是一个常量。另外a是个正数。这些常量在优化函数中都可以去掉，不影响优化结果。就可以得到B的最优值的计算公式（可以结合前面第一个和第二个优化函数看）：

![这里写图片描述](https://img-blog.csdn.net/20170831082040518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

显然，根据这个式子，**B的最优值就是W的值的符号。也就是（这个式子是求B最优值的最终式子）：**

![这里写图片描述](https://img-blog.csdn.net/20170831082106233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

举个例子，当W中某个位置的值是正数时，B中对应位置的值就是+1，反之为-1.

知道了B的最优值，接下来就是求a的最优值。**这里采用上面第2个J函数表达式对a求导，并让求导结果等于0，从而得到最优值。式子如下：**

![这里写图片描述](https://img-blog.csdn.net/20170831082139966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

通过简单换算并利用前面计算得到的B的最优值就可以得到下面这个求a最优值的最终的式子：

![这里写图片描述](https://img-blog.csdn.net/20170831082152559?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**也就是说a的最优值是W的每个元素的绝对值之和的均值。|| ||l1表示一范数。**

因此**Binary-Weight-Networks的算法总结**可以看下面这个介绍：第一个for循环是遍历所有层，第二个for循环是遍历某一层的所有卷积核。通过得到Alk和Blk就可以近似约等于原来的权重Wlk了，**另外在backward过程中的梯度计算也是基于二值权重。**

![这里写图片描述](https://img-blog.csdn.net/20170831082236079?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**XNOR-Networks**

前面介绍的**Binary-weights**是将权重W的值都用二值表示，而接下来要介绍的**XNOR-Networks不仅将权重W用二值表示，而且也将输入用二值表示。**

XNOR又叫同或门，假设输入是0或1，那么当两个输入相同时输出为1，当两个输入不同时输出为0。

我们知道卷积操作就是用卷积核去点乘（element-wise product）输入的某个区域然后得到最后的值，因此假设你的输入是X，卷积核是W，那么我们就希望得到β，H，a和B使得：

![这里写图片描述](https://img-blog.csdn.net/20170831082330731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里是用βH近似表示输入X，另外：

![这里写图片描述](https://img-blog.csdn.net/20170831082343310?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因此就有了下面这个优化式子：

![这里写图片描述](https://img-blog.csdn.net/20170831082357176?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里有个符号是圆圈中有的点号，表示的是element-wise product，也就是点乘（dot product）。如果用Y代替XW，C代替HB，γ=βa，那么优化式子就变成下面这样：

![这里写图片描述](https://img-blog.csdn.net/20170831082440277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这个式子就和**Binary-Weight-Networks部分介绍的优化式子：**

![这里写图片描述](https://img-blog.csdn.net/20170831082449916?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

类似。 
**因此可以利用Binary-Weight-Networks的推导结果来解这个优化式子。根据前面a和B的计算公式，相应可以得到这里C和γ的计算公式（就是下面两个式子的第一个等号）：**

![这里写图片描述](https://img-blog.csdn.net/20170831082529329?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

γ式子的第二个等号是因为|Xi|和|Wi|是相互独立的，所以可以直接拆开。

![这里写图片描述](https://img-blog.csdn.net/20170831082541242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**两个等式的最右端就是4个参数的最优解。以上就是关于输入和权值都二值化的最优值的解。**

**接下来的Figure2是具体的二值操作。**第一行就是前面介绍的**Binary-Weight-Networks**的最优值的求解。第二行是**XNOR-Networks**的最优值的求解，不过因为存在重复计算，所以采用第三行的方式，c表示通道数，A是通过对输入I求均值得到的。第四行其实和第三行含义是一样的，更加完整地表达了XNOR的计算过程，这里的K就是第三行计算得到的K，中括号里面的式子就是刚刚计算得到的最优的C。

![这里写图片描述](https://img-blog.csdn.net/20170831082624259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里第四行有个符号是圆圈里面有个*，表示的是convolutional opration using XNOR and bitcount operation。也就是说正常两个矩阵之间的点乘如果用在两个二值矩阵之间，那么就可以将点乘换成XNOR-Bitcounting operation，从32位浮点数之间的操作直接变成1位的XNOR门操作，这就是加速的核心。

实际添加了XNOR的网络如Figure3所示。设计细节可以参看论文。

![这里写图片描述](https://img-blog.csdn.net/20170831082647603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**实验结果：**

Table1是两种二值化方式和其他二值化方式的对比。最后的Full-Precision是不经过二值化的模型准确率。

![这里写图片描述](https://img-blog.csdn.net/20170831082708260?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Table2是两种二值化方式和正常的ResNet18的准确率对比，**可以看出在ResNet18上Binary-Weight-Networks对准确率的影响也比较大。**

![这里写图片描述](https://img-blog.csdn.net/20170831082729382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其他更多实验结果可以参看论文。




