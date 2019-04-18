# MXNet框架如何自定义evaluation metric - AI之路 - CSDN博客





2017年10月22日 17:01:45[AI之路](https://me.csdn.net/u014380165)阅读数：4172
所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)









**使用深度学习框架训练模型的时候都需要用到评价标准，比如准确率等，那么在MXNet框架下，这些评价标准(Evaluation Metric)是怎么实现的呢？如果我们要自定义一个不一样的评价标准要怎么做？一起来了解下吧。**

首先来看看在MXNet框架下关于evaluation metric的最基本的类和脚本。`mxnet.metric.EvalMetric`是MXNet框架中计算评价标准（evaluation metric）的基础类，这个基础类是在MXNet项目的`/incubator-mxnet/python/mxnet/metric.py`中定义的。`metric.py`脚本中不仅包含类基础类的定义，还包含MXNet所有和metric相关的类的定义，所以如果你想更深入了解的话，可以看看这个脚本。

**先来介绍下如何自定义一个evaluation metric**。作者在文档中说过，这个类没有办法直接使用，当你要用的时候，应该要先自定义一个继承该基础类的类，将你想实现的评价标准写在里面，然后再在你的训练代码中调用该类。先看一个简单的定义一个evaluation metric的例子：

```python
import mxnet as mx
class Accuracy(mx.metric.EvalMetric): # 在定义类名称的时候，括号里面表示继承哪个类
    def __init__(self, num=None):
        super(Accuracy, self).__init__('accuracy', num)

    def update(self, labels, preds):
        pred_label = mx.nd.argmax_channel(preds[0]).asnumpy().astype('int32')
        label = labels[0].asnumpy().astype('int32')

        mx.metric.check_label_shapes(label, pred_label)

        self.sum_metric += (pred_label.flat == label.flat).sum()
        self.num_inst += len(pred_label.flat)
```

在这个自定义类中主要包含一个`__init__`    函数和一个`update`函数，前者是用来初始化，后者是用来更新metric，比如说你定义每计算一个batch的样本，就更新一次metric。**updata函数的输入中，labels和preds都是一个NDArray的列表**。在update函数中有个`mx.metric.check_label_shaoes`函数，这个函数的定义也是在MXNet项目的`/incubator-mxnet/python/mxnet/metric.py`脚本中，是用来判断labels和preds的shape是否一致，因为labels和preds都是list，而且一般这个list中只包含一个NDArray，比如说你的batch size是16，类别数是1000，那么labels中的NDArray就是16*1，preds中的NDArray就是16*1000。

以上这些代码可以写在类似名字为`my_metric.py`脚本中，**那么怎么调用了**？在你的训练代码中，可以通过`mxnet.metric.CompositeEvalMetric`类来调用，这个类是用来管理你的evaluation metrics的，这个类的定义也是在MXNet项目的`/incubator-mxnet/python/mxnet/metric.py`脚本中。先来看看官方的关于使用这个类的例子：

```
>>> predicts = [mx.nd.array([[0.3, 0.7], [0, 1.], [0.4, 0.6]])]
>>> labels   = [mx.nd.array([0, 1, 1])]
>>> eval_metrics_1 = mx.metric.Accuracy()
>>> eval_metrics_2 = mx.metric.F1()
>>> eval_metrics = mx.metric.CompositeEvalMetric()
>>> for child_metric in [eval_metrics_1, eval_metrics_2]:
>>>     eval_metrics.add(child_metric)
>>> eval_metrics.update(labels = labels, preds = predicts) >>> print eval_metrics.get()
    (['accuracy', 'f1'], [0.6666666666666666, 0.8])
```

这里`mx.metric.Accuracy(`)和`mx.metric.F1()`就是定义的metric类的实现，当你要调用自定义的metric时，主要用到这个·CompositeEvalMetric·类的add方法，用一次add方法就能够增加一种评价方式，从而在训练界面显示，而最后的update方法是手动更新计算结果，一般我们仅需下面两行定义一个eval_metric即可，然后把eval_metric作为训练的fit函数的一个参数即可：

```
eval_metric = mx.metric.CompositeEvalMetric()
eval_metric.add(Accuracy())
```

在代码运行到要更新metric的时候（更具体的是`base_model.py`中的`self.metric(eval_metric, data_batch.label)`）这一行，然后会先调用`mxnet.metric.CompositeEvalMetric()`类的`update_dict()`函数，然后调用基类`mxnet.metric.EvalMetric`的`update_dict()`函数，

**上面说完了怎么自定义一个evaluation metric类以及如何调用自定义的metric类，那么下面就看看官方的**`/incubator-mxnet/python/mxnet/metric.py`**脚本中默认的一些metric类的写法是什么样的吧。非常喜欢MXNet代码的一个原因就是其代码注释和文档做得很好，看起来一目了然。**

