# TensorRT学习笔记



## 1.1 初始化tensorRT的方式

有两种方式初始化TensorRT： 
1、创建**IBuilder**对象去优化网络 
2、创建**IRuntime**对象执行一个被优化的网络

这两种方式，你都需要先实现一个**logging interface**（日志接口），TensorRT会通过这个接口进行报错、警告和消息提示

```
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
1234567891011
```

注意：你可能会创建很多的IBuilder和IRuntime，但是logger是一个**单例**！！！

------

## 1.2 在C++中创建一个网络定义

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

------

## 1.3 在C++使用解析器导入一个模型

使用C++解析器导入你需要遵循以下的步骤： 
1、创建TensorRT的builder和network 
2、为特殊的格式创建TensorRT的解析器 
3、使用解析器解析导入的模型并填充网络

注意：builder必须在network之前创建，因为builder是network的工厂；不同的解析器有不同的机制来标记网络输出。

### 1.3.1 导入caffe模型

```
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
12345678910
```

注意： 
最后的参数表示解析器会生成一个32位浮点数的网络，使用DataType::kHALF表示会生成16位浮点数的模型网络(原文如下：The final argument instructs the parser to generate a network whose weights are 32-bit floats. Using DataType::kHALF would generate a model with 16-bit weights instead)

除了填充网络定义之外，解析器返回一个字典，该字典从Caffe blob名称映射到TensorRT张量，与Caffe不同的是，TensorRT网络定义不存在in-place操作的概念。当一个Caffe模型使用一个in-place操作时，字典中返回的TensorRT张量对应于最后一个写入的blob。举个例子，如果一个卷积写进一个blob后面跟着一个in-place的ReLU，这个blob的名字会映射到TensorRT张量这是ReLU的输出

### 1.3.2 使用UFF导入tensorflow模型

和caffe一样4个步骤：

```
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
12345678910
```

ONNX模型导入就不做介绍了，基本流程一样

## 1.4 创建一个Network

这一节主要是介绍怎么使用C++API自己构造一个网络

```
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
12345678910111213141516171819
```

## 1.5 构建引擎

下一步是调用TensorRT builder来创建一个优化的runtime，构建器的功能之一是搜索CUDA内核的目录，以获得最快的实现，因此需要使用相同的GPU来构建优化引擎。

builder有很多属性可以去设置，这些属性可以控制网络运行的精度，以及自动调优参数。

您还可以查询建造器来找出哪些降低精度类型是被硬件原生支持的。

两个最主要的参数是maximum batch size （最大的批处理数量）和 maximum workspace size（最大的工作空间） 
1、最大批处理大小指定TensorRT将优化的批处理的数量，在运行时，更小的批处理数量可能被选择 
2、每层的算法通常需要临时工作空间，这个参数限制了网络的任一层的最大空间。如果没有划分足够的空间，会导致一个被给定的层无法实现

```
//使用builder创建引擎
builder->setMaxBatchSize(maxBatchSize);
builder->setMaxWorkspaceSize(1 << 20);
ICudaEngine* engine = builder->buildCudaEngine(*network);
1234
```

当引擎被创建，TensorRT会拷贝权重

## 1.6 序列化模型

引擎构建需要一些时间，你可以将引擎序列化，可以减少以后运行的时间。序列化后的引擎不能移植，因为其生成是根据现在特定版本的TensorRT和GPU的。

```
//序列化
IHostMemory *serializedModel = engine->serialize();
// store model to disk
// <…>
serializedModel->destroy();
//反序列化
IRuntime* runtime = createInferRuntime(gLogger);
ICudaEngine* engine = runtime->deserializeCudaEngine(modelData, modelSize,nullptr);
12345678
```

## 1.7 进行推断

一旦你有了一个引擎，你就可以执行推断：

```
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
1234567891011
```

## 1.8 内存管理

TensorRT提供了两种机制，允许应用程序对设备内存进行更多的控制

默认情况下，在创建IExecutionContext时，会分配持久设备内存来保存激活数据。

为了避免这种分配可以使用createExecutionContextWithoutDeviceMemory这个函数。然后，应用程序的职责就是调用IExecutionContext：：setDeviceMemory（）来提供运行网络所需的内存。可以通过ICudaEngine::getDeviceMemorySize()获取内存块的大小

此外，应用程序还可以通过实现IGpuAllocator接口，提供在构建和运行时使用的自定义分配器。一旦接口实现，调用setGpuAllocator(&allocator);在IBuilder或IRuntime接口上。所有的设备内存都将通过这个接口分配和释放

## 2.1 深度学习框架与TensorRT

并不支持所有的caffe和TensorFlow的操作： 
1、caffe的支持操作：
```
Convolution 
Pooling 
InnerProduct 
SoftMax 
ReLU, TanH, Sigmoid 
LRN 
Power 
ElementWise 
Concatenation 
Deconvolution 
BatchNormalization 
Scale 
Crop 
Reduction 
Reshape 
Permute 
Dropout
```
2、TensorFlow的支持操作：
```
Placeholder 
Const 
Add, Sub, Mul, Div, Minimum and Maximum 
iasAdd 
Negative, Abs, Sqrt, Rsqrt, Pow, Exp and Log 
usedBatchNorm 
ReLU, TanH, Sigmoid 
SoftMax 
Mean 
ConcatV2 
Reshape 
Transpose 
Conv2D 
DepthwiseConv2dNative 
ConvTranspose2D 
MaxPool ‣ AvgPool 
Pad is supported if followed by one of these TensorFlow layers:Conv2D,DepthwiseConv2dNative, MaxPool, and AvgPool
```











# python version

总结上文
在进入第二部分前，对第一部分的业务流程做一个总结：

