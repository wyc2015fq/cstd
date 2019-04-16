# MeanShift算法C++解析（一） - lyx的专栏 - CSDN博客





2015年11月03日 21:26:21[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1536









毕业设计的核心是MeanShift算法，作为一个小本，默默先抛开高端的MeanShift纯理论来研究一下程序对图像都做了什么吧。然后回过头去看数学理论会轻松很多吧。不说啦，这个程序也是我根据网上大牛的程序改的，自己简化和改造了一下下。后期当然还要做更多的优化啦~~这次先讲一下主函数吧。开发环境是Qt+OpenCV4.8，不过算法不用OpenCV自带的，只用了OpenCV的数据结构吧啦。


​主函数其实没做什么，首先获取了视频流，然后进入一个While（1）的循环。循环里面判断是否是在视频追踪状态，以及是否在鼠标选择目标状态，两个不同的分支将会调用不同的子函数。其中鼠标事件是一个回调函数。当然啦，这里提一句，Qt不是用回调机制的，这个机制的提供者是OpenCV。


主函数​


intmain(intargc,char*argv[])
{
CvCapture*capture=cvCreateFileCapture("H:/GraduationDesign/Qt/opencv/12.mp4");
current=cvQueryFrame(capture);//cvQuryFrame从摄像头或者文件中抓取一帧
while(1)
{
if(is_tracking)//internaldefineitiontheflagofthetrackingcommand，当鼠标选择完毕之后value为true
{
//此时追踪目标已经初始化
MeanShift_Tracking(current);
}
//=============
//====selectingtarget
//=============
intc=cvWaitKey(1);
//暂停
if(c=='p')//pushthekeyPtoselecttheareatotracking
{
pause=true;
cvSetMouseCallback("Meanshift",onMouse,0);//当你按下P暂停之后，设置鼠标选择的回调函数
}
while(pause)
{
if(cvWaitKey(0)=='p')
pause=false;
}//waitingforthetrackingstart(pushthekeypagain)
//==============
//=====selsectingover
//==============
cvShowImage("Meanshift",current);
current=cvQueryFrame(capture);//抓取一帧
cvSmooth(current,current,CV_MEDIAN,3,3,0,0);
}
cvNamedWindow("Meanshift",1);
cvReleaseCapture(&capture);
cvDestroyWindow("Meanshift");
return0;
}