```python
class Accuracy(EvalMetric): # 这里也是继承的mxnet.metric.EvalMetric这个类，只不过因为这两个类在同一个脚本中定义，因此就可以直接写EvalMetric
    """Computes accuracy classification score.

    The accuracy score is defined as

    .. math::
        \\text{accuracy}(y, \\hat{y}) = \\frac{1}{n} \\sum_{i=0}^{n-1}
        \\text{1}(\\hat{y_i} == y_i)

    Parameters
    ----------
    axis : int, default=1
        The axis that represents classes
    name : str
        Name of this metric instance for display.
    output_names : list of str, or None
        Name of predictions that should be used when updating with update_dict.
        By default include all predictions.
    label_names : list of str, or None
        Name of labels that should be used when updating with update_dict.
        By default include all labels.

    Examples
    --------
    >>> predicts = [mx.nd.array([[0.3, 0.7], [0, 1.], [0.4, 0.6]])]
    >>> labels   = [mx.nd.array([0, 1, 1])]
    >>> acc = mx.metric.Accuracy()
    >>> acc.update(preds = predicts, labels = labels)
    >>> print acc.get()
    ('accuracy', 0.6666666666666666)
    """
    def __init__(self, axis=1, name='accuracy',
                 output_names=None, label_names=None):
 # super这个函数是调用基类mx.metric.EvalMetric的__init__函数，__init__函数括号中的变量是要传递给基类的__init__函数的变量。super()括号中的Accuracy表示类名称。
        super(Accuracy, self).__init__(
            name, axis=axis,
            output_names=output_names, label_names=label_names)
        self.axis = axis

    def update(self, labels, preds):
        """Updates the internal evaluation result.

        Parameters
        ----------
        labels : list of `NDArray`
            The labels of the data.

        preds : list of `NDArray`
            Predicted values.
        """
        check_label_shapes(labels, preds)

        for label, pred_label in zip(labels, preds): # zip函数可以将输入的两个list的对应位置的值变成一个元组(tuple)，这样每个tuple就包含两个值，这两个值在这里都是NDArray格式。又因为pred_label的shape和label的shape是不一样的，所以都会进入下面这个if语句，也就是先将pred_label按行求出最大值所在的index，然后pred_label就和label是相同shape的NDArray了。
            if pred_label.shape != label.shape:
                pred_label = ndarray.argmax(pred_label, axis=self.axis)

# 先用asnumpy()方法将NDArray转换成numpy.ndarray，然后把数值转成32位int型，原来是浮点型
            pred_label = pred_label.asnumpy().astype('int32')
            label = label.asnumpy().astype('int32')

            check_label_shapes(label, pred_label)

# (pred_label.flat == label.flat) 会返回numpy.ndarray格式，内容是false或者true，表示相等或不相等，最后求一个和
            self.sum_metric += (pred_label.flat == label.flat).sum()
            self.num_inst += len(pred_label.flat)
```

**再看一个复杂一点的例子，也是官方的用于计算multi label的accuracy的类：**

```python
class Multi_Accuracy(mx.metric.EvalMetric):
    """Calculate accuracies of multi label"""

    def __init__(self, num=None):
        self.num = num
# super调用基类的__init__函数进行初始化，__init__其中还会调用reset方法，如果你没有重写reset方法，那就默认调用基类的reset方法
        super(Multi_Accuracy, self).__init__('multi-accuracy')


# 这里重写了基类的reset方法，因为原来基类的reset方法并不考虑self.num_inst和self.sum_metric是list的情况，因此基类里面的reset方法只是两行：self.num_inst = 0 和 self.sum_metric = 0.0。
    def reset(self):
        """Resets the internal evaluation result to initial state."""
        self.num_inst = 0 if self.num is None else [0] * self.num
        self.sum_metric = 0.0 if self.num is None else [0.0] * self.num


# update方法就是你要如何计算metric的实现
    def update(self, labels, preds):
        mx.metric.check_label_shapes(labels, preds)

        if self.num is not None:
            assert len(labels) == self.num

        for i in range(len(labels)):
            pred_label = mx.nd.argmax_channel(preds[i]).asnumpy().astype('int32')
            label = labels[i].asnumpy().astype('int32')

            mx.metric.check_label_shapes(label, pred_label)

            if self.num is None:
                self.sum_metric += (pred_label.flat == label.flat).sum()
                self.num_inst += len(pred_label.flat)
            else:
                self.sum_metric[i] += (pred_label.flat == label.flat).sum()
                self.num_inst[i] += len(pred_label.flat)


# get方法是计算当前的evaluation result。比如说你设定每计算20个batch后就计算一次准确率等结果，那么这个结果的计算就是调用这个get方法
    def get(self):
        """Gets the current evaluation result.

        Returns
        -------
        names : list of str
           Name of the metrics.
        values : list of float
           Value of the evaluations.
        """
        if self.num is None:
            return super(Multi_Accuracy, self).get()
        else:
            return zip(*(('%s-task%d'%(self.name, i), float('nan') if self.num_inst[i] == 0
                                                      else self.sum_metric[i] / self.num_inst[i])
                       for i in range(self.num)))

    def get_name_value(self):
        """Returns zipped name and value pairs.

        Returns
        -------
        list of tuples
            A (name, value) tuple list.
        """
        if self.num is None:
            return super(Multi_Accuracy, self).get_name_value()
        name, value = self.get()
        return list(zip(name, value))
```



