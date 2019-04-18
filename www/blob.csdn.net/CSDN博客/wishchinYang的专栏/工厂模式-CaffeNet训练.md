# 工厂模式-CaffeNet训练 - wishchinYang的专栏 - CSDN博客
2015年04月06日 08:59:12[wishchin](https://me.csdn.net/wishchin)阅读数：1564
参考链接：[http://blog.csdn.net/lingerlanlan/article/details/32329761](http://blog.csdn.net/lingerlanlan/article/details/32329761)
RNN神经网络：http://nbviewer.ipython.org/github/BVLC/caffe/blob/master/examples/detection.ipynb
官方链接：[http://nbviewer.ipython.org/github/BVLC/caffe/blob/master/examples/classification.ipynb](http://nbviewer.ipython.org/github/BVLC/caffe/blob/master/examples/classification.ipynb)
参考链接：[http://suanfazu.com/t/caffe-shen-du-xue-xi-kuang-jia-shang-shou-jiao-cheng/281/3](http://suanfazu.com/t/caffe-shen-du-xue-xi-kuang-jia-shang-shou-jiao-cheng/281/3)
模型定义中有一点比较容易被误解，信号在有向图中是自下而上流动的，并不是自上而下。
层的结构定义如下：
```
1 
name:层名称 2 
type:层类型 3 
top:出口 4 bottom:入口
```
Each layer type defines three critical computations: *setup*, *forward*, and*backward*.
- Setup: initialize the layer and its connections once at model initialization.
- Forward: given input from bottom compute the output and send to the top.
- Backward: given the gradient w.r.t. the top output compute the gradient w.r.t. to the input and send to the bottom. A layer with parameters computes the gradient w.r.t. to its parameters and stores it internally.
/home/wishchin/caffe-master/examples/hdf5_classification/train_val2.prototxt
```cpp
name: "LogisticRegressionNet"
layer {
  name: "data"
  type: "HDF5Data"
  top: "data"
  top: "label"
  include {
    phase: TRAIN
  }
  hdf5_data_param {
    source: "hdf5_classification/data/train.txt"
    batch_size: 10
  }
}
layer {
  name: "data"
  type: "HDF5Data"
  top: "data"
  top: "label"
  include {
    phase: TEST
  }
  hdf5_data_param {
    source: "hdf5_classification/data/test.txt"
    batch_size: 10
  }
}
layer {
  name: "fc1"
  type: "InnerProduct"
  bottom: "data"
  top: "fc1"
  param {
    lr_mult: 1
    decay_mult: 1
  }
  param {
    lr_mult: 2
    decay_mult: 0
  }
  inner_product_param {
    num_output: 40
    weight_filler {
      type: "gaussian"
      std: 0.01
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "relu1"
  type: "ReLU"
  bottom: "fc1"
  top: "fc1"
}
layer {
  name: "fc2"
  type: "InnerProduct"
  bottom: "fc1"
  top: "fc2"
  param {
    lr_mult: 1
    decay_mult: 1
  }
  param {
    lr_mult: 2
    decay_mult: 0
  }
  inner_product_param {
    num_output: 2
    weight_filler {
      type: "gaussian"
      std: 0.01
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "loss"
  type: "SoftmaxWithLoss"
  bottom: "fc2"
  bottom: "label"
  top: "loss"
}
layer {
  name: "accuracy"
  type: "Accuracy"
  bottom: "fc2"
  bottom: "label"
  top: "accuracy"
  include {
    phase: TEST
  }
}
```
**关于参数与结果的关系**：多次训练效果一直在0.7，后来改动了全链接层的初始化参数。高斯分布的标准差由0.001改为0.0001，就是调小了。
 我的结果有点相似。