创建流程图

![这里写图片描述](https://img-blog.csdn.net/20180831183450380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTI0NzY3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

推理流程图

![这里写图片描述](https://img-blog.csdn.net/20180831183436443?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTI0NzY3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



### 1.1 通过python使用TensorRT

只简单说明从tensorflow导入模型

```python
import tensorrt as trt
from tensorrt.parsers import uffparser
import uff

#实现logging接口
G_LOGGER = trt.infer.ConsoleLogger(trt.infer.LogSeverity.ERROR)
#创建引擎
engine = uff.from_tensorflow_frozen_model(frozen_file, ["fc2/Relu"])
#创建解析器，和C++不一样，不是从builder中创建的解析器
parser = uffparser.create_uff_parser()
#注册输入和输出
parser.register_input("Placeholder", (1, 28, 28), 0)
parser.register_output("fc2/Relu")
# 将模型导入引擎
trt.utils.uff_to_trt_engine(G_LOGGER,uff_model,parser,MAX_BATCHSIZE,MAX_WORKSPACE)
```

### 1.2 将Frozen Graph转化为UFF

```python
//通过命令行将.pb的Frozen Graph转化为UFF文件
convert-to-uff tensorflow -o name_of_output_uff_file --input-file name_of_input_pb_file -O name_of_output_tensor
//通过一下命令显示tensorflow的各层
convert-to-uff tensorflow --input-file name_of_input_pb_file -l
```
### 1.3 从tensorflow生成uff

```python
#尝试导入uff模块
try:
    import uff
except ImportError:
        raise ImportError("""Please install the UFF Toolkit""")

OUTPUT_NAMES = ["fc2/Relu"]
UFF_OUTPUT_FILENAME = path + "/trained_lenet5.uff"

def train(xxx):
    #省略训练过程
    #。。。
    #。。。
    #训练完毕后

    #导出当前计算图的GraphDef部分
    graphdef = tf.get_default_graph().as_graph_def()
    #保存指定的节点，并将节点值保存为常数
    frozen_graph = tf.graph_util.convert_variables_to_constants(sess, graphdef, OUTPUT_NAMES)
    #删除不需要进行推理（inference）的节点
    return tf.graph_util.remove_training_nodes(frozen_graph)

#获得冻结计算图
frozen_graph = run_training(MNIST_DATASETS)
#将计算图转化为UFF形式
uff.from_tensorflow(graphdef=frozen_graph,
                        output_filename=UFF_OUTPUT_FILENAME,
                        output_nodes=OUTPUT_NAMES,
                        text=True)
```
### 1.4 通过python使用UFF（官方例子tf_to_trt）

```python
    #截取代码
    
    #这个就是上面的train包装了一下
    #或者可以从文件读取，应该也能读取冻结计算图文件再进行转换
    tf_model = lenet5.learn()
    
    #导入UFF模型
    uff_model = uff.from_tensorflow(tf_model, ["fc2/Relu"])
    #Convert Tensorflow model to TensorRT model
    #创建一个解析器
    parser = uffparser.create_uff_parser()
    #注册输入和输出
    parser.register_input("Placeholder", (1, 28, 28), 0)
    parser.register_output("fc2/Relu")
    #记得实现logging接口
    G_LOGGER = trt.infer.ConsoleLogger(trt.infer.LogSeverity.INFO)
    #构建引擎
    engine = trt.utils.uff_to_trt_engine(G_LOGGER,
                                              uff_model,
                                              parser,
                                              MAX_BATCHSIZE,
                                              MAX_WORKSPACE)
    #判定引擎不为空
    assert(engine)
    #获得执行上下文
    context = engine.create_execution_context()
    
    #进行推断
    print("\n| TEST CASE | PREDICTION |")
    for i in range(ITERATIONS):
        img, label = lenet5.get_testcase()
        img = img[0]
        label = label[0]
        #应该就只推断了一个
        out = infer(context, img, 1)
        print("|-----------|------------|")
        print("|     " + str(label) + "     |      " + str(np.argmax(out)) + "     |")

# API CHANGE: Try to generalize into a utils function
#Run inference on device
#推断过程
def infer(context, input_img, batch_size):
    #load engine
    #通过上下文载入引擎
    engine = context.get_engine()
    #不是很明白，这个判断的是什么
    assert(engine.get_nb_bindings() == 2)
    #create output array to receive data
    #创建输出的数组去接受数据
    dims = engine.get_binding_dimensions(1).to_DimsCHW()
    #计算数组的大小
    elt_count = dims.C() * dims.H() * dims.W() * batch_size
    #convert input data to Float32
    #将输入转化成float32
    input_img = input_img.astype(np.float32)
    #Allocate pagelocked memory
    #cuda申请页锁内存，即CPU和GPU共同拥有这块内存，省去显示传输过程
    output = cuda.pagelocked_empty(elt_count, dtype=np.float32)

    #alocate device memory
    #为输入输出申请GPU内存
    d_input = cuda.mem_alloc(batch_size * input_img.size * input_img.dtype.itemsize)
    d_output = cuda.mem_alloc(batch_size * output.size * output.dtype.itemsize)
    #返回的是地址？可能需要研究一下
    bindings = [int(d_input), int(d_output)]
    #创建CUDA流
    stream = cuda.Stream()
    
    #transfer input data to device
    #将数据拷贝到GPU
    cuda.memcpy_htod_async(d_input, input_img, stream)
    #execute model
    #推断流程
    context.enqueue(batch_size, bindings, stream.handle, None)
    #transfer predictions back
    #计算完数据同步回CPU
    cuda.memcpy_dtoh_async(output, d_output, stream)
    
    #return predictions
    return output
```
