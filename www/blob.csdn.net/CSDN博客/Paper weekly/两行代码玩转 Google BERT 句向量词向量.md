# 两行代码玩转 Google BERT 句向量词向量 - Paper weekly - CSDN博客





2018年11月22日 13:06:16[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：2807









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




**关于作者：**肖涵博士，bert-as-service 作者。现为腾讯 AI Lab 高级科学家、德中人工智能协会主席。**肖涵的 Fashion-MNIST 数据集已成为机器学习基准集，在 Github 上超过 4.4K 星，一年来其学术引用数超过 300 篇。**




肖涵在德国慕尼黑工业大学计算机系取得了计算机博士（2014）和硕士学位（2011），在北邮取得了信息通信学士学位（2009）。他曾于 2014-2018 年初就职于位于德国柏林的 Zalando 电商，先后在其推荐组、搜索组和 Research 组担任高级数据科学家。肖涵所创办的德中人工智能协会（GCAAI）如今拥有 400 余名会员，致力于德中两国 AI 领域的合作与交流，是德国最具影响力的新型团体之一。




**WeChat:** hxiao1987 

**Blog:**https://hanxiao.github.io

**德中人工智能协会:**https://gcaai.org 




Google AI 几周前发布的 BERT (Bidirectional Encoder Representations from Transformers) 模型在 NLP 圈掀起了轩然大波，**其使用超多层 Transformer + 双任务预训练 + 后期微调的训练策略，在 11 个不同类型的 NLP 任务上刷新了纪录。**




Google 随后在 Github 上开源了 BERT 的代码，并提供了在维基百科语料上使用 TPU 预训练好的模型供大家下载。这其中也包括了基于字符级别的中文 BERT 预训练模型。 




> 
**BERT开源代码：**

https://github.com/google-research/bert








**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4pZRGibSj8VOQAD9BNEtEuSTsP3GUXylbNzso8GbPzKtlIzkibwrlxHTg/640)**



**bert-as-service 能让你简单通过两行代码，即可使用预训练好的模型生成句向量和 ELMo 风格的词向量： **



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC41vTmA3U4Qyre4ou0S0oJfLwO0kwSicOW7Vf81IRiaIK9etIg47AV0iaOA/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4mXbllib2qMSApwRbU5krNON1e5BzH6o36T2Psc9JQYia1Kia0YTzTzHqQ/640)




你可以将 bert-as-service 作为公共基础设施的一部分，部署在一台 GPU 服务器上，使用多台机器从远程同时连接实时获取向量，当做特征信息输入到下游模型。



# 回顾：BERT的训练机制




**BERT 模型的训练分为预训练（Pre-training）和微调（Pre-training）两步。**预训练和下游任务无关，却是一个非常耗时耗钱的过程。Google 坦言，对 BERT 的预训练一般需要 4 到 16 块 TPU 和一周的时间，才可以训练完成。




庆幸的是，大部分 NLP 研究者只需使用 Google 发布的预训练模型，而不需要重复这一过程。你可以把预训练模型想象成一个 Prior，是对语言的先验知识，一旦拥有就不需要重复构造。 




**微调取决于下游的具体任务。**不同的下游任务意味着不同的网络扩展结构：比如一个对句子进行情感分类的任务，只需要在 BERT 的输出层句向量上接入几个 Dense 层，走个 softmax。而对于 SQuAD 上的阅读理解任务，需要对 BERT 输出的词向量增加 match 层和 softmax。




**总体来说，对 BERT 的微调是一个轻量级任务，微调主要调整的是扩展网络而非 BERT 本身。** 换句话说，我们完全可以固定住 BERT 的参数，把 BERT 输出的向量编码当做一个特征（feature）信息，用于各种下游任务。 




无论下游是什么任务，对于 NLP 研究者来说，最重要的就是获取一段文字或一个句子的定长向量表示，而将变长的句子编码成定长向量的这一过程叫做 sentence encoding/embedding。 




