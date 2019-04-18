# 图像分割网络FCN原理详解 - wsp_1138886114的博客 - CSDN博客





2019年01月01日 18:40:01[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：398











### 文章目录
- [一、全卷积网络(FCN)](#FCN_1)
- [1.1 CNN网络](#11_CNN_5)
- [1.2 全卷积网络(FCN)](#12_FCN_10)
- [二、网络实现](#_14)




### 一、全卷积网络(FCN)

卷积神经网络从**图像分类**到到**对象检测**、**实例分割**、到图像**语义分割**、是卷积特征提取从粗糙输出到精炼输出的不断升级，基于卷积神经网络的**全卷积分割网络FCN**是像素级别的图像语义分割网络，相比以前传统的图像分割方法，基于卷积神经网络的分割更加的精准，适应性更强。

为了了解全卷积网络，我们必须他与普通CNN网络的区别：

##### 1.1 CNN网络

通常CNN网络在卷积层之后会接上若干个全连接层, 将卷积层产生的特征图(feature map)映射成一个固定长度的特征向量。以AlexNet为代表的经典CNN结构适合于图像级的分类和回归任务，因为它们最后都期望得到整个输入图像的一个数值描述（概率），比如AlexNet的ImageNet模型输出一个1000维的向量表示输入图像属于每一类的概率(softmax归一化)。
- 举例：下图中的猫, 输入AlexNet, 得到一个长为1000的输出向量, 表示输入图像属于每一类的概率, 其中在“tabby cat”这一类统计概率最高（如下图）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019010111302042.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
##### 1.2 全卷积网络(FCN)

FCN对图像进行像素级的分类，从而解决了语义级别的图像分割（semantic segmentation）问题。与经典的CNN在卷积层之后使用全连接层得到固定长度的特征向量进行分类（**全联接层＋softmax输出**）不同，FCN可以接受任意尺寸的输入图像，采用反卷积层**对最后一个卷积层的feature map进行上采样**, 使它恢复到输入图像相同的尺寸，从而可以对每个像素都产生了一个预测, 同时保留了原始输入图像中的空间信息, 最后在上采样的特征图上进行逐像素分类（如上图）。

简而言之：FCN与CNN的区域在把于CNN最后的全连接层换成卷积层，输出的是一张已经Label好的图片。

### 二、网络实现

FCN的卷积网络部分可以采用VGG、GoogleNet、AlexNet等作为前置基础网络，在这些的预训练基础上进行迁移学习与finetuning，对反卷积的结果跟对应的正向feature map进行叠加输出(这样做的目的是得到更加准确的像素级别分割)，根据上采样的倍数不一样分为FCN-8S、FCN-16S、FCN-32S，图示如下：
- 
详情：

对原图像进行卷积 conv1、pool1后原图像缩小为1/2；

之后对图像进行第二次 conv2、pool2后图像缩小为1/4；

继续对图像进行第三次卷积操作conv3、pool3缩小为原图像的1/8，此时保留pool3的featureMap；

继续对图像进行第四次卷积操作conv4、pool4，缩小为原图像的1/16，保留pool4的featureMap；

最后对图像进行第五次卷积操作conv5、pool5，缩小为原图像的1/32，

然后把原来CNN操作中的全连接变成卷积操作conv6、conv7，图像的featureMap数量改变但是图像大小依然为原图的1/32，此时图像不再叫featureMap而是叫heatMap。- 
实例

现在我们有1/32尺寸的heatMap，1/16尺寸的featureMap和1/8尺寸的featureMap，1/32尺寸的heatMap进行upsampling操作之后，因为这样的操作还原的图片仅仅是conv5中的卷积核中的特征，限于精度问题不能够很好地还原图像当中的特征，因此在这里向前迭代。把conv4中的卷积核对上一次upsampling之后的图进行反卷积补充细节（相当于一个差值过程），最后把conv3中的卷积核对刚才upsampling之后的图像进行再次反卷积补充细节，最后就完成了整个图像的还原。(关于上采样不懂的自行百度)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190101182852611.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190101183453196.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

github:[https://github.com/muyang0320/tf-fcn](https://github.com/muyang0320/tf-fcn)

在这里我们要注意的是FCN的缺点：- 
是得到的结果还是不够精细。进行8倍上采样虽然比32倍的效果好了很多，但是上采样的结果还是比较模糊和平滑，对图像中的细节不敏感。

- 
是对各个像素进行分类，没有充分考虑像素与像素之间的关系。忽略了在通常的基于像素分类的分割方法中使用的空间规整（spatial regularization）步骤，缺乏空间一致性。









