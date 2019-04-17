# 干货合集 | 教你如何使用Excel 9步实现卷积网络 - 知乎
# 



​

Excel实现卷积网络，可能有小伙伴会问，它们两有什么关系？是怎么样实现的？今天，小七来和大家一起分享下，Excel是如何实现卷积网络的，一起来看看这篇有趣、神奇的文章~~

**来源：Medium **

编译：weakish

> 编者按：Dave Smith使用Excel电子表格深入浅出地讲解了卷积神经网络（CNN）是如何识别人脸的。

​
![](https://pic3.zhimg.com/v2-57f9128c4d4b3e424bbc664db1fa2d96_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='422'></svg>)
​

当你入门的时候，可能觉得机器学习很复杂……甚至很可怕。另一方面，电子表格却很简单。电子表格并不酷炫，但却能避免分散你的注意力，同时帮助你以直观的方式可视化代码后面发生的事情。

我将循序渐进地通过电子表格（你可以通过下面的链接下载）向你展示卷积神经网络（CNN）是如何应用于计算机视觉任务的。其中包括一些数学，不过电子表格中包含了所有的公式。

[https://drive.google.com/open?id=1TJXPPQ6Cz-4kVRXTSrbj4u4orcaamtpGvY58yuJbzHk](https://link.zhihu.com/?target=https%3A//drive.google.com/open%3Fid%3D1TJXPPQ6Cz-4kVRXTSrbj4u4orcaamtpGvY58yuJbzHk)

如果无法访问Google网盘，可以在论智公众号（ID: jqr_AI）后台留言excel获取替代下载地址。

这一电子表格模型查看一幅图像，分析它的像素，并预测是否是Elon Musk、Jeff Bezos、Jon Snow……显然这三位是天网的最大威胁。
![](https://pic2.zhimg.com/v2-33300d88e59705b7c20799aabd9808b9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='269'></svg>)
​

终结者视觉——在电子表格中创建卷积神经网络

本文将介绍上图中的9个步骤，每个步骤都会使用类比的方法给你的直觉增压。

本文的目标是使用容易操作的电子表格，提供入门机器学习的简单路径，并向充满好奇心的读者展示尖端AI技术的奥秘。如果本文对你有帮助，请注册我创建的邮件列表，注册后可以收到更多后续的电子表格，帮助你入门机器学习和创建神经网络。
![](https://pic3.zhimg.com/v2-56d45fed2c63ac93f03fffc3484c02e2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='381'></svg>)
​

Facebook面部识别系统、某国奥威尔式的大规模监控系统、你的汽车（不久的将来）背后的基础都是计算机视觉。
![](https://pic2.zhimg.com/v2-808790fa461de1fec6ccf0d0f72d1409_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='601'></svg>)
​

**CNN神探**

让我们假装终结者的头脑里住着一个特别的侦探，名叫“夏洛克·卷积·福尔摩斯”。他的工作是仔细查看证据（输入图像），使用敏锐的眼神和推理能力（特征检测），预测图像中的人是谁，从而破案（正确分类图像）。
![](https://pic3.zhimg.com/v2-604d724589a95c46f7ffa06163417842_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='422'></svg>)
​

卷积网络架构

你将学习：
- 输入：计算机如何看
- 特征检测：像福尔摩斯一样思考
- 卷积数学：福尔摩斯的侦探工具
- ReLU：非线性模式识别
- 最大池化：保留最重要的线索
- 扁平化：排列所有线索
- 全连接：完成案件拼图
- Logit + Softmax：破案
- 交叉熵损失：福尔摩斯的“正/误”

​
![](https://pic2.zhimg.com/v2-5eaff91f8b33a6882c92331022b96339_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='123'></svg>)
​

**输入——一图胜千言**
![](https://pic1.zhimg.com/v2-5021cafd9385e0283a513cc14428217c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='284' height='284'></svg>)
替换高清大图




天网的最大威胁——Elon Musk

我从上面的图像中看到了一个有远见的人。他一方面不断改善地球这颗星球，另一方面创建逃离地球的火箭，以防终结者试图摧毁地球。和计算机不同，我看不到像素值，也无法辨认出一张图像不过是红光、绿光、蓝光的堆叠：
![](https://pic4.zhimg.com/v2-decb25c53a54be3b1ae2303b993b4287_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='217'></svg>)
​

另一方面，一台计算机（比如天网）是瞎的……它只能看到数字。

想象一下由3张电子表格（红、绿、蓝）堆叠构成的电子图像，每张表格是一个数字矩阵。拍照的时候，相机测量到达每个像素的红光、绿光、蓝光的量。接着它在0-255的范围内评估每个像素，然后将其记录到电子表格中。
![](https://pic2.zhimg.com/v2-67c94ae8958d1d41ec51b2821bac8005_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='445' height='688'></svg>)
​

计算机看到电子表格

上为一张28x28的图像，每个像素由三行（红、蓝、绿）表示，取值范围0-255. 每个像素的颜色对应其数值。
![](https://pic4.zhimg.com/v2-0726fbc57bc0746f02e52c6f8064ff3b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='325'></svg>)
​

终结者没有看到眼睛，他看到的是一串数字

将每种颜色单独放到一个矩阵中，我们得到了3个28x28矩阵，也就是我们之后用来训练神经网络的输入：
![](https://pic4.zhimg.com/v2-8b8d317f4b05f136517f580d7fd15113_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='356' height='505'></svg>)
​

如果你想知道如何将任意图像转换成电子表格，请访问以下网址：

[http://think-maths.co.uk/spreadsheet](https://link.zhihu.com/?target=http%3A//think-maths.co.uk/spreadsheet)

你将学到如何得到一张“Ex拍”，你的Excel迷朋友们会喜欢的……相信我，在电子表格中找到你的马克杯（或者他们的）会让他们捧腹大笑的🤳 （小图效果最佳）。

**训练概览——像计算机，又像小孩**

你刚出生的时候知道狗是什么样的吗？当然不知道。但随着时间的推移，你的父母会给你看书中、动画片中、真实生活中的狗的图像，渐渐地，你可以指着那些4条腿、毛茸茸的动物说“狗”。你的大脑中数亿神经元间的连接变得足够强，所以你可以识别狗。

终结者以同样的方式学习识别Elon。在一个被称为监督训练的过程中，人们给终结者看数千张Elon Musk、Jeff Bezos、Jon Snow的图像。刚开始，它只能随便乱猜（1/3的几率猜对），渐渐地，就像小孩一样，随着它在训练过程中看到越来越多的图像，它猜得越来越准。网络的连接（也就是“权重/偏置”）随着时间的推移而更新，使得它可以基于像素输入预测输出。这是我上一篇文章中讨论过的学习过程（梯度下降）。
![](https://pic4.zhimg.com/v2-8bb3465660d1d0a35c12b2e95b202a73_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='326'></svg>)
​

**卷积神经网络和普通神经网络的区别何在？**

用两个词概括：平移不变性。

不知所云？让我们解构一下：
- 平移 = 将某物从一个地方移动到另一个地方
- 不变性 = 没有改变

在计算机视觉中，这意味着，不管对象移动到图像中的何处（平移），不会改变对象是什么（不变性）。
![](https://pic2.zhimg.com/v2-67f99287e8614e2d88d807d081a53c81_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='241'></svg>)
​

平移不变性（以及缩放不变性）

这需要训练卷积神经网络识别Elon的特征，不管Elon在图像中的位置在哪里（平移），也不管Elon在图像中的大小（缩放不变性）。

在普通神经网络中，我们本来会将每个像素作为模型的一个输入（而不是3个矩阵），但这忽略了相近的像素具有特别的意义和结构这一事实。在CNN中，我们查看像素组，这允许模型学习形状、线条等局部模式。比方说，如果CNN看到许多白像素包围一个黑圆圈，它会识别出眼睛这一模式。

为了达到平移不变性，CNN需要依靠它的特征侦探夏洛克·卷积·福尔摩斯的服务。

类比： 图像就像由像素组成的电子表格。
![](https://pic2.zhimg.com/v2-44d8cdd2cf49c7e6aeb902b4032b4859_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='123'></svg>)
​

遇见夏洛克·卷积·福尔摩斯——特征侦探

​
*![](https://pic3.zhimg.com/v2-3d66619e39d410b14d2efbe62489c362_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='284' height='284'></svg>)*
寻找特征的夏洛克

夏洛克住在终结者的头脑中。他每次使用放大镜仔细检查一小片图像，寻找图像的重要特征（“线索”）。在收集到简单的线条和形状之类的线索后，他将它们堆叠起来，开始看到眼睛或鼻子之类的面部特征。

每个卷积层储存基于另一层构建的一组特征映射。最后，夏洛克组合所有线索，这样他就可以破案了（识别目标）。
![](https://pic2.zhimg.com/v2-210dfe52182ec759331795e3cbb2ea65_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='504'></svg>)
​

每个特征映射就像一条线索

网络的每个卷积层都包含一组特征映射，这些映射能够以下图所示的层次化的方式识别越来越复杂的模式/形状。

CNN基于数字的模式识别找到任意图像的最重要特征。随着CNN以更多的网络层不断堆叠这些模式，它可以创建非常复杂的特征映射。
![](https://pic3.zhimg.com/v2-38a3e820a7516d110e4d71d422368ef6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='518'></svg>)
真实生活中的CNN和夏洛克做一样的事情：
![](https://pic3.zhimg.com/80/v2-7eb6b002ab913da95caa6ea7b30752d2_b.jpg)谷歌AI 实验https://www.zhihu.com/video/1012413068621570048
CNN的奇妙之处在于可以自行学习这些特征……工程师不用编写寻找一双眼睛、一个鼻子、一张嘴的集合这样的代码。

以这种方式工作的工程师更像架构师，他们告诉夏洛克：“我给你两叠（‘卷积层’）空白特征映射（‘线索’），你的工作是分析图像，找出最重要的线索。第一叠有16个特征映射（‘线索’），第2叠有64个特征映射……现在发挥你的侦探技能，解决这个案件！”

类比： 每个特征映射就像案件中的一条线索。
![](https://pic3.zhimg.com/v2-258a6b9a50c4cc711b535f234ee6857a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='123'></svg>)
​

为了查明案件中的“线索”（即计算特征映射），夏洛克需要依靠他的侦探工具箱中的一些工具，我们会逐一介绍：
- 过滤器 —— 夏洛克的放大镜 🔍
- 卷积数学 —— 过滤器权重 x 输入图像像素
- 步进 —— 沿着输入图像移动过滤器 🔍 ➡️ 🔍 ➡️
- 补齐 —— 保护线索的犯罪现场隔离胶带 🚧




**夏洛克的放大镜/过滤器**

毫无疑问，夏洛克非常敏锐，具备出色的观察技能，但是，如果没有那些特制的放大镜（过滤器），夏洛克没法完成他的工作。他使用不同的放大镜帮助填充每张空白特征映射的细节。所以，如果他有16个特征映射……他会有16块放大镜。
![](https://pic4.zhimg.com/v2-716d0da0f73fd3ba99ea4bcb868c7bbf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='483'></svg>)
​

每块放大镜由多层玻璃组成，而每层玻璃由不同的权重组成。玻璃的层数，也就是过滤器深度，总是等于输入层的深度。

刚开始，夏洛克看到的输入图像有3层——红、绿、蓝。所以，放大镜也有3层。

随着我们进一步创建CNN，层的深度会增加，相应地，放大镜也会变厚。

为了创建1个特征映射（一条线索），夏洛克从取出一个放大镜，并置于输入图像的左上角开始。红层玻璃只能看到红输入图像，绿层玻璃只能看到绿图，而蓝层玻璃只能看到蓝图。

现在是数学部分。

**卷积数学**

特征映射中的每个像素是线索的一部分。为了计算每个像素，夏洛克需要进行一些基本的乘法和加法。

在下面的例子中，我们使用5x5x3的输入图像和3x3x3的过滤器，每个像素需要进行以下计算：
- 3x3x3过滤器每层的卷积乘法 = 27
- 将27个数字加起来
- 再加上1个数字——偏置
![](https://pic1.zhimg.com/80/v2-4709f7e732233acb5212e7fcc02d6a24_b.jpg)卷积神经组织 CNN人脸识别https://www.zhihu.com/video/1012413568746188800



让我们仔细看下数学。一个像素需要27次乘法（3层，每层9次乘法），下面的截图显示了27次乘法中的9次：
![](https://pic3.zhimg.com/v2-cc8627914a73869c5484bfe1bbb9f9ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='264'></svg>)
​

分素相乘

至于偏置，你可以把它想象成放大镜的把手。像权重一样，它是模型的另一个参数，在训练过程中自动调整，以提高模型的精确度，并更新特征映射细节。

过滤器权重——在上面的例子中，我为了简化数学，将权重的值设为-1、0、1；然而，一般而言，你需要用较小的值随机初始化权重……比如0.01到0.1之间的值，基于钟形曲线或正态分布取样。想要了解更多权重初始化的知识，可以看这篇入门。

**步进 —— 移动放大镜**

计算特征映射的第1个像素之后，夏洛克将把放大镜往哪移呢？
![](https://pic4.zhimg.com/v2-a18213d00131aa018c39b1eab97ed86f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='192' height='614'></svg>)
​

步进：每次移动放大镜1像素

答案取决于步进参数。作为架构师/工程师，我们需要告诉夏洛克，他应该将他的放大镜向右移动（步进）多少像素。实践中最常见的步长值为2或3，但出于简单性，这里我们将步长设为1. 这意味着夏洛克将放大镜向右移动1像素，然后进行和之前一样的卷积运算。

当他的放大镜到达输入图像的最右边时，他将放大镜移到最左，然后往下移动1像素。

**步长为何大于1？**

优点：更少运算，内存中储存的运算结果更少，从而使模型更快。

缺点：由于跳过像素有错过模式的潜在可能性，损失了关于图像的数据。

2或3步长通常是合理的，因为紧跟着一个像素的像素通常具有相似的值，而隔着2-3个像素的像素，更可能具有不同的值，这样的值对特征映射/模式而言可能很重要。

**如何预防信息损失（丢失线索）**

为了破案，夏洛克刚开始接触案件的时候需要大量线索。在我们上面的例子中，我们的输入为一张5x5x3的图像，或者75像素信息（75 = 5 x 5 x3），在第一个卷积层后，我们得到了一张3x3x2的图像，或者18像素（18 = 3 x 3 x 2）。这意味着我们损失了证据，这让夏洛克的搭档约翰·华生非常反感。
![](https://pic1.zhimg.com/v2-a8dc2dabe6a9f696f758c2375e0f9b58_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='440' height='248'></svg>)
​

在CNN的刚开始几层，夏洛克倾向于查看大量细微模式（更多线索）。在靠后的卷积层中，随着夏洛克堆叠细微的线索，查看较大的模式，“降采样”也就是降低像素的总量（更少线索）没什么问题。

那么，在CNN刚开始的时候，我们如何预防这样的信息损失呢？

**一、补齐——通过补齐图像保护犯罪现场**
![](https://pic4.zhimg.com/v2-e5e614b272cde7abd5b73cce8dedf9f7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='478'></svg>)
​

在上面的例子中，我们在撞上右边缘前，只能移动过滤器3次……从上往下同样如此。这意味着我们所得输出的高/宽为3x3，从左往右，损失了2像素，而从上往下又损失了2像素。

为了预防这种信息损失，常见的做法是用零“补齐”原始图像（称为全零补齐（zero padding或same padding））……有点类似用犯罪现场隔离胶带确保没人破坏证据。
![](https://pic3.zhimg.com/v2-8aea3defe7609a899c6999916b699a7a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='201' height='691'></svg>)
​

补齐之后，如果夏洛克再次使用相同的放大镜，他的两个特征映射的大小会是5x5而不是3x3.

这意味着我们最终得到了50像素的信息（5x5x2=50）。

50像素比18要好。不过别忘了……我们刚开始有75像素，所以我们仍然错过了一些线索。

所以我们还能做什么让夏洛克和约翰·华生满意？

**二、更多过滤器——至少在我们的卷积层中加上一个特征映射，给夏洛克更多线索**

模型对特征映射（“线索”）的数量并没有限制……这是我们可以控制的超参数。

如果我们至少将特征映射从2增加到3（5x5x2到5x5x3），那么总输出像素（75）就和输入像素（75）相等了。如果我们将映射增加到10，那么我们会有更多信息供夏洛克探究（5x5x10 = 250）。
![](https://pic2.zhimg.com/v2-c2cd6b81126274be958fb0acf0c7d301_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='498' height='249'></svg>)
​

事情变得越来越有趣了

总结一下，刚开始几层的信息像素总数一般高于输入图像，因为我们想要给夏洛克尽可能多的细微模式/线索。在网络的最后几层，我们常常进行降采样，信息像素变少，这是因为这些层识别图像中较大的模式。

类比： 过滤器就像放大镜，而补齐就像犯罪现场隔离胶带。
![](https://pic4.zhimg.com/v2-b33cb9adb09e78ff584bc6de1ab2a4ef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='123'></svg>)
​

**非线性模式识别——ReLU**

给夏洛克足够的案件信息很重要，但现在到了进行真正的侦探工作的时候了——非线性模式识别！比如耳廓和鼻洞。

到目前为止，夏洛克进行了大量数学运算以构建特征映射，但所有运算都是线性的（在每个输入像素上进行一些乘法和加法操作），因此，他只能识别像素的线性模式。

为了给CNN引入非线性，我们将使用一种称为修正线性单元（Rectified Linear Unit）的激活函数，简称ReLU。在我们初次进行卷积运算得出特征映射后，每个值都通过这一函数，看看是否点亮/激活。

如果输入值是负数，那么输出将为零。如果输入值是正数，那么输出将和输入一样。ReLU就像一个开关，让特征映射的每个值通过ReLU之后，就创建了非线性模式识别。

回到我们原本的CNN例子，我们在卷积之后马上应用ReLU：
![](https://pic4.zhimg.com/v2-1b4a3d61c30c7b19e84b6d4756679557_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='344'></svg>)
​

尽管有许多非线性激活函数可以为神经网络引入非线性（Sigmoid、Tanh、Leaky ReLU等），ReLU是CNN中目前最流行的激活函数，因为ReLU在算力上很高效，能加快训练。你可以参阅Andrej Karpathy的overview on non-linear activation functions了解每种函数的优劣（译者注：也可以参考理解神经网络的激活函数，同样比较了不同激活函数的优劣）。

**类比： ReLU就像开关。**
![](https://pic4.zhimg.com/v2-6df3d10b853275a6d9954ec6796f0077_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='123'></svg>)
​

**最大池化——在大脑阁楼中保留关键的少量信息**

现在夏洛克有一些特征映射（“线索”）要查看，如何确定哪些信息是关键的，哪些信息是无关的细节？最大池化。

夏洛克认为人类的大脑就像一个空阁楼。傻瓜会在里面存放各种各样的家具和物品，让有用的信息在一堆杂物中不知所踪。而智者仅仅储存最重要的信息，从而在需要的时候可以快速做出决定。从这个意义上说，最大池化是夏洛克版的大脑阁楼。为了更快地做出决定，他只保留最重要的信息。
![](https://pic1.zhimg.com/v2-f530680c4bfd44e59d87f5d4f764f10c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='359'></svg>)
​

通过最大池化，夏洛克在相邻的像素中仅仅保留最大值，证据中最重要的部分。

例如，如果他查看一个2x2区域（4像素），他仅仅保留其中值最高的像素，丢弃其余3个像素。这一技术让他可以快速地学习，同时也有助于他概括（而不是“记忆”）可用于未来图像的线索。

和之前的放大镜过滤器类似，我们同样可以控制最大池化的步长和大小。在下面的例子中，我们将步长设为1，池化尺寸设为2x2：
![](https://pic2.zhimg.com/v2-7279919dd1e9776e997a42fe5677eafd_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='624' height='480'></svg>)
​

最大池化之后，我们完成了1回合卷积/ReLU/最大池化。

典型的CNN在分类器之前会有若干回合卷积/ReLU/池化。在每一回合中，我们将在增加深度的同时挤压高/宽，这样我们不会在此过程中遗失证据。

第一步至第五步，我们专注于收集证据，现在是时候让夏洛克查看所有线索，侦破案件了：
![](https://pic3.zhimg.com/v2-2e12011e94bcc94691fd074ebb9d5dca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='422'></svg>)
​

现在我们已经有了证据，让我们开始挖掘证据的意义……

类比： 最大池化就像夏洛克的阁楼理论，保留关键信息，抛弃驳杂无用的信息。
![](https://pic3.zhimg.com/v2-d56ee4b882b0d167489ae1d4b4707d16_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='123'></svg>)
​

在训练周期的末尾，夏洛克得到了堆积如山的线索，需要找到一下子查看所有线索的方式。每条线索不过是一个2维矩阵，但我们有堆叠在一起的数以千计的矩阵。

作为一名私家侦探，夏洛克很擅长应付这样的混沌，但他需要将证据呈上法庭，整理证据以供陪审团查看。
![](https://pic1.zhimg.com/v2-4de3af948ccb3407d2fda73fcfba1754_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)
​

扁平化之前的特征映射

他通过一种简单的转换技术（称为扁平化）做到了这一点：
- 将每个由像素组成的2维矩阵转换为一列像素
- 将原本的二维矩阵（现在的一列像素）一个接一个地排列起来

在人类的眼睛看来，变换是这样的：
![](https://pic1.zhimg.com/v2-486ff4f2613207db97f1b525ca0ae1e0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='852'></svg>)
​

而在计算机看来，是这样的：
![](https://pic2.zhimg.com/v2-e644ee10ef8b3c207140be4530bb5c65_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='513' height='477'></svg>)
​

现在夏洛克已经整理好证据了，是时候说服陪审团证据清楚地指向一个嫌疑人。

**类比：扁平化就像把证据呈上法庭。**
![](https://pic1.zhimg.com/v2-053a7f13a437803388306081fee8e600_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='123'></svg>)
​

在全连接层中，我们将方方面面的证据连接起来。某种意义上，我们完成案件的拼图，向评审团表明证据和每个嫌疑人之间的联系：
![](https://pic2.zhimg.com/v2-323da60ce7b73824a98e7e22d716e7cd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='532'></svg>)
​

在计算机看来，全连接层是这样的：

​

在扁平层的每份证据和3个输出之前是一组权重和偏置。类似网络中的其他权重，这些值会在刚开始训练CNN的时候随机初始化，而随着时间的推移，CNN将“学习”如何调整这些权重/偏置以得到更精确的预测。

现在，到了夏洛克揭晓谜底的时间了！

类比：全连接层就像说服陪审团下决定。
![](https://pic4.zhimg.com/v2-6359a631986e5509d5e3ee0239cedaab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='123'></svg>)
​

在CNN的图像分类器阶段，模型的预测为得分最高的输出。模型的目标是让正确的输出得分最高，而让错误的输出得分较低。

评分分为两部分：
- Logit分数——原始得分
- Softmax——每个输出的概率（0-1之间）。所有输出得分之和等于1.




**1. Logit——逻辑得分**

每个输出的Logit得分是一个基本的线性函数：

Logit得分 = (证据 x 权重) + 偏置

每片证据乘以连接证据至输出的权重。所有乘积相加，最后加上偏置项，得分最高的为模型的猜测。
![](https://pic2.zhimg.com/v2-9c82fc7e9f6639ebc98031a32e0495a5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='293'></svg>)
​

所以，为什么不到此为止？直观地说，有两个原因：
- **夏洛克的置信度**——我们想要知道夏洛克对结果有多自信，这样，当夏洛克的置信度很高同时他是正确的时候，我们会奖励他，而在夏洛克的置信度很高同时他是错误的时候，我们会惩罚他。我们在下一部分计算损失（“夏洛克的精确度”）时会具体讨论奖励/惩罚。
- **夏洛克的置信度加权概率**——我们想要能够方便地解释这些0到1之间的概率，并且我们想要预测得分和实际输出（0或1）处于同一尺度。实际正确的结果（Elon）的概率为1，其他错误的结果（Jeff和Jon）的概率为0. 将正确输出转为一，错误输出转为零的过程称为独热编码。

夏洛克的目标是让他对正确输出的预测尽可能接近1.

**2. Softmax——夏洛克的置信度加权概率得分**

**2.1 夏洛克的置信度**

为了得出夏洛克的置信度，我们以e（等于2.71828…）为底数，以logit得分为指数。这样，一个较高的得分将变为非常高的置信度，而一个较低的得分将变为非常低的置信度。

这番指数运算同时也确保了我们不会有任何负分（logit得分“可能”是负数）。
![](https://pic3.zhimg.com/v2-c89ac87bbb3160fb918be1a58ec8f416_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='313'></svg>)
​

**2.2 夏洛克置信度加权概率**

为了得出置信度加权概率，我们将每个输出置信度除以所有置信度得分之和，这就确保了所有概率之和为1.
![](https://pic4.zhimg.com/v2-af3a7cc2d0e176ad8c2e0a745521b0ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='536'></svg>)
​

softmax分类器很直观。夏洛克认为有97%（置信度加权）的几率终结者查看的图像是Elon Musk.

模型的最后一步是计算损失。损失告诉我们侦探夏洛克到底有多棒（或者到底有多糟）。

类比：Logit + Softmax就像查看一排嫌疑人，并指出罪犯
![](https://pic2.zhimg.com/v2-851c0ff65de8f7a2784d468dd60d6505_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='123'></svg>)
​

所有神经网络都有一个损失函数，用来比较预测和实际值。在CNN训练的过程中，随着网络权重/偏置的调整，预测改进了（夏洛克的侦探技能变得更厉害了）。

CNN最常用的损失函数是交叉熵。用Google搜索交叉熵出现的一些解释都涉及大量希腊字母，很容易让人困惑。尽管这些描述各不相同，在机器学习的语境下它们都意味着一样东西，我们下面将介绍最常见的3种解释，便于你理解。

在描述每种公式变体之前，先概括一下它们的共同点：
- 比较正确分类的概率（Elon，1.00）和CNN的预测（Elon的softmax得分，0.97）。
- 当夏洛克对正确分类的预测接近1时（低损失），奖励他👏
- 当夏洛克对正确分类的预测接近0时（高损失），惩罚他👎

​
![](https://pic3.zhimg.com/v2-5637948c80ffeff27d8c4d295f31067e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='313'></svg>)
​

结果是一样的！3种不同的解释……

**1号解释——衡量实际概率和预测概率的距离**

距离捕捉了以下直觉：如果对正确标签的预测接近1，那么损失接近0. 如果对正确标签的预测接近0，那么将受到严厉的惩罚。目标是最小化正确分类的预测（Elon，0.97）和正确分类的实际概率（1.00）的距离。

赏罚“对数”公式背后的直觉将在2号解释中讨论。
![](https://pic3.zhimg.com/v2-baae3ba43e521f4ada740be13d9c5c3e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='264'></svg>)
​

**2号解释——最大化log似然或最小化负log似然**

在CNN中，“对数”实际上指“自然对数（ln）”，它是softmax中以自然对数为底的指数的逆运算。

对夏洛克远离1.00的预测而言，相比直接从实际概率（1.00）中减去预测概率（0.97）来计算损失，对数运算将惩罚以指数级别加重了。

最小化负对数似然和最大化对数似然均导向softmax预测趋向1.0，损失趋向0.0的结果。
![](https://pic1.zhimg.com/v2-517cc10e8c805bc60125560bdec19900_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='924' height='308'></svg>)
​
![](https://pic1.zhimg.com/v2-42f036568d8189b2027498c2db146e74_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='829' height='252'></svg>)
​

**3号解释——KL散度**

KL（Kullback-Leibler）散度衡量预测概率（softmax得分）和实际概率的差异。

KL散度的公式包括两部分：
- 实际概率的不确定性。
- 使用预测概率损失的信息量。

在监督学习分类问题的模型训练中，实际概率的不确定性总是等于0. 我们百分之百确定训练图像的分类无误。此时，最小化预测概率分布和实际概率分布之间的KL散度等价于最小化交叉熵。
![](https://pic3.zhimg.com/v2-1649ea5cc82f49fdb945b071a07112c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='266'></svg>)
​

**类比：交叉熵损失就像评估夏洛克的侦探技能。**

**总结**

在夏洛克·卷积·福尔摩斯这位特别的侦探的帮助下，终结者有了一对能够搜索并摧毁自由世界的保护者Elon Musk的眼睛（对不住了，Elon！）。

虽然我们仅仅训练了终结者识别Elon、Jeff、Jon……天网有无穷多的资源，可以随意训练图像，因此天网可以利用我们创建的模型训练终结者识别任何人类和物体。

**订阅邮件列表和分享**

如果你喜欢本文，并想收到更多通过Excel进行机器学习的内容，可以访问[http://excelwithml.com](https://link.zhihu.com/?target=http%3A//excelwithml.com)免费订阅邮件列表。

你也可以在Twitter上关注ExcelwithML。

更多可交互资源
- 画一个数字，查看CNN如何预测它：[http://scs.ryerson.ca/~aharley/vis/conv/flat.html](https://link.zhihu.com/?target=http%3A//scs.ryerson.ca/~aharley/vis/conv/flat.html)
- 用Google和你的摄像头训练自己的CNN: [https://experiments.withgoogle.com/teachable-machine](https://link.zhihu.com/?target=https%3A//experiments.withgoogle.com/teachable-machine)
- Andreij Karpathy的实时图像分类模型：[https://cs.stanford.edu/people/karpathy/convnetjs/demo/cifar10.html](https://link.zhihu.com/?target=https%3A//cs.stanford.edu/people/karpathy/convnetjs/demo/cifar10.html)
- Fast.AI讲解CNN的视频（不可交互，不过是一个很好的讲座）：[https://www.youtube.com/watch?time_continue=2548&v=9C06ZPF8Uuc](https://link.zhihu.com/?target=https%3A//www.youtube.com/watch%3Ftime_continue%3D2548%26v%3D9C06ZPF8Uuc)

在未来与机器的战争中，我们的命运就在你的手中😜

原文地址：[https://towardsdatascience.com/cutting-edge-face-recognition-is-complicated-these-spreadsheets-make-it-easier-e7864dbf0e1a](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/cutting-edge-face-recognition-is-complicated-these-spreadsheets-make-it-easier-e7864dbf0e1a)




想要获取更多知识信息，点击下方
[机器学习集训营 第六期「线上线下结合，线下在北上深广杭沈济郑成武西十一城」- 七月在线​www.julyedu.com![图标](https://pic2.zhimg.com/v2-53d60b87925804ace7cff4d480694769_180x120.jpg)](https://link.zhihu.com/?target=http%3A//www.julyedu.com/weekend/train6)[深度学习 第四期 [加送数学视频，且提供CPU和GPU双云平台，参团立减100]​www.julyedu.com![图标](https://pic4.zhimg.com/v2-beae58bf3f58f26863e792069479323f_180x120.jpg)](https://link.zhihu.com/?target=http%3A//www.julyedu.com/course/getDetail/112)[AI就业班 二期「BAT大咖一对一定向辅导」 - 七月在线​www.julyedu.com](https://link.zhihu.com/?target=https%3A//www.julyedu.com/weekend/jiuye2)



