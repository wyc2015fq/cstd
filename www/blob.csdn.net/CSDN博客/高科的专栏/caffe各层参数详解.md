
# caffe各层参数详解 - 高科的专栏 - CSDN博客

2018年10月08日 16:13:18[高科](https://me.csdn.net/pbymw8iwm)阅读数：471


在prototxt文件中，层都是用layer{}的结构表示，而里面包含的层的参数可以在caffe.proto文件中找到，比如说Data类型的结构由`message DataParameter`所定义，`Convolution`类型的结构由`message ConvolutionParameter`所定义。
具体说明下：
name表示该层的名称
type表示该层的类型，如Data或者Convolution类型
top表示该层后接的Blob结构的名称
bottom表示该层前接的Blob数据结构的名称
*_param表示该层的参数，比如对于某卷积层conv1来说，convolution_param中num_output表示输出通道数
pad表示卷积pad
kernel_size表示卷积核的大小
这里我推荐一个资源：[caffe网络模型各个层详解](https://download.csdn.net/download/pbymw8iwm/10706680)
[https://download.csdn.net/download/pbymw8iwm/10706680](https://download.csdn.net/download/pbymw8iwm/10706680)
数据层
`{
  name: "data"
  type: "Data"
  top: "data"
  top: "label"
  transparam {
    scale: 0.003920625        // 归一化：1/256
  }
  include {
    phase: TRAIN/TEST         // 训练阶段
  }
  data_param {
    source: "examples/mnist/train_lmdb"
    batch_size: 64           // 批量处理数
    backend: LMDB            // 数据格式，LMDB/HDF5等
  }
}`卷积层
卷积层是CNN网络的核心，一般用来提取图像特征。
`layer {
  name: "conv"
  type: "Convolution"
  bottom: "data"
  top: "conv"
  param {
    lr_mult: 1             // weight的学习率
    decay_mult: 1          // 权重衰减
  }
  param {
    lr_mult: 2            // bias的学习率
    decay_mult: 0
  }
  convolution_param {
    num_output: 20        // 通道数
    kernel_size: 5        // 卷积核大小
    stride: 1             // 步长
    weight_filter {       // 权重初始化，xavier/guassion/constant
      type: "xavier"      // 一种均匀化分布
    }
    bias_filter {         // 偏置初始化
      type: "constant"    // 值全为0
    }
  }
}`
Pool层
池化层，用来减小训练参数规模。
`layer {
  name: "pool"
  type: "Pooling"
  bottom: "conv"
  top: "pool"
  pooling_param {
    pool: MAX             // 最大化，还有均值化/最小化
    kernel_size: 2        // 卷积核大小，缩放效果
    stride: 2             // 步长
  }
}`全连接层
类似卷积层
`layer {
  name: "ip"
  type: "InnerProduct"
  bottom: "pool"
  top: "ip"
  param {
    lr_mult: 1
  }
  param {
    lr_mult: 2
  }
  inner_product_param {
    num_output: 10
    weight_filter {
      type: "xavier"
    }
    bias_filter {
      type: "constant"
    }
  }
}`Relu层
激活函数
`layer {
  name: "relu"
  type: "Relu"
  bottom: "ip"
  top: "ip"
}`Accuracy层
一般在测试阶段看到训练精度。
`layer {
  name: "accuracy"
  type: "Accuracy"
  bottom: "ip"
  bottom: "label"
  top: "accuracy"
  include {
    phase: TEST
  }
}`Softmax层
损失函数，常用于多个分类
`layer {
  name: "loss"
  type: "SoftmaxWithLoss"
  bottom: "ip"
  bottom: "label"
  top: "loss"
}`BatchNormal层
批量正则化，不依赖于初始值，加快训练速度。
`layer {
  name: "batchnorm"
  type: "BatchNorm"
  bottom: "conv"
  top: "conv"
  batch_norm_param {
    use_global_stats: true
  }
  include {
    phase: TRAIN/TEST
  }
}`Scale层
BN层后面一般都要带Scale层，和batchnorm计算公式中的参数有关。
`layer {
  name: "scale"
  type: "Scale"
  bottom: "conv"
  top: "conv"
  scale_param {
    bias_term: true
  }
}`Dropout层
Dropout是一个防止过拟合的trick。可以随机让网络某些隐含层节点的权重不工作。
`layer {
  name: "drop7"
  type: "Dropout"
  bottom: "fc7-conv"
  top: "fc7-conv"
  dropout_param {
    dropout_ratio: 0.5
  }
}`

