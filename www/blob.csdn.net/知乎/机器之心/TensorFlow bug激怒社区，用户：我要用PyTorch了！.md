# TensorFlow bug激怒社区，用户：我要用PyTorch了！ - 知乎
# 



机器之心报道，参与：张倩、路。

> 最近，TensorFlow 用户发现了一个 bug：tf.keras Dropout 层出问题了。在 tf.keras 1.11、1.12 版本中，Dropout 层出现问题。用户调用 model.fit 时，Dropout 层出现测试阶段行为，而它原本应该在训练和测试之间转换。Keras 作者 François Chollet、谷歌大脑作出回复。

昨天，randolf-scholz 开了一个 issue 讨论这个 bug。

issue 页面：[https://github.com/tensorflow/tensorflow/issues/25175](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tensorflow/issues/25175)

具体描述如下：

**系统信息**
- 并未写自定义代码；
- OS 平台和分布：Ubuntu 18.10；
- TensorFlow 安装源：conda 默认渠道
- TensorFlow 版本：1.12（conda 版本 mkl_py36h69b6ba0_0）
- Python 版本：3.6.8




**当前行为**

Dropout 层的行为是测试模式。

**期望行为**

Dropout 层在训练和测试模式之间转换。

复现该 issue 的教程代码：[https://www.tensorflow.org/tutorials/](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/tutorials/)


```
import tensorflow as tf
mnist = tf.keras.datasets.mnist

(x_train, y_train),(x_test, y_test) = mnist.load_data()
x_train, x_test = x_train / 255.0, x_test / 255.0

model = tf.keras.models.Sequential([
  tf.keras.layers.Flatten(),
  tf.keras.layers.Dense(512, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.2),
  tf.keras.layers.Dense(10, activation=tf.nn.softmax)
])
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

model.fit(x_train, y_train, epochs=5)
model.evaluate(x_test, y_test)
```





将 droprate 设置为 0.999 和设置为 0.2 的性能一样（准确率都超过 95%）。这不可能，这意味着 Dropout 层内的训练／测试转换受到损坏，Dropout 层总是返回 inputs，即使是在训练阶段也是这样。

在 TensorFlow 1.10 中，教程脚本在 droprate 为 99.9% 时，给出了「正确的」结果——大约 11% 的准确率。

**其他信息／日志**

问题根源似乎是 tensorflow.keras.backend.learning_phase，或者 model.fit 没有正确设置训练 flag。

该用户还表示：问题似乎出现 tf.keras，而不是单独的 keras 包。这个 issue 似乎还影响了 keras.backend.in_train_phase 功能。ta 在以下文件中进行了测试：
- [https://pastebin.com/SvTwrSC8](https://link.zhihu.com/?target=https%3A//pastebin.com/SvTwrSC8)
- [https://pastebin.com/VtBwL5mz](https://link.zhihu.com/?target=https%3A//pastebin.com/VtBwL5mz)




在该 issue 页面上，其他用户也进行了测试。

有用户表示，在 TensorFlow 1.13.0 版本上进行测试时，没有出现问题。

而 Keras 作者 François Chollet 表示：[[Python] #!/usr/bin/env python3  # -*- coding: utf-8 -*-  import keras      class TestLay - Pastebin.com​pastebin.com![图标](https://pic2.zhimg.com/v2-071174ef23ecb6b7cba95f041f577141_ipico.jpg)](https://link.zhihu.com/?target=https%3A//pastebin.com/SvTwrSC8)而 Keras 作者 François Chollet 表示：

> 我对该 issue 和脚本进行了详细调查，确认这种错误行为仅出现在 TensorFlow 1.11 和 1.12 版本中。

这种行为与 Dropout 层或 in_train_phase 后端性能无关。Dropout 层没有问题。

当用户在「deferred mode」中使用 Sequential 模型时，这个 bug 才会出现。deferred mode 是最近引入的使用 Sequential 模型的新方式，无需输入 input_shape 参数作为第一层。在这个案例中，似乎学习阶段值设置有误。

我认为这不会影响到很多用户，因为 deferred mode 还很新，并未得到广泛使用，几乎没有什么 Keras 样例使用它（[http://keras.io](https://link.zhihu.com/?target=http%3A//keras.io) 上也没有使用过它）。但是很不幸，[http://tensorflow.org](https://link.zhihu.com/?target=http%3A//tensorflow.org) 上的一些教程开始使用它。

我正在联系 devrel，确保更新代码示例，在 Sequential 模型中添加 input_shape 参数（这种方法更好，它允许静态层兼容性检查）。

你可以在第一个 flatten 层中输入 input_shape=(28, 28) 来修复这个 bug。

不久前这个 bug 已在 TF 端修复。

TensorFlow 是机器学习领域最广泛使用的框架，此 bug 一出现就引起了大量用户的关注和讨论。吐槽声甚重……

**网友RedEyed__：想弃坑了**

他们怎么能这样……这可是最常用的层。我也遇到了这个问题。这类问题的存在让我想转投 PyTorch。

**网友 duckbill_principate：开发者根本不关心用户体验**

把所有方法硬塞进 Keras 的想法真的很糟糕，尤其是 eager 模式。Keras 的设计对于标准模型来说还挺好用，但对以研究为导向的模型非常不友好（研究者不应该是 eager 模式的主要受益者吗？）eager 听起来不错，但你用得越多，发现的 bug 就越多：实现异常缓慢而且会出现 bug，或者干脆遗失非常重要的 ops。

我已经愉快地使用 TensorFlow 两年半了，但设计的改变、蹩脚的 eager 实现、bug 的增多都让我觉得非常不爽。我觉得开发者似乎没有多少紧迫感，也没有多关心用户体验，而是更多地追求自己的目标（这从来都不是一个好现象）。

我不想放弃 TF，因为它大部分还是好的，但我最终还是转向了 PyTorch（原因如上所述，而且我也失去了对 TF 开发者的信任）。对此，我从未后悔过。

PS：为什么两年之后 Tensorboard 的 UI 还是那么烂？调试、可视化和理解神经网络可以说是深度学习中最困难的部分，而在这方面能够提供帮助的主要工具在两年内几乎没有任何改进？！？！？！谷歌到底在干嘛？

PPS：数据 pipeline 为什么也还是那么烂？有时候读一些文档就像拔牙一样。

**谷歌大脑回应：尝试给 1.11 和 1.12 加补丁**

感谢您引起我们的注意并创建了这个 bug 讨论。我们保证会在 1.13 中修复这个 bug，同时看看能否给 1.11 和 1.12 加补丁。

尽管谷歌大脑官方已经作出了回应，但这一 bug 仍然破坏了用户对 TensorFlow 开发进程的信心。

**网友 duckbill_principate ：开发进程缓慢是可预见的**

说真的，我不指望到今年年底能用上 TensorFlow 2.x。如果我要开始一个大型研究或工程项目，我会在 1.12 或 1.3、1.4 上进行标准化，直到尘埃落定。以他们的速度，在正式启用之前，肯定还要花好几个月的时间修复 bug 并提速。令人沮丧的是，近一年来他们就像一列失事的火车缓慢前进。

感觉 TF 的团队好像分成了三部分（三路人分别忙着开发 2.0、改变 eager 及重构 keras），因此可以预见，整个开发进程都会慢很多。按照经验来看，在做其它事情之前，最好先挑一件事做好……我不明白他们为什么要同时做三件事，也许是怕 PyTorch 抢了自己饭碗吧。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
参考链接：

[https://github.com/tensorflow/tensorflow/issues/25175](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tensorflow/issues/25175)[Dropout layer is broken · Issue #25175 · tensorflow/tensorflow​github.com![图标](https://pic4.zhimg.com/v2-a536cfed78afd80bb1f1cdad57136437_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tensorflow/issues/25175)[https://github.com/tensorflow/tensorflow/issues/25175](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tensorflow/issues/25175)

[https://www.reddit.com/r/MachineLearning/comments/ajqmq9/d_tfkeras_dropout_layer_is_broken/](https://link.zhihu.com/?target=https%3A//www.reddit.com/r/MachineLearning/comments/ajqmq9/d_tfkeras_dropout_layer_is_broken/)






