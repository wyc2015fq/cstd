
# C++ 读取 .mat 文件 - 机器学习的小学生 - CSDN博客


2018年03月09日 19:01:25[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：530



```python
#define CPU_ONLY
```
```python
#include<vector>
```
```python
#include<iostream>
```
```python
#include<caffe/caffe.hpp>
```
```python
#include<opencv2/opencv.hpp>
```
```python
#include "caffe_reg.h"
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
caffe;
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
Mat read_mat_file(
```
```python
string
```
```python
file_path,
```
```python
int
```
```python
cross_id,
```
```python
bool
```
```python
isShow)
{
    MATFile *mean_pmat = NULL;
```
```python
// .mat file for mean_file
```
```python
mxArray *mean_var_cell = NULL;
```
```python
// mean variable in .mat file
```
```python
MATFile *std_pmat = NULL;
    mxArray *std_var_cell = NULL;
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
// variable names in .mat file
```
```python
int
```
```python
nvars;
```
```python
// the number of variables in .mat file
```
```python
// read .mat file
```
```python
mean_pmat = matOpen(file_path.c_str(),
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
(mean_pmat == NULL)
    {
```
```python
cout
```
```python
<<
```
```python
"cannot open *.mat file!"
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
**)matGetDir(mean_pmat, &nvars);
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
```
```python
cout
```
```python
<<
```
```python
" The name for all variables:"
```
```python
;
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
<<
```
```python
string
```
```python
(var_names[i]) << endl;
```
```python
// get the "mean_file_cross" ( a cell) variable from .mat file
```
```python
// because we only have one variable in .mat file , so the index of var_names is set as 0
```
```python
mean_var_cell = matGetVariable(mean_pmat, var_names[
```
```python
0
```
```python
]);
```
```python
// get the size of mean_var_cell: M: rows, N :cols
```
```python
int
```
```python
mean_cell_M = (
```
```python
int
```
```python
)mxGetM(mean_var_cell);
```
```python
int
```
```python
mean_cell_N = (
```
```python
int
```
```python
)mxGetN(mean_var_cell);
```
```python
//int cross_id = 1; // the cross_id for cross validation experiments
```
```python
// mxGetCell(cell_name,count) , "count" is a accumuative number , column as main direction
```
```python
// The above statement should be further checked.
```
```python
mxArray * mean_file = mxGetCell(mean_var_cell, cross_id -
```
```python
1
```
```python
);
```
```python
// start from 0
```
```python
cout
```
```python
<<
```
```python
" The dimension of mean_file: "
```
```python
<< (
```
```python
int
```
```python
)mxGetNumberOfDimensions(mean_file) << endl;
```
```python
double
```
```python
*data = (
```
```python
double
```
```python
*)mxGetData(mean_file);
```
```python
// get data from mxArray file
```
```python
// mean_file is color image ,so mxGetM, mxGetN  is instead by mxGetDimensios,
```
```python
const
```
```python
size_t * dims = mxGetDimensions(mean_file);
```
```python
int
```
```python
M = dims[
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
N = dims[
```
```python
1
```
```python
];
```
```python
int
```
```python
C = dims[
```
```python
2
```
```python
];
```
```python
double
```
```python
*ptr_data = (
```
```python
double
```
```python
*)mxGetPr(mean_file);
```
```python
cout
```
```python
<<
```
```python
"The shape of mean_file: "
```
```python
<<
```
```python
"("
```
```python
<< M <<
```
```python
","
```
```python
<< N <<
```
```python
","
```
```python
<< C <<
```
```python
")"
```
```python
<< endl;
    Mat mean_file_mat;
```
```python
if
```
```python
(mxGetNumberOfDimensions(mean_file) ==
```
```python
3
```
```python
)
    {
        mean_file_mat.create(M, N, CV_8UC3);
    }
```
```python
else
```
```python
{
        mean_file_mat.create(M, N, CV_8UC1);
    }
    size_t subs[
```
```python
3
```
```python
];
```
```python
// note that:  row,cols,c
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
; i < M; i++)
    {
        subs[
```
```python
0
```
```python
] = i;
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
; j < N; j++)
        {
            subs[
```
```python
1
```
```python
] = j;
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
k =
```
```python
0
```
```python
; k < C; k++)
            {
                subs[
```
```python
2
```
```python
] = k;
```
```python
int
```
```python
index = mxCalcSingleSubscript(mean_file,
```
```python
3
```
```python
, subs);
                mean_file_mat.at<Vec3b>(subs[
```
```python
0
```
```python
], subs[
```
```python
1
```
```python
])[k] = ptr_data[index];
            }
        }
    }
```
```python
// matlab: RGB  opencv: BGR
```
```python
Mat mean_file_img;
    cvtColor(mean_file_mat, mean_file_img, CV_RGB2BGR);
```
```python
// show for debug
```
```python
if
```
```python
(isShow)
    {
        imshow(file_path, mean_file_img);
        waitKey(
```
```python
0
```
```python
);
    }
```
```python
return
```
```python
mean_file_img;
}
```
```python
// 共享内存，仅用来演示
```
```python
void
```
```python
check_warp_op()
{
```
```python
// change the values in p-pointer, the values in mat will also changes
```
```python
// They share the same memory.
```
```python
float
```
```python
* p =
```
```python
new
```
```python
float
```
```python
[
```
```python
10
```
```python
];
```
```python
cout
```
```python
<<
```
```python
"before warp: "
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
    {
        p[i] = i;
```
```python
cout
```
```python
<< p[i] <<
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
    Mat wap_mat(
```
```python
1
```
```python
,
```
```python
10
```
```python
, CV_32FC1, p);
    wap_mat.at<
```
```python
float
```
```python
>(
```
```python
0
```
```python
,
```
```python
1
```
```python
) =
```
```python
10
```
```python
;
    wap_mat.at<
```
```python
float
```
```python
>(
```
```python
0
```
```python
,
```
```python
2
```
```python
) =
```
```python
10
```
```python
;
    wap_mat.at<
```
```python
float
```
```python
>(
```
```python
0
```
```python
,
```
```python
3
```
```python
) =
```
```python
10
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
"mat: "
```
```python
<< endl;
```
```python
cout
```
```python
<< wap_mat << endl;
```
```python
cout
```
```python
<<
```
```python
"after warp: "
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
    {
```
```python
cout
```
```python
<< p[i] <<
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

}
```
```python
/* Return the indices of the top N values of vector v. */
```
```python
// Argmax function and Paricompare(weici) are copied from caffe example
```
```python
static
```
```python
bool
```
```python
PairCompare(
```
```python
const
```
```python
std
```
```python
::pair<
```
```python
float
```
```python
,
```
```python
int
```
```python
>& lhs,
```
```python
const
```
```python
std
```
```python
::pair<
```
```python
float
```
```python
,
```
```python
int
```
```python
>& rhs) {
```
```python
return
```
```python
lhs.first > rhs.first;
}
```
```python
static
```
```python
std
```
```python
::
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
Argmax(
```
```python
const
```
```python
std
```
```python
::
```
```python
vector
```
```python
<
```
```python
float
```
```python
>
```
```python
& v,
```
```python
int
```
```python
N) {
```
```python
std
```
```python
::
```
```python
vector
```
```python
<
```
```python
std
```
```python
::pair<
```
```python
float
```
```python
,
```
```python
int
```
```python
>
```
```python
> pairs;
```
```python
for
```
```python
(size_t i =
```
```python
0
```
```python
; i < v.size(); ++i)
        pairs.push_back(
```
```python
std
```
```python
::make_pair(v[i],
```
```python
static_cast
```
```python
<
```
```python
int
```
```python
>(i)));
```
```python
std
```
```python
::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), PairCompare);
```
```python
std
```
```python
::
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
result;
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
; i < N; ++i)
        result.push_back(pairs[i].second);
```
```python
return
```
```python
result;
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
"ZOO_VGG16/deploy.prototxt"
```
```python
;
```
```python
string
```
```python
pre_trained_weight =
```
```python
"ZOO_VGG16/vgg__iter_10000.caffemodel"
```
```python
;
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
    net->CopyTrainedLayersFrom(pre_trained_weight);
```
```python
cout
```
```python
<<
```
```python
"number of input and output:"
```
```python
;
```
```python
cout
```
```python
<< net->num_inputs() <<
```
```python
" "
```
```python
<< net->num_outputs() << endl;
```
```python
// mean_file
```
```python
// std_file
```
```python
string
```
```python
lfw_data_path =
```
```python
"E:/xlh/eclipse_workplace/hello_world/data/lfw/"
```
```python
;
```
```python
string
```
```python
mean_file_path = lfw_data_path +
```
```python
"mean_file_cross224_10fold.mat"
```
```python
;
```
```python
string
```
```python
std_file_path = lfw_data_path +
```
```python
"std_file_cross224_10fold.mat"
```
```python
;
```
```python
string
```
```python
one_img = lfw_data_path +
```
```python
"/imgs224/Aaron_Eckhart_0001.jpg"
```
```python
;
```
```python
int
```
```python
cross_id =
```
```python
1
```
```python
;
    Mat mean_file = read_mat_file(mean_file_path, cross_id,
```
```python
false
```
```python
);
    Mat std_file = read_mat_file(std_file_path, cross_id,
```
```python
false
```
```python
);
    Mat input_img = imread(one_img);
```
```python
// only for CPU
```
```python
Caffe::set_mode(Caffe::CPU);
    Mat norm_img;
```
```python
//H W C(brg)
```
```python
cv::subtract(input_img, mean_file, norm_img);
    cv::divide(norm_img, std_file, norm_img);
```
```python
//
```
```python
bool
```
```python
isShow =
```
```python
false
```
```python
;
```
```python
// all the values are very small, so is black
```
```python
if
```
```python
(isShow)
    {
        Mat show_img;
        norm_img.convertTo(show_img, CV_8UC3);
        imshow(
```
```python
"normalize"
```
```python
, show_img);
        waitKey(
```
```python
0
```
```python
);
    }
```
```python
//
```
```python
Blob<
```
```python
float
```
```python
> * input_layer = net->input_blobs()[
```
```python
0
```
```python
];
```
```python
// or Blob<float> * input_layer = net->blobs["data"];
```
```python
int
```
```python
num_channels = input_layer->channels();
```
```python
cout
```
```python
<<
```
```python
"channels: "
```
```python
<< num_channels << endl;
```
```python
int
```
```python
npose =
```
```python
5
```
```python
;
```
```python
// for lfw dataset
```
```python
Blob<
```
```python
float
```
```python
> *output_layer = net->output_blobs()[
```
```python
0
```
```python
];
```
```python
cout
```
```python
<<
```
```python
"label size: "
```
```python
<< npose <<
```
```python
"; num_channels of output: "
```
```python
<< output_layer->channels() << endl;
```
```python
//check_warp_op(); a faster method ,dont need copy
```
```python
// for details, please refer to classification.cpp in caffe
```
```python
cout
```
```python
<<
```
```python
"input_layer.shape: "
```
```python
;
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
; i < input_layer->shape().size(); i++)
    {
```
```python
cout
```
```python
<< input_layer->shape()[i] <<
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
// an idiot method
```
```python
// input_blob.shape: [1,3,224,224]; nsample *  channels * height * width
```
```python
// input_blob.shape: [3,224,224];
```
```python
float
```
```python
* data = input_layer->mutable_cpu_data();
```
```python
int
```
```python
nrows = input_layer->shape()[
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
ncols = input_layer->shape()[
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
nchannels = input_layer->shape()[
```
```python
1
```
```python
];
```
```python
int
```
```python
num =
```
```python
0
```
```python
;
```
```python
float
```
```python
*p = data;
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
; i < nchannels; i++)
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
j =
```
```python
0
```
```python
; j < nrows; j++)
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
k =
```
```python
0
```
```python
; k < ncols; k++)
            {
                p[num] = norm_img.at<Vec3b>(j, k)[i];
                num +=
```
```python
1
```
```python
;
            }
        }
    }
    net->Forward();
```
```python
//
```
```python
const
```
```python
float
```
```python
* begin = output_layer->cpu_data();
```
```python
const
```
```python
float
```
```python
* end = begin + output_layer->channels();
```
```python
vector
```
```python
<
```
```python
float
```
```python
>
```
```python
output(begin, end);
```
```python
cout
```
```python
<<
```
```python
"the probabilites for each class: "
```
```python
;
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
; i < output.size(); i++)
    {
```
```python
cout
```
```python
<< output[i] <<
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
class_id = Argmax(output, npose);
```
```python
// left profile, left ,front ,right ,right profile
```
```python
cout
```
```python
<<
```
```python
"predict label is : "
```
```python
<< class_id[
```
```python
0
```
```python
] -
```
```python
2
```
```python
<< endl;
```
```python
// change from[0,4] to [-2,2]
```
```python
// free memory
```
```python
delete
```
```python
(net);
}
```
参考文献：
1.[http://blog.csdn.net/jnulzl/article/details/49623121](http://blog.csdn.net/jnulzl/article/details/49623121)
2.[http://blog.csdn.net/shaoxiaohu1/article/details/8269690](http://blog.csdn.net/shaoxiaohu1/article/details/8269690)

