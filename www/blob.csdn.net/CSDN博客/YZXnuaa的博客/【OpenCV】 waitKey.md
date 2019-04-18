# 【OpenCV】 waitKey - YZXnuaa的博客 - CSDN博客
2018年02月08日 15:06:10[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：82
                OpenCV中的一个函数
函数原型为： 
.
C++: int waitKey(int delay=0)
Python: cv2.waitKey([delay]) → retval
C: int cvWaitKey(int delay=0 )
Python: cv.WaitKey(delay=0) → int
函数功能：
waitKey()函数的功能是不断刷新图像，频率时间为delay，单位为ms。
返回值为当前键盘按键值。
所以显示图像时，如果需要在imshow(“xxxx”,image)后吐舌头加上while（cvWaitKey(n)==key）为大于等于0的数即可，那么程序将会停在显示函数处，不运行其他代码;直到键盘值为key的响应之后。
delay>0时，延迟”delay”ms，在显示视频时这个函数是有用的，用于设置在显示完一帧图像后程序等待”delay”ms再显示下一帧视频；如果使用waitKey(0)则只会显示第一帧视频。
返回值：如果delay>0,那么超过指定时间则返回-1；如果delay=0，将没有返回值。 
 　　如果程序想响应某个按键，可利用if(waitKey(1)==Keyvalue)； 
**如果delay<0,等待时间无限长，返回值为按键值**
**经常程序里面出现if( waitKey(10) >= 0 ) 是说10ms中按任意键进入此if块。**
注意：这个函数是HighGUI中唯一能够获取和操作事件的函数，所以在一般的事件处理中，它需要周期地被调用，除非HighGUI被用在某些能够处理事件的环境中。比如在MFC环境下，这个函数不起作用。
