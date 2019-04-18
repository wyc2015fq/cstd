# OpenCV Python教程 图像的载入、显示和保存 - YZXnuaa的博客 - CSDN博客
2018年01月29日 11:25:26[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：157
转载请详细注明原作者及出处，谢谢！
本文是OpenCV  2 Computer Vision Application Programming Cookbook读书笔记的第一篇。在笔记中将以Python语言改写每章的代码。
PythonOpenCV的配置这里就不介绍了。
注意，现在OpenCV for Python就是通过NumPy进行绑定的。所以在使用时必须掌握一些NumPy的相关知识！
图像就是一个矩阵，在OpenCV for Python中，图像就是NumPy中的数组！
如果读取图像首先要导入OpenCV包，方法为：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- import cv2  
## 读取并显示图像
在Python中不需要声明变量，所以也就不需要C++中的cv::Mat xxxxx了。只需这样：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- img = cv2.imread("D:\cat.jpg")  
OpenCV目前支持读取bmp、jpg、png、tiff等常用格式。更详细的请参考OpenCV的参考文档。
接着创建一个窗口
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- cv2.namedWindow("Image")  
然后在窗口中显示图像
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- cv2.imshow("Image", img)  
最后还要添上一句：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- cv2.waitKey (0)  
如果不添最后一句，在IDLE中执行窗口直接无响应。在命令行中执行的话，则是一闪而过。
完整的程序为：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- import cv2   
- 
- img = cv2.imread("D:\\cat.jpg")   
- cv2.namedWindow("Image")   
- cv2.imshow("Image", img)   
- cv2.waitKey (0)  
- cv2.destroyAllWindows()  
最后释放窗口是个好习惯！
## 创建/复制图像
新的OpenCV的接口中没有CreateImage接口。即没有cv2.CreateImage这样的函数。如果要创建图像，需要使用numpy的函数（现在使用OpenCV-Python绑定，numpy是必装的）。如下：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- emptyImage = np.zeros(img.shape, np.uint8)  
在新的OpenCV-Python绑定中，图像使用NumPy数组的属性来表示图像的尺寸和通道信息。如果输出img.shape，将得到(500, 375, 3)，这里是以OpenCV自带的cat.jpg为示例。最后的3表示这是一个RGB图像。
也可以复制原有的图像来获得一副新图像。
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- emptyImage2 = img.copy();  
如果不怕麻烦，还可以用cvtColor获得原图像的副本。
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- emptyImage3=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)  
- #emptyImage3[...]=0
后面的emptyImage3[...]=0是将其转成空白的黑色图像。
## 保存图像 
保存图像很简单，直接用cv2.imwrite即可。
cv2.imwrite("D:\\cat2.jpg", img)
第一个参数是保存的路径及文件名，第二个是图像矩阵。其中，imwrite()有个可选的第三个参数，如下：
cv2.imwrite("D:\\cat2.jpg", img，[int(cv2.IMWRITE_JPEG_QUALITY), 5])
第三个参数针对特定的格式： 对于JPEG，其表示的是图像的质量，用0-100的整数表示，默认为95。 注意，cv2.IMWRITE_JPEG_QUALITY类型为Long，必须转换成int。下面是以不同质量存储的两幅图：![](https://img-blog.csdn.net/20130612174753359)
对于PNG，第三个参数表示的是压缩级别。cv2.IMWRITE_PNG_COMPRESSION，从0到9,压缩级别越高，图像尺寸越小。默认级别为3：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- cv2.imwrite("./cat.png", img, [int(cv2.IMWRITE_PNG_COMPRESSION), 0])   
- cv2.imwrite("./cat2.png", img, [int(cv2.IMWRITE_PNG_COMPRESSION), 9])  
保存的图像尺寸如下：![](https://img-blog.csdn.net/20130612174818046)
还有一种支持的图像，一般不常用。
完整的代码为：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/9057415#)[copy](http://blog.csdn.net/sunny2038/article/details/9057415#)
- import cv2  
- import numpy as np  
- 
- img = cv2.imread("./cat.jpg")  
- emptyImage = np.zeros(img.shape, np.uint8)  
- 
- emptyImage2 = img.copy()  
- 
- emptyImage3=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)  
- #emptyImage3[...]=0
- 
- cv2.imshow("EmptyImage", emptyImage)  
- cv2.imshow("Image", img)  
- cv2.imshow("EmptyImage2", emptyImage2)  
- cv2.imshow("EmptyImage3", emptyImage3)  
- cv2.imwrite("./cat2.jpg", img, [int(cv2.IMWRITE_JPEG_QUALITY), 5])  
- cv2.imwrite("./cat3.jpg", img, [int(cv2.IMWRITE_JPEG_QUALITY), 100])  
- cv2.imwrite("./cat.png", img, [int(cv2.IMWRITE_PNG_COMPRESSION), 0])  
- cv2.imwrite("./cat2.png", img, [int(cv2.IMWRITE_PNG_COMPRESSION), 9])  
- cv2.waitKey (0)  
- cv2.destroyAllWindows()  
参考资料：
《OpenCV References Manuel》
《OpenCV  2 Computer Vision Application Programming Cookbook》
《OpenCV Computer Vision with Python》
未完待续...
转载请详细注明原作者及出处，谢谢！
            
