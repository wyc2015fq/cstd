# TSN(Temporal Segment Networks)算法笔记 - AI之路 - CSDN博客





2018年01月10日 23:04:18[AI之路](https://me.csdn.net/u014380165)阅读数：8441








论文：Temporal Segment Networks: Towards Good Practices for Deep Action Recognition 

论文链接：[https://arxiv.org/abs/1608.00859](https://arxiv.org/abs/1608.00859)

代码链接一：[https://github.com/yjxiong/temporal-segment-networks](https://github.com/yjxiong/temporal-segment-networks)

代码链接二：[https://github.com/yjxiong/tsn-pytorch](https://github.com/yjxiong/tsn-pytorch)
**这篇ECCV2016的文章主要提出TSN（temporal segment network）结构用来做video的action recognition。TSN可以看做是two stream系列的改进（关于two stream可以参看论文链接：[https://arxiv.org/abs/1406.2199](https://arxiv.org/abs/1406.2199)或者[博客链接](http://blog.csdn.net/u014380165/article/details/78756459)），在此基础上，文章要解决两个问题：1)how to design an effective and efficient video-level framework for learning video representation that is able to capture long-range temporal structure; 2) how to learn the ConvNet models given limited training samples.** 也就是说一个是解决长时间视频的行为判断问题（有些视频的动作时间较长）；一个是解决数据少的问题，数据量少会使得一些深层的网络难以应用到视频数据中，因为过拟合会比较严重。**算法效果**：Our approach obtains the state-the-of-art performance on the datasets of HMDB51 (69:4%) and UCF101 (94:2%). 另外提一下：Unlike image classication, end-to-end deep ConvNets remain unable to achieve signicant advantage over traditional hand-crafted features for video-based action recognition. 因此目前video方面的提升空间还是比较大的。

**针对第一个问题，首先，为什么目前的two stream结构的网络难以学习到long-range temporal structure？因为其针对的主要是single frame或a single stack of frames in a short snippet的数据，但这对于时间跨度较长的视频动作检测而言是不够的。**因此采用dense temporal sampling的方式来获取long-range temporal structure是比较常用的方法，但是作者发现视频的连续帧之间存在冗余，因此用sparse temporal sampling代替dense temporal sampling，也就是说在对视频做抽帧的时候采取较为稀疏的抽帧方式，这样可以去除一些冗余信息，同时降低了计算量。

**针对第二个问题**，可通过常规的data argument（random crop，horizontal flip，corner crop，scale jitter）；regularization；还有作者提到的cross-modality pre-training，dropout等方式来减少过拟合。这些后面会详细解释。

**Figure2是作者用到的输入数据形式**。我们知道对于图像而言，特征可能就只有图像本身，但是视频的特征就比较丰富，除了帧以外，还有optical flow等，如Figure2所示。**注意，这也是作者解决数据量少的一种方法，那就是从有限的数据中提取更多有用的特征**。从最左边开始说起，RGB image就是video中的某一帧；RGB difference是相邻两帧的差，可以用来表达动作信息；optical flow field和warped optical flow field是视频的光流信息。RGB image和optical flow正是two stream论文中的spatial stream convnet和temporal stream convnet两条支路的输入。因此这里作者相当于想额外研究RGB difference和warped optical flow对动作检测效果的影响。 
![这里写图片描述](https://img-blog.csdn.net/20180110225309319?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**Figure1就是作者提出的TSN网络**。网络部分是由双路CNN组成的，分别是**spatial stream ConvNets和temporal stream ConvNets**，这和two stream文章中介绍的结构类似，在文中这两个网络用的都是BN-Inception（two stream论文中采用的是较浅的网络：ClarifaiNet）。**那么这两路CNN以什么为输入呢？spatial stream ConvNets以single RGB images作为输入，temporal stream ConvNets以a stack of consecutive optical flow fields作为输入，换句话说两个网络的输入类型是不一样的。**
**当然最重要的是temporal segment network（包含多个spatial stream convnet和temporal convnet，参看Figure1）的输入不是single frame or frame stack，而是a sequence of short snippets sparsely sampled from the entire video，这就是为了获取long-range temporal structure所做的改进。**

Figure1的最左边是一个Video，用V表示，将V分成K份（文中K采用3），用(S1,S2,…,Sk)表示。这样TSN网络就可以用下面这个式子表示： 
![这里写图片描述](https://img-blog.csdn.net/20180110225432276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**这里的(T1,T2,….,Tk)表示a sequence of snippets，也就是说Tk是从(S1,S2,…,Sk)中对应的视频片段Sk中随机采样出来的结果，Tk是一个snippet，每个snippet包含一帧图像和两个optical flow特征图。这也就完成了作者说的sparse temporal sampling。**上面式子中的W就是网络的参数，因此F(Tk;W)就是网络的输出，也就是该snippet属于每个类的得分。**F函数的输出结果就是Figure1中spatial convnet或temporal convnet的输出结果，可以看Figure1中convnet图后面的绿色条形图，代表的就是socre在类别上的分布。g是一个aggregation function，在文中采用的是均值函数**，就是对所有snippet的属于同一类别的得分做个均值，毕竟我们最后要求的是这个video属于哪个类别，而不是这个video的某个snippet属于哪个类别。**g函数的输出结果就是Figure1中segmental consesus的输出结果。最后用H函数（文中用的softmax函数）根据得分算概率，概率最高的类别就是该video所属的类别。注意，在输入softmax之前就会将两条网络的结果进行合并。另外需要强调的是：All snippet-level ConvNets share the model parameters in temporal segment networks. 也就是Figure1中的K个spatial convnet的参数是共享的，K个temporal convnet的参数也是共享的，实际用代码实现时只是不同的输入过同一个网络。**
![这里写图片描述](https://img-blog.csdn.net/20180110225631877?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**网络的损失函数**如下（standard categorical cross-entropy），C表示类别数，yi是ground truth。 
![这里写图片描述](https://img-blog.csdn.net/20180110225710318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中G表示： 
![这里写图片描述](https://img-blog.csdn.net/20180110225720333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**也就是说G是一个长度为C的向量，表示一个video属于每个类别的得分。**
**注意：In the learning process, the loss values of video-level predictions, other than those of snippet-level predictions which were used in two-stream ConvNets, are optimized by iteratively updating the model parameters. 因此 Optimized in this manner, temporal segment network can learn model parameters from the entire video rather than a short snippet. 这是和two-stream的一个区别。**

**前面提到过为了防止过拟合，作者采取了多种措施，其中一种就是cross-modality pre-training，那么cross-modality pre-training是怎么做呢？**预训练我们都很熟悉，以RGB image为输入的spatial ConvNets网络可以用在imagenet数据集上预训练的网络来初始化。而以optical flow fields作为输入的temporal stream ConvNets网络该怎么初始化呢？答案就是cross-modality pre-training，原文如下：**We come up with a cross modality pre-training technique in which we utilize RGB models to initialize the temporal networks. First, we discretize optical flow fields into the interval from 0 to 255 by a linear transformation. This step makes the range of optical flow fields to be the same with RGB images. Then, we modify the weights of first convolution layer of RGB models to handle the input of optical flow fields. Specically, we average the weights across the RGB channels and replicate this average by the channel number of temporal network input.**

另外一个防止过拟合的方式是**partial BN**（属于regularization technique）。Batch Normalization是常用的数据归一化方式，那么partial BN是怎么做呢？原文如下：**Therefore, after initialization with pre-trained models, we choose to freeze the mean and variance parameters of all Batch Normalization layers except the first one. As the distribution of optical flow is different from the RGB images, the activation value of first convolution layer will have a different distribution and we need to re-estimate the mean and variance accordingly.**

至于**data augmentation**，In the original two-stream ConvNets, random cropping and horizontal flipping are employed to augment training samples. **We exploit two new data augmentation techniques: corner cropping and scale-jittering**. In corner cropping technique, the extracted regions are only selected from the corners or the center of the image to avoid implicitly focusing on the center area of a image. **也就是说crop的结果不仅包含center部分，还包含conner部分**。关于scale-jittering，we fix the size of input image or optical flow fields as 256*340, and the width and height of cropped region are randomly selected from {256; 224; 192; 168}. **Finally, these cropped regions will be resized to 224*224 for network training**. In fact, this implementation not only contains scale jittering, but also involves aspect ratio jittering.

**模型测试的时候**，Specically, we follow the testing scheme of the original two-stream ConvNets, where we sample 25 RGB frames or optical flow stacks from the action videos. Meanwhile, we **crop 4 corners and 1 center,** and their horizontal flipping from the sampled frames to evaluate the ConvNets. 另外两路CNN的结果采用加权求均值的方式进行合并，文中用的权重比例是spatial：temporal=1:1.5。When both normal and warped optical flow fields are used, the weight of temporal stream is divided to 1 for optical flow and 0.5 for warped optical flow.

**实验结果：**
**首先数据集主要是UCF101和HMDB51**，The UCF101 dataset contains 101 action classes and 13320 video clips. The HMDB51 dataset is composed of 6766 video clips from 51 action categories.

**Table1是关于不同训练策略下two stream convnets的效果对比**。Baseline就是two-stream的算法结果。主要可以看出cross modality pre-training（主要提升了temporal convnets的效果）和partial BN with dropout是有效的。 
![这里写图片描述](https://img-blog.csdn.net/20180110230119825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**Table2是关于网络的几种不同输入形式下two stream convnets的效果对比，数据集是UCF101。可以看到一般而言融合多种类型的输入可以达到更好的效果，尤其要利用optical flow的信息**。 
![这里写图片描述](https://img-blog.csdn.net/20180110230152016?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**Table3是关于前面TSN公式中的不同g函数对实验结果的影响**。最后文章采用的是average的融合方式。 
![这里写图片描述](https://img-blog.csdn.net/20180110230222989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**Table5是总结本文的一些基于two stream的改进效果。**
![这里写图片描述](https://img-blog.csdn.net/20180110230252584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**Table6是和其他视频行为识别算法在HMDB51和UCF101数据集上的对比，效果还是比较明显的。**
![这里写图片描述](https://img-blog.csdn.net/20180110230318683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**最后的Figure3是作者对网络的可视化**，个人感觉做的很不错。将两条网络，以及是否预训练的差异表达出来了。预训练后的网络显然可以提取到有效的特征信息。 
![这里写图片描述](https://img-blog.csdn.net/20180110230342408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)













