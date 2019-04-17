# Caffe学习（十）protobuf及caffe.proto解析 - BryantLJ学习的地方 - CSDN博客





2016年12月30日 17:01:18[遍地流金](https://me.csdn.net/u012177034)阅读数：6183








# 使用protobuf的原由

一个好的软件框架应该要有明确的输入和输出，对于CNN网络而言，其主要有两部分组成：网络具体结构和网络的具体优化算法及参数。对于框架的使用者而言，用户只需输入两个描述文件即可得到对该网络的优化结果，这无疑是非常方便的。

caffe框架选择使用谷歌的开源protobuf工具对这两部分进行描述，解析和存储，这一部分为caffe的实现节省了大量的代码。

如前面讲述的目标检测demo，py-faster-rcnn，其主要分为训练和测试两个过程，两个过程的核心文件都是prototxt格式的文本文件。 

如训练过程 

输入： 

（1）slover.prototxt。描述网络训练时的各种参数文件，如训练的策略，学习率的变化率，模型保存的频率等参数 

（2）train.prototxt。描述训练网络的网络结构文件。 

（3）test.prototxt。描述测试网络的网络结构文件。 

输出： 

VGG16.caffemodel：保存的训练好的网络参数文件。
# protobuf的使用流程

protobuf工具主要是数据序列化存储和解析。在实际使用的时候主要是作为一个代码自动生成工具来使用，通过生成对所定义的数据结构的标准读写代码，用户可以通过标准的读写接口从文件中进行数据的读取，解析和存储。 

目前proto支持C++，python，java等语言，这里主要演示caffe中使用的C++调用。 

主要使用过程为： 

（1）编写XXX.proto文件。该文件里主要定义了各种数据结构及对应的数据类型，如int，string等。 

（2）使用protoc对XXX.proto文件进行编译，生成对应的数据结构文件的读取和写入程序，程序接口都是标准化的。生成的文件一般名为XXX.pb.cc和XXX.pb.h。 

（3）在新程序中使用XXX.pb.c和XXX.pb.h提供的代码。
# 简易caffe.proto编写解析示例

为了后面更加清楚的理解protobuf工具，这里一个简单的caffe.proto为例进行solver.prototxt和train.prototxt的解析

## caffe.proto文件编写：

```
syntax = "proto2";

package caffe;//c++ namespace

message NetParameter {
  optional string name = 1; // consider giving the network a name
  repeated LayerParameter layer = 2;  // ID 100 so layers are printed last.
}

message SolverParameter {
  optional string train_net = 1;
  optional float base_lr = 2;
  optional string lr_policy = 3;
  optional NetParameter net_param = 4;
}

message ParamSpec {
  optional string name = 1;
  optional float lr_mult = 3 [default = 1.0];
  optional float decay_mult = 4 [default = 1.0];
}
// LayerParameter next available layer-specific ID: 147 (last added: recurrent_param)
message LayerParameter {
  optional string name = 1; // the layer name
  optional string type = 2; // the layer type
  repeated string bottom = 3; // the name of each bottom blob
  repeated string top = 4; // the name of each top blob
  repeated ParamSpec param = 6;
  // Layer type-specific parameters.
  optional ConvolutionParameter convolution_param = 106;
  optional PythonParameter python_param = 130;
}

message ConvolutionParameter {
  optional uint32 num_output = 1; // The number of outputs for the layer

  // Pad, kernel size, and stride are all given as a single value for equal
  // dimensions in all spatial dimensions, or once per spatial dimension.
  repeated uint32 pad = 3; // The padding size; defaults to 0
  repeated uint32 kernel_size = 4; // The kernel size
  repeated uint32 stride = 6; // The stride; defaults to 1
}

message PythonParameter {
  optional string module = 1;
  optional string layer = 2;
  // This value is set to the attribute `param_str` of the `PythonLayer` object
  // in Python before calling the `setup()` method. This could be a number,
  // string, dictionary in Python dict format, JSON, etc. You may parse this
  // string in `setup` method and use it in `forward` and `backward`.
  optional string param_str = 3 [default = ''];
}
```

## 编译生成caffe.pb.cc与caffe.pb.h文件
`protoc caffe.proto --cpp_out=.//在当前目录生成cpp文件及头文件`
## 编写测试文件main.cpp

```cpp
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#include "caffe.pb.h"
using namespace caffe;
using namespace std;

using google::protobuf::io::FileInputStream;
using google::protobuf::Message;
bool ReadProtoFromTextFile(const char* filename, Message* proto) {
  int fd = open(filename, O_RDONLY);
  FileInputStream* input = new FileInputStream(fd);
  bool success = google::protobuf::TextFormat::Parse(input, proto);
  delete input;
  close(fd);
  return success;
}


int main()
{
    SolverParameter SGD;

    if(!ReadProtoFromTextFile("solver.prototxt", &SGD))
    {
       cout<<"error opening file"<<endl; 
       return -1;
    }

    cout<<"hello,world"<<endl;
    cout<<SGD.train_net()<<endl;
    cout<<SGD.base_lr()<<endl;
    cout<<SGD.lr_policy()<<endl;

    NetParameter VGG16;
    if(!ReadProtoFromTextFile("train.prototxt", &VGG16))
    {
       cout<<"error opening file"<<endl; 
       return -1;
    }
    cout<<VGG16.name()<<endl;
    return 0;
}
```

## 编写solver与train网络描述文件

solver.prototxt内容

```
train_net: "/home/bryant/cuda-test/train.prototxt"
base_lr: 0.001
lr_policy: "step"
```

train.prototxt内容：

```
name: "VGG_ILSVRC_16_layers"
layer {
  name: 'input-data'
  type: 'Python'
  top: 'data'
  top: 'im_info'
  top: 'gt_boxes'
  python_param {
    module: 'roi_data_layer.layer'
    layer: 'RoIDataLayer'
    param_str: "'num_classes': 2"
  }
}

layer {
  name: "conv1_1"
  type: "Convolution"
  bottom: "data"
  top: "conv1_1"
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  convolution_param {
    num_output: 64
    pad: 1
    kernel_size: 3
  }
}
```

## 编译链接，生成main
`g++ caffe.pb.cc main.cpp -o main -lprotobuf`
## 运行结果

```
bryant@bryant:~/cuda-test/src$ ./main 
hello,world
/home/bryant/cuda-test/train.prototxt
0.001
step
VGG_ILSVRC_16_layers
bryant@bryant:~/cuda-test/src$
```





