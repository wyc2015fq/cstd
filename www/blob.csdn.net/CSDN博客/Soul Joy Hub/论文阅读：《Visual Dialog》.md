# 论文阅读：《Visual Dialog》 - Soul Joy Hub - CSDN博客

2018年10月24日 22:34:10[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：153


论文：[https://arxiv.org/pdf/1611.08669.pdf](https://arxiv.org/pdf/1611.08669.pdf)

原文：[http://www.hankcs.com/nlp/cs224n-visual-dialog.html](http://www.hankcs.com/nlp/cs224n-visual-dialog.html)

术语Visual Dialog，大致这么翻译（通俗理解为斗图）。你可以给聊天机器人发送图片，它能理解图片的意思，你们可以就图片内容展开对话；除了文本之外，让图片也成为交流的一部分；这可以帮助盲人理解周围或社交媒体上的信息。

这是自然语言处理与计算机视觉的交叉应用。

![hankcs.com 2017-06-21 下午1.54.35.png](http://wx4.sinaimg.cn/large/006Fmjmcly1fgss4n371hj31by0pihdt.jpg)

## 相关工作

已经有很多工作在为图像和文本搭建桥梁了。

### 图像视频自动标题

![hankcs.com 2017-06-21 下午1.57.00.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fgss71tne1j31e00pub29.jpg)

虽然效果有限，比如右边这只猫实际上在与一只貂戏耍，而不是一个玩具。

### 图像语义对齐

也就是将一个图片中的各个物体自动框出来，自动输出物体之间的语义联系：

![hankcs.com 2017-06-21 下午2.01.15.png](http://wx4.sinaimg.cn/large/006Fmjmcly1fgssbhlpaaj31e60nob29.jpg)

### 图像QA

这种应用已经快要接近图像对话了，只不过QA系统一般是独立的一问一答，而且机器给出的答案一般很短：

![hankcs.com 2017-06-21 下午2.04.02.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fgssedw4cgj319i0o07wh.jpg)

## 贡献

这篇文章提出了这么一种新的任务：图像对话。

然后开发了一个双人对话协议，整理了一个数据库。

介绍了一系列用于图像对话的神经网络模型。

## 技术细节

用于处理图像的CNN，用于处理问题的LSTM，和另一个用于处理历史对话内容的LSTM：

![hankcs.com 2017-06-21 下午2.09.47.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fgsskbitv9j31bm0nm4ih.jpg)

也没讲细节的东西。

## 数据集

数据集如下图左边所示：

![hankcs.com 2017-06-21 下午2.11.47.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fgssmmqel3j31d80pgneu.jpg)

![hankcs.com 2017-06-21 下午2.11.44.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fgssmczvdjj3004002jr7.jpg)

右边是与QA数据集的对比，可见比起QA数据集，VD的答案所含词语数更多，没有唯一答案的问题更多。

## 结果

有一些量化结果（但是没讲，应该是前多少个答案的准确率之类的标准）：

![hankcs.com 2017-06-21 下午2.15.05.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fgsspwij9fj31dy0pyh6z.jpg)

