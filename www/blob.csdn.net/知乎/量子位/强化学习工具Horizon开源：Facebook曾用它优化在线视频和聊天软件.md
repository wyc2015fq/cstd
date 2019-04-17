# 强化学习工具Horizon开源：Facebook曾用它优化在线视频和聊天软件 - 知乎
# 



> 晓查 发自 凹非寺
量子位 出品 | 公众号 QbitAI



![](https://pic2.zhimg.com/v2-ae5c0a03bc11e01804eccea347a998d5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='343'></svg>)



Facebook上周末开源了一个强化学习工具——**Horizon**。

虽然之前也有其他公司开源过强化学习工具，但Facebook声称Horizon是第一个开源的**“端到端”（end-to-end）强化学习工具**。

它由Python编写，使用Pytorch进行建模和Caffe2进行训练，用于解决一些数据集很大、反馈回路很慢的实际强化学习问题。而这些问题往往需要在现实世界中小心翼翼的进行试验，因为没有模拟器可供运行。

Facebook全球拥有20多亿用户，在这方面有丰富的经验。Horizon框架开发的工作始于2年半以前，并在去年投入公司内部使用。Facebook曾将它用在**优化网络360°全景视频**和**智能个性化推荐**等实际产品上。



![](https://pic2.zhimg.com/v2-4b4af408eaca843c89de9b5d9e116071_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='578' height='520'></svg>)



Horizon平台包含训练深度强化学习算法的工作流，像数据处理、特征转换、分布式训练、反事实策略评估、优化服务等。


## **改进的强化学习工具**

强化学习在AI领域取得了很多成就，比如人机围棋大战、AI在Dota 2团战中战胜人类这些大新闻，其中都有强化学习的功劳。



![](https://pic3.zhimg.com/v2-c49164059a23bab48b72e16600070c06_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='730' height='547'></svg>)



但是它却鲜有商业用途，原因是在游戏之外的地方，通过反复试错来学习的方法是不明智或不安全的。对于现实世界中的现象，也没有精确的模拟器可供算法进行安全的训练。


为了解决其中的一些限制，Facebook开发了Horizon工具，让公司团队在面临一些实际问题时也能使用强化学习。

Horizon最初训练算法是选取工程师指定的动作集合，而不是让算法从零开始并从随机动作中学习。然后，它使用基于现有数据的几种反事实分析来模拟算法可能采取的不同动作。通过这种方式，Horizon在模拟器中模拟训练算法，使其完善而不必担心它会在现实世界中造成严重破坏。

一般来说，使用实际的模拟器会比进行这种反事实分析更好。 但对于Facebook的许多问题，构建模拟器并非易事。目前Facebook团队正在研究如何从数据集构建模拟器。

一旦算法运行良好，Horizon就允许用户进行小规模在线实验，实时使用实际数据，然后逐步将新算法推广到更大的用户或数据集。再将完全训练的算法作为新的起点，不断重复此过程。

## **Facebook为何选择开源**

谈到为何将Horizon开源，Facebook应用机器学习主管Srinivas Narayanan说：“我们致力于开源，因此共享这个最新的产品化系统是很自然的事情。”

原本Horizon是为企业和研究团队大规模部署AI而开发的，它需要数以千计的CPU或GPU数十亿的观测结果。现在经过Facebook用Apache Spark预处理和用PyTorch训练系统后，它也能用在个人电脑上了。

Horizon项目负责人Jason Gauci认为强化学习是在采用ML的工业领域的下一个前沿，希望开源这个平台让用户开始使用强化学习。Horizon是为了规范大型数据集的训练，这也是强化学习中的常见问题，他说。

另外一些AI公司，如果DeepMind、谷歌大脑团队和OpenAI，也在之前开源过强化学习工具。Facebook此举或是紧随竞争对手的步伐。

## **附录**

项目地址：
[https://github.com/facebookresearch/Horizon](https://link.zhihu.com/?target=https%3A//github.com/facebookresearch/Horizon)


项目白皮书：

[https://research.fb.com/publications/horizon-facebooks-open-source-applied-reinforcement-learning-platform/](https://link.zhihu.com/?target=https%3A//research.fb.com/publications/horizon-facebooks-open-source-applied-reinforcement-learning-platform/)




— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


