# faster_rcnn 编译caffe-fast-rcnn时报错 - 站在巨人的肩膀上coding - CSDN博客





2018年01月20日 22:11:22[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：156








```cpp
cxx@cxx:~/py-faster-rcnn-original/caffe-fast-rcnn$ make
PROTOC src/caffe/proto/caffe.proto
CXX .build_release/src/caffe/proto/caffe.pb.cc
CXX src/caffe/layer_factory.cpp
In file included from ./include/caffe/util/device_alternate.hpp:40:0,
                 from ./include/caffe/common.hpp:19,
                 from ./include/caffe/blob.hpp:8,
                 from ./include/caffe/layer.hpp:8,
                 from src/caffe/layer_factory.cpp:8:
./include/caffe/util/cudnn.hpp: In function ‘void caffe::cudnn::createPoolingDesc(cudnnPoolingStruct**, caffe::PoolingParameter_PoolMethod, cudnnPoolingMode_t*, int, int, int, int, int, int)’:
./include/caffe/util/cudnn.hpp:127:41: error: too few arguments to function ‘cudnnStatus_t cudnnSetPooling2dDescriptor(cudnnPoolingDescriptor_t, cudnnPoolingMode_t, cudnnNanPropagation_t, int, int, int, int, int, int)’
         pad_h, pad_w, stride_h, stride_w));
                                         ^
./include/caffe/util/cudnn.hpp:15:28: note: in definition of macro ‘CUDNN_CHECK’
     cudnnStatus_t status = condition; \
                            ^
In file included from ./include/caffe/util/cudnn.hpp:5:0,
                 from ./include/caffe/util/device_alternate.hpp:40,
                 from ./include/caffe/common.hpp:19,
                 from ./include/caffe/blob.hpp:8,
                 from ./include/caffe/layer.hpp:8,
                 from src/caffe/layer_factory.cpp:8:
/usr/local/cuda/include/cudnn.h:803:27: note: declared here
 cudnnStatus_t CUDNNWINAPI cudnnSetPooling2dDescriptor(
                           ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetTanHLayer(const caffe::LayerParameter&) [with Dtype = double]’:
src/caffe/layer_factory.cpp:237:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetTanHLayer(const caffe::LayerParameter&) [with Dtype = float]’:
src/caffe/layer_factory.cpp:237:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetSoftmaxLayer(const caffe::LayerParameter&) [with Dtype = double]’:
src/caffe/layer_factory.cpp:214:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetSoftmaxLayer(const caffe::LayerParameter&) [with Dtype = float]’:
src/caffe/layer_factory.cpp:214:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetSigmoidLayer(const caffe::LayerParameter&) [with Dtype = double]’:
src/caffe/layer_factory.cpp:191:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetSigmoidLayer(const caffe::LayerParameter&) [with Dtype = float]’:
src/caffe/layer_factory.cpp:191:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetReLULayer(const caffe::LayerParameter&) [with Dtype = double]’:
src/caffe/layer_factory.cpp:168:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetReLULayer(const caffe::LayerParameter&) [with Dtype = float]’:
src/caffe/layer_factory.cpp:168:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetLRNLayer(const caffe::LayerParameter&) [with Dtype = double]’:
src/caffe/layer_factory.cpp:145:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetLRNLayer(const caffe::LayerParameter&) [with Dtype = float]’:
src/caffe/layer_factory.cpp:145:1: warning: control reaches end of non-void function [-Wreturn-type] }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetPoolingLayer(const caffe::LayerParameter&) [with Dtype = double]’:
src/caffe/layer_factory.cpp:108:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetPoolingLayer(const caffe::LayerParameter&) [with Dtype = float]’:
src/caffe/layer_factory.cpp:108:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetConvolutionLayer(const caffe::LayerParameter&) [with Dtype = double]’:
src/caffe/layer_factory.cpp:71:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
src/caffe/layer_factory.cpp: In function ‘boost::shared_ptr<caffe::Layer<Dtype> > caffe::GetConvolutionLayer(const caffe::LayerParameter&) [with Dtype = float]’:
src/caffe/layer_factory.cpp:71:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
Makefile:563: recipe for target '.build_release/src/caffe/layer_factory.o' failed
make: *** [.build_release/src/caffe/layer_factory.o] Error
```
caffe编译成功，但在编译caffe-fast-rcnn时报错，也就是说我的电脑新装的caffe和cuda，cndnn的版本是对应的。并且，在注释掉cudnn时，caffe-fast-rcnn的编译不出错。因此考虑作者发布的源码中caffe的版本较低，于我的7.0cudnn版本不匹配，因此需要对caffe-fast-rcnn中涉及cudnn的文件全部替换为原先可以成功编译的caffe文件。


解决方法：

 把caffe里面的所有与cudnn相关的.h 和.cpp 替换成能用cudnn 编译过的caffe

需要替换的cudnn有：

第一部分：

1.caffe-my/include/caffe/util 下面的cudnn.hpp

对应的cpp 文件为：caffe-my/src/caffe/util 下面的cudnn.cpp

2.第二部分：caffe-my/include/caffe/layers下面cudnn****_hpp

同样的对应的cpp 文件为：cudnn****_.cpp






