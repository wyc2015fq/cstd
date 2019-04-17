# 【用Python学习Caffe】4. 设计自己的网络结构 - tostq的专栏 - CSDN博客





2017年06月22日 22:15:50[tostq](https://me.csdn.net/tostq)阅读数：3021
所属专栏：[用Python学习Caffe](https://blog.csdn.net/column/details/16089.html)









## 4. 设计自己的网络结构

通过前文的例子，我们都知道了Caffe的网络都是一个prototxt的网络结构配置文件定义的，该文件可以用文本工具打开，打开后，我们可以看到如下结构：
    layer {
      name: "data"
      type: "Data"
      top: "data"
      top: "label"
      transform_param {
        scale: 0.00390625
      }
      data_param {
        source: "../../data/mnist/mnist_train_lmdb"
        batch_size: 64
        backend: LMDB
      }
    }
    layer {
      name: "conv1"
      type: "Convolution"
      bottom: "data"
      top: "conv1"
      param {
        lr_mult: 1
        decay_mult: 1
      }
      param {
        lr_mult: 2
        decay_mult: 0
      }
      convolution_param {
        num_output: 20
        pad: 0
        kernel_size: 5
        stride: 1
        weight_filler {
          type: "xavier"
        }
        bias_filler {
          type: "constant"
          value: 0
        }
      }
    }
    layer {
      name: "pool1"
      type: "Pooling"
      bottom: "conv1"
      top: "pool1"
      pooling_param {
        pool: MAX
        kernel_size: 2
        stride: 2
      }
    }
    ...

在prototxt文件中，层都是用layer{}的结构表示，而里面包含的层的参数可以在caffe.proto文件中找到，比如说Data类型的结构由`message DataParameter`所定义，`Convolution`类型的结构由`message ConvolutionParameter`所定义。 

具体说明下：
- `name`表示该层的名称
- `type`表示该层的类型，如`Data`或者`Convolution`类型
- `top`表示该层后接的`Blob`结构的名称
- `bottom`表示该层前接的`Blob`数据结构的名称
- `*_param`表示该层的参数，比如对于某卷积层`conv1`来说，`convolution_param`中 
- `num_output`表示输出通道数
- `pad`表示卷积`pad`
- `kernel_size`表示卷积核的大小
- 关于卷积操作的具体解释可以[参考`Theano`的对卷积的介绍](http://deeplearning.net/software/theano/tutorial/conv_arithmetic.html#convolution-arithmetic)。


对于如何使用层结构，一方面我们可以从`model_libs.py`及`caffenet.py`等文件找到如何使用层结构的例子。另外可能参考[这个教程](https://github.com/abhi-kumar/Caffe-Python-Basic-Tutorial)，里面给出caffe的所有层的Python函数使用例子。 

另一方面我们可以参照[caffe的C++类定义](http://caffe.berkeleyvision.org/doxygen/annotated.html)。在我们知道各层的描述后，可以将这些层结构拼接成我们自己的网络，手动在文本工具中直接编写prototxt文件。

但对于一些非常复杂的网络，很多情况下可能有成百上千的层。这样的情况下，我们能难人工一层接一层的编写prototxt文件，而且还很容易出错，因此可以通过python来直接自动生成结构文件。

在caffe目录下的example/pycaffe文件夹的caffenet.py给出了如何直接生成一个多层线性分类器的网络配置文件prototxt文件，而本节还将介绍一个直接生成mnist LeNet5网络的训练、测试及实施的三个网络配置文件prototxt。而我们将以后章节中介绍通过三个文件完成LeNet5网络的训练及测试任务。

> 
Tips：可以将一些常用的网络结构打包成基础体，比如在model_lib.py文件中将VGG网络、AlexNet等常用网络打包成基础网络。另外训练、测试及实施的三个网络的中间结构是一致的，只是输入层和最终输出层是不一样的，因此将可以中间相同层都用一个固定函数生成。


生成网络配置主要分为如下几个部分：

### 4.1 配置基础网络结构

这里生成训练、测试及实施的三个网络中都相同的网络中间部件
    # 此函数生成LeNet5的主体结构
    def lenet5_body(net, from_layer):
        # 网络参数
        kwargs = {
            # param定义学习率，这里是指基础学习率step的情况，lt_mult乘以基础学习率为实际学习率，为0表示权重不更新，decay_mult同权重衰减相关
            'param': [dict(lr_mult=1, decay_mult=1), dict(lr_mult=2, decay_mult=0)],
            'weight_filler': dict(type='xavier'), # 权重初始化模式
            'bias_filler': dict(type='constant', value=0)} # 权重偏差初始化模式

        # 判断是否存在from_layer层
        assert from_layer in net.keys()
        # conv1
        net.conv1 = L.Convolution(net[from_layer], kernel_size=5, stride=1, num_output=20, pad=0, **kwargs)
        net.pool1 = L.Pooling(net.conv1, pool=P.Pooling.MAX, kernel_size=2, stride=2)
        net.conv2 = L.Convolution(net.pool1, kernel_size=5, stride=1, num_output=50, pad=0, **kwargs)
        net.pool2 = L.Pooling(net.conv2, pool=P.Pooling.MAX, kernel_size=2, stride=2)
        net.ip1 = L.InnerProduct(net.pool2, num_output=500, **kwargs)
        net.relu1 = L.ReLU(net.ip1, in_place=True)
        net.ip2 = L.InnerProduct(net.relu1, name='ip2', num_output=10, **kwargs)

### 4.2 构建整体网络

训练、测试及实施的三个网络的结构有些许不同，主要集中在输入层和输出层。
    # 训练网络
    train_net = caffe.NetSpec()  # 基础网络
    # 带标签的数据输入层
    train_net.data, train_net.label = L.Data(source=train_data,backend=P.Data.LMDB, batch_size=64,ntop=2,transform_param=dict(scale=0.00390625))
    # 生成LeNet5的主体结构
    lenet5_body(train_net, 'data')
    # 生成误差损失层
    train_net.loss = L.SoftmaxWithLoss(train_net.ip2, train_net.label)
    # 测试网络
    test_net = caffe.NetSpec()  # 基础网络
    # 带标签的数据输入层
    test_net.data, test_net.label = L.Data(source=test_data, batch_size=100, backend=P.Data.LMDB, ntop=2,transform_param=dict(scale=0.00390625))
    # 生成LeNet5的主体结构
    lenet5_body(test_net, 'data')
    # 生成误差损失层
    test_net.loss = L.SoftmaxWithLoss(test_net.ip2, test_net.label)
    # 添加一个精确层
    test_net.accuracy = L.Accuracy(test_net.ip2, test_net.label)
    # 实施网络
    deploy_net = caffe.NetSpec()  # 基础网络
    # 带标签的数据输入层
    deploy_net.data = L.Input(input_param=dict(shape=dict(dim=[64,1,28,28])))
    # 生成LeNet5的主体结构
    lenet5_body(deploy_net, 'data')
    deploy_net.prob = L.Softmax(deploy_net.ip2)

### 4.3 prototxt文件的保存

通过to_proto将网络保存成prototxt文件
    # 保存训练文件
    with open(model_root+'train.prototxt', 'w') as f:
        print('name: "LenNet5_train"', file=f)
        print(train_net.to_proto(), file=f)
    with open(model_root+'test.prototxt', 'w') as f:
        print('name: "LenNet5_test"', file=f)
        print(test_net.to_proto(), file=f)
    with open(model_root+'deploy.prototxt', 'w') as f:
        print('name: "LenNet5_test"', file=f)
        print(deploy_net.to_proto(), file=f)

### 4.4 具体代码下载

GitHub仓库[Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)中的[prototxt.py](https://github.com/tostq/Caffe-Python-Tutorial/blob/master/prototxt.py)

项目地址：[https://github.com/tostq/Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)



