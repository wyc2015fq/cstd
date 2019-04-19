# Caffe源码中Pooling Layer文件分析 - 数据之美的博客 - CSDN博客
2017年03月19日 12:14:23[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：394
Caffe源码(caffe version commit: 09868ac , date: 2015.08.15)中有一些重要的头文件，这里介绍下include/caffe/vision_layers文件中PoolingLayer类，在最新版caffe中，PoolingLayer类被单独放在了include/caffe/layers/pooling_layer.hpp文件中，这两个文件中PoolingLayer类的内容及实现是完全一致的：
1．  include文件：
(1)、<caffe/blob.hpp>：此文件的介绍可以参考：[http://blog.csdn.net/fengbingchun/article/details/59106613](http://blog.csdn.net/fengbingchun/article/details/59106613)
(2)、<caffe/layer.hpp>：此文件的介绍可以参考：[http://blog.csdn.net/fengbingchun/article/details/60871052](http://blog.csdn.net/fengbingchun/article/details/60871052)
(3)、<caffe/proto/caffe.pb.h>：此文件的介绍可以参考：[http://blog.csdn.net/fengbingchun/article/details/55267162](http://blog.csdn.net/fengbingchun/article/details/55267162)
2．  类PoolingLayer：池化层，Layer类的子类
Pooling layer的主要作用是降维，缩小feature map，图像降采样，方法有：
(1)、均值采样：取区域平均值作为降采样值；
(2)、最大值采样：取区域最大值作为降采样值；
(3)、随机采样：取区域内随机一个像素值。
<caffe/layers/pooling_layer.hpp>文件的详细介绍如下：
**[cpp]**[view
 plain](http://blog.csdn.net/fengbingchun/article/details/60962777#)[copy](http://blog.csdn.net/fengbingchun/article/details/60962777#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2253801/fork)
- #ifndef CAFFE_POOLING_LAYER_HPP_
- #define CAFFE_POOLING_LAYER_HPP_
- 
- #include <vector>
- 
- #include "caffe/blob.hpp"
- #include "caffe/layer.hpp"
- #include "caffe/proto/caffe.pb.h"
- 
- namespace caffe {  
- /**
-  * @brief Pools the input image by taking the max, average, etc. within regions.
-  *
-  * TODO(dox): thorough documentation for Forward, Backward, and proto params.
-  */
- // 池化层，Layer类的子类，图像降采样，有三种Pooling方法：Max、Avx、Stochastic
- template <typename Dtype>  
- class PoolingLayer : public Layer<Dtype> {  
- public:  
- // 显示构造函数
- explicit PoolingLayer(const LayerParameter& param) : Layer<Dtype>(param) {}  
- // 参数初始化，通过类PoolingParameter获取成员变量值，包括：
- // global_pooling_、kernel_h_、kernel_w_、pad_h_、pad_w_、stride_h_、stride_w_
- virtualvoid LayerSetUp(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top);  
- // 调整top blobs的shape，并有可能会reshape rand_idx_或max_idx_；
- // 获取成员变量值，包括：channels_、height_、width_、pooled_height_、pooled_width_
- virtualvoid Reshape(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top);  
- // 获得Pooling layer的类型: Pooling
- virtualinlineconstchar* type() const { return"Pooling"; }  
- // 获得Pooling layer所需的bottom blobs的个数: 1
- virtualinlineint ExactNumBottomBlobs() const { return 1; }  
- // 获得Pooling layer所需的bottom blobs的最少个数: 1
- virtualinlineint MinTopBlobs() const { return 1; }  
- // MAX POOL layers can output an extra top blob for the mask;
- // others can only output the pooled inputs.
- // 获得Pooling layer所需的bottom blobs的最多个数: Max为2，其它(Avg, Stochastic)为1
- virtualinlineint MaxTopBlobs() const {  
- return (this->layer_param_.pooling_param().pool() ==  
-             PoolingParameter_PoolMethod_MAX) ? 2 : 1;  
-   }  
- 
- protected:  
- // CPU实现Pooling layer的前向传播，仅有Max和Ave两种方法实现
- virtualvoid Forward_cpu(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top);  
- // GPU实现Pooling layer的前向传播，Max、Ave、Stochastic三种方法实现
- virtualvoid Forward_gpu(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top);  
- // CPU实现Pooling layer的反向传播，仅有Max和Ave两种方法实现
- virtualvoid Backward_cpu(const vector<Blob<Dtype>*>& top,  
- const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);  
- // GPU实现Pooling layer的反向传播，Max、Ave、Stochastic三种方法实现
- virtualvoid Backward_gpu(const vector<Blob<Dtype>*>& top,  
- const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);  
- 
- // Caffe中类的成员变量名都带有后缀"_"，这样就容易区分临时变量和类成员变量
- int kernel_h_, kernel_w_; // 滤波器(卷积核)大小
- int stride_h_, stride_w_; // 步长大小
- int pad_h_, pad_w_; // 图像扩充大小
- int channels_; // 图像通道数
- int height_, width_; // 图像高、宽
- // 池化后图像高、宽
- // pooled_height_ = (height_ + 2 * pad_h_ - kernel_h_) / stride_h_)) + 1
- // pooled_width_ = (width_ + 2 * pad_w_ - kernel_w_) / stride_w_)) + 1
- int pooled_height_, pooled_width_;  
- bool global_pooling_; // 是否全区域池化(将整幅图像降采样为1*1)
-   Blob<Dtype> rand_idx_; // 随机采样索引,Pooling方法为STOCHASTIC时用到并会Reshape
-   Blob<int> max_idx_; // 最大值采样索引，Pooling方法为MAX时用到并会Reshape
- };  
- 
- }  // namespace caffe
- 
- #endif  // CAFFE_POOLING_LAYER_HPP_
在caffe.proto文件中，有一个message是与pooling layer相关的，如下：
**[cpp]**[view
 plain](http://blog.csdn.net/fengbingchun/article/details/60962777#)[copy](http://blog.csdn.net/fengbingchun/article/details/60962777#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2253801/fork)
- message PoolingParameter { // Pooling层参数类
- enum PoolMethod { // 枚举类型，Pooling的方法：Max(最大值采样)、AVE(均值采样)、STOCHASTIC(随机采样)
-     MAX = 0;  
-     AVE = 1;  
-     STOCHASTIC = 2;  
-   }  
-   optional PoolMethod pool = 1 [default = MAX]; // The pooling method, pooling方法
- // Pad, kernel size, and stride are all given as a single value for equal
- // dimensions in height and width or as Y, X pairs.
-   optional uint32 pad = 4 [default = 0]; // The padding size (equal in Y, X)，图像扩充大小(添加图像边界的像素大小)
-   optional uint32 pad_h = 9 [default = 0]; // The padding height,图像扩充大小，Y
-   optional uint32 pad_w = 10 [default = 0]; // The padding width，图像扩充大小，X
-   optional uint32 kernel_size = 2; // The kernel size (square)，滤波器(卷积核、滑动窗)的大小(高=宽)
-   optional uint32 kernel_h = 5; // The kernel height，滤波器(卷积核、滑动窗)的高
-   optional uint32 kernel_w = 6; // The kernel width，滤波器(卷积核、滑动窗)的宽
-   optional uint32 stride = 3 [default = 1]; // The stride (equal in Y, X),滑动步长(高=宽)，卷积核卷积时平移的步幅
-   optional uint32 stride_h = 7; // The stride height,滑动步长，高
-   optional uint32 stride_w = 8; // The stride width，滑动步长，宽
- enum Engine {  
-     DEFAULT = 0;  
-     CAFFE = 1;  
-     CUDNN = 2;  
-   }  
-   optional Engine engine = 11 [default = DEFAULT]; //
- // If global_pooling then it will pool over the size of the bottom by doing
- // kernel_h = bottom->height and kernel_w = bottom->width
-   optional bool global_pooling = 12 [default = false]; // 是否是全区域池化
- }  
pooling layer的测试代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/fengbingchun/article/details/60962777#)[copy](http://blog.csdn.net/fengbingchun/article/details/60962777#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2253801/fork)
- #include "funset.hpp"
- #include <string>
- #include <vector>
- #include "common.hpp"
- 
- int test_caffe_layer_pooling()  
- {  
-     caffe::Caffe::set_mode(caffe::Caffe::CPU); // set run caffe mode
- 
- // set layer parameter
-     caffe::LayerParameter layer_param;  
-     layer_param.set_phase(caffe::Phase::TRAIN);  
- 
- // cv::Mat -> caffe::Blob
-     std::string image_name = "E:/GitCode/Caffe_Test/test_data/images/a.jpg";  
-     cv::Mat mat1 = cv::imread(image_name, 1);  
- if (!mat1.data) {  
-         fprintf(stderr, "read image fail: %s\n", image_name.c_str());  
- return -1;  
-     }  
-     mat1.convertTo(mat1, CV_32FC3);  
-     std::vector<cv::Mat> mat2;  
-     cv::split(mat1, mat2);  
-     std::vector<int> mat_reshape{ 1, (int)mat2.size(), mat2[0].rows, mat2[0].cols };  
- 
-     caffe::Blob<float> blob;  
-     blob.Reshape(mat_reshape);  
- size_t size = mat2[0].rows * mat2[0].cols;  
- float* data = newfloat[mat2.size() * size];  
-     memcpy(data, mat2[0].data, size * sizeof(float));  
-     memcpy(data + size, mat2[1].data, size * sizeof(float));  
-     memcpy(data + 2 * size, mat2[2].data, size * sizeof(float));  
-     blob.set_cpu_data(data);  
- 
- for (int method = 0; method < 2; ++method) {  
- // set pooling parameter
-         caffe::PoolingParameter* pooling_param = layer_param.mutable_pooling_param();  
- if (method == 0) pooling_param->set_pool(caffe::PoolingParameter::MAX);  
- else pooling_param->set_pool(caffe::PoolingParameter::AVE);  
-         pooling_param->set_kernel_size(3);  
-         pooling_param->set_pad(2);  
-         pooling_param->set_stride(2);  
-         pooling_param->set_global_pooling(false);  
- 
-         std::vector<caffe::Blob<float>*> bottom_blob{ &blob }, top_blob{ &caffe::Blob<float>()/*, &caffe::Blob<float>() */ };  
- 
- // test PoolingLayer function
-         caffe::PoolingLayer<float> pooling_layer(layer_param);  
-         pooling_layer.SetUp(bottom_blob, top_blob);  
-         fprintf(stderr, "top blob info: channels: %d, height: %d, width: %d\n",  
-             top_blob[0]->channels(), top_blob[0]->height(), top_blob[0]->width());  
- 
-         pooling_layer.Forward(bottom_blob, top_blob);  
- 
- int height = top_blob[0]->height();  
- int width = top_blob[0]->width();  
- constfloat* p = top_blob[0]->cpu_data();  
-         std::vector<cv::Mat> mat3{ cv::Mat(height, width, CV_32FC1, (float*)p),  
-             cv::Mat(height, width, CV_32FC1, (float*)(p + height * width)),  
-             cv::Mat(height, width, CV_32FC1, (float*)(p + height * width * 2)) };  
-         cv::Mat mat4;  
-         cv::merge(mat3, mat4);  
-         mat4.convertTo(mat4, CV_8UC3);  
- if (method == 0) image_name = "E:/GitCode/Caffe_Test/test_data/images/forward0.jpg";  
- else image_name = "E:/GitCode/Caffe_Test/test_data/images/forward1.jpg";  
-         cv::imwrite(image_name, mat4);  
- 
- for (int i = 0; i < bottom_blob[0]->count(); ++i)  
-             bottom_blob[0]->mutable_cpu_diff()[i] = bottom_blob[0]->cpu_data()[i];  
- for (int i = 0; i < top_blob[0]->count(); ++i)  
-             top_blob[0]->mutable_cpu_diff()[i] = top_blob[0]->cpu_data()[i];  
- 
-         std::vector<bool> propagate_down{ true };  
-         pooling_layer.Backward(top_blob, propagate_down, bottom_blob);  
- 
-         height = bottom_blob[0]->height();  
-         width = bottom_blob[0]->width();  
-         p = bottom_blob[0]->cpu_diff();  
-         std::vector<cv::Mat> mat5{ cv::Mat(height, width, CV_32FC1, (float*)p),  
-             cv::Mat(height, width, CV_32FC1, (float*)(p + height * width)),  
-             cv::Mat(height, width, CV_32FC1, (float*)(p + height * width * 2)) };  
-         cv::Mat mat6;  
-         cv::merge(mat5, mat6);  
-         mat6.convertTo(mat6, CV_8UC3);  
- if (method == 0) image_name = "E:/GitCode/Caffe_Test/test_data/images/backward0.jpg";  
- else image_name = "E:/GitCode/Caffe_Test/test_data/images/backward1.jpg";  
-         cv::imwrite(image_name, mat6);  
-     }  
- 
- delete[] data;  
- return 0;  
- }  
执行结果如下图：
![](https://img-blog.csdn.net/20170309164000268)
图像结果(Lena.jpg)如下：前向传播、反向传播，前两幅为Max，后两幅为Avg方法
![](https://img-blog.csdn.net/20170309164141921)![](https://img-blog.csdn.net/20170309164155739)
![](https://img-blog.csdn.net/20170309164341641)![](https://img-blog.csdn.net/20170309164349491)
**GitHub**：[https://github.com/fengbingchun/Caffe_Test](https://github.com/fengbingchun/Caffe_Test)
