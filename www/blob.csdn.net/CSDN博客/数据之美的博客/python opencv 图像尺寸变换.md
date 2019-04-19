# python opencv 图像尺寸变换 - 数据之美的博客 - CSDN博客
2017年04月12日 16:37:32[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：19106
利用[Python](http://lib.csdn.net/base/python)[OpenCV](http://lib.csdn.net/base/opencv)中的  cv.Resize(源，目标，变换方法)就可以实现变换为想要的尺寸了
源文件：就不用说了
目标：你可以对图像进行倍数的放大和缩小  也可以直接的输入尺寸大小
变换的方法：
CV_INTER_NN - 最近邻插值,  
CV_INTER_LINEAR - 双线性插值 (缺省使用)  
CV_INTER_AREA - 使用象素关系重采样。当图像缩小时候，该方法可以避免波纹出现。当图像放大时，类似于 CV_INTER_NN 方法..  
CV_INTER_CUBIC - 立方插值.  
如 我要将一个图片变为32*32大小的
####   image=cv2.imread('test.jpg')  res=cv2.resize(image,(32,32),interpolation=cv2.INTER_CUBIC)  cv2.imshow('iker',res)  cv2.imshow('image',image)  cv2.waitKey(0)  cv2.destoryAllWindows()
当然要正常的导入opencv 库  自己试过了哈  网上很多的已经过时了 
