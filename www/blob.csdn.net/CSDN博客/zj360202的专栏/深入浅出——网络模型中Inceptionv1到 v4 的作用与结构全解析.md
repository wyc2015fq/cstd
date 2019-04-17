# 深入浅出——网络模型中Inceptionv1到 v4 的作用与结构全解析 - zj360202的专栏 - CSDN博客





2017年11月24日 15:51:35[zj360202](https://me.csdn.net/zj360202)阅读数：717








# [深入浅出——网络模型中Inception的作用与结构全解析](http://blog.csdn.net/u010402786/article/details/52433324)




转载地址http://blog.csdn.net/u010402786





目录[(?)](http://blog.csdn.net/u010402786/article/details/52433324#)[[+]](http://blog.csdn.net/u010402786/article/details/52433324#)




## 一 论文下载




　　本文涉及到的网络模型的相关论文以及下载地址： 


[v1] Going Deeper with Convolutions, 6.67% test error 
[http://arxiv.org/abs/1409.4842](http://arxiv.org/abs/1409.4842)

[v2] Batch Normalization: Accelerating Deep Network Training by Reducing Internal Covariate Shift, 4.8% test error 
[http://arxiv.org/abs/1502.03167](http://arxiv.org/abs/1502.03167)

[v3] Rethinking the Inception Architecture for Computer Vision, 3.5% test error 
[http://arxiv.org/abs/1512.00567](http://arxiv.org/abs/1512.00567)

[v4] Inception-v4, Inception-ResNet and the Impact of Residual Connections on Learning, 3.08% test error 
[http://arxiv.org/abs/1602.07261](http://arxiv.org/abs/1602.07261)



## 二 Inception结构引出的缘由




　　先引入一张CNN结构演化图： 
![这里写图片描述](https://img-blog.csdn.net/20160904154907381)


　　2012年AlexNet做出历史突破以来，直到GoogLeNet出来之前，主流的网络结构突破大致是网络更深（层数），网络更宽（神经元数）。所以大家调侃深度学习为“深度调参”，但是纯粹的增大网络的缺点：
<code class="hljs cs has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: "Source Code Pro", monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//1.参数太多，容易过拟合，若训练数据集有限；</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//2.网络越大计算复杂度越大，难以应用；</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//3.网络越深，梯度越往后穿越容易消失（梯度弥散），难以优化模型</span></code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right: 1px solid rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li></ul>

　　那么解决上述问题的方法当然就是增加网络深度和宽度的同时减少参数，Inception就是在这样的情况下应运而生。



## 三 Inception v1模型




　　Inception v1的网络，将1x1，3x3，5x5的conv和3x3的pooling，堆叠在一起，一方面增加了网络的width，另一方面增加了网络对尺度的适应性； 
![这里写图片描述](https://img-blog.csdn.net/20160904155917864)

![这里写图片描述](https://img-blog.csdn.net/20160904160721902)


　　第一张图是论文中提出的最原始的版本，所有的卷积核都在上一层的所有输出上来做，那5×5的卷积核所需的计算量就太大了，造成了特征图厚度很大。为了避免这一现象提出的inception具有如下结构，在3x3前，5x5前，max pooling后分别加上了1x1的卷积核起到了降低特征图厚度的作用，也就是Inception v1的网络结构。 


　　下面给出GoogLeNet的结构图： 
![这里写图片描述](https://img-blog.csdn.net/20160904161917654)



## 三 Inception v2模型




　　一方面了加入了BN层，减少了Internal Covariate Shift（内部neuron的数据分布发生变化），使每一层的输出都规范化到一个N(0, 1)的高斯； 

　　另外一方面学习VGG用2个3x3的conv替代inception模块中的5x5，既降低了参数数量，也加速计算； 
![这里写图片描述](https://img-blog.csdn.net/20160904162948225)

　　使用3×3的已经很小了，那么更小的2×2呢？2×2虽然能使得参数进一步降低，但是不如另一种方式更加有效，那就是Asymmetric方式，即使用1×3和3×1两种来代替3×3的卷积核。这种结构在前几层效果不太好，但对特征图大小为12~20的中间层效果明显。 
![这里写图片描述](https://img-blog.csdn.net/20160904165625720)




## 四 Inception v3模型




　　v3一个最重要的改进是分解（Factorization），将7x7分解成两个一维的卷积（1x7,7x1），3x3也是一样（1x3,3x1），这样的好处，既可以加速计算（多余的计算能力可以用来加深网络），又可以将1个conv拆成2个conv，使得网络深度进一步增加，增加了网络的非线性，还有值得注意的地方是网络输入从224x224变为了299x299，更加精细设计了35x35/17x17/8x8的模块。



## 五 Inception v4模型




　　v4研究了Inception模块结合Residual Connection能不能有改进？发现ResNet的结构可以极大地加速训练，同时性能也有提升，得到一个Inception-ResNet v2网络，同时还设计了一个更深更优化的Inception v4模型，能达到与Inception-ResNet v2相媲美的性能。 

　　待补充..



## 六 参考文献




1.[http://blog.csdn.net/stdcoutzyx/article/details/51052847](http://blog.csdn.net/stdcoutzyx/article/details/51052847)


2.[http://blog.csdn.net/cv_family_z/article/details/50789805](http://blog.csdn.net/cv_family_z/article/details/50789805)


3.[http://blog.csdn.net/sunbaigui/article/details/50807418](http://blog.csdn.net/sunbaigui/article/details/50807418)


4.[http://blog.csdn.net/bea_tree/article/details/51784026](http://blog.csdn.net/bea_tree/article/details/51784026)






