
# 小数据福音！BERT在极小数据下带来显著提升的开源实现 - Paper weekly - CSDN博客


2018年11月27日 12:09:46[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1051


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨金立达
单位丨笨鸟社交 AI Lab
学校丨英国帝国理工学院
研究方向丨自然语言处理、知识图谱

标注数据，可以说是 AI 模型训练里最艰巨的一项工作了。自然语言处理的数据标注更是需要投入大量人力。相对计算机视觉的图像标注，文本的标注通常没有准确的标准答案，对句子理解也是因人而异，让这项工作更是难上加难。

但是，谷歌最近发布的*BERT*[1]大大地解决了这个问题！根据我们的实验，BERT 在文本多分类的任务中，能在极小的数据下带来显著的分类准确率提升。并且，实验主要对比的是仅仅 5 个月前发布的 State-of-the-Art 语言模型迁移学习模型 –*ULMFiT*[2]，结果有着明显的提升。我们先看结果：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSDb1zB5pDqv6xU6qEB9iaSONOeIYIbeiaAxiaXVTiaxmvWAiay3KcvwEuGaQ/640)
▲图1. 实验结果对比，BERT在极少的数据集上表现非常出色

从上图我们可以看出，在不同的数据集中，BERT 都有非常出色的表现。我们用的实验数据分为 1000、 6700 和 12000 条，并且各自包含了测试数据，训练测试分割为 80%-20%。数据集从多个网页来源获得，并经过了一系列的分类映射。但 Noisy 数据集带有较为显著的噪音，抽样统计显示噪音比例在 20% 左右。

实验对比了几个模型，从最基础的卷积网络作为 Baseline，到卷积网络加上传统的词向量 Glove embedding， 然后是*ULMFiT*和**BERT**。为了防止过拟合，CNN 与 CNN+Glove 模型训练时加入了 Early stopping。

值得注意的是，**这里用的 BERT 模型均为基础版本**，“BERT-Base, Uncased”，12 层，110M 参数，**对比的是 ULMFiT 调整过的最优化参数**。可见 BERT 在此任务中的强大。

然而，在 12000 条样本的数据集上，BERT 的结果相对 6700 条并没有显著的提升。数据分类不平衡可能是导致此结果的一大因素。


**BERT 开源的多个版本的模型：**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSYdLqJp7nPjpjPBXOoBD7rOyLKVPice6B8ic2LujyoWejsGP9nZnGicI2A/640)
▲图2. 开源的多个版本的BERT模型

接下来，我们直奔主题 –**如何在自己的机器上实现 BERT 的文本 25 分类任务**。教程分为以下几部分：

运行环境
硬件配置
下载模型
输入数据准备
实现细节


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSfOumlDFZicQr7HLYPMc9ykJ2ej0icNdlnCqgj3icMFf5oBDhgu0hAliaicA/640)

# 运行环境

TensorFlow 版本为 Windows 1.10.0 GPU，具体安装教程可以参考此链接：

https://www.tensorflow.org/install/pip?lang=python3

Anaconda 版本为 1.9.2。


# 硬件配置

实验用的机器显卡为 NVIDIA GeoForce GTX 1080 Ti，BERT base 模型占用显存约为 9.5G。

# 下载模型

所有的运行环境设置好后，在这里可以下载到我们实验用的 BERT base：

https://storage.googleapis.com/bert_models/2018_10_18/uncased_L-12_H-768_A-12.zip

下载完后，放在*BERT_BASE_DIR*中。


# 输入数据准备

**我们需要将文本数据分为三部分：**

Train:train.tsv
Evaluate:dev.tsv
Test:test.tsv

下面可以看到每个文件的格式，非常简单，一列为需要做分类的文本数据，另一列则是对应的 Label。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSib7JzNF6ibTjEW986gB8L7kDXb3JSVtQROgasBhhrYbap0SI2IJ9IHRg/640)
▲图3. 输入文本格式样板

