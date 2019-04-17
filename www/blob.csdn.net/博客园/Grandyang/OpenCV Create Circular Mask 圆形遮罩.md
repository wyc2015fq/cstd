# OpenCV Create Circular Mask 圆形遮罩 - Grandyang - 博客园







# [OpenCV Create Circular Mask 圆形遮罩](https://www.cnblogs.com/grandyang/p/4731858.html)







在OpenCV中，比较常见的是矩形遮罩CvRect，没有专门提供圆形的mask，那么我们只能自己写一个来模拟圆形mask的函数，需要提供的参数为原图的大小，以及圆形mask的圆心位置和半径即可，返回一个cv::Mat型的mask，参见代码如下：



```
// C API
cv::Mat extractCircularMask(CvArr *img, int col, int row, int r) {
    IplImage *roi;
    roi = cvCreateImage(cvGetSize(img), 8, 1);
    cvZero(roi);
    cvCircle(roi, cvPoint(col, row), r, CV_RGB(255, 255, 255), -1, 8, 0);
    return cv::Mat(roi);
}
```



或者是：

```
// C++
cv::Mat extractCircularMask(cv::Mat img, int col, int row, int r) {
    cv::Mat cirMask = img.clone();
    cirMask.setTo(cv::Scalar::all(0));
    cv::circle(cirMask, cv::Point(col, row), r, cv::Scalar(255, 255, 255), -1, 8, 0);
    return cirMask;
}
```














