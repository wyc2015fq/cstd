# Depth-wise separable convolution 深度可分卷积 - 别说话写代码的博客 - CSDN博客





2019年03月06日 16:28:48[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：192标签：[Depth-wise separable convoluti																[深度可分卷积																[deepwise convolution																[pointwise convolution](https://so.csdn.net/so/search/s.do?q=pointwise convolution&t=blog)
个人分类：[Deep learning](https://blog.csdn.net/qq_21997625/article/category/7283460)





Depth-wise separable convolution :深度分离卷积，出自mobilenet和xception。

MobileNet是基于深度可分离卷积的，通俗来说，**深度可分离卷积就是：把标准卷积分解成深度卷积(depthwise convolution)和逐点卷积(pointwise convolution)。这么做的好处就是可以再损失精度不多的情况下大幅度降低参数量和计算量。**分解过程如下：

![](https://img-blog.csdnimg.cn/20190306161821719.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190306161821707.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

**深度分离卷积示例**：（话说这个实例非常好，出自哪里我找不到了，若作者看到联系我）

输入图片大小(6,6,3),原卷积操作用(4,4,3,5)的卷积(卷积核大小为4x4,卷积核通道为3，卷积核数量为5），stride=1,padding=0。输出的特征尺寸为(6-4)/1+1=3,即输出的特征映射为(3,3,5)

将标准卷积中选取序号为n的卷积核，大小为(4,4，3),标准卷积过程示意图如下（省略bias）：

![](https://img-blog.csdnimg.cn/20190306161821741.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

黑色的输入为(6,6,3) 与第n个卷积核对应，每个通道对应每个卷积核通道卷积得到输出，最终输出为2+0+1=3(这是常见的卷积操作，注意这里卷积核要和输入的通道数相同，即图中表示的3个通道~)

对于深度分离卷积，把标准卷积(4,4,3,5)分解为：
- 
深度卷积部分：大小为(4,4,1,3) ，作用在输入的每个通道上，输出特征映射为(3,3,3)

- 
逐点（分离）卷积部分：大小为(1,1,3,5)，作用在深度卷积的输出特征映射上，得到最终输出为(3,3,5)。**为啥要用分离卷积呢，因为输出的每个feature map都要包含所有输入层的feature map信息**。


例中深度卷积卷积过程示意图如下：



![](https://img-blog.csdnimg.cn/20190306161821740.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

输入有3个通道，对应着有3个大小为(4,4,1) 的深度卷积核，卷积结果共有3个大小为(3,3,1) ，我们按顺序将这卷积按通道排列得到输出卷积结果(3,3,3) 。

![](https://img-blog.csdnimg.cn/20190306161821673.png)](https://so.csdn.net/so/search/s.do?q=deepwise convolution&t=blog)](https://so.csdn.net/so/search/s.do?q=深度可分卷积&t=blog)](https://so.csdn.net/so/search/s.do?q=Depth-wise separable convoluti&t=blog)




