
# TensorFlow2.0来了 - 喜欢打酱油的老鸟 - CSDN博客


2019年03月15日 11:37:50[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：234


# 前言
3 月 7 日凌晨，谷歌 TensorFlow 开发者峰会（TensorFlow Dev Summit 2019）在美国加州举行。自今年 1 月份谷歌放出 TensorFlow 2.0 开发者预览版，开发者们如今终于迎来了万众期待的 TensorFlow 2.0 Alpha 版的正式发布。也许是为了呼应谷歌此前将 TensorFlow 2.0 称作重要的「里程碑」，TensorFlow 的 Logo 也从过去的三维积木状变成了扁平化风格的「T」和「F」字母拼接。
![TensorFlow2.0来了](http://p3.pstatp.com/large/pgc-image/3c5fad3ce21449e3abbef70e9b7128b1)
新Logo
除了发布 TensorFlow 2.0 Alpha 版，谷歌在本次大会上还发布了「一箩筐」围绕 TensorFlow 的其他软件的更新和最新成果，包括：经过更新的 TensorFlow Lite 1.0、TensorFlow js 1.0 和 Swift for TensorFlow 0.2 ，端到端的机器学习平台 TensorFlow Extended (TFX) 以及两款面向隐私问题的 TensorFlow Federated 开源框架和 TensorFlow Privacy 开源库。
另外值得一提的是，随着 TensorFlow 2.0 Alpha 版的到来，谷歌还发布了两门深度学习课程：Udacity 的《TensorFlow 深度学习简介》和 Deeplearning.ai 的《TensorFlow：从基础知识到掌握专业化》系列课程。
# TensorFlow 2.0 Alpha 版：强调简单易用性
今年 1 月份，谷歌就放出 TensorFlow 2.0 开发者预览版，引起了开发者们的满腔期待。而不到两个月后随之而来的 TensorFlow 开发者峰会，其最受关注的环节莫过于 TensorFlow 2.0 Alpha 版的发布。
据悉，针对 TensorFlow 2.0，TensorFlow 团队听取了开发者关于「简化 API、减少冗余并改进文档和示例」的建议来进行设计，将 TensorFlow 2.0 Alpha 版的更新重点放在简单和易用性上，主要进行了以下更新：
1、使用 Keras 和 eager execution，轻松建立简单的模型并执行
2、在任何平台上的实现生产环境的模型部署
3、为研究提供强大的实验工具
4、通过清除不推荐使用的 API 和减少重复来简化 API
同时，在过去的几年中，谷歌陆续在 TensorFlow 中添加了许多组件，而 TensorFlow 2.0 Alpha 版中则将这些组件将打包成了一个综合性平台，支持从训练到部署的机器学习工作流，其新架构的简化概念图如下所示：
![TensorFlow2.0来了](http://p1.pstatp.com/large/pgc-image/bf0c244e27e54e0ab263619492b6daf1)
新架构
而更简单化的新框架更是带来了更加简洁的工作流，即：先使用 tf.data 创建的输入管道读取训练数据；然后使用 tf.keras 或 Premade Estimators 构建、训练和验证模型；接着用 eager execution 进行运行和调试；再使用 Distribution Strategy API 在不更改模型定义的情况下，基于 CPU、GPU 等不同硬件配置上分布和训练模型；最后将模型导出到 SavedModel 保存。其工作流如下图所示：
![TensorFlow2.0来了](http://p9.pstatp.com/large/pgc-image/6497ccc276784e63b7c7a2d20e1763f7)
工作流图
此外，TensorFlow 2.0 Alpha 版还带来了一些新的功能，允许研究人员和高级用户使用丰富的扩展进行实验，如 Ragged Tensors、TensorFlow Probability、Tensor2Tensor 等。
为了简化代码迁移到 TensorFlow 2.0 的过程，谷歌还提供一个转换工具和指导文档用来更新 TensorFlow 1.x Python 代码以使用与 TensorFlow 2.0 兼容的 API，并将无法自动转换的代码标记出来。
**在 API 方面的更新也是 TensorFlow 2.0 Alpha 版的一大亮点，其将 Keras API 指定为构建和训练深度学习模型的高级 API，并舍弃掉其他 API**。另外值得一提的是，Keras 提供了几个模型构建 API，例如可以利用 Sequential API 构建模型，然后使用「compile」和「fit」，tensorflow.org 中所有常见的」tf.keras」示例均可在 2.0 中便捷使用。
**TensorFlow 2.0 Alpha 版的另一个最明显的改变就是将用于机器学习的实验和研究平台——Eager execution 设置为默认优先模式**，这就意味着任何运算在调用后就会立即运行，从而不再需要预先定义静态图，就可以通过「tf.Session.run()」执行图的各个部分，让执行过程变得更加简单和快捷。
此外，Eager execution 还有助于原型制作、调试和监控运行中的代码，用户可使用 Python 调试程序检查变量、层及梯度等对象，并利用装饰器「@tf.function」中内置的 Autograph 直接获取图表优化和效率，这整个过程不仅能够保留 TensorFlow1.x 基于静态计算图执行的所有优点：性能优化、远程执行，以及序列化、导出和部署的能力，同时还增加了用简单 Python 表达程序的灵活性和易用性。
**整体而言，TensorFlow 2.0 Alpha 版具有以下三大特性**：
第一，易用性，其主要体现在使用 tf.keras 作为高级 API，且将 Eager execution 作为默认模式。
第二，简洁性，其主要体现在 TensorFlow 2.0 Alpha 版删除了重复的功能，并且不同 API 的调用语法也变得一致、直观，同时它的兼容性更加完善。
第三，灵活性，其主要体现在 TensorFlow 2.0 Alpha 版提供了完整的低级 API，并可以在 tf.raw_ops 中访问内部操作，同时还提供了变量、checkpoint 以及层的可继承接口。
开发者峰会上，TensorFlow 工程总监 Rajat Monga 也向大家呈现了 TensorFlow 这几年来的「战绩」：自 2015 年 11 月推出以来，TensorFlow 总下载量超过 4100 万次，提交了 5 万多次代码更新，目前已有 1800 多名来自全世界的贡献者。
而随着更加顺从民意、使用更加简单的 TensorFlow 2.0 Alpha 版的发布，TensorFlow 又将收获怎样的成绩呢？大家可以拭目以待。
TensorFlow 2.0 Alpha 版官方网址：https://www.tensorflow.org/alpha
TensorFlow 2.0 Alpha 版开源地址：https://github.com/orgs/tensorflow/projects/4
[https://www.toutiao.com/a6666606736964583939/](https://www.toutiao.com/a6666606736964583939/)

