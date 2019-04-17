# OpenCV Show Image cvShowImage() 使用方法 - Grandyang - 博客园







# [OpenCV Show Image cvShowImage() 使用方法](https://www.cnblogs.com/grandyang/p/4111847.html)







新版的OpenCV在所有的函数和类前都加上了cv或Cv，这样很好的避免了区域污染（namespace pollution），而且不用在前面加‘cv::’，非常的使用。像之前的imshow()函数被现在的cvShowImage()所替代，现如今在OpenCV中显示一张图片可用如下代码：



C API:

```
IplImage *img = cvLoadImage("Input.jpg");
cvNamedWindow("Image:",1);
cvShowImage("Image:",img);
cvWaitKey();
cvDestroyWindow("Image:");
cvReleaseImage(&img);
```



C++:

```
cv::Mat image = cv::imread("img.jpg");   
cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
cv::imshow( "Display window", image );  
cv::waitKey(0);
cv::destroyWindow("Display window");
image.release();
```














