
//基于opencv的Niblack二值化算法
//Niblack算法是比较出名的二值化算法，网上很多Niblack代码是基于Matlab的，本人觉得其速度比较慢，所以便基于OpenCV改写了其算法，具体参考的博客链接已经忘记了，希望博主原谅。如果缺少某些函数，比如最大值最小值函数，可以参考本人其他博客，里面会提供。废话不多说，直接上代码：

/** @brief 计算单通道灰度图像的平均值

@param src 单通道灰度图
*/
static double GetMatAverage(const cv::Mat& src)
{
    CV_Assert(src.type() == CV_8UC1);
    double sum = 0.0;
    for (int y = 0; y < src.rows; ++y)
    {
        for (int x = 0; x < src.cols; ++x)
        {
            int value = src.at<uchar>(y, x);
            sum += value;
        }
    }

    return sum / (src.rows * src.cols);
}

/** @brief 计算单通道灰度图像的标准差

@param src 单通道灰度图
*/
static double GetMatStdDev(const cv::Mat& src, double meanValue)
{
    CV_Assert(src.type() == CV_8UC1);
    double sum = 0.0;
    for (int y = 0; y < src.rows; ++y)
    {
        for (int x = 0; x < src.cols; ++x)
        {
            int value = src.at<uchar>(y, x);
            double var = (value - meanValue)*(value - meanValue);
            sum += var;
        }
    }

    double stdDev = std::sqrt(double(sum) / double(src.rows * src.cols));
    return stdDev;
}

void Niblack(const cv::Mat & src, cv::Mat & dst, cv::Size wndSize)
{
    CV_Assert(src.type() == CV_8UC1);
    wndSize.width |= 1;
    wndSize.height |= 1;
    CV_Assert((wndSize.width % 2 == 1) && (wndSize.height % 2 == 1));
    CV_Assert((wndSize.width <= src.cols) && (wndSize.height <= src.rows));

    cv::Mat flag = cv::Mat::zeros(src.rows, src.cols, CV_64FC1);
    for (int y = wndSize.height / 2; y <= src.rows - wndSize.height / 2 - 1; ++y)
    {
        for (int x = wndSize.width / 2; x <= src.cols - wndSize.width / 2 - 1; ++x)
        {
            int value = src.at<uchar>(y, x);
            cv::Point center = cv::Point(x, y);
            cv::Point topLeftPoint = cv::Point(x - wndSize.width / 2, y - wndSize.height / 2);
            cv::Rect wnd = cv::Rect(topLeftPoint.x, topLeftPoint.y, wndSize.width, wndSize.height);
            cv::Mat roiMat = src(wnd);
            double avgValue = GetMatAverage(roiMat);
            double dev = GetMatStdDev(roiMat, avgValue);

            // 这里是0.2
            double flagValue = avgValue + 0.2 * dev;
            flag.at<double>(y, x) = flagValue;
        }
    }

    dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
    for (int y = 0; y < src.rows; ++y)
    {
        for (int x = 0; x < src.cols; ++x)
        {
            double flagValue = flag.at<double>(y, x);
            int value = src.at<uchar>(y, x);
            if (value > flagValue)
            {
                dst.at<uchar>(y, x) = 255;
            }
            else
            {
                dst.at<uchar>(y, x) = 0;
            }
        }
    }
}
