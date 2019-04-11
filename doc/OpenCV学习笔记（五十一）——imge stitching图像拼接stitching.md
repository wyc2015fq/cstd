# OpenCV学习笔记（五十一）——imge stitching图像拼接stitching

2012年05月22日 17:26:49

yang_xian521

阅读数：66885

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

stitching是OpenCV2.4.0一个新模块，功能是实现图像拼接，所有的相关函数都被封装在Stitcher类当中。这个类当中我们可能用到的成员函数有createDefault、estimateTransform、composePanorama、stitch。其内部实现的过程是非常繁琐的，需要很多算法的支持，包括图像特征的寻找和匹配，摄像机的校准，图像的变形，曝光补偿和图像融合。但这些模块的接口、调用，强大的OpenCV都为我们搞定了，我们使用OpenCV做图像拼接，只需要调用createDefault函数生成默认的参数，再使用stitch函数进行拼接就ok了。就这么简单！estimateTransform和composePanorama函数都是比较高级的应用，如果各位对stitching的流程不是很清楚的话，还是慎用。

实例也非常简单，下载链接哦：<http://download.csdn.net/detail/yang_xian521/4321158>。

输入原图（为了显示，我都压缩过）：

![img](https://img-my.csdn.net/uploads/201205/22/1337678637_2101.jpg)

![img](https://img-my.csdn.net/uploads/201205/22/1337678703_7543.jpg)

![img](https://img-my.csdn.net/uploads/201205/22/1337678743_8043.jpg)

![img](https://img-my.csdn.net/uploads/201205/22/1337678965_8201.jpg)