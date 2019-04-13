
# 2019年11个值得研究的Javascript机器学习库 - 喜欢打酱油的老鸟 - CSDN博客


2019年03月17日 17:48:14[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：252


[https://www.toutiao.com/a6669159846014616071/](https://www.toutiao.com/a6669159846014616071/)
虽然大部分机器学习都是用python这样的语言完成的，但在Javascript生态系统中，其前端和后端社区都很棒。这个有趣的交集促使我们探索并尝试了一起使用Javascript和机器学习的奇怪可能性。这里分享有一些有趣的库，它们将Javascript，机器学习，DNN甚至NLP结合在一起。
![2019年11个值得研究的Javascript机器学习库](http://p9.pstatp.com/large/pgc-image/548e562f9c554ebf8fd0f70777d62b08)

# 1. Brain.js
![2019年11个值得研究的Javascript机器学习库](http://p9.pstatp.com/large/pgc-image/ba3b1e85172447988f3a22a31801d27a)
Brain.js是神经网络的Javascript库，取代了（现已弃用的）“ 大脑 ”库，可以与Node.js一起使用或在浏览器中使用（笔记计算），并为不同的任务提供不同类型的网络。
Github地址：https://github.com/BrainJS/brain.js
这是一个训练网络识别颜色对比的演示。
![2019年11个值得研究的Javascript机器学习库](http://p1.pstatp.com/large/pgc-image/30bc91ca1d9e49f68cbc9010686688c8)

# 2. Synaptic
Synaptic是一个用于node.js和浏览器的Javascript神经网络库，它使您能够训练第一级甚至二级神经网络架构。该项目包括一些内置架构，如多层感知器，多层长短期记忆网络，液体状态机和能够训练真实网络的培训师。
Github地址：https://github.com/cazala/synaptic
![2019年11个值得研究的Javascript机器学习库](http://p3.pstatp.com/large/pgc-image/dc8f081a785c43de913fe7e19c76b606)

# 3. Neataptic
该库为浏览器和Node.js提供快速神经进化和反向传播，其中包括一些内置网络，包括感知器，LSTM，GRU，Nark等。这是一个简单培训的新手教程。
Github地址：https://github.com/wagenaartje/neataptic
![2019年11个值得研究的Javascript机器学习库](http://p3.pstatp.com/large/pgc-image/16229350dd8c483a97caa98fd34ae140)

# 4. ConvNetJS
由斯坦福大学博士开发，这个受欢迎的图书馆在过去的4年里一直没有得到维护，但绝对是名单上最有趣的项目之一。它是神经网络的Javascript实现，支持通用模块，分类，回归，实验强化学习模块，甚至能够训练处理图像的卷积网络。
Github地址：https://github.com/karpathy/convnetjs
它目前支持：
常见的神经网络模块（完全连接的层，非线性）
分类（SVM / Softmax）和回归（L2）成本函数
能够指定和训练处理图像的卷积网络
基于Deep Q Learning 的实验性强化学习模块
![2019年11个值得研究的Javascript机器学习库](http://p9.pstatp.com/large/pgc-image/67520cecadbe417c9f78104f3ffeac10)

# 5. WebDNN
这个日本制造的JavaScript库是为了在浏览器上快速运行深度神经网络预训练模型而构建的。由于在浏览器上执行DNN会消耗大量计算资源，因此该框架优化了DNN模型以压缩模型数据并通过JavaScript API（如WebAssembly和WebGPU）加速执行。
![2019年11个值得研究的Javascript机器学习库](http://p1.pstatp.com/large/pgc-image/58e89131914c43909e487b22a03cd371)
Github地址：https://github.com/mil-tokyo/webdnn
利用WebDNN做神经网络风格转移的例子：
![2019年11个值得研究的Javascript机器学习库](http://p9.pstatp.com/large/pgc-image/38e9f1c4340843c9982d8a00bc4eeec9)

# 6. Deeplearnjs
这个流行的库允许您在浏览器中训练神经网络或在推理模式下运行预先训练的模型，甚至声称它可以用作网络的NumPy。通过易于获取的API，该库可用于实用的有用应用程序，并且可以进行主动维护。
Github地址：https://github.com/tensorflow/tfjs-core
![2019年11个值得研究的Javascript机器学习库](http://p3.pstatp.com/large/pgc-image/e79ccf7e8b574817aaccf7e6f56bdf9e)
作为TensorFlow.js生态系统的一部分，它如今托管在@tensorflow/tfjs-core了，是TensorFlow.js核心API，它提供低级，硬件加速的线性代数运算和用于自动区分的急切API。
# 7. Tensorflow Deep Playground
Tensorflow Deep Playground是神经网络的交互式可视化，使用d3.js和TypeScript编写。虽然这个项目基本上包含了一个非常基本的张量流动操场，但它可以用于不同的手段或者用作不同目的的非常令人印象深刻的教育功能。
Github地址：https://github.com/tensorflow/playground
![2019年11个值得研究的Javascript机器学习库](http://p1.pstatp.com/large/pgc-image/f49850b628a64cb9a5635042d0d40195)

# 8. Compromise
这个非常受欢迎的库提供“javascript中适度的自然语言处理”。这是非常基本和直接的，甚至可以编译成一个小文件。出于某种原因，其适度的“足够好”的方法使其成为几乎任何需要基本NLP的应用程序的主要候选者。
Github地址：https://github.com/spencermountain/compromise
![2019年11个值得研究的Javascript机器学习库](http://p3.pstatp.com/large/pgc-image/e17c657429f240dfb1ad04244a0e122d)

# 9. Neuro.js
这个漂亮的项目是一个深度学习和强化学习Javascript库框架的浏览器。实现基于完整堆栈神经网络的机器学习框架以及扩展的强化学习支持，一些人认为该项目是convnetjs的继承者。
Github地址：https://github.com/janhuenermann/neurojs
![2019年11个值得研究的Javascript机器学习库](http://p9.pstatp.com/large/pgc-image/3d38fd3c273249fba9c99c5f23e6e626)

# 10. ml.js
一组存储库，为mljs组织开发的Javascript提供机器学习工具，包括监督和无监督学习，人工神经网络，回归算法和统计，数学等支持库。这是一个简短的演练。
Github地址：https://github.com/mljs
![2019年11个值得研究的Javascript机器学习库](http://p3.pstatp.com/large/pgc-image/99b21190711a4345842f62e121ab0f15)

# 11. Mind
Node.js和浏览器的灵活神经网络库，基本上学习如何进行预测，使用矩阵实现来处理训练数据并启用可配置的网络拓扑。您还可以即插即用已经学过的“思想”，这对您的应用程序非常有用。
Github地址：https://github.com/stevenmiller888/mind
![2019年11个值得研究的Javascript机器学习库](http://p1.pstatp.com/large/pgc-image/6c9f8249320143c8bc576e4bea077653)

