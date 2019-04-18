# SSD算法代码介绍（六）：模型训练的评价函数 - AI之路 - CSDN博客





2018年02月17日 16:59:09[AI之路](https://me.csdn.net/u014380165)阅读数：1030








这篇博客介绍训练过程中的评价函数，在MXNet框架下都可以通过继承mx.metric.EvalMetric类进行实现。

该项目的train文件夹下的metric.py定义了一个类：MultiBoxMetric，该类可以作为训练时候分类和回归损失的计算。

```python
import mxnet as mx
import numpy as np

class MultiBoxMetric(mx.metric.EvalMetric):
    """Calculate metrics for Multibox training """

# __init__中指定了两个损失的名称，和其他两个参数，
# 最后调用了该类的reset方法重置了一些计数变量。
    def __init__(self, eps=1e-8):
        super(MultiBoxMetric, self).__init__('MultiBox')
        self.eps = eps
        self.num = 2
        self.name = ['CrossEntropy', 'SmoothL1']
        self.reset()


# reset方法是重置变量的方法
    def reset(self):
        """
        override reset behavior
        """
        if getattr(self, 'num', None) is None:
            self.num_inst = 0
            self.sum_metric = 0.0
        else:
            self.num_inst = [0] * self.num
            self.sum_metric = [0.0] * self.num


# update方法是每训练一个batch数据时就会运行的代码，最后返回分类的损失和回归的损失。
# cls_prob是模型预测的每个anchor的类别概率，
# cls_label是每个anchor的真实类别，loc_loss是回归损失。分类的损失是采用的交叉熵损失函数，
# 所以只有预测的概率对应的类别是真实类别的概率才会进入损失函数计算中，
# 也就是代码中的indices变量，
# 另一方面，对负样本（背景）概率的预测损失是不加入到分类损失中的，也就是代码中的mask变量，
# 综合起来就得到了prob变量作为交叉熵损失函数的输入。
# 回归损失在symbol_builder.py中构造symbol的时候就定义好了，
# 所以这里不需要过多处理，直接累加更新即可。
    def update(self, labels, preds):
        """
        Implementation of updating metrics
        """
        # get generated multi label from network
        cls_prob = preds[0].asnumpy()
        loc_loss = preds[1].asnumpy()
        cls_label = preds[2].asnumpy()
        valid_count = np.sum(cls_label >= 0)
        # overall accuracy & object accuracy
        label = cls_label.flatten()
        # in case you have a 'other' class
        label[np.where(label >= cls_prob.shape[1])] = 0
        mask = np.where(label >= 0)[0]
        indices = np.int64(label[mask])
        prob = cls_prob.transpose((0, 2, 1)).reshape((-1, cls_prob.shape[1]))
        prob = prob[mask, indices]
        self.sum_metric[0] += (-np.log(prob + self.eps)).sum()
        self.num_inst[0] += valid_count
        # smoothl1loss
        self.sum_metric[1] += np.sum(loc_loss)
        self.num_inst[1] += valid_count

# 当要获取该评价函数的值时就会调用get方法，
# 该方法的作用就是返回前面update方法中计算的self.sum_metric和self.num_inst值。
    def get(self):
        """Get the current evaluation result.
        Override the default behavior

        Returns
        -------
        name : str
           Name of the metric.
        value : float
           Value of the evaluation.
        """
        if self.num is None:
            if self.num_inst == 0:
                return (self.name, float('nan'))
            else:
                return (self.name, self.sum_metric / self.num_inst)
        else:
            names = ['%s'%(self.name[i]) for i in range(self.num)]
            values = [x / y if y != 0 else float('nan') \
                for x, y in zip(self.sum_metric, self.num_inst)]
            return (names, values)
```

**测试过程的评价函数可以和训练过程不一样，接下来一篇博客介绍该项目中测试过程的评价函数定义，涉及object detection类算法中常用的mAP计算：[SSD算法代码介绍（七）：模型测试的评价函数](https://blog.csdn.net/u014380165/article/details/79332625)。**



