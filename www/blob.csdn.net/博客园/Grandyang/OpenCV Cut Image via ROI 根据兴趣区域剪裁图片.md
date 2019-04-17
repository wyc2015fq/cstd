# OpenCV Cut Image via ROI 根据兴趣区域剪裁图片 - Grandyang - 博客园







# [OpenCV Cut Image via ROI 根据兴趣区域剪裁图片](https://www.cnblogs.com/grandyang/p/4701589.html)







我们在使用OpenCV时，有时候需要对现有图片剪裁，比如只需要一个小窗口中的内容，那么我们可以通过OpenCV的兴趣区域 Region of Interest (ROI) 来很轻易的实现，操作也很简单，首先建立一个新的IplImage图片，大小为你需要剪裁出来的大小，然后建立一个该大小的CvRect变量，用来设定原图片的ROI的范围，然后此时拷贝原图片到新图片，就得到我们需要的内容了，最后别忘了把原图片的ROI还原，参见代码如下：



```
IplImage *img = cvLoadImage("image.jpg", 0);
CvRect roi = cvRect(1, 1, 50, 50);
IplImage *newImg = cvCreateImage(cvSize(roi.width, roi.height),img->depth, img->nChannels);
cvSetImageROI(img, roi); 
cvCopy(newImg, img);
cvResetImageROI(img);
```














