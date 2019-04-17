# 开源一年多的模型交换格式ONNX，已经一统框架江湖了？ - 知乎
# 



机器之心原创，作者：思源。

> 近日，微软亚洲研究院和华为举办了 ONNX 合作伙伴研讨会，这是 ONNX 开源社区成立以来首次在中国举办的活动。在研讨会中，微软、Facebook、华为和英特尔等的开发者介绍了他们在 ONNX 上的开源贡献及思考。

在过去的一年多中，ONNX 这种「通用」的神经网络交换格式已经有了很长远的发展，用不同框架编写的模型可以在不同的平台中流通。在这次研讨会中，我们确切地感受到了这一点，因为开源社区围绕着 ONNX 介绍了很多优化工具和资源库。

微软上个月开源了 ONNX Runtime，其专为 ONNX 格式的模型设计了高性能推理引擎。Facebook 早两个月前开源了 ONNXIFI，其为 ONNX 提供了用于框架集成的接口，即一组用于加载和执行 ONNX 计算图的跨平台 API。更早一些，英特尔在今年 3 月份就开源 nGraph，它能编译 ONNX 格式的模型，并在 CPU 或 GPU 等硬件加速模型的运行。

而到了昨天，微软又开源了 ONNX.JS，它是一种在浏览器和 Node.js 上运行 ONNX 模型的 JavaScript 库。它部署的模型效率非常高，且能实现交互式的直观推理。该开源项目给出了图像分类的交互式演示，且在 Chrome 浏览器和 CPU 下比 TensorFlow.JS 快了近 8 倍，后文将详细介绍这一开源库。

当然除了这些开源工作，ONNX 社区还有更多的实践，例如如何部署 ONNX 模型到边缘设备、如何维护一个包罗万象的 ONNX Model Zoo 等。本文主要从什么是 ONNX、怎样用 ONNX，以及如何优化 ONNX 三方面看看 ONNX 是不是已经引领「框架间的江湖」了。

**什么是 ONNX**

很多开发者在玩 GitHub 的时候都有这样「悲痛」的经历，好不容易找到令人眼前一亮的项目，然而发现它使用我们不熟悉的框架写成。其实我们会发现很多优秀的视觉模型是用 Caffe 写的，很多新的研究论文是用 PyTorch 写的，而更多的模型用 TensorFlow 写成。因此如果我们要测试它们就必须拥有对应的框架环境，但 ONNX 交换格式令我们在同一环境下测试不同模型有了依靠。

