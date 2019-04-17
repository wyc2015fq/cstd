# cvReleaseImage 释放内存出错 - Grandyang - 博客园







# [cvReleaseImage 释放内存出错](https://www.cnblogs.com/grandyang/p/4615036.html)







cvReleaseImage是OpenCV中C语言库中的释放图片内存的函数，比如我们加载或者克隆了一幅图片，当不需要这幅图片了时，我们为了避免内存泄露，需要释放这些空间，可以参见我之前的博客[OpenCV Show Image cvShowImage() 使用方法](http://www.cnblogs.com/grandyang/p/4111847.html)，其使用方法如下：

```
IplImage *img = cvLoadImage("Input.jpg");
cvNamedWindow("Image:",1);
cvShowImage("Image:",img);
cvWaitKey();
cvDestroyWindow("Image:");
cvReleaseImage(&img);
```



如上所示的cvReleaseImage的使用方法是没有问题的，但是有两种情况下使用cvReleaseImage来释放内存会出错，需要格外小心。

1. 当使用cvCaptureFromCAM函数时

这个函数是用OpenCV来打开摄像头并捕捉摄像头的图像时需要用到的，可以参见我的另一篇博客中的代码[OpenCV show two cameras 同时显示两个摄像头](http://www.cnblogs.com/grandyang/p/4302501.html)。这个函数会建立一个CvCapture的结构体，专门用来打开或关闭摄像头的，在OpenCV的官方文档中，我们会看到下面一段话：


Note： In C API, when you finished working with video, release CvCapture structure with cvReleaseCapture(), or use Ptr<CvCapture> that calls cvReleaseCapture() automatically in the destructor.


这告诉我们使用完了CvCapture，需要用cvReleaseCapture来释放内存，那么我们就不需要再用cvReleaseImage来释放每一张图像的内存了。官方文档中还有另一段注意事项：


Note ：OpenCV 1.x functions cvRetrieveFrame and cv.RetrieveFrame return image stored inside the video capturing structure. It is not allowed to modify or release the image! You can copy the frame using [cvCloneImage()](http://docs.opencv.org/modules/core/doc/old_basic_structures.html#IplImage*%20cvCloneImage%28const%20IplImage*%20image%29) and then do whatever you want with the copy.


这里明确指出了从摄像头获取的图片不能被修改和释放，所以如果我们对捕获的图像调用cvReleaseImage就会出错。



2. 当从别的图像类转为IplImage格式时

另外一种不能使用cvReleaseImage的情况是，当我们使用别的SDK时，比如FlyCapture2中的f2Image图像格式，或者是Qt5中的QImage图像格式，我们自己写函数将其转为为OpenCV中IplImage格式时，可参见我的博客[FlyCapture2 fc2Image OpenCV IplImage Conversion 两种图像格式之间的转换](http://www.cnblogs.com/grandyang/p/4612228.html)，我们自己申请了一段unsigned char格式的内存来保存图像数据，使用完了后要释放的时候，也不能使用cvReleaseImage，会出错。我们只能手动释放内存，我们需要看申请这段内存使用的是new还是malloc，如果是new的内存，用delete来释放，如果是malloc的内存，用free来释放。














