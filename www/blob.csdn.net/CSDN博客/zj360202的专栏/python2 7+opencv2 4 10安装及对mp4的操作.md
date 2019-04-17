# python2.7+opencv2.4.10安装及对mp4的操作 - zj360202的专栏 - CSDN博客





2017年03月13日 18:41:04[zj360202](https://me.csdn.net/zj360202)阅读数：4938












准备文件：


python2.7.12，下载链接[https://www.python.org/downloads/](https://www.python.org/downloads/)


pip8.1.1，下载链接[https://pypi.python.org/packages/41/27/9a8d24e1b55bd8c85e4d022da2922cb206f183e2d18fee4e320c9547e751/pip-8.1.1.tar.gz#md5=6b86f11841e89c8241d689956ba99ed7](https://pypi.python.org/packages/41/27/9a8d24e1b55bd8c85e4d022da2922cb206f183e2d18fee4e320c9547e751/pip-8.1.1.tar.gz#md5=6b86f11841e89c8241d689956ba99ed7)


opencv2.4.10 [http://opencv.org/downloads.html](http://opencv.org/downloads.html)



开始：

[OpenCV](http://lib.csdn.net/base/opencv)只支持[Python](http://lib.csdn.net/base/python)的2.6和2.7系列的版本，所以下载的时候一定要选择这2个中的版本。

1，python库的安装


点击下载下的python-2.7.12.amd64.msi，一路next下去，安装完成后，配置环境变量，在path里面加入安装的python的根目录，例如本人安装目录E:\Python27。


进入cmd命令行，输入，python，输出以下信息证明python安装成功。

![](https://img-blog.csdn.net/20160829163759061)




2，pippython setup安装


点击下载的pip-8.1.1.tar.gz，进行解压，例如本人解压到了E盘根目录下。cmd命令进入解压目录，执行，python setup.py install

![](https://img-blog.csdn.net/20160829163829705)



3, numpy,matplotlib的安装
还是在当前目录，执行pythonpip
 install numpy

![](https://img-blog.csdn.net/20160829163905828)



执行python pip install matplotlib
![](https://img-blog.csdn.net/20160829164046972)





4，opencv安装


点击下载的opencv-2.4.10.exe，一路next下去，例如本人安装到E盘根目录下。安装完成后，将E:\opencv\build\python\2.7\x64下的cv2.pyd拷贝到E:\Python27\Lib\site-packages下面。

将

1.把D:\opencv\sources\3rdparty\ffmpeg\opencv_ffmpeg_64.dll复制到E:\Python27[](http://lib.csdn.net/base/python)下；

2.把opencv_ffmpeg_64.dll重命名为opencv_ffmpeg2410_64.dll；把opencv_ffmpeg.dll重命名为opencv_ffmpeg2410.dll；规则就是将opencv_ffmpeg_64.dll按版本x.y.z弄成opencv_ffmpegxyz_64.dll


在cmd下面输入python进入python命令行，然后输入import  cv2，如没有错误信息证明安装成功。![](https://img-blog.csdn.net/20160829164115472)



5，一个简单的[测试](http://lib.csdn.net/base/softwaretest)


新建text.py，输入如下的程序，cmd下输入python text.py，如果可以正确的显示图片，证明安装成功。



**[python]**[view
 plain](http://blog.csdn.net/qq_14845119/article/details/52354394#)[copy](http://blog.csdn.net/qq_14845119/article/details/52354394#)



- import cv2  
- import numpy as np  
- 
- img = cv2.imread("1.jpg")  
- emptyImage = np.zeros(img.shape, np.uint8)  
- 
- emptyImage2 = img.copy()  
- 
- emptyImage3=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)  
- 
- cv2.imshow("EmptyImage3", emptyImage3)  
- cv2.waitKey (0)  
- cv2.destroyAllWindows()  




显示的图片如下所示

![](https://img-blog.csdn.net/20160829164236865)





6.开发操作mp4的代码


在python的工作空间下面建一个image的目录


写一个test.py


```python
# -*- coding: utf-8 -*-
 
import numpy as np
import cv2
import time

cap = cv2.VideoCapture('2.mp4')
c = 1

fps = cap.get(cv2.cv.CV_CAP_PROP_FPS)  #得到每一秒的帧数

print fps
i = 0;
while(cap.isOpened()):
    ret, frame = cap.read()
	
    if not ret :
	    break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    #cv2.imshow('frame',gray)
	
    if i%fps == 0 :#每一秒写一次文件
		cv2.imwrite('image/'+str(c) + '.jpg',frame) #存储为图像
    i = i+1
    c = c+1
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
```


感谢：

[windows下的python+ opencv安装攻略](http://blog.csdn.net/qq_14845119/article/details/52354394)


[opencv无法读取视频解决方法&&抽取视频帧存储为图像（python）](http://blog.csdn.net/andrewseu/article/details/49100093)








