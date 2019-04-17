# TensorFlow 1.9.0正式发布：新手指南更友好 - 知乎
# 



> 李林 编译整理
量子位 出品 | 公众号 QbitAI

TensorFlow 1.9.0正式版来了！

谷歌大脑研究员、Keras作者François Chollet对于这一版本评价甚高，他说：“不管是不是TF用户都应该看一看：TF最近进步巨大。这是通往ML未来的一大步。”
![](https://pic1.zhimg.com/v2-aa7729e1f2a1b4bcb3282537c516a45c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='582'></svg>)（帮他声明一下利益相关：这次更新与Keras关系密切。）
那么，TensorFlow 1.9.0究竟有什么变化呢？

首先是对Keras的更大支持。Keras是一个深度学习的高级API，把创建和训练模型所需的工作整合成了很多模块，TensorFlow是它的一个后端。在TensorFlow中，它叫tf.keras。

现在，TensorFlow的新手指南变了样，从Keras入手，还附上了一个详细的Keras Guide。而且，所有指南都有相应的Google Colab笔记本，可以说非常小白友好了。
![](https://pic4.zhimg.com/v2-4cbbb1465362c4ec1a2898a36d0c0f07_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='589'></svg>)



同时，TensorFlow里的Keras本身也有提升。tf.keras升级到了Keras 2.1.6 API，新增了tf.keras.layers.CuDNNGRU和tf.keras.layers.CuDNNLSTM，分别用于更快的GRU实现和更快是LSTM实现。

除了Keras之外，Eager execution也进入了TensorFlow的新版新手指南。

Eager execution为TensorFlow引入了动态图机制，不创建静态图，它也可以运行TensorFlow代码。

另外，TensorFlow 1.9.0还有这些主要的新功能：

通过feature columns和losses，新增了对梯度提升树估计器（gradient boosted trees estimators）的支持。

TFLite优化转换器的Python接口有所扩展，标准pip安装的情况下又包含了命令行界面。这一版本中的distributions.Bijector API还亮支持Bijectors的广播。

通过tf.decode_compressed和tf.string_strip优化了数据载入和文本处理。同时，也实验性地增加了tf.contrib.estimator.BaselineEstimator、tf.contrib.estimator.RNNClassifier和tf.contrib.estimator.RNNEstimator。
![](https://pic3.zhimg.com/v2-e45a3a4e46bb7748282205a84a640ee2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='688' height='388'></svg>)
新版本的更多特性，可以穿过这个传送门查看：

[tensorflow/tensorflow](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tensorflow/releases/tag/v1.9.0)

还有焕然一新的新手指南：

[https://www.tensorflow.org/tutorials/](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/tutorials/)

曾经从入门到放弃的同学可以从头再来啦~

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




