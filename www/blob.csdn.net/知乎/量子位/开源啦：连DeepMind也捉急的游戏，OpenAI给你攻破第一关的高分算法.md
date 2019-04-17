# 开源啦：连DeepMind也捉急的游戏，OpenAI给你攻破第一关的高分算法 - 知乎
# 



> 圆栗子 发自 凹非寺 
量子位 出品 | 公众号 QbitAI

*写作“很好奇”，读作“不怕死”。*
![](https://pic3.zhimg.com/v2-2a071ac734a4d75c7d1a0f1fac1de606_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='384' height='338'></svg>)△ 佛系操作
打游戏的时候，有些平淡的操作，**还没执行就看得到结果**。

比如玩马里奥，可以不去踩敌人，一路往前跳。

可是，没试过怎么知道，哪些敌人可以踩？

一扇门，不推开怎么知道里面是什么？
![](https://pic4.zhimg.com/v2-11297f89627d34d02e250fd616071aff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='288'></svg>)△ 就是死，也要进去看一眼 (来自山下智博)
人类玩家有好奇心，大概AI也要有好奇心比较好。

**OpenAI**想要鼓励强化学习AI，去**探索未知的世界**，不要局限在已知的舒适区。

于是，团队推出了一种方法，叫做“随机网络蒸馏 (Random Network Distillation, **RND**) ”，专注培养AI的好奇心：隐藏房间什么的，只有好奇的AI才能发现。

当然，这方法不止用来打马里奥，不然就屈才了。
![](https://pic3.zhimg.com/v2-c03fd67fcea6599ae377500c49759946_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='292' height='288'></svg>)△ 蒙特祖玛的复仇
用RND加持的算法打**蒙特祖玛的复仇 **(最难的雅达利游戏，可称强化学习AI的噩梦)，智能体逃出了第一关的**全部24个房间**，成绩远远超过人类的平均分数 (4.7k) ，以及现有最前沿的算法。

## **鼓励探索，当然是用高额奖励**

RND是一种**基于预测**的方法。

给每一个备选的动作，预测一下结果：

如果，结果**非常容易预测**，奖励分就偏低。

结果越**难预测**，就表示越“未知”，奖励分也越高。



![](https://pic4.zhimg.com/v2-c47fc21cbb30e1bd673b379c0ad3965b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)△ 第一次离开家，并不知道外面的危险 (还是来自山下智博)



重赏之下，AI探索新世界的意愿就会更强了。

不过，预测结果有个难点，叫做嘈杂电视问题 (**Noisy-TV Problem**) ：

举个栗子，在一个迷宫游戏里，摆一台“**电视**”，播放随机频道。



![](https://pic3.zhimg.com/v2-4119345b8f95572996f361970ba99666_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='320'></svg>)



走到电视里面前，智能体就停下来不走了。



![](https://pic4.zhimg.com/v2-ff2efb32541fb0356f3874aa86b97613_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='320'></svg>)



一旦没有了“电视”，智能体又能正常玩耍了。

是预测受到了干扰。

为了避免这样的干扰，团队定义了预测误差的**三个因素**：

> **一号因素**，预测误差很高，预测器无法从之前看到的例子中泛化。后面的经历会受到高预测误差的影像。
**二号因素**，预测误差很高，因为预测目标是随机 (Stochastic) 的。
**三号因素**，预测误差很高，因为缺少必要信息，或者预测器模型的局限性太大，无法适应复杂的目标函数。

OpenAI团队判断，一号是必要的，因为它把**新颖程度**量化了，二号和三号则需要竭力避免。

RND就是为了避免这两个因素，而诞生的**探索奖**。
![](https://pic1.zhimg.com/v2-684e3f5f27cd7315843d4741bed5f7ec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1058' height='1176'></svg>)
上图是**传统预测**和**RND预测**的对比。

避免二号因素，就要让神经网络给出**确定性**的答案，而不是给出多个答案和它们各自的可能性；

避免三号因素，就要选择和目标网络相同的架构。
![](https://pic2.zhimg.com/v2-9c962f856060be0f9f1911e6d4a3b715_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
对蒙特祖玛的复仇来说，这样特殊的奖励机制尤其必要：

如果是一般的游戏，简单的探索策略就够用了，但蒙特祖玛里面，除了拿到钥匙有奖励、碰到骷髅会死之外，**其余都是0**，AI很难感受到游戏规则，也不易学到有用的经验。

加重**探索奖**，才能让AI**更加理解游戏**。

团队表示，RND不止蒙特祖玛的复仇适用，马里奥等等其他游戏也适用。以及，越过游戏范畴，更加广泛的强化学习AI也都可以用这个方法，让智能体对新鲜事物更好奇。

## **好奇的宝宝成绩好**

从最简单的雅达利**打砖块**开始观察。
![](https://pic3.zhimg.com/v2-8140bda652493aa14dff3485cd70c646_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='240'></svg>)△ 砖块排列发生变化，奖励就会达到峰值
> **内在奖励**，指探索奖；
**外在奖励**，指游戏中直接体现的奖励，如游戏分值。

每当智能体打下一块砖，砖块有了**新的排列格式**，内在奖励就会达到峰值 (训练伊始) 。

当它**首次通过第一关**的时候，内在奖励也达到了峰值 (训练数小时后) 。




再看超级马里奥。
![](https://pic2.zhimg.com/80/v2-3678cdb56480e140085f2c1e73cd6b69_b.jpg)https://www.zhihu.com/video/1043232951403794432
**内在目标**与**外在目标**已经基本一致了。

智能体通过了11关，找到了许多隐藏的房间，并**打败了库巴大魔王**。




接下来，就是蒙特祖玛的复仇。
![](https://pic2.zhimg.com/80/v2-1ad1bb42e594e7d8d41ff59b3bff99b9_b.jpg)https://www.zhihu.com/video/1043233169637707776
最好成绩，当然是**24个房间**都攻破，顺利通过第一关，得分**17.5k**。

并且，多数情况下，智能体都能解锁**20-22个房间**，得分**14.5k**。

对比一下，人类平均分是**4.7k**。

## **有代码，有论文**

现在，OpenAI已经把RND开源了，可以从传送门前往瞻仰：
[openai/random-network-distillation​github.com![图标](https://pic2.zhimg.com/v2-03008cb2c4b3ce7e3dc36e94875fde61_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/openai/random-network-distillation)
还有，论文也公开了：
[https://arxiv.org/pdf/1810.12894.pdf​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1810.12894.pdf)
博客在这里：
[https://blog.openai.com/reinforcement-learning-with-prediction-based-rewards/​blog.openai.com](https://link.zhihu.com/?target=https%3A//blog.openai.com/reinforcement-learning-with-prediction-based-rewards/)![](https://pic3.zhimg.com/v2-dda54296d4f84f044c89728413553896_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='288'></svg>)△ 吃蘑菇，变成弹簧 (依然来自山下智博)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


