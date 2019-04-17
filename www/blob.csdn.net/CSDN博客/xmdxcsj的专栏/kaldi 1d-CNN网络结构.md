# kaldi 1d-CNN网络结构 - xmdxcsj的专栏 - CSDN博客





2017年03月15日 21:23:40[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1935








## 生成nnet.proto

> 
utils/nnet/make_cnn_proto.py


假设输入26维的fbank，feat_raw_dim=26，splice_num=8，delta_order=0： 

num_filters1=128 

num_filters2=256 

pool_size=3 

pool_step=3 

pool_type=MAX 

num_splice=8 

patch_step1=1 

patch_dim1=8 

patch_dim2=4
可以得到输出：

```xml
<NnetProto>
<ConvolutionalComponent> <InputDim> 208 <OutputDim> 2432 <PatchDim> 8 <PatchStep> 1 <PatchStride> 26 <BiasMean> -1.000000 <BiasRange> 2.000000 <ParamStddev> 0.020000 <MaxNorm> 30.000000
<MaxPoolingComponent> <InputDim> 2432 <OutputDim> 768 <PoolSize> 3 <PoolStep> 3 <PoolStride> 128 
<Rescale> <InputDim> 768 <OutputDim> 768 <InitParam> 1.000000
<AddShift> <InputDim> 768 <OutputDim> 768 <InitParam> 0.000000
<Sigmoid> <InputDim> 768 <OutputDim> 768 
<ConvolutionalComponent> <InputDim> 768 <OutputDim> 768 <PatchDim> 4 <PatchStep> 1 <PatchStride> 6 <BiasMean> -2.000000 <BiasRange> 4.000000 <ParamStddev> 0.100000 <MaxNorm> 50.000000
<Rescale> <InputDim> 768 <OutputDim> 768 <InitParam> 1.000000
<AddShift> <InputDim> 768 <OutputDim> 768 <InitParam> 0.000000
<Sigmoid> <InputDim> 768 <OutputDim> 768
```

### conv1
- patch_stride2 

patch_stride2=feat_raw_dim
- 一个滤波器参数： 

filter_dim1=num_splice * patch_dim_=8*8=64
- 所有滤波器参数： 

param1=num_filters1*filter_dim1=8192
- cnn1输入为： 

input1=splice_num*feat_raw_dim=8*26=208
- cnn1滤波器输出 

num_patch1 = 1 + (feat_raw_dim - o.patch_dim1) / o.patch_step1=19 

output1=num_filters1*num_patch1=128*19=2432
相当于输入是8*26的图片，使用8*8的fiter只在频率维度上面进行step为1的移动，1个filter可以得到19*1的输出，128个filter可以得到128*19的输出

### maxpooling
- pool个数 

num_pool = 1 + (num_patch1 - o.pool_size) / o.pool_step=1+(19-3)/3=6
- pool_stride，conv1的filter的个数 

pool_stride=o.num_filters1=128
- output 

o.num_filters1*num_pool=128*6=768

conv1每个滤波器的输出为19，在19这个维度上面，每3个数取一个最大值，得到6个数 

128个滤波器就可以得到128*6的矩阵

### conv2
- 输入为maxpooling的输出 

input2=128*6
- step和conv1一致 

patch_step2 = o.patch_step1=1
- stride和pooling的num_pool一致 

patch_stride2 = num_pool=6
- splice为filter1的个数 

num_splice2=input2/patch_stride2=128
- patch 

num_patch2 = 1 + (num_pool - patch_dim2) / patch_step2=1+(6-4)/1=3
- 一个滤波器参数 

filter_dim2=num_splice2 * patch_dim2_=128*4
- 其他跟conv1的计算方法一样

相当于输入是128*6的图片，使用128*4的filter进行step为1的移动，得到3*1的输出，256个filter可以得到256*3的输出

## 解析

### conv

> 
nnet/nnet-convolutional-component.h


num_splice = input_dim_ / patch_stride_=208/26=8 

num_patches = 1 + (patch_stride_ - patch_dim_) / patch_step_=1+(26-8)/1=19 

filter_dim = num_splice * patch_dim_=8*8=64 

num_filters = output_dim_ / num_patches=2432/19=128
### max pooling

> 
nnet-max-pooling-component.h


num_patches = input_dim_ / pool_stride_ 

num_pools = 1 + (num_patches - pool_size_) / pool_step_






