# Faster RCNN代码详解（六）：自定义评价函数 - AI之路 - CSDN博客





2018年05月20日 12:43:15[AI之路](https://me.csdn.net/u014380165)阅读数：3105








在前面几篇博文中基本上将Faster RCNN的算法细节都介绍完了，**这一篇博客主要来介绍评价函数。**自定义评价函数有助于我们灵活观察模型训练过程中的变量变化情况，比如RPN网络中的分类和回归损失在每次迭代过程中的变化情况，或者检测网络的分类和回归损失在每次迭代过程中的变化情况等。当然你还可以自定义mAP的计算函数等，跟下面要介绍的例子在结构上都是一样的。

自定义的评价函数类的具体实现所在脚本：~mx-rcnn/rcnn/core/metric.py，包含RPN网络的分类、回归损失计算和准确率计算，检测网络的分类、回归损失计算和准确率计算。

```python
import mxnet as mx
import numpy as np

from rcnn.config import config

# 这部分是获取RPN网络的变量名，因为RPN网络有多个输出，所以要根据变量名来读取对应的输出。
def get_rpn_names():
    pred = ['rpn_cls_prob', 'rpn_bbox_loss']
    label = ['rpn_label', 'rpn_bbox_target', 'rpn_bbox_weight']
    return pred, label

# 这部分是获取检测网络（Fast RCNN）的变量名，因为检测网络有多个输出，所以要根据变量名来读取对应的输出。
def get_rcnn_names():
    pred = ['rcnn_cls_prob', 'rcnn_bbox_loss']
    label = ['rcnn_label', 'rcnn_bbox_target', 'rcnn_bbox_weight']
    if config.TRAIN.END2END:
        pred.append('rcnn_label')
        rpn_pred, rpn_label = get_rpn_names()
        pred = rpn_pred + pred
        label = rpn_label
    return pred, label

# RPNAccMetric类是RPN网络的分类准确率计算。对于一张大小为600*800的输入图像而言，
# pred是1*2*342*50，label是1*17100(342*50)
class RPNAccMetric(mx.metric.EvalMetric):
    def __init__(self):
        super(RPNAccMetric, self).__init__('RPNAcc')
        self.pred, self.label = get_rpn_names()

# preds是包含多个输出的列表，所以通过index进行索引，labels同理。
    def update(self, labels, preds):
        pred = preds[self.pred.index('rpn_cls_prob')]
        label = labels[self.label.index('rpn_label')]

        # pred (b, c, p) or (b, c, h, w)
        pred_label = mx.ndarray.argmax_channel(pred).asnumpy().astype('int32')
        pred_label = pred_label.reshape((pred_label.shape[0], -1))
        # label (b, p)
        label = label.asnumpy().astype('int32')

        # filter with keep_inds
# 类别准确率的计算不包含无效标签。
        keep_inds = np.where(label != -1)
        pred_label = pred_label[keep_inds]
        label = label[keep_inds]

        self.sum_metric += np.sum(pred_label.flat == label.flat)
        self.num_inst += len(pred_label.flat)

# RCNNAccMetric类是检测网络的分类准确率计算。
class RCNNAccMetric(mx.metric.EvalMetric):
    def __init__(self):
        super(RCNNAccMetric, self).__init__('RCNNAcc')
        self.e2e = config.TRAIN.END2END
        self.pred, self.label = get_rcnn_names()

    def update(self, labels, preds):
        pred = preds[self.pred.index('rcnn_cls_prob')]
        if self.e2e:
            label = preds[self.pred.index('rcnn_label')]
        else:
            label = labels[self.label.index('rcnn_label')]

        last_dim = pred.shape[-1]
        pred_label = pred.asnumpy().reshape(-1, last_dim).argmax(axis=1).astype('int32')
        label = label.asnumpy().reshape(-1,).astype('int32')

        self.sum_metric += np.sum(pred_label.flat == label.flat)
        self.num_inst += len(pred_label.flat)

# RPNLogLossMetric类是用来计算RPN网络的分类损失。
class RPNLogLossMetric(mx.metric.EvalMetric):
    def __init__(self):
        super(RPNLogLossMetric, self).__init__('RPNLogLoss')
        self.pred, self.label = get_rpn_names()

    def update(self, labels, preds):
        pred = preds[self.pred.index('rpn_cls_prob')]
        label = labels[self.label.index('rpn_label')]

        # label (b, p)
        label = label.asnumpy().astype('int32').reshape((-1))
        # pred (b, c, p) or (b, c, h, w) --> (b, p, c) --> (b*p, c)
        pred = pred.asnumpy().reshape((pred.shape[0], pred.shape[1], -1)).transpose((0, 2, 1))
        pred = pred.reshape((label.shape[0], -1))

        # filter with keep_inds
# 分类loss不包含无效标签，分类loss的计算还是采用cross entropy loss。
        keep_inds = np.where(label != -1)[0]
        label = label[keep_inds]
        cls = pred[keep_inds, label]

        cls += 1e-14
        cls_loss = -1 * np.log(cls)
        cls_loss = np.sum(cls_loss)
        self.sum_metric += cls_loss
        self.num_inst += label.shape[0]

# RCNNLogLossMetric类是用来计算检测网络的分类损失。
class RCNNLogLossMetric(mx.metric.EvalMetric):
    def __init__(self):
        super(RCNNLogLossMetric, self).__init__('RCNNLogLoss')
        self.e2e = config.TRAIN.END2END
        self.pred, self.label = get_rcnn_names()

    def update(self, labels, preds):
        pred = preds[self.pred.index('rcnn_cls_prob')]
        if self.e2e:
            label = preds[self.pred.index('rcnn_label')]
        else:
            label = labels[self.label.index('rcnn_label')]

        last_dim = pred.shape[-1]
        pred = pred.asnumpy().reshape(-1, last_dim)
        label = label.asnumpy().reshape(-1,).astype('int32')
        cls = pred[np.arange(label.shape[0]), label]

        cls += 1e-14
        cls_loss = -1 * np.log(cls)
        cls_loss = np.sum(cls_loss)
        self.sum_metric += cls_loss
        self.num_inst += label.shape[0]

# RPNL1LossMetric类是用来计算RPN网络的回归损失。
class RPNL1LossMetric(mx.metric.EvalMetric):
    def __init__(self):
        super(RPNL1LossMetric, self).__init__('RPNL1Loss')
        self.pred, self.label = get_rpn_names()

    def update(self, labels, preds):
        bbox_loss = preds[self.pred.index('rpn_bbox_loss')].asnumpy()
        bbox_weight = labels[self.label.index('rpn_bbox_weight')].asnumpy()

        # calculate num_inst (average on those fg anchors)
# 只有包含object的bbox（标签是1）才计算loss，因此可以通过计算bbox_weight中大于0的个数并
# 除以4（4个坐标）来得到num_inst，num_inst也就是计算回归loss时bbox的数量（标签是1的bbox数量）。
# 另外因为在得到bbox_loss的时候就已经用rpn_bbox_weight相乘过了，
# 所以bbox_loss是非背景的预测框坐标损失，后面可以直接求和。
        num_inst = np.sum(bbox_weight > 0) / 4

        self.sum_metric += np.sum(bbox_loss)
        self.num_inst += num_inst

# RCNNL1LossMetric类是用来计算检测网络的回归损失。
class RCNNL1LossMetric(mx.metric.EvalMetric):
    def __init__(self):
        super(RCNNL1LossMetric, self).__init__('RCNNL1Loss')
        self.e2e = config.TRAIN.END2END
        self.pred, self.label = get_rcnn_names()

    def update(self, labels, preds):
        bbox_loss = preds[self.pred.index('rcnn_bbox_loss')].asnumpy()
        if self.e2e:
            label = preds[self.pred.index('rcnn_label')].asnumpy()
        else:
            label = labels[self.label.index('rcnn_label')].asnumpy()

        # calculate num_inst
        keep_inds = np.where(label != 0)[0]
        num_inst = len(keep_inds)

        self.sum_metric += np.sum(bbox_loss)
        self.num_inst += num_inst
```



