# Core ML 文档翻译 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月07日 10:20:01[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4490
> 
本文来自简书，原文地址:[http://www.jianshu.com/p/6c2dff59eaaf](http://www.jianshu.com/p/6c2dff59eaaf)
将机器学习模型集成到您的应用当中。
## 概览
借助 Core ML，您可以将已训练好的机器学习模型，集成到自己的应用当中。
![](http://upload-images.jianshu.io/upload_images/74454-4726f1eccb39b18c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
所谓已训练模型 (trained model)，指的是对一组训练数据应用了某个机器学习算法后，所生成的一组结果。举个例子，通过某个地区的历史房价来训练出一个模型，那么只要指定房间有几卧几卫，就有可能对未来该房间的房价做出预测。
Core ML 是领域特定 (domain-specific) 框架和功能的基础所在。Core ML 为 [Vision](https://developer.apple.com/documentation/vision) 提供了图像处理的支持，为 [Foundation](https://developer.apple.com/documentation/foundation) 提供了自然语言处理的支持（例如 [NSLinguisticTagger](https://developer.apple.com/documentation/foundation/nslinguistictagger) 类），为 [GameplayKit](https://developer.apple.com/documentation/gameplaykit) 提供了对学习决策树
 (learned decision tree) 进行分析的支持。Core ML 本身是基于底层基本类型而建立的，包括 [Accelerate](https://developer.apple.com/documentation/accelerate)、[BNNS](https://developer.apple.com/documentation/accelerate/bnns) 以及 [Metal
 Performance Shaders](https://developer.apple.com/documentation/metalperformanceshaders) 等。
![](http://upload-images.jianshu.io/upload_images/74454-e6480ca21450fd22.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Core ML 针对设备的性能进行了优化，最大限度地减少内存占用和功耗。通过在设备上运行的严格要求，不仅保护了用户数据隐私，而且当网络连接丢失的时候，还能够保证应用能正常工作和响应。
## 获取 Core ML 模型
> 
获取 Core ML 模型，以便能在您的应用当中使用。
Core ML 支持多种机器学习模型，其中包括了神经网络 (Neural Network)、组合树 (Tree Ensemble)、支持向量机 (Support Vector Machine) 以及广义线性模型 (Generalized Linear Model)。Core ML 的运行需要使用 Core ML 模型格式（也就是以 `.mlmodel` 扩展名结尾的模型）。
Apple 提供了一些常见的开源[模型](https://developer.apple.com/machine-learning)供大家使用，这些模型已经使用了 Core ML 模型格式。您可以自行下载这些模型，然后就可以开始在应用中使用它们了。此外，其他的研究机构和大学都发布了不少机器学习模型和训练数据，这些往往都不是以
 Core ML 模型格式发布出来的。如果您打算使用这些模型的话，需要对它们进行转换，具体内容详见「将已训练模型转换为 Core ML」。
## 将 Core ML 模型集成到应用中
> 
向应用中添加一个简单的模型，然后向模型中传入输入数据，并对模型的预测值进行处理。
[点击此处](https://docs-assets.developer.apple.com/published/51ff0c1668/IntegratingaCoreMLModelintoYourApp.zip)下载示例应用。
### 概述
本示例应用使用了一个已训练模型 `MarsHabitatPricer.mlmodel`，用以预测火星上的殖民地价值。
### 将模型添加到 Xcode 项目中
要想将模型添加到 Xcode 项目当中，只需要将模型拖曳进项目导航器 (project navigator) 当中即可。
您可以通过在 Xcode 中打开这个模型，从而来查看它的相关信息，其中包括有模型类型以及其预期输入和输出。模型的输入为太阳能板和温室的数量，以及殖民地的规模大小（以英亩为单位）。模型的输出则是对这个殖民地价值的预测。
![](http://upload-images.jianshu.io/upload_images/74454-34d662eefe02d509.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### 用代码创建模型
Xcode 同样也会使用模型的输入输出信息，来自动为模型生成一个自定义的编程接口，从而就可以在代码当中与模型进行交互。对于这个 `MarsHabitatPricer.mlmodel` 而言，Xcode
 会生成相应的接口，来分别表示模型本身(`MarsHabitatPricer`)、模型输入 (`MarsHabitatPricerInput`)
 以及模型输出 (`MarsHabitatPricerOutput`)。
使用所生成的 `MarsHabitatPricer` 类的构造器，便可以创建这个模型：
`let model = MarsHabitatPricer()`
### 获取输入值以传递给模型
示例应用使用了 `UIPickerView`，以便从用户那里获取模型的输入值。
```
func selectedRow(for feature: Feature) -> Int {
    return pickerView.selectedRow(inComponent: feature.rawValue)
}
let solarPanels = pickerDataSource.value(for: selectedRow(for: .solarPanels), feature: .solarPanels)
let greenhouses = pickerDataSource.value(for: selectedRow(for: .greenhouses), feature: .greenhouses)
let size = pickerDataSource.value(for: selectedRow(for: .size), feature: .size)
```
### 使用模型来进行预测
`MarsHabitatPricer` 类会生成一个名为 `prediction(solarPanels:greenhouses:size:)` 方法，从而就可以根据模型的输入值来预测价值，在本例当中，输入值为太阳能板的数量、温室的数量以及殖民地的规模大小（以英亩为单位）。这个方法的结果为一个 `MarsHabitatPricerOutput` 实例，这里我们将其取名为 `marsHabitatPricerOutput`。
```
guard let marsHabitatPricerOutput = try? model.prediction(solarPanels: solarPanels, greenhouses: greenhouses, size: size) else {
    fatalError("Unexpected runtime error.")
}
```
通过读取 `marsHabitatPricerOutput` 的 `price` 属性，就可以获取所预测的价值，然后就可以在应用的
 UI 当中对这个结果进行展示。
```
let price = marsHabitatPricerOutput.price
priceLabel.text = priceFormatter.string(for: price)
```
> 
注意
所生成的 `prediction(solarPanels:greenhouses:size:)` 方法会抛出异常。在使用
 Core ML 的时候，您大多数时候遇到的错误通常是：传递给方法的输入数据类型与模型预期的输入类型不同——例如，用错误格式表示的图片类型。在示例应用当中，预期的输入类型为 `Double`。所有的类型不匹配错误都会在编译时被捕获，如果遇到了某种错误，那么示例应用就会弹出一个致命错误。
### 构建并运行 Core ML 应用
Xcode 会将 Core ML 模型编译到资源当中，从而进行优化并能够在设备上运行。优化过的模型表征会包含在您的应用程序包当中，在应用在设备上运行的时候，就可以用之来进行预测。
## 将已训练模型转换为 Core ML
> 
将由第三方机器学习工具所创建的已训练模型，转换为 Core ML 模型格式。
### 概述
如果您已经使用了第三方机器学习工具来创建和训练模型，只要这个工具是受支持的，那么就可以使用 [Core ML Tools](https://developer.apple.com/machine-learning) 来将这些模型转换为 Core ML 模型格式。表 1 列出了我们支持的模型和第三方工具。
> 
注意
Core ML Tools 是一个 Python 包 (`coremltools`)，并挂载在 Python
 Package Index (PyPI) 上。要了解关于 Python 包的更多信息，请参阅 [Python Packaging User Guide](https://packaging.python.org/)。
|模型类型|支持的模型|支持的工具|
|----|----|----|
|神经网络 (Neural network)|前馈 (Feedforward)、卷积 (Convolutional)、循环 (Recurrent)|Caffe / Keras 1.2.2|
|组合树 (Neural networks)|随机森林 (Random Forests)、提升树 (Boosted Trees)、决策树 (Decision Trees)|scikit-learn 0.18 / XGBoost 0.6|
|支持向量机 (Support vector machines)|标量回归 (Scalar Regression)、多级分类 (Multiclass classification)|scikit-learn 0.18 / LIBSVM 3.22|
|广义线性模型 (Support vector machines)|线性回归 (Linear Regression)、逻辑回归 (Logistic Regression)|scikit-learn 0.18|
|特征工程 (Feature engineering)|稀疏向量矢量化 (Sparse Vectorization)、稠密向量矢量化 (Dense Vectorization)、分类处理 (Categorical Processing)|scikit-learn 0.18|
|管道模型 (Pipeline models)|顺序链模型 (Sequentially Chained Models)|scikit-learn 0.18|
### 模型转换
您可以使用 Core ML 转换器，并根据对应的模型第三方工具，来对模型进行转换。通过调用转换器的 `convert` 方法，然后再将结果保存为
 Core ML 模型格式 (`.mlmodel`)。
例如，如果您的模型是使用 Caffe 来创建的，您可以将 Caffe 模型 (`.caffemodel`)
 传递给 `coremltools.converters.caffe.convert` 方法。
```
import coremltools
coreml_model = coremltools.converters.caffe.convert('my_caffe_model.caffemodel')
```
然后将结果保存为 Core ML 模型格式。
`coreml_model.save('my_model.mlmodel')`
根据您模型的不同，您可能会需要更新输入、输出以及相关的参数标签，或者您还可能会需要声明图片名称、类型以及格式。转换工具内置了更详细的文档，因为可用的选项因工具而异。
### 或者，还可以编写自定义的转换工具
如果您需要转换的格式不在表 1 当中，那么您可以创建自己的转换工具。
编写自定义的转换工具，包括了将模型的输入、输出和架构表示转换为 Core ML 模型格式。您可以通过将每一层模型架构，以及层之间的连接关系进行定义，来实现这个操作。您可以通过 [Core ML Tools](https://developer.apple.com/machine-learning) 所提供的转换工具作为参考；它们演示了如何将各种第三方工具创建的模型类型，转换为
 Core ML 模型格式。
> 
注意
Core ML 模型格式由一系列 Protocol Buffer 文件所定义，具体信息请参见 [Core ML Model Specification](https://developer.apple.com/machine-learning)。
## Core ML API
> 
直接使用 Core ML API，从而支持自定义工作流以及更为高级的用例。
在绝大多数情况下，您只需要与模型动态生成的接口进行交互即可，也就是说当您将模型添加到 Xcode 项目当中的时候，这个接口就由 Xcode 自动创建完毕了。您可以直接使用 Core ML API，以便支持自定义工作流或者更为高级的用例。举个例子，如果您需要将输入数据异步收集到自定义结构体，从而来执行预测的话，那么您就可以让这个结构体实现 [MLFeatureProvider](https://developer.apple.com/documentation/coreml/mlfeatureprovider) 协议，从而来为模型提供输入功能。
具体的 API 列表请参见 [Core ML API](https://developer.apple.com/documentation/coreml/core_ml_api)。
