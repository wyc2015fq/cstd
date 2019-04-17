# faster rcnn cudnn版本不兼容问题 - zqjackking的博客 - CSDN博客





2017年07月18日 14:59:16[zqjackking](https://me.csdn.net/zqjackking)阅读数：399








转载自[http://blog.csdn.net/langxing1992/article/details/71440831](http://blog.csdn.net/langxing1992/article/details/71440831)

一般情况下，如果使用的是1080或者Titan的显卡，最好是使用CUDA8.0+cudnn5.1 

可是最新的faster rcnn（[https://github.com/rbgirshick/py-faster-rcnn](https://github.com/rbgirshick/py-faster-rcnn)）与cudnn 5.1，编译caffe-fast-rcnn的时候会报代码错误。 

解决方法有2种; 

(1) 

cd caffe-fast-rcnn 

git remote add caffe [https://github.com/BVLC/caffe.git](https://github.com/BVLC/caffe.git)

git fetch caffe 

git merge caffe/master  

最后注释掉 self_.attr(“phase”) = static_cast(this->phase_); from include/caffe/layers/python_layer.hpp after merging.  

这种方法有时候会成功，有时候则不会 

（2）手动修改文件 参考了卜居大神的博客 [http://blog.csdn.net/kkk584520/article/details/51163564](http://blog.csdn.net/kkk584520/article/details/51163564)

1. 用最新caffe源码的以下文件替换掉faster rcnn 的对应文件 

include/caffe/layers/cudnn_relu_layer.hpp, src/caffe/layers/cudnn_relu_layer.cpp, src/caffe/layers/cudnn_relu_layer.cu 

include/caffe/layers/cudnn_sigmoid_layer.hpp, src/caffe/layers/cudnn_sigmoid_layer.cpp, src/caffe/layers/cudnn_sigmoid_layer.cu 

include/caffe/layers/cudnn_tanh_layer.hpp, src/caffe/layers/cudnn_tanh_layer.cpp, src/caffe/layers/cudnn_tanh_layer.cu 

2 用caffe源码中的这个文件替换掉faster rcnn 对应文件 

include/caffe/util/cudnn.hpp 

（原博客中的第三步则不用）



