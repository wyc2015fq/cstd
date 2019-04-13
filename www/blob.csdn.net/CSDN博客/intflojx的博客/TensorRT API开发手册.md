
# TensorRT API开发手册 - intflojx的博客 - CSDN博客


2019年01月01日 16:20:46[paulpanwang](https://me.csdn.net/intflojx)阅读数：218


[第一章 综述](#%E7%AC%AC%E4%B8%80%E7%AB%A0%20%E7%BB%BC%E8%BF%B0)
[1.1 TensorRTLayers](#1.1%20TensorRTLayers)
[1.2 关键概念](#1.2%20%E5%85%B3%E9%94%AE%E6%A6%82%E5%BF%B5)
[1.3 TensorRT API’s](#1.3%20TensorRT%20API%E2%80%99s)
[1.3.1 Python Samples](#1.3.1%20Python%20Samples)
[1.3.2 Python Workflows](#1.3.2%20Python%20Workflows)
# 第一章 综述
NVIDIA的TensorRT是一个基于GPU高性能前向运算的C++库。TensorRT导入网络定义，通过合并tensors与layers，权值转换，选择高效中间数据类型，基于层参数与性能评估的选择，来进行网络优化。
TensorRT提供模型导入途径来帮助你对训练好的深度学习模型进行表示，同于TensorRT的优化与运行。这种优化工具自动进行图优化，层融合，并利用多样化的高度优化的计算核心寻找最快的模型实现方式。输出可以前向运算时可以运行这个网络的运行库。
TensorRT提供了一种基础能力，使得用户可以在Pascal与VoltaGPU上选择性进行快速多精度应用。TensorRT用gcc4.8进行编译。
## 1.1 TensorRTLayers
TensorRT直接支持以下类型的层：
**Activation**
激活层通过逐像素激活函数实现。支持的激活类型包括，RELU，tanh与sigmoid.
**BatchGemm**
批矩阵相乘层实现了批矩阵的乘法运算。在矩阵相乘前矩阵可以进行转置。当维度不匹配的时候会进行适当的矩阵扩展。
**Concatenation**
连接层在通道维度上将宽高相同的tensors进行连接。
**Constant**
Constant层是一个提供本层参数的tensor，使得常量在计算时可以方便使用。
**Convolution**
Convolution层计算3D卷积（通道，高，宽），可选择是否使用偏置。
**Deconvolution**
Deconvolution层实现了去卷积运算，可选择是否使用偏置。
**ElementWise**
ElementWise层也叫做Eltwise层，实现了逐像素操作。支持加法，乘法，最大，减法，除法，指数运算。
**Flatten**
Flatten层在保持batch_size的同时，将输入转成一个向量。假设第一个维度表示batch大小。Flatten只能在全连接层前使用。
**FullConnected**
全连接层实现了矩阵向量积运算，可选择是否使用偏置。
**Gather**
Gather层实现了聚集操作，将一个数据tensor，一个索引tensor，一个数据tensor axis作为输入，利用索引index将数据tensor重建索引。当前，只有TensorRT C++API支持。
**LRN**
LRN层实现跨通道局部响应归一化。
**MartrixMultipy**
矩阵乘法层实现了一系列的矩阵相乘运算。在乘法运算前可以选择是否将矩阵转置。当维度不匹配的时候会进行适当的矩阵扩展。
**Padding**
Padding层实现了tensors的空间0扩展。Padding可以是不同维度，不对称的，也可以是正的（tensor扩展结果），或者负的（均值结果）。
**Plugin**
Plugin层允许用户实现原生TensorRT不支持的层。
**Pooling**
Pooling层在通道维度实现池化操作。支持最大池化与均值池化。
**Ragged SoftMax**
Ragged SoftMax实现了包含不同长度序列tensor的跨通道的Softmax。序列的长度利用另外一个tensor输入。
**Reduce**
Reduce使用降维算法实现了tensors降维操作。降维运算支持像素相乘，最大，最小，平均。目前只有TensorRT的C++API支持这个层。
**RNN**
这个层在RNNV2层出现后已经弃用了，然而还是向下兼容。
**RNNv2**
RNNv2实现了类似递归神经网络，GRU,LSTM这样的递归层。当前支持RNN，GPU，LSTM。
**Scale**
Scale层实现了逐tensor，逐通道，逐权重的仿射变换，与或运算，以及常数幂运算。
**Shuffle**
Shuffle层用来实现tensors得重新排序，可以用来进行reshape或者数据转换。
**SoftMax**
SoftMax实现了跨通道的softmax。
**Squeeze**
Squeeze层将tensor中维度为1的shape。
**TopK**
TopK层在一个维度中寻找TopK元素，返回降维的tensor，与一个位置索引的tensor。
**Unary**
Unary支持元素级的一元操作。支持exp，log，sqrt，recip，abs与neg操作。
**注：**
BatchNormalization可以利用TensorRT的Scale层实现。
卷积层进行的操作实际上是相关操作。这样主要是考虑通过TensorRT API导入权重，而不是通过NVCaffe解析库。
## 1.2 关键概念
确保你对一下关键概念熟悉：
网络结果定义（Network definition）
网络定义有由一系列层与tensors集组成。
**层（layer）**
每个层根据一系列输入tensors计算输出的tensors。层具有一些参数，例如convolution size，stride，filter wights。
**Tensor**
Tensor既可以是网络的输入，也可以是输出。Tensors通过数据类型（data-type）来指定精确度，例如16-bit float或者32bit float。Tensors有三个维度，例如通道（channels），宽（weight），高（height）。输入tensors的维度由应用决定，输出的tensor维度由builder推理出来。支持的维度是N（P_1,P_2,P_3）CHW格式，其中P_1,P_2等是索引维。Tensors最多可以有Dims::MAX_DIMENSIONS维度，设为常量8。
Tensors在batch间可见。这样的tensor的batchsize维度（公式中为N）为1，不管网络在运行的时候batchsize多大。所以，当这个tensor是输入的时候，他会独立于batchsize。适当的时候使用的是标准的广播（broadcasting）规则。
输入tensor是否在batch间广播取决于网络定义，输出tensor则取决于builder。在确保build过程batch中每个元素数据的前提下，builder会将广播中间与输出tensors。所以batch间广播的操作往往会产生tensor在batch间广播的效果。例如，运算将在广播前进行，广播将推到运算链里。如果整个操作链都在没有广播的状态下实现，输出的tensor可能作为一个batch间广播的掩模，意味着是否在batch间广播取决于应用需求。
在读写batch间广播的tensor对应内存的时候，需要有些特别注意的问题。不管在运行是网络的batchsize是多少，数据都是按照batchsize等于1排列的。因此，batch间共享的tensors比由N个相同数据副本对应N各batch元素的tensors会更加节省内存。
每个层与tensor都有一个名字，这些名字在解析或者读取TensorRT生成日志的时候有用。
当使用NvCaffeParser时，tensor与layer的名字是根据NVCaffe的prototxt文件命名。
**数据类型（DataType）**
Tensors支持的有效数据类型是Kfloat,Khalf,Kint8与Kint32。对于网络输入来说，需要在设置网络输入时指定数据类型。其它的tensors类型可以通过tensor中的setType方式实现。输入与输出不能是kINT8类型。kINT32表示索引值，Kint8表示编码成8bit的浮点值。详见SampleINT8-Calibration 与8-bit Inference，取得更多有关8-bit整形的inference性能。
**Broadcasting**
Broadcasting当两个tensor间维度不匹配时起作用。当两个tensor同个索引的维度长度不等，并且其中一个tensors维度是1，这时broadcasting工作。假设两个tensor，一个为（N,1,H,W）一个为（1,C,H,W），第二个tensor在batch维度broadcastN次，第一个tensor在channel维度broadcastC次，得到（N,C,H,W）。不是所有层都支持Broadcasting。取得更多NumPy中broadcasting的意义，见Braodcasting。
## 1.3 TensorRT API’s
TensorRT API允许开发者导入，校验，生成与部署优化网络。网络可以直接通过NVCaffe或者其他支持UFF与ONNX格式的框架。也可以通过编程的方式，导入每层与相关参数与权重。
除了主要的C++接口外，TensorRT还带有python接口。TensorRT的python接口当前支持所有功能。接口通过NumPy格式引入层权重，通过PyCUDA引入输入输出数据。还提供了一系列公共函数来解决开发过程中可能会遇到的类似解析NVCaffe模型文件，从流中解析UFF模型，从UFF文件中读写PLAN文件这样的公共问题。这些功能都在tensorrt.utils中实现。
在tensorrt/examples/custom_layers中有python进行用户自实现层的例子。
注：
要使用户可以在python接口中使用自己的C++实现，还需要一些额外的依赖项，例如swig>=3.0，libnvinfer-dev。

### 1.3.1 Python Samples
Python接口支持之前只有C++接口支持的所有功能，包括：
NvCaffeParser
NvUffParser(TensorFlow)
NvONNXParser(Open Neural Network Exchange)
用于图定义的nvinfer API’s
用于建立高效前向引擎的builder
用于执行的前向运算引擎
用于注册自实现层的接口
用户可以在{PYTHON_PACKAGE_DIR}/tensorrt/examples中找到python示例。
TensorRT包自带一对示例应用实现。如果是TensorRT安装到系统中则可以找到。
Table1 应用例程位置
想了解更多用python将模型导入到TensorRT中，请参考NVCaffe Python Workflow,TensorFlow Python Workflow, and Converting A Model From An UnsupportedFramework To TensorRT With The TensorRT Python API。

### 1.3.2 Python Workflows
Python是一种流行并且通常再数据科学非常高效的语言并且再许多深度学习框架中都有使用。这里提供一下几种情况的使用用例
有一个ONNX格式的模型，TensorRT包含了ONNX解析器，使得我们可以直接将模型加载到tensorRT中。
有一个TensorFlow模型，使用者希望使用TensorRT调用。
将TensorFlow模型转为TensorRT格式
有一个NvCaffe模型，希望使用TensorRT调用。
将NVCaffe模型转为TensorRT格式。
开发者希望将TensorRT引擎作为像网站后端这样大规模的应用。
开发者希望TensorRT调用当前UFF不支持的训练框架训练的模型或者非NVCaffe训练出来的模型。
如需了解以上用例的详细步骤，请看NVCaffe Workflow,TensorFlow Workflow, andConverting A Model From An Unsupported Framework To TensorRT With The TensorRTPython API.
2. TensorRT初始化
有两种方式初始化TensorRT：
1、创建IBuilder对象去优化网络
2、创建IRuntime对象执行一个被优化的网络
这两种方式，你都需要先实现一个logging interface（日志接口），TensorRT会通过这个接口进行报错、警告和消息提示
class Logger : public ILogger
{
void log(Severity severity, const char* msg) override
{
//不提示INFO信息，只显示警告和错误
if (severity != Severity::kINFO)
{
std::cout << msg << std::endl;
}
}
}gLogger;
注意：你可能会创建很多的IBuilder和IRuntime，但是logger是一个单例！！！
1.2 在C++中创建一个网络定义
使用tensorRT的第一步就是从你的model中创建一个tensorRT的网络定义
可以从别的深度学习框架中进行导入，现在支持：
1、Caffe（both BVLC）
2、ONNX
3、UFF（used for tensorflow）
同样也可以使用TensorRT API直接构造模型，但是需要在网络层中使用少量API定义每一层，并且实现自己导入参数机制，来导入已经训练完毕的模型参数
在以上的方式你必须明确告诉tensorRT哪些张量是输出，因为被确定为输出的张量不会因为采取加速而被builder进行优化。
输入和输出张量都需要进行命名（使用ITensor::setName()）
同时推理的时候，需要为引擎提供输入和输出的buff缓存指针
还有对于tensorRT网络定义很重要的一个方面，就是包含指向模型权重的指针，它会被builder拷贝到优化引擎里
注意：如果网络定义是从语法中创建的，解析器占用权重的内存，因此解析器在builder运行前都不能被删除
1.3 在C++使用解析器导入一个模型
使用C++解析器导入你需要遵循以下的步骤：
1、创建TensorRT的builder和network
2、为特殊的格式创建TensorRT的解析器
3、使用解析器解析导入的模型并填充网络
注意：builder必须在network之前创建，因为builder是network的工厂；不同的解析器有不同的机制来标记网络输出。
1.3.1 导入caffe模型
//1、创建builder和network
IBuilder* builder = createInferBuilder(gLogger);
INetworkDefinition* network = builder->createNetwork();
//2、创建caffe的解析器
ICaffeParser* parser = createCaffeParser();
//3、使用解析器导入模型
const IBlobNameToTensor* blobNameToTensor = parser->parse("deploy_file" ,"modelFile", *network, DataType::kFLOAT);
//4、指定模型的输出
for (auto& s : outputs)
network->markOutput(*blobNameToTensor->find(s.c_str()));
注意：
最后的参数表示解析器会生成一个32位浮点数的网络，使用DataType::kHALF表示会生成16位浮点数的模型网络(原文如下：The final argument instructs the parser to generate a network whose weights are 32-bit floats. Using DataType::kHALF would generate a model with 16-bit weights instead)
除了填充网络定义之外，解析器返回一个字典，该字典从Caffe blob名称映射到TensorRT张量，与Caffe不同的是，TensorRT网络定义不存在in-place操作的概念。当一个Caffe模型使用一个in-place操作时，字典中返回的TensorRT张量对应于最后一个写入的blob。举个例子，如果一个卷积写进一个blob后面跟着一个in-place的ReLU，这个blob的名字会映射到TensorRT张量这是ReLU的输出
1.3.2 使用UFF导入tensorflow模型
和caffe一样4个步骤：
//1、创建builder和network
IBuilder* builder = createInferBuilder(gLogger);
INetworkDefinition* network = builder->createNetwork();
//2、创建Uff的解析器
IUFFParser* parser = createUffParser();
//3、指定模型输入和输出
parser->registerInput("Input_0", DimsCHW(1, 28, 28), UffInputOrder::kNCHW);
parser->registerOutput("Binary_3");
//4、使用解析器导入模型
parser->parse(uffFile, *network, nvinfer1::DataType::kFLOAT);
1
2
3
4
5
6
7
8
9
10
ONNX模型导入就不做介绍了，基本流程一样
1.4 创建一个Network
这一节主要是介绍怎么使用C++API自己构造一个网络
//1、同样创造一个builder和netword
IBuilder* builder = createInferBuilder(gLogger);
INetworkDefinition* network = builder->createNetwork();
//2、为network添加输入层，带上输入的维度，一个network可以有多个输入，尽管例子中只有一个
auto data = network->addInput(INPUT_BLOB_NAME, dt, Dims3{1, INPUT_H,INPUT_W});
//3、添加一个卷积层
layerName->getOutput(0)
auto conv1 = network->addConvolution(*data->getOutput(0), 20, DimsHW{5, 5},weightMap["conv1filter"], weightMap["conv1bias"]);
conv1->setStride(DimsHW{1, 1});
//4、添加池化层
auto pool1 = network->addPooling(*conv1->getOutput(0), PoolingType::kMAX,DimsHW{2, 2});
pool1->setStride(DimsHW{2, 2});
//5、添加全连接和激活函数
auto ip1 = network->addFullyConnected(*pool1->getOutput(0), 500,weightMap["ip1filter"], weightMap["ip1bias"]);
auto relu1 = network->addActivation(*ip1->getOutput(0),ActivationType::kRELU);
//6、添加SoftMax层计算最后的概率并将其设置为输出
auto prob = network->addSoftMax(*relu1->getOutput(0));
prob->getOutput(0)->setName(OUTPUT_BLOB_NAME);
network->markOutput(*prob->getOutput(0));
1.5 构建引擎
下一步是调用TensorRT builder来创建一个优化的runtime，构建器的功能之一是搜索CUDA内核的目录，以获得最快的实现，因此需要使用相同的GPU来构建优化引擎。
builder有很多属性可以去设置，这些属性可以控制网络运行的精度，以及自动调优参数。
您还可以查询建造器来找出哪些降低精度类型是被硬件原生支持的。
两个最主要的参数是maximum batch size （最大的批处理数量）和 maximum workspace size（最大的工作空间）
1、最大批处理大小指定TensorRT将优化的批处理的数量，在运行时，更小的批处理数量可能被选择
2、每层的算法通常需要临时工作空间，这个参数限制了网络的任一层的最大空间。如果没有划分足够的空间，会导致一个被给定的层无法实现
//使用builder创建引擎
builder->setMaxBatchSize(maxBatchSize);
builder->setMaxWorkspaceSize(1 << 20);
ICudaEngine* engine = builder->buildCudaEngine(*network);
当引擎被创建，TensorRT会拷贝权重
1.6 序列化模型
引擎构建需要一些时间，你可以将引擎序列化，可以减少以后运行的时间。序列化后的引擎不能移植，因为其生成是根据现在特定版本的TensorRT和GPU的。
//序列化
IHostMemory *serializedModel = engine->serialize();
// store model to disk
// <…>
serializedModel->destroy();
//反序列化
IRuntime* runtime = createInferRuntime(gLogger);
ICudaEngine* engine = runtime->deserializeCudaEngine(modelData, modelSize,nullptr);
1.7 进行推断
一旦你有了一个引擎，你就可以执行推断：
//1、创建一个上下文，主要用来存储中间值
IExecutionContext *context = engine->createExecutionContext();
//2、使用输入和输出blob名称来获得相应的输入和输出索引
int inputIndex = engine.getBindingIndex(INPUT_BLOB_NAME);
int outputIndex = engine.getBindingIndex(OUTPUT_BLOB_NAME);
//3、使用这些索引，设置一个缓冲数组，指向GPU上的输入和输出缓冲区
void* buffers[2];
buffers[inputIndex] = inputbuffer;
buffers[outputIndex] = outputBuffer;
//TensorRT的执行通常是异步的，因此将核加入队列放在CUDA流上
context.enqueue(batchSize, buffers, stream, nullptr);
1.8 内存管理
TensorRT提供了两种机制，允许应用程序对设备内存进行更多的控制
默认情况下，在创建IExecutionContext时，会分配持久设备内存来保存激活数据。
为了避免这种分配可以使用createExecutionContextWithoutDeviceMemory这个函数。然后，应用程序的职责就是调用IExecutionContext：：setDeviceMemory（）来提供运行网络所需的内存。可以通过ICudaEngine::getDeviceMemorySize()获取内存块的大小
此外，应用程序还可以通过实现IGpuAllocator接口，提供在构建和运行时使用的自定义分配器。一旦接口实现，调用setGpuAllocator(&allocator);在IBuilder或IRuntime接口上。所有的设备内存都将通过这个接口分配和释放
2.1 深度学习框架与TensorRT
并不支持所有的caffe和TensorFlow的操作：
1、caffe的支持操作：
‣ Convolution
‣ Pooling
‣ InnerProduct
‣ SoftMax
‣ ReLU, TanH, Sigmoid
‣ LRN
‣ Power
‣ ElementWise
‣ Concatenation
‣ Deconvolution
‣ BatchNormalization
‣ Scale
‣ Crop
‣ Reduction
‣ Reshape
‣ Permute
‣Dropout
2、TensorFlow的支持操作：
‣ Placeholder
‣ Const
‣ Add, Sub, Mul, Div, Minimum and Maximum
‣BiasAdd
‣ Negative, Abs, Sqrt, Rsqrt, Pow, Exp and Log
‣FusedBatchNorm
‣ ReLU, TanH, Sigmoid
‣ SoftMax
‣ Mean
‣ ConcatV2
‣ Reshape
‣ Transpose
‣ Conv2D
‣ DepthwiseConv2dNative
‣ ConvTranspose2D
‣ MaxPool ‣ AvgPool
‣ Pad is supported if followed by one of these TensorFlow layers:Conv2D,DepthwiseConv2dNative, MaxPool, and AvgPool


