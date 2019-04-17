# Facebook推出「Rosetta」会识别表情包，还会删帖 - 知乎
# 



> 郭一璞 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-3351b3735db6a422eb847db725f9a9e9_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='300' height='168'></svg>)


![](https://pic2.zhimg.com/v2-0def0203a27c74b84ca423791bc90e31_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='600'></svg>)


![](https://pic1.zhimg.com/v2-233616500293b226c43f8266f3104828_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='440' height='445'></svg>)
这些表情包，都见过吧？

平时斗图、发朋友圈、发微博、知乎抖机灵最离不开它们了。

不过，表情包上的那些网络金句都是.jpg或者.gif的图片格式，无法被搜索、无法被计算机监测，字太小不清晰的时候还会让视力不好的同学看不清楚。

总之，图片版的文字有种种不方便的地方。

所以Facebook干脆出了一个能识别表情包的AI，名叫Rosetta，看看你们发的消息里配的表情包都是些啥。
![](https://pic2.zhimg.com/v2-707915991b41a6623fa1a4dfc8bf57b5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='569'></svg>)
Facebook官方博客给到的例子是一个西班牙语的企鹅表情包。

虽然图上的西班牙文看不太懂，不过可以脑补一下，表情包上本来只有白色加粗大字，Rosetta用蓝色框选出表情包上的文字，然后识别出来，文字上面那一行是它的识别结果。

当然，不止表情包，还有各类其他图片。如果发小黄图或者其他什么不友善的图片被Rosetta发现，它就会启动删帖大法。

## **Rosetta模型**

Rosetta如何识别图片上的文字呢？

不是普通的OCR，而是用Faster R-CNN识别出有字的区域后，再进行文字识别。

具体步骤如下：
- 训练一个可以将图像表示为卷积特征映射的CNN；
- 训练一个区域提议网络（region proposal network，RPN），将图片分为宽5高7共35个小特征图作为输入，RPN找到一些看起来有文字的目标区输出；
- 从每个区域特征图中提取信息，用分类器识别，之后按提案置信度排序，选择最靠谱的提案。
![](https://pic1.zhimg.com/v2-4dbea4195300bc8925bc62794b930ba0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='309'></svg>)
上面这张图则表明了两步模型结构：

第一步是用Faster R-CNN检测字符；

第二步是用有CTC（Connectionist Temporal Classification）损失的ResNet-18完全卷积模型来进行文字识别。

上面两个模型独立训练。

而在训练中，Facebook用上了Caffe2支持的、最近开源的Detectron framework。

另外他们还用上了LSTM来提高模型准确性。
![](https://pic1.zhimg.com/v2-33bce42dd836f4fd1741cc4bb2a97834_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='183'></svg>)△ 最终的文本识别模型结构
## **有特殊的训练技巧**

考虑到低学习率会让模型欠拟合，而高学习率会导致模型发散，Facebook调整了训练程序：

一是先只用比较短的单词来训练，最多只有五六个字母，之后从少到多，用更长的单词来训练，逐步增强单词的长度；

二是从低学习率开始，保证模型不会发散，之后再逐步提高学习率，保证模型稳定。确保稳定之后再降低学习率。
![](https://pic1.zhimg.com/v2-5264d36d78e80f7396d1ce8417122c30_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='624' height='486'></svg>)△ 学习率![](https://pic4.zhimg.com/v2-668d82eb528dfe71cc72c8a27dd9a48b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='626' height='456'></svg>)△ 图像宽度![](https://pic2.zhimg.com/v2-050035f1acce34829b87d72d98dc8e05_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='616' height='480'></svg>)△ 单词长度
## **“我们不要手动搞数据集！”**

由于Rosetta并不是一个纯英文识别AI，它需要学的语言种类太多，单单训练英文识别都要大量数据，何况Facebook的全球用户来自各国，会使用各种不同的语言了。

于是，Facebook除了用人类手工标注的数据之外，还找了一些机器生成的数据集，也就是，找一个AI数据民工，强行给一些无辜的图片加字。

这个AI数据民工来自牛津大学VGG实验室2016年发表在CVPR上的论文《 Synthetic Data for Text Localisation in Natural Images》，可以给正常的自然景物照片上，添加一些蛇精病一样的文字。
![](https://pic2.zhimg.com/v2-adc3e941e37b69a9a18901ed9b276079_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='498'></svg>)
这样，批量生产的带字图片就自带了标注，一个AI生产数据，喂给另一个AI来训练，自给自足。如果以后的模型都用AI生产的数据来喂养的话，估计几十年后不少模型的卖点就变成了：

“人类标注，手动调参，纯天然原生态，古早味模型。”

## **不仅能认表情包**

除了识别表情包和删帖，Rosetta还可以用来实现照片搜索、识别菜谱、辅助视力障碍者认知图片等功能，而且这些功能并不仅仅适用于英文，连阿拉伯语都可以识别。

只不过，阿拉伯语是从右往左写的，所以Facebook在处理阿拉伯语的时候多加了一道从左往右翻过来的步骤。

## **传送门**

Rosetta发在了今年KDD上，论文：

[http://www.kdd.org/kdd2018/accepted-papers/view/rosetta-large-scale-system-for-text-detection-and-recognition-in-images](https://link.zhihu.com/?target=http%3A//www.kdd.org/kdd2018/accepted-papers/view/rosetta-large-scale-system-for-text-detection-and-recognition-in-images)

Facebook博客：

[https://code.fb.com/ai-research/rosetta-understanding-text-in-images-and-videos-with-machine-learning/](https://link.zhihu.com/?target=https%3A//code.fb.com/ai-research/rosetta-understanding-text-in-images-and-videos-with-machine-learning/)

VGG图片加字：

[https://github.com/ankush-me/SynthText](https://link.zhihu.com/?target=https%3A//github.com/ankush-me/SynthText)




— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


