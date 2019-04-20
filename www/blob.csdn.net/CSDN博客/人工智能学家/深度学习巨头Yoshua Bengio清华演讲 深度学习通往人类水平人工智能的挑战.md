# 深度学习巨头Yoshua Bengio清华演讲: 深度学习通往人类水平人工智能的挑战 - 人工智能学家 - CSDN博客
2018年11月11日 19:48:22[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：29
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpnSFx2wOTh37W3L5icYv4f1Jqyn7UrTM7Q69NsIFqic7n0iaRCDWAhyBAw/640?wx_fmt=jpeg)
来源：专知
摘要：2018年11月7日晚，被称为“深度学习三巨头”之一的蒙特利尔大学计算机科学与运算研究系教授Yoshua Bengio在清华大学做了《深度学习抵达人类水平人工智能所面临的挑战（Challenges for Deep Learning towards Human-Level AI》的学术报告。Yoshua Bengio教授客观的说，目前人工智能距离人类水平还仍然十分遥远，人工智能在工业应用的成功主要得益于监督学习方法，人工智能仍然面临巨大挑战，尤其在人类智能机理方面的研究还亟需加强。在报告中，Yoshua Bengio深度探讨了深度学习模型的具体内容，如何实现对抽象特征的多层次学习，如何更好地进行表示学习，使用判别器优化信息间的独立性、相关性和熵，Baby AI框架等话题。
**Yoshua Bengio**
**![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpW7LGfiaiak4pkmrz9hHVNJibob914VQ1SCbY7vmIicT0rcD3U7ViaRPDx0g/640?wx_fmt=jpeg)**
Yoshua Bengio教授是蒙特利尔大学计算机系教授和加拿大科学院院士，被称为神经网络三巨头之一。著有《Deep Learning》、《Learning Deep Architectures for AI》、《A neural probabilistic language model》等多部畅销书/课本。在他的倡导下，加拿大成立了Mila研究院，专注研究人工智能和深度学习。也正是因为在深度学习方面的重要贡献，Yoshua Bengio教授获得了加拿大总督功勋奖，该奖项主要为了纪念做出了卓越成就或者对国家做出了重大贡献的各领域人士，是加拿大公民的最高荣誉之一。
**演讲报告**
**![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpQsu7RNicibnTNZ73XjTQJUXvUAFiabkyianSX6cjgta3MA8thfZg1yjJCQ/640?wx_fmt=jpeg)**
人工智能的目标是让计算机能够进行人与动物所擅长的「决策」，为此，计算机需要掌握知识——这是几乎全体 AI 研究者都同意的观点。他们持有不同意见的部分是，我们应当如何把知识传授给计算机。经典 AI（符号主义）试图将我们能够用语言表达的那部分知识放入计算机中。但是除此之外，我们还有大量直观的（intuitive）、 无法用语言描述的、不能通过「意识」获得的知识，它们很难应用于计算机中，而这就是机器学习的用武之地——我们可以训练机器去获取那些我们无法以编程形式给予它们的知识。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpUUP1XuZY7a4AOrnpd54ag6RFZPSZicZjnNb2HQiaWA2EDjRIiagBA9HVw/640?wx_fmt=jpeg)
深度学习和 AI 领域有很大进步、大量行业应用。但是它们使用的都是监督学习，然而这些模型非常脆弱，极易受到外界干扰。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmphI6tfdqlIbWKS5afq0YjicwgzibNqhkA31z27qT9jiaI6eRxBz6OZPLLg/640?wx_fmt=jpeg)
现在的系统的失败之处在于，它们无法捕捉我们真正想让机器捕捉到的高级抽象（high level abstraction）。而这样的表征可以捕捉构成数据的根本因素。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpBnvWDwtE8GEw7SnKDfGOCduGoKSUw3vAGEBLph3RQHBiaBHbSJAiaMDg/640?wx_fmt=jpeg)
如何发现好的纠缠表征？一个好的表征空间中，不同要素的变化应该可以彼此分离。除了解纠缠变量，我们还希望系统能解纠缠计算。解纠缠和因果的概念相关，而因果正是机器学习界需要重点关注的领域。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmp9tjFAY7ibxhoYWrv8hlZWuJ6L4ia3aiaXgMnfjzqicCAcf9wYNmUAjibk4w/640?wx_fmt=jpeg)
人类的认知任务可以分为系统 1 认知和系统 2 认知。系统 1 认知任务是那些你可以在不到 1 秒时间内无意识完成的任务。例如你可以很快认出手上拿着的物体是一个瓶子，但是无法向其他人解释如何完成这项任务。这也是当前深度学习擅长的事情，「感知」。系统 2 认知任务与系统 1 任务的方式完全相反，它们很「慢」。大多数人需要遵循一定的规则、按照步骤完成计算。这是有意识的行为，你可以向别人解释你的做法，而那个人可以重现你的做法——这就是算法。计算机科学正是关于这项任务的学科。而我对此的观点是，AI 系统需要同时完成这两类任务。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpJ9jDMCSAsK3ITbKia9iaznnLlDjZEic9EH7TvJvQ8KvF3Bte8Eib6K9lqA/640?wx_fmt=png)
意识空间里的事物维度很低，因而可以在这样的空间里进行推理。意识先验就是假设有很多只包含很少变量但为真的事情，因此，好的表示的一个性质，就是当我们把数据映射到表示空间之后，变量之间只有少数相关。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpgK5229ZLaV0xG2TGD6so03POoiaw8HKPJIv1Mj098e3h1Tz5oEZbE4w/640?wx_fmt=png)
那么要如何实现这种表征呢？对此，注意力机制是一种很重要的工具。注意力机制可以按顺序选取重点关注的信息，来实现整个系统的端到端训练。我们不需要设计一个独立的系统来做这种选择。你可以将注意力机制作为在某些全局目标下端到端训练的更大系统的一部分。而这正是深度学习擅长的地方。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmp6QibvCKGWkOtqV6cudqL0ialkm2PYsbGzGOBjOjhJM9icvtibphmwqhL1w/640?wx_fmt=png)
在架构方面，意识先验在「原始输入」和「某些更高级的表征」之外，还引入了第三个层次：这也就是有意识状态（conscious state）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmphCjIdjXM7ulTDATicU8cMcjeN74fZtYUOjz25icMuqIN187CoRfABMWg/640?wx_fmt=png)
这个理论框架还有非常多的细节需要完善，去年我们主要关注其中的一个方面：目标函数。机器学习和深度学习中的标准训练目标函数都基于最大似然估计，而即使与最大似然无关的目标函数，例如 GAN 的一些目标函数，也是在像素级别进行构建的。然而，我们实际上想要在隐藏空间中表现出可预测性。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpPE7bEb4JWsLvEJkvPpRcuia7UHLN5I3JLEzMEpYiaxYvnKmCmmkQKMPg/640?wx_fmt=jpeg)
现在的NLP任务只是在文本+标注上训练模型。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpZBIWOIE2LUoVlTWRczam6Vn60dYRlUD3dfqwQHkzKgbj51FxPlyia2A/640?wx_fmt=jpeg)
这样会出现很多常识性的错误，因为它并没有理解语言内在的含义。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpag8UATzlau6uneqICF0M70xdylZet5WDPicIF9J6a5nCvZRLDEfE8OA/640?wx_fmt=jpeg)
要真正理解自然语言，不仅要对语言本身建模，还要对所处环境进行建模。要将语言学习和世界运转方式的学习相结合。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmp6icYYsawibBeabsNrqyiaGX96kfpliaNHibf5T8ewic9MFmGtciclWVicZKgow/640?wx_fmt=jpeg)
机器需要对事物之间的因果联系进行建模。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw3HkFcXVtibOcOiaRte27iabmpuMic3ucsP8hswcqhBX0tucsgsQXBvLam1xO03Kza9kSGM217GzC6raQ/640?wx_fmt=jpeg)
BabyAI通过19个由易到难的游戏关卡而不断学习，就像婴儿成长的过程一样。这很像课程学习（curriculum learning）。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
