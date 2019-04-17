# 怎样三天训练出AI围棋大师？教你AlphaGo Zero的3个trick - 知乎
# 



> 原作 Seth Weidman 
夏乙 问耕 编译自HackerNoon 
量子位 出品 | 公众号 QbitAI

过去一年，AI领域最exciting的进展可能要数AlphaGo的不断进步。AlphaGo Zero不仅提出了新的网络架构，也带来了一些新的神经网络训练技巧。

虽然DeepMind发表了论文，并在Reddit论坛上进行了一次公开答疑，后来还有人将AlphaGo Zero的算法实现了出来，但如何训练？其中有哪些trick？

发表在HackerNoon上的一篇最新博客文章做出了直观的解读：

先从AlphaGo各个版本一脉相承的两种方法说起：一是前瞻的蒙特卡洛树搜索，二是凭“直觉”来对落子位置进行评估，也就是DeepMind所说的策略网络和价值网络。这两种方法结合起来，构成了每一版AlphaGo的核心。
![](https://pic3.zhimg.com/v2-f5344889d72bd9bdce441ce6df5de8d6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='605'></svg>)
从更高的层面讨论，AlphaGo Zero的工作方式和AlphaGo差不多，它们都使用了基于MCTS的前向搜索，并辅以神经网络的指导。然而，AlphaGo Zero的神经网络，或者说它的“直觉”，跟AlphaGo的训练方式完全不同。


以下是三个你需要知道的tricks。
![](https://pic2.zhimg.com/v2-3bf0aa432959c030cc0b6df5424a16e9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='356'></svg>)
**Trick 1：如何训练AlphaGo Zero**

所谓神经网络学会下围棋，就是能在当前的局面下，判断出下一步的最佳落子位置。

DeepMind发现，无论神经网络智能程度如何，从一无所知到围棋大师，使用MTCS总是更好的评估判断方案。

从根本上来说，MCTS是一种前向搜索，如果有足够的时间，人类围棋大师也能完成。这个过程无非是不断脑补棋局未来的变化，评估哪些“套路”可能是最佳方案。

举个例子。第一种落子方案，最终可能导致平局，评估得分就是0.0。接着看第二种方案，神经网络推算出这么下可能会获胜，评估得分为0.5。
![](https://pic1.zhimg.com/v2-3eb8954513ad3c96654b0e38a674e8fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='922'></svg>)
这个过程一直持续进行的话，前向搜索总是能够评估出更好的落子方案。

当然，这也要求神经网络能够判断如何下棋落子能带来胜利。基于MCTS的改进评估与神经网络的当前状态，智能体Agent不断地展开自我对局。

DeepMind用来训练AlphaGo Zero的数据，全部都是通过前向搜索和自我博弈产生的。
![](https://pic2.zhimg.com/v2-306aadaaffe5d31d7d62c1fd5a629349_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='914' height='330'></svg>)
不使用人类下棋的数据，其实是一个trick。这样的话，对于每个给定的局面，神经网络都能通过执行基于MCTS的前向搜索，再用其提高智能体的棋力。

在这种方法的帮助下，AlphaGo Zero从一无所知成长为围棋大师。

**Trick 2: 双头怪**

AlphaGo Zero的神经网络，是一个“双头”架构。

这个网络的前20层左右，是常见的神经网络结构。然后跟着是两个“头”，一个头取走了前20层的输出，然后产生了下一步的落子概率，另一个头基于同样的数据，输出当前局面的获胜概率。
![](https://pic2.zhimg.com/v2-f989b573f51f40571c65114094840439_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='438' height='688'></svg>)
这是个不寻常的结构。几乎所有的应用中，神经网络都只给出一个固定的输出。如果有两个不同的输出结果，应该如何学习呢？

答案很简单。请记住，神经网络的本质只是一些数学函数，包括一系列影响决策的参数。所谓训练，就是反复向神经网络展示正确答案，让网络更新参数，使得输出更接近正确答案。

所以，当我们使用一个头进行预测时，只需要更新“身体”和“头1”中的参数。而使用另一个头时，更新“身体”和“头2”中的参数。
![](https://pic3.zhimg.com/v2-6672f17645505c78b0f915a7db4b81ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='471'></svg>)![](https://pic1.zhimg.com/v2-0965153e139e6bcf81fff7ab35d4c318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='492'></svg>)
这就是DeepMind训练单一、双头神经网络的方法，并且用其来指导MCTS的搜索。有点像AlphaGo使用了两个单独的神经网络。这个trick在技术上被称为硬参数共享的多任务学习（Multi-Task Learning with Hard Parameter Sharing）。

**Trick 3: 残差网络**

AlphaGo Zero还用了比之前版本更前沿的神经网络架构：残差网络。

残差网络是2015年孙剑在微软亚洲研究院带领的团队开发的，当时AlphaGo第一版的工作已经快要接近尾声。
![](https://pic4.zhimg.com/v2-cfe57d27a4dd786958b262973dde2ad3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='810' height='1804'></svg>)△ 残差网络和普通卷积神经网络的比较
这两种神经网络相关的技巧——用残差网络架构替代卷积架构、用“双头怪”神经网络替代各个单独的神经网络，都能让整个网络的性能增加一倍，两者结合起来，性能又有提升。如下图所示：
![](https://pic3.zhimg.com/v2-1732e056a6e9184a4f025f651bc7470a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='820' height='1212'></svg>)
**小结**

上面三个tricks让AlphaGo Zero展现出令人难以置信的能力。

值得注意的是，AlphaGo没有使用任何经典或者前沿的强化学习概念，没有Deep Q Learning，也没有Asynchronous Actor-Critic Agents。

DeepMind只是使用了模拟的方法为神经网络产生数据，然后以监督的方式学习。

Denny Britz曾经总结说：具有讽刺意味的是，强化学习在过去几年里的主要发展，都让强化学习看起来不像强化学习，反而更像监督学习。

**手把手训练AlphaGo Zero**

1、初始化神经网络

2、让神经网络开始自我对弈，每一步进行1600次MCTS模拟，大概耗时0.4秒
![](https://pic3.zhimg.com/v2-423a15d62161a57b0ee93a96598fd232_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='382'></svg>)
3、随着对弈数量的增长，从最近的50万场比赛中，采样2048个落子决定，以及比赛对应的输赢情况。

4、使用MCTS前向搜索产生的落子评估，以及输赢结果，一起对神经网络进行训练。

5、步骤3、4每进行1000次迭代，评估当前神经网络与此前最佳版本。如果新版本胜率达到55%，就开始用其生成新的游戏，而不再用此前的版本。

重复3、4步70万次，自我对局不要停，三天后，你也能训练出一个AlphaGo Zero。

**对AI未来的启示**

DeepMind的成就，对于人工智能研究的未来有很多启示，以下是关键几点：

**第一**，通过模拟产生的自我对弈数据对于神经网络的训练来说，已经足够好了。这证明了**模拟的自我对局数据能够训练AI智能体完成极其复杂的任务，超越人类表现**，甚至能够完全从0开始，根本不需要人类专家提供的数据。

**第二，为了让智能体在多个领域学习执行几个相关任务，“双头”的trick能提供很大帮助。**这似乎可以防止智能体的行为在任何单个任务上过拟合。DeepMind似乎非常喜欢这个trick，还用它的高级版本构建了可以在多个不同领域学习多个任务的智能体。
![](https://pic2.zhimg.com/v2-003774659c732f640df267552b6e972d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='722' height='718'></svg>)△ DeepMind的AI用多任务强化学习框架学习走迷宫
很多机器人项目，特别是用模拟环境训练机器人使用四肢完成任务的领域里，用这两种技巧取得了不错的效果。Pieter Abbeel在NIPS上的最新演讲展示了很多令人印象深刻的新成果，这些成果用了很多前沿强化学习技术加上了这些trick。

实际上，机器人的运动对于“双头”来说是一个完美的使用场景，比如说，要教机器人打棒球，其实包含两类动作，一是握住、挥动球棒，二是击打一个运动中的物体。用“双头”，两种动作的训练就可以同时完成，因为这二者包含一些共同技能，比如平衡、转体等等。
![](https://pic2.zhimg.com/v2-0acc58fdf47ec546d86745791f0e5dbd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)△ DeepMind将训练AlphaGo Zero的技巧用到了机器人运动上
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


