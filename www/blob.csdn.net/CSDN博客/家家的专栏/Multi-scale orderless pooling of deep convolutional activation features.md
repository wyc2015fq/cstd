# Multi-scale orderless pooling of deep convolutional activation features - 家家的专栏 - CSDN博客





2014年05月15日 21:58:01[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2648








**Multi-scale orderless pooling of deep convolutional activation features  （MOP-CNN）**

Deep convolutional neural networks (**Deep CNN**) (或者理解为全局的CNN activations) 缺少空间位置信息。

**空间pooling of local descriptors.**



**combining **activations extracted at **multiple local **image windows. 


三步走：

1、Extracting CNN activations for local pathces at multiple scales   （多尺度上局部patches抽取CNN activation）

2、+ Orderless VLAD pooling of these activations at each scale level  （每一尺度层上进行无序pooling (VLDA encoding)）

3、+combine the above results. （串联以上的每一层的activation）

实验是用在classification上。




**Bag of features (BOF)** : （orderless） 把图像看做是一些局部features的无序集合，而不会考虑任何全部空间信息。为了改进BOF缺失空间信息的问题，后引进 （Spatial Pyramid matching）SPM策略。  

**Deep CNN:**  （globally ordered）采用的全部不同的结构。原始图像像素输入到五个卷积层，每一层滤波出feature maps，然后在局部邻域进行max-pooling。 

                      filtering layers+max-pooling layers + fully connected layers=====>最后产生一个4096维的feature。

**实验设置**： 利用imagenet上预先trained的Caffe CPU implementation， 来抽取深度features。 

                      图像或者patch，resample到256*256， 减均值，作为network的输入。 

                      七层（全连接）处理后，输出4096维的特征。因为采用了rectified linear unit (ReLU)变换，所有的特征值都是非负的。

                       Linear SVM from the INRIA JSGD package.

                       规则化参数设为10^-5,  learning rate=0.2， train for 100 epochs. 




**Multi-scale orderless pooling of CNN features**

           利用the** vector of locally aggregated descriptors** (**VLAD**) framework, which is a simplified version of
**Fisher Vectors (FV**).  

           具体来说，P_j表示第j个image patch的feature （维度=4096）。 然后降维到500维。 

            对第二层或者第三层（第一层就是一个全局feature），分别学习k-means codebook (k=100) centers（c_i, i=1, 2, ...k）. 

![](https://img-blog.csdn.net/20140515215400609)

              然后对于pooled vector 进行规则化（power 或者L2 norm）。 这样对于每一层，或产生pooled vector 500*100=50,000维的feature，然后进行PCA降维

              产生4096维的feature。 最后对三层的4096维的feature进行combine为：4096*3维度的feature。

















