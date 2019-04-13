
# OpenCV3.3 DNN简介 - 郭云飞的专栏 - CSDN博客


2017年10月26日 18:30:46[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：3894


如今，在机器视觉领域，深度学习已经是最流行且增长最快的一个方向了。OpenCV自3.1版本其就在contrib中加入了DNN模块。到3.3版本时，将DNN模块由contrib提升到了正式代码块中。在住仓库中的位置为：[https://github.com/opencv/opencv/tree/master/modules/dnn](https://github.com/opencv/opencv/tree/master/modules/dnn)。同时相较于3.1版本，3.3版本对DNN做了很大改进！
该DNN模块除了libprotobuf，不依赖任何第三方库；并且libprotobuf已经包含在了OpenCV的ThirdParty，安装OpenCV时会一并安装，很方便。
目前，该DNN模块支持加载训练好的模型（即：这些模型需要实现在Caffe、TensorFlow、Torch/PyTorch等深度学习框架中提取训练好），并执行前向传播过程（即预测）。
**支持的深度学习库：**
Caffe 1
TensorFlow
Torch/PyTorch
**主要的层及函数：**
AbsVal（caffe/layers/absval_layer.hpp这一层比较简单：主要就是求绝对值）
AveragePooling（平均值池化）
BatchNormalization（就像激活函数层、卷积层、全连接层、池化层一样，BN也属于网络的一层；在网络中间层数据做一个归一化处理）
Concatenation（Caffe中通过Concatenation层，可以把多个的blobs链接成一个blob）
Convolution (including dilated convolution)
Crop
Deconvolution, a.k.a. transposed convolution or full convolution
DetectionOutput (SSD-specific layer)
Dropout
Eltwise (+, *, max)（caffe提供的按元素操作层。它支持3种基本操作：PROD按元素乘积；SUM按元素求和；MAX保存最大元素 ）
Flatten（Caffe中Flattening层是把一个输入的大小为n * c * h * w变成一个简单的向量，其大小为 n * (c*h*w) * 1 * 1）
FullyConnected
LRN（Local Response Normalization，caffe中LRN是对一个局部的输入区域进行的归一化）
LSTM
MaxPooling（最大池化）
MaxUnpooling
MVN
NormalizeBBox (SSD-specific layer)
Padding
Permute
Power
PReLU (including ChannelPReLU with channel-specific slopes)
PriorBox (SSD-specific layer)
ReLU
RNN
Scale
Shift
Sigmoid
Slice（Caffe中Slice layer 的作用是将bottom按照需要分解成多个tops）
Softmax（激活函数）
Split（Caffe中Splitting层可以把一个输入blob分离成多个输出blobs）
TanH（激活函数）
**一些已经经过测试的网络：**
AlexNet
GoogLeNet v1 (also referred to as Inception-5h)
ResNet-34/50/...
SqueezeNet v1.1
VGG-based FCN (semantical segmentation network)
ENet (lightweight semantical segmentation network)
VGG-based SSD (object detection network)
MobileNet-based SSD (light-weight object detection network
**例程：**
[https://github.com/opencv/opencv/tree/master/samples/dnn](https://github.com/opencv/opencv/tree/master/samples/dnn)










