//基于opencv的sauvola二值算法
//sauvola算法是一个比较不错的局部二值化算法，有不少人也具体实现了其代码，但是本人觉得里面存在一些bug和不方便opencv调用，所以对其适当的修改。 
//但是修改之后，仍然就是存在一些问题，就是难以找到一个比较大的数据类型去存储，这个bug等以后有空再处理。不过通过修改后的代码进行学习，还是很不错的。代码如下：


#include "opencv2/opencv.hpp"

static int CalcMaxValue(int a, int b)
{
    return (a > b) ? a : b;
}

static double CalcMaxValue(double a, double b)
{
    return (a > b) ? a : b;
}

static int CalcMinValue(int a, int b)
{
    return (a < b) ? a : b;
}

static double CalcMinValue(double a, double b)
{
    return (a < b) ? a : b;
}


/** @brief SauvolaThresh二值算法

此代码不适用与分辨率较大的图像, 此bug准备有空再处理

@param src 单通道灰度图
@param dst 单通道处理后的图
@param k  threshold = mean*(1 + k*((std / 128) - 1))
@param wndSize 处理区域宽高, 一定是奇数

*/
void SauvolaThresh(const cv::Mat& src, cv::Mat& dst, const int k, const cv::Size wndSize)
{
    CV_Assert(src.type() == CV_8UC1);
    CV_Assert((wndSize.width % 2 == 1) && (wndSize.height % 2 == 1));
    CV_Assert((wndSize.width <= src.cols) && (wndSize.height <= src.rows));

    dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

    // 产生标志位图像
    unsigned long* integralImg = new unsigned long[src.rows * src.cols];
    unsigned long* integralImgSqrt = new unsigned long[src.rows * src.cols];
    std::memset(integralImg, 0, src.rows *src.cols*sizeof(unsigned long));
    std::memset(integralImgSqrt, 0, src.rows *src.cols*sizeof(unsigned long));

    // 计算直方图和图像值平方的和
    for (int y = 0; y < src.rows; ++y)
    {
        unsigned long sum = 0;
        unsigned long sqrtSum = 0;
        for (int x = 0; x < src.cols; ++x)
        {
            int index = y * src.cols + x;
            sum += src.at<uchar>(y, x);
            sqrtSum += src.at<uchar>(y, x)*src.at<uchar>(y, x);
            if (y == 0)
            {
                integralImg[index] = sum;
                integralImgSqrt[index] = sqrtSum;
            }
            else
            {
                integralImgSqrt[index] = integralImgSqrt[(y - 1)*src.cols + x] + sqrtSum;
                integralImg[index] = integralImg[(y - 1)*src.cols + x] + sum;
            }
        }
    }

    double diff         = 0.0;
    double sqDiff       = 0.0;
    double diagSum      = 0.0;
    double iDiagSum     = 0.0;
    double sqDiagSum    = 0.0;
    double sqIDiagSum   = 0.0;
    for (int x = 0; x < src.cols; ++x)
    {
        for (int y = 0; y < src.rows; ++y)
        {
            int xMin = CalcMaxValue(0, x - wndSize.width / 2);
            int yMin = CalcMaxValue(0, y - wndSize.height / 2);
            int xMax = CalcMinValue(src.cols - 1, x + wndSize.width / 2);
            int yMax = CalcMinValue(src.rows - 1, y + wndSize.height / 2);
            double area = (xMax - xMin + 1)*(yMax - yMin + 1);
            if (area <= 0)
            {
                // blog提供源码是biImage[i * IMAGE_WIDTH + j] = 255;但是i表示的是列, j是行
                dst.at<uchar>(y, x) = 255;
                continue;
            }

            if (xMin == 0 && yMin == 0)
            {
                diff = integralImg[yMax*src.cols + xMax];
                sqDiff = integralImgSqrt[yMax*src.cols + xMax];
            }
            else if (xMin > 0 && yMin == 0)
            {
                diff = integralImg[yMax*src.cols + xMax] - integralImg[yMax*src.cols + xMin - 1];
                sqDiff = integralImgSqrt[yMax * src.cols + xMax] - integralImgSqrt[yMax * src.cols + xMin - 1];
            }
            else if (xMin == 0 && yMin > 0)
            {
                diff = integralImg[yMax * src.cols + xMax] - integralImg[(yMin - 1) * src.cols + xMax];
                sqDiff = integralImgSqrt[yMax * src.cols + xMax] - integralImgSqrt[(yMin - 1) * src.cols + xMax];;
            }
            else
            {
                diagSum = integralImg[yMax * src.cols + xMax] + integralImg[(yMin - 1) * src.cols + xMin - 1];
                iDiagSum = integralImg[(yMin - 1) * src.cols + xMax] + integralImg[yMax * src.cols + xMin - 1];
                diff = diagSum - iDiagSum;
                sqDiagSum = integralImgSqrt[yMax * src.cols + xMax] + integralImgSqrt[(yMin - 1) * src.cols + xMin - 1];
                sqIDiagSum = integralImgSqrt[(yMin - 1) * src.cols + xMax] + integralImgSqrt[yMax * src.cols + xMin - 1];
                sqDiff = sqDiagSum - sqIDiagSum;
            }
            double mean = diff / area;
            double stdValue = sqrt((sqDiff - diff*diff / area) / (area - 1));
            double threshold = mean*(1 + k*((stdValue / 128) - 1));
            if (src.at<uchar>(y, x) < threshold)
            {
                dst.at<uchar>(y, x) = 0;
            }
            else
            {
                dst.at<uchar>(y, x) = 255;
            }

        }
    }

    delete[] integralImg;
    delete[] integralImgSqrt;
}
