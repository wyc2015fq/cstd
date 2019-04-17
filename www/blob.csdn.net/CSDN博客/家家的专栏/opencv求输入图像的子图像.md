# opencv求输入图像的子图像 - 家家的专栏 - CSDN博客





2010年08月22日 19:54:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：807标签：[image																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)








opencv求输入图像的子图像///////////////////////////////////////////////////////////////////////////////////////
名称:      cvGetSubImage
功能:      求输入图像的子图像
算法:      
参数:
image  - 输入图像
roi    - 子图像的定义区域，region of interests
返回:
如果成功，返回创建好的子图像
注意事项:
子图像在函数中创建，用完后需要释放内存.
//F*/

IplImage* cvGetSubImage(IplImage *image, CvRect roi)
{
IplImage *result;
// 设置 ROI 
cvSetImageROI(image,roi);
// 创建子图像
result = cvCreateImage( cvSize(roi.width, roi.height), image->depth, image->nChannels );
cvCopy(image,result);
cvResetImageROI(image);
return result;
}  


转自：[http://hi.baidu.com/ilybeen/blog/item/c2fcf12d1697975d4fc2268a.html](http://hi.baidu.com/ilybeen/blog/item/c2fcf12d1697975d4fc2268a.html)




