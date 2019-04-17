# SSD算法代码介绍（七）：模型测试的评价函数 - AI之路 - CSDN博客





2018年02月17日 17:14:57[AI之路](https://me.csdn.net/u014380165)阅读数：1505








这篇博客介绍测试过程中的评价函数，在MXNet框架下都可以通过继承mx.metric.EvalMetric类进行实现。

该项目的evaluate文件夹下的一个脚本eval_metric.py定义了关于测试过程中的评价函数。这个脚本主要涉及两个类：MApMetric和VOC07MApMetric，后者是继承前者并重写了一些方法得到的，因此MApMetric类是核心。这两者都是用来计算object detection算法中的MAp（Mean avearage precision）。

```python
import mxnet as mx
import numpy as np
import os
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

class MApMetric(mx.metric.EvalMetric):
    """
    Calculate mean AP for object detection task

    Parameters:
    ---------
    ovp_thresh : float
        overlap threshold for TP
    use_difficult : boolean
        use difficult ground-truths if applicable, otherwise just ignore
    class_names : list of str
        optional, if provided, will print out AP for each class
    pred_idx : int
        prediction index in network output list
    roc_output_path
        optional, if provided, will save a ROC graph for each class
    tensorboard_path
        optional, if provided, will save a ROC graph to tensorboard
    """
    # __init__中还是执行常规的重置操作：reset()和一些赋值操作。
    def __init__(self, ovp_thresh=0.5, use_difficult=False, class_names=None,
                 pred_idx=0, roc_output_path=None, tensorboard_path=None):
        super(MApMetric, self).__init__('mAP')
        if class_names is None:
            self.num = None
        else:
            assert isinstance(class_names, (list, tuple))
            for name in class_names:
                assert isinstance(name, str), "must provide names as str"
            num = len(class_names)
            self.name = class_names + ['mAP']
            self.num = num + 1
        self.reset()
        self.ovp_thresh = ovp_thresh
        self.use_difficult = use_difficult
        self.class_names = class_names
        self.pred_idx = int(pred_idx)
        self.roc_output_path = roc_output_path
        self.tensorboard_path = tensorboard_path

    def save_roc_graph(self, recall=None, prec=None, classkey=1, path=None, ap=None):
        if not os.path.exists(path):
            os.mkdir(path)
        plot_path = os.path.join(path, 'roc_'+self.class_names[classkey])
        if os.path.exists(plot_path):
            os.remove(plot_path)
        fig = plt.figure()
        plt.title(self.class_names[classkey])
        plt.plot(recall, prec, 'b', label='AP = %0.2f' % ap)
        plt.legend(loc='lower right')
        plt.xlim([0, 1])
        plt.ylim([0, 1])
        plt.ylabel('Precision')
        plt.xlabel('Recall')
        plt.savefig(plot_path)
        plt.close(fig)

    def reset(self):
        """Clear the internal statistics to initial state."""
        if getattr(self, 'num', None) is None:
            self.num_inst = 0
            self.sum_metric = 0.0
        else:
            self.num_inst = [0] * self.num
            self.sum_metric = [0.0] * self.num
        self.records = dict()
        self.counts = dict()


# 当代码要读取MAp值时就会调用get方法，在get方法中通过调用_update方法计算self.records变量得到MAp值。
# 因为self.sum_metric和self.num_inst在这里是list，所以通过循环读取的方式最后返回tuple。
    def get(self):
        """Get the current evaluation result.

        Returns
        -------
        name : str
           Name of the metric.
        value : float
           Value of the evaluation.
        """
        self._update()  # update metric at this time
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


# update方法是更新MAp值的方法，目的是更新self.records变量。然后当代码要读取MAp值时就会调用get方法，
# 在get方法中通过再调用_update方法计算self.records变量得到MAp值。
    def update(self, labels, preds):
        """
        Update internal records. This function now only update internal buffer,
        sum_metric and num_inst are updated in _update() function instead when
        get() is called to return results.

        Params:
        ----------
        labels: mx.nd.array (n * 6) or (n * 5), difficult column is optional
            2-d array of ground-truths, n objects(id-xmin-ymin-xmax-ymax-[difficult])
        preds: mx.nd.array (m * 6)
            2-d array of detections, m objects(id-score-xmin-ymin-xmax-ymax)
        """
# IOU计算函数，就是计算两个框的交集面积除以并集面积的结果
        def iou(x, ys):
            """
            Calculate intersection-over-union overlap
            Params:
            ----------
            x : numpy.array
                single box [xmin, ymin ,xmax, ymax]
            ys : numpy.array
                multiple box [[xmin, ymin, xmax, ymax], [...], ]
            Returns:
            -----------
            numpy.array
                [iou1, iou2, ...], size == ys.shape[0]
            """
            ixmin = np.maximum(ys[:, 0], x[0])
            iymin = np.maximum(ys[:, 1], x[1])
            ixmax = np.minimum(ys[:, 2], x[2])
            iymax = np.minimum(ys[:, 3], x[3])
            iw = np.maximum(ixmax - ixmin, 0.)
            ih = np.maximum(iymax - iymin, 0.)
            inters = iw * ih
            uni = (x[2] - x[0]) * (x[3] - x[1]) + (ys[:, 2] - ys[:, 0]) * \
                (ys[:, 3] - ys[:, 1]) - inters
            ious = inters / uni
            ious[uni < 1e-12] = 0  # in case bad boxes
            return ious

        # independant execution for each image
# labels变量放的是batch size个图像的N个object的类别和坐标信息（非object的类别用-1表示），
# preds则是网络的输出（包含4个，这里取最后一个得到batch size个图像的M个anchor的预测类别、置信度和坐标信息）。
# 这个大的for循环就是循环batch中的每张图像。
        for i in range(labels[0].shape[0]):
            # get as numpy arrays
            label = labels[0][i].asnumpy()
            pred = preds[self.pred_idx][i].asnumpy()
            # calculate for each class
            while (pred.shape[0] > 0):
# 每次循环都去pred（二维）的第一行的第一列，该值是第一个anchor的预测类别，后面会把属于该类别的预测值都copy到别的变量，
# 然后将pred中该类别的预测值都删掉，所以每次循环时pred[0,0]的值都会变化，变化的次数就是你的类别数
                cid = int(pred[0, 0])
                indices = np.where(pred[:, 0].astype(int) == cid)[0]
# 如果是背景类别，则从pred变量中删除
                if cid < 0:
                    pred = np.delete(pred, indices, axis=0)
                    continue
# 将属于该预测类别的预测值copy给dets，然后从pred中删除该预测类别的预测值
                dets = pred[indices]
                pred = np.delete(pred, indices, axis=0)
                # sort by score, desceding
# 按照置信度从高到低进行排序，records的第二列用来记录每个预测值的tp（truth positive）和fp（false positive）值，
# 分别用1和2表示，初始化为0。
                dets[dets[:,1].argsort()[::-1]]
                records = np.hstack((dets[:, 1][:, np.newaxis], np.zeros((dets.shape[0], 1))))
                # ground-truths
# label_indices是输入的该图像中object类别等于前面预测的cid类别的object index，并将这些object的类别和位置信息保存在gts变量中
                label_indices = np.where(label[:, 0].astype(int) == cid)[0]
                gts = label[label_indices, :]
                label = np.delete(label, label_indices, axis=0)
# 如果真实的object类别和预测的cid类别有交集，则gts.size>0，否则跳过这个条件语句。
                if gts.size > 0:
                    found = [False] * gts.shape[0]
# 这个循环条件是遍历预测的类别值为cid的anchor，对每个anchor都计算其和真实的类别为cid的object框的IOU值。
# 取其中最大的IOU值赋给ovmax
                    for j in range(dets.shape[0]):
                        # compute overlaps
                        ious = iou(dets[j, 2:], gts[:, 1:5])
                        ovargmax = np.argmax(ious)
                        ovmax = ious[ovargmax]
# 当IOU大于ovp_thresh时候，因为gts.shape[1]==5，所以执行 records[j, -1] = 1
# 和found[ovargmax] = True。如果IOU没有达到这个阈值，则还是false positive。
                        if ovmax > self.ovp_thresh:
                            if (not self.use_difficult and
                                gts.shape[1] >= 6 and
                                gts[ovargmax, 5] > 0):
                                pass
                            else:
                                if not found[ovargmax]:
                                    records[j, -1] = 1  # tp
                                    found[ovargmax] = True
                                else:
                                    # duplicate
                                    records[j, -1] = 2  # fp
                        else:
# 这里相当于预测的类别在图像的所有object类别中都不存在，所以都是false positive
                            records[j, -1] = 2 # fp
                else:
                    # no gt, mark all fp
                    records[:, -1] = 2

                # ground truth count
                if (not self.use_difficult and gts.shape[1] >= 6):
                    gt_count = np.sum(gts[:, 5] < 1)
                else:
                    gt_count = gts.shape[0]

                # now we push records to buffer
                # first column: score, second column: tp/fp
                # 0: not set(matched to difficult or something), 1: tp, 2: fp
# 过滤掉records中既不是fp也不是tp的预测值，然后将符合条件的records通过_insert方法插入到self.records，
# 最后得到的self.records就是整个batch的总结果。
                records = records[np.where(records[:, -1] > 0)[0], :]
                if records.size > 0:
                    self._insert(cid, records, gt_count)

            # add missing class if not present in prediction
            while (label.shape[0] > 0):
                cid = int(label[0, 0])
                label_indices = np.where(label[:, 0].astype(int) == cid)[0]
                label = np.delete(label, label_indices, axis=0)
                if cid < 0:
                    continue
                gt_count = label_indices.size
                self._insert(cid, np.array([[0, 0]]), gt_count)


#_update方法是作者自定义的一个内部方法，用来帮助算法在调用get方法的时候获取所需的计算值，要注意和update方法的差别。
# 该方法基于前面update方法计算得到的sel.records来计算ap，self.records是一个包含number class个键值对的字典。
# recall, prec = self._recall_prec(v, self.counts[k])是计算recall和precision，
# ap = self._average_precision(recall, prec)是计算平均的recall和precision。
    def _update(self):
        """ update num_inst and sum_metric """
        aps = []
        for k, v in self.records.items():
            recall, prec = self._recall_prec(v, self.counts[k])
            ap = self._average_precision(recall, prec)
            if self.roc_output_path is not None:
                self.save_roc_graph(recall=recall, prec=prec, classkey=k, path=self.roc_output_path, ap=ap)
            aps.append(ap)
# 因为k值是遍历所有object的类别，所以这里self.sum_metric[k]放的就是k这个类别的ap值。
# 因此最后在界面上会显示每个类别的MAp值。
            if self.num is not None and k < (self.num - 1):
                self.sum_metric[k] = ap
                self.num_inst[k] = 1
        if self.num is None:
            self.num_inst = 1
            self.sum_metric = np.mean(aps)
# 在sum_metric和self.num_inst的最后位置插入平均结果，所以在界面上会显示所有类别的平均MAp值。
        else:
            self.num_inst[-1] = 1
            self.sum_metric[-1] = np.mean(aps)

#_recall_prec方法是前面_update方法调用的一个辅助方法。
    def _recall_prec(self, record, count):
        """ get recall and precision from internal records """
        record = np.delete(record, np.where(record[:, 1].astype(int) == 0)[0], axis=0)
        sorted_records = record[record[:,0].argsort()[::-1]]
        tp = np.cumsum(sorted_records[:, 1].astype(int) == 1)
        fp = np.cumsum(sorted_records[:, 1].astype(int) == 2)
        if count <= 0:
            recall = tp * 0.0
        else:
            recall = tp / float(count)
        prec = tp.astype(float) / (tp + fp)
        return recall, prec

    def _average_precision(self, rec, prec):
        """
        calculate average precision

        Params:
        ----------
        rec : numpy.array
            cumulated recall
        prec : numpy.array
            cumulated precision
        Returns:
        ----------
        ap as float
        """
        # append sentinel values at both ends
        mrec = np.concatenate(([0.], rec, [1.]))
        mpre = np.concatenate(([0.], prec, [0.]))

        # compute precision integration ladder
        for i in range(mpre.size - 1, 0, -1):
            mpre[i - 1] = np.maximum(mpre[i - 1], mpre[i])

        # look for recall value changes
        i = np.where(mrec[1:] != mrec[:-1])[0]

        # sum (\delta recall) * prec
        ap = np.sum((mrec[i + 1] - mrec[i]) * mpre[i + 1])
        return ap

# 将有效的records插入到self.records变量中
    def _insert(self, key, records, count):
        """ Insert records according to key """
        if key not in self.records:
            assert key not in self.counts
            self.records[key] = records
            self.counts[key] = count
        else:
            self.records[key] = np.vstack((self.records[key], records))
            assert key in self.counts
            self.counts[key] += count


class VOC07MApMetric(MApMetric):
    """ Mean average precision metric for PASCAL V0C 07 dataset """
    def __init__(self, *args, **kwargs):
        super(VOC07MApMetric, self).__init__(*args, **kwargs)

    def _average_precision(self, rec, prec):
        """
        calculate average precision, override the default one,
        special 11-point metric

        Params:
        ----------
        rec : numpy.array
            cumulated recall
        prec : numpy.array
            cumulated precision
        Returns:
        ----------
        ap as float
        """
        ap = 0.
        for t in np.arange(0., 1.1, 0.1):
            if np.sum(rec >= t) == 0:
                p = 0
            else:
                p = np.max(prec[rec >= t])
            ap += p / 11.
        return ap
```



