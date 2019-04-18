# Faster RCNN代码详解（五）：关于检测网络（Fast RCNN）的proposal - AI之路 - CSDN博客





2018年05月20日 11:02:45[AI之路](https://me.csdn.net/u014380165)阅读数：3454








在[Faster RCNN代码详解（二）：网络结构构建](https://blog.csdn.net/u014380165/article/details/80303812)中介绍了Faster RCNN算法的网络结构，其中有一个用于生成ROI proposal target的自定义层，**该自定义层的输出作为检测网络（Fast RCNN）的输入，这篇博客就来介绍这个自定义层的内容。**

**该自定义层的实现所在脚本~/mx-rcnn/rcnn/symbol/proposal_target.py，该层返回的group列表包含4个值，分别是rois，label，bbox_target，bbox_weight。roi用于ROI Pooling层，label用于检测网络的分类支路、bbox_target和bbox_weight用于检测网络的回归支路**。

**通过系列四中对RPN网络中anchor的介绍，你应该明白这里的label、bbox_target、bbox_weight和RPN网络中的不同，RPN网络中的label、bbox_target和bbox_weight等变量的定义方式和这里不同，同时在RPN网络中那边变量是服务于anchor的。**

```python
"""
Proposal Target Operator selects foreground and background roi and assigns label, bbox_transform to them.
"""

import logging
import mxnet as mx
import numpy as np
from distutils.util import strtobool

from ..logger import logger
from rcnn.io.rcnn import sample_rois

class ProposalTargetOperator(mx.operator.CustomOp):
    def __init__(self, num_classes, batch_images, batch_rois, fg_fraction):
        super(ProposalTargetOperator, self).__init__()
        self._num_classes = num_classes
        self._batch_images = batch_images
        self._batch_rois = batch_rois
        self._fg_fraction = fg_fraction

        if logger.level == logging.DEBUG:
            self._count = 0
            self._fg_num = 0
            self._bg_num = 0

    def forward(self, is_train, req, in_data, out_data, aux):
        assert self._batch_rois % self._batch_images == 0, \
            'BATCHIMAGES {} must devide BATCH_ROIS {}'.format(self._batch_images, self._batch_rois)
        rois_per_image = int(self._batch_rois / self._batch_images)
        fg_rois_per_image = int(round(self._fg_fraction * rois_per_image))


# all_rois的维度是(2000,5)，不过all_rois除了4列坐标外，剩下一列全是0，
# 并不表示roi的标签，仅仅是batch的index标识。gt_boxes的维度是(x,5)，x是object的数量。
        all_rois = in_data[0].asnumpy()
        gt_boxes = in_data[1].asnumpy()

        # Include ground-truth boxes in the set of candidate rois
# 初始化的zeros替换掉gt_boxes中object的类别，然后和原来的all_rois做合并，
# 最后得到的all_rois的维度是(2000+x,5)。因为all_rois变量中并不需要ground truth的标签，
# 所以都用0值替代。从最后的assert语句也可以看出第一列0值的含义是和batch相关。
        zeros = np.zeros((gt_boxes.shape[0], 1), dtype=gt_boxes.dtype)
        all_rois = np.vstack((all_rois, np.hstack((zeros, gt_boxes[:, :-1]))))
        # Sanity check: single batch only
        assert np.all(all_rois[:, 0] == 0), 'Only single item batches are supported'


# 调用sample_rois函数对roi做抽样，目的是让roi总数保持为rois_per_image（默认是128），
# 同时正负roi的比例是1:3左右，详见最后附录1的sample_rois函数代码。all_rois就是维度
# 为(2000+x,5)的roi输入；输入中fg_rois_per_image表示一个图像中带有object的roi数量，
# 或者叫foreground的数量；rois_per_image是每张图像的roi数量；gt_boxes就是维度为(x,5)的gt_boxes输入。
        rois, labels, bbox_targets, bbox_weights = \
            sample_rois(all_rois, fg_rois_per_image, rois_per_image, self._num_classes, gt_boxes=gt_boxes)

        if logger.level == logging.DEBUG:
            logger.debug("labels: %s" % labels)
            logger.debug('num fg: {}'.format((labels > 0).sum()))
            logger.debug('num bg: {}'.format((labels == 0).sum()))
            self._count += 1
            self._fg_num += (labels > 0).sum()
            self._bg_num += (labels == 0).sum()
            logger.debug("self._count: %d" % self._count)
            logger.debug('num fg avg: %d' % (self._fg_num / self._count))
            logger.debug('num bg avg: %d' % (self._bg_num / self._count))
            logger.debug('ratio: %.3f' % (float(self._fg_num) / float(self._bg_num)))

        for ind, val in enumerate([rois, labels, bbox_targets, bbox_weights]):
            self.assign(out_data[ind], req[ind], val)

    def backward(self, req, out_grad, in_data, out_data, in_grad, aux):
        self.assign(in_grad[0], req[0], 0)
        self.assign(in_grad[1], req[1], 0)

@mx.operator.register('proposal_target')
class ProposalTargetProp(mx.operator.CustomOpProp):
    def __init__(self, num_classes, batch_images, batch_rois, fg_fraction='0.25'):
        super(ProposalTargetProp, self).__init__(need_top_grad=False)
        self._num_classes = int(num_classes)
        self._batch_images = int(batch_images)
        self._batch_rois = int(batch_rois)
        self._fg_fraction = float(fg_fraction)

    def list_arguments(self):
        return ['rois', 'gt_boxes']

    def list_outputs(self):
        return ['rois_output', 'label', 'bbox_target', 'bbox_weight']

    def infer_shape(self, in_shape):
        rpn_rois_shape = in_shape[0]
        gt_boxes_shape = in_shape[1]

        output_rois_shape = (self._batch_rois, 5)
        label_shape = (self._batch_rois, )
        bbox_target_shape = (self._batch_rois, self._num_classes * 4)
        bbox_weight_shape = (self._batch_rois, self._num_classes * 4)

        return [rpn_rois_shape, gt_boxes_shape], \
               [output_rois_shape, label_shape, bbox_target_shape, bbox_weight_shape]

    def create_operator(self, ctx, shapes, dtypes):
        return ProposalTargetOperator(self._num_classes, self._batch_images, self._batch_rois, self._fg_fraction)

    def declare_backward_dependency(self, out_grad, in_data, out_data):
        return []
```

附录1： 

~/mx-rcnn/rcnn/io/rcnn.py脚本的sample_rois函数，用来对roi做采样，让roi的总数以及正负样本比例保持在设定数值。

```python
def sample_rois(rois, fg_rois_per_image, rois_per_image, num_classes,
                labels=None, overlaps=None, bbox_targets=None, gt_boxes=None):
    """
    generate random sample of ROIs comprising foreground and background examples
    :param rois: all_rois [n, 4]; e2e: [n, 5] with batch_index
    :param fg_rois_per_image: foreground roi number
    :param rois_per_image: total roi number
    :param num_classes: number of classes
    :param labels: maybe precomputed
    :param overlaps: maybe precomputed (max_overlaps)
    :param bbox_targets: maybe precomputed
    :param gt_boxes: optional for e2e [n, 5] (x1, y1, x2, y2, cls)
    :return: (labels, rois, bbox_targets, bbox_weights)
    """
# bbox_overlaps函数是计算每个rois和ground truth的IOU，得到的overlaps维度是(2000+x,x)；
# gt_assignment是计算每个rois和哪个ground truth之间的IOU最大；overlaps是最大的IOU值；
# labels是每个rois对应IOU最大的ground truth的标签，维度是(2000+x,1)。
    if labels is None:
        overlaps = bbox_overlaps(rois[:, 1:].astype(np.float), gt_boxes[:, :4].astype(np.float))
        gt_assignment = overlaps.argmax(axis=1)
        overlaps = overlaps.max(axis=1)
        labels = gt_boxes[gt_assignment, 4]

    # foreground RoI with FG_THRESH overlap
# 接下来这部分是计算正样本roi的index。config.TRAIN.FG_THRESH默认是0.5，这里是将
# 和ground truth的最大IOU大于这个阈值的roi认为是foreground，也就是有object，
# 一般fg_indexes都不多，常见几十。如果fg_indexes的数量超过了原本设定
# 的fg_rois_per_image，那么就对fg_indexes做随机采样。
    fg_indexes = np.where(overlaps >= config.TRAIN.FG_THRESH)[0]
    # guard against the case when an image has fewer than fg_rois_per_image foreground RoIs
    fg_rois_per_this_image = np.minimum(fg_rois_per_image, fg_indexes.size)
    # Sample foreground regions without replacement
    if len(fg_indexes) > fg_rois_per_this_image:
        fg_indexes = npr.choice(fg_indexes, size=fg_rois_per_this_image, replace=False)

    # Select background RoIs as those within [BG_THRESH_LO, BG_THRESH_HI)
# 接下来这部分是计算负样本roi的index。config.TRAIN.BG_THRESH_HI默认是0.5，
# config.TRAIN.BG_THRESH_LO默认是0，因此当IOU介于两者之间则为background。
# 这里要注意的是在Fast RCNN中，下限采用的是0.1而不是0。同样当 bg_indexes的数量超
# 过了原本设定的bg_rois_per_this_image，那么就对bg_indexes做随机采样。
    bg_indexes = np.where((overlaps < config.TRAIN.BG_THRESH_HI) & (overlaps >= config.TRAIN.BG_THRESH_LO))[0]
    # Compute number of background RoIs to take from this image (guarding against there being fewer than desired)
    bg_rois_per_this_image = rois_per_image - fg_rois_per_this_image
    bg_rois_per_this_image = np.minimum(bg_rois_per_this_image, bg_indexes.size)
    # Sample foreground regions without replacement
    if len(bg_indexes) > bg_rois_per_this_image:
        bg_indexes = npr.choice(bg_indexes, size=bg_rois_per_this_image, replace=False)

    # indexes selected
# 前面对roi做foreground和background的采样后，roi的数量就从2000+x降为
# rois_per_image(默认是128)，具体的index保存在keep_indexes中。
# while语句用来保证roi正负样本总数等于rois_per_image，这只有在前面的roi正负样本
# 达不到设定的rois_per_image时候才会执行，因此最终都会保证正负roi总数为rois_per_image。
    keep_indexes = np.append(fg_indexes, bg_indexes)
    neg_idx = np.where(overlaps < config.TRAIN.FG_THRESH)[0]
    neg_rois = rois[neg_idx]
    # pad more to ensure a fixed minibatch size
    while keep_indexes.shape[0] < rois_per_image:
        gap = np.minimum(len(neg_rois), rois_per_image - keep_indexes.shape[0])
        gap_indexes = npr.choice(range(len(neg_rois)), size=gap, replace=False)
        keep_indexes = np.append(keep_indexes, neg_idx[gap_indexes])

    # select labels
# 最后得到的rois是从原来的rois变量中选择前面”精挑细选“得到roi正负样本，维度是(128,5)，
# 但要注意5中除了4个坐标外，另一列是index，而不是标签。roi对应的标签保存在labels变量中。
    labels = labels[keep_indexes]
    # set labels of bg_rois to be 0
    labels[fg_rois_per_this_image:] = 0
    rois = rois[keep_indexes]

    # load or compute bbox_target
# bbox_targets默认是None，所以执行else语句对bbox_targets进行分配，rois[:,1:]是
# 维度为(128,4)的roi坐标，gt_boxes[gt_assignment[keep_indexes], :4]是维度为(128,4)的ground truth，
# bbox_transform函数在之前介绍过是计算坐标的offset（这里是roi和ground truth的offset）。
#  config.TRAIN.BBOX_NORMALIZATION_PRECOMPUTED参数对应的条件语句是用来对bbox_target做归一化。
# bbox_target_data的维度是(128,5)，第一列是真实标签，剩下4列是回归的预测目标。
    if bbox_targets is not None:
        bbox_target_data = bbox_targets[keep_indexes, :]
    else:
        targets = bbox_transform(rois[:, 1:], gt_boxes[gt_assignment[keep_indexes], :4])
        if config.TRAIN.BBOX_NORMALIZATION_PRECOMPUTED:
            targets = ((targets - np.array(config.TRAIN.BBOX_MEANS))
                       / np.array(config.TRAIN.BBOX_STDS))
        bbox_target_data = np.hstack((labels[:, np.newaxis], targets))


# 调用expand_bbox_regression_targets函数计算bbox_targets和bbox_weights，
# 该函数的实现细节可以看最后的附录2。
    bbox_targets, bbox_weights = \
        expand_bbox_regression_targets(bbox_target_data, num_classes)

    return rois, labels, bbox_targets, bbox_weights
```

附录2： 

~mx.rcnn/rcnn/processing/bbox_regression.py脚本的expand_bbox_regression_targets函数如下：

```python
def expand_bbox_regression_targets(bbox_targets_data, num_classes):
    """
    expand from 5 to 4 * num_classes; only the right class has non-zero bbox regression targets
    :param bbox_targets_data: [k * 5]
    :param num_classes: number of classes
    :return: bbox target processed [k * 4 num_classes]
    bbox_weights ! only foreground boxes have bbox regression computation!
    """
# 对于VOC数据集的20类而言，bbox_targets和bbox_weights的维度都是(128,84)，128个roi，
# 每个roi有84个回归目标值？咋一听起来有点不可思议，其实这84个值中只有4个是回归目标值，
# 剩下80个都是0，那4个回归目标值的位置和该rois的标签是对应的。之所以这样做是为了和
# 回归支路（全连接层）的num_hidden参数统一。然后选择bbox_target_data的标签列（第一列，
# 也就是classes）大于0的为正样本对应的index：indexes。因此类别0是背景，真正object的类别要从1开始。
    classes = bbox_targets_data[:, 0]
    bbox_targets = np.zeros((classes.size, 4 * num_classes), dtype=np.float32)
    bbox_weights = np.zeros(bbox_targets.shape, dtype=np.float32)
    indexes = np.where(classes > 0)[0]

# 接下来的循环对每个正样本的index都分配对应的回归目标bbox_targets_data[index, 1:]
# 给变量bbox_targets[index, start:end]，同样的，分配对应的
# 回归权重config.TRAIN.BBOX_WEIGHTS给变量bbox_weights[index, start:end]，权重默认是4个1。
# 因此对于负样本而言，bbox_targets和bbox_weights对应位置都为0，
# 后者在构建回归支路会乘以smooth_l1的结果，实现的效果就是只有正样本的坐标预测值参与损失回传和梯度更新。
    for index in indexes:
        cls = classes[index]
        start = int(4 * cls)
        end = start + 4
        bbox_targets[index, start:end] = bbox_targets_data[index, 1:]
        bbox_weights[index, start:end] = config.TRAIN.BBOX_WEIGHTS
    return bbox_targets, bbox_weights
```

**至此，关于Faster RCNN算法的核心内容基本上就介绍完了，重点还是网络结构的构建、RPN网络中anchor的生成和标签定义、检测网络（Fast RCNN）中proposal的确定和标签的定义等**。

接下来一篇博客要介绍下该算法的评价函数，也就是在训练代码过程中呈现出来的一些评价变量的值，比如损失函数值、准确率等等，**要注意的是这里定义的评价函数值只是在训练界面展示用，跟网络回传的值没有关系，所以即便你写错了计算公式，也仅仅是展示出来结果异常，而代码本身还是会正常训练的**。接下来就来看看：[Faster RCNN代码详解（六）：自定义评价函数](https://blog.csdn.net/u014380165/article/details/80381417)。



