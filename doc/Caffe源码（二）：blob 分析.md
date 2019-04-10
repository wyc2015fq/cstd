# Caffe源码（二）：blob 分析

2015年08月10日 14:53:37 [seven_first](https://me.csdn.net/seven_first) 阅读数：15828



# 目录



- [目录](https://blog.csdn.net/seven_first/article/details/47398613#%E7%9B%AE%E5%BD%95)
- [简单介绍](https://blog.csdn.net/seven_first/article/details/47398613#%E7%AE%80%E5%8D%95%E4%BB%8B%E7%BB%8D)
- 源代码分析
  - [Reshape 函数](https://blog.csdn.net/seven_first/article/details/47398613#1reshape-%E5%87%BD%E6%95%B0)
  - [Blob 构造函数](https://blog.csdn.net/seven_first/article/details/47398613#2blob-%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
  - [data_数据操作函数](https://blog.csdn.net/seven_first/article/details/47398613#3data%E6%95%B0%E6%8D%AE%E6%93%8D%E4%BD%9C%E5%87%BD%E6%95%B0)
  - [反向传播导数diff_ 操作函数](https://blog.csdn.net/seven_first/article/details/47398613#4%E5%8F%8D%E5%90%91%E4%BC%A0%E6%92%AD%E5%AF%BC%E6%95%B0diff-%E6%93%8D%E4%BD%9C%E5%87%BD%E6%95%B0)
  - [ShareData 函数](https://blog.csdn.net/seven_first/article/details/47398613#5sharedata-%E5%87%BD%E6%95%B0)
  - [Updata 函数](https://blog.csdn.net/seven_first/article/details/47398613#6updata-%E5%87%BD%E6%95%B0)
  - [asum_data 函数](https://blog.csdn.net/seven_first/article/details/47398613#7asumdata-%E5%87%BD%E6%95%B0)
  - [asum_diff 函数](https://blog.csdn.net/seven_first/article/details/47398613#8asumdiff-%E5%87%BD%E6%95%B0)
  - [sumsq_data 函数](https://blog.csdn.net/seven_first/article/details/47398613#9sumsqdata-%E5%87%BD%E6%95%B0)
  - [sumsq_diff函数](https://blog.csdn.net/seven_first/article/details/47398613#10sumsqdiff%E5%87%BD%E6%95%B0)
  - [scale_data 函数](https://blog.csdn.net/seven_first/article/details/47398613#11scaledata-%E5%87%BD%E6%95%B0)
  - [scale_diff函数](https://blog.csdn.net/seven_first/article/details/47398613#12scalediff%E5%87%BD%E6%95%B0)
  - [ShapeEquals函数](https://blog.csdn.net/seven_first/article/details/47398613#13shapeequals%E5%87%BD%E6%95%B0)
  - [CopyFrom 函数](https://blog.csdn.net/seven_first/article/details/47398613#14copyfrom-%E5%87%BD%E6%95%B0)
  - [FromProto 函数](https://blog.csdn.net/seven_first/article/details/47398613#15fromproto-%E5%87%BD%E6%95%B0)
  - [ToProto 函数](https://blog.csdn.net/seven_first/article/details/47398613#16toproto-%E5%87%BD%E6%95%B0)



# 简单介绍

Blob 在caffe源码 blob.hpp中是一个模板类。 
protected 的成员变量有：data_ , diff_ , shape_ , count_ , capacity_ ，其中data_ 和 diff_ 是共享SyncedMemory 类（在syncedmem的源码中定义）的智能指针，shape_是int型的vector，count_ 和capacity_ 是整型变量。 
其成员函数主要有：Reshape 、ReshapeLike、SharedData、 Updata 等等。 
blob.hpp 包含了caffe.pb.h ，说明caffe protobuf 会向blob 传递参数。

# 源代码分析

## 1.Reshape 函数：

```
template <typename Dtype>
void Blob<Dtype>::Reshape(const int num, const int channels, const int height,
    const int width) {
  vector<int> shape(4);
  shape[0] = num;
  shape[1] = channels;
  shape[2] = height;
  shape[3] = width;
  Reshape(shape); 
}                                       //该函数将num,channels,height,width传递给vector shape_ 

template <typename Dtype>
void Blob<Dtype>::Reshape(const vector<int>& shape) {
  CHECK_LE(shape.size(), kMaxBlobAxes);
  count_ = 1;
  shape_.resize(shape.size());          //重新定义vector shape_ 的size
  for (int i = 0; i < shape.size(); ++i) {
    CHECK_GE(shape[i], 0);              //确保shape 每个元素为正数
    count_ *= shape[i];
    shape_[i] = shape[i];
  }
  if (count_ > capacity_) {
    capacity_ = count_;  
    data_.reset(new SyncedMemory(capacity_ * sizeof(Dtype)));
    diff_.reset(new SyncedMemory(capacity_ * sizeof(Dtype)));
  }                                    //由于count_超过了当前capacity_ 因此需要重新分配内存空间

}

template <typename Dtype>            // BlobShape 在caffe.proto 中定义
void Blob<Dtype>::Reshape(const BlobShape& shape) {
  CHECK_LE(shape.dim_size(), kMaxBlobAxes);
  vector<int> shape_vec(shape.dim_size());
  for (int i = 0; i < shape.dim_size(); ++i) {
    shape_vec[i] = shape.dim(i);    //dim 包含num，channels，height， width
  }
  Reshape(shape_vec);               //用protobuf传递来dim 对shape_ 进行reshape
}

template <typename Dtype>
void Blob<Dtype>::ReshapeLike(const Blob<Dtype>& other) {
  Reshape(other.shape());
}                                 //用已知的Blob的shape来对shape_ 进行reshape

123456789101112131415161718192021222324252627282930313233343536373839404142434445
```

## 2.Blob 构造函数：

```
//用构造函数的重载的方法定义2个构造函数，以便提供不同的初始化的方法。

template <typename Dtype>
Blob<Dtype>::Blob(const int num, const int channels, const int height,
    const int width)
  // capacity_ must be initialized before calling Reshape
  : capacity_(0) {
  Reshape(num, channels, height, width);
}//用num，channels，height， width 初始化

template <typename Dtype>
Blob<Dtype>::Blob(const vector<int>& shape)
  // capacity_ must be initialized before calling Reshape
  : capacity_(0) {
  Reshape(shape);
}//用shape 初始化
1234567891011121314151617
```

## 3.data_数据操作函数：

```
template <typename Dtype>
const Dtype* Blob<Dtype>::cpu_data() const {
  CHECK(data_);
  return (const Dtype*)data_->cpu_data();
} //返回cpu 中的数据

template <typename Dtype>
void Blob<Dtype>::set_cpu_data(Dtype* data) {
  CHECK(data);
  data_->set_cpu_data(data);
}// 清空cpu 数据

template <typename Dtype>
const Dtype* Blob<Dtype>::gpu_data() const {
  CHECK(data_);
  return (const Dtype*)data_->gpu_data();
}//返回gpu 中的数据
123456789101112131415161718
```

## 4.反向传播导数diff_ 操作函数：

```
template <typename Dtype>
const Dtype* Blob<Dtype>::cpu_diff() const {
  CHECK(diff_);
  return (const Dtype*)diff_->cpu_data();
}//返回cpu 中的数据

template <typename Dtype>
const Dtype* Blob<Dtype>::gpu_diff() const {
  CHECK(diff_);
  return (const Dtype*)diff_->gpu_data();
}//返回gpu 中的数据1234567891011
```

## 5.ShareData 函数：

```
template <typename Dtype>
void Blob<Dtype>::ShareData(const Blob& other) {
  CHECK_EQ(count_, other.count());
  data_ = other.data();
}//当前的blob 的data_ 指向已知blob的数据

template <typename Dtype>
void Blob<Dtype>::ShareDiff(const Blob& other) {
  CHECK_EQ(count_, other.count());
  diff_ = other.diff();
}//当前的blob 的diff_ 指向已知blob的反向传播导数

12345678910111213
```

## 6.Updata 函数：

该函数用于参数blob的更新（weight，bias 等减去对应的导数）

```
template <typename Dtype>
void Blob<Dtype>::Update() {
  // We will perform update based on where the data is located.
  switch (data_->head()) {
  case SyncedMemory::HEAD_AT_CPU:    //数据在cpu上，则在cpu上进行计算
    caffe_axpy<Dtype>(count_, Dtype(-1),
        static_cast<const Dtype*>(diff_->cpu_data()),
        static_cast<Dtype*>(data_->mutable_cpu_data()));  //data_-diff_
    break;
  case SyncedMemory::HEAD_AT_GPU:
  case SyncedMemory::SYNCED:
#ifndef CPU_ONLY //如果没有定义CPU_ONLY，且数据在gpu上，则在gpu上进行计算
    caffe_gpu_axpy<Dtype>(count_, Dtype(-1),
        static_cast<const Dtype*>(diff_->gpu_data()),
        static_cast<Dtype*>(data_->mutable_gpu_data()));
#else
    NO_GPU;
#endif
    break;
  default:
    LOG(FATAL) << "Syncedmem not initialized.";
  }
}
123456789101112131415161718192021222324
```

## 7.asum_data 函数：

```
template <typename Dtype>
Dtype Blob<Dtype>::asum_data() const {
  if (!data_) { return 0; }
  switch (data_->head()) {
  case SyncedMemory::HEAD_AT_CPU:         //数据在cpu上
    return caffe_cpu_asum(count_, cpu_data());     
  case SyncedMemory::HEAD_AT_GPU:
  case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
  {
    Dtype asum;
    caffe_gpu_asum(count_, gpu_data(), &asum);
    return asum;
  }
#else
    NO_GPU;
#endif
  case SyncedMemory::UNINITIALIZED:
    return 0;
  default:
    LOG(FATAL) << "Unknown SyncedMemory head state: " << data_->head();
  }
  return 0;
}

template <> unsigned int Blob<unsigned int>::asum_diff() const {
  NOT_IMPLEMENTED;
  return 0;
}  // 返回data_ 中所有 element 的绝对值之和
123456789101112131415161718192021222324252627282930
```

## 8.asum_diff 函数：

```
template <typename Dtype>
Dtype Blob<Dtype>::asum_diff() const {
  if (!diff_) { return 0; }
  switch (diff_->head()) {
  case SyncedMemory::HEAD_AT_CPU:
    return caffe_cpu_asum(count_, cpu_diff());
  case SyncedMemory::HEAD_AT_GPU:
  case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
  {
    Dtype asum;
    caffe_gpu_asum(count_, gpu_diff(), &asum);
    return asum;
  }
#else
    NO_GPU;
#endif
  case SyncedMemory::UNINITIALIZED:
    return 0;
  default:
    LOG(FATAL) << "Unknown SyncedMemory head state: " << diff_->head();
  }
  return 0;
}  // 返回diff_ 中所有 element 的绝对值之和
12345678910111213141516171819202122232425
```

## 9.sumsq_data 函数：

```
template <typename Dtype>
Dtype Blob<Dtype>::sumsq_data() const {
  Dtype sumsq;
  const Dtype* data;
  if (!data_) { return 0; }
  switch (data_->head()) {
  case SyncedMemory::HEAD_AT_CPU: //数据在cpu上
    data = cpu_data();
    sumsq = caffe_cpu_dot(count_, data, data);  //sumsq = sum(data[i]^2)
    break;
  case SyncedMemory::HEAD_AT_GPU:
  case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
    data = gpu_data();    //数据在gpu上
    caffe_gpu_dot(count_, data, data, &sumsq);
#else
    NO_GPU;
#endif
    break;
  case SyncedMemory::UNINITIALIZED:
    return 0;
  default:
    LOG(FATAL) << "Unknown SyncedMemory head state: " << data_->head();
  }
  return sumsq;
}//返回 data_ 中所有 element 的平方和

12345678910111213141516171819202122232425262728
```

## 10.sumsq_diff函数：

```
template <typename Dtype>
Dtype Blob<Dtype>::sumsq_diff() const {
  Dtype sumsq;
  const Dtype* diff;
  if (!diff_) { return 0; }
  switch (diff_->head()) {
  case SyncedMemory::HEAD_AT_CPU:
    diff = cpu_diff();
    sumsq = caffe_cpu_dot(count_, diff, diff);
    break;
  case SyncedMemory::HEAD_AT_GPU:
  case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
    diff = gpu_diff();
    caffe_gpu_dot(count_, diff, diff, &sumsq);
    break;
#else
    NO_GPU;
#endif
  case SyncedMemory::UNINITIALIZED:
    return 0;
  default:
    LOG(FATAL) << "Unknown SyncedMemory head state: " << data_->head();
  }
  return sumsq;
}//返回 diff_ 中所有 element 的平方和
123456789101112131415161718192021222324252627
```

## 11.scale_data 函数:

```
template <typename Dtype>
void Blob<Dtype>::scale_data(Dtype scale_factor) {
  Dtype* data;
  if (!data_) { return; }
  switch (data_->head()) {
  case SyncedMemory::HEAD_AT_CPU:
    data = mutable_cpu_data();
    caffe_scal(count_, scale_factor, data);
    return;
  case SyncedMemory::HEAD_AT_GPU:
  case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
    data = mutable_gpu_data();
    caffe_gpu_scal(count_, scale_factor, data);
    return;
#else
    NO_GPU;
#endif
  case SyncedMemory::UNINITIALIZED:
    return;
  default:
    LOG(FATAL) << "Unknown SyncedMemory head state: " << data_->head();
  }
}// 给data乘以scale_factor
12345678910111213141516171819202122232425
```

## 12.scale_diff函数

```
template <typename Dtype>
void Blob<Dtype>::scale_diff(Dtype scale_factor) {
  Dtype* diff;
  if (!diff_) { return; }
  switch (diff_->head()) {
  case SyncedMemory::HEAD_AT_CPU:
    diff = mutable_cpu_diff();
    caffe_scal(count_, scale_factor, diff);
    return;
  case SyncedMemory::HEAD_AT_GPU:
  case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
    diff = mutable_gpu_diff();
    caffe_gpu_scal(count_, scale_factor, diff);
    return;
#else
    NO_GPU;
#endif
  case SyncedMemory::UNINITIALIZED:
    return;
  default:
    LOG(FATAL) << "Unknown SyncedMemory head state: " << diff_->head();
  }
}// 给diff乘以scale_factor
12345678910111213141516171819202122232425
```

## 13.ShapeEquals函数：

```
template <typename Dtype>
bool Blob<Dtype>::ShapeEquals(const BlobProto& other) {
//BlobProto 是定义在caffe.proto 中的一个message，其字段有 data， diff， shape， num， channels， height， width
  if (other.has_num() || other.has_channels() ||
      other.has_height() || other.has_width()) {
    // Using deprecated 4D Blob dimensions --
    // shape is (num, channels, height, width).
    // Note: we do not use the normal Blob::num(), Blob::channels(), etc.
    // methods as these index from the beginning of the blob shape, where legacy
    // parameter blobs were indexed from the end of the blob shape (e.g., bias
    // Blob shape (1 x 1 x 1 x N), IP layer weight Blob shape (1 x 1 x M x N)).
    return shape_.size() <= 4 &&
           LegacyShape(-4) == other.num() &&
           LegacyShape(-3) == other.channels() &&
           LegacyShape(-2) == other.height() &&
           LegacyShape(-1) == other.width();
  }
  vector<int> other_shape(other.shape().dim_size());
  for (int i = 0; i < other.shape().dim_size(); ++i) {
    other_shape[i] = other.shape().dim(i);
  }
  return shape_ == other_shape;
}//检查当前的blob和已知的 other 的 shape 是否相同，相同返回true1234567891011121314151617181920212223
```

## 14.CopyFrom 函数：

```
template <typename Dtype>
void Blob<Dtype>::CopyFrom(const Blob& source, bool copy_diff, bool reshape) {
  if (source.count() != count_ || source.shape() != shape_) {
    if (reshape) {
      ReshapeLike(source);   
    } else {
      LOG(FATAL) << "Trying to copy blobs of different sizes.";
    }
  }
  switch (Caffe::mode()) {
  case Caffe::GPU:
    if (copy_diff) {
      caffe_copy(count_, source.gpu_diff(),
          static_cast<Dtype*>(diff_->mutable_gpu_data()));
    } else {
      caffe_copy(count_, source.gpu_data(),
          static_cast<Dtype*>(data_->mutable_gpu_data()));
    }
    break;
  case Caffe::CPU:
    if (copy_diff) {
      caffe_copy(count_, source.cpu_diff(),
          static_cast<Dtype*>(diff_->mutable_cpu_data()));
    } else {
      caffe_copy(count_, source.cpu_data(),
          static_cast<Dtype*>(data_->mutable_cpu_data()));
    }
    break;
  default:
    LOG(FATAL) << "Unknown caffe mode.";
  }
}
//从source 拷贝数据 ， copy_diff控制是拷贝diff还是data
12345678910111213141516171819202122232425262728293031323334
```

## 15.FromProto 函数：

```
template <typename Dtype>
void Blob<Dtype>::FromProto(const BlobProto& proto, bool reshape) {
  if (reshape) {
    vector<int> shape;
    if (proto.has_num() || proto.has_channels() ||
        proto.has_height() || proto.has_width()) {
      // Using deprecated 4D Blob dimensions --
      // shape is (num, channels, height, width).
      shape.resize(4);
      shape[0] = proto.num();
      shape[1] = proto.channels();
      shape[2] = proto.height();
      shape[3] = proto.width();
    } else {
      shape.resize(proto.shape().dim_size());
      for (int i = 0; i < proto.shape().dim_size(); ++i) {
        shape[i] = proto.shape().dim(i);
      }
    }
    Reshape(shape);  
  } else {//如果不做reshape要求当前的blob的shape和proto传入的shape相同
    CHECK(ShapeEquals(proto)) << "shape mismatch (reshape not set)";
  }
  // copy data
  Dtype* data_vec = mutable_cpu_data();
  for (int i = 0; i < count_; ++i) {
    data_vec[i] = proto.data(i);
  }//将proto传入的data拷贝到cpu数据
  if (proto.diff_size() > 0) {
    Dtype* diff_vec = mutable_cpu_diff();
    for (int i = 0; i < count_; ++i) {
      diff_vec[i] = proto.diff(i);
    }//将proto传入的diff 拷贝到cpu数据
  }
}1234567891011121314151617181920212223242526272829303132333435
```

## 16.ToProto 函数：

```
template <typename Dtype>
void Blob<Dtype>::ToProto(BlobProto* proto, bool write_diff) const {
  proto->clear_shape();
  for (int i = 0; i < shape_.size(); ++i) {
    proto->mutable_shape()->add_dim(shape_[i]);
  }
  proto->clear_data();
  proto->clear_diff();
  const Dtype* data_vec = cpu_data();
  for (int i = 0; i < count_; ++i) {
    proto->add_data(data_vec[i]);
  }//将data写入proto
  if (write_diff) {
    const Dtype* diff_vec = cpu_diff();
    for (int i = 0; i < count_; ++i) {
      proto->add_diff(diff_vec[i]);
    }//将diff写入proto
  }
}
INSTANTIATE_CLASS(Blob);
template class Blob<int>;
template class Blob<unsigned int>;
}  //
```