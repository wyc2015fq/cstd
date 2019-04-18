# DNN：windows使用 YOLO V1，V2 - wishchinYang的专栏 - CSDN博客
2017年09月05日 13:34:32[wishchin](https://me.csdn.net/wishchin)阅读数：2463
          本文有修改，如有疑问，请移步原文。
          原文链接：[YOLO v1之总结篇（linux+windows）](http://blog.csdn.net/qq_14845119/article/details/53612362)
                  此外：  [YOLO-V2总结篇](http://blog.csdn.net/qq_14845119/article/details/53589282)   Yolo9000的改进还是非常大的
          由于原版的官方YOLOv1是只支持[linux](http://lib.csdn.net/base/linux) 和mac的，如果要自己修改，可能需要走好对哦的坑，同时还得具备一定的技术水平，幸好有革命斗士为我们走出了这一步，
          可以参考下面2个YOLO-windows，
[https://github.com/frischzenger/yolo-windows](https://github.com/frischzenger/yolo-windows)
[https://github.com/AlexeyAB/yolo-windows](https://github.com/AlexeyAB/yolo-windows)
~~ 推荐使用第二个版本，比较新，使用CUDA8.0，第一个版本使用CUDA6.5.~~
         推荐使用YOLO-V2版本，V1版本没有编译成功。
         随便下载上面的一个yolo，然后进入yolo-windows-master\build\darknet\下面，用vs2013直接打开darknet.sln，选择，x64版本，只要配置[OpenCV](http://lib.csdn.net/base/opencv)和pthreads就可以，
为了方便大家参考，这里贴出本人的环境配置...........................
         .................................
         配置好编译时，出现：**error C2275: 'size_t'**.....问题
         解决方法： 把出错的地方，变量声明全部移到函数头部.....
         此外，一步编译成功......
编译成功：
             进入，yolo-windows-master\build\darknet\x64\Release\下面，
             去官网链接，~~http://pjreddie.com/media/files/yolov1.weights，下载好yolov1.weights~~，[https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)，使用
[`https://pjreddie.com/media/files/yolo.weights`](https://pjreddie.com/media/files/yolo.weights)
           得到预训练的权值。
            然后运行，~~darknet.cmd~~ darknet_coco.cmd，随便输入一张图像，例如，person.jpg，就会出来运行效果。
            其中第一个为cpu+debug版本，第二个为cpu+release版本，
下图为运行结果：
![](https://img-blog.csdn.net/20170905132954100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170905133007340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170905133004251?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**编译cudnn版本**：
        在convlutional_kernels.cu 工程出现 unsolved external symbol cudnn_handle. 
        在darkNet源代码里面 cuda.h 已经声明！
### 训练篇：
这里假定我要实现一个简单的人脸检测。
（1）首先就是数据集的准备，这里建议使用python+QT开发的抠图小工具，labelImg。
（2）模仿VOC的格式建立相应的文件夹，执行，
请拜访原文：[YOLO-V2总结篇](http://blog.csdn.net/qq_14845119/article/details/53589282)
