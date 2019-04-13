
# caffe+鉴黄︱yahoo+open_nsfw 中resnet_50_1by2、遇到问题 - 素质云笔记-Recorder... - CSDN博客

2017年03月15日 17:27:35[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：8207



> NSFW：Not Suitable for Work； SFW：Suitable for Work

> github：
> [https://github.com/yahoo/open_nsfw](https://github.com/yahoo/open_nsfw)
![这里写图片描述](https://img-blog.csdn.net/20170315172216120?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170315172216120?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 同样参考机器之心的文章：
> [《雅虎开源首个色情图像检测深度学习解决方案》](http://tech.163.com/16/1001/13/C29UEO4N00097U80.html)

> .

> [
](https://img-blog.csdn.net/20170315172216120?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 公众号“素质云笔记”定期更新博客内容：
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 一、NSFW来历

> 1、模型来历

> finetuned来源于ImageNet 1000 class dataset。使用的框架是：resnet 50 1by2

> 其中将每个网络的最后一层（FC1000）更换为 2 节点的全连接层。然后我们精调 NSFW 数据集中的权重。注意我们让与最后的 FC 层相乘的学习率是精调后的其他层的 5 倍。我们还调整了超参数（hyper parameters）（步长、基本学习率）以优化性能。

> 2、色情指数

> 雅虎公开的此算法为概率0-1值，二分类。

> 分数<0.2代表大致安全；分数>0.8代表很高可能性不适合办公环境观看。

> 当然，这个阈值根据实际情况确定。

> .

> .

> 二、caffe的实践

> .

> 1、GPU报错

> 参考来源：
> [ResNet-50 Testing](https://www.bountysource.com/issues/35937456-resnet-50-testing)

> Check failed: target_blobs.size() == source_layer.blobs_size() (
> 5
> vs.
> 3
> ) Incompatible
> number
> of
> blobs
> for
> layer bn_conv1
> 笔者在实践GPU时候遇到这个问题，那么网上的说法为：

> It looks like that Batchnorm layer and adjacent Scale layer are

> integrated in single Batchnorm layer in NVIDIA caffe. However original

> BVLC/caffe doesn’t integrate them. That means any networks and

> pretrained models using batchnorm and scale layers with original Caffe

> cannot use in NVIDIA caffe and DIGITS.

> original Caffe包含：Batchnorm层和adjacent Scale层分开的；

> NVIDIA Caffe/DIGITS包含：Batchnorm层和adjacent Scale层合并在一个BN层中；

> 所以在预测过程中不能使用NVIDIA Caffe，也就是GPU。

> 那么之后，调用了CPU就可以了，但是比较慢，一张图12s左右（K80）.当然，他们还使用了CaffeOnSpark，后者是一个用于分布式学习的强大开源框架，令你可以在 Hadoop 和 Spark 模型训练集群中使用 Caffe 深度学习。

> .

> 2、框架的选择与数据操作

> 没有公开的数据集（你懂得…），在训练过程中，图像被重新调整到256x256像素，水平翻转进行数据增强，并被随机裁剪为224x224像素，然后送入网络。在训练残差网络时，我们使用了 ResNet 论文中所描述的规模增大（scale augmentation）来避免过度拟合。

> 模型的选择：考察了以下几类框架MS_CTC（微软限制时间成本的那篇论文提出。它在卷积层和全连接层相结合的速度和精度方面秒杀了 AlexNet）、Squeezenet（架构提出了 fire 模块——包含层挤压，然后扩大输入数据团。这有助于节省参数数量，使 Imagenet 的精度与 AlexNet 的一样好，尽管内存需求仅为6MB）、VGG（常规的，这种架构有13层卷积层和 3层 FC 层）、GoogLeNet（GoogLeNet 提出了 Inception 模块并拥有 20 个卷积层阶段。它还在中间层中使用 hanging loss functions 来解决深度网络中的梯度递减问题）、ResNet（ResNet 使用快捷连接来解决梯度递减问题）

> 最后发布的是： thin ResNet 50 模型，因为它在准确度方面做了很好的折中，并且该模型在运行时间（CPU 上运行时间 < 0.5 秒）和内存（~ 23 MB）方面体量轻巧。

> .

> 3、resnet-50_1by2中：BN层中的use_global_stats

> （参考博客：
> [caffe Resnet-50 finetune 所有代码+需要注意的地方](http://blog.csdn.net/tangwenbo124/article/details/56070322)
> ）

> 1.在训练时所有BN层要设置use_global_stats: false（也可以不写，caffe默认是false）

> 2.在测试时所有BN层要设置use_global_stats: true

> 影响：

> 1.训练如果不设为false，会导致模型不收敛

> 2.测试如果不设置为true，会导致准确率极低

> 区别：

> use_global_stats: false是使用了每个Batch里的数据的均值和方差；

> use_global_stats: true是使用了caffe内部的均值和方差。

> 其中：resnet_50_1by2，resnet_50,resnet68_1by2等这些框架的caffemodel+deploy可见github：
> [https://github.com/jay-mahadeokar/pynetbuilder/tree/master/models/imagenet](https://github.com/jay-mahadeokar/pynetbuilder/tree/master/models/imagenet)

> .

> 4、命令行调用SNFW

> python ./classify_nsfw.py \
 -
> -model_def nsfw_model/deploy.prototxt \
> -
> -pretrained_model nsfw_model/resnet_50_1by2_nsfw.caffemodel \
> INPUT_IMAGE_PATH
> .

> 延伸一：生成网络-利用open_nsfw模型生成情 色图片

> 脑洞太大，有人尝试了用开源的nsfw来生成图像，
> [博客链接](https://open_nsfw.gitlab.io/)
> 。

> 像google的deep dream一样，最大化激活某些特定的分类神经元，不同的是通过生成网络的G函数优化激活过程，同时进行下面的梯度优化，获得了一些有意义的图像生成：
![这里写图片描述](https://img-blog.csdn.net/20170316134345638?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170316134345638?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 贴一些SFW的图像:
![这里写图片描述](https://img-blog.csdn.net/20170316134517656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> 延伸二：雅虎源代码运行时报错

> IOError：cannot identify
> image
> file
> <StringIO.stringIO
> instance
> at xxxxxxxxxxx>
> 不知道为什么老是不能用stringIO？？

> 后面笔者自己就直接抛弃stringIO，网上有人用opencv改过这部分，cv2.imwrite，可见
> [博客](http://tieba.baidu.com/p/4827109128)

> .

> 延伸三：利用YCbCr理论的肤色检测简单鉴定

> YCbCr 色彩空间

> Y’为颜色的亮度(luma)成分、而CB和CR则为蓝色和红色的浓度偏移量成份

> def
> Nsfw_YCbCr
> (path)
> :
> img = Image.open(path).convert(
> 'YCbCr'
> )  
    w, h = img.size
    data = img.getdata()
    cnt =
> 0
> for
> i, ycbcr
> in
> enumerate(data): 
        y, cb, cr = ycbcr
> if
> 86
> <= cb <=
> 117
> and
> 140
> <= cr <=
> 168
> :
            cnt +=
> 1
> print
> '%s %s a porn image.'
> %(path,
> 'is'
> if
> cnt > w * h *
> 0.3
> else
> 'is not'
> )
> 参考来源：
> [（已加马赛克）10 行代码判定色](http://blog.csdn.net/gzlaiyonghao/article/details/3166735)
> 情
> [图片——Python 也可以系列之二](http://blog.csdn.net/gzlaiyonghao/article/details/3166735)

> .

> 延伸四：一些灵感

> 来源知乎：
> [小密圈的反黄策略该怎么做？](https://zhuanlan.zhihu.com/p/25991807)

> 现在的问题是，我发布的一些图片，没有涉黄，只露臀但不暴露敏感部位，被误判为黄图，而其他圈子的有些图片，暴露了敏感部位，而通过了机器的审核。这么看来，这个黄图的缺点在于，对是否暴露敏感部位的识别功能不够！一种可能是，标注样本中，模棱两可的图片不够充分，模型学不到真正的Decision Boundary。一方面，可以补充多点这种图片往模型训练。另外一方面，分别对敏感部位进行图像识别建模，比如敏感部位是两个，那么再增加两个模型，当然这两个模型不能对原图直接判别，已经超越了图像识别，而是属于图像检测。

> 优化方案：

> 1 模型1的概率：CNN黄图识别模型

> 2 模型2的概率：敏感部位1的检测模型

> 3 模型3的概率：敏感部位2的检测模型

> 4 用户近1天，7天，30天，90天上传黄图的次数

> 5 圈子近1天，7天，30天，90天上传黄图的次数

> .

> 延伸五：肤色像素检测与皮肤区域划分算法

> 本程序根据颜色（肤色）找出图片中皮肤的区域，然后通过一些条件判断是否为色情图片。

> 详细可参考实验楼code：
> [https://www.shiyanlou.com/courses/589/labs/1964/document](https://www.shiyanlou.com/courses/589/labs/1964/document)

> 程序的关键步骤如下

> 遍历每个像素，检测像素颜色是否为肤色

> 将相邻的肤色像素归为一个皮肤区域，得到若干个皮肤区域

> 剔除像素数量极少的皮肤区域

> 我们定义非色情图片的判定规则如下（满足任意一个判定为真）：

> 皮肤区域的个数小于 3 个

> 皮肤区域的像素与图像所有像素的比值小于 15%

> 最大皮肤区域小于总皮肤面积的 45%

> 皮肤区域数量超过60个

> 这些规则你可以尝试更改，直到程序效果让你满意为止

> 关于像素肤色判定这方面，公式可以在网上找到很多，但世界上不可能有正确率 100% 的公式

> 你可以用自己找到的公式，在程序完成后慢慢调试

> RGB 颜色模式

> 第一种：r > 95 and g > 40 and g < 100 and b > 20 and max([r, g, b]) - min([r, g, b]) > 15 and abs(r - g) > 15 and r > g and r > b

> 第二种：nr = r / (r + g + b), ng = g / (r + g + b), nb = b / (r +g + b), nr / ng > 1.185 and r * b / (r + g + b)
> * 2 > 0.107 and r
> g / (r + g + b) ** 2 > 0.112

> HSV 颜色模式

> h > 0 and h < 35 and s > 0.23 and s < 0.68

> YCbCr 颜色模式

> 97.5 <= cb <= 142.5 and 134 <= cr <= 176

> 一幅图像有零个到多个的皮肤区域，程序按发现顺序给它们编号，第一个发现的区域编号为 0，第 n 个发现的区域编号为 n-1

> 我们用一种类型来表示像素，我们给这个类型取名为 Skin，包含了像素的一些信息：唯一的 编号（id），是/否肤色（skin），皮肤区域号（region），横坐标（x），纵坐标（y）

> 遍历所有像素时，我们为每个像素创建一个与之对应的 Skin 对象，并设置对象的所有属性

> 其中 region 属性即为像素所在的皮肤区域编号，创建对象时初始化为无意义的 None

> 关于每个像素的 id 值，左上角为原点，像素的 id 值按像素坐标排布，那么看起来如下图
![这里写图片描述](https://img-blog.csdn.net/20170402115117838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 其实 id 的顺序也即遍历的顺序

> 遍历所有像素时，创建 Skin 对象后，如果当前像素为肤色，且相邻的像素有肤色的，那么我们把这些肤色像素归到一个皮肤区域

> 相邻像素的定义：通常都能想到是当前像素周围的 8 个像素，然而实际上只需要定义 4 个就可以了，位置分别在当前像素的左方，左上方，正上方，右上方；因为另外四个像素都在当前像素后面，我们还未给这4个像素创建对应的 Skin 对象
![这里写图片描述](https://img-blog.csdn.net/20170402115130636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 延伸六：Clarifai公司推出识别成人内容的模型和API

> 【图像识别】著名人工智能、视频分析、深度学习公司Clarifai公司官网，前天推出了一项新的服务，即用于识别成人内容的模型和API，据说模型很准确能识别含有裸体和半裸的图片和视频具体参见：
> [http://blog.clarifai.com/moderate-filter-or-curate-adult-content-with-clarifais-nsfw-model/\#.Vw9I3bEis_8[](http://blog.clarifai.com/moderate-filter-or-curate-adult-content-with-clarifais-nsfw-model/#.Vw9I3bEis_8[)

> 昨天他们发布一篇技术方面的博文，深度学习是如何“看”裸体的。从1996的论文“Finding Naked People”说起，当时只有60%的准确性，经常和近似皮肤颜色的图片混淆，例如动物皮肤、木材、面包、白色墙壁，然后分析2014年他们的Paper，并给出些代码和结果。

> 详细地址见：
> [http://blog.clarifai.com/what-convolutional-neural-networks-see-at-when-they-see-nudity/\#.VxbLgLEis_9](http://blog.clarifai.com/what-convolutional-neural-networks-see-at-when-they-see-nudity/#.VxbLgLEis_9)
![这里写图片描述](http://www.thinkface.cn/data/attachment/forum/201604/21/122955oajzlgqbd9qyx5b8.png)
> .

> 延伸六：
> [视频鉴黄大家都听过，那音频审核的技术你知道多少呢？](https://mp.weixin.qq.com/s?__biz=MTM2ODM0ODYyMQ==&mid=2651422844&idx=2&sn=8a7246c85e66959f8f758f681e7fa3e3&chksm=624d80de553a09c8b9f827c7d26d70cbfcf54fc916014f77012eec4ec4b90beb489c75535acb&mpshare=1&scene=1&srcid=0417dMgTtqiX3g4mwdeHpMkM#rd)

> 来源雷锋网

> 基于图像识别得视频涉黄检测准确率可以达到99%以上，可以为视频直播平台节省70%以上的工作量。

> 还有一些是语音为主的直播节目，比如谈话聊天、脱口秀、在线广播等。视频检测所使用到的图像技术就很难在这些应用场景发挥作用，所以音频检测需要有针对性的技术手段。

> 语音识别的关键技术——声学模型

> 语音识别的声学模型主要有以下两种：混合声学模型和端到端的声学模型。

> 混合声学模型通常是隐马尔科夫模型结合混合高斯、深度神经网络、深度循环神经网络以及深度卷积神经网络的一个模型。

> 端到端声学模型目前有两大类，一是连接时序分类—长短时记忆模型，二是注意力模型。

> .

> 延伸七：
> [成人网站PornHub爬虫，一天可爬取500万以上的视频](https://mp.weixin.qq.com/s?__biz=MzA4NzE1NzYyMw==&mid=2247489951&idx=4&sn=a101122e986e286cb3416b1f2ce1299e&chksm=903ce787a74b6e91e0840528803fc554c8b4ffa477ef47e02625d728ed6288d7a2edee7feb2d&mpshare=1&scene=1&srcid=0503McLR1xTSPjv2LwVcKznn#rd)

> GitHub资源：
> [https://github.com/xiyouMc/WebHubBot](https://github.com/xiyouMc/WebHubBot)

> 开发语言: Python2.7

> 开发环境: MacOS系统、4G内存

> 数据库: MongoDB：

> 主要使用 scrapy 爬虫框架

> 从Cookie池和UA池中随机抽取一个加入到Spider

> start_requests 根据 PorbHub 的分类，启动了5个Request，同时对五个分类进行爬取。

> 并支持分页爬取数据，并加入到待爬队列。


