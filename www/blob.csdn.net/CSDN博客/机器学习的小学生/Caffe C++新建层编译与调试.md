
# Caffe C++新建层编译与调试 - 机器学习的小学生 - CSDN博客


2018年03月14日 14:48:17[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：225


实验环境： vs2013, python2.7(Miniconda2),Windows10, cuda8.0
感觉用windows系统做深度学习就是败笔。
以前用caffe提供的python接口，利用python层新建层，但是新建的层不支持GPU。因此需要自己动手写GPU的C++代码。面临的问题对C++的各个接口不是太熟悉，这是其一，其二习惯了python中数据的输入方式，例如使用HDF5或者建立一个Python层用来batch式的读取数据。
利用Python接口时，使用HDF5数据，好像不会对数据的第一维度(即样本数）进行检查例如，hdf5数据集中存储如下形式的数据：
```python
data:
```
```python
64
```
```python
*
```
```python
3
```
```python
*
```
```python
224
```
```python
*
```
```python
224
```
```python
(nsample * channel * height * width)
label:
```
```python
64
```
```python
*
```
```python
5
```
```python
(nsample * label_distribution)
boxes:
```
```python
128
```
```python
*
```
```python
4
```
```python
(nbound_box * position)
```
使用caffe C++ 读取.prototxt指定的hdf5文件时，会提示错误，bottom[0].shape[0] vs
bottom[2].shape[0] (64 vs 128) 不匹配。
如果简单的对自己的层进行测试，caffe 提供了DummyData层：
```python
layer{
  name:
```
```python
"data"
```
```python
type:
```
```python
"DummyData"
```
```python
top:
```
```python
"data"
```
```python
top:
```
```python
"labels"
```
```python
top:
```
```python
"roi"
```
```python
dummy_data_param {  
    data_filler {  
      type:
```
```python
"constant"
```
```python
value:
```
```python
0.01
```
```python
}  
    shape {  
      dim:
```
```python
16
```
```python
dim:
```
```python
3
```
```python
dim:
```
```python
224
```
```python
dim:
```
```python
224
```
```python
} 
    data_filler{
    type:
```
```python
"constant"
```
```python
value:
```
```python
0.02
```
```python
} 
  shape{
  dim:
```
```python
16
```
```python
dim:
```
```python
1
```
```python
dim:
```
```python
1
```
```python
dim:
```
```python
1
```
```python
}
  data_filler{
  type:
```
```python
"constant"
```
```python
value:
```
```python
0.03
```
```python
}
  shape{
  dim:
```
```python
48
```
```python
dim:
```
```python
5
```
```python
dim:
```
```python
1
```
```python
dim:
```
```python
1
```
```python
}
}
}
```
caffe提供了几种数据填充方式，感觉仅能做简单的预测或者测试，对于自己新模块的测试，可能需要特定的数据才能观察出问题来，因此还是需要在forward前访问到数据的地址，填充自己的定制化数据。上面三种填充方式分别对应上面的三个blob.
Input层 可以设置不同形状的blob，例如：
```python
layer {
  name:
```
```python
"data"
```
```python
type:
```
```python
"Input"
```
```python
top:
```
```python
"data"
```
```python
top:
```
```python
"labels"
```
```python
top:
```
```python
"rois"
```
```python
input_param {
    shape {
      dim:
```
```python
16
```
```python
dim:
```
```python
3
```
```python
dim:
```
```python
224
```
```python
dim:
```
```python
224
```
```python
}
    shape{
    dim:
```
```python
16
```
```python
dim:
```
```python
1
```
```python
dim:
```
```python
1
```
```python
dim:
```
```python
1
```
```python
}
    shape{
    dim:
```
```python
48
```
```python
dim:
```
```python
5
```
```python
dim:
```
```python
1
```
```python
dim:
```
```python
1
```
```python
}
  }
}
```
```python
# 下面是旧版的输入方式
```
```python
input:
```
```python
"data"
```
```python
input_dim:
```
```python
16
```
```python
input_dim:
```
```python
3
```
```python
input_dim:
```
```python
224
```
```python
input_dim:
```
```python
224
```
```python
input:
```
```python
"labels"
```
```python
input_dim:
```
```python
16
```
```python
input_dim:
```
```python
1
```
```python
input_dim:
```
```python
1
```
```python
input_dim:
```
```python
1
```
```python
input:
```
```python
"labels"
```
```python
input_dim:
```
```python
48
```
```python
input_dim:
```
```python
5
```
```python
input_dim:
```
```python
1
```
```python
input_dim:
```
```python
1
```
上面三个blob，即data, headposes, rois 在训练的时候可以重新reshape其形状，然后feed 数据。
## 为caffe c++ 定制输入数据进行检测自定义模块
习惯使用 matlab和python来处理图像数据。对于C++的话，可以使用OpenCV库来完成图像数据的输入。
下面将测试数据封装成 matlab文件，然后利用c++（vs）读取matlab文件中的数据，将数据赋给input层中的blobs.
新建一个vs工程，在vs中添加matlab的头文件和库目录：
头文件：
```python
D:\MATLAB\R2014a\extern\include
```
库目录：
```python
D:\MATLAB\R2014a\extern\lib\win64\microsoft\libmx.lib
D:\MATLAB\R2014a\extern\lib\win64\microsoft\libmex.lib
D:\MATLAB\R2014a\extern\lib\win64\microsoft\libmat.lib
D:\MATLAB\R2014a\extern\lib\win64\microsoft\libeng.lib
```
在vs工程中添加编译的caffe release版本的库（debug版本总是出现一些意想不到的错误，并且幸运的是，release模式下也可以对自己编写的代码进行调试），及其头文件：
```python
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8
```
```python
.0
```
```python
\include
E:\caffe-windows2\include
E:\caffe-windows2\scripts\build\include
E:\caffe-windows2\scripts\build\libraries\include
E:\caffe-windows2\scripts\build\libraries\include\boost-
```
```python
1
```
```python
_61
```
库目录（连接器输入）：
```python
E:\caffe-windows2\scripts\build\lib\Release\caffe.lib
E:\caffe-windows2\scripts\build\lib\Release\proto.lib
E:\caffe-windows2\scripts\build\lib\Release\gtest.lib
E:\caffe-windows2\scripts\build\libraries\lib\boost_system-vc120-mt-
```
```python
1
```
```python
_61.lib
E:\caffe-windows2\scripts\build\libraries\lib\boost_thread-vc120-mt-
```
```python
1
```
```python
_61.lib
E:\caffe-windows2\scripts\build\libraries\lib\boost_chrono-vc120-mt-
```
```python
1
```
```python
_61.lib
E:\caffe-windows2\scripts\build\libraries\lib\boost_date_time-vc120-mt-
```
```python
1
```
```python
_61.lib
E:\caffe-windows2\scripts\build\libraries\lib\boost_atomic-vc120-mt-
```
```python
1
```
```python
_61.lib
E:\caffe-windows2\scripts\build\libraries\lib\boost_python-vc120-mt-
```
```python
1
```
```python
_61.lib
E:\caffe-windows2\scripts\build\libraries\lib\glog.lib
E:\caffe-windows2\scripts\build\libraries\lib\gflags.lib
shlwapi.lib
E:\caffe-windows2\scripts\build\libraries\lib\libprotobuf.lib
E:\caffe-windows2\scripts\build\libraries\lib\caffehdf5_hl.lib
E:\caffe-windows2\scripts\build\libraries\lib\caffehdf5.lib
E:\caffe-windows2\scripts\build\libraries\lib\lmdb.lib
ntdll.lib
E:\caffe-windows2\scripts\build\libraries\lib\leveldb.lib
E:\caffe-windows2\scripts\build\libraries\lib\snappy_static.lib
E:\caffe-windows2\scripts\build\libraries\lib\caffezlib.lib
E:\caffe-windows2\scripts\build\libraries\x64\vc12\lib\opencv_highgui310.lib
E:\caffe-windows2\scripts\build\libraries\x64\vc12\lib\opencv_imgcodecs310.lib
E:\caffe-windows2\scripts\build\libraries\x64\vc12\lib\opencv_imgproc310.lib
E:\caffe-windows2\scripts\build\libraries\x64\vc12\lib\opencv_core310.lib
E:\caffe-windows2\scripts\build\libraries\lib\libopenblas.dll.a
D:\python27\libs\python27.lib
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8
```
```python
.0
```
```python
\lib\x64\cudart.lib
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8
```
```python
.0
```
```python
\lib\x64\curand.lib
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8
```
```python
.0
```
```python
\lib\x64\cublas.lib
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8
```
```python
.0
```
```python
\lib\x64\cublas_device.lib
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8
```
```python
.0
```
```python
\lib\x64\cudnn.lib
E:\caffe-windows2\scripts\build\libraries\lib\libboost_filesystem-vc120-mt-
```
```python
1
```
```python
_61.lib
E:\caffe-windows2\scripts\build\libraries\lib\libboost_system-vc120-mt-
```
```python
1
```
```python
_61.lib
E:\caffe-windows2\scripts\build\libraries\lib\libboost_date_time-vc120-mt-
```
```python
1
```
```python
_61.lib
```
上面的库可能有些不一定用到，全包含总是好的。
第一次写cu文件，在.cu文件中，使用了caffe_set< Dtype>函数就会下面这样的错误：
```python
0xC0000006
```
```python
:
```
```python
写入位置 0x0000000C1C000000 时发生页面中的错误
```
因为，在执行.cu文件中的函数时，数据通常存储在GPU上，所以应使用GPU上对应的指令，caffe对其进行了封装，即 caffe_gpu_set< Dtype>。
有时可能也会碰到下面从错误:
```python
0xC0000005
```
```python
:
```
```python
写入位置 0x000000001D2B1000 时发生访问冲突。
```
copy时 src和dst地址大小不同引起的。
进行测试层的代码如下:
```python
#include<iostream>
```
```python
#include<string>
```
```python
#include<vector>
```
```python
#include<opencv2/opencv.hpp>
```
```python
#include<caffe/caffe.hpp>
```
```python
#include"caffe_reg.h"
```
```python
#include"sin_layer.hpp"
```
```python
#include<memory>
```
```python
#include<mat.h>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
using
```
```python
namespace
```
```python
cv;
```
```python
using
```
```python
namespace
```
```python
caffe;
```
```python
void
```
```python
print_shape(
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
& shape)
{
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < shape.size(); i++)
```
```python
cout
```
```python
<< shape[i] <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;
}
```
```python
void
```
```python
read_data_from_mat(
```
```python
string
```
```python
filename,
```
```python
float
```
```python
* * feat_ptr,
```
```python
float
```
```python
* * head_ptr,
```
```python
float
```
```python
* *mult_ptr,
```
```python
int
```
```python
& feat_num,
```
```python
int
```
```python
& head_num,
```
```python
int
```
```python
& mult_num)
{
    MATFile *data_pmat = NULL;
    mxArray * feat_var = NULL;
    mxArray * head_var = NULL;
    mxArray * mult_var = NULL;
```
```python
const
```
```python
char
```
```python
** var_names;
```
```python
// variable names in .mat file ,testFeatures, testHeadposes, testMultiboxes
```
```python
int
```
```python
nvars;
    data_pmat = matOpen(filename.c_str(),
```
```python
"r"
```
```python
);
```
```python
if
```
```python
(data_pmat == NULL)
    {
```
```python
cout
```
```python
<<
```
```python
"cannot open .mat file !"
```
```python
<< endl;
```
```python
exit
```
```python
(-
```
```python
1
```
```python
);
    }
```
```python
// read the varaibles in .mat file
```
```python
var_names = (
```
```python
const
```
```python
char
```
```python
**)matGetDir(data_pmat, &nvars);
```
```python
if
```
```python
(var_names == NULL)
    {
```
```python
cout
```
```python
<<
```
```python
"The .mat file contarin 0 variable, please perform checking!"
```
```python
<< endl;
```
```python
exit
```
```python
(-
```
```python
1
```
```python
);
    }
```
```python
// output the number of variable in .mat file
```
```python
cout
```
```python
<<
```
```python
"The numboer of variable in.mat file is : "
```
```python
<< nvars << endl;
    feat_var = matGetVariable(data_pmat, var_names[
```
```python
0
```
```python
]);
```
```python
// nsample * c * h * w
```
```python
head_var = matGetVariable(data_pmat, var_names[
```
```python
1
```
```python
]);
```
```python
//nsample * 1 * 1 * 1
```
```python
mult_var = matGetVariable(data_pmat, var_names[
```
```python
2
```
```python
]);
```
```python
//(nsmaple * 3) * 5
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < nvars; i++)
```
```python
cout
```
```python
<< var_names[i] <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;
```
```python
int
```
```python
feat_dim = mxGetNumberOfDimensions(feat_var);
```
```python
int
```
```python
head_dim = mxGetNumberOfDimensions(head_var);
```
```python
int
```
```python
mult_dim = mxGetNumberOfDimensions(mult_var);
```
```python
cout
```
```python
<<
```
```python
"dim1: "
```
```python
<< feat_dim <<
```
```python
";dim2: "
```
```python
<< head_dim <<
```
```python
"dim3: "
```
```python
<< mult_dim << endl;
```
```python
const
```
```python
size_t * feat_dims = mxGetDimensions(feat_var);
```
```python
const
```
```python
size_t * head_dims = mxGetDimensions(head_var);
```
```python
const
```
```python
size_t * mult_dims = mxGetDimensions(mult_var);
```
```python
// output
```
```python
feat_num = feat_dims[
```
```python
0
```
```python
] * feat_dims[
```
```python
1
```
```python
] * feat_dims[
```
```python
2
```
```python
] * feat_dims[
```
```python
3
```
```python
];
    head_num = head_dims[
```
```python
0
```
```python
] * head_dims[
```
```python
1
```
```python
];
    mult_num = mult_dims[
```
```python
0
```
```python
] * mult_dims[
```
```python
1
```
```python
];
```
```python
// note that: three variables in .mat file is single float type
```
```python
*feat_ptr = (
```
```python
float
```
```python
*)mxGetData(feat_var);
    *head_ptr = (
```
```python
float
```
```python
*)mxGetData(head_var);
    *mult_ptr = (
```
```python
float
```
```python
*)mxGetData(mult_var);
}
```
```python
//int main_test_sampleadd_train()
```
```python
int
```
```python
main(
```
```python
void
```
```python
)
{
```
```python
// for  train
```
```python
Caffe::set_mode(Caffe::GPU);
```
```python
bool
```
```python
status_GPU =
```
```python
true
```
```python
;
    SolverParameter solver_param;
```
```python
string
```
```python
solver_file =
```
```python
"ZOO_VGG16/solver_sampleadd.prototxt"
```
```python
;
```
```python
//string train_proto = "ZOO_VGG16/train_sinlayer.prototxt";
```
```python
ReadSolverParamsFromTextFileOrDie(solver_file, &solver_param);
```
```python
//boost::shared_ptr<Solver<float> > solver(
```
```python
//  SolverRegistry<float>::CreateSolver(solver_param));
```
```python
Solver<
```
```python
float
```
```python
> * solver = SolverRegistry<
```
```python
float
```
```python
>::CreateSolver(solver_param);
    boost::
```
```python
shared_ptr
```
```python
<Net<
```
```python
float
```
```python
> > net = solver->net();
```
```python
//train net
```
```python
// read testFeatures, testHeapdoses, testMultiboxes from .mat file
```
```python
float
```
```python
* feat_ptr = NULL;
```
```python
float
```
```python
* head_ptr = NULL;
```
```python
float
```
```python
* mult_ptr = NULL;
```
```python
int
```
```python
feat_num;
```
```python
// the number of elements in blob
```
```python
int
```
```python
head_num;
```
```python
int
```
```python
mult_num;
```
```python
string
```
```python
filename =
```
```python
"data/debug_data6.mat"
```
```python
;
    read_data_from_mat(filename, &feat_ptr, &head_ptr, &mult_ptr, feat_num, head_num, mult_num);
```
```python
// copy the data in mat to blobs
```
```python
Blob<
```
```python
float
```
```python
> * feat_blob = net->input_blobs()[
```
```python
0
```
```python
];
    Blob<
```
```python
float
```
```python
> * head_blob = net->input_blobs()[
```
```python
1
```
```python
];
    Blob<
```
```python
float
```
```python
> * mult_blob = net->input_blobs()[
```
```python
2
```
```python
];
```
```python
int
```
```python
nsample =
```
```python
6
```
```python
;
```
```python
int
```
```python
nbox =
```
```python
3
```
```python
;
    feat_blob->Reshape(nsample,
```
```python
3
```
```python
,
```
```python
224
```
```python
,
```
```python
224
```
```python
);
    head_blob->Reshape(nsample,
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
);
    mult_blob->Reshape(nsample * nbox,
```
```python
5
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
);
    net->Reshape();
```
```python
float
```
```python
* feat_blob_ptr = NULL;
```
```python
float
```
```python
* head_blob_ptr = NULL;
```
```python
float
```
```python
* mult_blob_ptr = NULL;
```
```python
if
```
```python
(status_GPU)
    {
        feat_blob_ptr = feat_blob->mutable_gpu_data();
        head_blob_ptr = head_blob->mutable_gpu_data();
        mult_blob_ptr = mult_blob->mutable_gpu_data();
    }
```
```python
else
```
```python
{
        feat_blob_ptr = feat_blob->mutable_cpu_data();
        head_blob_ptr = head_blob->mutable_cpu_data();
        mult_blob_ptr = mult_blob->mutable_cpu_data();
    }
```
```python
cout
```
```python
<<
```
```python
"check the data in memory.... "
```
```python
<< endl;
```
```python
int
```
```python
mini_batch =
```
```python
10
```
```python
;
```
```python
// the following data is in cpu memory,i.e. the pointers (feat_ptr,head_ptr,) point to cpu memory
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < mini_batch; i++)
```
```python
cout
```
```python
<< feat_ptr[i] <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < mini_batch; i++)
```
```python
cout
```
```python
<< head_ptr[i] <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;

    caffe_copy(feat_num, feat_ptr, feat_blob_ptr);
    caffe_copy(head_num, head_ptr, head_blob_ptr);
    caffe_copy(mult_num, mult_ptr, mult_blob_ptr);
```
```python
//feat_blob_ptr: this pointer is point to GPU memory
```
```python
cout
```
```python
<<
```
```python
"after copy: "
```
```python
<< endl;
```
```python
if
```
```python
(status_GPU)
    {
```
```python
// in order to show the results, we should access to cpu memory ,
```
```python
const
```
```python
float
```
```python
*temp_feat = feat_blob->cpu_data();
```
```python
const
```
```python
float
```
```python
*temp_head = head_blob->cpu_data();
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < mini_batch; i++)
```
```python
cout
```
```python
<< temp_feat[i] <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < mini_batch; i++)
```
```python
cout
```
```python
<< temp_head[i] <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;
    }
```
```python
else
```
```python
{
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < mini_batch; i++)
```
```python
cout
```
```python
<< feat_blob_ptr[i] <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < mini_batch; i++)
```
```python
cout
```
```python
<< head_blob_ptr[i] <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;
    }
```
```python
//solver->Step(1);
```
```python
cout
```
```python
<<
```
```python
"forward... "
```
```python
<< endl;
    net->Forward();
```
```python
// forward once
```
```python
//check forward data
```
```python
//const float *sample = net->blob_by_name("sampleadd5")->gpu_data();
```
```python
//const float * roi = net->blob_by_name("roi_pool5")->gpu_data();
```
```python
const
```
```python
float
```
```python
*sample = NULL;
```
```python
const
```
```python
float
```
```python
*roi = NULL;
```
```python
if
```
```python
(status_GPU)
    {
        sample = net->blob_by_name(
```
```python
"sampleadd5"
```
```python
)->gpu_data();
        roi = net->blob_by_name(
```
```python
"roi_pool5"
```
```python
)->gpu_data();
    }
```
```python
else
```
```python
{
        sample = net->blob_by_name(
```
```python
"sampleadd5"
```
```python
)->cpu_data();
        roi = net->blob_by_name(
```
```python
"roi_pool5"
```
```python
)->cpu_data();
    }
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
sample_shape = net->blob_by_name(
```
```python
"sampleadd5"
```
```python
)->shape();
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
roi_shape = net->blob_by_name(
```
```python
"roi_pool5"
```
```python
)->shape();
```
```python
cout
```
```python
<<
```
```python
" The shape of input: "
```
```python
<< endl;
    print_shape(roi_shape);
```
```python
cout
```
```python
<<
```
```python
"The shape of output: "
```
```python
<< endl;
    print_shape(sample_shape);
```
```python
cout
```
```python
<<
```
```python
"start check the partial results for forward...."
```
```python
<< endl;
```
```python
int
```
```python
volume = sample_shape[
```
```python
1
```
```python
] * sample_shape[
```
```python
2
```
```python
] * sample_shape[
```
```python
3
```
```python
];
```
```python
int
```
```python
sample_size = sample_shape[
```
```python
0
```
```python
] * volume;
```
```python
int
```
```python
roi_size = roi_shape[
```
```python
0
```
```python
] * roi_shape[
```
```python
1
```
```python
] * roi_shape[
```
```python
2
```
```python
] * roi_shape[
```
```python
3
```
```python
];
```
```python
int
```
```python
index =
```
```python
0
```
```python
;
```
```python
bool
```
```python
cudaStatus;
```
```python
float
```
```python
* temp_sample = NULL;
```
```python
float
```
```python
* temp_roi = NULL;
```
```python
if
```
```python
(status_GPU)
    {
        temp_sample =
```
```python
new
```
```python
float
```
```python
[sample_size];
        temp_roi =
```
```python
new
```
```python
float
```
```python
[roi_size];
```
```python
// copy gpu data to cpu
```
```python
cudaStatus = cudaMemcpy(temp_sample, sample, sample_size *
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
), cudaMemcpyDeviceToHost);
        cudaStatus = cudaMemcpy(temp_roi, roi, roi_size *
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
), cudaMemcpyDeviceToHost);
        sample = temp_sample;
        roi = temp_roi;
    }
```
```python
int
```
```python
out_size =
```
```python
3
```
```python
;
```
```python
cout
```
```python
<<
```
```python
"val1: "
```
```python
<< endl;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < out_size; i++)
```
```python
cout
```
```python
<< sample[i] <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;
```
```python
float
```
```python
*res =
```
```python
new
```
```python
float
```
```python
[out_size];
```
```python
cout
```
```python
<<
```
```python
"val2: "
```
```python
<< endl;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < out_size; i++)
    {
        res[i] =
```
```python
0
```
```python
;
```
```python
// initial its values to zeros
```
```python
for
```
```python
(
```
```python
int
```
```python
j =
```
```python
0
```
```python
; j < nbox; j++)
        {
            res[i] += roi[i + j*volume];
        }
```
```python
cout
```
```python
<< res[i] <<
```
```python
" "
```
```python
;
    }
```
```python
cout
```
```python
<< endl;
```
```python
delete
```
```python
res;
```
```python
if
```
```python
(status_GPU)
    {
```
```python
delete
```
```python
temp_sample;
```
```python
delete
```
```python
temp_roi;
    }
```
```python
cout
```
```python
<<
```
```python
"backward..."
```
```python
<< endl;
    net->Backward();
```
```python
// backward once
```
```python
cout
```
```python
<<
```
```python
"start check the partial result for backward.... "
```
```python
<< endl;
```
```python
const
```
```python
float
```
```python
* top_diff = NULL;
```
```python
const
```
```python
float
```
```python
* bottom_diff = NULL;
```
```python
if
```
```python
(status_GPU)
    {
        bottom_diff = net->blob_by_name(
```
```python
"roi_pool5"
```
```python
)->gpu_diff();
        top_diff = net->blob_by_name(
```
```python
"sampleadd5"
```
```python
)->gpu_diff();
```
```python
// check the data in cpu == gpu?
```
```python
const
```
```python
float
```
```python
* deb_bottom = net->blob_by_name(
```
```python
"roi_pool5"
```
```python
)->cpu_diff();
```
```python
const
```
```python
float
```
```python
* deb_top = net->blob_by_name(
```
```python
"sampleadd5"
```
```python
)->cpu_diff();
```
```python
// print the first-10 data in cpu
```
```python
cout
```
```python
<<
```
```python
"The data in cpu: "
```
```python
<< endl;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i <
```
```python
10
```
```python
; i++)
```
```python
cout
```
```python
<< *deb_bottom <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;
```
```python
float
```
```python
* temp_top =
```
```python
new
```
```python
float
```
```python
[sample_size];
```
```python
float
```
```python
* temp_bottom =
```
```python
new
```
```python
float
```
```python
[roi_size];
```
```python
// copy gpu data to cpu
```
```python
cudaStatus = cudaMemcpy(temp_top, top_diff, sample_size *
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
), cudaMemcpyDeviceToHost);
        cudaStatus = cudaMemcpy(temp_bottom, bottom_diff, roi_size *
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
), cudaMemcpyDeviceToHost);
```
```python
//print the first-10 data in gpu
```
```python
cout
```
```python
<<
```
```python
"The data in gpu: "
```
```python
<< endl;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i <
```
```python
10
```
```python
; i++)
```
```python
cout
```
```python
<< *temp_bottom <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;

        bottom_diff = temp_bottom;
        top_diff = temp_top;
    }
```
```python
else
```
```python
{
        bottom_diff = net->blob_by_name(
```
```python
"roi_pool5"
```
```python
)->cpu_diff();
        top_diff = net->blob_by_name(
```
```python
"sampleadd5"
```
```python
)->cpu_diff();
    }
```
```python
cout
```
```python
<<
```
```python
"the values below should be the same:"
```
```python
<< endl;
```
```python
cout
```
```python
<<
```
```python
"diff1: "
```
```python
<< endl;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < out_size; i++)
```
```python
cout
```
```python
<< top_diff[i] <<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<< endl;
```
```python
cout
```
```python
<<
```
```python
"diff2: "
```
```python
<< endl;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < out_size; i++)
```
```python
cout
```
```python
<< bottom_diff[i +
```
```python
0
```
```python
* volume] <<
```
```python
" "
```
```python
<< bottom_diff[i +
```
```python
1
```
```python
* volume] <<
```
```python
" "
```
```python
<< bottom_diff[i +
```
```python
2
```
```python
* volume] << endl;
```
```python
return
```
```python
0
```
```python
;
}
```
如果碰见错误：
`math_functions.cpp:91] Check failed: error == cudaSuccess (77 vs. 0)`
则设置batch_size小一些。网上对于这个错误众说纷纭，也不知道到底是不是内存不足引起的。难道是自己写的.cu文件不太合理？这是很有可能的。
对于gpu代码的调试，传统的设置断点跟踪的时候不会给出其内部的值，因为相关知识薄弱，采用笨方法调试，将产生的GPU数据拷贝到CPU上，即通过如下的方式：
```python
bool
```
```python
cudaStatus;
cudaStatus = cudaMemcpy(out_result, out_p,
```
```python
1
```
```python
*
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
), cudaMemcpyDeviceToHost);
cudaStatus = cudaMemcpy(in_result, in_p,
```
```python
1
```
```python
*
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
), cudaMemcpyDeviceToHost);
```
### 其他注意事项
如果运行过程中提示未知的层类型（自己定义的），则可能需要新建一个头文件
如caffe_new_head.hpp
```python
#include<caffe/common.hpp>
```
```python
#include<caffe/fast_rcnn_layers.hpp>
```
```python
#include"sample_add_layer.hpp"
```
```python
#include"my_softmax_layer.hpp"
```
```python
namespace
```
```python
caffe
```
```python
{
```
```python
extern INSTANTIATE_CLASS(ROIPoolingLayer);
    extern INSTANTIATE_CLASS(SampleAddLayer);
    extern INSTANTIATE_CLASS(MySoftmaxWithLossLayer);
}
```
在写cpp代码的时候，即使一些函数还没有去实现（具体的实现）也要加上实例化类和注册层类，这样就可以通过caffe的初始化检测，并且可以通过送入数据的方式来调试一些函数是否写正确。
```python
NSTANTIATE_CLASS(MySoftmaxWithLossLayer)
```
```python
;
```
```python
REGISTER_LAYER_CLASS(MySoftmaxWithLoss)
```
```python
;
```

