# Pascal VOC 数据集介绍 - YZXnuaa的博客 - CSDN博客
2018年03月08日 11:22:11[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：250
介绍Pascal VOC数据集：
- Challenge and tasks， 只介绍Detection与Segmentation相关内容。
- 数据格式
- 衡量方式
- voc2007, voc2012
# Challenge and tasks
给定自然图片， 从中识别出特定物体。 
待识别的物体有20类：
- person
- bird, cat, cow, dog, horse, sheep
- aeroplane, bicycle, boat, bus, car, motorbike, train
- bottle, chair, dining table, potted plant, sofa, tv/monitor
有以下几个task： 
* Classification（略过） 
* Detection: 将图片中所有的目标用bounding box（bbox）框出来 
* Segmentation: 将图片中所有的目标分割出来 
* Person Layout（略过）
接下来本文只介绍Detection与Segmentation相关的内容。
# Dataset
- 所有的标注图片都有Detection需要的label， 但只有部分数据有Segmentation Label。
- VOC2007中包含9963张标注过的图片， 由train/val/test三部分组成， 共标注出24,640个物体。
- VOC2007的test数据label已经公布， 之后的没有公布（只有图片，没有label）。
- 对于检测任务，VOC2012的trainval/test包含08-11年的所有对应图片。 trainval有11540张图片共27450个物体。
- 对于分割任务， VOC2012的trainval包含07-11年的所有对应图片， test只包含08-11。trainval有 2913张图片共6929个物体。
## Detection Ground Truth and Evaluation
### Ground truth
```xml
<annotation>
    <folder>VOC2007</folder>
    <filename>009961.jpg</filename>
    <source>
        <database>The VOC2007 Database</database>
        <annotation>PASCAL VOC2007</annotation>
        <image>flickr</image>
        <flickrid>334575803</flickrid>
    </source>
    <owner>
        <flickrid>dictioncanary</flickrid>
        <name>Lucy</name>
    </owner>
    <size><!--image shape-->
        <width>500</width>
        <height>374</height>
        <depth>3</depth>
    </size>
    <segmented>0</segmented><!--是否有分割label-->
    <object>
        <name>dog</name> <!--类别-->
        <pose>Unspecified</pose><!--物体的姿态-->
        <truncated>0</truncated><!--物体是否被部分遮挡（>15%）-->
        <difficult>0</difficult><!--是否为难以辨识的物体， 主要指要结体背景才能判断出类别的物体。虽有标注， 但一般忽略这类物体-->
        <bndbox><!--bounding box-->
            <xmin>69</xmin>
            <ymin>4</ymin>
            <xmax>392</xmax>
            <ymax>345</ymax>
        </bndbox>
    </object>
</annotation>
```
### Evaluation
提交的结果存储在一个文件中， 每行的格式为：
```xml
<image identifier> <confidence> <left> <top> <right> <bottom>
```
- 1
例如：
```
comp3_det_test_car.txt:
000004 0.702732 89 112 516 466
000006 0.870849 373 168 488 229
000006 0.852346 407 157 500 213
000006 0.914587 2 161 55 221
000008 0.532489 175 184 232 201
```
- confidence会被用于计算mean average precision(mAP). 简要流程如下， 详细可参考[https://sanchom.wordpress.com/tag/average-precision/](https://sanchom.wordpress.com/tag/average-precision/)
- 根据confidence对结果排序，计算top-1, 2, …N对应的precision和recall
- 将recall划分为n个区间`t in [t1, ..., tn]`
- 找出满足`recall>=t`的最大presicision
- 最后得到n个最大precision， 求它们的平均值
```
aps = []
        for t in np.arange(0., 1.1, 0.1):#将recall分为多个区间
        # 在所有 recall > t对应的precision中找出最大值
            mask = tf.greater_equal(recall, t)
            v = tf.reduce_max(tf.boolean_mask(precision, mask)) 
            aps.append(v / 11.)
        # 得到其平均值
        ap = tf.add_n(aps)
        return ap
```
代码给出的是voc07的计算方式， voc2010在recall区间区分上有变化： 假如有M个正样例，则将recall划分为`[1/M, 1/(M - 1), 1/(M - 2), ... 1]`。其余步骤不变。
- 如输出的bbox与一个ground truth bbox的 IOU大于0.5， 且类别相同，则为True Positive, 否则为False Positive
- 对于一个ground truth bbox， 只会有一个 true positive， 其余都为false positive.
## Segmentation
### Ground Truth
分割的label由两部分组成： 
* class segmentation: 标注出每一个像素的类别 
* object segmentation: 标注出每一个像素属于哪一个物体 
![这里写图片描述](https://img-blog.csdn.net/20170430185425942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM1NjUzMzE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### Evaluation
每类的precision和总体precision.
# Reference
- [http://host.robots.ox.ac.uk/pascal/VOC/](http://host.robots.ox.ac.uk/pascal/VOC/)
- [http://host.robots.ox.ac.uk/pascal/VOC/voc2007/](http://host.robots.ox.ac.uk/pascal/VOC/voc2007/)
- [http://host.robots.ox.ac.uk/pascal/VOC/voc2007/devkit_doc_07-Jun-2007.pdf](http://host.robots.ox.ac.uk/pascal/VOC/voc2007/devkit_doc_07-Jun-2007.pdf)
- [http://host.robots.ox.ac.uk/pascal/VOC/voc2012/devkit_doc.pdf](http://host.robots.ox.ac.uk/pascal/VOC/voc2012/devkit_doc.pdf)
