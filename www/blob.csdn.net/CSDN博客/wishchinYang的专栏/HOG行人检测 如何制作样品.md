# HOG行人检测 如何制作样品 - wishchinYang的专栏 - CSDN博客
2013年11月04日 19:56:29[wishchin](https://me.csdn.net/wishchin)阅读数：929
个人分类：[艺术/图像评价](https://blog.csdn.net/wishchin/article/category/1649519)
原文链接：[http://blog.csdn.net/Armily/article/details/8333836](http://blog.csdn.net/Armily/article/details/8333836)
[如何制作训练样本](http://www.opencv.org.cn/forum/viewtopic.php?f=1&t=9146#p35176)
分析了原作者的数据集，结合网上一些资料，下面描述如何制作训练样本
1、如何从原始图片生成样本
对比INRIAPerson\INRIAPerson\Train\pos（原始图片），INRIAPerson\train_64x128_H96\pos（生成样本）可以发现，作者从原始图片裁剪出一些站立的人，要求该人不被遮挡，然后对剪裁的图片left-right reflect。以第一张图片为例crop001001，它剪裁了2个不被遮挡的人，再加上原照片，共3张，再加左右镜像，总共6张。
2、裁剪
　可利用基于opencv1.0的程序imageclipper，进行裁剪并保存,它会自动生成文件名并保存在同一路径下新生成的imageclipper文件夹下。
3.改变图片大小
　可以利用Acdsee软件，Tools/open in editor,进去后到Resize选项;　tools/rotate还可实现left-right reflect
4. 制作pos.lst列表　 进入dos界面，定位到需要制作列表的图片文件夹下，输入 dir /b> pos.lst，即可生成文件列表；
仔细分析了cvhop.cpp中的compute函数，可以直接调用它来获得样本HOG，然后训练得到检测算子
1.制作样本
2.对每一张图片调用
hog.compute(img, descriptors,Size(8,8), Size(0,0));
可以生成hog descriptors，把它保存到文件中
for(int j=0;j<3780;j++)
fprintf(f,"%f,",descriptors[j]);
3.利用SVM进行训练和分类，可得到权重系数，即getDefaultPeopleDetector()函数中调用的检测算子detector[ ]