简而言之 ONNX 就是一种框架间的转换格式，例如我们用 TensorFlow 写的模型可以转换为 ONNX 格式，并在 Caffe2 环境下运行该模型。
- 项目地址：[https://github.com/onnx/onnx](https://link.zhihu.com/?target=https%3A//github.com/onnx/onnx)

ONNX 定义了一种可扩展的计算图模型、一系列内置的运算单元（OP）和标准数据类型。每一个计算流图都定义为由节点组成的列表，并构建有向无环图。其中每一个节点都有一个或多个输入与输出，每一个节点称之为一个 OP。这相当于一种通用的计算图，不同深度学习框架构建的计算图都能转化为它。

如下所示，目前 ONNX 已经支持大多数框架，使用这些框架构建的模型可以转换为通用的 ONNX 计算图和 OP。现阶段 ONNX 只支持推理，所以导入的模型都需要在原框架完成训练。
![](https://pic4.zhimg.com/v2-415f5c354155c08997349176b566bc17_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='509'></svg>)
其中 Frameworks 下的框架表示它们已经内嵌了 ONNX，开发者可以直接通过这些框架的内置 API 将模型导出为 ONNX 格式，或采用它们作为推理后端。而 Converters 下的框架并不直接支持 ONNX 格式，但是可以通过转换工具导入或导出这些框架的模型。

其实并不是所有框架都支持导入和导出 ONNX 格式的模型，有一些并不支持导入 ONNX 格式的模型，例如 PyTorch 和 Chainer 等，TensorFlow 的 ONNX 导入同样也正处于实验阶段。下图展示了各框架对 ONNX 格式的支持情况：
![](https://pic2.zhimg.com/v2-5e6c4a6beff36f8dcef38f48218262dd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='669'></svg>)
**怎样使用 ONNX**

对于内建了 ONNX 的框架而言，使用非常简单，只需要调用 API 导出或导入已训练模型就可以了。例如对 PyTorch 而言，只需要几个简单的步骤就能完成模型的导出和导入。简单而言，首先加载 torch.onnx 模块，然后导出预训练模型并查看模型结构信息，最后再将导出的 ONNX 模型加载到另外的框架就能执行推理了。


```
from torch.autograd import Variable
import torch.onnx
import torchvision

dummy_input = Variable(torch.randn(10, 3, 224, 224)).cuda()
model = torchvision.models.alexnet(pretrained=True).cuda()

input_names = [ "actual_input_1" ] + [ "learned_%d" % i for i in range(16) ]
output_names = [ "output1" ]

torch.onnx.export(model, dummy_input, "alexnet.onnx", verbose=True, input_names=input_names, output_names=output_names)
```


如上所示将导出 ONNX 格式的 AlexNet 模型，其中"alexnet.onnx"为保存的模型，input_names、output_names 和 verbose=True 都是为了打印出模型结构信息。同样随机产生的「图像」dummy_input 也是为了了解模型结构，因为我们可以通过它理解输入与每一层具体的参数维度。以下展示了 ONNX 输出的简要模型信息：


```
graph(%actual_input_1 : Float(10, 3, 224, 224)
      %learned_0 : Float(64, 3, 11, 11)
      %learned_1 : Float(64)
      # ---- omitted for brevity ----
      %learned_14 : Float(1000, 4096)
      %learned_15 : Float(1000)) {
  %17 : Float(10, 64, 55, 55) = onnx::Conv[dilations=[1, 1], group=1, kernel_shape=[11, 11], pads=[2, 2, 2, 2], strides=[4, 4]](%actual_input_1, %learned_0, %learned_1), scope: AlexNet/Sequential[features]/Conv2d[0]
  %18 : Float(10, 64, 55, 55) = onnx::Relu(%17), scope: AlexNet/Sequential[features]/ReLU[1]
  %19 : Float(10, 64, 27, 27) = onnx::MaxPool[kernel_shape=[3, 3], pads=[0, 0, 0, 0], strides=[2, 2]](%18), scope: AlexNet/Sequential[features]/MaxPool2d[2]
  # ---- omitted for brevity ----
  %output1 : Float(10, 1000) = onnx::Gemm[alpha=1, beta=1, broadcast=1, transB=1](%45, %learned_14, %learned_15), scope: AlexNet/Sequential[classifier]/Linear[6]
  return (%output1);
}
```


其实我们也可以借助 ONNX 检查中间表征，不过这里并不介绍。后面加载另外一个框架并执行推理同样非常简单。如下所示，我们可以从 caffe2 中加载 ONNX 的后端，并将前面保存的模型加载到该后端，从而在新框架下进行推理。这里我们能选择执行推理的硬件，并直接推理得出输出结果。


```
import caffe2.python.onnx.backend as backend
import numpy as np
import onnx

model = onnx.load("alexnet.onnx")
rep = backend.prepare(model, device="CUDA:0") # or "CPU"
outputs = rep.run(np.random.randn(10, 3, 224, 224).astype(np.float32))
```


其实也就两三行代码涉及 ONNX 的核心操作，即导出模型、加载模型和加载另一个框架的后端。TensorFlow 或 CNTK 等其它框架的具体 API 可能不一样，但主要过程也就这简单的几步。

**怎样优化 ONNX**

前面就已经介绍了 Model Zoo、ONNX Runtime 和 ONNX.JS，现在，我们可以具体看看它们都是什么，它们怎样才能帮助我们优化 ONNX 模型的选择与推理速度。

**Model Zoo**

ONNX Model Zoo 包含了一系列预训练模型，它们都是 ONNX 格式，且能获得当前最优的性能。因此只要下载这样的模型，我们本地不论是 TensorFlow 还是 MXNet，只要是只是能加载模型的框架，就能运行这些预训练模型。
- 项目地址：[https://github.com/onnx/models](https://link.zhihu.com/?target=https%3A//github.com/onnx/models)

更重要的是，这个 Model Zoo 不仅有调用预训练模型的代码，它还为每个预训练模型开放了对应的训练代码。训练和推理代码都是用 Jupyter Notebook 写的，数据和模型等都有对应的链接。

目前该 Model Zoo 主要从图像分类、检测与分割、图像超分辨、机器翻译和语音识别等 14 个方向包含 19 种模型，还有更多的模型还在开发中。如下展示了图像分类中已经完成的模型，它们都是通用的 ONNX 格式。
![](https://pic2.zhimg.com/v2-83688bf86d9d5f0589586dbea147c369_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='914' height='758'></svg>)
此外在这次的研讨会中，Model Zoo 的维护者还和大家讨论了目前面临的问题及解决方法，例如目前的预训练模型主要集中在计算机视觉方面、ONNX 缺少一些特定的 OP、权重计算图下载慢等。因此 Model Zoo 接下来也会更关注其它语音和语言等模型，优化整个 GitHub 项目的下载结构。

**ONNX Runtime**

微软开源的 ONNX Runtime 推理引擎支持 ONNX 中定义的所有运算单元，它非常关注灵活性和推理性能。因此不论我们的开发环境是什么，Runtime 都会基于各种平台与硬件选择不同的自定义加速器，并希望以最小的计算延迟和资源占用完成推理。
- 文档地址：[https://docs.microsoft.com/en-us/python/api/overview/azure/onnx/intro](https://link.zhihu.com/?target=https%3A//docs.microsoft.com/en-us/python/api/overview/azure/onnx/intro)

ONNX Runtime 可以自动调用各种硬件加速器，例如英伟达的 CUDA、TensorRT 和英特尔的 MKL-DNN、nGraph。如下所示，ONNX 格式的模型可以传入到蓝色部分的 Runtime，并自动完成计算图分割及并行化处理，最后我们只需要如橙色所示的输入数据和输出结果就行了。
![](https://pic3.zhimg.com/v2-bb8e04e6d72573c7bb6d1a7950b1ae7e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='510'></svg>)
其实在实际使用的时候，开发者根本不需要考虑蓝色的部分，不论是编译还是推理，代码都贼简单。如下所示，导入 onnxruntime 模块后，调用 InferenceSession() 方法就能导入 ONNX 格式的模型，并完成上图一系列复杂的优化。最后只需要 session.run() 就可以进行推理了，所有的优化过程都隐藏了细节。


```
import onnxruntime

session = onnxruntime.InferenceSession("your_model.onnx") 
prediction = session.run(None, {"input1": value})
```


在研讨会中，开发者表示 Runtime 的目标是构建高性能推理引擎，它需要利用最好的加速器和完整的平台支持。只需要几行代码就能把计算图优化一遍，这对 ONNX 格式的模型是个大福利。

**ONNX.JS**

ONNX.js 是一个在浏览器上运行 ONNX 模型的库，它采用了 WebAssembly 和 WebGL 技术，并在 CPU 或 GPU 上推理 ONNX 格式的预训练模型。
- 项目地址：[https://github.com/Microsoft/onnxjs](https://link.zhihu.com/?target=https%3A//github.com/Microsoft/onnxjs)
- Demo 展示地址：[https://microsoft.github.io/onnxjs-demo](https://link.zhihu.com/?target=https%3A//microsoft.github.io/onnxjs-demo)

通过 ONNX.js，开发者可以直接将预训练的 ONNX 模型部署到浏览器，这些预训练模型可以是 Model Zoo 中的，也可以是自行转换的。部署到浏览器有很大的优势，它能减少服务器与客户端之间的信息交流，并获得免安装和跨平台的机器学习模型体验。如下所示为部署到网页端的 SqueezeNet：
![](https://pic3.zhimg.com/v2-2ec45cb8665ebe2b1b95f57f52e00a32_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='481'></svg>)
如上若是选择 GPU，它会采用 WebGL 访问 GPU。如果选择 CPU，那么其不仅会采用 WebAssembly 以接近原生的速度执行模型，同时也会采用 Web Workers 提供的「多线程」环境来并行化数据处理。该项目表明，通过充分利用 WebAssembly 和 Web Workers，CPU 可以获得很大的性能提升。这一点在项目提供的 Benchmarks 中也有相应的展示：
![](https://pic1.zhimg.com/v2-7a08f50d504bbf0151b5f5b69e2d494c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='398'></svg>)
以上微软在 Chrome 和 Edge 浏览器中测试了 ResNet-50 的推理速度，其中比较显著的是 CPU 的推理速度。这主要是因为 Keras.js 和 TensorFlow.js 在任何浏览器中都不支持 WebAssembly。

最后，从 ONNXIFI 到 ONNX.js，开源社区已经为 ONNX 格式构建出众多的优化库、转换器和资源。很多需要支持多框架的场景也都将其作为默认的神经网络格式，也许以后，ONNX 真的能统一神经网络之间的江湖。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*



