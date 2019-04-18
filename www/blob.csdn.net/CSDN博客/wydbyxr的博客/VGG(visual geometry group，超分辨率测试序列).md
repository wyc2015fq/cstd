# VGG(visual geometry group，超分辨率测试序列) - wydbyxr的博客 - CSDN博客
2018年11月11日 10:11:59[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：191
# VGG(visual geometry group，超分辨率测试序列)
  这里有两个表格，其中第一个表格是描述的是VGGNet的诞生过程。为了解决初始化（权重初始化）等问题，VGG采用的是一种Pre-training的方式，这种方式在经典的神经网络中经常见得到，就是先训练一部分小网络，然后再确保这部分网络稳定之后，再在这基础上逐渐加深。表1从左到右体现的就是这个过程，并且当网络处于D阶段的时候，效果是最优的，因此D阶段的网络也就是最后的VGGNet啦！
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181127095508393.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## test过程
  采用multi-scale输入尺寸的形式（输入尺寸介于[256:512]），具体执行流程如下：
  1）采用不同规格的图片作为输入（4种或6种规格）；
  2）最后一个maxpool层之前按照原有网络执行；
  3）修改最后一个maxpool层为shift-max-pool，对网络中的最后一个卷积层的每一个feature map按照(0,1,2)的变化量分别在(x,y)方位上移位操作，之后进行max-pool操作，这样每个卷积层的feature map得到了9个shift-max-pool后的feature map。
## offset max-pooling
  移位过程如下图所示，假设最后一个卷积层的一个feature map有一个1维的20 pixel的数据，按照size = 3，stride=3进行max_pooling，小于stride的部分不进行pooling。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111101151935.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  会产生多种（我们举个例子是9种）池化结果，分别送入后面的网络层，最后我们的图片分类输出结果就可以得到9个预测结果(每个类别都可以得到9种概率值，然后我们对每个类别的9种概率，取其最大值，做为此类别的预测概率值)。
