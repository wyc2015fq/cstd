# OpenCV实现图像连通组件标记与分析 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2018年06月12日 10:54:46[gloomyfish](https://me.csdn.net/jia20003)阅读数：3215








## OpenCV实现图像连通组件标记与分析

### 一：连通组件标记算法介绍

连接组件标记算法(connected component labeling algorithm)是图像分析中最常用的算法之一，算法的实质是扫描一幅图像的每个像素，对于像素值相同的分为相同的组(group),最终得到图像中所有的像素连通组件。扫描的方式可以是从上到下，从左到右，对于一幅有N个像素的图像来说，最大连通组件个数为N/2。扫描是基于每个像素单位，对于二值图像而言，连通组件集合可以是V={1|白色}或者V={0|黑色}, 取决于前景色与背景色的不同。对于灰度图像来说，连图组件像素集合可能是一系列在0 ～ 255之间k的灰度值。常见的连通组件标记算法非为两类 

1. 基于无向图搜索递归算法 

2. 基于无向图搜索与堆栈非递归算法 

3. 两步法，基于扫描与等价类合并算法
### 二：OpenCV中连通组件标记API

OpenCV中支持连通组件扫描的API有两个，一个是带统计信息一个不带统计信息，不带统计信息的API及其解释如下：

```
int cv::connectedComponents(
InputArray  image, // 输入二值图像，黑色背景
OutputArray     labels, // 输出的标记图像，背景index=0
int     connectivity = 8, // 连通域，默认是8连通
int     ltype = CV_32S // 输出的labels类型，默认是CV_32S
)
```

带有统计信息的API及其解释如下：

```
int cv::connectedComponentsWithStats(
InputArray  image, // 输入二值图像，黑色背景
OutputArray     labels, // 输出的标记图像，背景index=0
OutputArray     stats, // 统计信息，包括每个组件的位置、宽、高与面积
OutputArray     centroids, // 每个组件的中心位置坐标cx, cy
int     connectivity, // 寻找连通组件算法的连通域，默认是8连通
int     ltype, // 输出的labels的Mat类型CV_32S
int     ccltype // 连通组件算法
)
```

其中stats包括以下枚举类型数据信息： 

CC_STAT_LEFT 

组件的左上角点像素点坐标的X位置. 

CC_STAT_TOP 

组件的左上角点像素点坐标的Y位置. 

CC_STAT_WIDTH 

组件外接矩形的宽度 

CC_STAT_HEIGHT 

组件外接矩形的高度. 

CC_STAT_AREA 

当前连通组件的面积（像素单位）
三：代码演示 

基于两个API分别进行了代码演示，选取了一张比较典型的大米图像， 灰度与二值化之后，分别使用这两个连通组件算法API对其进行分析，最终得到以下输出的连通组件的统计信息： 

- 大米的数目 

- 面积 

- 外接矩形大小 

- 中心位置 

原图如下： 
![这里写图片描述](https://img-blog.csdn.net/20180612104813569?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYTIwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

二值图像如下： 
![这里写图片描述](https://img-blog.csdn.net/20180612104825806?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYTIwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
连通组件寻找结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20180612104836740?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYTIwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
连通组件寻找+统计分析的结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20180612104847480?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYTIwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
控制台输出参数如下： 
![这里写图片描述](https://img-blog.csdn.net/20180612105022354?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYTIwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
源代码如下：

```cpp
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

RNG rng(12345);
void connected_component_demo(Mat &image);
void connected_component_stats_demo(Mat &image);
int main(int argc, char** argv) {
    Mat src = imread("D:/javaopencv/rice.png");
    if (src.empty()) {
        printf("could not load image...\n");
    }
    imshow("input", src);
    connected_component_stats_demo(src);

    waitKey(0);
    return 0;
}

void connected_component_demo(Mat &image) {
    // 二值化
    Mat gray, binary;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // 形态学操作
    Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    morphologyEx(binary, binary, MORPH_OPEN, k);
    morphologyEx(binary, binary, MORPH_CLOSE, k);
    imshow("binary", binary);
    imwrite("D:/ccla_binary.png", binary);
    Mat labels = Mat::zeros(image.size(), CV_32S);
    int num_labels = connectedComponents(binary, labels, 8, CV_32S);
    printf("total labels : %d\n", (num_labels - 1));
    vector<Vec3b> colors(num_labels);

    // background color
    colors[0] = Vec3b(0, 0, 0);

    // object color
    for (int i = 1; i < num_labels; i++) {
        colors[i] = Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
    }

    // render result
    Mat dst = Mat::zeros(image.size(), image.type());
    int w = image.cols;
    int h = image.rows;
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            int label = labels.at<int>(row, col);
            if (label == 0) continue;
            dst.at<Vec3b>(row, col) = colors[label];
        }
    }
    imshow("ccla-demo", dst);
    imwrite("D:/ccla_dst.png", dst);
}

void connected_component_stats_demo(Mat &image) {
    // 二值化
    Mat gray, binary;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // 形态学操作
    Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    morphologyEx(binary, binary, MORPH_OPEN, k);
    morphologyEx(binary, binary, MORPH_CLOSE, k);
    imshow("binary", binary);
    Mat labels = Mat::zeros(image.size(), CV_32S);
    Mat stats, centroids;
    int num_labels = connectedComponentsWithStats(binary, labels, stats, centroids, 8, 4);
    printf("total labels : %d\n", (num_labels - 1));
    vector<Vec3b> colors(num_labels);

    // background color
    colors[0] = Vec3b(0, 0, 0);

    // object color
    int b = rng.uniform(0, 256);
    int g = rng.uniform(0, 256);
    int r = rng.uniform(0, 256);
    for (int i = 1; i < num_labels; i++) {
        colors[i] = Vec3b(0, 255, 0);
    }

    // render result
    Mat dst = Mat::zeros(image.size(), image.type());
    int w = image.cols;
    int h = image.rows;
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            int label = labels.at<int>(row, col);
            if (label == 0) continue;
            dst.at<Vec3b>(row, col) = colors[label];
        }
    }

    for (int i = 1; i < num_labels; i++) {
        Vec2d pt = centroids.at<Vec2d>(i, 0);
        int x = stats.at<int>(i, CC_STAT_LEFT);
        int y = stats.at<int>(i, CC_STAT_TOP);
        int width = stats.at<int>(i, CC_STAT_WIDTH);
        int height = stats.at<int>(i, CC_STAT_HEIGHT);
        int area = stats.at<int>(i, CC_STAT_AREA);
        printf("area : %d, center point(%.2f, %.2f)\n", area, pt[0], pt[1]);
        circle(dst, Point(pt[0], pt[1]), 2, Scalar(0, 0, 255), -1, 8, 0);
        rectangle(dst, Rect(x, y, width, height), Scalar(255, 0, 255), 1, 8, 0);
    }
    imshow("ccla-demo", dst);
    imwrite("D:/ccla_stats_dst.png", dst);
}
```

***  欢迎关注本博客，欢迎关注本人微信公众号【OpenCV学堂】  ***
**谢绝转载，转载请取得本人同意！**









