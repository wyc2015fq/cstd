# Class_Segmentation_and_Object_Localization_with_Superpixel_Neighborhoods - xiahouzuoxin - CSDN博客





2012年11月07日 21:02:04[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1938
个人分类：[Image/Audio/ML...](https://blog.csdn.net/xiahouzuoxin/article/category/1203619)









        《Class_Segmentation_and_Object_Localization_with_Superpixel_Neighborhoods》是一篇关于使用超像素实现目标查找的文章，作者一方面使用QuickSHift获取超像素，另一方面使用SIFT提取原图像的特征，将特征用K-means聚类。然后将超像素和聚类后的类别对应，用聚类后的类别特征描述超像素。由于本人做研究需要用到，并且觉得是一篇不错的文章，因此翻译出来，这只是其中最核心的理论部分，没有论文前的Introduction等内容，也没有后面的Expriments。混着过六级的人英语毕竟不咋滴，仅供参考。




![](https://img-my.csdn.net/uploads/201211/07/1352293864_3462.JPG)


![](https://img-my.csdn.net/uploads/201211/07/1352293871_7255.JPG)


![](https://img-my.csdn.net/uploads/201211/07/1352293890_5019.JPG)

![](https://img-my.csdn.net/uploads/201211/07/1352293900_1371.JPG)


![](https://img-my.csdn.net/uploads/201211/07/1352293909_8076.JPG)















