
# OpenCV字节对齐 - 郭云飞的专栏 - CSDN博客


2016年10月13日 18:08:18[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：273


IplImage中有两个属性易错：一是width属性；二是widthStep属性。前者是表示图像的每行像素数，后者指表示存储一行像素需要的字节数。
在OpenCV里边，widthStep必须是4的倍数，从而实现字节对齐，有利于提高运算速度。如果8U单通道图像宽度为3，那么widthStep是4，加一个字节补齐。这个图像的一行需要4个字节，只使用前3个，最后一个空着。也就是一个宽3高3的图像的imageData数据大小为4*3=12字节。需要注意的是，空着的那个像素并不是无效的，它仍然可以被操作，这就是导致错误的根源。

错例：
假如现在有一个char* data的指针指向一个15*15的灰度图像的数据起始地址，我们想把图像数据通过cvShowImage函数显示出来，比较直观的一种做法如下：

```python
......
IplImage* image = cvCreateImage(cvSize(15, 15), 8, 1);
memcpy(image->imageData, data, 15*15);
cvNamedWindow("window");
cvShowImage("window", image);
cvWaitKey();
cvReleaseImage(&image);
cvDestroyWindow("window");
......
```
你会发现，显示的图像奇怪的往左下角歪过去了。当你看完这篇文章后希望不要再因为这个问题浪费你的时间了（shamed：这个问题郁闷了我整整一天）。其实原因就在于，在cvCreateImage的时候，OpenCV为实现字节对齐，使得每行数据实际有16个字节（多出一个），在使用memcpy的过程中，这些多出的字节就把对应的数据给“吃”了，因为这些数据在cvShowImage的时候并不会显示出来，这样，第二行就少一个字节，第三行少两个字节，……，所以整个图像就偏向左下角了！知道这一点后可以将memcpy语句更改如下：

```python
for(int i = 0; i<15; i++)
{
    memcpy(image->imageData + image->widthStep*i, data + 15*i, 15);
}
```
这样，程序才能按我们的设想运行。