并将这三个文件放入*DATA_DIR*中。

# 实现细节

首先我们 Clone 官方的 BERT Github repo：

https://github.com/google-research/bert

由于我们要做的是文本多分类任务，可以在*run_classifier.py*基础上面做调整。

这里简单介绍一下这个脚本本来的任务，也就是 BERT 示范的其中一个任务。这个例子是在 Microsoft Research Paraphrase Corpus (MRPC) corpus 数据集上面做微调，数据集仅包含 3600 个样本，在 GPU 上面几分钟就可完成微调。

此数据集可以用以下脚本下载：

https://gist.github.com/W4ngatang/60c2bdb54d156a41194446737ce03e2e

注意运行的时候要用 --*tasks all*参数来下载。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSUHLG6CXhUfmFM1GL7uALGrricqGJ4WU2gjxQPZAbGZP08XtViavaiapgQ/640)
▲图4. 运行脚本下载MRPC数据集

可以打开看一下输入数据的结构，都是以*tsv*的形式保存：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSKxZXI567fcj9ugibo4scoVtNnjibE0Dpm0h7KKZY8Oia4tHHYh3bhlMuw/640)
▲图5. MRPC数据集输入数据样本

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSmQEZGqnFDQfyVSu3qNyW3YLElUJxLPkY1bd8ynKyb3czQiaicOPhq34g/640)
▲图6. MRPC数据集结构样本

这里 MRPC 的任务是 Paraphrase Identification，输入为两个句子，然后判断二者是否表示相同的意思，输出为二分类：是和不是。我们的分类任务只需要一个输入，而不是一对句子，这个在读取的过程中可以自动识别，并调整相应的 Sentence Embedding 如下图所示：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzS2eacs8DQJLibGWyPbm9Syka7cWlqRVZVib1ibIM06xHfQJarrQMLxuI8w/640)
▲图7. BERT Sentence Embedding自动调整过程

*run_classifier.py*的脚本中，由于输入格式和之前有少许不同，我们需要更改*_create_examples*函数里面的读取顺序，原本的读取位置为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSALib8icmJHCr03ibLUPQcrQsia2JN6rfx1AYlBzicHvIZD2AozB8H5ET7mg/640)
▲图8. MRPC数据集输入文本读取方式

我们需要让*text_a*读取被分类的文本，而*label*读取我们的标注：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSrJWgLHYZiamto0iaVgibgqicIo7icicLtNnxlSxuhQ071b4UPXhsiaTwk3FicQ/640)
▲图9. 在文本多分类的任务中，读取输入的方式

同时由于没有*text_b*，我们需要在后面制作*example*的时候将他赋值为*None*：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSsetN2BaYjTAchFA5YP5sreEfcARj9TBx9Vm8ObUxD3D2s7mw9FhibvQ/640)
▲图10. 由于没有Sentence Pair输入，在这里需要将*text_b*定义为*None*

接下来，相对于原本的二分类，我们需要针对多分类做出一些调整。代码中原本将标签列表手动设置为 0 和 1：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSD5DGpCfoQd81zEkiak3TQk8iaVWCJvHaePgIgsRJW4rMG5070AYNx2jQ/640)
▲图11. 原本直接将标注列表定义为0和1

这里我们加入一个新的输入，然后将输出调整如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSQqty7qaVTWyZR4pHBVEn5qGlvnFpSfck7SBYeHDN55xussz4lloVjA/640)
▲图12. 调整get_labels的输入和输出

这里*labels*输入为新添加的所有训练样本的 Label，然后通过*set()*返回所有 25 个标签的列表。调整之后，代码可以自动根据分类数量定义标签列表，可以适配多种多分类任务。

同时，在*_create_examples*中，我们增加两个返回值，*labels*和*labels_test*：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSHPhAwxl5w98a5gPKrQVTtGYNwEqxloexBuiby9buI1DKJiakT1ib0pogQ/640)
▲图13.*_create_examples*函数增加两个返回值，*labels*和*label_test*

