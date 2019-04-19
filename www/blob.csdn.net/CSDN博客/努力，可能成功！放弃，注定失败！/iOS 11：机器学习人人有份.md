# iOS 11：机器学习人人有份 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年07月13日 16:48:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：313

Core ML
> - 原文地址：[iOS 11: Machine Learning for everyone](http://machinethink.net/blog/ios-11-machine-learning-for-everyone/)
- 原文作者：[Matthijs Hollemans](https://twitter.com/mhollemans)
- 译文出自：[掘金翻译计划](https://github.com/xitu/gold-miner)
- 译者：[Changkun Ou](https://github.com/changkun/)
- 校对者：[wilsonandusa](https://github.com/wilsonandusa)[atuooo](https://github.com/atuooo)
**本文同步发布于公众号：AIMaster**。在这里，我们一起学习 AI.
WWDC 2017 使一件事情变得非常清楚，那就是：Apple 正在全力以赴地支持「**设备上的机器学习**」了。
他们希望 App 的开发者们能够尽可能的简单的加入他们的行列中。
Apple 去年发布了可以用于创建基本的卷积神经网的 Metal CNN 和 BNNS 框架。今年，Metal 得到了进一步扩展，增加了一个全新的计算机视觉框架，以及
**Core ML**：一个能够轻松地将机器学习集成到 App 中的工具包。
![](https://user-gold-cdn.xitu.io/2017/6/28/e05cc2ef96011b03d2bb8727cf8b0a64)
Core ML framework
在这片文章中，我将就 iOS 11 和 macOS 10.13 中这些新推出的机器学习的内容，分享我自己的一些想法和经验。
## Core ML
Core ML 在 WWDC 上获得了极大的关注度，原因很简单：大部分开发者希望能够在他们的 App 中使用这个框架。
Core ML 的 API 非常简单。你只能用它做这些事情：
- 加载一个训练好的模型
- 做出预测
- 收益！！！
这看起来好像很有限，但实际上你一般只会在 App 中加载模型和做出预测这两件事。
在 Core ML 之前，加载训练好的模型是非常困难的 —— 实际上，我写过[一个框架](http://github.com/hollance/Forge)来减轻这种痛苦。所以现在我对这一个简单的两步过程感到非常高兴。
模型被包含在了一个 **.mlmodel** 的文件中。这是一种新的[开源文件格式](https://pypi.python.org/pypi/coremltools)，用于描述模型中的 layer、输入输出、标签，以及需要在数据上产生的任何预处理过程。它还包括了所有的学习参数（权重和偏置）。
使用模型所需的一切都在这一个文件里面了。
你只需要将 mlmodel 文件放入你的项目中，Xcode 将会自动生成一个 Swift 或 Objective-C 的包装类，使你能简单的使用这个模型。
举个例子，如果你把文件 **ResNet50.mlmodel** 添加到你的 Xcode 项目中，那么你就可以这么写来实例化这个模型：
`let model = ResNet50()`
然后做出预测：
```
let pixelBuffer: CVPixelBuffer = /* your image */if let prediction = try? model.prediction(image: pixelBuffer) {
  print(prediction.classLabel)
}
```
这差不多就是所有要写的东西了。你不需要编写任何代码来加载模型，或者将其输出转换成可以从 Swift 直接使用的内容 —— 这一切都将由 Core ML 和 Xcode 来处理。
**注意:** 要了解背后发生了什么，可以在 Project Navigator 里选择 **mlmodel** 文件，然后点击 Swift generated source 右边的箭头按钮，就能够查看生成的帮助代码了。
Core ML 将决定自己到底是在 CPU 上运行还是 GPU 上运行。这使得它能够充分的利用可以用的资源。Core ML 甚至可以将模型分割成仅在 GPU 上执行的部分（需要大量计算的任务）以及 CPU 上的其他部分（需要大量内存的任务）。
Core ML 使用 CPU 的能力对于我们开发者来说另一个很大的好处是：你可以从 iOS 模拟器运行它，从而运行那些对于 Metal 来说做不到，同时在单元测试中也不太好的任务。
### Core ML 支持什么模型？
上面的 ResNet50 例子展示的是一个图像分类器，但是 Core ML 可以处理几种不同类型的模型，如：
- 支持向量机 SVM
- 诸如随机森林和提升树的决策树集成
- 线性回归和 logistic 回归
- 前馈神经网、卷积神经网、递归神经网
所有这些模型都可以用于回归问题和分类问题。此外，你的模型可以包含这些典型的机器学习预处理操作，例如独热编码（one-hot encoding）、特征缩放（feature scaling）、缺失值处理等等。
Apple 提供了很多已经训练好的模型[可供下载](http://developer.apple.com/machine-learning/)，例如 Inception v3、ResNet50 和 VGG16 等，但你也可以使用
[Core ML Tools](https://pypi.python.org/pypi/coremltools) 这个 Python 库来转换自己的模型。
目前，你可以转换使用 Keras、Caffe、scikit-learn、XGBoost 和 libSVM 训练的模型。转换工具只会支持具体指定的版本，比如 Keras 支持 1.2.2 但不支持 2.0。辛运的是，该工具是开源的，所以毫无疑问它将来会支持更多的训练工具包。
如果这些都不行，你还是可以随时编写自己的转换器。**mlmodel** 文件格式是开源且可以直接使用的（由 Apple 制定发布的一种 protobuf 格式）
### 局限
如果你想在你的 App 上马上运行一个模型， Core ML 很不错。然而使用这样一个简单的 API 一定会有一些限制。
- 仅支持**有监督**学习的模型，无监督学习和增强学习都是不行的。（不过有一个「通用」的神经网络类型支持，因此你可以使用它）
- 设备上不能进行训练。你需要使用离线工具包来进行训练，然后将它们转换到 Core ML 格式。
- 如果 Core ML 不支持某种类型的 layer，那么你就不能使用它。在这一点上，你**不能**使用自己的 kernel 来扩展 Core ML。在使用 TensorFlow 这样的工具来构建通用计算图模型时，mlmodel 文件格式可能就不那么灵活了。
- Core ML 转换工具只支持**特定版本**的数量有限的训练工具。例如，如果你在 TensorFLow 中训练了一个模型，则无法使用此工具，你必须编写自己的转换脚本。正如我刚才提到的：如果你的 TensorFlow 模型具有一些 mlmodel 不支持的特性，那么你就不能在 Core ML 上使用你的模型。
- 你不能查看**中间层**的输出，只能获得最后一层网络的预测值。
- 我感觉下载模型更新会造成一些问题，如果你不想每次重新训练模型的时候都重写一个新版本的 App，那么 Core ML 不适合你。
- Core ML 对外屏蔽了它是运行在 CPU 上还是 GPU 上的细节 —— 这很方便 —— 但你必须相信它对你的 App 能做出正确的事情。即便你真的需要，你也不能强迫 Core ML 运行在 GPU 上。
如果你能够忍受这些限制，那么 Core ML 对你来说就是正确的选择。
否则的话，如果你想要完全的控制权，那么你必须使用 Metal Performance Shader 或 Accelerate 框架 —— 甚至一起使用 —— 来驱动你的模型了！
当然，真正的黑魔法不是 Core ML，而是你的模型。**如果你连模型都没有，Core ML 是没有用的**。而设计和训练一个模型就是机器学习的难点所在……
### 一个快速示例程序
我写了一个使用了 Core ML 的简单的示例项目，和往常一样，你可以在 GitHub 上找到[源码](https://github.com/hollance/MobileNet-CoreML)。
![](https://user-gold-cdn.xitu.io/2017/6/28/947f45c259b265eeb15ac8b7c2985155)
The demo app in action
这个示例程序使用了 [MobileNet](https://arxiv.org/abs/1704.04861v1) 架构来分类图片中的猫。
最初这个模型是[用 Caffe 训练](https://github.com/shicai/MobileNet-Caffe)得出的。我花了一点时间来搞清楚如何将它转换到一个 mlmodel 文件，但是一旦我有了这个转换好的模型，便很容易集成到 App 中了（[转换脚本](https://github.com/hollance/MobileNet-CoreML/blob/master/Convert/coreml.py)包含在
 GitHub 中）。
虽然这个 App 不是很有趣 —— 它只输出了一张静态图片的前五个预测值 —— 但却展示了使用 Core ML 是多么的简单。几行代码就够了。
**注意:** 示例程序在模拟器上工作正常，但是设备上运行就会崩溃。继续阅读来看看为什么会发生这种情况 ;-)
当然，我想知道发生了什么事情。事实证明 **mlmodel** 实际上被编译进应用程序 bundle 的 **mlmodelc** 文件夹中了。这个文件夹里包含了一堆不同的文件，一些二进制文件，一些 JSON文件。所以你你可以看到 Core ML 是如何将 mlmodel 在实际部署到应用中之前进行转换的。
例如，MobileNet Caffe 模型使用了批量归一化（Batch Normalization）层，我验证了这些转换也存在于 **mlmodel** 文件中。但是在编译的 mlmodelc 中，这些批量归一化 layer 似乎就被移除了。这是个好消息：Core ML 优化了该模型。
尽管如此，它似乎可以更好的优化该模型的结构，因为 **mlmodelc** 仍然包含一些不必要的 scaling layer。
当然，我们还处在 iOS 11 beta 1 的版本，Core ML 可能还会改进。也就是说，在应用到 Core ML 之前，还是值得对模型进一步优化的 —— 例如，[通过「folding」操作对 layer 进行批量归一化（Batch Normalization）](http://machinethink.net/blog/object-detection-with-yolo/#converting-to-metal)
 —— 但这是你必须对你的特性模型进行测量和比较的东西。
还有其他一些你必须检查的：你的模型是否在 CPU 和 GPU 上运行相同。我提到 Core ML 将选择是否在 CPU 上运行模型（使用 Accelerate 框架）或 GPU（使用 Metal ）。事实证明，这两个实现可能会有所不同 —— 所以你两个都需要测试！
例如，MobileNet 使用所谓的「depthwise」卷积层。原始模型在 Caffe 中进行训练，Caffe 通过使正常卷积的 `groups` 属性等于输出通道的数量来支持 depthwise 卷积。所得到的
**MobileNet.mlmodel** 文件也一样。这在 iOS 模拟器中工作正常，但它在设备上就会崩溃！
发生这一切的原因是：模拟器使用的是 Accelerate 框架，但是该设备上使用的却是 Metal Performance Shaders。由于 Metal 对数据进行编码方式的特殊性，
`MPSCNNConvolution` 内核限制了：不能使 groups 数等于输出通道的数量。噢嚯！ 
我向 Apple 提交了一个 bug，但是我想说的是：模型能在模拟器上运行正常并不意味着它在设备上运行正常。**一定要测试！**
### 有多快？
我没有办法测试 Core ML 的速度，因为我的全新 10.5 寸 iPad Pro 下个星期才能到（呵呵）。
我感兴趣的是我自己写的 [Forge 库](https://github.com/hollance/Forge)和 Core ML （考虑到我们都是一个早期的测试版）之间运行 MobileNets 之间的性能差异。
敬请关注！当我有数据可以分享时，我会更新这一节内容。
## Vision
下一个要讨论的事情就是全新的 **Vision** 框架。
你可能已经从它的名字中猜到了，Vision 可以让你执行**计算机视觉**任务。在以前你可能会使用 [OpenCV](http://opencv.org/)，但现在 iOS 有自己的 API 了。
![](https://user-gold-cdn.xitu.io/2017/6/28/eb305ec39c5457216e8ec6ddf2824f96)
Happy people with square faces
Vision 可以执行的任务有以下几种：
- 在图像中寻找人脸。然后对每个脸给出一个矩形框。
- 寻找面部的详细特征，比如眼睛和嘴巴的位置，头部的形状等等。
- 寻找矩形形状的图像，比如路标。
- 追踪视频中移动的对象。
- 确定地平线的角度。
- 转换两个图像，使其内容对齐。这对于拼接照片非常有用。
- 检测包含文本的图像中的区域。
- 检测和识别条形码。
Core Image 和 AVFoundation 已经可以实现其中的一些任务，但现在他们都集成在一个具有一致性 API 的框架内了。
如果你的应用程序需要执行这些计算机视觉任务之一，再也不用跑去自己实现或使用别人的库了 - 只需使用 Vision 框架。你还可以将其与 Core Image 框架相结合，以获得更多的图像处理能力。
更好的是：**你可以使用 Vision 驱动 Core ML**，这允许你使用这些计算机视觉技术作为神经网络的预处理步骤。例如，你可以使用 Vision 来检测人脸的位置和大小，将视频帧裁剪到该区域，然后在这部分的面部图像上运行神经网络。
事实上，任何时候当你结合图像或者视频使用 Core ML 时，使用 Vision 都是合理的。原始的 Core ML 需要你确保输入图像是模型所期望的格式。如果使用 Vision 框架来负责调整图像大小等，这会为你节省不少力气。
使用 Vision 来驱动 Core ML 的代码长这个样子：
```
// Core ML 的机器学习模型
let modelCoreML = ResNet50()
```
```
// 将 Core ML 链接到 Vision
let visionModel = try? VNCoreMLModel(for: modelCoreML.model)
```
```
let classificationRequest = VNCoreMLRequest(model: visionModel) {
  request, error iniflet observations = request.results as? [VNClassificationObservation] {
    /* 进行预测 */
  }
}
let handler = VNImageRequestHandler(cgImage: yourImage)
try? handler.perform([classificationRequest])
```
请注意，`VNImageRequestHandler` 接受一个请求对象数组，允许你将多个计算机视觉任务链接在一起，如下所示：
`try? handler.perform([faceDetectionRequest, classificationRequest])`
Vision 使计算机视觉变得非常容易使用。 但对我们机器学习人员很酷的事情是，你可以将这些计算机视觉任务的输出输入到你的 Core ML 模型中。 结合 Core Image 的力量，批量图像处理就跟玩儿一样！
## Metal Performance Shaders
我最后一个想要讨论的话题就是 **Metal** —— Apple 的 GPU 编程 API。
我今年为客户提供的很多工作涉及到使用 [Metal Performance Shaders (MPS)](http://machinethink.net/blog/convolutional-neural-networks-on-the-iphone-with-vggnet/) 来构建神经网络，并对其进行优化，从而获得最佳性能。但是 iOS 10 只提供了几个用于创建神经网络的基本 kernel。通常需要编写自定义的 kernel 来弥补这个缺陷。
所以我很开心使用 iOS 11，可用的 kernel 已经增长了许多，更好的是：我们现在有一个用于构建图的 API 了！
![](https://user-gold-cdn.xitu.io/2017/6/28/4045dee5353292178785c42883ddcb62)
Metal Performance Shaders
**注意:** 为什么要使用 MPS 而不是 Core ML？好问题！最大的原因是当 Core ML 不支持你想要做的事情时，或者当你想要完全的控制权并获得最大运行速度时。
MPS 中对于机器学习来说的最大的变化是：
**递归神经网络**。你现在可以创建 RNN，LSTM，GRU 和 MGU 层了。这些工作在 `MPSImage` 对象的序列上，但也适用于
`MPSMatrix` 对象的序列。这很有趣，因为所有其他 MPS layer 仅处理图像 —— 但显然，当你使用文本或其他非图像数据时，这不是很方便。
**更多数据类型**。以前的权重应该是 32 位浮点数，但现在可以是 16 位浮点数（半精度），8 位整数，甚至是 2 进制数。卷积和 fully-connected 的 layer 可以用 2 进制权重和 2 进制化输入来完成。
**更多的层**。到目前为止，我们不得不采用普通的常规卷积、最大池化和平均池化，但是在 iOS 11 MPS 中，你可以进行扩张卷积（Dilated Convolution）、子像素卷积（Subpixel Convolution）、转置卷积（Transposed Convolution）、上采样（Upsampling）和重采样（Resampling）、L2 范数池化（L2-norm pooling）、扩张最大池化（dilated max pooling），还有一些新的激活函数。
 MPS 还没有所有的 Keras 或 Caffe layer 类型，但差距正在缩小...
**更方便**。使用 `MPSImages` 总是有点奇怪，因为 Metal 每次以 4 个通道的片段组织数据（因为图像由
`MTLTexture` 对象支持）。但是现在，`MPSImage` 有用于读取和写入数据的方法，这些数据不会让你感到困惑。
`MPSCNNConvolutionDescriptor` 还有一个新方法，可以让你在 layer 上设置批量归一化参数。这意味着你不再需要将批量归一化到卷积层中，而 MPS 会为你处理这些事情。非常方便！
**性能改进**。现有的内核变得更快。这总是好消息。 �
**图 API**。这是我最关心的消息。手动创建所有 layer 和（临时）图像总是令人讨厌的。现在你可以描述一个图，就像你在Keras 中一样。 MPS 将自动计算出图像需要多大，如何处理填充，如何设置 MPS 内核的
`offset` 等等。甚至可以通过融合不同的 layer 来优化整个图。
看起来所有的 MPS 内核都可以使用 `NSSecureCoding` 进行序列化，这意味着你可以将图保存到文件中，然后将其还原。并且使用这个图来推断现在只是一个单一的方法调用。它不像 Core ML 那么简单，但使用 MPS 绝对比以前好用得多。
有一件事情我目前还不太清楚，那就是我不知道你是否可以编写自己的 kernel 并在这个图中使用。在我客户的工作中，我发现通常需要使用 Metel Shading 语言编写的自定义着色器来进行预处理步骤。据我所知，似乎没有一个「`MPSNNCustomKernelNode`」类。这还要再多研究一下！
结论：用于机器学习的 Metal Performance Shaders 已经在 iOS 11 中变得更加强大，但是大多数开发人员应该转而使用 Core ML（对于那些使用MPS的来说）。
**注意**：新的图 API 使我的 [Forge 库](http://github.com/hollance/Forge)基本上过时了，除非你希望在 App 中继续支持 iOS 10。我将尽快将示例应用移植到新的图 API 上，然后将写一个更详细的博客文章。
## 杂项
还有一些其他的更新：
**Accelerate 框架:** 似乎 [Accelerate 框架中的 BNNS](http://machinethink.net/blog/apple-deep-learning-bnns-versus-metal-cnn/) 并没有获得太多功能上的更新。它终于有了 Softmax 层，但 MPS 却没有新的 layer 类型。也许无关紧要：使用 CPU 进行深层神经网络可能不是一个好主意。也就是说，我喜欢 Accelerate，它有很多好玩的东西。而今年，它确实获得了对稀疏矩阵的更多支持，很棒。
**自然语言处理:** Core ML不仅仅只能处理图像，它还可以处理大量不同类型的数据，包括文本。 使用的 API 
```
NSLinguisticTagger
```
 类已经存在了一段时间，但是与 iOS 11 相比变得更加有效了。`NSLinguisticTagger` 现在已经能进行语言鉴别，词法分析，词性标注，词干提取和命名实体识别。
我没有什么 NLP 的经验，所以我没办法比较它与其他 NLP 框架的区别，但`NSLinguisticTagger` 看起来相当强大。 如果要将 NLP 添加到 App 中，此 API 似乎是一个好的起点。
## 都是好消息吗?
Apple 向我们开发者提供所有的这些新工具都非常的好，但是大多数 Apple API 都有一些很重要的问题：
- 闭源
- 有局限
- 只有在新 OS 发布时候才会更新
这三个东西加在一起意味着苹果的 API **总会落后**于其他工具。如果 Keras 增加了一个很炫酷的新的 layer 类型，那么在 Apple 更新其框架和操作系统之前，你都没办法将它和 Core ML 一起使用了。
如果某些 API 得到的计算结果并不是你想要的，你没办法简单的进去看看到底是 Core ML 的问题还是模型的问题，再去修复它 —— 你必须绕开 Core ML 来解决这个问题（并不总是可能的）；要么就只能等到下一个 OS 发布了（需要你所有的用户进行升级）。
当然我不希望 Apple 放弃他们的秘密武器，但是就像其他大多数机器学习工具开源一样，为什么不让 Core ML 也开源呢？ 
