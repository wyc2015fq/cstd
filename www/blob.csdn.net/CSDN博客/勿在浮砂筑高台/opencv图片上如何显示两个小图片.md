# opencv图片上如何显示两个小图片 - 勿在浮砂筑高台 - CSDN博客





2014年11月23日 23:02:00[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：970







这个问题产生于第四章习题1.b我琢磨了大半天，但是最终明白了。
**首先解释一下cvCreateImage（）与cvCreateImageHeader（）两个函数的区别**

**         这两个函数参数都是一样的,这两个函数的区别是cvCreateImage 除了分配图像头之外，还分配图像数据，而cvCreateImageHeader仅仅是分配图像头，并没有分配图像数据。之所以被称为图片头，我想是因为没有属于自己的内存空间数据，但是imagedata可以指向其他的图像数据区，或者调用****void cvSetData( CvArr* arr, void* data, int step )****。详细区别，百度一下...........**

进入正题： 

  IplImage *unit =cvCreateImage(cvSize(frame->width * 2,frame->height),frame->depth,3);

**  IplImage *m_1  = cvCreateImageHeader(cvSize(frame->width ,frame->height),frame->depth,3);**

** 创建了一个宽度是两倍帧宽的图像，并分配内存。** 创建了一个帧宽的图像头，没有自己的数据区。

**m_1->widthStep = unit->widthStep;                                    //重要啦。。。。**

**m_1->imageData = unit->imageData;**

**cvCopy(frame,m_1);**

m_1->imageData = unit->imageData+frame->widthStep;

cvCopy(frame,m_1);

**这样就可以在一个图片上，显示两个相同的图像。**


![](https://img-blog.csdn.net/20141126134956032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**问题来了，为什么是**m_1->widthStep = unit->widthStep而不是m_1->widthStep
 = frame->widthStep

**这是内存上数据排列顺序的问题。我们可以这样理解首先让图像头指到大图像的数据开始处，先拷贝一行像素的数据，然后第二行数据肯定要拷贝到当前的正下方，而不能顺着继续存放。那么就要有跳跃的存放，跳跃的宽度就一个frame->widthStep。但是我们一定要告知m_1的属性widthStep。一般情况下widthStep == width *nChannels,但是这是普通的情况，有时一行像素后会有垃圾区域，这就导致widthStep ！= width *nChannels。这时我们就属于后一种情况，我们不能把有效数据存放到垃圾区域，就要有一个标志widthStep，标示图像宽度真正大小。当存放了width *nChannels，就要跳跃(widthStep-width *nChannels)多个内存区，接着存放下一行像素数据。这里如果**

m_1->widthStep = frame->widthStep 相当于(frame->widthStep-frame->width
 *nChannels) == 0，**跳跃宽度为0，这肯定不对。所以m_1->widthStep =****frame->widthStep *2 **，也即m_1->widthStep = unit->widthStep才对。



