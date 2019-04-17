# 十个生成模型(GANs)的最佳案例和原理｜代码+论文 - 知乎
# 



> 王小新 编译
原文作者：Sumeet Agrawal
量子位 出品 | 公众号 QbitAI

生成对抗网络（GANs）是一种能“教会”计算机胜任人类工作的有趣方法。一个好的对手能让你成长更快，而GANs背后就是“从竞争中学习”的思路。

GANs最先是由蒙特利尔大学的Ian Goodfellow提出，已在图像生成和风格迁移等领域获得了巨大的成功，充分展示了“无监督学习”技术的潜力。

## GANs是如何工作的？
![](https://pic3.zhimg.com/v2-0db41673f9a0cd3642f7f20a854de636_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='638' height='359'></svg>)
**△** GAN结构示意图

一个GAN中包含两个相互竞争的神经网络模型。一个网络称为生成器（generator），能将噪声作为输入并生成样本；另一个网络称为鉴别器（discriminator），能接收生成器数据和真实训练数据，训练得到能正确区分数据类型的分类器。

这两个网络互相对抗。其中，生成器通过学习来产生更逼近真实数据的新样本，用于愚弄鉴别器，反之鉴别器也需要更好地区分生成数据与真实数据。

将这两个网络联合训练，经过大量回合“交锋”后，鉴别器将无法区分生成样本和实际数据。做个比喻，生成器就像是想制作一些高仿品的生产商，而鉴别器就像是试图检测出这些仿制品的警察。

由于整个过程是自动完成的，仅受限于实际的计算能力，因此GANs可用来实现一些很有趣的功能。

以下是一些用GANs完成的酷炫应用。

## 1、让机器拥有想象力

神经网络对于物体的本质是怎么“想”的？

为了搞懂这个问题，谷歌大脑的研究人员使用GAN，让神经网络的“想法”呈现在你的眼前。当然，这些“想法”看起来十分迷幻。

其实这原本是一个图像分类器，而生成的迷幻图片，是故意对图像进行过度处理的副产品。现在这套系统有个单独的名字：“深梦（Deep Dream）”。
![](https://pic1.zhimg.com/v2-b3f428ad149513af0b4af3fd05994b20_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='216'></svg>)
**△** Deep Dream模型利用普通照片生成的一张奇幻照片
![](https://pic1.zhimg.com/v2-e402befdb770ef26c83781955eac8408_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='534'></svg>)
**△** Deep Dream模型将塔楼、房屋和小鸟等对象融入图像中的效果示例

想要运行Deep Dream模型，你只需要输入一张图像，然后这个模型就开始穷尽所能，寻找被训练识别的目标。在一张完全不相关的图像中，神经网络可能会发现一些与狗、房子、水母等物体的相似之处。

就好像你看到云朵会产生联想一样，Deep Dream模型会放大它找到的相似之处。

举个例子，当你运行这个辨识网络时，它指出某张图是狗的可能性为40%，然后开始修改输入图像，使得这张图为狗的可能性增加到60%。并不断重复这个过程，直到输入图像明显地转化成一张看起来像狗的图像。

按照这种方式，通过将图像逐渐转化为的另一种物体，这个神经网络就把自己的“想象”，展现在你的面前。

谷歌提出的Deep Dream模型将传统的思路（即给定相同输入只产生一种输出的想法），改变为不断修改输入来获取最佳输出。

更多信息

博客：

[http://googleresearch.blogspot.ch/2015/06/inceptionism-going-deeper-into-neural.html](https://link.zhihu.com/?target=http%3A//googleresearch.blogspot.ch/2015/06/inceptionism-going-deeper-into-neural.html)

代码：

[https://github.com/google/deepdream](https://link.zhihu.com/?target=https%3A//github.com/google/deepdream)

## 2、能模仿人的智能体

这是通过GANs实现模仿学习。不同于传统的奖励机制，某些AI研究人员希望针对自主学习智能体，提出一种全新的方法。

他们将实际的示范数据输入到智能体，然后智能体从中学习并尝试模仿相同的动作。
![](https://pic4.zhimg.com/v2-8cc1a7226600ba0fad43c2719dc51193_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='320'></svg>)
**△** 一个小人试图模仿人类来自学跑步

在这个模型中，Jonathan Ho和Stefano Ermon提出了一种模仿学习的新方法。一般来说，强化学习通常需要设计一个衡量智能体运动行为的奖励机制。但是，在实际中，可能要经历成本昂贵的试错过程，才能得到合理的前进表现。

然而，在模仿学习中，智能体从示范数据（机器人的远程操作或是人类行为）中学习，消除了设计奖励机制的要求。关于强化学习在这方面的最新研究可移步量子位编译过的《DeepMind智能体自学跑酷：略显智障，结果尚好》（可在微信公众号内查看）。

更多信息

博客：

[https://blog.openai.com/generative-models/](https://link.zhihu.com/?target=https%3A//blog.openai.com/generative-models/)

代码：

[https://github.com/openai/imitation](https://link.zhihu.com/?target=https%3A//github.com/openai/imitation)

## 3、指马为斑马

用图像生成图像，这也是生成网络的一个有趣应用。在实验中，研究人员能够更改视频中的动物种类、图片中的季节类型或是其他类似任务。

这项任务的目标是通过一系列配对图像的训练集，学习从输入图像到输出图像的映射关系。然而，在大多数情况下，不容易获得所需的配对训练数据。

为了克服这个问题，引入了两个逆映射，使得每个映射的输出与另一个映射的输入相同，从而可利用少量数据来建立一个图像间尽可能完整的映射关系（即无监督学习）。

关于这方面工作的详细解读可移步量子位编译过的“带你理解CycleGAN，并用TensorFlow轻松实现”。

以下是这项研究工作的一些实例。



![](https://pic4.zhimg.com/v2-c1e4eb499bd611af33cf67b0ab2b5183_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='448' height='128'></svg>)
**△** 通过检测视频中正在移动中的野马并把斑马条纹叠加上去，可将野马转换成斑马。
![](https://pic4.zhimg.com/v2-1b49c91c2e4a9d2e7890d969b8b48a6f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='254'></svg>)
**△** 改变图像中的季节，如把优胜美地的冬天变成夏天，把冬天变成春天，反之亦可。

更多信息

首页：

[https://junyanz.github.io/CycleGAN/](https://link.zhihu.com/?target=https%3A//junyanz.github.io/CycleGAN/)

代码：

[https://github.com/junyanz/pytorch-CycleGAN-and-pix2pix](https://link.zhihu.com/?target=https%3A//github.com/junyanz/pytorch-CycleGAN-and-pix2pix)

## 4、给草图上色

这项任务可根据物体轮廓生成逼真图像。在现实中，图像处理是一项具有挑战性的工作，因为它需要以一种用户可控的方式来修改图像外观，同时保留结果的逼真程度。这需要一定的技巧，一个艺术家可能需要苦练多年才能达到这一高度。
![](https://pic4.zhimg.com/v2-df67d31072aa9d22c1d5df76342f050f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='540' height='360'></svg>)
**△** GANs可利用物体轮廓生成逼真图像

研究人员构建了一个模型，在给定物体轮廓后，它能够识别这个物体并生成该物体的逼真图像。

然而，在伯克利大学的一篇论文中，作者Jun-Yan Zhu提出了一种利用生成对抗网络直接从数据中学习自然图像流形的方法。该模型能自动调整输出以保持所有编辑尽可能地逼真，以及所有操作都是用约束优化来表示的，处理过程近乎实时。

所提出的这种方法可进一步用于图像生成研究，或是基于个人涂鸦完整地产生新奇图像的研究中。

更多信息

论文：

[https://arxiv.org/pdf/1609.03552v2.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1609.03552v2.pdf)

代码：

[https://github.com/junyanz/iGAN](https://link.zhihu.com/?target=https%3A//github.com/junyanz/iGAN)

首页：

[https://people.eecs.berkeley.edu/~junyanz/projects/gvm/](https://link.zhihu.com/?target=https%3A//people.eecs.berkeley.edu/~junyanz/projects/gvm/)

## 5、仅根据文本描述来生成图像

能否仅根据文本来自动生成逼真图像？这听起来就十分有趣。

最近，研究人员提出了一种深度卷积生成对抗网络（Deep convolutional generative adversarial networks），可以产生一些高度逼真的图像，但是目前仅针对特定类别，如人脸、唱片封面和房间内饰。

该模型的训练数据为一些文本描述及其对应图像的样本集。当给出任何物体描述时，这个模型会尝试根据描述来生成图像。
![](https://pic3.zhimg.com/v2-6cc08077de45f2cf9c632c4b14d3ef3a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='353'></svg>)
**△** 该模型能根据详细的文字描述来生成似乎合理的花鸟图像。

在这项工作中，所构建的模型首先获取关于图像合成的文本描述，其次学习捕捉文本中描述重要视觉信息的特征，然后使用这些特征来合成出能愚弄人的逼真图像。

更多信息

论文：

[https://arxiv.org/pdf/1605.05396.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1605.05396.pdf)

代码：

[https://github.com/paarthneekhara/text-to-image](https://link.zhihu.com/?target=https%3A//github.com/paarthneekhara/text-to-image)

## 6、通过“好奇心”驱动学习

“好奇心”机制推动了对深层神经网络的探索。在许多现实场景中，对操作者的外部奖励是非常稀疏的或几乎不存在的。因此，由于这种内在特性，操作者处于被动状态，很难进行进化和学习。

在这种情况下，好奇心可以作为一种内在的奖励信号，让操作者去探索未知的新环境，学习一些在以后生活中可能有用的技能。在这些情况下，主动学习者的效果远远好于被动学习者。

在这个模型中，我们将操作者预测自身行为可能导致后果的能力，即预测值和真实值之间的误差称为好奇心（Curiosity）。当然，在此空间中，机器人也可通过程序员建立的奖励机制来学习。

我们可将这个模型类比为小孩的认知过程。如果父母不告诉这个小孩，他并不知道他碰到一个热火炉后会发生什么，但是一旦他这么做了，他就学会下次不这么做，因为触摸到炉子后，他会感到十分疼痛，这就存在前后的因果关系。

“好奇心”机制推动他去探索，同时奖励机制会标记这个行动的好坏。
![](https://pic2.zhimg.com/v2-0423e5a5c2d29bc3d7286b2640f22b29_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='320'></svg>)
**△** 在这个游戏中，小蛇正在学习收集能增加奖励的绿球，同时避开能减少奖励的红球，这些是通过“好奇心”机制实现的。

“好奇心”驱动学习有如下三个大概的研究背景：

1).极少的外部奖励，与外部环境的相互作用远远少于期望值；

2).不输入外部奖励,好奇心促使操作者的探索更有效；

3).推广到未接触过的场景（例如同一游戏的新关卡），从早期经历获得知识的操作者比从头开始探索新地点的操作者学习要快得多。

我们在两个游戏环境中评估所提出的方法：毁灭战士（VizDoom）和超级马里奥兄弟（Super Mario Bros）。

更多信息

论文：

[https://pathak22.github.io/noreward-rl/resources/icml17.pdf](https://link.zhihu.com/?target=https%3A//pathak22.github.io/noreward-rl/resources/icml17.pdf)

代码：

[https://github.com/pathak22/noreward-rl](https://link.zhihu.com/?target=https%3A//github.com/pathak22/noreward-rl)

首页：

[https://pathak22.github.io/noreward-rl/index.html#demoVideo](https://link.zhihu.com/?target=https%3A//pathak22.github.io/noreward-rl/index.html%23demoVideo)

## 7、AI设计游戏

这个研究希望使用GANs来设计游戏的用户界面。研究者的思路是，如果能生成一些有说服力的虚拟视频游戏截图，我们可以从中得到一些启发，并将部分图像片材用于设计新游戏。
![](https://pic3.zhimg.com/v2-041c392f76ade274b08f1ce167608aa2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='320'></svg>)
**△** 真实的游戏背景图像作为输入
![](https://pic2.zhimg.com/v2-a0ecf504d91b5abb54aec0964507fbe9_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='400'></svg>)
**△** 随着GANs的训练，生成的背景图像变得越来越好
![](https://pic1.zhimg.com/v2-6d36763cae47cb1ab3fb15697822b344_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='320'></svg>)
**△** 最后得到一些可能对新游戏有启发的场景

此程序的目标是生成一些类似的游戏素材。为了实现这一点，该程序会从现有的各种游戏中收集出一堆图像。然后，程序根据这些打乱的图像小块来生成独特的新图像。这些图像可以被用作设计新游戏的背景。

更多信息

博客：

[https://medium.com/@ageitgey/abusing-generative-adversarial-networks-to-make-8-bit-pixel-art-e45d9b96cee7](https://link.zhihu.com/?target=https%3A//medium.com/%40ageitgey/abusing-generative-adversarial-networks-to-make-8-bit-pixel-art-e45d9b96cee7)

## 8、预测视频中可能的发展动向

这个研究是通过场景动力学（Scene Dynamics）来生成视频。

了解物体运动和场景动力学是计算机视觉中的一个核心问题。对于视频识别任务（如动作分类）和视频生成任务（如未来预测），需要构建一个能理解如何进行转换场景的模型。然而，创建动态模型十分具有挑战性，因为物体和场景的变化方式有很多种。
![](https://pic3.zhimg.com/v2-60abedae79d4f293f057e602114e9082_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)
**△** 对火车站、海滩、婴儿和高尔夫球场景下的视频进行未来预测

这是利用一个能从背景中分离前景的模型实现的，它通过强制背景变成静止的，并且该网络能学习到哪些物体正在移动以及哪些处于静止。

这些视频不是真实的，而是通过视频生成模型“想象”得来的。虽然看起来不够逼真，但根据训练时所使用的场景类别，这些生成动作是合理的。

量子位之前也有过类似报道。

更多信息

论文：

[http://carlvondrick.com/tinyvideo/paper.pdf](https://link.zhihu.com/?target=http%3A//carlvondrick.com/tinyvideo/paper.pdf)

代码：

[https://github.com/cvondrick/videogan](https://link.zhihu.com/?target=https%3A//github.com/cvondrick/videogan)

首页：

[http://carlvondrick.com/tinyvideo/](https://link.zhihu.com/?target=http%3A//carlvondrick.com/tinyvideo/)

## 9、生成逼真的假人脸

这个项目叫做“神经人脸（Neural Face）”，是一种能生成逼真脸部图像的人工智能技术。这个项目参考了Facebook公司AI研究团队开发的深度卷积生成对抗网络（DCGAN），这种方法也应用在上面提到的“仅根据文本描述来生成图像”。
![](https://pic1.zhimg.com/v2-4afa81095b36710b61f036025bdc3fd8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='600'></svg>)
**△** 由GANs生成的独特人脸图

该AI团队使用一个向量Z表示每张图像，其中该向量Z由100个[0,1]区间的实数组成。

生成器计算得到脸部图像的分布后，利用高斯分布从向量Z中生成图像。生成器不断学习来生成新的脸部图像，以愚弄鉴别器；反过来，鉴别器在竞争过程中可以更好地区分生成脸部图像和真实脸部图像。

更多信息

代码：

[https://github.com/carpedm20/DCGAN-tensorflow](https://link.zhihu.com/?target=https%3A//github.com/carpedm20/DCGAN-tensorflow)

首页：

[http://carpedm20.github.io/faces/](https://link.zhihu.com/?target=http%3A//carpedm20.github.io/faces/)

## 10、改变照片中的面部表情和容貌

这个项目是使用GANs对面部图像进行向量运算。在实验中，研究人员通过向系统输入一系列样本图像，来生成具有多种面部表情的人脸图像。例如，它可以将一张非微笑的脸变为微笑的脸、在脸上添加一个物体（如眼镜），或突出某些特征。
![](https://pic1.zhimg.com/v2-d12eaec1f77ae6061050f01aa021df1c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='703'></svg>)
**△** 使用算术运算，我们可以将非微笑的人脸图像转换为微笑的人脸图像，或者向一张没有戴眼镜的人脸上添加眼镜。

这种方法的基本原理是，把图像的每一列都表示成一个向量X，把所有X平均后得到均值向量Y。

然后对向量Y执行诸如加法和减法的算术运算，来创建单张图像，即向量Z是通过戴眼镜的男人减去不戴眼镜的男人，再加上不戴眼镜的女人得到的。接着，这个图形向量Z被传输到生成器中，以生成上图中右侧所示的效果，分别是微笑的男人和戴着眼镜的女人。
![](https://pic3.zhimg.com/v2-c5f4543e6372c1943a6e97dd63103d32_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='363'></svg>)
**△** 将左侧脸部图分步转换成右侧脸部图

我们也可模拟人脸的旋转、缩放和移位等变换，得到逼真的效果。为了实现这个，我们先拍摄了一系列左侧脸部图和右侧脸部图的图像样本。接着，平均所有图像得到一个转动图像向量（Turn Image Vector）。然后，我们通过沿着图像向量的轴线进行插值，将这种“变换”应用于新面孔中。

更多信息

论文：

[https://arxiv.org/pdf/1511.06434.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1511.06434.pdf)

代码：

[https://github.com/Newmu/dcgan_code](https://link.zhihu.com/?target=https%3A//github.com/Newmu/dcgan_code)

博客：

[https://hackernoon.com/how-do-gans-intuitively-work-2dda07f247a1](https://link.zhihu.com/?target=https%3A//hackernoon.com/how-do-gans-intuitively-work-2dda07f247a1)

## 结论

GANs的提出对人工智能和机器学习的发展有着十分深远的意义。但是上面提到的这些还只是GANs的早期研究成果。上述的十个例子，看起来十分酷炫，但是仍然只涉及到这个网络理论的一些皮毛。

作为一名工程师，我们应该意识到，它是一种强大的方法，可训练出能完成任何复杂人类任务的神经网络。GANs用实际性能证明，创造力不再是人类独有的特质。

## 学习资源

如果你想深入学习有关生成模型和DCGAN的知识，这里列出一些资源。

1．50行代码实现生成对抗网络：

[https://medium.com/@devnag/generative-adversarial-networks-gans-in-50-lines-of-code-pytorch-e81b79659e3f](https://link.zhihu.com/?target=https%3A//medium.com/%40devnag/generative-adversarial-networks-gans-in-50-lines-of-code-pytorch-e81b79659e3f)

2．GANs教程（Ian Goodfellow，NIPS 2016）:

[https://channel9.msdn.com/Events/Neural-Information-Processing-Systems-Conference/Neural-Information-Processing-Systems-Conference-NIPS-2016/Generative-Adversarial-Networks](https://link.zhihu.com/?target=https%3A//channel9.msdn.com/Events/Neural-Information-Processing-Systems-Conference/Neural-Information-Processing-Systems-Conference-NIPS-2016/Generative-Adversarial-Networks)

3．GANs入门讲解（5分钟短视频）：

[https://www.youtube.com/watch?v=deyOX6Mt_As](https://link.zhihu.com/?target=https%3A//www.youtube.com/watch%3Fv%3DdeyOX6Mt_As)

—— **完** ——

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)

վ'ᴗ' ի 追踪AI技术和产品新动态


