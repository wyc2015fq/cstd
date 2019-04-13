
# 基于TensorRT的神经网络推理与加速 - intflojx的博客 - CSDN博客


置顶2018年08月15日 22:52:32[paulpanwang](https://me.csdn.net/intflojx)阅读数：2111


**一. 引言**
NVIDIA TensorRT是一种高性能神经网络推理(Inference)引擎，用于在生产环境中部署深度学习应用程序，应用于图像分类、分割和目标检测等，可提供最大的推理吞吐量和效率。TensorRT是第一款可编程推理加速器，能加速现有和未来的网络架构。TensorRT包含一个为优化生产环境中部署的深度学习模型而创建的库，可获取经过训练的神经网络(通常使用32位或16位数据)，并针对降低精度的INT8运算来优化这些网络。借助CUDA的可编程性，TensorRT可以大幅度加速，服务提供商能够以经济实惠的成本部署这些计算密集型人工智能工作负载。TensorRT包含一个为优化生产环境中部署的深度学习模型而创建的库，可获取经过训练的神经网络(通常使用32位或16位数据)，并针对降低精度的INT8运算来优化这些网络。借助CUDA的可编程性，TensorRT将能够加速助推深度 神经网络日益多样化、复杂的增长趋势。通过TensorRT的大幅度加速，服务提供商能够以经济实惠的成本部署这些计算密集型人工智能工作负载。已有来自各行各业的公司开始采用NVIDIA推理平台，借助此从数据中获得全新洞察，并为企业和消费者部署智能服务。
![](https://img-blog.csdn.net/20180815221117146?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
TensorRT由英伟达(NVIDIA)发布，目前包括TensorRT1、TensorRT 2、TensorRT 3，是深度学习软件包，支持FP16特性。TensorRT支持使用Caffe的模型。TensorRT相对简单易用，在深度学习算法推理阶段能将GPU的计算能力更大程度释放出来。 TensorRT在不断的改进过程中，在保证软件精度的同时，不断提高速度。TensorRT针对运行时性能自动优化训练过的神经网络。 TensorRT是一个C++库。
使用TensorRT包含两个阶段：构建(build)和部署(deployment)。在构建阶段，TensorRT对网络配置进行优化，并生成一个优化了的plan用于计算深度神经网络的前向传播。这个plan是一个优化了的目标代码，可以序列化存储在内存或磁盘上。 部署阶段通常采用长时间运行的服务或用户应用程序的形式，该服务或用户应用程序接受批量输入数据，通过对输入数据执行plan来执行推理，并返回批量输出数据。使用TensorRT，你无需在部署硬件上安装并运行深度学习框架。 TensorRT构建阶段：TensorRT运行时需要三个文件来部署一个分类神经网络：一个网络体系结构文件(deploy.prototxt)，已训练的权值(net.caffemodel)和一个标签文件为每个输出类提供一个名称。另外，你必须定义batch size和输出层。 TensorRT对神经网络图(neural network graph)进行了几个重要的转换和优化：消除未使用的输出的层以避免不必要的计算；在可能的情况下，convolution、bias和ReLU层被融合以形成单个层，包括垂直层融合和水平层融合。 在TensorRT解析器读入已训练的网络和配置文件后，TensorRT在构建阶段对API用户透明地执行其转换。 在构建阶段，TensorRT优化网络，在部署阶段，TensorRT以最小化延迟和最大化吞吐量运行优化了的网络。 TensorRT 2.1关键特性：(1)、支持自定义层；(2)、INT8支持以用于性能改进；(3)、提供递归神经网络(LSTM(Long Short-Term Memory)、GRU(Gated Recurrent Unit))实现；(4)、”original” RNN层实现。 2017年9月，NVIDIA发布了神经网络推理加速器TensorRT3，TensorRT 3是一款针对人工智能应用生产部署的高性能优化编译器和运行时引擎，用于在生产环境中部署深度学习程序。它能够快速优化、验证并部署经过训练的神经网络， 从而在超大型数据中心、嵌入式GPU或车用GPU平台上开展推理工作。它能够确保高度精确的INT8和FP16网络执行。 TensorRT 3能支持Caffe2、Mxnet、Pytorch、TensorFlow等所有的深度学习框架，将TensorRT 3和NVIDIA的GPU结合起来，能在所有的框架中进行超快速和高效的推理传输，支持图像和语言识别、自然语言处理、可视化搜索和个性化推荐等AI服务。 借助该推理引擎可以大幅提升云端及包括机器人、无人驾驶汽车在内的终端设备的推理性能，并有效降低成本。**使用TensorRT，你无需在部署硬件上安装并运行深度学习框架**。TensorRT可以从多种框架（包括Caffe2、MXNet 和 PyTorch）中导入 开放神经网络交换 (ONNX) 格式的模型。
对于计算量庞大的CNN，效率一直是大家所关注的，接触过深度网络压缩的同学应该知道网络压缩最关键的两个思路，剪枝和量化。TensorRT可以将FP32位权值数据优化为 FP16 或者 INT8，而推理精度不发生明显的降低。同时可以合并卷积操作，将不同的层进行合并（包括水平与垂直），可以将对结果无影响的部分进行删除达到压缩的效果。
关于TensorRT总结为以下几点：
> 1. TensorRT是NVIDIA开发的深度学习推理工具，只支持推理，不支持训练；

> 目前TensorRT已经支持Caffe、Caffe2、TensorFlow、MxNet、Pytorch等主流深度学习库；

> 2. TensorRT底层针对NVIDIA显卡做了多方面的优化，不仅仅是量化，可以和 CUDA CODEC SDK 结合使用，

> 也就是另一个开发包DeepStream；

> 3. TensorRT独立于深度学习框架，通过解析框架文件来实现，不需要额外安装DL库；

> 可从
> [https://developer.nvidia.com/nvidia-tensorrt-download](https://developer.nvidia.com/nvidia-tensorrt-download)
> 下载。可以参考官方TensorRT的介绍，更权威一些：
> [https://developer.nvidia.com/tensorrt](https://developer.nvidia.com/tensorrt)
**二. 使用方式**
TensorRT只能用来做Inference(推理)，不能用来进行train。 TensorRT基本处理过程：(1)、caffe model转化GIE的model，或者从磁盘或者网络加载GIE可用的model；(2)、运行GIE引擎(数据提前copy到GPU中)；(3)、提取结果。 转化GIE model两种方式：(1)、caffeToGIEModel；(2)、参考sampleMNIST API自己构建GIE model.
用深度神经网络解决监督机器学习问题包含两个步骤：第一步是使用GPU对海量标签数据进行深度神经网络训练，训练时需要迭代的通过网络进行前向传播和反向传播。最终会生成训练好的model文件。第二步是推理(Inference)即使用训练好的模型对新 数据做出预测，仅需通过网络进行前向传播。TensorRT是一款高性能的推理引擎，旨在为常见的深度学习应用如图像分类、分割、目标检测等提供最大的推理吞吐量和效率。针对运行时性能，TensorRT会优化已训练的神经网络。
下面以Caffe为例介绍TensorRT的使用：
1. caffeToGIEModel － 将 caffe model 转换到 TensorRT 格式
// 1.创建builder
IBuilder* builder = createInferBuilder(gLogger);
// 2.解析caffe模型，保存到 Network
INetworkDefinition* network = builder->createNetwork();
ICaffeParser* parser = createCaffeParser();
const IBlobNameToTensor* blobNameToTensor = parser->parse(locateFile(deployFile,                 directories).c_str(), locateFile(modelFile, directories).c_str(),*network, DataType::kFLOAT);
// 3.指定输出Tensor
for (auto& s : outputs)
network->markOutput(*blobNameToTensor->find(s.c_str()));
// 4.构建engine
builder->setMaxBatchSize(maxBatchSize);
builder->setMaxWorkspaceSize(1 << 20);
ICudaEngine* engine = builder->buildCudaEngine(*network);
assert(engine);
// 5.销毁parser
network->destroy();
parser->destroy();
// 6.将engine序列化到GIE，退出
gieModelStream = engine->serialize();
engine->destroy();
builder->destroy();
2. 执行过程 main
// 1.从caffe模型创建GIE模型，序列化到流
IHostMemory *gieModelStream{nullptr};
caffeToGIEModel("mnist.prototxt", "mnist.caffemodel", std::vector < std::string > { OUTPUT_BLOB_NAME }, 1, gieModelStream);
// x.数据获取（略）
// x.解析mean文件(略)
// 2.反序列化，得到Runtime engine
IRuntime* runtime = createInferRuntime(gLogger);
ICudaEngine* engine = runtime->deserializeCudaEngine(gieModelStream->data(), gieModelStream->size(), nullptr);
if (gieModelStream) gieModelStream->destroy();
// 3.创建上下文
IExecutionContext *context = engine->createExecutionContext();
// 4.运行inference
float prob[OUTPUT_SIZE];
doInference(*context, data, prob, 1);
// 5.销毁engine
context->destroy();
engine->destroy();
runtime->destroy();
3. 推理过程 doInference
const ICudaEngine& engine = context.getEngine();
// 传递给引擎的输入输出buffer指针- 需要精确的 IEngine::getNbBindings(),这里1个输入＋1个输出
assert(engine.getNbBindings() == 2);
void* buffers[2];
// 1.为了绑定buffer，需要知道输入和输出tensor的names
int inputIndex = engine.getBindingIndex(INPUT_BLOB_NAME),
outputIndex = engine.getBindingIndex(OUTPUT_BLOB_NAME);
// 2.创建 GPU buffer 和 stream
CHECK(cudaMalloc(&buffers[inputIndex], batchSize * INPUT_H * INPUT_W * sizeof(float)));
CHECK(cudaMalloc(&buffers[outputIndex], batchSize * OUTPUT_SIZE * sizeof(float)));
cudaStream_t stream;
CHECK(cudaStreamCreate(&stream));
// 3.通过DMA 输入到 GPU,  异步之行batch，并通过DMA回传
CHECK(cudaMemcpyAsync(buffers[inputIndex], input, batchSize * INPUT_H * INPUT_W * sizeof(float), cudaMemcpyHostToDevice, stream));
context.enqueue(batchSize, buffers, stream, nullptr);
CHECK(cudaMemcpyAsync(output, buffers[outputIndex], batchSize * OUTPUT_SIZE*sizeof(float), cudaMemcpyDeviceToHost, stream));
cudaStreamSynchronize(stream);
// 4.释放 stream 和 buffer
cudaStreamDestroy(stream);
CHECK(cudaFree(buffers[inputIndex]));
CHECK(cudaFree(buffers[outputIndex]));
**三. 模型转化**
对于caffe的支持比较简单，可以直接通过载入deploy file和caffemodel来做，而对于tensorflow则是通过转换为uff格式来加载，可以参考样例程序。
网络模型转换及部署可以分为三个步骤：
> 1）训练模型并保存为.pb文件；

> 2）将.pb文件转成.uff格式；

> 3）利用TensorRT加载并运行模型；

**四. 关于统一模型的讨论**
ONNX是Facebook、微软联合推出​的一个开放标准，旨在不同框架之间完成互操作。可以将不同框架的模型转为ONNX格式再使用TensorRT进行加速。以Pytorch为例，通过保存成.pth或者.pkl文件，再转为onnx，代码如下：
from torch.autograd import Variable
import torch.onnx
import pickle as pk
import os
from model import Generator,ESPCNGenerator
from PIL import Image
from torchvision.transforms import ToTensor, ToPILImage
UPSCALE_FACTOR = 3
netG = ESPCNGenerator(UPSCALE_FACTOR)
MODEL_G = 'netG_epoch_3_1.pth'
netG.load_state_dict(torch.load('epochs/' + MODEL_G))
netG = netG.cuda()
image = Image.open('./contour1.jpg')
dummy_input = Variable(ToTensor()(image), volatile=True).unsqueeze(0).cuda()
\#torch.onnx.export(netG, dummy_input, "SRGAN_Resnet.onnx")
torch.onnx.export(netG,dummy_input, "SRGAN_Resnet.onnx")
output = netG(dummy_input)
print(output)
保存成ONNX之后可以通过下面的工具进行模型可视化：
![](https://img-blog.csdn.net/20180815223148878?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从而知道网络在哪个位置转换出错，效果如下所示：
![](https://img-blog.csdn.net/20180815223314539?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
再通过编译onnx2trt：
\#include "NvOnnxParser.h"
\#include "onnx_utils.hpp"
\#include <google/protobuf/io/coded_stream.h>
\#include <google/protobuf/io/zero_copy_stream_impl.h>
\#include <google/protobuf/text_format.h>
\#include <fstream>
\#include <unistd.h> // For ::getopt
\#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
\#include <ctime>
\#include <fcntl.h> // For ::open
\#include <limits>
struct InferDeleter {
template<typename T>
void operator()(T* obj) const {
if( obj ) {
obj->destroy();
}
}
};
template<typename T>
inline std::shared_ptr<T> infer_object(T* obj) {
if( !obj ) {
throw std::runtime_error("Failed to create object");
}
return std::shared_ptr<T>(obj, InferDeleter());
}
// Logger for GIE info/warning/errors
class TRT_Logger : public nvinfer1::ILogger {
nvinfer1::ILogger::Severity _verbosity;
std::ostream* _ostream;
public:
TRT_Logger(Severity verbosity=Severity::kWARNING,
std::ostream& ostream=std::cout)
: _verbosity(verbosity), _ostream(&ostream) {}
void log(Severity severity, const char* msg) override {
if( severity <= _verbosity ) {
time_t rawtime = std::time(0);
char buf[256];
strftime(&buf[0], 256,
"%Y-%m-%d %H:%M:%S",
std::gmtime(&rawtime));
const char* sevstr = (severity == Severity::kINTERNAL_ERROR ? "    BUG" :
severity == Severity::kERROR          ? "  ERROR" :
severity == Severity::kWARNING        ? "WARNING" :
severity == Severity::kINFO           ? "   INFO" :
"UNKNOWN");
(*_ostream) << "[" << buf << " " << sevstr << "] "
<< msg
<< std::endl;
}
}
};
bool ParseFromFile_WAR(google::protobuf::Message* msg,
const char*                filename) {
int fd = ::open(filename, O_RDONLY);
google::protobuf::io::FileInputStream raw_input(fd);
raw_input.SetCloseOnDelete(true);
google::protobuf::io::CodedInputStream coded_input(&raw_input);
// Note: This WARs the very low default size limit (64MB)
coded_input.SetTotalBytesLimit(std::numeric_limits<int>::max(),
std::numeric_limits<int>::max()/4);
return msg->ParseFromCodedStream(&coded_input);
}
bool ParseFromTextFile(google::protobuf::Message* msg,
const char*                filename) {
int fd = ::open(filename, O_RDONLY);
google::protobuf::io::FileInputStream raw_input(fd);
raw_input.SetCloseOnDelete(true);
return google::protobuf::TextFormat::Parse(&raw_input, msg);
}
std::string onnx_ir_version_string(int64_t ir_version=::ONNX_NAMESPACE::IR_VERSION) {
int onnx_ir_major = ir_version / 1000000;
int onnx_ir_minor = ir_version % 1000000 / 10000;
int onnx_ir_patch = ir_version % 10000;
return (std::to_string(onnx_ir_major) + "." +
std::to_string(onnx_ir_minor) + "." +
std::to_string(onnx_ir_patch));
}
void print_usage() {
cout << "ONNX to TensorRT model parser" << endl;
cout << "Usage: onnx2trt onnx_model.pb" << "\n"
<< "                [-o engine_file.trt]  (output TensorRT engine)" << "\n"
<< "                [-t onnx_model.pbtxt] (output ONNX text file without weights)" << "\n"
<< "                [-T onnx_model.pbtxt] (output ONNX text file with weights)" << "\n"
<< "                [-b max_batch_size (default 32)]" << "\n"
<< "                [-w max_workspace_size_bytes (default 1 GiB)]" << "\n"
<< "                [-d model_data_type_bit_depth] (32 => float32, 16 => float16)" << "\n"
<< "                [-l] (list layers and their shapes)" << "\n"
<< "                [-g] (debug mode)" << "\n"
<< "                [-v] (increase verbosity)" << "\n"
<< "                [-q] (decrease verbosity)" << "\n"
<< "                [-V] (show version information)" << "\n"
<< "                [-h] (show help)" << endl;
}
void print_version() {
cout << "Parser built against:" << endl;
cout << "  ONNX IR version:  " << onnx_ir_version_string(::ONNX_NAMESPACE::IR_VERSION) << endl;
cout << "  TensorRT version: "
<< NV_TENSORRT_MAJOR << "."
<< NV_TENSORRT_MINOR << "."
<< NV_TENSORRT_PATCH << endl;
}
int main(int argc, char* argv[]) {
GOOGLE_PROTOBUF_VERIFY_VERSION;
std::string engine_filename;
std::string text_filename;
std::string full_text_filename;
size_t max_batch_size = 32;
size_t max_workspace_size = 1 << 30;
int model_dtype_nbits = 32;
int verbosity = (int)nvinfer1::ILogger::Severity::kWARNING;
bool print_layer_info = false;
bool debug_builder = false;
int arg = 0;
while( (arg = ::getopt(argc, argv, "o:b:w:t:T:d:lgvqVh")) != -1 ) {
switch (arg){
case 'o':
if( optarg ) { engine_filename = optarg; break; }
else { cerr << "ERROR: -o flag requires argument" << endl; return -1; }
case 't':
if( optarg ) { text_filename = optarg; break; }
else { cerr << "ERROR: -t flag requires argument" << endl; return -1; }
case 'T':
if( optarg ) { full_text_filename = optarg; break; }
else { cerr << "ERROR: -T flag requires argument" << endl; return -1; }
case 'b':
if( optarg ) { max_batch_size = atoll(optarg); break; }
else { cerr << "ERROR: -b flag requires argument" << endl; return -1; }
case 'w':
if( optarg ) { max_workspace_size = atoll(optarg); break; }
else { cerr << "ERROR: -w flag requires argument" << endl; return -1; }
case 'd':
if( optarg ) { model_dtype_nbits = atoi(optarg); break; }
else { cerr << "ERROR: -d flag requires argument" << endl; return -1; }
case 'l': print_layer_info = true; break;
case 'g': debug_builder = true; break;
case 'v': ++verbosity; break;
case 'q': --verbosity; break;
case 'V': print_version(); return 0;
case 'h': print_usage(); return 0;
}
}
int num_args = argc - optind;
if( num_args != 1 ) {
print_usage();
return -1;
}
std::string onnx_filename = argv[optind];
nvinfer1::DataType model_dtype;
if(      model_dtype_nbits == 32 ) { model_dtype = nvinfer1::DataType::kFLOAT; }
else if( model_dtype_nbits == 16 ) { model_dtype = nvinfer1::DataType::kHALF; }
//else if( model_dtype_nbits ==  8 ) { model_dtype = nvinfer1::DataType::kINT8; }
else {
cerr << "ERROR: Invalid model data type bit depth: " << model_dtype_nbits << endl;
return -2;
}
::ONNX_NAMESPACE::ModelProto onnx_model;
bool is_binary = ParseFromFile_WAR(&onnx_model, onnx_filename.c_str());
if( !is_binary && !ParseFromTextFile(&onnx_model, onnx_filename.c_str()) ) {
cerr << "Failed to parse ONNX model" << endl;
return -3;
}
if( verbosity >= (int)nvinfer1::ILogger::Severity::kWARNING ) {
int64_t opset_version = (onnx_model.opset_import().size() ?
onnx_model.opset_import(0).version() : 0);
cout << "----------------------------------------------------------------" << endl;
cout << "Input filename:   " << onnx_filename << endl;
cout << "ONNX IR version:  " << onnx_ir_version_string(onnx_model.ir_version()) << endl;
cout << "Opset version:    " << opset_version << endl;
cout << "Producer name:    " << onnx_model.producer_name() << endl;
cout << "Producer version: " << onnx_model.producer_version() << endl;
cout << "Domain:           " << onnx_model.domain() << endl;
cout << "Model version:    " << onnx_model.model_version() << endl;
cout << "Doc string:       " << onnx_model.doc_string() << endl;
cout << "----------------------------------------------------------------" << endl;
}
if( onnx_model.ir_version() > ::ONNX_NAMESPACE::IR_VERSION ) {
cerr << "WARNING: ONNX model has a newer ir_version ("
<< onnx_ir_version_string(onnx_model.ir_version())
<< ") than this parser was built against ("
<< onnx_ir_version_string(::ONNX_NAMESPACE::IR_VERSION) << ")." << endl;
}
if( !text_filename.empty() ) {
if( verbosity >= (int)nvinfer1::ILogger::Severity::kWARNING ) {
cout << "Writing ONNX model (without weights) as text to " << text_filename << endl;
}
std::ofstream onnx_text_file(text_filename.c_str());
std::string onnx_text = pretty_print_onnx_to_string(onnx_model);
onnx_text_file.write(onnx_text.c_str(), onnx_text.size());
}
if( !full_text_filename.empty() ) {
if( verbosity >= (int)nvinfer1::ILogger::Severity::kWARNING ) {
cout << "Writing ONNX model (with weights) as text to " << full_text_filename << endl;
}
std::string full_onnx_text;
google::protobuf::TextFormat::PrintToString(onnx_model, &full_onnx_text);
std::ofstream full_onnx_text_file(full_text_filename.c_str());
full_onnx_text_file.write(full_onnx_text.c_str(), full_onnx_text.size());
}
TRT_Logger trt_logger((nvinfer1::ILogger::Severity)verbosity);
auto trt_builder = infer_object(nvinfer1::createInferBuilder(trt_logger));
auto trt_network = infer_object(trt_builder->createNetwork());
auto trt_parser  = infer_object(nvonnxparser::createParser(
trt_network.get(), trt_logger));
// TODO: Fix this for the new API
//if( print_layer_info ) {
//  parser->setLayerInfoStream(&std::cout);
//}
(void)print_layer_info;
if( verbosity >= (int)nvinfer1::ILogger::Severity::kWARNING ) {
cout << "Parsing model" << endl;
}
{
std::ifstream onnx_file(onnx_filename.c_str(),
std::ios::binary | std::ios::ate);
std::streamsize file_size = onnx_file.tellg();
onnx_file.seekg(0, std::ios::beg);
std::vector<char> onnx_buf(file_size);
if( !onnx_file.read(onnx_buf.data(), onnx_buf.size()) ) {
cerr << "ERROR: Failed to read from file " << onnx_filename << endl;
return -4;
}
if( !trt_parser->parse(onnx_buf.data(), onnx_buf.size()) ) {
int nerror = trt_parser->getNbErrors();
for( int i=0; i<nerror; ++i ) {
nvonnxparser::IParserError const* error = trt_parser->getError(i);
if( error->node() != -1 ) {
::ONNX_NAMESPACE::NodeProto const& node =
onnx_model.graph().node(error->node());
cerr << "While parsing node number " << error->node()
<< " [" << node.op_type();
if( node.output().size() ) {
cerr << " -> \"" << node.output(0) << "\"";
}
cerr << "]:" << endl;
if( verbosity >= (int)nvinfer1::ILogger::Severity::kINFO ) {
cerr << "--- Begin node ---" << endl;
cerr << node << endl;
cerr << "--- End node ---" << endl;
}
}
cerr << "ERROR: "
<< error->file() << ":" << error->line()
<< " In function " << error->func() << ":\n"
<< "[" << static_cast<int>(error->code()) << "] " << error->desc()
<< endl;
}
return -5;
}
}
bool fp16 = trt_builder->platformHasFastFp16();
if( !engine_filename.empty() ) {
if( verbosity >= (int)nvinfer1::ILogger::Severity::kWARNING ) {
cout << "Building TensorRT engine, FP16 available:"<< fp16 << endl;
cout << "    Max batch size:     " << max_batch_size << endl;
cout << "    Max workspace size: " << max_workspace_size / (1024. * 1024) << " MiB" << endl;
}
trt_builder->setMaxBatchSize(max_batch_size);
trt_builder->setMaxWorkspaceSize(max_workspace_size);
if( fp16 && model_dtype == nvinfer1::DataType::kHALF) {
trt_builder->setHalf2Mode(true);
} else if( model_dtype == nvinfer1::DataType::kINT8 ) {
// TODO: Int8 support
//trt_builder->setInt8Mode(true);
cerr << "ERROR: Int8 mode not yet supported" << endl;
return -5;
}
trt_builder->setDebugSync(debug_builder);
auto trt_engine = infer_object(trt_builder->buildCudaEngine(*trt_network.get()));
if( verbosity >= (int)nvinfer1::ILogger::Severity::kWARNING ) {
cout << "Writing TensorRT engine to " << engine_filename << endl;
}
auto engine_plan = infer_object(trt_engine->serialize());
std::ofstream engine_file(engine_filename.c_str());
engine_file.write((char*)engine_plan->data(), engine_plan->size());
engine_file.close();
}
if( verbosity >= (int)nvinfer1::ILogger::Severity::kWARNING ) {
cout << "All done" << endl;
}
return 0;
}
再通过TensorRT进行序列化：
./onnx2trt /home/panwangpan/SRGAN/SRGAN_Resnet.onnx -o SRGAN.trt -b 1 -l
----------------------------------------------------------------
Input filename:   /home/panwangpan/SRGAN/SRGAN_Resnet.onnx
ONNX IR version:  0.0.2
Opset version:    2
Producer name:    pytorch
Producer version: 0.3
Domain:
Model version:    0
Doc string:
----------------------------------------------------------------
![](https://img-blog.csdn.net/2018081714592192?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
比如这是我要加速的模型，有卷积层、concat（Dense block）、swish激活函数等，需要用到953M左右的workspace等。
![](https://img-blog.csdn.net/20180817150233186?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以去除一些dead-layer，对卷积进行合并，对concat层消除。
![](https://img-blog.csdn.net/20180817150501589?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Writing ONNX model (without weights) as text to SRGAN.txt
![](https://img-blog.csdn.net/20180815224500329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180815224516368?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
得到SRGAN.txt 和SRGAN.trt, 再通过反序列化得到kernel，不需要部署深度学习框架，即可进行神经网络推理与加速。



