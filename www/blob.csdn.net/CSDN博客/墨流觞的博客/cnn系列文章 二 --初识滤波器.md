# cnn系列文章 二  --初识滤波器 - 墨流觞的博客 - CSDN博客





2018年09月12日 11:39:06[墨氲](https://me.csdn.net/dss_dssssd)阅读数：279








[cnn系列文章一  --特征选择](https://blog.csdn.net/dss_dssssd/article/details/82658430)

惯例，分享一句歌词。

> 
《明月天涯》

唯此间江湖年少 偏爱纵横天下

恩仇趁年华轻剑快马
这一部分主要按照Andrew Ng的DeepLearning课程编写

## 卷积概念的引入以及理解

![](./cn.png)![这里写图片描述](https://img-blog.csdn.net/20180912110949121?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这是一个典型的多层卷积神经网络各层的输出，比如第一层从图像中检测到边缘信息，第二层检测到不完整的人脸信息，而第三层则检测到了完整的人脸信息。
接下来将介绍如何从图像中检测轮廓线等边缘信息，当然是使用卷积核了。

比如从下面这个图像中检测出垂直边缘信息和水平边缘信息
![](./3.png)![这里写图片描述](https://img-blog.csdn.net/20180912111003492?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
接下来看一个例子,

下图为$6 \times 6$的灰度图，构造$3 \times 3$的过滤器(filter),有得论文中也成为卷积核(kernal),**本系列文章之后都称作过滤器**，最终经过一系列的操作后会得到一个$4 \times 4$的输出图像，
![](./4.png)![这里写图片描述](https://img-blog.csdn.net/20180912111015641?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

接下来看一下计算过程，以左上角为例:
$$ 3 \times 1 + 1 \times 1+2 \times 1 + 0 \times 0 + 5 \times 0 + 7 \times 0 + 1 \times -1 + 8 \times -1 +2 \times -1  = -5$$

接下来为计算下一个值，将过滤器从原图像往左移动一格，
![](./5.png)![这里写图片描述](https://img-blog.csdn.net/20180912111023745?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

同样计算得到-4
最后得到的$4 \times 4$的输出图像为：

![](./6.png)![这里写图片描述](https://img-blog.csdn.net/20180912111038680?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
接下来看一个动态计算的例子就会很简单了

![](./8.gif)![这里写图片描述](https://img-blog.csdn.net/20180912111055638?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在tensorflow框架下，有tf.nn.conv2d函数实现卷积操作，在课程最后会讲解函数的使用以及如何使用tensorflow构建自己的卷积神经网络结构

接下来看一个更明显的卷及操作的例子：
![](./9.png)![这里写图片描述](https://img-blog.csdn.net/20180912111122156?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最左边是$6 \times 6$的灰度图像，可以看出从左到右有一个图像从亮到暗的过渡，也就是说存在一个垂直边缘，使用$3 \times 3$的过滤器去做卷积操作，最终在输出的$4 \times 4$的图像中得到了中间的垂直边缘。
当然还有很多其他的过滤器种类，比如sobel算子，高斯核，laplace算子等，opencv库也提供很多版本的算子，

如下图，在这我们不在使用自己手动设计的过滤器，而是把过滤器当做神经网络的参数，通过深度学习网络来学习正确的参数，构造适合于当前任务的过滤器，从而实现图像特征值的提取。事实证明，这种通过学习构造过滤器的效果要比自己手动设计的过滤器好非常多的。

![](./10.png)![这里写图片描述](https://img-blog.csdn.net/20180912111134867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
以上就是卷积的基本概念，接下来我们将要讲解卷积更细节的知识点。[padding，stride，filter的超参数设定对输出图像维度的影响等。](https://blog.csdn.net/dss_dssssd/article/details/82659316)












