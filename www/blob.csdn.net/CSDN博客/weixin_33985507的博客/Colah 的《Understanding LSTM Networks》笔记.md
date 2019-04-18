# Colah 的《Understanding LSTM Networks》笔记 - weixin_33985507的博客 - CSDN博客
2017年04月04日 19:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
最近做毕设读论文读得心累，很多论文公式一堆提升逼格，很多想法也不具普适性，好像大家都在各玩各的。
前两日看到知乎一个问题 [如何评价深度学习文章平台Distill？](https://link.jianshu.com?t=https://www.zhihu.com/question/57639134)，好奇点进一看究竟。啊真是通透易懂，平易近人。因为最近在研究Reservoir Comuting和ESN的东西，就着重看了一下RNN一块，做个小笔记。
### Recurrent Neural Networks
人类的认知是连续的，认知事物总有先前经验，不会将所有事都重新思考。RNN是一种能让信息暂存的神经网络，它有loops，loop能够将信息不断往下一个网络状态传。
![1298340-7dadfee4662ad006.png](https://upload-images.jianshu.io/upload_images/1298340-7dadfee4662ad006.png)
LSTM是一种特殊的RNN，表现突出。
### The Problem of Long-Term Dependencies
比如说要预测 “the clouds are in the ___”，这个填空很简单肯定是sky，这是RNN能做的一件对过去信息需求量(通俗地说就是经验)比较小的事情。但是如果对过去信息需求量大的话，RNN表现不太好。但是LSTM就超棒der。
### LSTM Networks
> 
LSTMs are explicitly designed to avoid the long-term dependency problem.
![1298340-5e32789173aae671.png](https://upload-images.jianshu.io/upload_images/1298340-5e32789173aae671.png)
标准RNN的循环模块只有单层
![1298340-13c87af56e65183a.png](https://upload-images.jianshu.io/upload_images/1298340-13c87af56e65183a.png)
LSTM的循环模块有四个相互作用层
根据LSTM的模型图，\sigma 是介于0~1之间，决定保留多少信息来保障cell的状态，决定每个状态下的输出。
涉及到具体公式就不往下写了。这是一篇为了让爸妈看懂我在干啥的小笔记～
原文：[Understanding LSTM Networks](https://link.jianshu.com?t=http://colah.github.io/posts/2015-08-Understanding-LSTMs/)
