# ImageNet Classification with Deep ConvolutionalNeural Networks - 李鑫o_O - CSDN博客





置顶2016年06月19日 15:11:33[hustlx](https://me.csdn.net/HUSTLX)阅读数：843
个人分类：[论文笔记](https://blog.csdn.net/HUSTLX/article/category/6277270)









ImageNet Classification with Deep ConvolutionalNeural Networks




**ReLU**(Rectified Linear Units (ReLUs))激活函数的优势：从用梯度下降法的训练时间角度来看，双曲正切或者sigmoid函数这类饱和非线性函数要比ReLU：f=max(0,x)这个非饱和的非线性函数要慢的多(好几倍的速度)。




**Local Response Normalization**：实验发现用下式归一化能降低错误率。

![](https://img-blog.csdn.net/20160619151012577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**Overlapping Pooling**：实验发现用有重叠的pooling能降低错误率，而且不易过拟合。



**网络结构**：5层卷积层，3层全连接层，最后一层全连接输入到1000个类的多类逻辑回归softmax分类器。具体来说，输入224*224*3的图像，第一个卷积层96个核，大小11*11*3（因为RGB三个图层，96个11*11*3的卷积核生成的特征图map为96个55*55图，注意11*11*3的卷积核具体操作的时候就是用11*11的卷积核去卷积每个输入图层然后再把得到的所有图对应位置相加得到这个卷积核的特征图），步长为4，然后跟局部归一化加max-pooling，第二个卷积层256核，大小5*5*48（因为第一层96张map分到2个GPU上，每个48张图，所以卷积大小5*5*48，得到256个大小27*27的图），加局部归一化和max-pooling，三四五层卷积均没有局部归一化和max-pooling。第三层卷积层384核，大小3*3*256（因为第三层卷积层连着两个GPU的第二层卷积，所以共256张图的输入，输出为384张13*13的特征图），第四卷积层384核3*3*192（因为第三层384张图分到两个GPU上，每个192张图，生成384个13*13的特征图），第五层卷积层256核3*3*192（256张特征图），全连接层4096个神经元。




**减少过拟合的方法**：a、增大数据集：一是从原始256*256的图像上随机切割224*224的patch。将数据集增大了2048倍。二是在RGB颜色分量加高斯扰动，使得物体识别对于强度和颜色亮度不敏感。这个方法减少了1%的错误率。b、Dropout：对于每个隐含层神经元，其输出以0.5的概率置0.被Dropout的神经元对网络的前向传播无贡献，不参加反向传播。这个技术减少了复杂的神经元之间依赖性，因为一个神经元不能依赖其他特定神经元的表现。这使得网络强迫学习一些更加鲁棒性的特征。在第二层全连接层用了dropout。




**训练**：对权重用0均值0.01方差高斯初始化，对于第二四五层、全连接层偏置初始化为1，这使得ReLU输入为正从而加速了学习过程。其他偏置初始化为0；



