# makefile在编译的过程中出现“except class name” - weixin_33985507的博客 - CSDN博客
2017年12月25日 18:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
今天写了部分代码，在添加到项目中后就那些编译，出现问题如下：
```
logistic_regression_layer.h:20:16: error: expected class name
        public Layer{
```
部分源代码:
```
template <typename Dtype>
class Logistic_regression_layer :
        public Layer{
```
刚开始以为是makefile 没有写好，仔细核对后，发现原来继承类Layer用的模板类，在继承的时候没有指定类型。而造成这个错误. 耽误好几个小时，跪.
改为这个之后，顺利通过，记录下这个有趣的过程. 
```
using mxnet::cpp::Symbol;
using mxnet::cpp::NDArray;
template <typename Dtype>
class Logistic_regression_layer :
        public Layer<Dtype>{
```
完.
