# 让AI自动调参，才是百度无人车的进化方式：适应环境全靠自己 - 知乎
# 



> 车栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-b7d7cf3d5fecb6f57922099b26ca4be0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='607'></svg>)
自动驾驶汽车，需要应对各式各样的路况，工作**环境**是每时每刻在**变化**的。

所以，训练好**L4**级的自动驾驶系统**并不简单**。需要依赖**奖励函数** (Reward Function) 和**代价函数** (Cost Function) 。

如此一来，研究人员需要花大量精力，给**强化学习**里的这些函数**调参**。**环境**越复杂，调参的工作就越难做。
![](https://pic3.zhimg.com/v2-30b749f566993569d59e15aba049a10a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1004' height='352'></svg>)
不过，**百度**自动驾驶部门的人类，想要解放双手，将**调参**重任托付给**AI**自己。

于是，他们开发了**自动调参**方法，让AI能够用更短的训练时间，获得应对**复杂驾驶场景**的能力。

划重点：**快速适应多种环境**。

## **离线调参更安全**

自动驾驶汽车，需要能应付各种场景的AI系统。
![](https://pic4.zhimg.com/v2-21f1dc9f281ac862d4d54536e360de3b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)
这个**动作规划系统**，是基于百度**Apollo自动驾驶框架**研发的。

系统是**数据**驱动的，用到的数据包括**专家驾驶数据**和**周围环境数据**。
![](https://pic2.zhimg.com/v2-dad0ec25e684ab79529a4db3033df2e9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='628' height='444'></svg>)
上图可以看出，系统分为**离线**和**在线**两个部分：

> 1.**在线模块**，负责生成一条最优的**运动轨迹**，用的是**奖励函数**。
2.**离线调参模块**，才是用来生成**奖励函数**和**代价函数**的，且是可以随着环境调整的函数。

所以，第二部分是重点。要看一组参数好不好，**模拟测试**和**路测**都不可少 (如下图) 。
![](https://pic2.zhimg.com/v2-bd19dac482a8948a5f1104df042369dd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='423'></svg>)
为了减少反馈循环 (Feedback Cycles) 消耗的时间，百度用基于排名的条件逆强化学习 (Rank Based Conditional IRL) 框架，来调教奖励/代价函数，代替漫长的手动调参。

## **模型是如何炼成的**

那么，看看模型具体的样子：
![](https://pic3.zhimg.com/v2-fd458f0b9c2fe7843af3a2056efa46c2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='562'></svg>)
还是**在线**和**离线**两部分，不过可以看出这个新的强化学习调参框架 (**RC-IRL**) 所在的位置了。

## **工作流程**

原始特征生成器 (Raw Feature Generator)，从环境里获取输入，评估**采样轨迹**或者**专家驾驶轨迹**。从中选出一些轨迹，给在线模块和离线模块共同使用。

从轨迹中，把原始特征提取出来之后，在线评估器中的奖励/代价函数，会给出一个**分数**。

最后，把分数排列出来，或者用**动态规划** (Dynamic Programming) ，来选择最终输出的一条轨迹。

## **训练过程**

训练数据是从**1000+**小时的专家驾驶数据里选出来的，把**没有障碍物**又**没有车速变化**的部分剔除了，余下**7.18亿帧**，保障训练的难度。

训练过程是**离线**的，适用于**大规模测试**，也适用于处理边角案例 (Corner Cases) 。

另外，数据也是自动收集、自动标注的，又为人类节省了体力。
![](https://pic1.zhimg.com/v2-7bc6449ddd7b7ce25b67c72addca7940_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='952' height='822'></svg>)
价值函数，用**SIAMESE**网络来训练。这一部分，是用来**捕捉驾驶行为**的，依据许多**特征**来捕捉。

训练好了，就去参加测试。**模拟测试**的内容包括：停车，转弯，变线，超车以及更加复杂的场景。

模拟器之后是路测。截至今年7月25日，系统已经历了超过**25,000**英里的路测。团队说，AI到目前为止表现良好。

## **再赢一次？**

两天前，百度宣布和神州优车达成合作，一同探索自动驾驶技术的商业化。

一周前，Waymo子公司“惠摩”落户上海。

自动驾驶的赛场上，谁也不会放慢脚步。

有一天，如果Waymo无人车来了中国，不知百度能不能“再赢一次”。

论文传送门：

[https://arxiv.org/pdf/1808.04913.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1808.04913.pdf)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


