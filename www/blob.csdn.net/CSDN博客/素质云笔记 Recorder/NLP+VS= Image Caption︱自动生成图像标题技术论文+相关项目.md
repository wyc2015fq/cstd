# NLP+VS=>Image Caption︱自动生成图像标题技术论文+相关项目 - 素质云笔记/Recorder... - CSDN博客





2017年01月22日 18:40:11[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：11239标签：[计算机视觉																[自然语言处理																[机器学习](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[图像︱相关技术跟踪与商业变现](https://blog.csdn.net/sinat_26917383/article/category/6818531)





> 
读聪明人的笔记，是不是也能变聪明呢？


Image Caption是一个融合计算机视觉、自然语言处理和机器学习的综合问题，它类似于翻译一副图片为一段描述文字。 

Image Caption问题可以定义为二元组(I,S)的形式， 其中I表示图，S为目标单词序列，其中S={S1,S2,…}，其中St为来自于数据集提取的单词。训练的目标是使最大似然p(S|I)取得最大值，即使生成的语句和目标语句更加匹配，也可以表达为用尽可能准确的用语句去描述图像。

Image Caption主要研究分为以下几个方向：

1、用单独的CNN来获取图像的特征，然后，利用这些特征进行生成句子（排序，检索，生成）； 

2、将CNN获取的特征和句子特征联合嵌入到一个空间内，然后从中进行选择最优描述； 

3、利用一些全新的机制，将CNN和RNN结合，目的在利用CNN的全局特征或者局部特征来指导描述的生成。 

——以上三种类似神经网络翻译——- 

4、使用模板的方法，填入一些图像中的物体； 

5、使用检索的方法，寻找相似描述。这两种方法都使用了一种泛化的手段，使得描述跟图片很接近，但又不那么准确。
（博客：[image caption：Show, Attend and Tell: Neural Image Caption Generation with Visual Attention](http://www.cnblogs.com/taojake-ML/p/6113459.html) ） 

.

# 1 NIC——最简版encoder-decoder

> 
Vinyals O, Toshev A, Bengio S, et al. Show and tell: A neural image 

  caption generator[J]. Computer Science, 2015:3156-3164.

用CNN来学图像特征，全连接层后接LSTM，让其学写描述句子。


效果： 
![这里写图片描述](https://img-blog.csdn.net/20170122175845921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
当前大多数的Image Caption方法基于encoder-decoder模型。其中encoder一般为卷积神经网络，利用最后全连接层或者卷积层的特征作作为图像的特征，decoder一般为递归神经网络，主要用于图像描述的生成。 

由于普通RNN存在梯度下降的问题，RNN只能记忆之前有限的时间单元的内容，而LSTM是一种特殊的RNN架构，能够解决梯度消失等问题，并且其具有长期记忆，所以一般在decoder阶段采用LSTM.

![这里写图片描述](https://img-blog.csdn.net/20170122175619632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Show and Tell: A Neural Image Caption Generator 

本文提出了一种encoder-decoder框架，其中通过CNN提取图像特征，然后经过LSTM生成目标语言，其目标函数为最大化目标描述的最大似然估计。 
![这里写图片描述](https://img-blog.csdn.net/20170122175645060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

该模型主要包括encoder-decoder两个部分。encoder部分为一个用于提取图像特征的卷积神经网络，可以采用VGG16，VGG19, GoogleNet等模型, decoder为经典的LSTM递归神经网络，其中第一步的输入为经过卷积神经网络提取的图像特征，其后时刻输入为每个单词的词向量表达。对于每个单词首先通过one-hot向量进行表示，然后经过词嵌入模型，变成与图像特征相同的维度。
参考于：爱可可-爱生活微博、paperweekly公众号发文 

. 

.
# 2、MS Captivator

> 
Fang H, Gupta S, Iandola F, et al. From captions to visual concepts 

  and back[C]// IEEE Conference on Computer Vision and Pattern 

  Recognition. IEEE, 2015:1473-1482.
先通过目标检测+物体识别，把图像中的实体词都识别出来，实体词相关之间的连接词是构造完整句子的核心，文章用了Multiple Instance Learning(MIL)的弱监督方法进行造句。

![这里写图片描述](https://img-blog.csdn.net/20170122180702132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

步骤是： 

detect words：识别实体词 

generate sentences：生成句子 

re-rank sentences：重整句子结构
![这里写图片描述](https://img-blog.csdn.net/20170122180921338?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

同时，文章《Yao T, Pan Y, Li Y, et al. Boosting Image Captioning with Attributes[J]. 2016.》研究了图像属性特征对于描述结果的影响，其中图像属性特征通过多实例学习[2]的方法进行提取。获得标签项后进行文本重组，使用了5种不同的办法，来增强利用所提取的关键词。

参考于：paperweekly公众号发文 

. 

.
# 3、Hard-Attention Soft-Attention

> 
Xu K, Ba J, Kiros R, et al. Show, Attend and Tell: Neural Image 

  Caption Generation with Visual Attention[J]. Computer Science, 

  2016:2048-2057.
本文的贡献是1、提出两种attention机制利用在image caption任务中，hard和soft；2、利用可视化手段来清晰的理解attention机制的效果。 

在encoder阶段 

与之前直接通过全连接层提取特征不同，作者使用较低层的卷积层作为图像特征，其中卷积层保留了图像空间信息，然后结合注意机制，能够动态的选择图像的空间特征用于decoder阶段。
在decoder阶段 

输入增加了图像上下文向量，该向量是当前时刻图像的显著区域的特征表达。

![这里写图片描述](https://img-blog.csdn.net/20170122181705194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

一部分编码部分encoder，目的是获取image的特征，不同于其他方法直接将最后全连接层的vector（反映图片整体特征）拿过来，此处，作者提取的是卷积层的输出，这样能够将局部的图片信息提取出来，分别进行生成sentence；另一部分为解码部分，采用LSTM模型，其结构如下： 
![这里写图片描述](https://img-blog.csdn.net/20170122181714163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

attention的加入，能够显著提高描述的性能，并且可分为hard和soft两种attention机制，hard更难进行训练和理解，但hard相对soft，其提高并没有很明显，需要继续改进和提高。
参考于：[机器都能学习–>你还不学习？](http://www.cnblogs.com/taojake-ML/)博客、paperweekly公众号发文 

. 

.
# 4、sentence-condition

> 
Zhou L, Xu C, Koch P, et al. Image Caption Generation with 

  Text-Conditional Semantic Attention[J]. 2016.


encoder：该模型首先利用卷积神经网络提取图像特征 

decoder:然后结合图像特征和词嵌入的文本特征作为gLSTM的输入。 

由于之前gLSTM的guidance都采用了时间不变的信息，忽略了不同时刻guidance信息的不同，而作者采用了text-conditional的方法，并且和图像特征相结合，最终能够根据图像的特定部分用于当前单词的生成。 
![这里写图片描述](https://img-blog.csdn.net/20170122182517905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**关于gLSTM**

语义LSTM（Guiding long-short term memory for image caption generation）： 

其中gLSTM是使用语义信息来指导LSTM在各个时刻生成描述。由于经典的NIC[1]模型，只是在LSTM模型开始时候输入图像，但是LSTM随着时间的增长，会慢慢缺少图像特征的指导，所以采取了三种不同的语义信息，用于指导每个时刻单词的生成，其中guidance分别为Retrieval-based guidance (ret-gLSTM), Semantic embedding guidance(emb-gLSTM) ,Image as guidance (img-gLSTM). 

参考于：爱可可-爱生活微博、paperweekly公众号发文 

. 

.
# 5、Att-CNN+LSTM

> 
Wu Q, Shen C, Liu L, et al. What Value Do Explicit High Level Concepts 

  Have in Vision to Language Problems?[J]. Computer Science, 2016.


![这里写图片描述](https://img-blog.csdn.net/20170122182946058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

encoder：作者首先利用VggNet模型在ImageNet数据库进行预训练，然后进行多标签数训练。给一张图片，首先产生多个候选区域，将多个候选区域输入CNN产生多标签预测结果，然后将结果经过max pooling作为图像的高层语义信息，最后输入到LSTM用于描述的生成（decoder）。 

该方法相当于保留了图像的高层语义信息，不仅在Image Caption上取得了不错的结果，在VQA问题上，也取得很好的成绩。 
![这里写图片描述](https://img-blog.csdn.net/20170122183027621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考于：paperweekly公众号发文

.
6、Show and Tell: 神经图说生成器
> 


GitHub 地址：[](https://github.com/tensorflow/models/tree/master/im2txt)https://github.com/tensorflow/models/tree/master/im2txt

  来源文章《【榜单】GitHub 最受欢迎深度学习应用项目 Top 16（持续更新）》

这是 Oriol Vinyals et. al.（2016）的论文“Show and Tell: Lessons learned from the 2015 MSCOCO Image Captioning Challenge”的用TensorFlow实现的 image-to-text 图片说明生成模型。

Show and Tell 模型是一个学习如何描述图片的深度神经网络。生成的图片说明是一个完整的句子，下面是一些例子： 
![这里写图片描述](https://img-blog.csdn.net/20170123155600841?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.7、NeuralTalk2
> 


Github 地址：[](https://github.com/karpathy/neuraltalk2)https://github.com/karpathy/neuraltalk2 来源文章《【榜单】GitHub 

  最受欢迎深度学习应用项目 Top 16（持续更新）》

![这里写图片描述](https://img-blog.csdn.net/20170123160003030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

循环神经网络（RNN）可以用于给图像取标题。NeuralTalk2 比原始版本的 NeuralTalk 更快而且性能更好。与原来的 NeuralTalk 相比，NeuralTalk2 的实现是批量的，可以使用 Torch 在 GPU上运行，并且支持 CNN 微调。这些都使得语言模型（~100x）的训练速度大大加快，但由于我们还有一个 VGGNet，因此总体上的提升没有很多。但是这仍然是个好模型，可以在 2~3 天里训练好，而且表现出的性能非常好。

Google Brain 2016年9月22日发布了 Vinyals et al.（2015）的图说模型（前文介绍的Show and Tell 模型）。它的核心模型与 NeuralTalk2（一个CNN后面跟着RNN）非常相似，但由于 Google 有更好的CNN，加上一些小技巧和更细致的工程，Google 发布的模型应该比 NeuralTalk2 的效果更好。这个项目里用 Torch 实现的代码将作为教育目的保留。 

.8、neural-storyteller
> 


GitHub 地址：[](https://github.com/ryankiros/neural-storyteller)https://github.com/ryankiros/neural-storyteller

  来源文章《【榜单】GitHub 最受欢迎深度学习应用项目 Top 16（持续更新）》

Neural-storyteller 是一个能够根据图像内容生成一个小故事的循环神经网络。这个 GitHub 库里包含了使用任意图像生成故事的代码，以及用于训练新模型的说明。整个方法包含以下4个部分： 

skip-thought vectors 

image-sentence embeddings 

条件神经语言模型 

风格转换

例如，对下面这张图像，模型生成的故事如下： 
![这里写图片描述](https://img-blog.csdn.net/20170123160112781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

We were barely able to catch the breeze at the beach, and it felt as if someone stepped out of my mind. She was in love with him for the first time in months, so she had no intention of escaping. The sun had risen from the ocean, making her feel more alive than normal. She’s beautiful, but the truth is that I don’t know what to do. The sun was just starting to fade away, leaving people scattered around the Atlantic Ocean. I’d seen the men in his life, who guided me at the beach once more.

.9、由文字生成图片：GAN+txt2img模型

来源于极市平台何之源老师的文章：[](http://mp.weixin.qq.com/s?__biz=MzI5MDUyMDIxNA==&mid=2247484130&idx=1&sn=481e485f2541a13b5336355935085c2b&chksm=ec1fef1bdb68660d4ac590771e9105ca5ecf84dadb1092ea311b87c48fcd2d582a5c703375f9&mpshare=1&scene=1&srcid=0317WQITSkaQIekwxx8DkPZ5#rd)通过文字描述来生成二次元妹子！聊聊conditional GAN与txt2img模型

前些日子在Qiita上看到了一篇神奇的帖子：[](http://qiita.com/Hiroshiba/items/d5749d8896613e6f0b48)http://qiita.com/Hiroshiba/items/d5749d8896613e6f0b48。帖子里面提到利用GAN，通过文字描述来生成二次元图像。这篇文章的作者还把他的想法搭建成了一个网站（网站地址：[](https://hiroshiba.github.io/girl_friend_factory/index.html)https://hiroshiba.github.io/girl_friend_factory/index.html） 

这个网站提供的属性非常多，我简单地把它们翻译了一下 

我会在后面详细地介绍它的实现原理，在那之前，我们先来试着玩一玩这个网站。 

进入网站后，首先要等待模型加载（注意：这个网站国内可能出现连接不上的问题，需要自行解决。另外网站大概会下载70M左右的模型，需要耐心等待。）加载好后，点击上方的“無限ガチャ”（無限ガチャ实际上是“无限扭蛋器”的意思），就可以进行生成了。 

先生成一个金发+碧眼，多次点击生成按钮可以生成多个，效果还可以：

基本原理： 

目标实际上是通过“文字”生成“图像”。为此我们需要解决以下两个问题：- 
如何把文字描述表示成合适的向量。- 
如何利用这个描述向量生成合适的图片。

其中，第一个问题是用了之前一篇论文中的技术（[](https://arxiv.org/abs/1605.05395)https://arxiv.org/abs/1605.05395），这里就不细讲了。这里就假设文字描述为，我们可以通过一个函数将其转换为一个向量。

第二个问题，如何利用向量生成合适的图像？这就是GAN的工作，文中GAN的结构如下图所示：

这实际上就是一个条件GAN(conditional GAN)。对照原始的GAN，只有几个简单的修改。到了这里，就可以直接训练了，也可以完成我们文字 -> 图片的生成任务。但是直接训练的生成图片的质量不是很好，对此作者又提出了两点改进。 

.- 
改进一：GAN-CLS（针对判别器Ｄ的改进）

为什么直接训练的效果不好？仔细想一下，我们会发现，在原始的GAN中，判别网络D只需要做一件事情，即判断生成的图片是否正常。但在这里，判别网络D需要做两件事情，一是和原来一样，判断图片是否正常，二是判断生成的图片是否符合文字描述。

因此，我们可以对原来的训练步骤做一些改进。不仅给D提供生成的图片和真实的图片两类样本，还给D提供真实图片 + 虚假描述的样本，强化Ｄ的训练效果，强迫D判断生成的图片是否真的符合文字描述。具体的训练步骤如下- 
改进二：GAN-INT（针对G的改进）

要理解这部分改进，首先要明白，G的训练梯度只和有关（见上面的图片）。是什么呢？它只和G生成的图片和正确的文字描述两项有关系。注意到是和真实图片无关的，只和文字描述有关。对于文字描述，我们可不可以用一种方法，增加它的样本数呢？

答案是可以，因为我们只用到了文字描述的嵌入，在嵌入空间中我们是可以做一些简单的加减运算来增加样本数的。

设一个文字描述是，另一个文字描述是，我们可以得到他们的一个内插值。其中。这样的内插实际上是得到了两个文字描述的某种“中间态”，为我们增加了样本数量。

我们知道，对于深度学习，样本数量越多，效果就会越好，所以这里的GAN-INT是对效果的提升有帮助的，实验中验证了这一点。

作者把上面的两种改进合在一起，就成了GAN-INT-CLS，也是这篇文章的最终方法。 

放一张论文作者实验的图，他是做了花的生成，最上面一行是Ground Truth，下面依次是GAN，GAN-CLS，GAN-INT，GAN-INT-CLS：

.延伸一：用TensorFlow为图片添加字幕

图片字幕生成模型结合了近年来计算机视觉和机器翻译方面的进步，通过使用神经网络来生成现实图片的字幕。对于一个给定的输入图片，神经图像字幕模型被训练来最大化生成一个字幕的可能性。可以被用来产生新颖的图像描述。例如，下面是用MS COCO数据集训练的一个神经图像字幕生成器所产生的字幕。

![这里写图片描述](https://img-blog.csdn.net/20170507211502312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

利用一个RNN/LSTM网络里的循环特点（想了解更多，请参考这篇“理解LSTM网络”）。这些网络被训练来预测在给定的一系列前置词汇和图片表示的情况下的下一个词。

长短期记忆（LSTM）单元能让这个模型能更好地选择什么样的信息可以用于字幕词汇序列，什么需要记忆，什么信息需要忘掉。TensorFlow提供了一个封装的功能可以对于给定的输入和输出维度生成一个LSTM层。

为了把词汇能变化成适合LSTM的固定长度表示的输入，我们使用一个向量层来把词汇映射成256维的特征（也叫词向量）。词向量帮助我们把词汇表示成向量，同时相似的词向量在语义上也相似。想了解更多关于词向量如何获取不同词汇之间的关系，请看这篇《用深度学习来获取文本语义》。

在这个VGG-16图片分类器里，卷积层抽取出了4096维表示，然后送入最后的softmax层来做分类。因为LSTM单元需要的是256维的文本输入，我们需要把图片表示转化成目标字幕所需的这种表示。为了实现这个目标，我们需要加入另外一个向量层来学习把4096维的图片特征映射成256维的文本特征空间。](https://so.csdn.net/so/search/s.do?q=自然语言处理&t=blog)](https://so.csdn.net/so/search/s.do?q=计算机视觉&t=blog)




