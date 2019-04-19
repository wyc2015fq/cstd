# C++primer学习及OPENCV总结 - qq_19833853的博客 - CSDN博客
2016年12月04日 22:33:33[jessie Fei](https://me.csdn.net/qq_19833853)阅读数：172
申明：本篇博文主要是用来督促博主自己的学习，所以博友们如果有什么问题，你可以问，但是博主不一定会回答！
2016-12-4~2016-12-11
opencv基本数据结构网站
http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html?highlight=rotatedrect
知识点1：创建Mat图像
Mat dstImage = **createImage**(getSize(srcImage), 8, 1);
知识点2：Mat与IplImage相互转换
# IplImage->Mat
IplImage*dst3;
Mat dst4=dst3;
Mat->IplImage
Mat img;
IplImage*src;
src=&IplImage(img);
知识点3：将Mat srcgujia 图片扩大，然后进行复制
        //对图像进行扩充
Mat midgujia = cvCreateImage(Size(srcgujia.cols+100,srcgujia.rows+100), 8, 1);
IplImage*mid;
mid = &IplImage(midgujia);
cvZero(mid);
for (int i = 0; i<srcgujia.rows; i++)
{
/*uchar*data=srcgujia.ptr<uchar>(i);
uchar*data1 = midgujia.ptr<uchar>(i + 50);*/
for (int j = 0; j<srcgujia.cols; j++)
{
uchar*data = srcgujia.ptr<uchar>(i, j);
uchar*data1 = midgujia.ptr<uchar>(i + 50, j + 50);
*data1++ = *data++;//只能使用指针++，而不能使用data1[i,j]=data[i,j]   原因不明确
}
}
复制的结果图
![](https://img-blog.csdn.net/20161205113656575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
知识点4
cvLine( CvArr* img,
      CvPoint pt1, 
                      CvPoint pt2, 
      CvScalar color,
             int thickness=1, 
     int line_type=8, 
                     int shift=0 );
第一个参数img：要划的线所在的图像;
第二个参数pt1：直线起点
第二个参数pt2：直线终点
第三个参数color：直线的颜色 e.g:Scalor(0,0,255)
第四个参数thickness=1：线条粗细
第五个参数line_type=8, 
   8 (or 0) - 8-connected line（8邻接)连接 线。
   4 - 4-connected line(4邻接)连接线。
   CV_AA - antialiased 线条。
第六个参数：坐标点的小数点位数。
知识点5：需找物体的凸包
vector<Point>points;
Point point;
for (int i = 0; i < midgujia.rows; i++)
{
uchar*data2 = midgujia.ptr<uchar>(i);
for (int j = 0; j < midgujia.cols; j++)
{
if (data2[j] == 255)
{
point.x = j;
point.y = i;
points.push_back(point);
}
}
}
RotatedRect box = minAreaRect(points);
Point2f vertex[4];
box.points(vertex);
for (int i = 0; i < 4; i++)
line(midgujia, vertex[i], vertex[(i + 1) % 4], Scalar(255, 255, 255), 1,8);
imshow("矩形包围后", midgujia);
waitKey(0);
//问题  ：若minAreaRect的参数不是点就会出现错误
![](https://img-blog.csdn.net/20161205215542087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
知识点6

