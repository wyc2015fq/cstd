# ncnn载入108点关键点检测模型 - sinat_31425585的博客 - CSDN博客
2019年04月08日 20:04:58[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：18
最近发现一款强大的开源部署框架，左庆博士开源的ZQCNN：[https://github.com/zuoqing1988/ZQCNN](https://github.com/zuoqing1988/ZQCNN)
发现里面有个108个点的人脸关键点检测模型效果特别好，就将其对应mxnet模型转换成ncnn对应模型，测试代码如下：
```cpp
#include <iostream>
#include "opencv2/opencv.hpp"
#include "ncnn/net.h"
int main(int argc, char* argv[]) {
    cv::Mat img_src = cv::imread("test.jpg");
    if (img_src.empty()) {
        std::cout << "the input image is empty." << std::endl;
        return -1;
    }
    cv::resize(img_src, img_src, cv::Size(480, 480));
    ncnn::Net net;
    if (net.load_param("landmark.param") == -1 ||
        net.load_model("landmark.bin") == -1) {
        std::cout << "load model failed." << std::endl;
        return -1;
    }
    ncnn::Extractor ex = net.create_extractor();
    ncnn::Mat ncnn_in = ncnn::Mat::from_pixels_resize(img_src.data,
        ncnn::Mat::PIXEL_BGR, img_src.cols, img_src.rows, 48, 48);
    const float meanVals[3] = { 127.5f, 127.5f, 127.5f };
    const float normVals[3] = { 0.0078125f, 0.0078125f, 0.0078125f };
    ncnn_in.substract_mean_normalize(meanVals, normVals);
    ex.input("data", ncnn_in);
    ncnn::Mat ncnn_out;
    ex.extract("bn6_3", ncnn_out);
    std::cout << "channels: " << ncnn_out.w << std::endl;
    for (int i = 0; i < 106; ++i) {
        std::cout << "ncnn out: " << ncnn_out[2 * i] << " " << ncnn_out[2 * i + 1] << std::endl;
        float x = abs(ncnn_out[2 * i] * img_src.cols);
        float y = abs(ncnn_out[2 * i + 1]  * img_src.rows);
        cv::Point curr_pt = cv::Point(x, y);
        cv::circle(img_src, curr_pt, 4, cv::Scalar(0, 255, 0), 2);
    }
    cv::imshow("result", img_src);
    cv::waitKey(0);
    return 0;
}
```
**效果如下：**
![](https://img-blog.csdnimg.cn/20190408200337564.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1,size_16,color_FFFFFF,t_70)
**参考资料：**
[**[1] **https://github.com/zuoqing1988/ZQCNN/issues/50](https://github.com/zuoqing1988/ZQCNN/issues/50)
