# TensorFlow 2.0和PyTorch谁更好？大牛们争了好几天 - 知乎
# 



> 铜灵 伊瓢 发自 凹非寺
量子位 出品 | 公众号 QbitAI

国庆长假已经结束了，应该收收心了，调整心态，准备过年【手动滑稽脸。

在你小长假踏遍山和大海的时候，国外的机器学习大牛们可没闲着。

TensorFlow 2.0和PyTorch谁更好这个问题，大牛们可是争论了好几天。

究竟国外的大牛如何看待这个问题？和大家平常的观点有何不同？量子位挑选机器学习界KOL精彩观点，带你一文尽览。
![](https://pic2.zhimg.com/v2-fd3b1a6f721eb2c8834834bc0670d0fd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='293'></svg>)
## **较量双方**

**黄队：TensorFlow 2.0**

TensorFlow是机器学习框架排行榜里的佼佼者，Google家族的公子哥。家族产业庞大，因此常年奔波处理各种家族事务中，三年前宣布对外开源，因此得空游走四方。江湖传言2.0版的它易用性增强，核心技能为Eager Execution。
![](https://pic2.zhimg.com/v2-6202551d502d1d307a5f8d645144f1d1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='381'></svg>)
**红队：PyTorch**

PyTorch是神经网络框架新秀，口碑不错的Torch框架的表亲。父母来自Facebook AI研究团队，自带在GPU加速的基础上实现张量计算和动态神经网络的天赋。

在前两天发布的1.0开发者预览版中，PyTorch摇身一变法力增强，不仅自带新的混合前端，还增加了经过改进的torch.distributed库和针对关键性能研究的Eager模式C++接口。
![](https://pic4.zhimg.com/v2-a128cd54519c80227ca3ffdda9e8fddf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='281'></svg>)
## **大牛投票**

话题一出，大牛们纷纷投票站队。
![](https://pic1.zhimg.com/v2-185fdfb9c500c347ad0c5d98d868f80c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='320'></svg>)
**Yann LeCun**

“深度学习三巨头”之一，Facebook首席AI科学家

投票：PyTorch

毫无疑问，作为Facebook首席AI科学家，LeCun当然站PyTorch。
![](https://pic1.zhimg.com/v2-3393626afbc8eb556c8ea28cb03875fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='368'></svg>)
面对有人认为Facebook做PyTorch只是一项“爱好”的观点，LeCun说，Facebook为PyTorch投入了巨大的工程力量，以用来训练产品系统，绝非仅仅是一项爱好。另外，他还强调了一点：PyTorch是一个开源项目，并非Facebook独享的项目。

微软AI合伙人Phil Waymouth显然也是一个PyTorch粉丝，他透露上周微软将PyTorch放在了Azure支持的第一梯队框架中，微软内部有一个PyTorch工程团队，持续在为此开源项目做贡献。
![](https://pic4.zhimg.com/v2-83437f93576e7adc00f1183e07b31c33_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='293' height='293'></svg>)
**Aurélien Geron**

《Scikit-Learn与TensorFlow机器学习实用指南》作者，前YouTube产品经理。

**投票：TensorFlow**

作为一个写了关于TensorFlow的书的作者，Aurélien Geron对TensorFlow 2.0的评价是这样一张图。
![](https://pic1.zhimg.com/v2-9f0221013efd7919420827c622a8d198_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='316'></svg>)
Geron觉得TensorFlow 2.0的更新完全改变了TensorFlow和PyTorch的竞争格局，eager模式和Keras将会使天平倾向TensorFlow那一方。
![](https://pic4.zhimg.com/v2-ba94cd002e00a3caea935c3d4e08a34b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='358'></svg>)
不过，他也顺手表扬了一下PyTorch，PyTorch在易用性方面起了带头作用，不过TensorFlow通过支持eager模式和Keras赶上了赶上了PyTorch的发展。PyTorch 1.0将增加好一些图形功能，所以，竞争让两个框架都迅速发展，变得更好。
![](https://pic4.zhimg.com/v2-dc1d33fd76e678dbad68d4fce0be0c8b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='300'></svg>)
**Jeremy Howard**

fast.ai创始人，曾任Kaggle总裁兼首席科学家。

投票：PyTorch
![](https://pic4.zhimg.com/v2-3f8601dde906ec0c5ae41ae3a71340ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='392'></svg>)
作为PyTorch的铁杆粉丝，Jeremy Howard把TensorFlow略微嘲讽了一下，总结了TensorFlow的进步：

一是TensorFlow 2.0将会Python化；

二是TensorFlow 2.0将会支持eager模式和静态图；

虽然这两个功能是极大的进步，不过，终究是抄PyTorch的，所以，上面那个天平……怕是来搞笑的吧。

不过，对于“抄”这一点，威斯康星大学麦迪逊分校助理教授Sebastian Raschka并不认为这带有某种负面意义：
![](https://pic4.zhimg.com/v2-81d17e2575ad3fb174a4869b5b83ad1f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='359'></svg>)
“抄”并不意味着会让一个工具变坏，反而是让它朝着变好的方向发展，这是一种双赢策略：PyTorch更专注于静态图，TensorFlow更专注于动态图，两者都是不同的框架，它们的存在都有价值。

**Denny Britz**

毕业于斯坦福大学，前Google Brain深度学习研究员，推特粉丝29.9K。
![](https://pic2.zhimg.com/v2-3ffdce85a9fa35556ab50f87a182ef95_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='614' height='460'></svg>)
**投票：中立票**

对于这个问题，Denny Britz认为，除了一些非常特殊的用力（比如嵌入式设备等），选择TF还是Pytorch影响不大。

“特别是现在，它们看起来越来越相似了，将一个框架的知识转移应用到另一个上面就很简单了。”Britz说。
![](https://pic4.zhimg.com/v2-7a54777587d60108f1804de2beeb854f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='305'></svg>)
**François Chollet**

现就职于Google，被称为“Keras之父”，推特粉丝115K
![](https://pic1.zhimg.com/v2-ce17ad6859e9a83a26a473b101afceec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='414'></svg>)
投票：TensorFlow

虽然这些争论没有Keras什么事，但公认的Keras之父却有些按捺不住要跳出来说两句了。Chollet给PyTorch投了一张大大的反对票，认为现在公众认为它势头这么足，与宣传营销做得好关系不小啊。

Chollet表示，根据自己监测的数据，PyTorch的用户量在2018年的4月-5月已经达到了顶峰，之后就要走下坡路了。

—**完**—

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


