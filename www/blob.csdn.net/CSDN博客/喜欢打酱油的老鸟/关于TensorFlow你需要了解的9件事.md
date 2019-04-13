
# 关于TensorFlow你需要了解的9件事 - 喜欢打酱油的老鸟 - CSDN博客


2018年09月03日 13:35:00[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：90标签：[TensorFlow																](https://so.csdn.net/so/search/s.do?q=TensorFlow&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


关于TensorFlow你需要了解的9件事
https://www.cnblogs.com/DicksonJYL/p/9577984.html
**摘要**：本文对近期在旧金山举办的谷歌 Cloud Next大会上有关TensorFlow的一些特点进行了总结。

**1****、它是一个强大的机器学习框架**
TensorFlow是一个基于数据流图的机器学习框架，它是Google Brain的第二代机器学习系统，常被应用于各种感知、语言理解、语音识别、图像识别等多项机器深度学习领域。Tensor（张量）代表了N维数组，Flow（流）代表了基于数据流图的计算。
如果你有足够多的数据，而且正处在人工智能的深度学习、神经网络、高级人工智能的阶段，那么它可能就会成为你最好的帮手了。TensorFlow不是一个工具，而是一个框架，假如你想通过20×2的电子表格返回一条回归线，那么现在你可以停止学习开始使用了。
当你想要得到的成就比较高端宏大，那么你就一定会兴奋了，TensorFlow已经被应用在太空领域来寻找新的行星，可以通过帮助医生筛查糖尿病视网膜病变来预防失明，而且还可以通过警告非法砍伐森林的活动来帮助拯救森林。AlphaGo和Google Cloud Vision建在了TensorFlow上面，这是你需要关注的。另外，TensorFlow是开源的，大家可以免费下载并随时使用。
在TensorFlow的帮助下发现的系外恒星Kepler-90i，使得KePLer-90星系成为我们已知唯一的太阳系外的，并且有八颗行星围绕着它不停运行的星系。还没有任何其它的星系有超过八颗行星。
TensorFlow下载地址：
https://www.tensorflow.org/install/
TensorFlow初始教程：
https://www.datacamp.com/community/tutorials/tensorflow-tutorial

**2****、方法是可选的**
如果你之前曾经尝试使用过TensorFlow，然后就被吓得没再用了。因为它逼的你像一个学术研究者一样，而不是像一个开发者那样，不过目前，有了更多的选择，所以就赶紧回来继续用吧。
TensorFlow eager execution可以让你像一个Python程序员那样与系统进行交互：所有的即时编码和调试都是按行执行的，而不是像其它语言那样编写大段的程序代码块令人畏惧。我是一个学术研究的人，但我从一开始就喜欢上了TensorFlow eager execution，因此就尽快开始用吧。
![](https://img-blog.csdn.net/20180903133147208?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**3****、支持逐行构造神经网络**
Keras + TensorFlow =便捷构建神经网络
Keras是一个基于TensorFlow的深度学习库，它由纯Python编写而成，其特点是对用户友好的，并且能够提供简易和快速的原型设计，这对一些低版本的TensorFlow会有更多的帮助。如果你喜欢面向对象的思维方法，并且更愿意一次构建一层神经网络，那么你就会彻底喜欢上TensorFlow.keras。在下面几行的代码中，我们创建了一个连贯的神经网络，并带有好似走了音的标准铃声和口哨声。
![](https://img-blog.csdn.net/20180903133206687?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**4****、不仅仅是Python**
可能你现在一直在抱怨TensorFlow对Python的偏执。现在好了，TensorFlow不再仅仅针对Python的开发人员了。它现在支持运行在多种语言环境中，从R到Swift再到JavaScript，所支持的语言请见下图：
![](https://img-blog.csdn.net/20180903133247402?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**5****、可以在浏览器中做任何事情**
提到JavaScript，你可以用TensorFlow.js在浏览器中训练和执行模型。
![](https://img-blog.csdn.net/20180903133304685?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
利用TensorFlow.js在浏览器中进行实时人体姿态评估。打开你的相机，并请看这个示例。
**6****、给微型设备一个简单的版本**
TensorFlow Lite可以让模型在各种设备中执行，个人电脑或者服务器的单个或多个CPU或GPU上，甚至是移动设备和物联网（IoT）上。TensorFlow会给你带来超过原来3倍的性能，它对线程、队列和异步计算具有很好的支持，最大程度的利用现有可用的硬件，可以自由的将TensorFlow数据流图中计算元素分配到不同的设备上，让TensorFlow来处理副本。现在你就可以在Raspberry Pi电脑或手机上开始机器学习了。在大会上的演讲中，劳伦斯做了一件勇敢的事情，在成千上万的人面前，通过在一个Android仿真模拟器上进行了图像分类，的确效果不错。
![](https://img-blog.csdn.net/20180903133321399?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**7****、专业设备更好一些**
如果你厌倦了一直等待CPU完成用提供的数据来训练神经网络的工作，那么你现在就可以用Cloud TPUs给这项工作专门设计的硬件了。就在几周前，谷歌在Alpha平台上发布了第3代的TPU（张量处理单元），它是一个专门为机器学习和TensorFlow定制的ASIC（集成电路芯片技术）。TPU是一个可编程的人工智能加速器，提供高吞吐量的低精度计算（如8位），面向使用或运行模型而不是训练模型。
![](https://img-blog.csdn.net/2018090313335043?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**8****、新的数据管道提高显著**
你现在在数据管道方面还在用numpy吗？假如你想用TensorFlow的话，目前tf.data namespace可以使你在TensorFlow的输入处理上更具表现力。tf.data可以为您提供快速、灵活、易于使用的数据管道，同时还提供同步的训练。
![](https://img-blog.csdn.net/20180903133417750?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**9****、你不需要从头开始**
都知道从头开始机器学习不是什么有趣的方式。打开编辑器，里面只有一个空白的新页面，也没有实例代码。这时候，你可以使用TensorFlow Hub，就可以继续一个比较古老的习惯，就是用别人的代码来帮助自己编写代码，并称之为是你自己的代码。
![](https://img-blog.csdn.net/20180903133441968?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
文章原标题《9 Things You Should Know About TensorFlow》



