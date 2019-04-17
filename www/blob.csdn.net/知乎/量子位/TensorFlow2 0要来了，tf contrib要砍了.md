# TensorFlow2.0要来了，tf.contrib要砍了 - 知乎
# 



> 圆栗子 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

TensorFlow 2.0要来了。
![](https://pic2.zhimg.com/v2-be119779dc5c30596592e38b9da54b7d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='876' height='236'></svg>)
昨天，谷歌大脑的Martin Wicke在一封公开邮件里，给这座即将造好的**里程碑**，做了个**预告**。

他说今年晚些时候，TensorFlow 2.0的一个预览 (Preview) 版本，就会和程序员们见面了。

在那之前，大家应该对这次重大更新，抱些怎样的**期待**？
![](https://pic2.zhimg.com/v2-6f671d5d2f51f570cecb2f3866996fcd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='261' height='193'></svg>)
邮件列举了以下几条：

· **Eager Execution**成为2.0的一个核心功能。这个命令式的编程环境，会让入门TensorFlow变得更容易。

· 支持更多**平台**、更多**语言**；通过**交换格式**的标准化以及API的协调，来提升组件之间的兼容性和对等性。

· 移除**已弃用**的API，并减少重复，也是减少用户的困惑。

往下，看看详情。

## **兼容性与连续性**

邮件写到，TensorFlow 2.0是一个**纠正错误**、**做出改进**的好机会。有些改善，在**语义化版本控制** (Semantic Versioning) 之下，通常是做不到的。
![](https://pic1.zhimg.com/v2-dee0f703bf84be227ccd26b281e83e70_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='309' height='163'></svg>)
﻿为了**顺滑地过渡**到新版本，团队会做一个转换工具，用于更新Python代码，这样便能使用TensorFlow 2.0兼容的API了。

如果遇到**无法自动转换**的情况，系统也会提醒人类。想当年，向1.0过渡的时候，类似的工具也帮了大忙。
![](https://pic4.zhimg.com/v2-9027728106d527315a0ce9b42162cd1f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='849' height='565'></svg>)
并非所有改变，都能自动完成。比如，新版本将会弃用一些API，而其中有些API并没有直接对等的替换。

针对这样的情况，TensorFlow会提供一个兼容模块 (tensorflow.compat.v1) ，里面有完整的**TensorFlow 1.x API**，并且在2.x的生命周期里，会得到**持续的维护**。 
![](https://pic4.zhimg.com/v2-57ebaa85cb87765a0f8f686109c18393_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='260'></svg>)
不过，一旦2.0正式版发布，1.x就不会再有任何**功能更新**了。但在那之后，团队还是会给最后一版1.x，提供**一年**的**安全补丁**。

## **磁盘兼容性**

SavedModel和GraphDef，都不会发生什么重大的变化。

但2.0意味着，原始**检查点** (Checkpoint) 里的变量名称，需要转换，才能跟新的模型兼容。

## **tf.contrib被砍了**

**contrib**模块的成长，超出了TensorFlow团队 (在一个repo里) 能维护的范围。

Wicke说，更大的项目，分开维护可能会更好。
![](https://pic4.zhimg.com/v2-c1120d7f38a83d889f0b4b7ef45b65df_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1060' height='600'></svg>)
不过，团队依然会在2.0里孵化一些**小型**的扩展。

所以，2.0的一个重大的变化，就是**tf.contrib**被**完全弃用**了。

在未来几个月，团队会与现有contrib模块的主人们，指定**迁移计划**，比如怎样在社区页面上或以文件的形式，发表自己的TensorFlow扩展。

每一个contrib模块，命运有三种：

**1.**集成到TensorFlow里。

**2.**移到一个单独的Repo里。

**3.**彻底移除。 
![](https://pic2.zhimg.com/v2-a69f3c557736d0fd1d511898fa1b9961_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='506'></svg>)
现在，新的tf.contrib项目，已经**停止添加**了。

老项目的主人/维护者，也要赶快跟TensorFlow团队联系了。

## **你有什么意见？**

重大革新发生之前，要听听广大**开发者**怎么说。

TensorFlow 2.0的开发，包含一个公开设计审查 (Public Design Reviews) 环节，大家都可以发表自己的意见。

获得参与感，请至以下邮箱：

developers@tensorflow.org

以及GitHub传送门：

[https://github.com/tensorflow/community/blob/master/governance/TF-RFCs.md](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/community/blob/master/governance/TF-RFCs.md)

对于新的TensorFlow，你还期待怎样的改变？
![](https://pic3.zhimg.com/v2-4fa73117813bba72e0f56bcdba96cb96_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='247'></svg>)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


