# Caffe源码（三）：layer 分析

2015年08月11日 13:36:32 [seven_first](https://me.csdn.net/seven_first) 阅读数：10408



# 目录



- [目录](https://blog.csdn.net/seven_first/article/details/47418887#%E7%9B%AE%E5%BD%95)
- 简单介绍
  - [Message LayerParameter](https://blog.csdn.net/seven_first/article/details/47418887#message-layerparameter)
  - Layer 类
    - [构造函数](https://blog.csdn.net/seven_first/article/details/47418887#%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
    - [LayerSetUp 函数](https://blog.csdn.net/seven_first/article/details/47418887#layersetup-%E5%87%BD%E6%95%B0)
    - [Forword 函数](https://blog.csdn.net/seven_first/article/details/47418887#forword-%E5%87%BD%E6%95%B0)
    - [Backword 函数](https://blog.csdn.net/seven_first/article/details/47418887#backword-%E5%87%BD%E6%95%B0)
    - [blobslayer_param函数](https://blog.csdn.net/seven_first/article/details/47418887#blobslayerparam%E5%87%BD%E6%95%B0)
    - [ToProto 函数](https://blog.csdn.net/seven_first/article/details/47418887#toproto-%E5%87%BD%E6%95%B0)
    - [loss set_loss函数](https://blog.csdn.net/seven_first/article/details/47418887#loss-setloss%E5%87%BD%E6%95%B0)
    - [一些返回特定参数的函数](https://blog.csdn.net/seven_first/article/details/47418887#%E4%B8%80%E4%BA%9B%E8%BF%94%E5%9B%9E%E7%89%B9%E5%AE%9A%E5%8F%82%E6%95%B0%E7%9A%84%E5%87%BD%E6%95%B0)
    - [set_param_propagate_downparam_propagate_down 函数](https://blog.csdn.net/seven_first/article/details/47418887#setparampropagatedownparampropagatedown-%E5%87%BD%E6%95%B0)
    - [CheckBlobCounts 函数](https://blog.csdn.net/seven_first/article/details/47418887#checkblobcounts-%E5%87%BD%E6%95%B0)
    - [SetLossWeights 函数](https://blog.csdn.net/seven_first/article/details/47418887#setlossweights-%E5%87%BD%E6%95%B0)



# 简单介绍

Caffe 中的 layer 是组成 net 的 component . laye 必须执行向前传播函数，即输入 Blob bottom，产生一个输出 Blob top。 绝大多数layer 要执行反向传播函数，即计算关于输入 Blob 的 error 梯度 diff .

## Message LayerParameter

首先让我们回顾一下定义在caffe.proto中的Message LayerParameter：

```
message LayerParameter {
  optional string name = 1; // the layer name
  optional string type = 2; // the layer type
  repeated string bottom = 3; // the name of each bottom blob
  repeated string top = 4; // the name of each top blob
  optional Phase phase = 10; // The train / test phase for computation.

  // The amount of weight to assign each top blob in the objective.
  // Each layer assigns a default value, usually of either 0 or 1,
  // to each top blob.
  repeated float loss_weight = 5;

  // Specifies training parameters (multipliers on global learning constants,
  // and the name and other settings used for weight sharing).
  repeated ParamSpec param = 6;

  // The blobs containing the numeric parameters of the layer.
  // See detials in message BlobProto 
  repeated BlobProto blobs = 7;

  // Rules controlling whether and when a layer is included in the network,
  // based on the current NetState.  You may specify a non-zero number of rules
  // to include OR exclude, but not both.  If no include or exclude rules are
  // specified, the layer is always included.  If the current NetState meets
  // ANY (i.e., one or more) of the specified rules, the layer is
  // included/excluded.
  repeated NetStateRule include = 8;
  repeated NetStateRule exclude = 9;

  // Parameters for data pre-processing.
  // See detials in message TransformationParameter 
  optional TransformationParameter transform_param = 100;

  // Parameters shared by loss layers.
  // See detials in message LossParameter 
  optional LossParameter loss_param = 101;

  // Layer type-specific parameters.
  // Note: certain layers may have more than one computational engine
  // for their implementation. These layers include an Engine type and
  // engine parameter for selecting the implementation.
  // The default for the engine is set by the ENGINE switch at compile-time.
  optional AccuracyParameter accuracy_param = 102;
  optional ArgMaxParameter argmax_param = 103;
  optional ConcatParameter concat_param = 104;
  optional ContrastiveLossParameter contrastive_loss_param = 105;
  optional ConvolutionParameter convolution_param = 106;
  optional DataParameter data_param = 107;
  optional DropoutParameter dropout_param = 108;
  optional DummyDataParameter dummy_data_param = 109;
  optional EltwiseParameter eltwise_param = 110;
  optional ExpParameter exp_param = 111;
  optional HDF5DataParameter hdf5_data_param = 112;
  optional HDF5OutputParameter hdf5_output_param = 113;
  optional HingeLossParameter hinge_loss_param = 114;
  optional ImageDataParameter image_data_param = 115;
  optional InfogainLossParameter infogain_loss_param = 116;
  optional InnerProductParameter inner_product_param = 117;
  optional LRNParameter lrn_param = 118;
  optional MemoryDataParameter memory_data_param = 119;
  optional MVNParameter mvn_param = 120;
  optional PoolingParameter pooling_param = 121;
  optional PowerParameter power_param = 122;
  optional PReLUParameter prelu_param = 131;
  optional PythonParameter python_param = 130;
  optional ReLUParameter relu_param = 123;
  optional SigmoidParameter sigmoid_param = 124;
  optional SoftmaxParameter softmax_param = 125;
  optional SliceParameter slice_param = 126;
  optional TanHParameter tanh_param = 127;
  optional ThresholdParameter threshold_param = 128;
  optional WindowDataParameter window_data_param = 129;
}12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364656667686970717273
```

## Layer 类

### 构造函数：

```
explicit Layer(const LayerParameter& param)
    : layer_param_(param) {
      // Set phase and copy blobs (if there are any).
      phase_ = param.phase();
      if (layer_param_.blobs_size() > 0) {
        blobs_.resize(layer_param_.blobs_size());
        for (int i = 0; i < layer_param_.blobs_size(); ++i) {
          blobs_[i].reset(new Blob<Dtype>());
          blobs_[i]->FromProto(layer_param_.blobs(i));
        }
      }
    }//用protobuf 传入的参数对blobs_ 做初始化，blobs_ 是一个vector 存放指向Blob类的智能指针。123456789101112
```

### LayerSetUp 函数：

```
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {}
123
```

用于各个layer的初始化，不同的layer定义不同，在这里只做了声明，在具体的layer中定义。

### Forword 函数：

```
 template <typename Dtype>
inline Dtype Layer<Dtype>::Forward(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  Dtype loss = 0;
  switch (Caffe::mode()) {
  case Caffe::CPU:
    Forward_cpu(bottom, top);//对输入的blob bottom 进行前向传播，计算输出blob top 或者loss，具体layer具体定义。
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      if (!this->loss(top_id)) { continue; }
      const int count = top[top_id]->count();
      const Dtype* data = top[top_id]->cpu_data();
      const Dtype* loss_weights = top[top_id]->cpu_diff();
      loss += caffe_cpu_dot(count, data, loss_weights);
    }
    break;
  case Caffe::GPU:
    Forward_gpu(bottom, top);
#ifndef CPU_ONLY
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      if (!this->loss(top_id)) { continue; }
      const int count = top[top_id]->count();
      const Dtype* data = top[top_id]->gpu_data();
      const Dtype* loss_weights = top[top_id]->gpu_diff();
      Dtype blob_loss = 0;
      caffe_gpu_dot(count, data, loss_weights, &blob_loss);
      loss += blob_loss;
    }
#endif
    break;
  default:
    LOG(FATAL) << "Unknown caffe mode.";
  }
  return loss;
}
1234567891011121314151617181920212223242526272829303132333435
```

对输入的blob bottom 进行前向传播，计算输出blob top 或者loss，具体layer具体定义。

### Backword 函数：

```
 template <typename Dtype>
inline void Layer<Dtype>::Backward(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob<Dtype>*>& bottom) {
  switch (Caffe::mode()) {
  case Caffe::CPU:
    Backward_cpu(top, propagate_down, bottom);
    //根据blob top 的error 梯度（diff）计算bottom 的 error 梯度。 propagate_down 是长度 和bottom 相同的vector ，用于控制是否需要对对应的bottom 元素传播梯度。具体layer具体定义。
    break;
  case Caffe::GPU:
    Backward_gpu(top, propagate_down, bottom);
    break;
  default:
    LOG(FATAL) << "Unknown caffe mode.";
  }
}

// Serialize LayerParameter to protocol buffer
template <typename Dtype>
void Layer<Dtype>::ToProto(LayerParameter* param, bool write_diff) {
  param->Clear();
  param->CopyFrom(layer_param_);
  param->clear_blobs();
  for (int i = 0; i < blobs_.size(); ++i) {
    blobs_[i]->ToProto(param->add_blobs(), write_diff);
  }
}123456789101112131415161718192021222324252627
```

### blobs，layer_param函数：

```
vector<shared_ptr<Blob<Dtype> > >& blobs() {
    return blobs_;
  }//返回vector  blobs_
 const LayerParameter& layer_param() const { 
    return layer_param_;
  }//返回layer parameter123456
```

### ToProto 函数：

```
virtual void ToProto(LayerParameter* param, bool write_diff = false);1
```

将layer plarameter 写入protobuf

### loss ,set_loss函数：

```
inline Dtype loss(const int top_index) const {
    return (loss_.size() > top_index) ? loss_[top_index] : Dtype(0);
  }
inline void set_loss(const int top_index, const Dtype value) {
    if (loss_.size() <= top_index) {
      loss_.resize(top_index + 1, Dtype(0));
    }
    loss_[top_index] = value;
  }123456789
```

返回 ,设置一个blob top 在给定 index 的 loss

### 一些返回特定参数的函数：

```
 virtual inline const char* type() const { return ""; }//返回layer类型

 virtual inline int ExactNumBottomBlobs() const { return -1; }//返回该layer 所需 bottom 的数量，如果没有要求返回-1

 virtual inline int ExactNumTopBlobs() const { return -1; }//返回该layer 所需 top 的数量  
123456
```

### set_param_propagate_down，param_propagate_down 函数：

```
  inline bool param_propagate_down(const int param_id) {
    return (param_propagate_down_.size() > param_id) ?
        param_propagate_down_[param_id] : false;
  }
  inline void set_param_propagate_down(const int param_id, const bool value) {
    if (param_propagate_down_.size() <= param_id) {
      param_propagate_down_.resize(param_id + 1, true);
    }
    param_propagate_down_[param_id] = value;
  }12345678910
```

设置对于那些bottom 需要反向传播。

### CheckBlobCounts 函数

```
  virtual void CheckBlobCounts(const vector<Blob<Dtype>*>& bottom,
                               const vector<Blob<Dtype>*>& top) {
    if (ExactNumBottomBlobs() >= 0) {
      CHECK_EQ(ExactNumBottomBlobs(), bottom.size())
          << type() << " Layer takes " << ExactNumBottomBlobs()
          << " bottom blob(s) as input.";
    }// 保证输入bottom 数量和要求的相同
    if (MinBottomBlobs() >= 0) {
      CHECK_LE(MinBottomBlobs(), bottom.size())
          << type() << " Layer takes at least " << MinBottomBlobs()
          << " bottom blob(s) as input.";
    }//保证输入的bottom数量大于或等于要求的最小数量
    if (MaxBottomBlobs() >= 0) {
      CHECK_GE(MaxBottomBlobs(), bottom.size())
          << type() << " Layer takes at most " << MaxBottomBlobs()
          << " bottom blob(s) as input.";
    }//保证输入的bottom数量小于或等于要求的最大数量
    if (ExactNumTopBlobs() >= 0) {
      CHECK_EQ(ExactNumTopBlobs(), top.size())
          << type() << " Layer produces " << ExactNumTopBlobs()
          << " top blob(s) as output.";
    }// 保证输入top数量和要求的相同
    if (MinTopBlobs() >= 0) {
      CHECK_LE(MinTopBlobs(), top.size())
          << type() << " Layer produces at least " << MinTopBlobs()
          << " top blob(s) as output.";
    }//保证输入的top数量大于或等于要求的最小数量
    if (MaxTopBlobs() >= 0) {
      CHECK_GE(MaxTopBlobs(), top.size())
          << type() << " Layer produces at most " << MaxTopBlobs()
          << " top blob(s) as output.";
    }//保证输入的top数量小于或等于要求的最大数量
    if (EqualNumBottomTopBlobs()) {
      CHECK_EQ(bottom.size(), top.size())
          << type() << " Layer produces one top blob as output for each "
          << "bottom blob input.";
    }//保证输入的bottom数量和输出的top数量相同
  }
123456789101112131415161718192021222324252627282930313233343536373839
```

### SetLossWeights 函数：

```
  inline void SetLossWeights(const vector<Blob<Dtype>*>& top) {
    const int num_loss_weights = layer_param_.loss_weight_size();
    if (num_loss_weights) {
      CHECK_EQ(top.size(), num_loss_weights) << "loss_weight must be "
          "unspecified or specified once per top blob.";
      for (int top_id = 0; top_id < top.size(); ++top_id) {
        const Dtype loss_weight = layer_param_.loss_weight(top_id);
        if (loss_weight == Dtype(0)) { continue; }//如果为0不对loss进行操作
        this->set_loss(top_id, loss_weight);
        const int count = top[top_id]->count();
        Dtype* loss_multiplier = top[top_id]->mutable_cpu_diff();
        caffe_set(count, loss_weight, loss_multiplier);//将loss_multiplier设为loss_weight
      } 
    }
  }
```