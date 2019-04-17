# 用自己的数据集训练Faster-RCNN的几种方法 - Snoopy_Dream - CSDN博客





2018年03月20日 22:32:04[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：195








## 用自己的数据集训练Faster-RCNN的几种方法

Faster R-CNN官方公布的代码主要是针对Pascal、COCO数据集的，如果要针对特定任务训练自己的数据集，需要做一些额外的工作。

**方式一：**自己标注 [https://saicoco.github.io/object-detection-4/](https://saicoco.github.io/object-detection-4/)

[https://github.com/ChaoPei/create-voc2007-dataset](https://github.com/ChaoPei/create-voc2007-dataset)

**方式二：**下载外部数据集，通常格式各异，又分两种情况：
- 
**修改工程代码，适应自己的数据集格式：**

[http://www.cnblogs.com/louyihang-loves-baiyan/p/4903231.html](http://www.cnblogs.com/louyihang-loves-baiyan/p/4903231.html)

- 
**将自己的数据格式做成VOC2007形式的数据集：**

[http://blog.csdn.net/sinat_30071459/article/details/50723212](http://blog.csdn.net/sinat_30071459/article/details/50723212)
[http://blog.csdn.net/gvfdbdf/article/details/52214008](http://blog.csdn.net/gvfdbdf/article/details/52214008)

##### **修改后还需要将类别信息相关代码改一下：**

[http://blog.csdn.net/sinat_30071459/article/details/51332084](http://blog.csdn.net/sinat_30071459/article/details/51332084)


原文链接：[http://blog.csdn.net/hongxingabc/article/details/79039537](http://blog.csdn.net/hongxingabc/article/details/79039537)



