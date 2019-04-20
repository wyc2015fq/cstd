# Google正式发布TensorFlow Lite预览版，针对移动/嵌入设备的轻量级解决方案 - 人工智能学家 - CSDN博客
2017年11月16日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：110
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWRBVicF9p6unIbnARWyJO4xySScNUvx5BdvR2mUMvpCHVMv9g3PMSSd1dMSRDApFZPeltVhTibfrTg/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
*来源：AI科技评论*
*概要：日前，谷歌正式发布 TensorFlow Lite 开发者预览版，这是针对移动和嵌入式设备的轻量级解决方案。*
日前，谷歌正式发布 TensorFlow Lite 开发者预览版，这是针对移动和嵌入式设备的轻量级解决方案。TensorFlow Lite 是一种全新的设计，具有三个重要特征——轻量级（Lightweight）、跨平台（Cross-platform）、快速（Fast）。
谷歌于今天正式发布 TensorFlow Lite 开发者预览版，这是针对移动和嵌入式设备的轻量级解决方案。TensorFlow 可以在许多平台上运行，从机架上大量的服务器到小型的物联网设备，但近几年，随着大家使用的机器学习模型呈指数级增长，因此需要将训练模型的平台扩展到移动和嵌入式设备上。TensorFlow Lite 支持设备上机器学习模型的低时延推理。
TensorFlow Lite 是一种全新的设计，它支持以下功能：
**轻量级（Lightweight）**：支持机器学习模型的推理在较小二进制数下进行，能快速初始化/启动。
**跨平台（Cross-platform）**：可以在许多不同的平台上运行，现在支持 Android 和 iOS
**快速（Fast）**：针对移动设备进行了优化，包括大大减少了模型加载时间、支持硬件加速。
如今，越来越多的移动设备中含有专用的定制硬件来更高效地进行机器学习。TensorFlow Lite 支持 Android 神经网络API（Android Neural Networks API），大家在使用 TensorFlow Lite 时可以利用这些有用的加速器。
当加速器（硬件设备）不可用时，TensorFlow Lite 会返回到 CPU 来执行，这将保证模型仍然可以在一大批设备上快速运行。
**结构**
下图是 TensorFlow Lite 的结构设计：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmiaGibZTuyZN6SyA0yPianWNxSiatvgu3gUia7gqibJBDCaJxAHRoptTxClx4xiaVXQfEUqdoLZ5a4MXFLHA/640?wx_fmt=jpeg)
**模块如下:**
TensorFlow Model: 存储在硬盘上已经训练好的 TensorFlow 模型
TensorFlow Lite Converter: 将模型转换为 TensorFlow Lite 文件格式的程序。
TensorFlow Lite Model File: 基于 FlatBuffers 的模型文件格式，针对速度和大小进行了优化。
可以将 TensorFlow Lite Model File 部署到 Mobile App ，如上图中所示：
Java API: 处于 Android App 中 C++ App 上，方便封装。
C++ API: 加载 TensorFlow Lite Model File，调用解释器（Interpreter）。
上面的这两个库在 Android 和 iOS 端都可用。
Interpreter：使用一组运算符来执行模型。运算符可以选择，如果不含运算符，只有70KB，加载所有的运算符之后为300KB。比起需要1.5 M(使用一组正规的操作符)的 TensorFlow Mobile，能使容量大大减小。
在 Android 设备上，Interpreter 支持 Android神经网络API，可以用它进行硬件加速。如果没有可用的加速器，则默认使用CPU。
开发人员还可以使用C++ API来自定义 kernel。
**模型**
TensorFlow Lite 目前支持很多针对移动端训练和优化好的模型。
MobileNet：能够识别1000种不同对象类的视觉模型，为实现移动和嵌入式设备的高效执行而设计。
Inception v3：图像识别模型，功能与 MobileNet 相似，它提供更高的精度，但相对来说更大。
Smart Reply： 设备对话模型，可以即时回复聊天消息，在 Android Wear 上有使用这一功能。
Inception v3 和 MobileNets 已经在 ImageNet 数据集上训练了。大家可以利用迁移学习来轻松地对自己的图像数据集进行再训练。
**关于TensorFlow Mobile**
正如大家知道的那样，TensorFlow 可以通过 TensorFlow Mobile API 对模型进行移动和嵌入式部署。展望未来，TensorFlow Lite 应该被看作是 TensorFlow Mobile 的升级。随着一步步的成熟，它将成为在移动和嵌入式设备上部署模型的推荐解决方案。
TensorFlow Lite 目前是预览版，大家仍然可以使用 TensorFlow Mobile。
TensorFlow Lite 的功能有很多，目前仍处于紧锣密鼓的开发阶段。这次的发布中，谷歌特意使用受限平台，来保证一些最重要的常见模型的性能不受到影响。
谷歌计划根据用户的需要来考虑未来优先扩展的功能，其开发目标是简化开发人员的体验，并让模型能部署到一系列移动和嵌入式设备上。
很高兴开发者也在帮助TensorFlow Lite项目的顺利进行。谷歌将会以与TensorFlow项目相同的热情来支持和启动TensorFlow Lite社群。欢迎大家来使用TensorFlow Lite。
**更多相关发布**
作为软件资源库的一部分，谷歌也发布了一个可以运行在设备上的聊天模型以及一个demo app，它们是谷歌编写的运行在TensorFlow Lite上的自然语言应用的样例，供开发人员和研究者们研究学习、开发更多新的本地运行的机器智能功能。输入聊天对话消息以后，这个模型就可以生成一条建议的回复；它的推理过程非常高效，可以轻松嵌入到各种聊天软件中，利用设备自身的计算能力提供智能的聊天功能。
谷歌发布的这个本地运行的聊天模型运用了一种训练紧凑神经网络（以及其它机器学习模型）的新机器学习架构，它基于一个联合优化范式，最初发表在论文 ProjectionNet: Learning Efficient On-Device Deep Networks Using Neural Projections 中。这种架构可以高效地运行在计算能力和内存都较为有限的移动设备上，通过高效的“投影”操作，它可以把任意输入转换成一个紧凑的位向量表征，这个过程中类似的输入会被投影到相邻的向量中；根据投影类型的不同这些向量可以是密集的也可以是稀疏的。比如，“嘿如何了？”和“兄弟你如何了？”两条消息就有可能被投影到相同的向量表征上去。
通过这样的想法，谷歌的聊天模型就以很低的计算开销和内存消耗加入了这些高效的操作。这个在设备本地运行的模型是谷歌用端到端的方法训练的，训练过程中使用了联合训练两个不同模型的机器学习框架；这两个训练的模型，一个是紧凑的“投影”模型（如上文所述），同时还结合了一个“训练器”模型。两个模型是联合训练的，投影模型从训练器模型中学习；训练器模型有着专家的特质，它是用更大、更复杂的机器学习架构创建的，而投影模型就像一个跟在后面学习的学生。在训练过程中，也可以叠加其它的量化、蒸馏之类的技术，达到更紧凑的压缩效果，或者也可以选择性地优化目标函数的某些部分。一旦训练结束，这个更小的投影模型就可以直接在设备上做推理任务。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmiaGibZTuyZN6SyA0yPianWNxSAdcibsYkfr8ObL4ntAykSWkqiaXvY4yD5iaB43a8jUib26OI7wh5CKWfpQ/640?wx_fmt=png)
在推理过程中，训练后的投影模型会被编译成一系列 TensorFlow Lite 的操作，而这些操作都是为移动平台的快速执行优化过的，可以直接在设备上执行。这个本地运行的聊天模型的TensorFlow Lite推理图如下所示。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmiaGibZTuyZN6SyA0yPianWNxSI6OAUV7nZrsVreIU0L9zQNmaYoeY7aD3LwasJvOQIqYNecfRYxo4bw/640?wx_fmt=png)
这个用上述的联合训练方法端到端训练的聊天模型是开源的，今天（美国时间11月14日）就会和代码一起发布出来。同时还会发布一个demo app，这样研究人员和开发人员就可以轻松地下载它们、在自己的移动设备上试试看它提供的一键智能回复功能。这个架构能根据应用需求提供不同模型大小、不同预测质量的配置功能，操作也很方便。除了一些已知的模型可以给出很好回复的消息之外，系统还可以把一组固定的聊天对话中观察到、然后学习编译到模型中的流行的回复语句作为预测失败后的备选语句。它背后的模型和谷歌在自家应用中提供“智能回复”功能的模型有一些区别。
### **在聊天模型之后**
有趣的是，上面描述的机器学习架构保证了背后隐含的模型具有各种灵活的选择。谷歌的研究人员们把这个架构设计得可以与不同的机器学习方法兼容，比如，与TensorFlow深度学习共同使用时，就可以为隐含模型学到一个轻量化的神经网络（“投影网络”），并用一个图框架（“投影图”）来表征这个模型，不再是神经网络的形式。
联合训练框架也可以用来给使用其它机器学习建模架构的任务训练轻量级的本地运行模型。比如，谷歌把一个复杂的前向传输或者循环网络架构（比如LSTM）作为训练器模型，训练得到的投影架构就可以简单地由动态投影操作和寥寥几层全连接层组成。整个架构是以端到端的方式在TensorFlow中通过反向传播训练的。训练结束后，紧凑的投影网络就可以直接用来做推理。通过这样的方法，谷歌的研究人员们成功训练了不少小巧的投影模型，它们不仅在模型大小方面有大幅度下降（最高可以缩小几个数量级），而且在多种视觉和语言分类任务中可以保证同样的准确率但性能高得多。类似地，他们也用图学习范式训练了其它的轻量级模型，即便是在半监督学习的设定中。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmiaGibZTuyZN6SyA0yPianWNxS3cfzSo4u2K0GWyaMo5JOMqSRZiaDFvjejdLyAuxnIiaV2kn16dTpcySQ/640?wx_fmt=png)
谷歌表示，他们在开源TensorFlow Lite后会持续改进以及发布新版本。通过这些机器学习架构学到的模型，不管是已经发布的还是将在未来发布的，都不仅可以应用在多种自然语言和计算机视觉应用中，也可以嵌入已有的应用中提供机器智能的功能。同时谷歌当然也希望及机器学习和自然语言处理大家庭中的其它研究者和开发者也可以在这些基础上共同应对谷歌尚未发现或者尚未解决的新问题。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBULg0PyXjjVDR3OiaCudIgwDjRdBUkpx1Dw58Xa9VkyJUuH0piaT7Qyem2gHwfTj6ic45jwaL7y3Zdaw/0?wx_fmt=png)
