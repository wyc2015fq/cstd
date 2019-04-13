
# Caffe: create a simple sin layer - 机器学习的小学生 - CSDN博客


2018年03月10日 15:40:23[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：101个人分类：[Caffe																](https://blog.csdn.net/xuluhui123/article/category/6777566)



```python
// test new_creat layer
```
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
wrapInputLayer(Net<
```
```python
float
```
```python
>* net,
```
```python
vector
```
```python
<Mat>
```
```python
* input_channels)
{
    Blob<
```
```python
float
```
```python
>* input_layer = net->input_blobs()[
```
```python
0
```
```python
];
```
```python
int
```
```python
width = input_layer->width();
```
```python
int
```
```python
height = input_layer->height();
```
```python
float
```
```python
*input_data = input_layer->mutable_cpu_data();
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
; i < input_layer->channels(); ++i)
    {
        Mat channel(height, width, CV_32FC1, input_data);
        input_channels->push_back(channel);
        input_data += width * height;
    }
}
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
// for test
```
```python
string
```
```python
deploy_proto =
```
```python
"ZOO_VGG16/deploy_sinlayer.prototxt"
```
```python
;
```
```python
//string pre_trained_weight = "ZOO_VGG16/vgg__iter_10000.caffemodel";
```
```python
Net<
```
```python
float
```
```python
> * net =
```
```python
new
```
```python
Net<
```
```python
float
```
```python
>(deploy_proto, caffe::Phase::TEST,
```
```python
0
```
```python
);
```
```python
// use safe pointer
```
```python
//shared_ptr<Net<float> > net; // warp Net<float> to a pointer
```
```python
//net.reset(new Net<float>(deploy_proto, TEST));
```
```python
//net->CopyTrainedLayersFrom(pre_trained_weight);
```
```python
// create input_data of 1*3*6*6
```
```python
int
```
```python
rows =
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
cols =
```
```python
6
```
```python
;
```
```python
vector
```
```python
<Mat>
```
```python
input_channels;
    wrapInputLayer(net,&input_channels);
```
```python
// bind net->input_data to vector<mat>
```
```python
//
```
```python
input_channels[
```
```python
0
```
```python
] = Mat::ones(rows, cols, CV_32FC1);
    input_channels[
```
```python
1
```
```python
] = Mat::ones(rows, cols, CV_32FC1) *
```
```python
2
```
```python
;
    input_channels[
```
```python
2
```
```python
] = Mat::ones(rows, cols, CV_32FC1) *
```
```python
3
```
```python
;
    net->Forward();
```
```python
const
```
```python
float
```
```python
* data = net->blob_by_name(
```
```python
"data"
```
```python
)->cpu_data();
```
```python
const
```
```python
float
```
```python
*sin_data = net->blob_by_name(
```
```python
"sin"
```
```python
)->cpu_data();
```
```python
int
```
```python
count_input = net->blob_by_name(
```
```python
"data"
```
```python
)->count();
```
```python
int
```
```python
count_output = net->blob_by_name(
```
```python
"sin"
```
```python
)->count();
    assert(count_input == count_output);
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
; i < count_input; i++)
    {
```
```python
cout
```
```python
<< data[i] <<
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
; i < count_output; i++)
    {
```
```python
cout
```
```python
<< sin_data[i] <<
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
cout
```
```python
<<
```
```python
"sin(1): sin(2): sing(3)"
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
sin
```
```python
(
```
```python
1
```
```python
) <<
```
```python
";"
```
```python
<<
```
```python
sin
```
```python
(
```
```python
2
```
```python
) <<
```
```python
";"
```
```python
<<
```
```python
sin
```
```python
(
```
```python
3
```
```python
)<<endl;
```
```python
// output class probability
```
```python
cout
```
```python
<<
```
```python
"The probabilites for each classs: "
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
*prob = net->blob_by_name(
```
```python
"prob"
```
```python
)->cpu_data();
```
```python
int
```
```python
nclass = net->blob_by_name(
```
```python
"prob"
```
```python
)->channels();
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
; i < nclass; i++)
    {
```
```python
cout
```
```python
<< prob[i] <<
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
//free Net
```
```python
}
```
C++ caffe train
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
int
```
```python
main()
{
```
```python
// for  train
```
```python
Caffe::set_mode(Caffe::GPU);
    SolverParameter solver_param;
```
```python
string
```
```python
solver_file =
```
```python
"ZOO_VGG16/solver.prototxt"
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
```
```python
//Sover<float> 是纯虚函数，所有不能实例化对象，通过指针的形式指向子类。
```
```python
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
//net->CopyTrainedLayersFrom(pre_trained_weight);
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
const
```
```python
float
```
```python
* data = net->blob_by_name(
```
```python
"data"
```
```python
)->cpu_data();
```
```python
const
```
```python
float
```
```python
*sin_data = net->blob_by_name(
```
```python
"sin"
```
```python
)->cpu_data();
```
```python
cout
```
```python
<< endl;
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
参考文献：
[https://github.com/BVLC/caffe/wiki/Simple-Example:-Sin-Layer](https://github.com/BVLC/caffe/wiki/Simple-Example:-Sin-Layer)

