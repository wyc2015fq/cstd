
# TLD（Tracking-Learning-Detection）学习与源码理解之（五） - zouxy09的专栏 - CSDN博客


2012年08月21日 20:28:45[zouxy09](https://me.csdn.net/zouxy09)阅读数：23031


**TLD（Tracking-Learning-Detection）学习与源码理解之（五）**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
下面是自己在看论文和这些大牛的分析过程中，对代码进行了一些理解，但是由于自己接触图像处理和机器视觉没多久，另外由于自己编程能力比较弱，所以分析过程可能会有不少的错误，希望各位不吝指正。而且，因为编程很多地方不懂，所以注释得非常乱，还海涵。
LKTracker.h
```cpp
#include<tld_utils.h>
#include <opencv2/opencv.hpp>
//使用金字塔LK光流法跟踪，所以类的成员变量很多都是OpenCV中calcOpticalFlowPyrLK()函数的参数
class LKTracker{
private:
  std::vector<cv::Point2f> pointsFB;
  cv::Size window_size;  //每个金字塔层的搜索窗口尺寸
  int level;            //最大的金字塔层数
  std::vector<uchar> status;   //数组。如果对应特征的光流被发现，数组中的每一个元素都被设置为 1， 否则设置为 0
  std::vector<uchar> FB_status;   
  std::vector<float> similarity;  //相似度
  std::vector<float> FB_error;   //Forward-Backward error方法，求FB_error的结果与原始位置的欧式距离
                                 //做比较，把距离过大的跟踪结果舍弃
  float simmed;
  float fbmed;
  //TermCriteria模板类，取代了之前的CvTermCriteria，这个类是作为迭代算法的终止条件的
  //该类变量需要3个参数，一个是类型，第二个参数为迭代的最大次数，最后一个是特定的阈值。
  //指定在每个金字塔层，为某点寻找光流的迭代过程的终止条件。
  cv::TermCriteria term_criteria;
  float lambda;   //某阈值？？Lagrangian 乘子
  // NCC 归一化交叉相关，FB error与NCC结合，使跟踪更稳定  交叉相关的图像匹配算法？？
  //交叉相关法的作用是进行云团移动的短时预测。选取连续两个时次的GMS-5卫星云图，将云图区域划分为32×32像素
  //的图像子集，采用交叉相关法计算获取两幅云图的最佳匹配区域，根据前后云图匹配区域的位置和时间间隔，确
  //定出每个图像子集的移动矢量（速度和方向），并对图像子集的移动矢量进行客观分析，其后，基于检验后的云
  //图移动矢量集，利用后向轨迹方法对云图作短时外推预测。
  void normCrossCorrelation(const cv::Mat& img1, const cv::Mat& img2, std::vector<cv::Point2f>& points1, std::vector<cv::Point2f>& points2);
  bool filterPts(std::vector<cv::Point2f>& points1,std::vector<cv::Point2f>& points2);
public:
  LKTracker();
  //特征点的跟踪？？
  bool trackf2f(const cv::Mat& img1, const cv::Mat& img2,
                std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2);
  float getFB(){return fbmed;}
};
```

LKTracker.cpp
```cpp
#include <LKTracker.h>
using namespace cv;
//金字塔LK光流法跟踪
//Media Flow 中值光流跟踪 加 跟踪错误检测
//构造函数，初始化成员变量
LKTracker::LKTracker(){
  ////该类变量需要3个参数，一个是类型，第二个参数为迭代的最大次数，最后一个是特定的阈值。
  term_criteria = TermCriteria( TermCriteria::COUNT + TermCriteria::EPS, 20, 0.03);
  window_size = Size(4,4);
  level = 5;
  lambda = 0.5;
}

bool LKTracker::trackf2f(const Mat& img1, const Mat& img2, vector<Point2f> &points1, vector<cv::Point2f> &points2){
  //TODO!:implement c function cvCalcOpticalFlowPyrLK() or Faster tracking function
  //Forward-Backward tracking
  //基于Forward-Backward Error的中值流跟踪方法
  //金字塔LK光流法跟踪
  //forward trajectory 前向轨迹跟踪
  calcOpticalFlowPyrLK( img1,img2, points1, points2, status, similarity, window_size, level, term_criteria, lambda, 0);
  //backward trajectory 后向轨迹跟踪
  calcOpticalFlowPyrLK( img2,img1, points2, pointsFB, FB_status,FB_error, window_size, level, term_criteria, lambda, 0);
  
  //Compute the real FB-error
  //原理很简单：从t时刻的图像的A点，跟踪到t+1时刻的图像B点；然后倒回来，从t+1时刻的图像的B点往回跟踪，
  //假如跟踪到t时刻的图像的C点，这样就产生了前向和后向两个轨迹，比较t时刻中 A点 和 C点 的距离，如果距离
  //小于一个阈值，那么就认为前向跟踪是正确的；这个距离就是FB_error
  //计算 前向 与 后向 轨迹的误差
  for( int i= 0; i<points1.size(); ++i ){
        FB_error[i] = norm(pointsFB[i]-points1[i]);   //norm()求矩阵或向量的范数??绝对值？
  }
  //Filter out points with FB_error[i] <= median(FB_error) && points with sim_error[i] > median(sim_error)
  normCrossCorrelation(img1, img2, points1, points2);
  return filterPts(points1, points2);
}
//利用NCC把跟踪预测的结果周围取10*10的小图片与原始位置周围10*10的小图片（使用函数getRectSubPix得到）进
//行模板匹配（调用matchTemplate）
void LKTracker::normCrossCorrelation(const Mat& img1,const Mat& img2, vector<Point2f>& points1, vector<Point2f>& points2) {
        Mat rec0(10,10,CV_8U);
        Mat rec1(10,10,CV_8U);
        Mat res(1,1,CV_32F);
        for (int i = 0; i < points1.size(); i++) {
                if (status[i] == 1) {  //为1表示该特征点跟踪成功
						//从前一帧和当前帧图像中（以每个特征点为中心？）提取10x10象素矩形，使用亚象素精度
                        getRectSubPix( img1, Size(10,10), points1[i],rec0 );   
                        getRectSubPix( img2, Size(10,10), points2[i],rec1);
						//匹配前一帧和当前帧中提取的10x10象素矩形，得到匹配后的映射图像
						//CV_TM_CCOEFF_NORMED 归一化相关系数匹配法
						//参数分别为：欲搜索的图像。搜索模板。比较结果的映射图像。指定匹配方法
                        matchTemplate( rec0,rec1, res, CV_TM_CCOEFF_NORMED); 
                        similarity[i] = ((float *)(res.data))[0];  //得到各个特征点的相似度大小
                } else {
                        similarity[i] = 0.0;
                }
        }
        rec0.release();
        rec1.release();
        res.release();
}
//筛选出 FB_error[i] <= median(FB_error) 和 sim_error[i] > median(sim_error) 的特征点
//得到NCC和FB error结果的中值，分别去掉中值一半的跟踪结果不好的点
bool LKTracker::filterPts(vector<Point2f>& points1,vector<Point2f>& points2){
  //Get Error Medians
  simmed = median(similarity);   //找到相似度的中值
  size_t i, k;
  for( i=k = 0; i<points2.size(); ++i ){
        if( !status[i])
          continue;
        if(similarity[i]> simmed){   //剩下 similarity[i]> simmed 的特征点
          points1[k] = points1[i];   
          points2[k] = points2[i];
          FB_error[k] = FB_error[i];
          k++;
        }
    }
  if (k==0)
    return false;
  points1.resize(k);
  points2.resize(k);
  FB_error.resize(k);
  fbmed = median(FB_error);     //找到FB_error的中值
  for( i=k = 0; i<points2.size(); ++i ){
      if( !status[i])
        continue;
      if(FB_error[i] <= fbmed){   /
        points1[k] = points1[i];   //再对上一步剩下的特征点进一步筛选，剩下 FB_error[i] <= fbmed 的特征点
        points2[k] = points2[i];
        k++;
      }
  }
  points1.resize(k);
  points2.resize(k);
  if (k>0)
    return true;
  else
    return false;
}


/*
 * old OpenCV style
void LKTracker::init(Mat img0, vector<Point2f> &points){
  //Preallocate
  //pyr1 = cvCreateImage(Size(img1.width+8,img1.height/3),IPL_DEPTH_32F,1);
  //pyr2 = cvCreateImage(Size(img1.width+8,img1.height/3),IPL_DEPTH_32F,1);
  //const int NUM_PTS = points.size();
  //status = new char[NUM_PTS];
  //track_error = new float[NUM_PTS];
  //FB_error = new float[NUM_PTS];
}

void LKTracker::trackf2f(..){
  cvCalcOpticalFlowPyrLK( &img1, &img2, pyr1, pyr1, points1, points2, points1.size(), window_size, level, status, track_error, term_criteria, CV_LKFLOW_INITIAL_GUESSES);
  cvCalcOpticalFlowPyrLK( &img2, &img1, pyr2, pyr1, points2, pointsFB, points2.size(),window_size, level, 0, 0, term_criteria, CV_LKFLOW_INITIAL_GUESSES | CV_LKFLOW_PYR_A_READY | CV_LKFLOW_PYR_B_READY );
}
*/
```