*labels*返回的是所有训练样本的 label，用来输入到之前提到的*get_labels()*。*Labels*的定义如下图所示：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSKuBcqZ84lM8gDa4ZIDpwdSwicDDMwQBJQxVlyO259fvQOAJCPwy12jQ/640)
▲图14. 新添加的变量labels

接下来我们需要调整*main() function*里面的一些顺序，因为现在的*get_labels()*需要额外的输入（读取的完整 label list），我们需要把读取训练集的步骤放到前面。**原来的顺序：**

**1. 获取****label_list****；**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSuER8seJR2p7rEwcRJ9NYylzXFWqW0Ycp8zF1BN3cW74KZQLUlLwiaQA/640)
▲图15. 第一步

**2. 如果在训练模式，再读取训练集。**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSI07k6Aia7NOlvLgV91nCCKBfMuxMXbkljUxcicf4NgaZUm4agwPnMhRw/640)
▲图16. 第二步

现在需要调整为：

**1. 无论什么模式都读取训练集**，因为需要用到训练标签，注意新添加的输出变量*train_labels*；

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSP2ULXqJIibpIItfibP72yTgY1hwzpjuJiaJZpTUWTUJZs27GNXA1OexVg/640)
▲图17. 第一步

**2. 然后再获取****label_list****，用前面的****train_labels****。**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSLLAeBqsgSvNnC9YnEFwKMdZzYrLNHQJneUANcl2SL5WUdjMpxEyYXQ/640)
▲图18. 第二步

最后，我们在开头设置好参数，可以直接输入默认值来运行。下面拿*DATA_DIR*来举例：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzS8bEEL218OdicJmg4qYrl85clk2FBTr1GOc3XEk140vIZwmX0iakIEibmw/640)
▲图19. 原始参数

调整后的输入参数：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSrZFINE8SSiaTAqSXrkqnuPHoTz9Tc9MqmqLlbUkqKp2QcGqCrf2KrKg/640)
▲图20. 调整后的参数

1000 条样本数据 10 分类，BERT 运行结果如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSr7sOU6dkmILM2SX1ian8XOXxMqDYcXaF0uY45hFenicQJnXibzuE5QEGQ/640)
▲图21. 1000条样本数据10分类BERT结果

# 总结

本文介绍了如何实现 BERT 的文本多分类任务，并对比了 Baseline 以及不久前的State-of-the-Art模型*ULMFiT*。实验结果可以看出 BERT 在此任务中，可以轻松打败先前的 SOTA。

**这里附上本教程的开源代码：**

https://github.com/Socialbird-AILab/BERT-Classification-Tutorial

我们依然会在 BERT 的基础上不断尝试，继续努力研究，也欢迎大家积极交流。

# 参考文献

[1] Devlin, Jacob and Chang, Ming-Wei and Lee, Kenton and Toutanova, Kristina. BERT: Pre-training of Deep Bidirectional Transformers for Language Understanding. arXiv preprint arXiv:1810.04805.
[2] Jeremy Howard and Sebastian Ruder. 2018. Universal language model fine-tuning for text classification. In ACL. Association for Computational Linguistics.

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看作者其他文章：**

[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)
[深度](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)[解读DeepMind新作：史上最强GAN图像生成器](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)
[这16篇最新论文，帮你轻松积攒知识点](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492912&idx=1&sn=e223b0bf570148493313ea8780cef2fc&chksm=96ea3ab0a19db3a6eb87b8c8d6cb41d1a4ae0d85b5fd7b616baa970234124c320fda1cdcc7d9&scene=21#wechat_redirect)
[TensorSpace：超酷炫3D神经网络可视化框架](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492746&idx=1&sn=921d7315a973b85dd4e802cb5fd456fb&chksm=96ea3b0aa19db21c48841ddcee38592a3c086ae8fa1a9893cf46ff974f0f38fb350bcd528265&scene=21#wechat_redirect)

**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 获取最新论文推荐


