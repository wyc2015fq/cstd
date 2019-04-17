# TensorFlow 2.0「开发者预览版」上线，内容每日更新 - 知乎
# 



机器之心报道，参与：刘晓坤、李泽南。

> TensorFlow 2.0 预览版上线了！近日，谷歌 AI 团队成员 Martin Wicke 在社交网络上向大家发布了这一最流行深度学习框架的「开发者预览版」，该版本又被称为「Nightly 版」——因为框架会每晚更新。这不是一个稳定的版本，但开发者们可以尝试使用，熟悉新版本的特性。

不过，即使是在 TensorFlow 2.0 中，目前也没有对于 Python 3.7 的支持。

还有网友表示：

Nightly 版更接近于 beta 版本，为用户提供即将发布的内容的预览，以及有机会使用/贡献新功能。Nightly 版绝不是一个专为产品而设计的稳定版本，它旨在测试开发人员正在使用的最新功能，不会自动提供文档，可以找到 bug，无法保证功能的可用性。

预览版官方页面：[https://www.tensorflow.org/versions/r2.0/api_docs/python/tf](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/versions/r2.0/api_docs/python/tf)
![](https://pic2.zhimg.com/v2-38743cb0c8d078268519a8b723143f99_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='357'></svg>)
开发者们可以输入如下代码安装预览版：

`pip install tf-nightly-2.0-preview`

安装 GPU 版的方法是：

`pip install tf-nightly-gpu-2.0-preview`

目前的版本号是：tf-nightly-2.0-preview-1.13.0.dev20181214

TensorFlow 2.0 如今在 pypi 上提供 tf-nightly-2.0-preview 和 tf-nightly-gpu-2.0-preview 版本。你还可以通过传递—config=v2 到 bazel 命令来从源代码构建版本（在运行 configure 之后）。TensorFlow 2.0 是从同一个源码树构建的，因此如果你是从源代码构建，只需要从主服务器生成。

可从预览版官方页面获取 Nightly 版生成的文档。因为 Nightly 版还在开发中，文档可能随时变得不再有用或不完整。

在 TensorFlow 2.0 的开发中，谷歌专注于可用性，并对指定（specify）和运行计算的方式进行了重大更改。2018 年秋季发布的 RFC（[https://github.com/tensorflow/community/tree/master/rfcs](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/community/tree/master/rfcs)）实现了这样的重大变化。

pip 软件包附带一个转换器工具，可以升级（大多数）1.x TensorFlow 代码，因此它可以在 Nightly 版安装的 2.0 的情况下运行。tf_upgrade_v2 工具大量使用兼容性模块：tf.compat.v1。此模块包含 TensorFlow 1.x 中存在的所有符号及其原始功能。此外，升级工具也处于开发阶段，它可能无法在复杂项目上运行。

这个 Nightly 版的 2.0 仍然不完整。因此，谷歌表示不保证稳定性，还存在未解决的性能问题，并且缺少某些功能（例如，仅支持某些分布式机制，特别是 TPU 的支持仍然不完整），并且 TensorFlow 生态系统尚未和 2.0 同步更新（例如，TFHub）。谷歌表示会在创建 2.0-alpha 之前解决这些问题。

如果有兴趣尝试 Nightly 版，谷歌表示很欢迎报告使用中发现的问题。

在这里查看提交错误报告的方法：[https://github.com/tensorflow/community/blob/master/governance/tensorflow-testing.md](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/community/blob/master/governance/tensorflow-testing.md)
![](https://pic2.zhimg.com/v2-c209d8c61573e7bcdcfd4d58a87805f5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='711' height='400'></svg>)
对于最流行的机器学习框架来说，TensorFlow 2.0 将是一个重要的里程碑：新版本意味着大量的内容梗概，所有一切都以人人可以使用机器学习为目标。但是，这些更改可能会要求老用户重新学习如何使用框架。

去年 9 月，谷歌在开发者大会上就曾表示，在 TensorFlow 2.0 版本中，Eager 模式会成为默认执行模式，让开发者更简洁高效地搭建原型。

冯亦菲则在开发者大会上表示，TensorFlow 2.0 beta 版将会在 2018 年底公开（看来有点延迟了），而正式版预计于 2019 年的第一个季度（最迟第二季度）问世。Eager 模式变为默认设置之后，开发者可以在原型搭建完成后，利用 AutoGraph 把在 Eager 模式下搭建的模型自动变成计算图。开发者也可以进一步对 AutoGraph 生成的计算图进行优化，或者关掉 Eager 模式自己构建计算图。 
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
参考内容：

[https://www.reddit.com/r/MachineLearning/comments/aem203/d_tensorflow_20_nightly_build_is_up/](https://link.zhihu.com/?target=https%3A//www.reddit.com/r/MachineLearning/comments/aem203/d_tensorflow_20_nightly_build_is_up/)

[https://groups.google.com/a/tensorflow.org/forum/#!topic/developers/aKdmUOiyzGM](https://link.zhihu.com/?target=https%3A//groups.google.com/a/tensorflow.org/forum/%23%21topic/developers/aKdmUOiyzGM)




