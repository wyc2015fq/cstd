# 如何使用TensorFlow中的高级API：Estimator、Experiment和Dataset - 知乎
# 



近日，背景调查公司 Onfido 研究主管 Peter Roelants 在 Medium 上发表了一篇题为《Higher-Level APIs in TensorFlow》的文章，通过实例详细介绍了如何使用 TensorFlow 中的高级 API（Estimator、Experiment 和 Dataset）训练模型。值得一提的是 Experiment 和 Dataset 可以独立使用。这些高级 API 已被最新发布的 TensorFlow1.3 版收录。

TensorFlow 中有许多流行的库，如 Keras、TFLearn 和 Sonnet，它们可以让你轻松训练模型，而无需接触哪些低级别函数。目前，Keras API 正倾向于直接在 TensorFlow 中实现，TensorFlow 也在提供越来越多的高级构造，其中的一些已经被最新发布的 TensorFlow1.3 版收录。

在本文中，我们将通过一个例子来学习如何使用一些高级构造，其中包括 Estimator、Experiment 和 Dataset。阅读本文需要预先了解有关 TensorFlow 的基本知识。
![](https://pic2.zhimg.com/v2-37cb108ee73b4bfbb67e169faff0232d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='364'></svg>)
Experiment、Estimator 和 DataSet 框架和它们的相互作用（以下将对这些组件进行说明）

在本文中，我们使用 MNIST 作为数据集。它是一个易于使用的数据集，可以通过 TensorFlow 访问。你可以在这个 gist 中找到完整的示例代码。使用这些框架的一个好处是我们不需要直接处理图形和会话

Estimator

Estimator（评估器）类代表一个模型，以及这些模型被训练和评估的方式。我们可以这样构建一个评估器：
![](https://pic1.zhimg.com/v2-55541c804656a999f2bdbf4d90d43d88_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='890' height='169'></svg>)
为了构建一个 Estimator，我们需要传递一个模型函数，一个参数集合以及一些配置。
- 参数应该是模型超参数的集合，它可以是一个字典，但我们将在本示例中将其表示为 HParams 对象，用作 namedtuple。
- 该配置指定如何运行训练和评估，以及如何存出结果。这些配置通过 RunConfig 对象表示，该对象传达 Estimator 需要了解的关于运行模型的环境的所有内容。
- 模型函数是一个 Python 函数，它构建了给定输入的模型（见后文）。

**模型函数**

模型函数是一个 Python 函数，它作为第一级函数传递给 Estimator。稍后我们就会看到，TensorFlow 也会在其他地方使用第一级函数。模型表示为函数的好处在于模型可以通过实例化函数不断重新构建。该模型可以在训练过程中被不同的输入不断创建，例如：在训练期间运行验证测试。

模型函数将输入特征作为参数，相应标签作为张量。它还有一种模式来标记模型是否正在训练、评估或执行推理。模型函数的最后一个参数是超参数的集合，它们与传递给 Estimator 的内容相同。模型函数需要返回一个 EstimatorSpec 对象——它会定义完整的模型。

EstimatorSpec 接受预测，损失，训练和评估几种操作，因此它定义了用于训练，评估和推理的完整模型图。由于 EstimatorSpec 采用常规 TensorFlow Operations，因此我们可以使用像 TF-Slim 这样的框架来定义自己的模型。

**Experiment**

Experiment（实验）类是定义如何训练模型，并将其与 Estimator 进行集成的方式。我们可以这样创建一个实验类：
![](https://pic2.zhimg.com/v2-6af2b1c430ff83b4f7d6f6f156160f8d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='888' height='305'></svg>)
Experiment 作为输入：
- 一个 Estimator（例如上面定义的那个）。
- 训练和评估数据作为第一级函数。这里用到了和前述模型函数相同的概念，通过传递函数而非操作，如有需要，输入图可以被重建。我们会在后面继续讨论这个概念。
- 训练和评估钩子（hooks）。这些钩子可以用于监视或保存特定内容，或在图形和会话中进行一些操作。例如，我们将通过操作来帮助初始化数据加载器。
- 不同参数解释了训练时间和评估时间。
- 一旦我们定义了 experiment，我们就可以通过 learn_runner.run 运行它来训练和评估模型：
![](https://pic4.zhimg.com/v2-fb71510b88e60c74bb1e8aaa5765f97b_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='881' height='204'></svg>)
与模型函数和数据函数一样，函数中的学习运算符将创建 experiment 作为参数。

**Dataset**

我们将使用 Dataset 类和相应的 Iterator 来表示我们的训练和评估数据，并创建在训练期间迭代数据的数据馈送器。在本示例中，我们将使用 TensorFlow 中可用的 MNIST 数据，并在其周围构建一个 Dataset 包装器。例如，我们把训练的输入数据表示为：
![](https://pic4.zhimg.com/v2-37ca8c32b98f2fa78d932b0989c678d7_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='843' height='795'></svg>)


![](https://pic1.zhimg.com/v2-01320321b1a8740225f9a31e369889a4_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='849' height='407'></svg>)
调用这个 get_train_inputs 会返回一个一级函数，它在 TensorFlow 图中创建数据加载操作，以及一个 Hook 初始化迭代器。

本示例中，我们使用的 MNIST 数据最初表示为 Numpy 数组。我们创建一个占位符张量来获取数据，再使用占位符来避免数据被复制。接下来，我们在 from_tensor_slices 的帮助下创建一个切片数据集。我们将确保该数据集运行无限长时间（experiment 可以考虑 epoch 的数量），让数据得到清晰，并分成所需的尺寸。

为了迭代数据，我们需要在数据集的基础上创建迭代器。因为我们正在使用占位符，所以我们需要在 NumPy 数据的相关会话中初始化占位符。我们可以通过创建一个可初始化的迭代器来实现。创建图形时，我们将创建一个自定义的 IteratorInitializerHook 对象来初始化迭代器：
![](https://pic4.zhimg.com/v2-18d704e9e043bf3d033fd89d4a473cb3_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='862' height='271'></svg>)
IteratorInitializerHook 继承自 SessionRunHook。一旦创建了相关会话，这个钩子就会调用 call after_create_session，并用正确的数据初始化占位符。这个钩子会通过 get_train_inputs 函数返回，并在创建时传递给 Experiment 对象。

train_inputs 函数返回的数据加载操作是 TensorFlow 操作，每次评估时都会返回一个新的批处理。

**运行代码**

现在我们已经定义了所有的东西，我们可以用以下命令运行代码：
![](https://pic1.zhimg.com/v2-417f12ccfa65ad4167dfbfeaeed715ec_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='883' height='55'></svg>)
如果你不传递参数，它将使用文件顶部的默认标志来确定保存数据和模型的位置。训练将在终端输出全局步长、损失、精度等信息。除此之外，实验和估算器框架将记录 TensorBoard 可以显示的某些统计信息。如果我们运行：
![](https://pic3.zhimg.com/v2-14ca49b4d57f0a70b847a0b56bd3d136_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='852' height='62'></svg>)
我们就可以看到所有训练统计数据，如训练损失、评估准确性、每步时间和模型图。
![](https://pic3.zhimg.com/v2-720f0da615d693d003dda7a886975d52_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='324' height='213'></svg>)
评估精度在 TensorBoard 中的可视化

在 TensorFlow 中，有关 Estimator、Experiment 和 Dataset 框架的示例很少，这也是本文存在的原因。希望这篇文章可以向大家介绍这些架构工作的原理，它们应该采用哪些抽象方法，以及如何使用它们。如果你对它们很感兴趣，以下是其他相关文档。

关于 Estimator、Experiment 和 Dataset 的注释
- 论文《TensorFlow Estimators: Managing Simplicity vs. Flexibility in High-Level Machine Learning Frameworks》：[https://terrytangyuan.github.io/data/papers/tf-estimators-kdd-paper.pdf](https://link.zhihu.com/?target=https%3A//terrytangyuan.github.io/data/papers/tf-estimators-kdd-paper.pdf)
- Using the Dataset API for TensorFlow Input Pipelines：[https://www.tensorflow.org/versions/r1.3/programmers_guide/datasets](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/versions/r1.3/programmers_guide/datasets)
- tf.estimator.Estimator：[https://www.tensorflow.org/api_docs/python/tf/estimator/Estimator](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/api_docs/python/tf/estimator/Estimator)
- tf.contrib.learn.RunConfig：[https://www.tensorflow.org/api_docs/python/tf/contrib/learn/RunConfig](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/api_docs/python/tf/contrib/learn/RunConfig)
- tf.estimator.DNNClassifier：[https://www.tensorflow.org/api_docs/python/tf/estimator/DNNClassifier](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/api_docs/python/tf/estimator/DNNClassifier)
- tf.estimator.DNNRegressor：[https://www.tensorflow.org/api_docs/python/tf/estimator/DNNRegressor](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/api_docs/python/tf/estimator/DNNRegressor)
- Creating Estimators in tf.estimator：[https://www.tensorflow.org/extend/estimators](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/extend/estimators)
- tf.contrib.learn.Head：[https://www.tensorflow.org/api_docs/python/tf/contrib/learn/Head](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/api_docs/python/tf/contrib/learn/Head)
- 本文用到的 Slim 框架：[https://github.com/tensorflow/models/tree/master/slim](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/models/tree/master/slim)

完整示例
![](https://pic4.zhimg.com/v2-5b6ee4b8b26abf16f6aff269f7cefe27_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='840' height='747'></svg>)


![](https://pic4.zhimg.com/v2-953275c1c9b02bf57fbb48e57f81b33f_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='831' height='755'></svg>)


![](https://pic3.zhimg.com/v2-5b6f7f6a8a8c8df8c29dd02298500bba_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='840' height='693'></svg>)


![](https://pic4.zhimg.com/v2-46ddac47b685d9b86e7234e2dc29475f_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='847' height='741'></svg>)


![](https://pic4.zhimg.com/v2-6bb7cd04c1f3664746f6f5c84891e5ff_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='842' height='805'></svg>)


![](https://pic3.zhimg.com/v2-19be70c864df2893ab827e4f0129fbfa_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='844' height='685'></svg>)


![](https://pic4.zhimg.com/v2-298fd441137e40061170df7cf208492b_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='836' height='818'></svg>)


![](https://pic4.zhimg.com/v2-eab902df4961815c28778de99fd043df_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='838' height='770'></svg>)


![](https://pic4.zhimg.com/v2-f60d11589652a23d4053ab99462ca56f_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='841' height='886'></svg>)


![](https://pic4.zhimg.com/v2-90d1b3f9fbb07d1bcddc54ca045f897f_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='841' height='247'></svg>)
**推理训练模式**

在训练模型后，我们可以运行 estimateator.predict 来预测给定图像的类别。可使用以下代码示例。 



![](https://pic4.zhimg.com/v2-8e81cc08e0eb485736e797363eeacc13_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='840' height='576'></svg>)


![](https://pic4.zhimg.com/v2-7e70b8a4c8604ee1ea0e8399ea33dc17_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='844' height='767'></svg>)


![](https://pic1.zhimg.com/v2-58bf0f7a8ae0c0d4c9e8a9f2b96f3da4_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='868' height='350'></svg>)
*原文链接：[https://medium.com/onfido-tech/higher-level-apis-in-tensorflow-67bfb602e6c0](https://link.zhihu.com/?target=https%3A//medium.com/onfido-tech/higher-level-apis-in-tensorflow-67bfb602e6c0)*




选自Medium

作者：Peter Roelants

机器之心编译

参与：李泽南、黄小天




***本文为机器之心编译，转载请联系本公众号获得授权。***
















