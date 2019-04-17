# caffe 如何prototxt文件中，添加并传递变量 - 家家的专栏 - CSDN博客





2014年12月22日 21:24:26[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：7791








隔几天没有记录下来，竟然自己都快忘记了的节奏。。。

********solver.prototxt************************************************************************************************

在**solver.prototxt**文件中添加变量：**resultforsnap  = 0.64**

在src/caffe/proto/**caffe.proto **文件中，

message SolverParameter {}中添加：

**optional float resultforsnap = 40 [default = 1e-8];**




**在include/caffe/solver.hpp文件中添加变量：**

**float maxresult；**




如何使用：

在solver.cpp中调用示例为：

maxresult = param_.resultforsnap();  



**并且一定要make clean--->make...**



*******************************************************************************************************






********train_test.prototxt************************************************************************************************

在prototxt文件中，添加：

**softmaxloss_param{    temp： 0.5 }**







在src/caffe/proto/caffe.proto 文件中，

添加（如果已经有，就不用了）：

**optional SoftmaxLossParameter softmaxloss_param =41;**




同时添加：

**message SoftmaxLossParameter {  optional float temp= 1 [default =0.5];}**




如何使用？

请在对应的layer中，

**this->layer_param_.softmaxloss_param().temp()**




就可以调用其参数啦！




************************************************************************************************


************************************************************************************************





哈哈，整理完毕！

整理完，自己这条理也理清楚他们的关系啦~
















