# 【论文笔记】Efficient Inference in Fully Connected CRFs with Gaussian Edge Potentials - xiaxzhou的博客 - CSDN博客





2017年07月01日 19:32:53[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：3045








### Efficient Inference in Fully Connected CRFs with Gaussian Edge Potentials

#### 摘要：

most state-of-the-art techniques for multi-class image segmentation and labeling use conditional random fields defined over pixels or image regions. while regionlevel models often feature dense pairwise connectivity, pixel-level models are considerably larger and have only permitted sparse graph structures. in this paper, we consider fully connected crf models defined on the complete set of pixels in an image. the resulting graphs have billions of edges, making traditional inference algorithms impractical. our main contribution is a highly efficient approximate inference algorithm for fully connected crf models in which the pairwise edge potentials are defined by a linear combination of gaussian kernels. our experiments demonstrate that dense connectivity at the pixel level substantia substantially improves segmentation and labeling accuracy.

多数图像分割和标记技术大多采用基于像素或图像区域的条件随机场。虽然区域级模型通常具有密集的成对连通性，而像素级的模型是相当大的，只允许稀疏图结构。在本文中，我们考虑在一个图像定义完整的像素全连通的CRF模型。由此产生的图形具有数十亿的边缘，使得传统的推理算法变得不切实际。我们的主要贡献是在完全连接的CRF模型中使用一个高效的近似推理算法，成对的边缘势是由高斯核的线性组合定义的。实验表明，在像素级的致密连接提高了分割和标签的准确性。

CRF最大限度地提高相似像素之间的标签，并可以集成更精细的术语，模型对象类之间的上下文关系

### **简介**：

**在语义分割中，CRF推理的关键思想是将标签的分配问题作为一个概率分布问题。**

We use a fully connected CRF that establishes pairwise potentials on all pairs of pixels in the image. 

我们使用一个完全连接的CRF，在图像中所有的像素上建立成对的势函数。

The main challenge is the size of the model, which has tens of thousands of nodes and billions of edges even on low-resolution images. 

主要的问题是模型的大小，即使在低分辨率的图像上，也有成千上万的节点和数十亿的边缘。

![这里写图片描述](https://img-blog.csdn.net/20170701145931659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Our main contribution is a highly efficient inference algorithm for fully connected CRF models in which the pairwise edge potentials are defined by a linear combination of Gaussian kernels in an arbitrary feature space 

在这个模型中，在任意特征空间的高斯核函数的线性组合中定义了成对像素的势函数

The algorithm is based on a mean field approximation to the CRF distribution .This approximation is iteratively optimized through a series of message passing steps, each of which updates a single variable by aggregating information from all other variables. 

We show that a mean field update of all variables in a fully connected CRF can be performed using Gaussian filtering in feature space. This allows us to reduce the computational complexity of message passing from quadratic to linear in the number of variables by employing efficient approximate high-dimensional filtering [16, 2, 1]. The resulting approximate inference algorithm is sublinear in the number of edges in the model 

本文算法基于CRF的平均近似分布。 

通过一系列消息传递步骤迭代优化该近似，每一步通过汇总来自所有其他变量的信息来更新单个变量。 

模型中的消息传递可以用特征空间中的高斯滤波进行 
**一个完全连接的CRF的所有变量的平均场更新可以在特征空间使用高斯滤波完成。这使得我们可以通过采用高效的近似高维滤波来减少消息传递，从而将消息传递的二次复杂复杂度降低为线性复杂度**
进一步参考：[http://www.jianshu.com/p/434b3e22a47e](http://www.jianshu.com/p/434b3e22a47e)




