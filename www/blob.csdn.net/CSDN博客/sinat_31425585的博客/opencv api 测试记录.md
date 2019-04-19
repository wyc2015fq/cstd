# opencv api 测试记录 - sinat_31425585的博客 - CSDN博客
2018年10月23日 13:42:04[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：142
**  1、仿射变换warpAffine对三通道图像进行变换：**
```cpp
#include <iostream>
#include <opencv2/opencv.hpp>
int main(int arc, char* argv[]) {
    cv::Mat img = cv::imread("test.jpg", 1);
    if(img.empty()) {
        std::cout << "the input is empty..." << std::endl;
    }
    cv::Point center = cv::Point(img.cols / 2, img.rows / 2);
    double angle = 90;
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, 1);
    cv::Mat warped = cv::Mat(img.rows, img.cols, CV_8UC3, cv::Scalar(128, 128, 128));
    cv::warpAffine(img, warped, rot_mat, warped.size());
    cv::imwrite("result.jpg", warped);
    return 0;
}
```
    效果如图1所示：
![](https://img-blog.csdn.net/20181023121217425?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                            图1 仿射变换
**2、获取视频参数**
```cpp
cv::VideoCapture cap;
int width = cap.get(CV_CAP_PROP_FRAME_WIDTH); // width
int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); // height
int frameRate = cap.get(CV_CAP_PROP_FPS); // FPS
int totalFrames = cap.get(CV_CAP_PROP_FRAME_COUNT); // total number of video
```
**3、uchar* 数组转Mat**
```cpp
for(int i = 0; i < src.rows; i++) {
    uchar* data = src.ptr<uchar>(i);
    uchar* p_buffer = arr + i * src.cols * src.channels();
    memcpy(data, p_buffer, src.cols * src.channels());
}
```
**4、标注label**
```cpp
char text[256];
        sprintf(text, "%s %d", objects.at(i).label, objects.at(i).confidence);
        int base_line = 0;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &base_line);
        cv::putText(img_bgr, text, cv::Point(rect.x, rect.y - label_size.height),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
```
**5、颜色检测**
```cpp
cv::inRange(hsv_img, cv::Scalar(h_low, s_low, v_low), cv::Scalar(h_high, s_high, v_high), result);
```
**6、计时程序**
```cpp
double t1 = static_cast<double>(cv::getTickCount());
....
double t2 = static_cast<double>(cv::getTickCount());
double t = (t2 - t1) / (cv::getTickFrequency()) * 1000;
std::cout << "the time cost is: " << t << std::endl;
```
**7、计算轮廓的中心**
```cpp
cv::Moments mu;
mu = cv::moments(contours[i], false)；
cv::Point2f center = cv::Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
```
**8、计算轮廓面积**
```cpp
double area = cv::contourArea(contours[i]);
```
**9、findContours中，轮廓嵌套存储**
hierachy[i][0]：平级轮廓，hierachy[i][1]：前一个轮廓，hierachy[i][2]：子轮廓，hierachy[i][3]：父轮廓。
```cpp
std::vector<std::vector<cv::Point> > contours;
std::vector<cv::Vec4i> hierarchy;
cv::findContours(img_threshold, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));
```
**10、最小外接矩形**
```cpp
cv::RotatedRect box = cv::minAreaRect(cv::Mat(contours[i]));
```
获取最小外接矩形的顶点
```cpp
std::vector<cv::Point2f> points(4);
box.points(&points[0]);
```
**11、删除轮廓**
```cpp
std::vector<std::vector<cv::Point>>::iterator itc= contours.begin()
while(itc != contours.end()) {
    cv::Rect rect = cv::boundingRect(*itc)
    if(...) {
        contours.erase(itc);
    } else {
        itc++;
    }
    
}
```
这种方法最大的好处就在于，不需要考虑删除过程中造成数组大小动态的变化。
**12、反色操作**
```cpp
img = cv::Scalar(255) - img;
```
非常简单明了~
**13、轮廓长度求解**
```cpp
double length = cv::arcLength(contours[i], true);
```
求取轮廓长度，第二个参数为判断轮廓是否闭合，非常有用~
**14、旋转操作**
flip函数
逆时针翻转90度：
```cpp
cv::flip(img, img_90, 0);
```
顺时针翻转90度：
```cpp
cv::flip(img, img_90, 1);
```
逆时针旋转180度
```cpp
cv::flip(img, img_180, -1);
```
**15、形态学处理**
其中形态学操作可以通过如下来进行设置：
```cpp
cv::MORPH_ERODE：腐蚀
cv::MORPH_DILATE：膨胀
cv::MORPH_OPEN：开运算
cv::MORPH_CLOSE：闭运算
cv::MORPH_BLACKHAT：黑帽运算
cv::MORPH_TOPHAT：顶帽运算
```
```cpp
cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
cv::morphologyEx(src, dst, cv::MORPH_ERODE, kernel);
```
**16、mixChannels**
将输入Mat某个通道复制到输出矩阵的某个通道中去
```cpp
CV_EXPORTS void mixChannels(const Mat* src, size_t nsrcs, Mat* dst, size_t ndsts,
                            const int* fromTo, size_t npairs);
```
src为输入矩阵，可以是一个或多个矩阵，nsrcs表示输入矩阵的个数，dst为输出矩阵也可以是一个或多个矩阵，ndsts表示输出矩阵个数，fromTo表示复制通道的关系，例如fromto[] = {0, 1}表示复制输入矩阵的第一个通道到输出矩阵的第二个通道，npairs表示输入输出关系的组别数，如：
```cpp
int ch[] = {1, 0};
cv::mixChannels(&src, 1, &dst, 1, ch, 1);
```
**17、写视频**
  写视频流程：
```cpp
cv::VideoWriter writer;
// 获取读取到视频参数
int width = cam.get(CV_CAP_PROP_FRAME_WIDTH); // width
int height = cam.get(CV_CAP_PROP_FRAME_HEIGHT); // height
int frame_rate = cam.get(CV_CAP_PROP_FPS); // FPS
int total_frames = cam.get(CV_CAP_PROP_FRAME_COUNT); // total number of video
// 打开视频写操作
writer.open("result.avi", CV_FOURCC('M', 'J', 'P', 'G'),
    frame_rate, cv::Size(width, height), true);
// 写入图像
writer << frame;
// 最后不要忘记关闭写操作
writer.release();
```
**   注意：**
1） 如果是读取视频，可以获取FPS，然而如果是直接从摄像头获取的视频流，获取FPS会失败，需要将frame_rate手动设置为定值，才能进行视频读写。
2）如果发现写出来的视频只有几k，需要检查一下，写入Mat的尺寸和设置写视频的size是否匹配；
**18、获取文件夹下图片路径**
使用glob函数，获取文件夹下图片路径，传入第一个参数为数据根目录，第二个参数为获取到的文件路径集，第三个参数为是否遍历子文件夹，true表示遍历，false表示不遍历。
```cpp
std::vector<cv::String> samples;
std::string data_path = "./data";
cv::glob(data_path, samples, false);
```
~~~~~~未完待续~~~~~~
