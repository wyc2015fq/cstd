# 谷歌AI翻译娘的异常进化：拿到音频，不做语音识别，也不翻成文本，直接生产英文音频 - 知乎
# 



> 翻栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI

在我们的世界里，谷歌翻译是这样的：
![](https://pic3.zhimg.com/v2-746e5670a1c997bedde4d4285329c0d2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='736' height='675'></svg>)△ 一直被调戏的翻译娘



但在谷歌的世界里，谷歌翻译是这样的：
![](https://pic1.zhimg.com/v2-3e20e0ed8261734d082f5289c559b324.jpg)△西语→英语：你不问就不会知道了https://www.zhihu.com/video/1101801077627801600
请注意，视频里的文字只是为了便于观赏，才存在的。

### 而AI在翻译语音的时候，并**没有把西语音频识别成文本，也不去生成英语的文本，就直接产出了英语音频**。且和标准答案 (↑蓝字↑) 几乎一致。

概括一下：虽然不知道你在说啥，但我已经帮你翻译好了。

这就是谷歌团队的最新成果，脑洞很大，也很有效。

## **怎么会不用看文本？**

这个翻译模型，名字叫做**S2ST** (全称Speech-to-Speech Translation) 。

不看文本只靠听，背后的原理是**把一种语音的声谱图** (Spectrogram) ，**映射到另一种语音的声谱图上**。

那么，声谱图什么样？

下图就是 (西语) “你好么，嘿，我是威廉，你怎么样啊？”的声谱图。
![](https://pic4.zhimg.com/v2-0501e05b7656ee9ff67835163376b1cb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='222'></svg>)△ 横轴是时间，纵轴是Mel频率
这是目标，英文声谱图：
![](https://pic3.zhimg.com/v2-dbab57153e213a44a71504c97ce0fd6e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='215'></svg>)
AI只要从大量的成对数据里，**学懂英文和西语的声谱映射关系**，就算不识别人类说了什么字，依然能当上翻译员。

当然，一个完整的翻译模型，并没有上面说的这么简单，它由三个部分组成：

> **一是**基于注意力的序列到序列 (seq2seq) 神经网络。就是下图的**蓝色**部分，它负责生成目标声谱图，这只是第一步，还不是音频；
**二是**一个声码器(Vocoder) 。下图的**红色**部分，它会把声谱图转换成时域波形 (Time-Domain Waveforms) ，这已经是带有时间顺序的正经声波了；
**三是**个可选的附加功能，原本说话人的编码器。**绿色**部分，经过它的加工，翻译出的英文，和原本的西语，听上去就像同一个人发出来的。
![](https://pic3.zhimg.com/v2-1975025df1a9989f280e11e4bf45b8f6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='597'></svg>)
当然，蓝色部分还是主角。

里面的编码器 (左) ，是8层双向LSTM堆起来的；而解码器 (Spectrogram Decoder) ，团队说要选4-6层LSTM的，深一点效果比较好。

## **成功了**

模型是用人类自发的对话 (比如打电话的语音) 端到端训练出来的，一起来看看成果吧。

**第一题**，短语。“克兰菲尔德大学的新员工”，翻译和标答一字不差：
![](https://pic3.zhimg.com/v2-d94c9ca1cffd4e159f5344bd320cacf2.jpg)https://www.zhihu.com/video/1101642068975370240
> 原文：nuevos empleados de Cranfield University
标答：**New hires at Cranfield University**




**第二题**，句子。“看看这个国家上下，你看到了什么”，依然和标答一致：
![](https://pic1.zhimg.com/v2-162c9bf5b76e7c557717d59678d24188.jpg)https://www.zhihu.com/video/1101642180430524416
> 原文：Por lo tanto, mirar alrededor del país y lo que ves.
标答：**So, look around the country and what do you see?**

**对手**表现怎样？借助转换文本来翻译的AI，缺了个“do”字：
![](https://pic2.zhimg.com/v2-8989171bd21e7593a14fb1df101645ad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='534' height='198'></svg>)



**第三题**，带各种从句的句子。“我的表 (堂) 兄弟姐妹们小的时候，我照顾过他们也教过他们，有过一些这样的经历。”
![](https://pic3.zhimg.com/v2-e3c2586c97a8bf19e22046d7397fccb6.jpg)https://www.zhihu.com/video/1101642401344552960
> 原文：Tengo cierta experiencia en cuidar y enseñar a mis primos cuando eran jóvenes.
标答：**I’ve got some experience in looking after and teaching my cousins when they were young.**

**照顾** (Taking **Care of**) 的意思有缺失，其他部分对比标答是完整的。

再看对手，“照顾 (Care) ”和“教 (Teach) ”都用了动词原形，语法不是很严格：
![](https://pic3.zhimg.com/v2-f4e479905bfb1ae31b33fcb65460ab6a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='538' height='202'></svg>)



肉眼观赏之后，再让**S2ST**和**先转换文本再翻译的AI**对比一下**BLEU分**。

在“Conversational”大数据集上，S2ST的BLEU分比对手差了6分：42.7比48.7。
![](https://pic2.zhimg.com/v2-2a35dc59a31ca93605e0e97d8f1b9d05_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='601'></svg>)
的确还有一段距离，但毕竟对手是依靠文本，算是开卷考了。

这样说来，**跳过文本**的想法，虽然听起来有些飘，但证明是可行的。

所以，谷歌团队说，大有可为啊。

论文传送门：
[https://arxiv.org/pdf/1904.06037.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1904.06037.pdf)

更多样本传送门：
[https://google-research.github.io/lingvo-lab/translatotron/](https://link.zhihu.com/?target=https%3A//google-research.github.io/lingvo-lab/translatotron/)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


