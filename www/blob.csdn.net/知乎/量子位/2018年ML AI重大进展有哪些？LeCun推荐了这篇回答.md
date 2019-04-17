# 2018年ML/AI重大进展有哪些？LeCun推荐了这篇回答 - 知乎
# 



> 乾明 编译整理
量子位 报道 | 公众号 QbitAI

回望2018，AI大潮依旧浩浩汤汤，势头不减。

这一年都有哪些重要进展呢？2018年即将过去，一些大牛也给出了自己的看法。

刚刚，前Quora技术VP、AI领域技术专家Xavier Amatriain在Quora上回答了一个相关的问题：2018年ML/AI领域最重要的进展是什么？



![](https://pic3.zhimg.com/v2-d58d80b97cea827d1117f7d81f508fc6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='450' height='450'></svg>)



他在答案中给出了4个方面：
- 回归理性，炒作降温；
- 舍虚务实，更关注具体问题；
- 深度学习在NLP领域大放异彩；
- 框架之争愈发激烈，强化学习成焦点。

答案发布之后，便引发了大量围观，Quora上点赞近400，Yann LeCun也在Twitter上转发推荐。



![](https://pic1.zhimg.com/v2-e529a7b1f44819dc1d707d0e280d6db4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='483'></svg>)



当然，答案不仅仅只有这4句话，Amatriain也都给出了解释。


## **炒作降温**

2017年，是AI炒作无所不在的一年。最突出的，就是马斯克和扎克伯格等各方大佬就AI对于人类是福是祸进行了隔空论战。这些论战为AI赚足了注意力。

Amatriain表示，与2017年相比，我们好像冷静下来了。一个主要的原因可能是这些大佬们忙于处理其他事情了。

比如Facebook深陷数据与隐私旋涡，麻烦事情不断。马斯克也历经特斯拉生产地狱，度过了艰难的一年。


与此同时，虽然很多人都认为自动驾驶以及类似的技术正在向前发展，但就目前事故不断的情况，所谓的“明天”，还有很远。

## **更关注具体问题**

相对于AI是福是祸的讨论，2018年对AI的关注也开始变得愈加务实了。

首先是公平性。2018年，对公平性的讨论，并不仅仅限于发表一些论文或者言论。谷歌还上线了相应的课程。
[谷歌推出针对AI歧视的新课程！60分钟的ML公平自学训练模块 | 资源​mp.weixin.qq.com![图标](https://pic3.zhimg.com/v2-2d9ceed78978badf52f685b50ced44c6_ipico.jpg)](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247506252%26idx%3D3%26sn%3D88fd5777626f8828d6f2351668d65d46%26chksm%3De8d06e3edfa7e728889ae2dd15a947d087344b86bebd01e38962b497463b8dbd35671b015918%26scene%3D21%23wechat_redirect)
其次是可解释性和因果关系。因果关系之所以重新成为了人们关注的焦点，主要是因为图灵奖得主、贝叶斯网络之父Judea Pearl出版了《The Book of Why》一书，在Twitter上引发了关于因果关系的大讨论。



![](https://pic3.zhimg.com/v2-0d4add2feb097517254bc50b6b0feebe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='572'></svg>)



而且，ACM Recsys上获得最佳论文奖的论文，也探讨了如何在嵌入中包含因果关系的问题。

讨论也不仅仅限于学界，大众媒体《大西洋月刊》也发表文章指出，这是对现有人工智能方法的“挑战”。

虽然因果关系引发了不少的讨论，但也有许多学者认为，从某种程度上来说， 因果关系其实分散了人们对理论的关注，应该关注更加具体的问题，比如模型的可解释性。

其中最具代表性的，就是华盛顿大学Marco Tulio Ribeiro等人发表的论文，这篇论文是对著名的LIME（一种解释任何机器学习分类器的预测的技术）模型的跟进。

论文链接：
[https://homes.cs.washington.edu/~marcotcr/aaai18.pdf​homes.cs.washington.edu](https://link.zhihu.com/?target=https%3A//homes.cs.washington.edu/~marcotcr/aaai18.pdf)
## **深度学习在NLP领域大放异彩**

2018年，深度学习依旧受到了质疑。CMU学者Simon DeDeo在Twitter上猛烈炮轰Google Brain团队，称这技术（机器学习）现在所做的事情，跟1990年没什么差别，顶多就是规模更大，但并没有给我们带来比20年前更深刻的见解。
[“Google只认钱！机器学习20年没进步”，CMU学者炮轰AI第一大厂​mp.weixin.qq.com![图标](https://pic4.zhimg.com/v2-98953b3f2fbb01d771291f3117ae0e5f_180x120.jpg)](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247501243%26idx%3D1%26sn%3D1e53cac9c794163cb0107a3e82d35a8f%26chksm%3De8d072c9dfa7fbdf73f8c343becb9547789c7dfbd3f2fc3ac4503ed348953a06d26e24aa4f62%26scene%3D21%23wechat_redirect)
Amatriain说，深度学习等技术并没有止步不前，还有很多领域没有运用相关的技术。具体来说，深度学习在计算机视觉之外的领域取得了前所未有的成功。

最为突出的就是NLP领域。谷歌的Smart Compose（Gmail中智能预测拼写神经网络）和Duplex对话系统（会打电话的AI），可以说是2018年最令人印象深刻的两个AI应用了。



![](https://pic1.zhimg.com/v2-929d274738949588112c61aaa82cb564_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='665'></svg>)



NLP领域的进展，也不仅仅只体现在应用上。在语言模型上也有了很大的进步。最大的功臣是Fast.ai的UMLFit，推广了相关的概念与想法。

然后是其他的方法，比如艾伦研究所的ELMo、OpenAI的Transformers、谷歌最近的BERT等等，都取得了非常好的效果。

它们提供了即用型的预训练和通用模型，可以针对特定任务微调。因此，这些模型的出现，也被描述为“NLP的Imagenet时刻”。

除了这些之外，还有其他一些进步，比如Facebook的多语言嵌入。而且，我们也看到了这些方法被整合到通用的NLP框架中的速度变得非常快了，比如AllenNLP或Zalando的FLAIR。

关于NLP领域的总结，还有一篇文章，推荐给你阅读：
[不只有BERT！盘点2018年NLP令人激动的10大想法​mp.weixin.qq.com![图标](https://pic3.zhimg.com/v2-2d9ceed78978badf52f685b50ced44c6_ipico.jpg)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247510739%26idx%3D2%26sn%3D3b1d8c968712ba9b5a8f8280dfa86697%26scene%3D21%23wechat_redirect)
## **框架之争愈发激烈，强化学习成焦点**

最令人惊讶的一点是，就在Pytorch 1.0发布的时候，Pytorch似乎快要赶上了TensorFlow。

虽然在生产的过程中使用Pytorch仍旧不太理想，但在可行性、文档和教育方面，Pytorch已经超过了TensorFlow。

这其中，选择Pytorch作为实现Pytorch库的框架可能起到了很大的作用。

谷歌也已经意识到了这一点，也正在朝着这个方向努力，将Keras纳入框架，并吸纳Paige Bailey这样的开发者领袖加入其中。



![](https://pic1.zhimg.com/v2-fb432d366c84108f476998919a788eb4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='602' height='314'></svg>)



虽然今年强化学习领域的进展比不上前些年，只有DeepMind最近的IMPALA还算令人印象深刻。但基本上AI领域所有的“玩家”都发布了强化学习框架。


谷歌发布了Dopamine框架，DeepMind发布了有点竞争性的TRFL，Facebook当然不会落后，发布了Horizon，微软则发布了TextWorld，专门用于训练基于文本的智能体。

希望这些开源工具的出现，强化学习能在2019年有更多的突破进展。

此外，框架方面还有一个有趣的进展。谷歌最近发布了基于TensorFlow的TFRank。排序是一个非常重要的ML应用，它应该得到更多的关注。
[谷歌开源TF-Ranking可扩展库，支持多种排序学习​mp.weixin.qq.com![图标](https://pic3.zhimg.com/v2-2d9ceed78978badf52f685b50ced44c6_ipico.jpg)](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247509651%26idx%3D4%26sn%3D5aaabea252f98da82d5d129219118f07%26chksm%3De8d011e1dfa798f74bf43467dbf39bdb8c4e4efa229eea4eec7c3b62de26d1b958ef82e9f591%26scene%3D21%23wechat_redirect)
## **其他一些进展**

围绕着数据改进，深度学习领域仍然有非常有趣的进展。

比如说，对于深度学习非常关键的数据扩充（data augmentation）在今年有了新的进展。谷歌发布了auto-augment，一种深度强化学习方法，可以自动扩充训练数据。

一个更加极端想法是用合成数据训练深度学习模型，许多人都认为这是AI未来发展的关键。英伟达在《Training Deep Learning with Synthetic Data》论文中提出了一些新的想法。



![](https://pic2.zhimg.com/v2-13b8dc08a77d990c80469e66c3367b31_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='485'></svg>)



论文链接：
[Training Deep Networks with Synthetic Data: Bridging the Reality Gap by Domain Randomization​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.06516)
在《Learning from the Experts》一文中，展示了如何使用专家系统合成数据。



![](https://pic4.zhimg.com/v2-e21f04885320343ea20f92de929769ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='399'></svg>)



论文链接：

[Learning from the experts: From expert systems to machine-learned diagnosis models​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.08033)
最后，还有一种方法是“weak supervision”，可以减少对大量手工标注数据的需求。Snorkel是一个非常有趣的项目，想要提供了一个通用的框架，来推进这种方法。

项目地址：
[Snorkel: rapid training data creation with weak supervision​blog.acolyer.org![图标](https://pic3.zhimg.com/v2-63cf04c814f8628485fc0853bd2f261e_180x120.jpg)](https://link.zhihu.com/?target=https%3A//blog.acolyer.org/2018/08/22/snorkel-rapid-training-data-creation-with-weak-supervision/amp/%3F__twitter_impression%3Dtrue)
Amatriain说，就AI领域更为基础的突破，今年并没有看到太多。

但他不同意Hinton的看法，即认为缺乏创新是因为这个领域年轻人太多，资深的人太少。

在他看来，缺乏突破的主要原因是，现有的方法仍旧有许多地方可以应用，因此很少有人去冒险近尝试不切实际的想法。尤其是当前大多数研究都是由大公司资助的，让这一特点更加突出了。

不过，还是有一些人在尝试，代表性的论文有两篇。
![](https://pic2.zhimg.com/v2-1dfd8a83ef5874c0d934e082d1ef2689_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='308'></svg>)



论文链接：



[https://arxiv.org/pdf/1803.01271.pdf​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1803.01271.pdf)



虽然这篇论文是高度实验性的，并且使用的是已知的方法，但它打开了新方法的大门。因为它证明了现有的最佳方法，并不是最好的。



![](https://pic2.zhimg.com/v2-12e4a36d7a9acc31e36b86e63ff101c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='428'></svg>)



论文链接：
[Neural Ordinary Differential Equations​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1806.07366)
这篇论文是最近NeurIPS最佳论文获得者，它挑战了深度学习中的一些基本内容，包括层本身的概念。

最后，向你推荐量子位此前发布的文章：
[一文看尽2018全年AI技术大突破​mp.weixin.qq.com![图标](https://pic3.zhimg.com/v2-2d9ceed78978badf52f685b50ced44c6_ipico.jpg)](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247510685%26idx%3D1%26sn%3D215d8b68c6e2c5eb58eb171ee0cea167%26chksm%3De8d01defdfa794f9a8266298cad8db9f4c28fd911860cbd1813f158fd24f45837d6bf14e4d52%26scene%3D21%23wechat_redirect)
— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态
[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)

**诚挚招聘**

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。


