# Core ML 与 Vision：iOS 11 机器学习教程 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月22日 13:26:20[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：910
此文翻译自 [Core
 ML and Vision: Machine Learning in iOS 11 Tutorial](https://www.raywenderlich.com/164213/coreml-and-vision-machine-learning-in-ios-11-tutorial)
本文来自简书，原文地址:[http://www.jianshu.com/p/1df8ac6af865](http://www.jianshu.com/p/1df8ac6af865)
> 
注意：此教程需要 Xcode 9 Beta1 或更新的版本、Swift 4 以及 iOS 11.
机器学习正在肆虐横行。很多人都听说过，但很少有人知道这是什么。
这篇《iOS 机器学习教程》会为你介绍 Core ML 和 Vision，iOS 11 中推出的两个全新框架。
具体来说，你会学习如何借助 Places205-GoogLeNet 模型，使用新的 API 对图像的场景进行分类。
![](http://upload-images.jianshu.io/upload_images/861914-9c67ed9603358262.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 开始
下载[起始项目](https://koenig-media.raywenderlich.com/uploads/2017/06/SceneDetector_Starter-1.zip)。它已包含了用于显示图片的用户界面，并允许用户从照片库中选择另一张图片。这样你就可以专注于实现
 App 的机器学习和视觉方面。
构建并运行该项目；可以看到一张城市夜景图，以及一个按钮：
![](http://upload-images.jianshu.io/upload_images/861914-92577c93fa303416.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
从“照片” App 的照片库中选择另一张图片。此起始项目的 Info.plist 已经有 Privacy – Photo Library Usage Description，所以你会被提示允许使用。
图片和按钮之间的空隙有一个 label，将会在此显示模型对图片场景的分类。
## iOS 机器学习
机器学习是一种人工智能，计算机会“学习”而不是被明确编程。不用编写算法，机器学习工具通过在大量数据中寻找模式，使计算机能够开发和优化算法。
### 深度学习
自20世纪50年代以来，AI 研究人员开发了许多机器学习方法。苹果的 Core ML 框架支持神经网络、树组合、支持向量机、广义线性模型、特征工程和流水线模型。但是，神经网络最近已经取得了很多极为神奇的成功，开始于 2012 年谷歌使用 YouTube 视频训练 AI 来识别猫和人。仅仅五年后，谷歌正在赞助一场确定 5000 种植物和动物的比赛。像 Siri 和 Alexa
 这样的 App 也存在它们自己的神经网络。
神经网络尝试用节点层来模拟人脑流程，并将节点层用不同的方式连接在一起。每增加一层都需要增加大量计算能力：Inception v3，一个对象识别模型，有48层以及大约2000万个参数。但计算基本上都是矩阵乘法，GPU 来处理会非常有效。GPU 成本的下降使我们能够创建多层深度神经网络，此为深度学习。
![](http://upload-images.jianshu.io/upload_images/861914-4f20b3dc2c785729.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
神经网络，circa 2016
神经网络需要大量的训练数据，这些训练数据理想化地代表了全部可能性。用户生成的数据爆炸性地产生也促成了机器学习的复兴。
训练模型意味着给神经网络提供训练数据，并让它计算公式，此公式组合输入参数以产生输出。训练是离线的，通常在具有多个 GPU 的机器上。
要使用这个模型，就给它新的输入，它就会计算输出：这叫做推论。推论仍然需要大量计算，以从新的输入计算输出。因为有了 Metal 这样的框架，现在可以在手持设备上进行这些计算。
在本教程的结尾你会发现，深度学习远非完美。真的很难建立具有代表性的训练数据，很容易就会过度训练模型，以至于它会过度重视一些古怪的特征。
### 苹果提供了什么？
苹果在 iOS 5 里引入了 `NSLinguisticTagger` 来分析自然语言。iOS
 8 出了 Metal，提供了对设备 GPU 的底层访问。
去年，苹果在 Accelerate 框架添加了 Basic Neural Network Subroutines (BNNS)，使开发者可以构建用于推理（不是训练）的神经网络。
今年，苹果给了我们 Core ML 和 Vision！
- Core ML 让我们更容易在 App 中使用训练过的模型。
- Vision 让我们轻松访问苹果的模型，用于面部检测、面部特征点、文字、矩形、条形码和物体。
你还可以在 Vision 模型中包装任意的图像分析 Core ML 模型，我们在这篇教程中就干这个。由于这两个框架是基于 Metal 构建的，它们能在设备上高效运行，所以不需要把用户的数据发送到服务器。
## 将 Core ML 模型集成到你的 App
本教程使用 Places205-GoogLeNet 模型，可以从苹果的[“机器学习”页面](https://developer.apple.com/machine-learning/)下载。往下滑找到 Working
 with Models，下载第一个。还在这个页面，注意一下其它三个模型，它们都用于在图片中检测物体——树、动物、人等等。
> 
注意：如果你有一个训练过的模型，并且是使用受支持的机器学习工具训练的，例如 Caffe、Keras 或 scikit-learn，[Converting
 Trained Models to Core ML](https://developer.apple.com/documentation/coreml/converting_trained_models_to_core_ml) 介绍了如何将其转换为 Core ML 格式。
## 为你的项目添加模型
下载 GoogLeNetPlaces.mlmodel 后，把它从 Finder 拖到项目导航器的 Resources 组里：
![](http://upload-images.jianshu.io/upload_images/861914-5425960c41207b82.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
选择该文件，然后等一会儿。Xcode 生成了模型类后会显示一个箭头：
![](http://upload-images.jianshu.io/upload_images/861914-e00a802f64f0ae13.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
点击箭头，查看生成的类：
![](http://upload-images.jianshu.io/upload_images/861914-1268b5b918fccf07.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Xcode 已生成了输入和输出类以及主类 `GoogLeNetPlaces`，主类有一个 `model` 属性和两个 `prediction` 方法。
`GoogLeNetPlacesInput` 有一个 `CVPixelBuffer` 类型的 `sceneImage` 属性。哭了，这些都是什么鬼？！不要害怕，Vision
 框架会负责把我们熟悉的图片格式转换成正确的输入类型。:]
Vision 框架还会把 `GoogLeNetPlacesOutput` 属性转换为自己的 `results` 类型，并管理对 `prediction` 方法的调用，所以在所有生成的代码中，我们只会使用 `model` 属性。
### 在 Vision Model 中包装 Core ML Model
终于，要开始写代码了！打开 ViewController.swift，并在 
```
import
 UIKit
```
 下面 import 两个框架：
```
import CoreML
import Vision
```
下一步，在 `IBActions` 扩展下方添加如下扩展：
```
// MARK: - Methods
extension ViewController {
  func detectScene(image: CIImage) {
    answerLabel.text = "detecting scene..."
    // 从生成的类中加载 ML 模型
    guard let model = try? VNCoreMLModel(for: GoogLeNetPlaces().model) else {
      fatalError("can't load Places ML model")
    }
  }
}
```
我们上面的代码做了这些事：
首先，给用户显示一条消息，让他们知道正在发生什么事情。
GoogLeNetPlaces 的指定初始化方法会抛出一个 error，所以创建时必须用 `try`。
`VNCoreMLModel` 只是用于
 Vision 请求的 Core ML 模型的容器。
标准的 Vision 工作流程是创建模型，创建一或多个请求，然后创建并运行请求处理程序。我们刚刚已经创建了模型，所以下一步是创建请求。
在 `detectScene(image:)` 的末尾添加如下几行：
```
// 创建一个带有 completion handler 的 Vision 请求
let request = VNCoreMLRequest(model: model) { [weak self] request, error in
  guard let results = request.results as? [VNClassificationObservation],
    let topResult = results.first else {
      fatalError("unexpected result type from VNCoreMLRequest")
  }
  // 在主线程上更新 UI
  let article = (self?.vowels.contains(topResult.identifier.first!))! ? "an" : "a"
  DispatchQueue.main.async { [weak self] in
    self?.answerLabel.text = "\(Int(topResult.confidence * 100))% it's \(article) \(topResult.identifier)"
  }
}
```
`VNCoreMLRequest` 是一个图像分析请求，它使用
 Core ML 模型来完成工作。它的 completion handler 接收 `request` 和 `error` 对象。
检查 `request.results` 是否是 `VNClassificationObservation` 对象数组，当
 Core ML 模型是分类器，而不是预测器或图像处理器时，Vision 框架就会返回这个。而 `GoogLeNetPlaces` 是一个分类器，因为它仅预测一个特征：图像的场景分类。
`VNClassificationObservation` 有两个属性：`identifier` -
 一个 `String`，以及 `confidence`-
 介于0和1之间的数字，这个数字是是分类正确的概率。使用对象检测模型时，你可能只会看到那些 confidence 大于某个阈值的对象，例如 30％ 的阈值。
然后取第一个结果，它会具有最高的 confidence 值，然后根据 identifier 的首字母把不定冠词设置为“a”或“an”。最后，dispatch 回到主线程来更新 label。你很快会明白分类工作为什么不在主线程，因为它会很慢。
现在，做第三步：创建并运行请求处理程序。
把下面几行添加到 `detectScene(image:)` 的末尾：
```
// 在主线程上运行 Core ML GoogLeNetPlaces 分类器
let handler = VNImageRequestHandler(ciImage: image)
DispatchQueue.global(qos: .userInteractive).async {
  do {
    try handler.perform([request])
  } catch {
    print(error)
  }
}
```
VNImageRequestHandler 是标准的 Vision 框架请求处理程序；不特定于 Core ML 模型。给它 image 作为 `detectScene(image:)` 的参数。然后调用它的 `perform` 方法来运行处理程序，传入请求数组。在这个例子里，我们只有一个请求。
`perform` 方法会抛出
 error，所以用 try-catch 将其包住。
### 使用模型来分类场景
哇，刚刚写了好多代码！但现在只需要在两个地方调用 `detectScene(image:)` 就好了。
把下面几行添加到 `viewDidLoad()` 的末端和 `imagePickerController(_:didFinishPickingMediaWithInfo:)` 的末端：
```
guard let ciImage = CIImage(image: image) else {
  fatalError("couldn't convert UIImage to CIImage")
}
detectScene(image: ciImage)
```
现在构建并运行。不需要多久就可以看见分类：
![](http://upload-images.jianshu.io/upload_images/861914-bb5bec38118f91ff.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
哈哈，是的，图片里有 skyscrapers（摩天大楼）。还有一列火车。
点击按钮，选择照片库里的第一张图片：一些树叶上太阳光斑的特写：
![](http://upload-images.jianshu.io/upload_images/861914-e194482f995324a1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
75%这是一个水族池
啊哈哈哈哈哈，眯起眼睛，也许可以想象尼莫或多莉正在里面游泳？但至少你知道应该用 “a” 还是 “an”。;]
## 看一眼苹果的 Core ML 示例 App
本教程的项目和 WWDC 2017 Session 506 Vision Framework: Building on Core ML 的[示例项目](https://developer.apple.com/sample-code/wwdc/2017/ImageClassificationwithVisionandCoreML.zip)很相似。Vision
 + ML Example App 使用 MNIST 分类器，可以识别手写数字——对邮政分类自动化非常有帮助。它还使用原生 Vision 框架方法 `VNDetectRectanglesRequest`，还包括
 Core Image 的代码来矫正矩形检测的透视。
还可以从 [Core
 ML 文档页面](https://developer.apple.com/documentation/coreml/integrating_a_core_ml_model_into_your_app)下载另一个示例项目。MarsHabitatPricePredictor 模型的输入只是数字，因此代码直接使用生成的 `MarsHabitatPricer` 方法和属性，而不是将模型包装在
 Vision 模型中。每次都改一下参数，很容易看出模型只是一个线性回归：
`137 * solarPanels + 653.50 * greenHouses + 5854 * acres`
## 下一步？
可以从[这里](https://koenig-media.raywenderlich.com/uploads/2017/06/SceneDetector_Final-1.zip)下载教程的完整项目。如果模型显示为缺失，将其替换为你下载的那个。
你现在已经有能力将现有的模型整合到你的 App 中。这里有一些资源可以更详细地介绍：
- 苹果的 [Core ML Framework](https://developer.apple.com/documentation/coreml) 文档
- [WWDC 2017 Session 703](https://developer.apple.com/videos/play/wwdc2017/703/) 介绍 Core ML
- [WWDC 2017 Session 710](https://developer.apple.com/videos/play/wwdc2017/710/) Core ML in depth
2016 年的：
- [WWDC 2016 Session 605](https://developer.apple.com/videos/play/wwdc2016/605) What’s New in Metal, Part 2:
 demos show how fast the app does the Inception model classification calculations, thanks to Metal.
- 苹果的 [Basic Neural Network Subroutines](https://developer.apple.com/documentation/accelerate/bnns) 文档
想构建自己的模型？恐怕这超出了本教程的范围（以及我的专业知识）。但这些资源可能会帮你上手：
- RWDevCon 2017 Session 3 [Machine
 Learning in iOS](https://videos.raywenderlich.com/courses/81-rwdevcon-2017-vault-tutorials/lessons/3?_ga=2.4195450.836034365.1496798586-179527185.1486188143): Alexis Gallagher 做了一项绝对精彩的工作，指导你一系列流程，为神经网络收集训练数据（你微笑或皱眉的视频），训练，然后检查它是否有效。他的结论：“不需要是数学家或大公司也可以建立有效的模型。”
- [Quartz
 article on Apple’s AI research paper](https://qz.com/873043/apples-first-research-paper-tries-to-solve-a-problem-facing-every-company-working-on-ai/): Dave Gershgorn’s 有关 AI 的文章都很清晰和翔实。此文做了一项杰出的工作，总结了[苹果的第一篇 AI 研究论文](https://arxiv.org/pdf/1612.07828)：研究人员使用基于`真实`图像训练的神经网络来优化图像`合成`，从而有效地产生了大量高质量的新训练数据，而没有个人数据隐私问题。
最后，我从 Andreessen Horowitz 的 Frank Chen 那里真的学习了很多 AI 的简史：[AI
 and Deep Learning a16z podcast](http://a16z.com/2016/06/10/ai-deep-learning-machines/)。
希望本教程对你有所帮助。随意在下方加入讨论！
