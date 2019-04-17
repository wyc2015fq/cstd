# 从Brain.js到Mind，一文收录11个移动端Javascript机器学习库 - 知乎
# 



文章选自[http://blog.bitsrc.io](https://link.zhihu.com/?target=http%3A//blog.bitsrc.io)，**作者：Jonathan Saring，机器之心编译。**

> 本文作者在构建 Bit 的过程中探索和尝试了把 Javascript 和机器学习结合起来使用的可能性，并由此发现了一些简洁优雅的库，可以把 Javascript、机器学习、DNN 甚至 NLP 整合起来。

「等等，什么？？这是一个可怕的想法！」

当我第一次和我们的 NLP 主要研究人员谈起这个概念时，她的原话是这样的。可能她是对的，但它也是一个非常有趣的概念，最近在 Javascript 领域得到了越来越多的关注。

在过去的一年中，我们的团队正在构建 Bit（[https://bitsrc.io/](https://link.zhihu.com/?target=https%3A//bitsrc.io/)），它使得用组件构建软件变得更加简单。作为工作的一部分，我们开发了 ML 和 NLP 算法，来更好地理解代码是如何编写、组织和使用的。

虽然大多数工作都是用 Python 这样的语言完成的，但 Bit 位于 Javascript 生态系统中，它的前、后端社区都是如此。

这个有趣的交集让我们探索和尝试了一起使用 Javascript 和机器学习的奇怪可能性。通过我们的研究发现，这里有一些简洁的库，可以将 Javascript、机器学习、DNN 甚至 NLP 结合在一起。




**1. Brain.js**

Brain.js 是一个用于神经网络的 Javascript 库，用于代替 (现在已经弃用的）「brain」库，它可以和 Node.js 一起使用，或在 browser（注意计算）中使用，并为不同任务提供不同类型的网络。以下是训练网络来识别色彩对比的例子。

链接：[https://github.com/BrainJS/brain.js](https://link.zhihu.com/?target=https%3A//github.com/BrainJS/brain.js)
![](https://pic4.zhimg.com/v2-f91d38ae12e60b51575ec1453268a5bb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='468' height='378'></svg>)训练 Brain.js 识别色彩对比
**2. Synaptic**

Synaptic 是一个用于 node.js 和浏览器的 Javascript 神经网络库，它使你能够训练一阶甚至是二阶神经网络结构。该项目包括一些内置的体系结构，如多层感知机、多层长短期记忆网络、液体状态机和能够训练真实网络的训练器。

链接：[https://github.com/cazala/synaptic](https://link.zhihu.com/?target=https%3A//github.com/cazala/synaptic)
![](https://pic3.zhimg.com/v2-989a4f9f8df2c96d4a33ec01893df6ba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='413' height='502'></svg>)Synaptic 图像过滤感知器
**3. Neataptic**

这个库为浏览器和 Node.js 提供快速的神经元进化和反向传播，并且有一些内置的网络，包括感知器、LSTM、GRU、Nark 等等。这里是一个简单训练的新手教程：[https://wagenaartje.github.io/neataptic/docs/tutorials/training/](https://link.zhihu.com/?target=https%3A//wagenaartje.github.io/neataptic/docs/tutorials/training/)。

链接：[https://github.com/wagenaartje/neataptic](https://link.zhihu.com/?target=https%3A//github.com/wagenaartje/neataptic)
![](https://pic4.zhimg.com/v2-38037d79290d01e339f9ee074947b17b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='671' height='500'></svg>)目标寻求 AI 演示
**4. Conventjs**

这个由斯坦福大学博士开发的流行库，在过去的 4 年里一直没有维护，但它是列表上最有趣的项目之一。它是神经网络的 Javascript 实现，支持通用模块、分类、回归、一个试验性的强化学习模块，甚至能够训练处理图像的卷积网络。

链接：[https://github.com/karpathy/convnetjs](https://link.zhihu.com/?target=https%3A//github.com/karpathy/convnetjs)
![](https://pic4.zhimg.com/v2-4b1178120402ec4c1d395eeafaba013b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='847' height='472'></svg>)用 2 层神经网络进行玩具 2D 分类的 Conventjs 演示
**5. Webdnn**

这个日本制造的库用于在浏览器上运行深度神经网络预训练模型，并且运行速度很快。由于在浏览器上运行 DNN 会消耗大量计算资源，因此该框架优化了 DNN 模型来压缩模型数据，并通过 JavaScript APIs（如 WebAssembly 和 WebGPU）加速执行。

链接：[https://github.com/mil-tokyo/webdnn](https://link.zhihu.com/?target=https%3A//github.com/mil-tokyo/webdnn)
![](https://pic1.zhimg.com/v2-1ab457f8eefdf619dfcdf3f9e774cf04_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='732' height='416'></svg>)



**6. Deeplearnjs**

这个流行的库允许你在浏览器中训练神经网络，或者在推理模式下运行预训练模型，甚至声称它可以用作网页版 NumPy。通过易于读取的 API，该库可用于有用应用程序的真实性，并被积极地维护。

链接：[https://github.com/tensorflow/tfjs-core](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tfjs-core)
![](https://pic2.zhimg.com/v2-d4133ed7e3f4cfd75532b6ebb47c0705_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='529'></svg>)



**7. Tensorflow Deep Playground**

Deep playground 是神经网络的交互式可视化，使用 d3.js 在 TypeScript 中编写。虽然这个项目包含了一个非常基本的 tensorflow playground，但它可以被用于不同的目的，或用作令人印象深刻的不同用途的教育功能。

链接：[https://github.com/tensorflow/playground](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/playground)
![](https://pic1.zhimg.com/v2-c730979b1f9eb6fbf437a9694faef3a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='505'></svg>)Tensorflow playground
**8. Compromise**

这个非常流行的库提供了「在 JavaScript 中适度的自然语言处理」。它非常基本和直接，甚至可以编译成一个小文件。出于某种原因，它的适度的「足够好」的方法使其成为几乎所有需要基本 NLP 的应用程序的首选。

链接：[https://github.com/spencermountain/compromise](https://link.zhihu.com/?target=https%3A//github.com/spencermountain/compromise)
![](https://pic2.zhimg.com/v2-da0ce3d28db980072a140c9372602e31_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='788' height='538'></svg>)Compromise 提醒我们英语真的很简单
**9. Neuro.js**

这个项目很棒，它为浏览器提供一个深度学习和强化学习的 Javascript 库框架。它在扩展强化学习支持下，实现一个基于全栈神经网络的机器学习框架，有些人认为这个项目是 convnet.js 的继承者。

链接：[https://github.com/janhuenermann/neurojs](https://link.zhihu.com/?target=https%3A//github.com/janhuenermann/neurojs)
![](https://pic2.zhimg.com/v2-68efed380e2a454de9583fc15ae012b1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='894' height='596'></svg>)
**10. mljs**

由 mljs 组织开发的一组库，为 Javascript 提供机器学习工具，它包括监督和非监督学习、人工神经网络、回归算法和用于统计、数学等的支持库。这里有一个简短的指导：[https://hackernoon.com/machine-learning-with-javascript-part-1-9b97f3ed4fe5。](https://link.zhihu.com/?target=https%3A//hackernoon.com/machine-learning-with-javascript-part-1-9b97f3ed4fe5%25E3%2580%2582)

链接：[https://github.com/mljs](https://link.zhihu.com/?target=https%3A//github.com/mljs)
![](https://pic1.zhimg.com/v2-7f6ce46fba2cb42452b0b4402b80b4b0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1046' height='606'></svg>)



**11. Mind**

一个用于 Node.js 和浏览器的灵活的神经网络库，主要学习做预测，使用矩阵来处理训练数据并启用可配置的网络拓扑。你还可以即插即用已学习的「Mind」，这对你的应用很有用。

链接：[https://github.com/stevenmiller888/mind](https://link.zhihu.com/?target=https%3A//github.com/stevenmiller888/mind)
![](https://pic2.zhimg.com/v2-6215956305de29e91a291bfc6ee5e405_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='609' height='611'></svg>)



**其他重要的库：**




**Natural**

Node.js 的一个积极维护的库，它提供标记法、词干提取（减少不必要的词根）、分类、语音学、tf-idf、WordNet、字符串相似度等。

链接：[https://github.com/NaturalNode/natural](https://link.zhihu.com/?target=https%3A//github.com/NaturalNode/natural)




**Incubator-mxnet**

Apache MXNet 是一个深度学习框架，它允许你将符号和命令式编程与图形优化层在线结合起来以提高性能。MXnet.js 为浏览器带来了一个深度学习推理 API。

链接：[https://github.com/apache/incubator-mxnet](https://link.zhihu.com/?target=https%3A//github.com/apache/incubator-mxnet)




**Keras JS**

该库在浏览器中运行 Keras 模型，使用 WebGL 并支持 GPU。由于 Keras 使用了许多框架作为后端，所以模型也可以在 TensorFlow、CNTK 和其他框架中进行训练。

链接：[https://github.com/transcranial/keras-js](https://link.zhihu.com/?target=https%3A//github.com/transcranial/keras-js)




**Deepforge**

一个深度学习的开发环境，它使你能够快速设计神经网络结构和机器学习管道，并使用内置版本控制再现实验。值得一试。

链接：[https://github.com/deepforge-dev/deepforge](https://link.zhihu.com/?target=https%3A//github.com/deepforge-dev/deepforge)




**Land Lines**

与其说它是一个库，不如说是一个基于谷歌 Chrome 实验的非常酷的演示/网页游戏。尽管我不确定该如何处理，但它肯定会为你带来一天中最愉快的 15 分钟。

链接：[https://lines.chromeexperiments.com/](https://link.zhihu.com/?target=https%3A//lines.chromeexperiments.com/)
![](https://pic4.zhimg.com/v2-5a39d8c1a16b1fcf7a6d81a8f7334b6b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='833' height='436'></svg>)Google 提供的 Land lines
**下一步是什么？**

显然，Javascript 远没有成为机器学习选择的语言。然而，诸如性能、矩阵操作和丰富有用的库等常见问题正在慢慢消失，缩小了常见应用和有用的机器学习之间的差距。




原文链接：[https://blog.bitsrc.io/11-javascript-machine-learning-libraries-to-use-in-your-app-c49772cca46c](https://link.zhihu.com/?target=https%3A//blog.bitsrc.io/11-javascript-machine-learning-libraries-to-use-in-your-app-c49772cca46c)




