# ncnn 载入insightface训练好的性别年龄识别模型 - sinat_31425585的博客 - CSDN博客
2019年03月28日 21:33:14[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：57
个人分类：[ncnn](https://blog.csdn.net/sinat_31425585/article/category/8720223)
**1、模型转换**
从insightface项目中下载mxnet模型：[https://github.com/deepinsight/insightface/tree/master/gender-age/model](https://github.com/deepinsight/insightface/tree/master/gender-age/model)
**2、使用ncnn的模型转换工具mxnet2ncnn进行模型转换**
```cpp
./mxnet2ncnn model-symbol.json model-0000.params ag.param ag.bin
```
**3、使用下面代码测试：**
```cpp
#include "ncnn/net.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <vector>
int main(int argc, char* argv[]) {
    cv::Mat img_src = cv::imread("test.png");
    if (img_src.empty()) {
        std::cout << "input image is empty." << std::endl;
        return -1;
    }
    ncnn::Net net;
    if (net.load_param("ag.param") == -1 ||
        net.load_model("ag.bin") == -1) {
        std::cout << "load ga model failed." << std::endl;
        return -1;
    }
    ncnn::Extractor ex = net.create_extractor();
    ncnn::Mat img_ncnn = ncnn::Mat::from_pixels_resize(img_src.data,
        ncnn::Mat::PIXEL_BGR, img_src.cols, img_src.rows, 112, 112);
    ex.input("data", img_ncnn);
    ncnn::Mat img_out;
    ex.extract("fc1", img_out);
    std::vector<float> out;
    for (int i = 0; i < img_out.w; ++i) {
        out.push_back(img_out[i]);
    }
    if (out[0] > out[1]) {
        std::cout << "female" << std::endl;
    } else {
        std::cout << "male" << std::endl;
    }
    int counts = 0;
    for (int i = 2; i < 102; ++i) {
        if (out[2 * i] < out[2 * i + 1]) {
            ++counts;
        }
    }
    std::cout << "age: " << counts << std::endl;
    return 0;
}
```
**参考资料：**
[1] [https://github.com/Tencent/ncnn](https://github.com/Tencent/ncnn)
[2] [https://github.com/deepinsight/insightface](https://github.com/deepinsight/insightface)
