# deeplearning4j – 分布式DL开源项目 - wishchinYang的专栏 - CSDN博客
2014年07月06日 17:58:08[wishchin](https://me.csdn.net/wishchin)阅读数：2834
原文链接：[http://www.52ml.net/16157.html](http://www.52ml.net/16157.html)
     Deeplearning4j is the first commercial-grade deep learning library written in Java. It is meant to be used in business environments, rather than as a research tool for extensive data exploration. Deeplearning4j is most
 helpful in solving distinct problems, like identifying faces, voices, spam or e-commerce fraud.
     Deeplearning4j aims to be cutting-edge plug and play, more convention than configuration. By following its conventions, you get an infinitely scalable deep-learning architecture. The framework has a domain-specific language (DSL) for neural networks,
 to turn their multiple knobs.
     Deeplearning4j includes a **distributed deep-learning framework** and a normal deep-learning framework; i.e. it runs on a single thread as well. Training takes place in the cluster, which means it can process massive amounts of data. Nets
 are trained in parallel via iterative reduce.
     The distributed framework is made for data input and neural net training at scale, and its output should be highly accurate predictive models.
项目地址：[http://deeplearning4j.org/](http://deeplearning4j.org/)
简单介绍一下项目发起者 Gibson：
        谷歌微软等科技巨头都在进军人工智能领域。谷歌有语音识别服务，直接向手机说话就可以进行搜索；微软最近发布的新版Skype中加入了即时翻译功能；Facebook最近也成立了一个新部门，探索人脸识别技术。所有这些技术都可以称为深度学习（deeping learning），通过模仿人脑加工信息的方式，让机器拥有“学习”的能力。
        人工智能技术似乎如此高端，只能是科技巨头们的游戏，但Adam Gibson，旧金山的一名独立软件工程师，希望解除巨头对这一新技术的封锁。他认为任何有兴趣的网站，公司和开发者都应该能接触深度学习技术，并因此成立了一家创业公司Skymind。
         “我们希望不需要雇一位数据专家就能使用机器学习技术”，Gibson表示，他24岁，中途退学后通过研究学术论文自学了深度学习，曾在多个公司担任机器学习顾问，并在一些教育机构讲课。
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------————————————————————我是华丽的分割线——————————————————————————————-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
转到主页：[http://deeplearning4j.org/](http://deeplearning4j.org/)
         Deeplearning4j is the first commercial-grade deep-learning library written in Java. It is meant to be used in business environments, rather than as a research tool for extensive data exploration. Deeplearning4j is most helpful in solving distinct
 problems, like identifying [faces](http://deeplearning4j.org/facial-reconstruction-tutorial.html), voices, spam or e-commerce fraud.
         Deeplearning4j是一个使用Java书写的商业级的深度学习库。有意思. 一定要完整的看看....
