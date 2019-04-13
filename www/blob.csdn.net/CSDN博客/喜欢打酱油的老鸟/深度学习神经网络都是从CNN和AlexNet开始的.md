
# 深度学习神经网络都是从CNN和AlexNet开始的 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月25日 08:18:41[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：170


[https://www.toutiao.com/a6649718938127041031/](https://www.toutiao.com/a6649718938127041031/)
2019-01-23 23:45:40
在直接进入AlexNet之前，让我们先了解神经网络的一些技术细节。
我们将按以下顺序进行讨论：
卷积神经网络
ReLu非线性
局部响应归一化
Drop Out层
PCA颜色增强
AlexNet架构
学习机制
**CNN （图像分类背后的智慧）**
卷积神经网络是目前为止图像分类和计算机视觉相关问题的最佳解决方案。
CNN使用过滤器（内核）来区分图像中的边缘。如下所示：
![深度学习神经网络都是从CNN和AlexNet开始的](http://p1.pstatp.com/large/pgc-image/a83ed83a2c9440379588472b2002417e)
看看上面的GIF，它显示了过滤器（权重）如何将图像矩阵转换为新矩阵的工作方式。现在让我们假设过滤矩阵V和下面给出的另一个过滤矩阵H. 尝试使用这两个过滤器图像I进行卷积。
当与输入图像进行卷积时，如果行上的变化很大，滤波矩阵V就会给出较大的值；如果列上的变化很小，滤波矩阵V就不会产生任何影响。从上面的案例中可以看出。像素值中沿着行的值实质上表示垂直方向上的边缘，同理，沿着列的的值实质上表示了水平方向的边缘。因此，CNN过滤器以非常特殊的方式确定图像中的边缘。这些过滤器的权重是由模型学习，并且这些权重可以是任何数字集合，这些数字可以尝试学习和检测图像中任何给定角度的边缘，该模型看起来便于分类。
对于RGB图像，过滤器可以是一维甚至是三维的，甚至可以有多个过滤器用于从图像中的不同方向提取信息。
具有一个过滤器的卷积输出将始终产生二维结果，并且对于同一图像上的不同过滤器将得到一个二维矩阵，当堆叠在一起时将给出三维结果，深度=过滤器数量。
注意：任何过滤器的深度始终等于应用它的矩阵的深度。
请参阅下面的GIF，以使用三维过滤器可视化卷积。
![深度学习神经网络都是从CNN和AlexNet开始的](http://p9.pstatp.com/large/pgc-image/4f18494547eb48ebbf72ba104627cf57)
在卷积过程中，尺寸为（3,3,3）的三维过滤器贯穿整个图像以产生一个二维结果。
使用多个这样的过滤器，并且当堆叠在一起时给出三维结果，输出矩阵的深度等于所使用的过滤器的数量。
注意： 每个过滤器的大小必须相同才能将输出堆叠成三维矩阵，并且每个过滤器的深度必须等于输入图像的深度才能进行卷积。在这种情况下，图像深度为3，因此过滤器的深度也必须为3。
**CNN的更多概念如下：**
**Stride（步幅）**-它实际上是图像中每个过滤器应用于图像的步骤。在上面的例子中，Stride是1。下面是stride = 2的卷积的例子。
**Padding（填充）**
在这种情况下，角落周围的图像用0填充，以便保持输出矩阵的形状不变，或者与没有填充的矩阵相比减少较少的数量。
有效卷积：无填充的卷积称为有效卷积。
相同的卷积：如果我们想要将卷积结果的维数保持为与该层的输入维数相同，我们使用相同的卷积。它是在填充的帮助下完成的，选择填充深度使得输入高度和宽度保持与矩阵的输出深度和宽度相同。（注意 ：深度可以不同，它仅取决于使用的过滤器的数量。稍后将讨论输出维度随过滤器大小的变化。）
所有上述概念填充和步幅都使用下面的GIF显示。
![深度学习神经网络都是从CNN和AlexNet开始的](http://p3.pstatp.com/large/pgc-image/8cd5d1f5175f43d880289ac19014f8e6)
带填充的卷积，stride = 2和多维图像以及多个过滤器
**Pooling（池化）**
我们通常执行池化以减小维度。这使得我们能够减少参数的数量，这既缩短了训练时间，又避免了过度拟合。池化层可以独立地对每个特征映射进行下采样，降低高度和宽度，保持深度不变。一种重要的池化类型是**MAX POOLING**（最大池化）。
**Max pooling**基本上是从过滤器决定的图像的给定区域中获取最大值。如下所示。
![深度学习神经网络都是从CNN和AlexNet开始的](http://p1.pstatp.com/large/pgc-image/57b7c3930d044b81930e2ba38439b112)
stride = 2
![深度学习神经网络都是从CNN和AlexNet开始的](http://p1.pstatp.com/large/pgc-image/d459ea859e594f648977bb17b9ca6a9e)
stride = 1
Max pooling目前最流行的池化类型。它主要是通过在过滤区域中存储最大值来帮助保留最活跃的神经元信息。这也可以称为通过仅将重要值传递到下一层来防止过度拟合。
使用的其他类型的池化是 -**Average Pooling（均值池化（mean-pooling））**。它计算过滤区域中值的平均值。我个人认为他的缺点是，因为它平均所有神经元的信息，区分高度活跃的神经元和消极神经元完全失败的。因此，它变得容易过度拟合。只是个人意见。
**Overlapping pooling（重叠池化）**：重叠池化是指stride-size(s) < filter-size (f)时的条件。这将导致相同的输入在多个实例中通过相同的过滤器。请参阅上面的第二个GIF（max-pooling）stride =1。
**Non-overlapping pooling（非重叠池化）**：这指的是当stride-size(s) > = filter-size (f)时的条件。因为这使得任何一个像素在任何一个过滤器中只被计数一次。请参阅上面的第一个GIF（max-pooling）。
尺寸变化：通过过滤器时的输出大小由给定的公式决定。这里，s = stride，f =filter size（考虑square filter），n =input dimensions（长度或宽度），p =padding。
![深度学习神经网络都是从CNN和AlexNet开始的](http://p1.pstatp.com/large/pgc-image/142de1b149b74d92921a8a285604b467)
需要注意的一点是，池化层只有超参数，没有参数。
![深度学习神经网络都是从CNN和AlexNet开始的](http://p3.pstatp.com/large/pgc-image/8efa3958b89446249cd4c6706adeb539)
**完全连接的层**-这是传统的神经网络，其中一个层中的每个节点与前一个层中的每个节点相连接。
与具有相似大小的层的标准前馈神经网络相比，CNN具有更少的连接和参数，因此它们更容易训练。
在Alex-Net中可以看到较少数量的参数，因为它的8个层中有5个是卷积层3个是完全连接的，但仍然只有1％的参数由卷积层贡献。
使用CNN而不是完全连接层的优点是：
参数共享 - 在图像的一部分中有用的特征检测器（例如垂直边缘检测器）可能在图像的另一部分中也有用，因为过滤器在整个图像中都是使用的。
连接的稀疏性 - 在每个层中，每个输出值只依赖于少量的输入。
这些属性为CNN提供了translational in-variance，也就是说移动图像不会影响模型，因为在整个过程中都应用了相同的过滤器。
在AlexNet实现中，还实现了许多其他层。
让我们深入研究AlexNet中使用的其他层。
**ReLU非线性：**
神经网络需要非线性函数来捕获数据中的非线性依赖关系。
![深度学习神经网络都是从CNN和AlexNet开始的](http://p9.pstatp.com/large/pgc-image/facbeeccb5734e8d911506856ae03add)
ReLU非线性不会像其他像sigmoid或tanh函数那样饱和，sigmoid函数在两个极端都饱和，而ReLU则不是。
![深度学习神经网络都是从CNN和AlexNet开始的](http://p1.pstatp.com/large/pgc-image/b67d01e2063645839a086f280dd9e6ce)
在梯度下降训练时间方面，这些饱和非线性比非饱和非线性慢得多。ReLU函数（实线）达到25%的的误差率比非饱和非线性tanh函数（虚线）快6倍。
局部响应归一化（Local-Response Normalisation）：
这一层是AlexNet的独特之处。虽然现在还没有使用它，但我认为理解这个概念很重要，因为谁知道它有可能对你的模型有用。
这个概念可以通过考虑神经科学的概念来理解，神经科学的概念陈述了横向抑制的概念，其中相邻的神经层被抑制，以便产生局部最大值以在该区域中产生对比并因此增加感知强度。同样的现象是通过使用局部响应归一化实现。这为使用不同内核计算的神经元输出之间的大活动创造了竞争。这种机制通过抑制不重要的特征来帮助减少过度拟合。
LRN的数学表示如下所示：
![深度学习神经网络都是从CNN和AlexNet开始的](http://p1.pstatp.com/large/pgc-image/7b2b85d33c2549579d8736f8c422a2ba)
局部响应归一化还试图抑制输入值，因为ReLU的输出可能非常大。
ReLU具有理想的属性，它们不需要输入归一化来防止饱和，因此LRN在ReLU之后应用。
和在相同的空间位置上运行在相邻的内核映射上，最多可以包含该位置上的所有内核(过滤器)映射。
让我们用一个例子来解释上面这一行：
假设您有5个过滤器，那么您将获得深度为5的3维体积卷积。LRN将对给定（x，y）处的值进行归一化，但但是通过深度中的所有元素并在z的不同值上替换这些(x,y)处的值。
归一化邻域的大小实际上定义了要归一化求和的样本数量。也就是说，在上面的例子中，我们不是对所有5个样本求和（即邻域（n）= 2），我们甚至可以只对n=1即邻域的3个样本求和。
**Drop-Out层：**
Drop-Out本质上是一个规范化的层，广泛用于计算机视觉领域。
假设您基于硬币投掷随机选择一层中的一半节点并丢弃它们，这样它的所有连接都会丢失，并且这些节点既不用于正向传播也不用于反向传播，这样我们就得到了一个更小的网络。那么你刚刚做的这个过程将以0.5的概率实现Drop-Out。
![深度学习神经网络都是从CNN和AlexNet开始的](http://p9.pstatp.com/large/pgc-image/666b44186b5e40eb8b0dabf07b2bccd3)
左 - 没有drop-out；右-输入prob=0.5实现drop-out，第一层prob=0.6，
dropout层接受超参数，该超参数是称为保持概率（keep-prob）的概率值，位于（0,1）之间。保持概率的值决定了训练过程中正向和反向传播时随机选择的样本所占的比例。例如，参见图。如果keep-prob = 1，则表示考虑了所有节点，并发生了dropout。然而，如果keep-prob = 0.1，则在该层中丢弃所有节点的90％，而训练只在该层中剩下的10%的节点中进行。
为什么drop-out有效——drop-out有效是因为它消除了我们的模型对神经网络中某些特定输入或特定节点的依赖，并阻止模型学习神经元的复杂协同适应，因为神经元不能依赖于特定其他神经元的存在。通常情况下，我们主要在计算机视觉任务中没有足够的数据，在这些情况下也可能会有所帮助。
到目前为止，我们已经了解了AlexNet中实现的所有层和背后的原因。我们只剩下一个技术细节称为数据增强和一种特殊类型的增强称为PCA颜色增强。
**数据增强：**
减少图像数据过度拟合的最简单和最常用的方法是使用label-preserving变换人为地扩大数据集。
一些常见的数据增强技术：
镜像 - 沿水平和垂直轴进行镜像。
随机裁剪 - 例如：将256 x 256分批裁剪成四个224 x 224。
颜色转换 - 随机改变图像的颜色（RGB通道）。
完成所有这些转换时请记住，我们仍然保留了图像的label-preserving特性。
在AlexNet中应用的一种非常著名的颜色转换是PCA颜色转换。
**PCA颜色转换：**
简单地说，主成分分析的颜色移动可以理解为改变像素对应的颜色比例的方差或说其值的传播。这意味着假设红色通道中的像素从10到255变化，而绿色通道从150到200变化，而对于蓝色通道从190到195变化，那么每个颜色通道中相应的变化对于红色最大，然后是绿色，然后是蓝色通道即与其方差成比例的种类。
假设我们有一个mxn维彩色图像。然后我们有相应图像的mxnx3维矩阵。我们将每个图像转换为（m * n，3）维的二维矩阵。这样我们在每列中都有m * n个元素和3个这样的列。我们可以计算该矩阵的协方差矩阵（它是一个3x3维的矩阵，列数是3）。
下一步是通过计算协方差矩阵的特征向量，按特征值的降序排序，计算出RGB颜色样本的最大方差方向。这就给出了RGB彩色通道的主要组成部分。
在色彩增强图像的旁边，我们取特征向量与相应特征值的点积乘以一个随机数。（三种颜色的随机数不同，取正态分布）
这形成了我们想要添加到规范化图像数据的δ（delta）。（记住我们在计算PCA之前规范化数据）。
δ的数学公式如下，
![深度学习神经网络都是从CNN和AlexNet开始的](http://p3.pstatp.com/large/pgc-image/a8210d527a94400e8311e97448d375a7)
δ值显示在上面。这个值被加/减以获得新的颜色值。这里p表示特征向量，α表示随机正态值，λ表示特征值。
接下来，我们将这些数据转换回原始形式，即对数据进行反规范化。
该方案近似地捕获了自然图像的一个重要特性，即物体的特征不随光照强度和颜色的变化而变化。
上述过程最终输出PCA颜色增强数据，如下所示。下面的两张图片是由我使用下面提到的代码生成的。仔细观察你会发现第二张图像比第一张图像亮一点。
![深度学习神经网络都是从CNN和AlexNet开始的](http://p1.pstatp.com/large/pgc-image/d4edd6ea1f36476cbb6aa72acf4e70e7)
原始图像
![深度学习神经网络都是从CNN和AlexNet开始的](http://p9.pstatp.com/large/pgc-image/5a345cae4ded46e298dac7661b62d879)
PCA颜色增强图像
\#importing libraries
import numpy as np
import cv2 as cv
\#read in an image using opencv
original_image=cv.imread('abc2.jpg')
\#first you need to unroll the image into a nx3 where 3 is the no. of colour channels
renorm_image = np.reshape(original_image,(original_image.shape[0]*original_image.shape[1],3))
\#Before applying PCA you must normalize the data in each column separately as we will be applying PCA column-wise
mean = np.mean(renorm_image, axis=0) \#computing the mean
std = np.std(renorm_image, axis=0) \#computing the standard deviation
renorm_image = renorm_image.astype('float32') \#we change the datatpe so as to avoid any warnings or errors
renorm_image -= np.mean(renorm_image, axis=0)
renorm_image /= np.std(renorm_image, axis=0) \# next we normalize the data using the 2 columns
cov = np.cov(renorm_image, rowvar=False) \#finding the co-variance matrix for computing the eigen values
\#and eigen vectors.
lambdas, p = np.linalg.eig(cov) \# finding the eigen values lambdas and the vectors p
\#of the covarince matrix
alphas = np.random.normal(0, 0.1, 3) \# aplha here is the gaussian random no. generated
delta = np.dot(p, alphas*lambdas) \#delta here represents the value which will be
\#added to the re_norm image
pca_augmentation_version_renorm_image = renorm_image + delta \#forming augmented normalised image
pca_color_image = pca_augmentation_version_renorm_image * std + mean \#de-normalising the image
pca_color_image = np.maximum(np.minimum(pca_color_image, 255), 0).astype('uint8') \# necessary conditions which need to be checked
pca_color_image=np.ravel(pca_color_image).reshape((original_image.shape[0],original_image.shape[1],3)) \#rollong back the image into a displayable just as
\#original image
cv.imshow('image2',pca_color_image) \#Displaying the images
cv.imshow('image1',original_image)
cv.waitKey(0)
cv.destroyAllWindows()
![深度学习神经网络都是从CNN和AlexNet开始的](http://p3.pstatp.com/large/pgc-image/aa2364ba191043ff82793183d31d5ad7)
现在，完成所有这些后，您就可以完全处理AlexNet了。
**AlexNet：**
![深度学习神经网络都是从CNN和AlexNet开始的](http://p9.pstatp.com/large/pgc-image/e97b5b13e93f4c57b5d9a3e279747f56)
**数据预处理和生成：**
让我们从我们将在阅读本文时将要查看的数据集开始，即ILSRVC的数据，它是ImageNet的一个子集。
**ILSRVC（ImageNet大规模视觉识别挑战）**-是一年一度的竞赛。它使用分布在1000个类中的ImageNet中的120万个示例。这些数据用作AlexNet Paper的训练数据集。
**ImageNet**- ImageNet是超过22,000个类别中超过1500万张高分辨率标签图像。图像是从网上收集的，并使用亚马逊的Mechanical Turk众包工具进行人工标记。
ImageNet的数据由可变分辨率图像组成，因此我们需要将所有图像采样到固定大小。在这里，它被下采样到256x256的分辨率，这是通过首先将短边重新缩放到256然后从中裁剪中心（256,256）图像来完成的。
除了这种类型的数据预处理外，还包括数据增强以减少过拟合。我们做了两种类型的增强：
从（256,256）样本中随机提取（224,224）大小的图像，并进行水平反射。该网络在这个提取的维度上进行了训练（224,224）。这种机制将数据集的大小增加了2048倍（因为（224,224）的维度可以从256,256图像中抽取32 * 32（1024）个样本，水平反射使每个图像的样本数增加了一倍，每个图像可以生成2048个样本）。我们在测试时使用了类似的机制，即我们从原始的大小样本（256,256）中提取了5个相同大小的（224,224），并平均了10个网络的soft-max层的预测（由于水平反射增加了一倍）。
第二种类型的数据增强指的是PCA颜色变换，其方式与前面提到的完全相同。
这就是数据预处理和扩充的全部内容。
**架构：**
神经网络的架构是最重要的部分，AlexNet也不例外。神经网络的顺序稍有变化将严重影响模型的性能。
![深度学习神经网络都是从CNN和AlexNet开始的](http://p3.pstatp.com/large/pgc-image/79dcf3278e0d455792b8e7e562215cb7)
**多GPU训练：**
由于需要很大的计算能力，通过在两个GTX 580和3 gb内存上并行计算，非常明智地完成了Alex-Net计算。
我们使用的并行化方案实际上是将一半的内核(或神经元)放在每个GPU上，还有一个额外的技巧，即GPU的通信只在特定的层中进行。这意味着，例如，第3层的内核从第2层的所有内核映射中获取输入。但是，第4层中的内核只从驻留在相同GPU上的第3层中的内核映射中获取输入。
上述概念是减少训练时间。其原理图如图所示：
![深度学习神经网络都是从CNN和AlexNet开始的](http://p3.pstatp.com/large/pgc-image/e211e077fbb240988c66b42173d293c6)
Alex-Net原始架构
应用此GPU实现和上面的层描述，我们完成了神经网络的架构。
**学习：**
在数据预处理和架构完成之后，接下来就是学习过程。
使用随机梯度下降优化器训练模型，动量为0.9，权重衰减为0.0005。
权重的更新规则由下面的等式给出，
![深度学习神经网络都是从CNN和AlexNet开始的](http://p3.pstatp.com/large/pgc-image/1dd90b92a92243888c4b68bc9fce02a0)
来自Alex-Net
其中i是迭代索引，v是动量变量，ε是学习率。这里的导数项代表了第i批Di的平均值目标函数关于w的导数，在wi处的值。
每层的权重初始化为零均值高斯分布，方差为0.01。第2层、第4层和第5层卷积层的偏差初始化为1，因为这种初始化通过向ReLU提供正输入来加速学习的早期阶段，并且倾向于开始学习过程。其余各层的偏差初始化为零。
在所有层中使用相同的学习率，在整个训练期间正常调整。接下来的启发式是当验证错误率不再随着当前的学习率提高时，将学习率除以10。
学习从0.01开始，减少3次直到终止。
通过120万张图像运行了90个周期。
**结论**
如果您已了解到目前为止，那么您已成功完成了Alex-Net及其所有图层。

