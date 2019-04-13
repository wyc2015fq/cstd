
# NIPS 2017 | 线上分享第一期：似自然梯度的优化算法KFAC与强化学习算法ACKTR - Paper weekly - CSDN博客


2017年11月07日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1030



上周我们发布了[《攻略
 | 虽然票早已被抢光，你可以从机器之心关注 NIPS 2017》](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650732638&idx=5&sn=afe45d3766d85b8941d6a1a89f911ea5&chksm=871b3c20b06cb5364164a89ff1d9c2ca50e65979d44f4f4e5307f8d4d46cdc2f8ba8ec6b6605&scene=21#wechat_redirect)，在 NIPS 2017 正式开始前，我们将选出数篇优质论文，邀请论文作者来做线上分享，聊聊理论、技术和研究方法。

所以，第一期分享来了。

北京时间 11 月 8 日20:00至 21:00，多伦多大学在读三年级博士生 Yuhuai Wu（吴宇怀）将为大家分享一篇 NIPS 2017 论文《Scalable trust-region method for deep reinforcement learning using Kronecker-factored approximation》。

![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWicAiaQDw9hIAFyoXuE9LfPlvfrxE56h2jPfSKGQ1icWb38nkagjeibj1LgJxtTiaZyEoM5LYHqsibpwoTg/?wxfrom=5&wx_lazy=1)

**分享者简介**

Yuhuai Wu（吴宇怀），多伦多大学在读三年级博士生，师从 Roger Grosse。曾在 Geoffrey Hinton、Yoshua Bengio、Ruslan Salakhutdinov 等指导下做研究。主要研究方向是优化算法、强化学习。是 2017 年 Google fellowship in machine learning 的得主之一。2017 暑假于 OpenAI 做实习，开源了
 ACKTR 算法。
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWicAiaQDw9hIAFyoXuE9LfPlv480BtKl2Tql16brqI4YzaQ1j7XZBXCiaTEgADMrRxW7ujwNtVTrD4IQ/?)


**演讲主题**

这次演讲主要研讨一个最近研发的似自然梯度的优化算法 -- KFAC（似二阶算法）。我们将其用在强化学习的问题上，并提出了新的强化学习算法 -- ACKTR，发现比前人的算法有效 2-3 倍。

针对这篇论文，机器之心曾发布过一篇简介：[《OpenAI
 开源算法 ACKTR 与 A2C：把可扩展的自然梯度应用到强化学习》](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650729930&idx=2&sn=35da79af4a09ed2a4f0e3ef0d6d7c77b&chksm=871b29b4b06ca0a2618055a370f07f34a5910c9084354146401f50102a3982676d74a6f8f878&scene=21#wechat_redirect)，请大家做课前预习。

**参与方式**

线上分享将在「NIPS 2017 机器之心官方学霸群」中进行。**加群方式：**长按扫描下方二维码，添加「机器之心小助手Ⅱ」，**备注暗号：147**，由小助手拉大家入群。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW9SQxAyrE3STC2uGyVXwh51j3srzvYS4c4TbL4TMW3AicpQsfQyaYJh5P6ibTZqMs6BbTzK5RfQ5zFg/?)

戳**「阅读原文」**，查看机器之心 NIPS 2017 专题策划并参与其中。



