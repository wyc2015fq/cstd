# 场景分割：MIT Scene Parsing 与DilatedNet 扩展卷积网络 - wishchinYang的专栏 - CSDN博客
2018年03月16日 15:38:29[wishchin](https://me.csdn.net/wishchin)阅读数：873

**MIT Scene Parsing Benchmark简介**
        Scene parsing is to segment and parse an image into different image regions associated with semantic categories, such as sky, road, person, and bed. MIT Scene Parsing Benchmark (SceneParse150) provides a standard training
 and evaluation platform for the algorithms of scene parsing. The data for this benchmark comes from[ADE20K Dataset](http://groups.csail.mit.edu/vision/datasets/ADE20K/) which contains more than 20K scene-centric images exhaustivelyannotated
 with objects and object parts. Specifically, the benchmark is divided into 20K images for training, 2K images for validation, and another batch of held-out images for testing. There are totally 150 semantic categories included for evaluation, which include
 stuffs like sky, road, grass, and discrete objects like person, car, bed. Note that there are non-uniform distribution of objects occuring in the images, mimicking a more natural object occurrence in daily scene.
        scene Benchmark包含了150个物体类别，包括一般无定型的墙壁、水域、地板、道路，也包括常见的室内目标如窗户、桌子、椅子、床、杯子等粘附和非粘附目标，包含了COCO数据集的大多数类别。
        主页链接：[http://sceneparsing.csail.mit.edu](http://sceneparsing.csail.mit.edu)/                 
        预训练模型： [http://sceneparsing.csail.mit.edu/model](http://sceneparsing.csail.mit.edu/model)/         
        Model ZOO ： [https://github.com/CSAILVision/sceneparsing/wiki/Model-Zo](https://github.com/CSAILVision/sceneparsing/wiki/Model-Zo)o           
        一些State 的结果：[https://drive.google.com/drive/folders/0B9CKOTmy0DyaQ2oxUHdtYUd2Mm8?usp=](https://drive.google.com/drive/folders/0B9CKOTmy0DyaQ2oxUHdtYUd2Mm8?usp=)sharing
        挑战结果： [http://placeschallenge.csail.mit.edu/results_challenge.html](http://placeschallenge.csail.mit.edu/results_challenge.html)  Face++ 暂时排在第一名
![](https://img-blog.csdn.net/20180316155015731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**1. FCN与去卷积网络**
  deconv的其中一个用途是做upsampling，即增大图像尺寸。而dilated conv并不是做upsampling，而是增大感受野。
  参考：如何[理解深度学习中的去卷积网络](https://www.zhihu.com/question/43609045/answer/132235276)层
         (1) s>1，即卷积的同时做了downsampling，卷积后图像尺寸减小；
         (2) s=1，普通的步长为1的卷积，比如在tensorflow中设置padding=SAME的话，卷积的图像输入和输出有相同的尺寸大小；
         (3) 0<s<1，fractionally strided convolution，相当于对图像做upsampling。比如s=0.5时，意味着在图像每个像素之间padding一个空白的像素后，stride改为1做卷积，得到的feature map尺寸增大一倍。
         而dilated conv不是在像素之间padding空白的像素，而是在已有的像素上，skip掉一些像素，或者输入不变，对conv的kernel参数中插一些0的weight，达到一次卷积看到的空间范围变大的目的。
**2. 所谓孔洞卷积**
dilated conv，中文可以叫做空洞卷积或者扩张卷积。
        参考：如何理解[扩展卷积网络](https://www.zhihu.com/question/54149221)？下一段摘抄于此文
        参考：[Multi-scale context aggregation by dilated convolution](http://blog.csdn.net/guvcolie/article/details/77884530?locationNum=10&fps=1)s
        诞生背景，在图像分割领域，图像输入到CNN（典型的网络比如FCN[3]）中，FCN先像传统的CNN那样对图像做卷积再pooling，降低图像尺寸的同时增大感受野，但是由于图像分割预测是pixel-wise的输出，所以要将pooling后较小的图像尺寸upsampling到原始的图像尺寸进行预测（upsampling一般采用deconv反卷积操作，deconv可参见知乎答案[如何理解深度学习中的deconvolution
 networks？](https://www.zhihu.com/question/43609045/answer/132235276)），之前的pooling操作使得每个pixel预测都能看到较大感受野信息。因此图像分割FCN中有两个关键，一个是pooling减小图像尺寸增大感受野，另一个是upsampling扩大图像尺寸。在先减小再增大尺寸的过程中，肯定有一些信息损失掉了，那么能不能设计一种新的操作，不通过pooling也能有较大的感受野看到更多的信息呢？答案就是dilated conv。
下面看一下dilated conv原始论文[4]中的示意图：
![](https://img-blog.csdn.net/20180316152627715?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(a)图对应3x3的1-dilated conv，和普通的卷积操作一样，(b)图对应3x3的2-dilated conv，实际的卷积kernel size还是3x3，但是空洞为1，也就是对于一个7x7的图像patch，只有9个红色的点和3x3的kernel发生卷积操作，其余的点略过。也可以理解为kernel的size为7x7，但是只有图中的9个点的权重不为0，其余都为0。 可以看到虽然kernel size只有3x3，但是这个卷积的感受野已经增大到了7x7（如果考虑到这个2-dilated conv的前一层是一个1-dilated
 conv的话，那么每个红点就是1-dilated的卷积输出，所以感受野为3x3，所以1-dilated和2-dilated合起来就能达到7x7的conv）,(c)图是4-dilated conv操作，同理跟在两个1-dilated和2-dilated conv的后面，能达到15x15的感受野。对比传统的conv操作，3层3x3的卷积加起来，stride为1的话，只能达到(kernel-1)*layer+1=7的感受野，也就是和层数layer成线性关系，而dilated conv的感受野是指数级的增长。
       dilated的好处是不做pooling损失信息的情况下，加大了感受野，让每个卷积输出都包含较大范围的信息。在图像需要全局信息或者语音文本需要较长的sequence信息依赖的问题中，都能很好的应用dilated conv，比如图像分割[3]、语音合成WaveNet[2]、机器翻译ByteNet[1]中。
       可以把网络看做一个pooling层插值网络。
      参考：Yu, Fisher, and Vladlen Koltun. "Multi-scale context aggregation by dilated convolutions."*arXiv preprint arXiv:1511.07122* (2015).
使用预训练模型得到的一些结果：
![](https://img-blog.csdn.net/20180316153706942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
预处理模型效果不是很好，应该使用竞赛排名top的几个模型
