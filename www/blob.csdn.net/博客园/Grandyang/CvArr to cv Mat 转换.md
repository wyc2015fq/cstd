# CvArr* to cv::Mat 转换 - Grandyang - 博客园







# [CvArr* to cv::Mat 转换](https://www.cnblogs.com/grandyang/p/4731957.html)







OpenCV中的CvArr*的定义的一个空类型的指针，可以转换为其派生类CvMat和IplImage，那么如何将其转化为cv::Mat呢，其实很简单，只需要一行代码即可：



```
// CvArr *_img
cv::Mat img = cv::cvarrToMat(_img);
```














