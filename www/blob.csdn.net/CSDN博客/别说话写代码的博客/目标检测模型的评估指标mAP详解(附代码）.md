# 目标检测模型的评估指标mAP详解(附代码） - 别说话写代码的博客 - CSDN博客





2018年12月10日 15:38:48[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：774








文章转自：[https://zhuanlan.zhihu.com/p/37910324](https://zhuanlan.zhihu.com/p/37910324)

对于使用机器学习解决的大多数常见问题，通常有多种可用的模型。每个模型都有自己的独特之处，并随因素变化而表现不同。

每个模型在**“验证/测试”数据集**上来评估性能，性能衡量使用各种统计量如**准确度**（accuracy），**精度**（precision），**召回率**（recall）等。选择的统计量通常针对特定应用场景和用例。 对于每个应用场景，选择一个能够客观比较模型的度量指标非常重要。

这篇文章将介绍目标检测（Object Detection）问题中的最常用评估指标-*Mean Average Precision*，即mAP。

大多数时候，这些指标很容易理解和计算。例如，在二元分类中，精确度和召回率是一个一个简单直观的统计量。然而，目标检测是一个非常不同且有趣的问题。即使你的目标检测器在图片中检测到猫，但如果你无法定位，它也没有用处。由于你要预测的是图像中各个物体是否出现及其位置，如何计算mAP将非常有趣。

在讲解mAP之前，我们先定义目标检测问题。

## 目标检测问题

在目标检测问题中，给定一个图像，找到它所包含的物体，找到它们的位置并对它们进行分类。目标检测模型通常是在一组特定的类集合上进行训练的，所以模型只会定位和分类图像中的那些类。另外，对象的位置通常采用矩形边界框表示。因此，目标检测涉及图像中物体的定位和分类。

![](https://img-blog.csdnimg.cn/2018121920304918.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)



图1 几个常见的计算机视觉问题（来自Stanford’s CS231n）

下面所述的Mean Average Precision特别适用于同时预测物体位置及类别的算法。 因此，从图1可以看出，它对评估定位模型、目标检测模型和分割模型非常有用。

## 评估目标检测模型

**为什么是mAP?**

目标检测问题中的每个图片都可能包含一些不同类别的物体。如前所述，需要评估模型的物体分类和定位性能。因此，用于图像分类问题的标准指标precision不能直接应用于此。 这就是为什么需要mAP。 我希望读完这篇文章后，你将能够理解它的含义。

**关于Ground Truth**

对于任何算法，评估指标需要知道ground truth（真实标签）数据。 我们只知道训练、验证和测试数据集的ground truth。对于目标检测问题，ground truth包括图像中物体的类别以及该图像中每个物体的真实边界框。

![](https://img-blog.csdnimg.cn/20181219203119730.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)



Ground truth可视化

这里给出了一个实际图片（jpg、png等格式），以及相应的文本注释（边界框坐标 ![(x, y, w, h)](http://www.zhihu.com/equation?tex=%28x%2C+y%2C+w%2C+h%29) 和类别），如图中红色框以及文本标签所示。

对于这个特殊例子，模型在训练时需要原始的图片：

![](https://img-blog.csdnimg.cn/20181219203143888.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

原始图片

以及ground truth的3个坐标及类别（这里假定图片大小是1000x800px，所有的坐标值都是以像素为单位的近似值）：

![](https://img-blog.csdnimg.cn/20181219203204647.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

下面让我们动一下手，去看如何计算mAP。这里我们不谈论不同的目标检测算法，假定我们已经有了一个训练好的模型，现在只需要在验证集上评估其性能。

## mAP含义及计算

前面展示了原始图像和以及对应的ground truth。训练集和验证集中所有图像都以此方式标注。

训练好的目标检测模型会给出大量的预测结果，但是其中大多数的预测值都会有非常低的置信度（confidence score），因此我们只考虑那些置信度高于某个阈值的预测结果。

将原始图片送入训练好的模型，在经过置信度阈值筛选之后，目标检测算法给出带有边界框的预测结果：

![](https://img-blog.csdnimg.cn/20181219203235506.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

模型的预测结果

现在，由于我们人类是目标检测专家，我们可以知道这些检测结果大致正确。但我们如何量化呢？我们首先需要判断每个检测的正确性。这里采用IoU（Intersection over Union），它可以作为评价边界框正确性的度量指标。 这是一个非常简单的指标。从名称看，有些人会发现这个名字是自解释的，但我们需要更好的解释。这里会以简短的方式解释IoU，如果想深入理解，可以参考[Adrian Rosebrock](http://link.zhihu.com/?target=https%3A//www.pyimagesearch.com/)的[这篇文章(Intersection over Union (IoU) for object detection)](http://link.zhihu.com/?target=https%3A//www.pyimagesearch.com/2016/11/07/intersection-over-union-iou-for-object-detection/)。

**IoU**

IoU是预测框与ground truth的交集和并集的比值。这个量也被称为Jaccard指数，并于20世纪初由Paul Jaccard首次提出。为了得到交集和并集，我们首先将预测框与ground truth放在一起，如图所示。

![](https://img-blog.csdnimg.cn/20181219203259478.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

预测框与ground truth（这里只给出horse）

对于每个类，预测框和ground truth重叠的区域是交集，而横跨的总区域就是并集。其中horse类的交集和并集如下图所示（这个例子交集比较大）：

![](https://img-blog.csdnimg.cn/20181219203315781.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

其中蓝绿色部分是交集，而并集还包括橘色的部分。那么，IoU可以如下计算：

![](https://img-blog.csdnimg.cn/20181219203334138.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

图片启发自 pyimagesearch，后者启发自University of Pittsburg’s CS1699课程

**鉴别正确的检测结果并计算precision和recall**

为了计算precision和recall，与所有机器学习问题一样，我们必须鉴别出**True Positives**（真正例）、**False Positives**（假正例）、**True Negatives**（真负例）和 **False Negatives**（假负例）。

为了获得True Positives and False Positives，我们需要使用IoU。计算IoU，我们从而确定一个检测结果（Positive）是正确的（True）还是错误的（False）。最常用的阈值是0.5，即如果IoU> 0.5，则认为它是True Positive，否则认为是False Positive。而COCO数据集的评估指标建议对不同的IoU阈值进行计算，但为简单起见，我们这里仅讨论一个阈值0.5，这是PASCAL VOC数据集所用的指标。

为了计算Recall，我们需要Negatives的数量。由于图片中我们没有预测到物体的每个部分都被视为Negative，因此计算True Negatives比较难办。但是我们可以只计算False Negatives，即我们模型所漏检的物体。

另外一个需要考虑的因素是模型所给出的各个检测结果的置信度。通过改变置信度阈值，我们可以改变一个预测框是Positive还是 Negative，即改变预测值的正负性(不是box的真实正负性，是预测正负性)。基本上，阈值以上的所有预测（Box + Class）都被认为是Positives，并且低于该值的都是Negatives。

对于每一个图片，ground truth数据会给出该图片中各个类别的实际物体数量。我们可以计算每个Positive预测框与ground truth的IoU值，并取最大的IoU值，认为该预测框检测到了那个IoU最大的ground truth。然后根据IoU阈值，我们可以计算出一张图片中各个类别的正确检测值（True Positives, TP）数量以及错误检测值数量（False Positives, FP）。据此，可以计算出各个类别的precision：
![\\precision = \frac{TP}{TP+FP}](http://www.zhihu.com/equation?tex=%5C%5Cprecision+%3D+%5Cfrac%7BTP%7D%7BTP%2BFP%7D)

既然我们已经得到了正确的预测值数量（True Positives），也很容易计算出漏检的物体数（False Negatives, FN）。据此可以计算出Recall（其实分母可以用ground truth总数）：
![\\recall = \frac{TP}{TP+FN}](http://www.zhihu.com/equation?tex=%5C%5Crecall+%3D+%5Cfrac%7BTP%7D%7BTP%2BFN%7D)

**计算mAP**

mAP这个术语有不同的定义。此度量指标通常用于信息检索和目标检测领域。然而这两个领域计算mAP的方式却不相同。这里我们只谈论目标检测中的mAP计算方法。

在目标检测中，mAP的定义首先出现在PASCAL Visual Objects Classes(VOC)竞赛中，这个大赛包含许多图像处理任务，详情可以参考这个[paper](http://link.zhihu.com/?target=http%3A//homepages.inf.ed.ac.uk/ckiw/postscript/ijcv_voc09.pdf)（里面包含各个比赛的介绍以及评估等）。

前面我们已经讲述了如何计算Precision和Recall，但是，正如前面所述，至少有两个变量会影响Precision和Recall，即IoU和置信度阈值。IoU是一个简单的几何度量，可以很容易标准化，比如在PASCAL VOC竞赛中采用的IoU阈值为0.5，而COCO竞赛中在计算mAP较复杂，其计算了一系列IoU阈值（0.05至0.95）下的mAP。但是置信度却在不同模型会差异较大，可能在我的模型中置信度采用0.5却等价于在其它模型中采用0.8置信度，这会导致precision-recall曲线变化。为此，PASCAL VOC组织者想到了一种方法来解决这个问题，即要采用一种可以用于任何模型的评估指标。在paper中，他们推荐使用如下方式计算Average Precision（AP）：

> For a given task and class, the precision/recall curve is computed from a method’s ranked output. Recall is defined as the proportion of all positive examples ranked above a given rank. Precision is the proportion of all examples above that rank which are from the positive class. The AP summarises the shape of the precision/recall curve, and is defined as the mean precision at a set of eleven equally spaced recall levels [0,0.1,...,1]:

可以看到，为了得到precision-recall曲线，首先要对模型预测结果进行排序（ranked output，按照各个预测值置信度降序排列）。那么给定一个rank，Recall和Precision仅在高于该rank值的预测结果中计算，改变rank值会改变recall值。这里共选择11个不同的recall（[0, 0.1, ..., 0.9, 1.0]），可以认为是选择了11个rank，由于按照置信度排序，所以实际上等于选择了11个不同的置信度阈值。那么，AP就定义为在这11个recall下precision的平均值，其可以表征整个precision-recall曲线（曲线下面积）。

![](https://img-blog.csdnimg.cn/20181219203402370)

另外，在计算precision时采用一种插值方法（interpolate）：

> The precision at each recall level r is interpolated by taking the maximum precision measured for a method for which the corresponding recall exceeds r:

The intention in interpolating the precision/recall curve in this way is to reduce the impact of the “wiggles” in the precision/recall curve, caused by small variations in the ranking of examples.

及对于某个recall值r，precision值取所有recall>=r中的最大值（这样保证了p-r曲线是单调递减的，避免曲线出现摇摆）:

![](https://img-blog.csdnimg.cn/20181219203402389)

不过这里VOC数据集在2007年提出的mAP计算方法，而在2010之后却使用了所有数据点，而不是仅使用11个recall值来计算AP（详细参考这篇[paper](http://link.zhihu.com/?target=http%3A//host.robots.ox.ac.uk/pascal/VOC/pubs/everingham15.pdf)）：

> Up until 2009 interpolated average precision (Salton and Mcgill 1986) was used to evaluate both classification and detection. However, from 2010 onwards the method of computing AP changed to use all data points rather than TREC-style sampling (which only sampled the monotonically decreasing curve at a fixed set of uniformly-spaced recall values 0, 0.1, 0.2,..., 1). The intention in interpolating the precision–recall curve was to reduce the impact of the ‘wiggles’ in the precision–recall curve, caused by small variations in the ranking of examples. However, the downside of this interpolation was that the evaluation was too crude to discriminate between the methods at low AP.

对于各个类别，分别按照上述方式计算AP，取所有类别的AP平均值就是mAP。这就是在目标检测问题中mAP的计算方法。可能有时会发生些许变化，如COCO数据集采用的计算方式更严格，其计算了不同IoU阈值和物体大小下的AP（详情参考[COCO Detection Evaluation](http://link.zhihu.com/?target=http%3A//cocodataset.org/%23detection-eval)）。

当比较mAP值，记住以下要点：
- mAP通常是在一个数据集上计算得到的。
- 虽然解释模型输出的绝对量化并不容易，但mAP作为一个相对较好的度量指标可以帮助我们。 当我们在流行的公共数据集上计算这个度量时，该度量可以很容易地用来比较目标检测问题的新旧方法。
- 根据训练数据中各个类的分布情况，mAP值可能在某些类（具有良好的训练数据）非常高，而其他类（具有较少/不良数据）却比较低。所以你的mAP可能是中等的，但是你的模型可能对某些类非常好，对某些类非常不好。因此，建议在分析模型结果时查看各个类的AP值。这些值也许暗示你需要添加更多的训练样本。

## 代码实现

Facebook开源的Detectron包含[VOC数据集的mAP计算](http://link.zhihu.com/?target=https%3A//github.com/facebookresearch/Detectron/blob/05d04d3a024f0991339de45872d02f2f50669b3d/lib/datasets/voc_eval.py)，这里贴出其核心实现，以对mAP的计算有更深入的理解。首先是precision和recall的计算：

```python
# 按照置信度降序排序
sorted_ind = np.argsort(-confidence)
BB = BB[sorted_ind, :]   # 预测框坐标
image_ids = [image_ids[x] for x in sorted_ind] # 各个预测框的对应图片id

# 便利预测框，并统计TPs和FPs
nd = len(image_ids)
tp = np.zeros(nd)
fp = np.zeros(nd)
for d in range(nd):
    R = class_recs[image_ids[d]]
    bb = BB[d, :].astype(float)
    ovmax = -np.inf
    BBGT = R['bbox'].astype(float)  # ground truth

    if BBGT.size > 0:
        # 计算IoU
        # intersection
        ixmin = np.maximum(BBGT[:, 0], bb[0])
        iymin = np.maximum(BBGT[:, 1], bb[1])
        ixmax = np.minimum(BBGT[:, 2], bb[2])
        iymax = np.minimum(BBGT[:, 3], bb[3])
        iw = np.maximum(ixmax - ixmin + 1., 0.)
        ih = np.maximum(iymax - iymin + 1., 0.)
        inters = iw * ih

        # union
        uni = ((bb[2] - bb[0] + 1.) * (bb[3] - bb[1] + 1.) +
               (BBGT[:, 2] - BBGT[:, 0] + 1.) *
               (BBGT[:, 3] - BBGT[:, 1] + 1.) - inters)

        overlaps = inters / uni
        ovmax = np.max(overlaps)
        jmax = np.argmax(overlaps)
    # 取最大的IoU
    if ovmax > ovthresh:  # 是否大于阈值
        if not R['difficult'][jmax]:  # 非difficult物体
            if not R['det'][jmax]:    # 未被检测
                tp[d] = 1.
                R['det'][jmax] = 1    # 标记已被检测
            else:
                fp[d] = 1.
    else:
        fp[d] = 1.

# 计算precision recall
fp = np.cumsum(fp)
tp = np.cumsum(tp)
rec = tp / float(npos)
# avoid divide by zero in case the first detection matches a difficult
# ground truth
prec = tp / np.maximum(tp + fp, np.finfo(np.float64).eps)
```

这里最终得到一系列的precision和recall值，并且这些值是按照置信度降低排列统计的，可以认为是取不同的置信度阈值（或者rank值）得到的。然后据此可以计算AP：

```python
def voc_ap(rec, prec, use_07_metric=False):
    """Compute VOC AP given precision and recall. If use_07_metric is true, uses
    the VOC 07 11-point method (default:False).
    """
    if use_07_metric:  # 使用07年方法
        # 11 个点
        ap = 0.
        for t in np.arange(0., 1.1, 0.1):
            if np.sum(rec >= t) == 0:
                p = 0
            else:
                p = np.max(prec[rec >= t])  # 插值
            ap = ap + p / 11.
    else:  # 新方式，计算所有点
        # correct AP calculation
        # first append sentinel values at the end
        mrec = np.concatenate(([0.], rec, [1.]))
        mpre = np.concatenate(([0.], prec, [0.]))

        # compute the precision 曲线值（也用了插值）
        for i in range(mpre.size - 1, 0, -1):
            mpre[i - 1] = np.maximum(mpre[i - 1], mpre[i])

        # to calculate area under PR curve, look for points
        # where X axis (recall) changes value
        i = np.where(mrec[1:] != mrec[:-1])[0]

        # and sum (\Delta recall) * prec
        ap = np.sum((mrec[i + 1] - mrec[i]) * mpre[i + 1])
    return ap
```

计算各个类别的AP值后，取平均值就可以得到最终的mAP值了。但是对于COCO数据集相对比较复杂，不过其提供了计算的API，感兴趣可以看一下[cocodataset/cocoapi](http://link.zhihu.com/?target=https%3A//github.com/cocodataset/cocoapi/blob/master/PythonAPI/pycocotools/cocoeval.py)。



