
# 基于CNN和序列标注的对联机器人 | 附数据集 & 开源代码 - Paper weekly - CSDN博客


2019年01月15日 12:07:25[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：103


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨苏剑林
单位丨广州火焰信息科技有限公司
研究方向丨NLP，神经网络
个人主页丨kexue.fm

# 缘起

前几天看到了这个[脑洞清奇的对联AI，大家都玩疯了](https://mp.weixin.qq.com/s?__biz=MzIzNjc1NzUzMw==&mid=2247511349&idx=1&sn=8e5968c0d58c70bab29ab11216b2ac4f&chksm=e8d01a47dfa7935158b0a195c3ff70fa1ebcfac6a62d6c7d5f0db5c6290e4375b87cae14f80e&mpshare=1&scene=21&srcid=0114ebz0Q6v4SqrUdwyEUD1Q#wechat_redirect)一文，觉得挺有意思，难得的是作者还整理并公开了数据集，所以决定自己尝试一下。

# 动手

“对对联”，我们可以看成是一个句子生成任务，可以用 Seq2Seq 完成，跟我之前写的[玩转Keras之Seq2Seq自动生成标题](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491314&idx=1&sn=3e22d4a6d732b0877fdc567d2bce1076&chksm=96e9c172a19e48646005da05e143751aa9012c141dd1cf9846a2b418cbf854c7d343013105a1&scene=21#wechat_redirect)一样，稍微修改一下输入即可。上面提到的文章所用的方法也是 Seq2Seq，可见这算是标准做法了。

**分析**

然而，我们再细想一下就会发现，**相对于一般的句子生成任务，“对对联”有规律得多：**1）上联和下联的字数一样；2）上联和下联的每一个字几乎都有对应关系。

如此一来，其实对对联可以直接看成一个序列标注任务，跟分词、命名实体识别等一样的做法即可。这便是本文的出发点。

说到这，其实本文就没有什么技术含量了，序列标注已经是再普通不过的任务了，远比一般的Seq2Seq来得简单。

所谓序列标注，就是指输入一个向量序列，然后输出另外一个通常长度的向量序列，最后对这个序列的“每一帧”进行分类。相关概念来可以在[简明条件随机场CRF介绍 | 附带纯Keras实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489378&idx=1&sn=0e0ed4424bb336022f36d8e2236f96cc&chksm=96e9c8e2a19e41f4d1fb67254ee3c057ce66a4eaa4084db89d53f314c833b73fb79b8ee3c0dd&scene=21#wechat_redirect)一文进一步了解。

**模型**

本文直接边写代码边介绍模型。如果需要进一步了解背后的基础知识的读者，还可以参考**《中文分词系列：基于双向LSTM的Seq2Seq字标注》**[1]、**《中文分词系列：基于全卷积网络的中文分词》**[2]、**《基于CNN和VAE的作诗机器人：随机成诗》**[3]。

我们所用的模型代码如下：

```python
x_in = Input(shape=(
```
```python
None
```
```python
,))
```

```python
x = x_in
```

```python
x = Embedding(len(chars)+
```
```python
1
```
```python
, char_size)(x)
```

```python
x = Dropout(
```
```python
0.25
```
```python
)(x)
```

```python
x = gated_resnet(x)
```

```python
x = gated_resnet(x)
```

```python
x = gated_resnet(x)
```

```python
x = gated_resnet(x)
```

```python
x = gated_resnet(x)
```

```python
x = gated_resnet(x)
```

```python
x = Dense(len(chars)+
```
```python
1
```
```python
, activation=
```
```python
'softmax'
```
```python
)(x)
```

```python
model = Model(x_in, x)
```

```python
model.compile(loss=
```
```python
'sparse_categorical_crossentropy'
```
```python
,
```

```python
optimizer=
```
```python
'adam'
```
```python
)
```

其中gated_resnet是我定义的门卷积模块：

```python
def
```
```python
gated_resnet
```
```python
(x, ksize=
```
```python
3
```
```python
)
```
```python
:
```

```python
# 门卷积 + 残差
```

```python
x_dim = K.int_shape(x)[
```
```python
-1
```
```python
]
```

```python
xo = Conv1D(x_dim*
```
```python
2
```
```python
, ksize, padding=
```
```python
'same'
```
```python
)(x)
```

```python
return
```
```python
Lambda(
```
```python
lambda
```
```python
x: x[
```
```python
0
```
```python
] * K.sigmoid(x[
```
```python
1
```
```python
][..., :x_dim]) \
```

```python
+ x[
```
```python
1
```
```python
][..., x_dim:] * K.sigmoid(-x[
```
```python
1
```
```python
][..., :x_dim]))([x, xo])
```

仅此而已，就这样完了，剩下的都是数据预处理的事情了。当然，读者也可以尝试也可以把gated_resnet换成普通的层叠双向 LSTM，但我实验中发现层叠双向 LSTM 并没有层叠gated_resnet效果好，而且 LSTM 相对来说也很慢。

**效果**

训练的数据集来自以下链接，感谢作者的整理。

https://github.com/wb14123/couplet-dataset

**完整代码：**

https://github.com/bojone/seq2seq/blob/master/couplet_by_seq_tagging.py

**训练过程：**

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCaybtelrx6UzhoC3kSOrI3dDrw3z92hJ05xrYVqhBNdZjBnhEyABwQhA/640?wx_fmt=png)
**▲**对联机器人训练过程

**部分效果：**

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCaNnWargNnCLPZhw60kN9WYRP46m7gwo3WVcs7yQfywOiaIOTsz7g44eQ/640?wx_fmt=png)

看起来还是有点味道的。注意“晚风摇树树还挺”是训练集的上联，标准下联是“晨露润花花更红”，而模型给出来的是“夜雨敲花花更香”，说明模型并不是单纯地记住训练集的，还是有一定的理解能力；甚至我觉得模型对出来的下联更生动一些。

总的来说，基本的字的对应似乎都能做到，就缺乏一个整体感。总体效果没有下面两个好，但作为一个小玩具，应该能让人满意了。

> 王斌版AI对联：
> https://ai.binwang.me/couplet/

> 微软对联：
> https://duilian.msra.cn/default.htm


# 结语

最后，也没有什么好总结的。我就是觉得这个对对联应该算是一个序列标注任务，所以就想着用一个序列标注的模型来试试看，结果感觉还行。

当然，要做得更好，需要在模型上做些调整，还可以考虑引入 Attention 等，然后解码的时候，还需要引入更多的先验知识，保证结果符合我们对对联的要求。这些就留给有兴趣做下去的读者继续了。

# 相关链接

[1] https://kexue.fm/archives/3924
[2] https://kexue.fm/archives/4195
[3] https://kexue.fm/archives/5332

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看作者其他文章：**

[变分自编码器VAE：原来是这么一回事 | 附开源代码](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)
[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)
[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)
从变分编码、信息瓶颈到正态分布：论遗忘的重要性

[深度学习中的互信息：无监督提取特征](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492040&idx=1&sn=f90a6b899e62748c4db489ce06276869&chksm=96ea3e48a19db75e8c07d942a4772bb6c784fac7bcb117da2023186546cfe1876b121a8121cc&scene=21#wechat_redirect)
[全新视角：用变分推断统一理解生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)
[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)
[细水长flow之f-VAEs：Glow与VAEs的联姻](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491695&idx=1&sn=21c5ffecfd6ef87cd4f1f754795d2d63&chksm=96ea3fefa19db6f92fe093e914ac517bd118e80e94ae61b581079023c4d29cedaaa559cb376e&scene=21#wechat_redirect)
[深度学习中的Lipschitz约束：泛化与生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492180&idx=1&sn=3ea92a3a9f1306efde89ce1777b80da6&chksm=96ea3dd4a19db4c20dcbc9627b0eb307672b4d61008a93c42814fa6728ca7b6f7c293cff1d80&scene=21#wechat_redirect)




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。

📝**来稿标准：**
• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向）
• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接
• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志

**📬 投稿邮箱：**
• 投稿邮箱：hr@paperweekly.site
• 所有文章配图，请单独在附件中发送
• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通



🔍

现在，在**「知乎」**也能找到我们了
进入知乎首页搜索**「PaperWeekly」**
点击**「关注」**订阅我们的专栏吧


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 查看作者博客


