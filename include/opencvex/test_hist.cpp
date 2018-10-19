#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int test_hist1()
{
  //首先肯定是彩色形式读取图片
  cv::Mat sourcePic = cv::imread("E:/data/testpic/farm2.jpg");
  cv::imshow("Source Picture", sourcePic);

  //定义函数需要的一些变量
  //图片数量nimages
  int nimages = 1;
  //通道数量,我们总是习惯用数组来表示，后面会讲原因
  int channels[3] = { 0,1,2 };
  //输出直方图
  cv::Mat outputHist_red, outputHist_green, outputHist_blue;
  //维数
  int dims = 1;
  //存放每个维度直方图尺寸（bin数量）的数组histSize
  int histSize[3] = { 256,256,256 };
  //每一维数值的取值范围ranges
  float hranges[2] = { 0, 255 };
  //值范围的指针
  const float *ranges[3] = { hranges,hranges,hranges };
  //是否均匀
  bool uni = true;
  //是否累积
  bool accum = false;

  //计算图像的直方图(红色通道部分)
  cv::calcHist(&sourcePic, nimages, &channels[0], cv::Mat(), outputHist_red, dims, &histSize[0], &ranges[0], uni, accum);
  //计算图像的直方图(绿色通道部分)
  cv::calcHist(&sourcePic, nimages, &channels[1], cv::Mat(), outputHist_green, dims, &histSize[1], &ranges[1], uni, accum);
  //计算图像的直方图(蓝色通道部分)
  cv::calcHist(&sourcePic, nimages, &channels[2], cv::Mat(), outputHist_blue, dims, &histSize[2], &ranges[2], uni, accum);

  //遍历每个箱子(bin)检验，这里的是在控制台输出的。
  //for (int i = 0; i < 256; i++)
  //std::cout << "bin/value:" << i << "=" << outputHist_red.at<float>(i) << std::endl;

  //画出直方图
  int scale = 1;
  //直方图的图片,因为尺寸是一样大的,所以就以histSize[0]来表示全部了.
  cv::Mat histPic(histSize[0], histSize[0] * scale * 3, CV_8UC3, cv::Scalar(0, 0, 0));
  //找到最大值和最小值,索引从0到2分别是红,绿,蓝
  double maxValue[3] = { 0, 0, 0 };
  double minValue[3] = { 0, 0, 0 };
  cv::minMaxLoc(outputHist_red, &minValue[0], &maxValue[0], NULL, NULL);
  cv::minMaxLoc(outputHist_green, &minValue[1], &maxValue[1], NULL, NULL);
  cv::minMaxLoc(outputHist_blue, &minValue[2], &maxValue[2], NULL, NULL);
  //测试
  std::cout << minValue[0] << " " << minValue[1] << " " << minValue[2] << std::endl;
  std::cout << maxValue[0] << " " << maxValue[1] << " " << maxValue[2] << std::endl;

  //纵坐标缩放比例
  double rate_red = (histSize[0] / maxValue[0])*0.9;
  double rate_green = (histSize[0] / maxValue[1])*0.9;
  double rate_blue = (histSize[0] / maxValue[2])*0.9;

  for (int i = 0; i < histSize[0]; i++)
  {
    float value_red = outputHist_red.at<float>(i);
    float value_green = outputHist_green.at<float>(i);
    float value_blue = outputHist_blue.at<float>(i);
    //分别画出直线
    cv::line(histPic, cv::Point(i*scale, histSize[0]), cv::Point(i*scale, histSize[0] - value_red*rate_red), cv::Scalar(0, 0, 255));
    cv::line(histPic, cv::Point((i + 256)*scale, histSize[0]), cv::Point((i + 256)*scale, histSize[0] - value_green*rate_green), cv::Scalar(0, 255, 0));
    cv::line(histPic, cv::Point((i + 512)*scale, histSize[0]), cv::Point((i + 512)*scale, histSize[0] - value_blue*rate_blue), cv::Scalar(255, 0, 0));
  }
  cv::imshow("histgram", histPic);
  cv::waitKey(0);
  return 0;
}


#include <vector>
using namespace std;
using namespace cv;
int test_hist(int argc, char** argv)
{
  Mat src, dst;
  src = cv::imread("E:/data/testpic/farm2.jpg");

  if (!src.data)
  {
    return -1;
  }

  vector<Mat> rgb_planes;
  split(src, rgb_planes);

  int histSize = 255;
  float range[] = { 0, 255 };
  const float* histRange = { range };
  bool uniform = true;
  bool accumulate = false;

  Mat r_hist, g_hist, b_hist;

  // 计算直方图:  
  calcHist(&rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
  calcHist(&rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
  calcHist(&rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);

  // 创建直方图画布  
  int hist_w = 400;
  int hist_h = 400;
  int bin_w = cvRound((double)hist_w / histSize);

  Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

  // 将直方图归一化到范围 [ 0, histImage.rows ]  
  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

  // 在直方图画布上画出直方图  
  for (int i = 1; i < histSize; i++)
  {
    line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
      Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
      Scalar(0, 0, 255), 2, 8, 0);
    line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
      Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
      Scalar(0, 255, 0), 2, 8, 0);
    line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
      Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
      Scalar(255, 0, 0), 2, 8, 0);
  }

  /// 显示直方图  
  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
  imshow("calcHist Demo", histImage);
  waitKey(0);
  return 0;

}
