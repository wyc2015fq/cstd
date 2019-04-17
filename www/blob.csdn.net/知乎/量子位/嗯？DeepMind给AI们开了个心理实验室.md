# 嗯？DeepMind给AI们开了个心理实验室 - 知乎
# 



> 夏乙 允中 编译整理
量子位 出品 | 公众号 QbitAI

最近，DeepMind开了个心理学实验室，名叫**Psychlab**，地点就在DeepMind Lab里。

当然，不是为人类开的。
![](https://pic3.zhimg.com/v2-d1d0d511bd01d0b9d811bc3833e71772_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='637'></svg>)
Psychlab所处的DeepMind Lab是个第一人称视角3D游戏世界，这个心理学实验室当然也是个模拟环境，研究对象是其中的深度强化学习智能体（Agents）。Psychlab能够实现传统实验室中的经典心理学实验，让这些本来用来研究人类心理的实验，也可以用在AI智能体上。

关于这个实验室，DeepMind刚刚公开了一篇论文：*Psychlab: A Psychology Laboratory for Deep Reinforcement Learning Agents*，在里边详细介绍了Psychlab的环境、API，还展示了一些示例任务。
![](https://pic2.zhimg.com/v2-ec699981547fa29ba3a299c5f499c9b5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1053'></svg>)
他们在论文里说，有了Psychlab，就可以直接用认知心理学和视觉心理物理学的任务来测试，将深度强化学习智能体与人类做比较，这种比较可以**丰富我们对智能体的理解，有助于优化智能体的设计**。

那么，

**Psychlab究竟什么样？**

在Psychlab中，智能体站在一个平台上，面前摆着一个大屏幕，上面显示着刺激。这个智能体可以注视着屏幕，也可以环顾周围，看看地面看看天，都没有问题。



![](https://pic1.zhimg.com/v2-d98f4031e7beb8a65b6d3e125e295a6c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='232'></svg>)Agent在Psychlab中完成视觉搜索任务
就像上图所示，智能体的注视方向决定了画面显示的场景。①的智能体注视着屏幕中心，②和③的智能体都在搜索目标，④显示的是智能体找到了它的目标：洋红色的T。随着智能体注视方向的变化，屏幕上显示出它的不同视野。和你玩《使命召唤》时的视野变化差不多。
![](https://pic1.zhimg.com/v2-dae23109de554cbf9b244b4953fc47ac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='847' height='471'></svg>)
除了盯着屏幕找目标，智能体在这个实验室里还能干什么呢？**DeepMind自己在论文中展示了8种任务**：
![](https://pic2.zhimg.com/v2-98d44840973f5ada926372d9bce9ba75_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='393'></svg>)Psychlab中的各种任务
为了与常见的行为测试方法保持一致，所有Psychlab的任务都被分割成离散的测试。这些测试也是基本的分析单位。在DM-Lab环节可以进行任意数量的测试，试验次数、时间等都可以自行配置。所有的测试都是通过注视屏幕中心的红十字来启动的。

下面，我们来看看Psychlab中这些测试任务的实例。

**1. 持续识别**，判断面前的物品是否曾经出现过，新的就往左看，旧的就往右看。
![](https://pic2.zhimg.com/v2-7c7dc2f610d24b684f6497d641efdb91_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='484' height='360'></svg>)
**2. 任意的视觉映射**，面前出现一个物体和四个标签，标签中只有一个是绿色。下次这个物体再出现的时候，要判断出对应的绿色标签在哪个位置。
![](https://pic1.zhimg.com/v2-b8e711065b9554b5f669380098937814_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='484' height='360'></svg>)
**3. 变化检测**，判断前后出现的两组图，是否发生了变化。
![](https://pic1.zhimg.com/v2-d80ea8ee333b603c8fad37d8293f5f14_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='484' height='360'></svg>)
**4. 视力和对比敏感度测试** (Landolt C)。画面中间有一个C型环，被测试者需要指出缺口的方向，C型环会不断的变小，对比度也会不断降低。
![](https://pic2.zhimg.com/v2-eae535196f4ec9824f5f2e49ef70b861_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='484' height='360'></svg>)
**5. 玻璃图案测试**，要判断两个图案中，哪一个是同心的玻璃图案。
![](https://pic3.zhimg.com/v2-48823b6832213698056a8c35b98cacfa_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='484' height='360'></svg>)
**6. 视觉搜索**，在一堆物体中发现要寻找的目标。
![](https://pic4.zhimg.com/v2-f4ad3f98a40fc9f281f2790c44f03f67_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='484' height='360'></svg>)
**7. 随机点运动方向判断**，画面中间的圆形区域，会出现大量的随机点，你需要指出主要的运动方向。随机点运动速度特别快的时候，真是一个挑战。
![](https://pic2.zhimg.com/v2-ebc5418034c95d5545dd7eb3cb3e6c11_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='360'></svg>)
**8. 多对象追踪**。画面中有一组两种颜色的小球，随后全部变成统一颜色并开始移动，最后指定一个小球，你要判断原来这个球的颜色是什么。

其实看完这些任务，量子位很想说：

> 这……会不会有点为难AI了？

不过DeepMind在论文中说，既然深度强化学习智能体连“去找蓝色气球”这种自然语言指令都能听懂了，可见智商还不错，是时候把这些心理物理学、认知心理学领域实验方案拿出来让它们领教一下了。

为了让更多同行创造更多任务来给AI领教，**DeepMind开放了一个Psychlab API**。这个API是在lua中编写的一个简单的GUI框架，把部件（widget）放在Psychlab中的虚拟屏幕上，就可以创建任务。

这些小部件可以是任意的视觉形象，在事件发生时，比如当智能体的注视中心进入或离开小部件区域，小部件会调用回调。这个框架还支持在完成时使用定时器来调用回调。

在实验中，智能体也会获得奖励。当正确完成实验，智能体获得的奖励为1，其他步骤为0。

DeepMind建立这个“实验室”，是为了在心理学和现代人工智能之间建立一个联系点，这样，心理学找到了一种验证认知理论的新模式，而AI研究获得了更多能分离出认知核心方面的任务。

我们前面也说过，这样的研究有助于优化智能体的设计。空口无凭你大概不信，于是DeepMind又举出了一个栗子：一个Jaderberg等人2016年提出的非常厉害的**智能体UNREAL，就可以用这些心理学测试来改进。**

为了测试UNREAL智能体的视觉敏感度，DeepMind用上了上文列举的第4、5、6项测试：Landolt C。
![](https://pic2.zhimg.com/v2-c1c1031731071e4f2e1962707a297be9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='904'></svg>)UNREAL和人类视觉敏感度的比较![](https://pic4.zhimg.com/v2-a2eeb3e5a1c0e80e094148dd9ba32ecb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='800'></svg>)在玻璃图案测试中，UNREAL和人类的心理测量曲线![](https://pic3.zhimg.com/v2-6ac53f877925c1b9bb8e01a2c0d0dc5a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='539'></svg>)在指向目标任务中，UNREAL对目标和诱饵的大小非常敏感，目标大时学习速度要快得多，诱饵大时最终性能不太理想
种种实验表明，UNREAL会更快地学习大的目标刺激，这种发现也带来了**对一个简单的中央凹视觉模型的具体改进**，显著提高了UNREAL在Psychlab任务和标准DeepMind Lab任务上的表现。

Psychlab这篇论文作者也不少，包括Joel Z. Leibo, Cyprien de Masson d’Autume, Daniel Zoran, David Amos, Charles Beattie, Keith Anderson, Antonio García Castañeda, Manuel Sanchez, Simon Green, Audrunas Gruslys, Shane Legg, Demis Hassabis, Matthew M. Botvinick，全部来自DeepMind。

如果你也想用一下这个Psychlab……论文里的确提到了是在DM-Lab里开源了的，不知更新出来没有，找到的同学吱一声儿~
[https://github.com/deepmind/lab](https://link.zhihu.com/?target=https%3A//github.com/deepmind/lab)

论文地址：
[https://arxiv.org/abs/1801.08116](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1801.08116)

BTW，看PDF有点累，量子位现在比较喜欢用arxiv [https://www.arxiv-vanity.com/papers/1801.08116/](https://link.zhihu.com/?target=https%3A//www.arxiv-vanity.com/papers/1801.08116/)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