bert-as-service 正是出于此设计理念，将预训练好的 BERT 模型作为一个服务独立运行，客户端仅需通过简单的 API 即可调用服务获取句子、词级别上的向量。在实现下游任务时，无需将整个 BERT 加载到 tf.graph 中，甚至不需要 TensorFlow 也不需要 GPU，就可以在 scikit-learn, PyTorch, Numpy 中直接使用 BERT。




# bert-as-service




bert-as-service 将 BERT模型作为一个独立的句子编码（sequence encoding/embedding）服务，**在客户端仅用两行代码就可以对句子进行高效编码**。其主要特色如下： 



- 
**state-of-the-art：**基于 Google 最新发布的 BERT 模型；



- 
**易用：**客户端仅需简单两行代码即可调用； 



- 
**快速：**每秒 780 个句子（见**详细评测**）；



- 
**并发性：**自动扩展到多块 GPU，多客户端，高效任务调度，无延迟（见**针对多客户端并发的评测**）。







> 
**速度评测：**

https://github.com/hanxiao/bert-as-service#Benchmark




**并发评测：**

https://github.com/hanxiao/bert-as-service#speed-wrt-num_client





# 使用方法




**1. 下载 Google 发布的预训练 BERT 模型**




从下方链接下载 Google 发布的预训练模型，解压到某个路径下，比如：/tmp/english_L-12_H-768_A-12/




> 
**预训练模型下载：**

https://github.com/google-research/bert#pre-trained-models





你可以使用包括BERT-Base, Multilingual和BERT-Base, Chinese 在内的任意模型。




**2. 开启 BERT 服务**



`python app.py -model_dir /tmp/english_L-12_H-768_A-12/ -num_worker=4`



这个代码将开启一个 4 进程的 BERT 服务，意味着它可以最高处理来自 4 个客户端的并发请求。虽然同一时刻连接服务的客户端数量没有限制，但在某时刻多余 4 个的并发请求将被暂时放到一个负载均衡中，等待执行。有关 bert-as-service 背后的架构可以参考 FAQ 和并发客户端性能评测。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4Yq5fJbEd8TdV8mmEtYQc8MGI0qWZGibYVCLoTIQLL5tE9yG680KdeDg/640)




**3. 使用客户端获取句子向量编码**




对于客户端来说，你唯一需要的文件就是 service/client.py ，因为我们需要从中导入 BertClient 。




```
from service.client import BertClient
bc = BertClient()
bc.encode(['First do it', 'then do it right', 'then do it better'])
```




这会返回一个 3 x 768 的 ndarray 结构，每一行代表了一句话的向量编码。你也可以通过设置，让其返回 Python 类型的 List[List[float]] 。




**在另一台机器上使用 BERT 服务**




客户端也可以从另一台机器上连接 BERT 服务，只需要一个 IP 地址和端口号：




```
# on another CPU machine
from service.client import BertClient
bc = BertClient(ip='xx.xx.xx.xx', port=5555)  # ip address of the GPU machine
bc.encode(['First do it', 'then do it right', 'then do it better'])
```




你还可以把服务架设在 docker container 中使用，详情可以参考项目的 README.md。**bert-as-service 所支持的 C/S 模式可以用下图总结：**




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4HsmulRsiclOrTeCflUbAWXbKguT1HI1vB41NicUHs1faSyMKUyY7gCDw/640)




# 性能评测




作为一个基础服务，速度和伸缩性（scalability）非常关键。只有当下游的模型能够通过其快速流畅地获取数据时，该服务的意义才能得到最大体现。BERT 的网络复杂度众所周知，**那么 bert-as-service 能否达到工程级别的速度？**为了验证这一点，我们做了如下方面的评测。




 max_seq_len **对速度的影响**




 max_seq_len 是服务端的一个参数，用来控制 BERT 模型所接受的最大序列长度。当输入的序列长度长于 max_seq_len 时，右侧多余字符将被直接截断。所以如果你想处理很长的句子，服务器端正确设置 max_seq_len 是其中一个关键指标。而从性能上来讲，过大的 max_seq_len 会拖慢计算速度，并很有可能造成内存 OOM。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4ib6r4ibKJgeU2fzZQ3DBWwyicDibXAXLnzuGkT88e7yTpn9KibDK2BGLXMg/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC42m9m039ZYSB7I6Oktc9LOsL4KQeuEtbLJib6af7nw3h5WHuwrqubjIg/640)




 client_batch_size **对速度的影响**




 client_batch_size 是指每一次客户端调用 encode() 时所传给服务器 List 的大小。出于性能考虑，请尽可能每次传入较多的句子而非一次只传一个。比如，使用下列方法调用：




