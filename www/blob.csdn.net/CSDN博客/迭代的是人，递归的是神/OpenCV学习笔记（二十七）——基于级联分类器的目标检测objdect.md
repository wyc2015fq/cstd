# OpenCV学习笔记（二十七）——基于级联分类器的目标检测objdect - 迭代的是人，递归的是神 - CSDN博客





2011年11月16日 10:58:10[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：100299
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









OpenCV支持的目标检测的方法是利用样本的Haar特征进行的分类器训练，得到的级联boosted分类器（Cascade Classification）。注意，新版本的C++接口除了Haar特征以外也可以使用LBP特征。

先介绍一下相关的结构，级联分类器的计算特征值的基础类FeatureEvaluator，功能包括读操作read、复制clone、获得特征类型getFeatureType，分配图片分配窗口的操作setImage、setWindow，计算有序特征calcOrd，计算绝对特征calcCat，创建分类器特征的结构create函数。级联分类器类CascadeClassifier。目标级联矩形的分组函数groupRectangles。

接下来，我尝试使用CascadeClassifier这个级联分类器类检测视频流中的目标（haar支持的目标有人脸、人眼、嘴、鼻、身体。这里尝试比较成熟的人脸和眼镜）。用load函数加载XML分类器文件（目前提供的分类器包括Haar分类器和LBP分类器（LBP分类器数据较少））具体步骤如下：

这里再补充一点：后来我又进行了一些实验，对正面人脸分类器进行了实验，总共有4个，alt、alt2、alt_tree、default。对比下来发现alt和alt2的效果比较好，alt_tree耗时较长，default是一个轻量级的，经常出现误检测。所以还是推荐大家使用haarcascade_frontalface_atl.xml和haarcascade_frontalface_atl2.xml。

### 1）加载级联分类器

调用CascadeClassifier类成员函数load实现，代码为：



```cpp
CascadeClassifier face_cascade;
face_cascade.load("haarcascade_frontalface_alt.xml");
```


### 2）读取视频流



这部分比较基础啦~~从文件中读取图像序列，读取视频文件，读取摄像头视频流看过我之前的文章，这3种方法应该了然于心。

### 3）对每一帧使用该分类器

这里先将图像变成灰度图，对它应用直方图均衡化，做一些预处理的工作。接下来检测人脸，调用detectMultiScale函数，该函数在输入图像的不同尺度中检测物体，参数image为输入的灰度图像，objects为得到被检测物体的矩形框向量组，scaleFactor为每一个图像尺度中的尺度参数，默认值为1.1，minNeighbors参数为每一个级联矩形应该保留的邻近个数（没能理解这个参数，-_-|||），默认为3，flags对于新的分类器没有用（但目前的haar分类器都是旧版的，CV_HAAR_DO_CANNY_PRUNING利用Canny边缘检测器来排除一些边缘很少或者很多的图像区域，CV_HAAR_SCALE_IMAGE就是按比例正常检测，CV_HAAR_FIND_BIGGEST_OBJECT只检测最大的物体，CV_HAAR_DO_ROUGH_SEARCH只做初略检测），默认为0.minSize和maxSize用来限制得到的目标区域的范围。这里调用的代码如下：



```cpp
face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
```


### 4）显示目标



这个也比较简单，调用ellips函数将刚才得到的faces矩形框都显示出来

更进一步，也可以在得到的每一幅人脸中得到人眼的位置，调用的分类器文件为haarcascade_eye_tree_eyeglasses.xml，先将脸部区域选为兴趣区域ROI，重复上诉步骤即可，这里就不详细介绍了。当然，感兴趣的朋友也可以试试其他的xml文件作为分类器玩一下啊，感觉LBP特征虽然xml文件的大小很小，但效果还可以，不过我没有做过多的测试。光说不练假把式，最后贴上效果图和源代码的下载地址

![](http://hi.csdn.net/attachment/201111/16/0_1321411793W6Mn.gif)


代码下载地址：[http://download.csdn.net/detail/yang_xian521/3800468](http://download.csdn.net/detail/yang_xian521/3800468)



