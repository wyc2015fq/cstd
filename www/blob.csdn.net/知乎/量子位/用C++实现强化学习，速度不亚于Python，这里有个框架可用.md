# 用C++实现强化学习，速度不亚于Python，这里有个框架可用 - 知乎
# 



> 乾明 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

没法用Python，怎么实现强化学习？

现在，有了一个新选择。

一位名叫Isaac Poulton的英国小哥，开源了一个名为**CppRL**的C++强化学习框架。



![](https://pic2.zhimg.com/v2-fb30a2f8b211243f8dc67725b1749e15_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='466'></svg>)



整个框架，用PyTorch C++编写而成，主要的使用场景，就是在没法使用Python的项目中实现强化学习。

现在，这个框架已经可以实现A2C（Advantage Actor Critic）、PPO（近端策略优化）算法。

而且，用户只需要很少的设置，就能够在电脑的桌面程序中使用。

小哥说，之所以做这个框架，是因为C++中还没有一个通用的强化学习框架。

但自己的个人项目中需要一个，就借着PyTorch C++前端的发布，做了一个出来，还顺便训练了一批LunarLander-v2游戏中的智能体。

## **框架有何特性？用起来效果如何？**

根据小哥的介绍，这个框架一共有五大特性：
- 首先，它能够实现强化学习中很关键的两个算法A2C和PPO。
- 其次，支持基于门控循环单元（GRU）的循环策略。
- 第三，具备跨平台兼容性，已经在Windows 10和Ubuntu 16.04和Ubuntu 18.04上进行了测试。
- 第四，有可靠的测试覆盖率。
- 第五，能够适度进行优化，可以开放PR来推进框架优化。

此外，框架中还有对OpenAI Gym的实现，其通过ZeroMQ通信来测试框架在Gym环境中的表现。

基于当前的版本的框架，小哥在自己的笔记本电脑（i7-8550处理器）上，平均用60秒训练一个智能体就可以获得200奖励。在登月游戏LunarLander-v2中实现的效果如下：
![](https://pic1.zhimg.com/v2-d18d83e215197afcaf3acffa4b2e959c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='870' height='606'></svg>)
这8个智能体中，有5个智能体完成了任务。

在回答Reddit上网友的提问时，他介绍了训练智能体的速度，基本上和用Python实现速度相当。

但是，在一些环境中速度会比较慢。比如OpenAI Gym客户端中，必须要与Python接口，并通过TCP发送观察结果，所以训练速度会大幅下降。

不过，小哥说会通过重做Gym客户端来解决这个问题。

这一框架未来会如何？

小哥表示，要把它打造成一个可扩展的、经过合理优化的、随时可以使用的框架。

在Reddit的帖子中，他也发起了呼吁，如果你有兴趣来一起完成这一项目，非常欢迎提交PR~

## **传送门**

C++强化学习框架项目地址：
[https://github.com/Omegastick/pytorch-cpp-rl](https://link.zhihu.com/?target=https%3A//github.com/Omegastick/pytorch-cpp-rl)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤




