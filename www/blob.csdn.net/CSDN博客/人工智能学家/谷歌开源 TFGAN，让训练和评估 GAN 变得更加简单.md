# 谷歌开源 TFGAN，让训练和评估 GAN 变得更加简单 - 人工智能学家 - CSDN博客
2017年12月14日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：507
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBUeT7Mfa98yWQ99p7gLR6DSQ6dG4FmficUziaZyFhe8mGs1uObFoicBe3cXqD982gU2ib6XQzsejPKRiaQ/0?wx_fmt=jpeg)
*作**者：思颖*
*概要：训练神经网络的时候，通常需要定义一个损失函数来告诉网络它离目标还有多远。*
三年前，蒙特利尔大学 Ian Goodfellow 等学者提出「生成式对抗网络」（Generative Adversarial Networks，GANs）的概念，并逐渐引起 AI 业内人士的注意。自 2016 年以来，学界、业界对 GAN 的兴趣出现「井喷」。近日，谷歌开源 TFGAN 轻量级的工具库，据悉，其设计初衷是让训练和评估 GAN 变得更加简单。
训练神经网络的时候，通常需要定义一个损失函数来告诉网络它离目标还有多远。例如，图像分类网络中通常会有一个损失函数，一旦给出错误的分类就会进行惩罚。如果一个网络把狗的照片错当成猫了，那将会出现很高的损失值。
然而，有些问题并不能轻松用损失函数来定义，特别是当它们涉及到人的感知时，比如图像压缩或文本转语音系统。
生成式对抗网络 (GAN) 这种机器学习技术已经在广泛的应用领域引导我们取得了进步，包括基于文本生成图像、超分辨率，以及机器人抓取的解决方案。然而，GAN 在理论和软件工程上都引入了新的挑战，想要跟上它这种快速的研究步伐也很难。
为了让基于 GAN 的实验更加容易，谷歌选择开源 TFGAN，这是一个轻量级的库，设计初衷是让训练和评估 GAN 变得简单。
它提供训练 GAN 的基础设施，也提供经过良好测试的损失和评估指标，并包括易于使用的示例，可以看到这个库极具表达性和灵活性。与此同时，谷歌还发布了一个教程，其中包括能快速基于自己的数据进行模型训练的高级 API。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBUeT7Mfa98yWQ99p7gLR6DSQ6dG4FmficUziaZyFhe8mGs1uObFoicBe3cXqD982gU2ib6XQzsejPKRiaQ/0?wx_fmt=jpeg)
上图展示了对抗损失对图像压缩的影响。最上面那行是来自 ImageNet 数据集的图像 patch。中间展示了用传统损失训练的图像压缩神经网络对图像进行压缩和解压缩的结果。底部是用传统损失和对抗性损失训练的网络对图像进行压缩和解压缩的结果。
虽然基于 GAN 损失训练的图像比起原图来，信息还是有所丢失，但比起其他的方法，图片更加清晰，包含更多细节。
**TFGAN 可以从以下几个方面来支持实验。**
- 
它提供简单的函数调用功能，能覆盖大部分的 GAN 用例，因此仅需几行代码你就能用自己的数据训练模型，而且因为是采用模块化的方式构建，它能覆盖更特殊的 GAN 设计。
- 
你可以任意使用自己想要的模块——损失、评估、特征、训练等模块，这些都是独立的。TFGAN 的轻量级设计意味着你可以将它与其他框架或原生 TensorFlow 代码一起使用。
- 
用 TFGAN 编写的 GAN 模型很容易从未来基础设施的改进中受益，你可以从大量已经实现的损失和特征值中进行选择，而不需要再重写。
- 
最后，代码经过了良好的测试，因此你不必担心使用 GAN 库时容易出现的数值或统计错误。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBUeT7Mfa98yWQ99p7gLR6DSiaiabwP3PY107pGNgiaE2Hntox7mOyTWpkPqv85O3agVRtGuO5PkibMYEg/0?wx_fmt=jpeg)
如上图，大多数文本转语音 (TTS) 神经系统产生的频谱图都过于平滑。当应用到 Tacotron TTS 系统时，GAN 可以重新创建一些更真实的纹理，这将减少输出音频中的人工痕迹。
TFGAN 的开源意味着你将与许多 Google 的研究人员使用的相同的工具，任何人都可以受益于谷歌在库中最先进的改进。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBULg0PyXjjVDR3OiaCudIgwDjRdBUkpx1Dw58Xa9VkyJUuH0piaT7Qyem2gHwfTj6ic45jwaL7y3Zdaw/0?wx_fmt=png)
未来智能实验室致力于研究互联网与人工智能未来发展趋势，观察评估人工智能发展水平，由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎支持和加入我们。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXoz49H9w8lfGXmBjcGga9W15bmYAvZDqbbI8p0liaLcxEh1iao7YXIgJvp6MLHpskd7jHq8ibX0QzDQ/640?wx_fmt=png)
