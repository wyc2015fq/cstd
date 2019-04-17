# 提取caffe和ncnn的权重 - zqjackking的博客 - CSDN博客





2017年10月12日 21:00:32[zqjackking](https://me.csdn.net/zqjackking)阅读数：2437








之前用腾讯的ncnn的框架做灰度图分类，发现caffe和ncnn的结果不一致。 

然后试着将caffe的caffemodel和ncnn的bin文件里面的权重提取出来。 

1  提取caffemodel里的参数。
```
#!/usr/bin/env python

# 引入“咖啡”
import caffe

import numpy as np

# 使输出的参数完全显示
# 若没有这一句，因为参数太多，中间会以省略号“……”的形式代替
np.set_printoptions(threshold='nan')

# deploy文件
MODEL_FILE = 'caffe_deploy.prototxt'
# 预先训练好的caffe模型
PRETRAIN_FILE = 'caffe_iter_10000.caffemodel'

# 保存参数的文件
params_txt = 'params.txt'
pf = open(params_txt, 'w')

# 让caffe以测试模式读取网络参数
net = caffe.Net(MODEL_FILE, PRETRAIN_FILE, caffe.TEST)

# 遍历每一层
for param_name in net.params.keys():
    # 权重参数
    weight = net.params[param_name][0].data
    # 偏置参数
    bias = net.params[param_name][1].data

    # 该层在prototxt文件中对应“top”的名称
    pf.write(param_name)
    pf.write('\n')

    # 写权重参数
    pf.write('\n' + param_name + '_weight:\n\n')
    # 权重参数是多维数组，为了方便输出，转为单列数组
    weight.shape = (-1, 1)

    for w in weight:
        pf.write('%ff, ' % w)

    # 写偏置参数
    pf.write('\n\n' + param_name + '_bias:\n\n')
    # 偏置参数是多维数组，为了方便输出，转为单列数组
    bias.shape = (-1, 1)
    for b in bias:
        pf.write('%ff, ' % b)

    pf.write('\n\n')

pf.close
```

代码比较简单。就是构建一个网络，然后将里面所有层的权重和bias（如果有参数的话）提出来存在txt里面。 

2 提取ncnn的bin文件里面的参数 

ncnn没有python接口，必须要用C++，所以就稍微麻烦一点。
```
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "net.h"
#include "layer.h"
#include "layer/convolution.h"
#include "layer/innerproduct.h"
#include "mat.h"
using namespace std;
void lenet()
{
    ncnn::Net lenet;
    lenet.load_param("new_province.param");
    lenet.load_model("new_province.bin");
    //int layer_index;
    //layer_index = lenet.find_layer_index_by_name("conv1");
    //find_layer_index_by_name这个函数式protected的，要想使用就要规避
    ncnn::Convolution* conv1 = (ncnn::Convolution*)lenet.extract_layer(1);
    ncnn::Convolution* conv2 = (ncnn::Convolution*)lenet.extract_layer(3);
    ncnn::InnerProduct* fc1 = (ncnn::InnerProduct*)lenet.extract_layer(5);
    ncnn::InnerProduct* fc2 = (ncnn::InnerProduct*)lenet.extract_layer(7);
    ncnn::Mat conv1_weight; 
    ncnn::Mat conv2_weight;
    ncnn::Mat fc1_weight;
    ncnn::Mat fc2_weight;
    conv1_weight = conv1->weight_data;
    conv2_weight = conv2->weight_data;
    fc1_weight = fc1->weight_data;
    fc2_weight = fc2->weight_data;
    int i;
    ofstream fout("fc1.txt");
    for(i=0;i<500;i++)
    {
        fout << fc1_weight.data[i] << endl;
    }

}

int main()
{
    lenet();
    return 0;
}
```

此处顺便粘贴上CMakeLists.txt.

```
find_package(OpenCV REQUIRED core highgui imgproc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../src)
add_executable(squeezenet squeezenet.cpp)
target_link_libraries(squeezenet ncnn ${OpenCV_LIBS})
```

有几个问题， 

（1）CMakeLists.txt里面的include_directories包含了src目录。mat.h,layer.h,net.h都在这里面，但是convolution.h和innerproduct.h都在layer子文件夹里。CMakeLists.txt里不用再添加layer子文件夹，但是在include的时候要加上layer子文件夹。 

（2）此处思路还是构建一个网络（名字叫lenet），lenet是一个net对象，但是net对象的std::vector
```
transform_param {
    scale: 0.00390625
  }
```

这个数字代表1/256，是将像素值从[0,256)缩放到[0,1)。 

还有一种处理方法是，将所有像素值减去127.5，然后除以128,这是为了将像素值缩放到(-1,1) 

所以我在ncnn里将像素值也缩放到[0,1),
```
const float mean_vals[1] = {0};
const float norm_vals[1] = {0.00390625f};
in.substract_mean_normalize(mean_vals,norm_vals);
```

发现这样也不能解决问题，缩放对结果并没有影响。唯一的不同是：不缩放的话，最后softmax分类的概率第一个为1，其他的都是0。而如果缩放到[0,1)，第1个并达不到1（比如1=0.76,3=0.14,6=0.1）。所以并不是这里的问题。 

4 分析可能是resize的问题。因为ncnn是用的自己的resize的方法，而不是调用的opencv的resize（caffe是调用的这个）。 

解决思路。对于同一张图片，分别用opencv和ncnn resize一下，并且将其每个像素值打出来。 

首先用caffe  resize。
```cpp
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
using namespace std;
int main()
{
    int cv_read_flag = 0;
    string filename = "/home/zq/caffe/01232.jpg";
    cv::Mat cv_img_origin = cv::imread(filename,cv_read_flag);
    cv::Mat cv_img;
    cv::resize(cv_img_origin,cv_img,cv::Size(32,32));
    ofstream fout("01232.txt");
    for(int i=0;i<cv_img.rows;i++)
    {
        for(int j=0;j<cv_img.cols;j++)
        {
            uchar val = cv_img.at<uchar>(i,j);
            fout << (int)val << endl;
        }
    }
    fout.close();
    return 0;
}
```

以上代码来自[http://blog.csdn.net/moc062066/article/details/6949826](http://blog.csdn.net/moc062066/article/details/6949826)

最后发现是ncnn resize的问题。使用cv::imread的时候，一定要注意flag要与原图一致。如果是灰度图就是0，三通道的就是1。








