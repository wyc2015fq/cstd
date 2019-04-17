# 第二弹！谷歌大脑2017总结下篇：Jeff Dean梳理6大领域研究 - 知乎
# 



> 李杉 维金 编译自 Google Blog
量子位 出品 | 公众号 QbitAI

传奇一般的Jeff Dean今天发布了谷歌大脑2017总结的第二弹。

在这篇总结中，Jeff Dean详细论述了谷歌大脑过去一年的AI应用研究，例如如何将机器学习等技术应用于医疗、机器人、创意、公平等等多个领域。

这在某种程度上，也代表了2017人工智能具体应用的最高水平研究。

**昨天量子位推送了[这份总结的上篇](https://zhuanlan.zhihu.com/p/32873102)，以下是第二弹：**

在这篇文章中，我们将深入到谷歌大脑在多个特定领域所做的研究。

## **医疗**

我们认为，机器学习技术在医疗行业的应用潜力巨大。我们正在解决各种各样的问题，包括协助病理学家**检测癌症**，**理解各种对话**来为医生和病人提供帮助，使用机器学习**解决基因组学中的各种问题**，其中包括一个名叫DeepVariant的开源工具，用深度神经网络来从DNA测序数据中快速精确识别碱基变异位点。

癌症检测报道：[Google用深度学习做癌症病理检测，准确率达89％](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247484475%26idx%3D2%26sn%3Db365b8b2ce589976f1eccf5e9f35e2eb%26chksm%3De8d3b349dfa43a5f9a3219c7a6ed3f8f7c27dc6b6ce45098c43623a31744087997a4cac4a0e3%26scene%3D21%23wechat_redirect)

相关论文：
- Detecting Cancer Metastases on Gigapixel Pathology Images [https://drive.google.com/file/d/0B1T58bZ5vYa-QlR0QlJTa2dPWVk/view](https://link.zhihu.com/?target=https%3A//drive.google.com/file/d/0B1T58bZ5vYa-QlR0QlJTa2dPWVk/view)

理解对话论文：
- Speech recognition for medical conversations
[https://arxiv.org/abs/1711.07274](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1711.07274)

基因检测报道：[谷歌推出开源工具DeepVariant](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247491757%26idx%3D4%26sn%3D79e9a7414d84d44442c56ed1549b5ac8%26chksm%3De8d057dfdfa7dec9686dcc89adc202fd9003ccef69e07e2b0287d7386190e532826aeec2304a%26scene%3D21%23wechat_redirect)
- 代码：[https://github.com/google/deepvarian](https://link.zhihu.com/?target=https%3A//github.com/google/deepvarian)
![](https://pic1.zhimg.com/v2-30c3efa429b569fb3a9fa16cb865c4d0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='508' height='640'></svg>)
我们还致力于尽早发现糖尿病视网膜病变(DR)和黄斑水肿，并于2016年12月在《美国医学协会杂志》(JAMA)上发表论文。

相关论文：
- Development and Validation of a Deep Learning Algorithm for Detection of Diabetic Retinopathy in Retinal Fundus Photographs
[https://jamanetwork.com/journals/jama/article-abstract/2588763](https://link.zhihu.com/?target=https%3A//jamanetwork.com/journals/jama/article-abstract/2588763)

2017年，我们将这个项目从研究阶段过渡到实际的临床影响阶段。我们与Verily（Alphabet旗下的一家生命科学公司）合作，通过严格的流程来引导这项工作，我们还一起将这项技术整合到尼康的Optos系列眼科相机中。

此外，我们在印度努力部署这套系统，因为印度的眼科医生缺口多达12.7万人，因此，几乎一半的患者确诊时间过晚，并因为这种疾病而导致视力下降。作为试点的一部分，我们启动了这个系统，帮助Aravind Eye Hospitals眼科医院的学生更好地诊断糖尿病眼病。
![](https://pic3.zhimg.com/v2-c6371959b6c7cef10e504e18b400305a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='392'></svg>)
[Verily还可以通过视网膜图像用AI检测心脏病](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247488638%26idx%3D2%26sn%3D02f3334d5514ef016a309c6a4f75f0ea%26chksm%3De8d3a30cdfa42a1a87ebfc67790d068df202eb454e6e0188eb98e2c6c9687994ea75e0c9bc42%26scene%3D21%23wechat_redirect)

我们还与合作伙伴共同了解影响糖尿病眼睛护理的人类因素，从患者和医疗服务提供者的人种学研究，到研究眼科医生如何与人工智能系统之间的互动方式。

我们也与领先的医疗组织和医疗中心的研究人员（包括斯坦福大学、加州大学旧金山分校和芝加哥大学），共同演示机器学习利用匿名病历来预测医疗结果所能达到的具体效果（例如，考虑到病人的现状，我们相信可以用针对其他数百万病人的病程进行的研究来预测这个病人的未来，以此帮助医疗专业人士做出更好的决策）。我们对这项工作感到非常兴奋，我们期待着在2018年告诉你更多与之有关的事情。

## **机器人**

我们在机器人领域的长期目标是设计各种学习算法，让机器人在混乱的现实环境中运行，并通过学习快速获得新的技能和能力。而不是让它们身处精心控制的环境中，处理当今机器人所从事的那些为数不多的手工编程任务。

我们研究的一个重点是开发物理机器人的技术，利用他们自己的经验和其他机器人的经验来建立新的技能和能力，分享经验，共同学习。我们还在探索如何将基于计算机的机器人任务模拟与物理机器人的经验结合起来，从而更快地学习新任务。

相关博客：[https://research.googleblog.com/2017/10/closing-simulation-to-reality-gap-for.html](https://link.zhihu.com/?target=https%3A//research.googleblog.com/2017/10/closing-simulation-to-reality-gap-for.html)

虽然模拟器的物理效果并不完全与现实世界相匹配，但我们观察到，对于机器人来说，模拟的经验加上少量的真实世界经验，比大量的实际经验更能带来更好的结果。

除了真实世界的机器人经验和模拟的机器人环境，我们还开发了机器人学习算法，可以学习通过观察人类的演示进行学习。我们相信，这种模仿学习模式是一种非常有前途的方法，可以让机器人快速掌握新的能力，不需要明确编程或明确规定一个活动的具体目标。

相关报道：[谷歌教机器人理解语义，像人一样学习复杂技能](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247487317%26idx%3D3%26sn%3D6b3e51158a23afa75750e32bd2f12bd3%26chksm%3De8d3b827dfa43131eda364056557340ce93d219474eba26c3ef6140ba1f272cfe9fc004d3701%26scene%3D21%23wechat_redirect)

论文及项目：
- Unsupervised Perceptual Rewards for Imitation Learning
[https://sermanet.github.io/rewards/](https://link.zhihu.com/?target=https%3A//sermanet.github.io/rewards/)
- Time-Contrastive Networks: Self-Supervised Learning from Multi-View Observation
[https://sermanet.github.io/tcn/](https://link.zhihu.com/?target=https%3A//sermanet.github.io/tcn/)
- End-to-End Learning of Semantic Grasping
[https://arxiv.org/abs/1707.01932](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1707.01932)

例如，机器人会从不同的角度观察人类执行任务的过程，然后努力模仿他们的行为，从而在15分钟的真实世界体验中学会用杯子倒水。就像教自己3岁的孩子一样，我们可能会给予它鼓励，告诉它只洒出来一点水。

我们还共同组织并主持了11月召开的第一届Conference on Robot Learning （CoRL），汇集了在机器学习和机器人技术的交叉领域工作的研究人员。这次会议的总结包含了更多的信息，我们很期待明年在苏黎世召开的会议。

会议总结：[https://research.googleblog.com/2017/12/a-summary-of-first-conference-on-robot.html](https://link.zhihu.com/?target=https%3A//research.googleblog.com/2017/12/a-summary-of-first-conference-on-robot.html)

## **基础科学**

我们也很看好机器学习技术解决重要科学问题的长期潜力。去年，我们利用神经网络预测了量子化学中的分子性质。

相关论文：
- Machine learning prediction errors better than DFT accuracy
[https://arxiv.org/abs/1702.05532](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1702.05532)
- Neural Message Passing for Quantum Chemistry
[https://arxiv.org/abs/1704.01212](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1704.01212)

通过分析天文数据发现了新的系外行星。

相关博客：[https://www.blog.google/topics/machine-learning/hunting-planets-machine-learning/](https://link.zhihu.com/?target=https%3A//www.blog.google/topics/machine-learning/hunting-planets-machine-learning/)

对地震的余震进行预测，并利用深度学习来指导自动证明系统。

相关论文：
- Deep Network Guided Proof Search
[https://arxiv.org/abs/1701.06972](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1701.06972)
![](https://pic2.zhimg.com/v2-b3b3cd5e62716a2a9de0d0278c495d79_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='389' height='235'></svg>)![](https://pic4.zhimg.com/v2-1c7c4339a04b243ca5541b30efb73f03_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='198'></svg>)
## **创意**

我们也很感兴趣，如何利用机器学习技术去协助创意活动。2017年，我们开发了一个人工智能钢琴二重奏工具。

相关报道：[Google推出一个弹钢琴的AI](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247484194%26idx%3D3%26sn%3D905644876c071bd5f48d08adcb1beb8e%26chksm%3De8d3b450dfa43d4615d978024916fdf2fc39055d4934f75c0ba9d81e33fad27b9d48c0341937%26scene%3D21%23wechat_redirect)

弹琴地址：
[https://experiments.withgoogle.com/ai/ai-duet/view/](https://link.zhihu.com/?target=https%3A//experiments.withgoogle.com/ai/ai-duet/view/)

帮助YouTube音乐人Andrew Huang制作了新的音乐。

对这个感兴趣，可以前往YouTube观看：
[https://youtu.be/AaALLWQmCdI](https://link.zhihu.com/?target=https%3A//youtu.be/AaALLWQmCdI)

并展示了如何教机器画画。

相关报道：[怎样教机器学会画画和抽象“算术”？](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247485157%26idx%3D1%26sn%3Deaedde43600714e47b873031663486f2%26chksm%3De8d3b197dfa438810578f60a933162e90e155ebdffaffd77f6cdacd963bbb7a5250ce42bbcda%26scene%3D21%23wechat_redirect)

相关论文：

A Neural Representation of Sketch Drawings
[https://arxiv.org/abs/1704.03477](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1704.03477)

画画地址：
[http://autodraw.com](https://link.zhihu.com/?target=http%3A//autodraw.com)
![](https://pic2.zhimg.com/v2-c6cd9de48fc4e1106f39d80a1dec5a2d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)
我们还演示了，如何控制运行在浏览器中的深度生成模型，制作新的音乐。这项工作赢得了NIPS 2017的“最佳演示奖”，这也是谷歌大脑团队Magenta项目的成员连续第二年赢得这个奖项。

Demo地址：
[https://deeplearnjs.org/demos/performance_rnn/index.html](https://link.zhihu.com/?target=https%3A//deeplearnjs.org/demos/performance_rnn/index.html)

在NIPS 2016上，来自Magenta项目的互动音乐即兴创作也赢得了“最佳演示奖”。

MusicVAE地址：
[https://colab.research.google.com/notebook#fileId=/v2/external/notebooks/magenta/music_vae/music_vae.ipynb](https://link.zhihu.com/?target=https%3A//colab.research.google.com/notebook%23fileId%3D/v2/external/notebooks/magenta/music_vae/music_vae.ipynb)

## **People + AI研究项目（PAIR）**

机器学习的进步为人类与计算机的交互带来了全新的可能。与此同时，同样重要的是让全社会从我们开发的技术中受益。我们将这方面的机遇和挑战视为高优先级工作，并与谷歌内部的许多团队合作，成立了PAIR项目（[https://ai.google/pair](https://link.zhihu.com/?target=https%3A//ai.google/pair)）。

PAIR的目标是研究和设计人类与人工智能系统互动最高效的方式。我们发起了公共研讨会，将多个领域，包括计算机科学、设计，甚至艺术等领域的学术专家和实践者聚集在一起。PAIR关注多方面课题，其中一些我们已有所提及：尝试解释机器学习系统，帮助研究者理解机器学习，以及通过deeplearn.js扩大开发者社区。关于我们以人为中心的机器学习工程方法，另一个案例是Facets的推出。这款工具实现训练数据集的可视化，帮助人们理解训练数据集。




## **机器学习的公平性和包容性**

随着机器学习在技术领域发挥越来越大的作用，对包容性和公平性的考量也变得更重要。谷歌大脑团队和PAIR正努力推动这些领域的进展。

我们发表的论文涉及，如何通过因果推理来避免机器学习系统的偏见，在开放数据集中地理多样性的重要性，以及对开放数据集进行分析，理解多元化和文化差异。我们也一直与跨行业项目Partnership on AI密切合作，确保公平性和包容性成为所有机器学习实践者的目标。

相关论文：
- Avoiding Discrimination through Causal Reasoning
[https://arxiv.org/pdf/1706.02744.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1706.02744.pdf)
- No Classification without Representation: Assessing Geodiversity Issues in Open Data Sets for the Developing World
[https://arxiv.org/abs/1711.08536](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1711.08536)



![](https://pic2.zhimg.com/v2-6bcfcd09d20dcf77078e766102c04185_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='401'></svg>)
## **我们的文化**

我们团队文化的一个重要方面在于，赋能研究员和工程师，帮助他们解决他们认为最重要的基本研究问题。9月份，我们公布了开展研究的一般方法。

相关博客：[https://research.googleblog.com/2017/09/the-google-brain-teams-approach-to.html](https://link.zhihu.com/?target=https%3A//research.googleblog.com/2017/09/the-google-brain-teams-approach-to.html)

在我们的研究工作中，教育和指导年轻研究员贯穿始终。去年，我们团队吸纳了100多名实习生，2017年我们研究论文的约25%共同作者是实习生。

2016年，我们启动了“谷歌大脑入驻”项目，给有志于学习机器学习研究的人们提供指导。在项目启动第一年（2016年6月到2017年5月），27名入驻者加入我们团队。我们在项目进行到一半时，以及结束后公布了进展，列出了入驻者的研究成果。项目第一年的许多入驻者都是全职研究员和研究工程师，他们大部分人没有参加过伯克利、卡耐基梅隆、斯坦福、纽约大学和多伦多大学等顶级机器学习研究机构的博士研究。

2017年7月，我们迎来了第二批入驻者，他们将与我们一同工作至2018年7月。他们已经完成了一些令人兴奋的研究，成果在许多研究场合发表。

详情可见：[https://research.google.com/pubs/AIResidency.html](https://link.zhihu.com/?target=https%3A//research.google.com/pubs/AIResidency.html)

现在，我们正在扩大项目范围，引入谷歌内部的许多其他研究团队，并将项目更名为“Google AI Residency program”项目。（今年项目的申请截止时间已过，可以通过链接[http://g.co/airesidency/apply](https://link.zhihu.com/?target=http%3A//g.co/airesidency/apply)了解明年的项目情况）。

2017年，我们所做的工作远远超出我们在这两篇博客中介绍的内容。我们致力于在顶级研究场合发表我们的成果。去年，我们团队发表了140篇论文，包括在ICLR、ICML和NIPS上发表的超过60篇论文。如果想要进一步了解我们的工作，你可以仔细阅读我们的研究论文。

论文详情：[https://research.google.com/pubs/BrainTeam.html](https://link.zhihu.com/?target=https%3A//research.google.com/pubs/BrainTeam.html)

你也可以在这段视频中了解我们的团队成员。

视频地址：[https://youtu.be/rsN690cfWsM](https://link.zhihu.com/?target=https%3A//youtu.be/rsN690cfWsM)
![](https://pic3.zhimg.com/v2-e86be583609db6a2f70b44825a799926_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='603'></svg>)
或是读一读我们在第二次“Ask Me Anything”活动上的回答。（你也可以看看我们2016年的同一活动。）

AMA报道：[Jeff Dean领衔全面解答AI现状与未来](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247489094%26idx%3D1%26sn%3Df603188ec42f738644bb182677358448%26chksm%3De8d3a134dfa4282257a6f6a300f6b0cced0bc752667e0b20129dd61e8069363debe938ca7206%26scene%3D21%23wechat_redirect)

谷歌大脑团队正在开枝散叶，在北美和欧洲都吸纳了成员。如果你觉得我们所做的工作听起来很有趣，想要加入我们，那么可以看看我们正在招聘的空缺职位，申请实习，参与人工智能入驻项目，拜访我们的研究员，或是通过[http://g.co/brain](https://link.zhihu.com/?target=http%3A//g.co/brain)最下方的链接去跟踪全职的研究和工程开发岗位。

你可以通过谷歌研究博客，或Twitter帐号@GoogleResearch关注我们2018年的工作，你也可以关注我的个人帐号[@JeffDean](https://link.zhihu.com/?target=https%3A//twitter.com/JeffDean)。
![](https://pic2.zhimg.com/v2-9f4373047d517c340876fb5346ba368d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='412'></svg>)
Jeff Dean还在Twitter上等待大家的反馈呢~

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


