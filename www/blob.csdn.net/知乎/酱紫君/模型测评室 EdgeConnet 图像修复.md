# 模型测评室: EdgeConnet 图像修复 - 知乎
# 

EdgeConnet 是个今年初新开源的 Inpainting 模型, 使用双阶段处理的方法达到了不错的效果.

我觉得这种 Progressive 而不是一步到位的手法是正确的发展方向, 以后会有越来越多的多阶段 GAN 模型.

## 效果分: ★★★★

效果看上去很爆炸, 不过今年都 2019 了, 这个结果其实并不能稳居 SOTA.
![](https://pic2.zhimg.com/v2-de7229b18544ec2a1ee35d9cf4d7e32d_b.jpg)
原始地址: [knazeri/edge-connect](https://link.zhihu.com/?target=https%3A//github.com/knazeri/edge-connect)

APP下载: [NetApplication/EdgeConnet-Inpainting.app](https://link.zhihu.com/?target=https%3A//m.vers.site/NetApplication/EdgeConnet%2520Inpainting.app)

## 项目分: ★★★★☆

Config 很全, 直接能跑, 模型也很好改, 注释恰到好处, 是模范级的开源项目!

## 综合分: ★★★

创意上的话, 这文其实并不算很有创意, 因为画边修改图片的做法在前深度学习时代就有了.

我去年还见了一篇交互式的效果爆炸的 PS 模型, 可以用绿笔添加, 用红笔删除, 一时半会儿找不到这个文了.

近一点的模型可以看这个: [https://arxiv.org/pdf/1806.03589.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1806.03589.pdf)

既然如此, 那么让机器自己描边是个自然而然的想法.

模型的缺点么, 也没啥缺点, 除了 GAN 共同的缺点, 模型迁移能力非常差, 对于每个数据集要专门训练模型. 

比如上面的人脸, 其实只适用于类似 CelebA 的大头数据集, 用生活照就模式崩坏了...

模式崩坏的图比较克苏鲁, 不放了.....

还有高清图的修复还是有明显扭曲的, 这个和空洞卷积块有关系, 白条太大了没法覆盖掉.

## 结构分: ★★

结构的话, GAN结构都很简单, 中规中矩, 二阶段GAN算一点创新吧

整个模型分为两个阶段.

第一阶段就是 GAN 根据各种信息生成具有指导作用的 EdgeMap

**注意这里所有的卷积反卷积都要执行谱归一化.**
![](https://pic2.zhimg.com/v2-fd18b23487c680d884620969f3df8f01_b.jpg)
这个阶段没复现成功, 而且可以发现可操作性很低

他需要三个东西, Gray, 好说, 就是图片本身, Mask, 也好说, 就是指定一个底色就行了, Edge, 这个就麻烦了, 看论文里的意思是用 Canny 撸了一遍.

那行吧, 我也用 Canny 撸了一遍, 然后输入网络得到了:
![](https://pic1.zhimg.com/v2-07c1830099f7c778e693f56587aa83a0_b.jpg)
啥玩意儿这是....Canny 撸一遍的话 Mask 边缘肯定也会被检测出来, 我倒是也不知道怎么得到和论文一样的效果了...

然后就是第二阶段了, 和第一阶段完全一样的模型, 而且更简单, 因为没有 SpectralNorm 了
![](https://pic4.zhimg.com/v2-e0dd96e59a0fb5920b64a2daed1171d3_b.jpg)
写出来差不多是下面这样:
![](https://pic3.zhimg.com/v2-cf5bc90a033ce308c789eeebd0b189d6_b.jpg)
其中 Refiner 是一个 ResBlock, 卷积使用空洞卷积, InstanceNorm 使用默认初始化, 不参与训练.

如果 APP 跑不起来或者有其他问题的, 可以提 issue.
[GalAster/WLNet-ModelZoo​github.com![图标](https://pic2.zhimg.com/v2-deeb6582665c607b92bb1e152fc6b479_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/GalAster/WLNet-ModelZoo)
你的 Mathematica 至少得是 11.3 且你有一个 GPU.

如果你想要部署某个模型但是没有 mxnet 和 onnx 的实现也可以提出 request.

说到图像修复, 我就想到了某绅士项目: [deeppomf/DeepCreamPy](https://link.zhihu.com/?target=https%3A//github.com/deeppomf/DeepCreamPy)
![](https://pic4.zhimg.com/v2-44d9be2c5c31a86f5887188927ba7683_b.jpg)
不过这个用到了部分卷积, 部分卷积的 mxnet 实现有点困难, 因为缺少某些 operator, 用 ndarray 可以但是却没有对应的 symbol. 这我得等官方弄出 api 了.

