# CNN中感受野的计算 - 数据之美的博客 - CSDN博客
2017年08月28日 16:45:54[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：231
感受野（receptive field）是怎样一个东西呢，从CNN可视化的角度来讲，就是输出featuremap某个节点的响应对应的输入图像的区域就是感受野。
比如我们第一层是一个3*3的卷积核，那么我们经过这个卷积核得到的featuremap中的每个节点都源自这个3*3的卷积核与原图像中3*3的区域做卷积，那么我们就称这个featuremap的节点感受野大小为3*3
如果再经过pooling层，假定卷积层的stride是1，pooling层大小2*2，stride是2，那么pooling层节点的感受野就是4*4
有几点需要注意的是，padding并不影响感受野，stride只影响下一层featuremap的感受野，size影响的是该层的感受野。
至于如何计算感受野，我的建议是top to down的方式。下面我拿一个例子来算算
![](https://img-blog.csdn.net/20150710132345536?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
pool3的一个输出对应pool3的输入大小为2*2
依次类推，对应conv4的输入为4*4，因为2*2的每个角加一个3*3的卷积核，就成了4*4，当然这是在stride=1的情况下才成立的，但是一般都是stride=1，不然也不合理
对应conv3的输入为6*6
对应pool2的输入为12*12
对应conv2的输入为14*14
对应pool1的输入为28*28
对应conv1的输入为30*30
所以pool3的感受野大小就是30*30