```
# prepare your sent in advance
bc = BertClient()
my_sentences = [s for s in my_corpus.iter()]
# doing encoding in one-shot
vec = bc.encode(my_sentences)
```




而不要使用：




```
bc = BertClient()
vec = []
for s in my_corpus.iter():
    vec.append(bc.encode(s))
```




如果你把 bc = BertClient() 放在了循环之内，则性能会更差。当然在一些时候，一次仅传入一个句子无法避免，尤其是在小流量在线环境中。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4Uf32Fjm3JDQ7ExIKsfyfnoFeaveAkLFibc5ThET8DzYN59Tj7MJntnA/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4aXBCLMicWGNOuCFfHqJsJqSBVCA1VkOzSQCC6icB73wgVIS3qiaYDa1Tw/640)




 num_client **对并发性和速度的影响**




 num_client 指同时连接服务的客户端数量。当把 bert-as-service 作为公共基础设施时，可能会同时有多个客户端连接到服务获取向量。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4iaicbFich6vmw5GvGh9tzfviciaC596WtiaFnaVOWRGZt7zribZs4gCxtpnJg/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4INoytPVrWDFqicmKhhJHxibb1PY4lmlJ8MHQowKGzKgK013Cf0e4ic4Hw/640)




可以看到一个客户端、一块 GPU 的处理速度是每秒 381 个句子（句子的长度为 40），两个客户端、两个 GPU 是每秒 402 个，四个客户端、四个 GPU 的速度是每秒 413 个。这体现了 bert-as-service 良好的伸缩性：当 GPU 的数量增多时，服务对每个客户端请求的处理速度保持稳定甚至略有增高（因为空隙时刻被更有效地利用）。




# 其它常见问题列表和详细指南




参见：




https://github.com/hanxiao/bert-as-service




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看更多论文解读：**




- 
[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492002&idx=1&sn=2d2c8551cd44ee5506cac3ff38a438f3&chksm=96ea3e22a19db73404bc9af4de8b3c406cbc1097f305b63da82efd1d07915500f4042927ee22&scene=21#wechat_redirect)[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492226&idx=1&sn=eafb23c1658f487f47254128bcc6e1b2&chksm=96ea3d02a19db4149215378b7c18d6a92dcd6bbacbbeb73e9bfa2d1594073528adab03ccd031&scene=21#wechat_redirect)[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492415&idx=1&sn=a359e72ee99555f7a2fb4e21b2ad51db&chksm=96ea3cbfa19db5a913b9ed01490df6e902abfeb36856bb5be946527a070d399ee37a3ff6c750&scene=21#wechat_redirect)[深度](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)[解读DeepMind新作：史上最强GAN图像生成器](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)

- 
[GraphWave：一种全新的无监督网络嵌入方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492940&idx=1&sn=4e9799959400f56fd2371b80e7696c96&chksm=96ea3acca19db3da5e870cb6bc3bba225fb9755945e8b6e45f8307df0f3816b2c78703443caf&scene=21#wechat_redirect)

- 
[这16篇最新论文，帮你轻松积攒知识点](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492912&idx=1&sn=e223b0bf570148493313ea8780cef2fc&chksm=96ea3ab0a19db3a6eb87b8c8d6cb41d1a4ae0d85b5fd7b616baa970234124c320fda1cdcc7d9&scene=21#wechat_redirect)

- 
[TensorSpace：超酷炫3D神经网络可视化框架](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492746&idx=1&sn=921d7315a973b85dd4e802cb5fd456fb&chksm=96ea3b0aa19db21c48841ddcee38592a3c086ae8fa1a9893cf46ff974f0f38fb350bcd528265&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。




📝 **来稿标准：**

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

▽ 点击 | 阅读原文| 获取最新论文推荐




