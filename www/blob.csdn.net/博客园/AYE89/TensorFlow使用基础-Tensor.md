
# TensorFlow使用基础-Tensor - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[TensorFlow使用基础-Tensor](https://www.cnblogs.com/eniac1946/p/7832349.html)
|Posted on|2017-11-14 14:22|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7832349)|[收藏](#)
|使用 TensorFlow 之前你需要了解关于 TensorFlow 的以下基础知识 :
|• 使用图 (graphs) 来表示计算 .
|• 在会话 ( Session ) 中执行图 .
|• 使用张量 (tensors) 来代表数据 .
|• 通过变量 ( Variables ) 维护状态 .
|• 使用供给 ( feeds ) 和取回 ( fetches ) 将数据传入或传出任何操作
|概述
|TensorFlow 是一个以|图 (graphs)|来表示计算的编程系统 , 图中的|节点|被称之为|op(op-
|eration|的缩写 ). 一个 op 获得零或多个|张量 (tensors)|执行计算 , 产生零或多个张量。张量
|是一个按类型划分的多维数组。例如 , 你可以将一小组图像集表示为一个四维浮点数数
|组 , 这四个维度分别是 [batch, height, width, channels] 。
|TensorFlow 的图是一种对计算的抽象描述。在计算开始前 , 图必须在 会话 ( Session
|() ) 中被启动 . 会话将图的 op 分发到如 CPU 或 GPU 之类的设备 ( Devices() ) 上 , 同时提供
|执行 op 的方法。这些方法执行后 , 将产生的|张量 (tensor)|返回。|在 Python 语言中 , 将返
|回 numpy 的|ndarray 对象|;|在 C 和 C++ 语言中 , 将返回 tensorflow::Tensor 实例。
|计算图
|通常， TensorFlow 编程可按两个阶段组织起来 : 构建阶段和执行阶段 ; 前者用于组
|织计算图，而后者利用 session 中执行计算图中的 op 操作。
|构建过程完成后就可运行执行过程。为了载入之前所构建的图，必须先创建一个
|会话对象 ( Session object) 。会话构建器在未指明参数时会载入默认的图。
|完整的会话 API 资料，请参见会话类 (Session object) 。
|TensorFlow 事实上通过一个“翻译”过程， 将定义的图转化为不同的可用计算资源
|间实现分布计算的操作，如 CPU 或是显卡 GPU 。通常不需要用户指定具体使用的 CPU
|或 GPU ， TensorFlow 能自动检测并尽可能的充分利用找到的第一个 GPU 进行运算。
|张量
|TensorFlow 程序使用 tensor 数据结构来代表所有的数据 , 计算图中 , 操作间传递的
|数据都是 tensor. 你可以把 TensorFlow 的张量看作是一个 n 维的数组或列表 . 一个 tensor
|包含一个静态类型 rank, 和一个 shape. 想了解 TensorFlow 是如何处理这些概念的 , 参见
|Rank, Shape, 和 Type] 。
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171114135628781-1044531018.png)
|每个Tensor中的元素都有相同的data type，并且data type总是已知的。
|shape表示有几维和每个维度的size，可能有部分未知
|除了tf.Variable，一个张量的值是不可变的
|Rank
|tf.Tensor对象的rank表示有几个维度
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171114140900906-329090440.png)
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171114140954952-563604136.png)
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171114141025515-999750659.png)
|tf.Tensor切片
|[ ]中每个数字代表一个|索引（index）|，index本身可以是一个tf.Tensor的标量
|Rank为2的tensor，输入两个索引得到一个标量，如下：
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171114144459484-764704267.png)
|Shape
|表示每个维度的size
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171114142138515-212498109.png)
|Data types
|tensor中的数据类型，引用tf.DataType
|Tensor.eval
|用于计算一个tensor的值
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171114142630765-1242777704.png)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
