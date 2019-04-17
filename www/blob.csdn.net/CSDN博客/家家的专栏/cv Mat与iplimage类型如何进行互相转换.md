# cv::Mat与iplimage类型如何进行互相转换 - 家家的专栏 - CSDN博客





2011年07月30日 10:19:46[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3082








                
 转载：h[ttp://www.cnblogs.com/TravelingLight/archive/2010/12/21/1912614.html](http://www.cnblogs.com/TravelingLight/archive/2010/12/21/1912614.html)


IplImage -> cv::Mat
IplImage* pImg = cvLoadImage("lena.jpg");
cv::Mat img(pImg,0);  // 0是不複製影像，也就是pImg與img的data共用同個記憶體位置，header各自有



cv::Mat -> IplImage
// converts header to IplImage; no data is copied
// operator IplImage() const; 
// img 是格式为cv::Mat的数据


IplImage pImg;
pImg = IplImage(img);  // 一樣檔案沒複製

CvMat 和 IplImage 的类型转换，也可以用 cvGetMat , cvGetImage 两个函数来实现。



