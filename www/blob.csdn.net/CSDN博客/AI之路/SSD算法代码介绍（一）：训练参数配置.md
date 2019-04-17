# SSD算法代码介绍（一）：训练参数配置 - AI之路 - CSDN博客





2018年02月17日 15:14:26[AI之路](https://me.csdn.net/u014380165)阅读数：17759








SSD算法是object detection领域比较经典的算法，github上有一个写得比较好的MXNet版本的实现代码，项目地址：[https://github.com/zhreshold/mxnet-ssd](https://github.com/zhreshold/mxnet-ssd)，目前该项目代码也已经并入MXNet官方github。想要本地实现可以参考项目地址中README.md的介绍或者参考博客：[SSD算法的MXNet实现](http://blog.csdn.net/u014380165/article/details/78219584)。

**接下来这一系列博客想介绍该代码中关于实现SSD算法的一些细节，也会涉及部分Python语言的巧妙代码，以训练模型为切入口展开介绍，最好按顺序阅读，详细注释已经在代码中给出。**

**这一篇博客介绍训练模型的入口代码：train.py脚本，该脚本主要包含一些参数设置和主函数。**

```python
import argparse
import tools.find_mxnet
import mxnet as mx
import os
import sys
from train.train_net import train_net

def parse_args():
    parser = argparse.ArgumentParser(description='Train a Single-shot detection network')
    parser.add_argument('--train-path', dest='train_path', help='train record to use',
                        default=os.path.join(os.getcwd(), 'data', 'train.rec'), type=str)
    parser.add_argument('--train-list', dest='train_list', help='train list to use',
                        default="", type=str)
    parser.add_argument('--val-path', dest='val_path', help='validation record to use',
                        default=os.path.join(os.getcwd(), 'data', 'val.rec'), type=str)
    parser.add_argument('--val-list', dest='val_list', help='validation list to use',
                        default="", type=str)
    parser.add_argument('--network', dest='network', type=str, default='vgg16_reduced',
                        help='which network to use')
    parser.add_argument('--batch-size', dest='batch_size', type=int, default=32,
                        help='training batch size')
    parser.add_argument('--resume', dest='resume', type=int, default=-1,
                        help='resume training from epoch n')
    parser.add_argument('--finetune', dest='finetune', type=int, default=-1,
                        help='finetune from epoch n, rename the model before doing this')
    parser.add_argument('--pretrained', dest='pretrained', help='pretrained model prefix',
                        default=os.path.join(os.getcwd(), 'model', 'vgg16_reduced'), type=str)
    parser.add_argument('--epoch', dest='epoch', help='epoch of pretrained model',
                        default=1, type=int)
    parser.add_argument('--prefix', dest='prefix', help='new model prefix',
                        default=os.path.join(os.getcwd(), 'model', 'ssd'), type=str)
    parser.add_argument('--gpus', dest='gpus', help='GPU devices to train with',
                        default='0', type=str)
    parser.add_argument('--begin-epoch', dest='begin_epoch', help='begin epoch of training',
                        default=0, type=int)
    parser.add_argument('--end-epoch', dest='end_epoch', help='end epoch of training',
                        default=240, type=int)
    parser.add_argument('--frequent', dest='frequent', help='frequency of logging',
                        default=20, type=int)
    parser.add_argument('--data-shape', dest='data_shape', type=int, default=300,
                        help='set image shape')
    parser.add_argument('--label-width', dest='label_width', type=int, default=350,
                        help='force padding label width to sync across train and validation')
    parser.add_argument('--lr', dest='learning_rate', type=float, default=0.004,
                        help='learning rate')
    parser.add_argument('--momentum', dest='momentum', type=float, default=0.9,
                        help='momentum')
    parser.add_argument('--wd', dest='weight_decay', type=float, default=0.0005,
                        help='weight decay')
    parser.add_argument('--mean-r', dest='mean_r', type=float, default=123,
                        help='red mean value')
    parser.add_argument('--mean-g', dest='mean_g', type=float, default=117,
                        help='green mean value')
    parser.add_argument('--mean-b', dest='mean_b', type=float, default=104,
                        help='blue mean value')
    parser.add_argument('--lr-steps', dest='lr_refactor_step', type=str, default='80, 160',
                        help='refactor learning rate at specified epochs')
    parser.add_argument('--lr-factor', dest='lr_refactor_ratio', type=str, default=0.1,
                        help='ratio to refactor learning rate')
    parser.add_argument('--freeze', dest='freeze_pattern', type=str, default="^(conv1_|conv2_).*",
                        help='freeze layer pattern')
    parser.add_argument('--log', dest='log_file', type=str, default="train.log",
                        help='save training log to file')
    parser.add_argument('--monitor', dest='monitor', type=int, default=0,
                        help='log network parameters every N iters if larger than 0')
    parser.add_argument('--pattern', dest='monitor_pattern', type=str, default=".*",
                        help='monitor parameter pattern, as regex')
    parser.add_argument('--num-class', dest='num_class', type=int, default=20,
                        help='number of classes')
    parser.add_argument('--num-example', dest='num_example', type=int, default=16551,
                        help='number of image examples')
    parser.add_argument('--class-names', dest='class_names', type=str,
                        default='aeroplane, bicycle, bird, boat, bottle, bus, \
                        car, cat, chair, cow, diningtable, dog, horse, motorbike, \
                        person, pottedplant, sheep, sofa, train, tvmonitor',
                        help='string of comma separated names, or text filename')
    parser.add_argument('--nms', dest='nms_thresh', type=float, default=0.45,
                        help='non-maximum suppression threshold')
    parser.add_argument('--overlap', dest='overlap_thresh', type=float, default=0.5,
                        help='evaluation overlap threshold')
    parser.add_argument('--force', dest='force_nms', type=bool, default=False,
                        help='force non-maximum suppression on different class')
    parser.add_argument('--use-difficult', dest='use_difficult', type=bool, default=False,
                        help='use difficult ground-truths in evaluation')
    parser.add_argument('--voc07', dest='use_voc07_metric', type=bool, default=True,
                        help='use PASCAL VOC 07 11-point metric')
    args = parser.parse_args()
    return args

def parse_class_names(args):
    """ parse # classes and class_names if applicable """
    num_class = args.num_class
    if len(args.class_names) > 0:
        if os.path.isfile(args.class_names):
            # try to open it to read class names
            with open(args.class_names, 'r') as f:
                class_names = [l.strip() for l in f.readlines()]
        else:
            class_names = [c.strip() for c in args.class_names.split(',')]
        assert len(class_names) == num_class, str(len(class_names))
        for name in class_names:
            assert len(name) > 0
    else:
        class_names = None
    return class_names

if __name__ == '__main__':
    args = parse_args()
    # context list
    ctx = [mx.gpu(int(i)) for i in args.gpus.split(',') if i.strip()]
    ctx = [mx.cpu()] if not ctx else ctx
    # class names if applicable
    class_names = parse_class_names(args)
    # start training
    train_net(args.network, args.train_path, # train_path是训练数据的.rec格式的路径
              args.num_class, args.batch_size, 
              args.data_shape, [args.mean_r, args.mean_g, args.mean_b],
              # resume如果大于0，表示从之前保存的检测模型开始训练，如果设置为-1，表示重新训练检测模型。
              args.resume, args.finetune, args.pretrained, 
              args.epoch, args.prefix, ctx, args.begin_epoch, args.end_epoch,
              # frequent就是模型每训练多少个batch显示训练结果，momentum是和梯度下降相关的参数，weight_decay是正则项的系数
              args.frequent, args.learning_rate, args.momentum, args.weight_decay, 
              args.lr_refactor_step, args.lr_refactor_ratio, # 这两个都是学习率策略的参数
              val_path=args.val_path,
              num_example=args.num_example,
              class_names=class_names, # object detection问题需要标出object的label，对应这边的class_names
              label_pad_width=args.label_width,
              # str，默认是“^(conv1_|conv2_).*”，是一个正则表达式，简单讲就是将conv1_开头和conv2_开头的参数都固定，
              # 比如conv1_1_weight，conv1_1_bias等，在模型更新过程中参数不变。
              freeze_layer_pattern=args.freeze_pattern, 
              iter_monitor=args.monitor,
              monitor_pattern=args.monitor_pattern,
              # 训练模型时候得到的log文件存放的路径和命名，代码中默认是叫train.log，放在当前目录下
              log_file=args.log_file, 
              # nms是去掉重复框的一个操作，这个nms_thresh是一个阈值，表示高于这个阈值就说明重复太厉害了，需要去掉
              nms_thresh=args.nms_thresh, 
              force_nms=args.force_nms,
              ovp_thresh=args.overlap_thresh,
              # 在evaluation的时候用不同的ground-truths
              use_difficult=args.use_difficult, 
              voc07_metric=args.use_voc07_metric)
```

**从上面的代码可以看出，主要的训练代码是通过train_net.py脚本的train_net函数进行的，因此接下来就一起来看看train_net.py脚本的内容吧：[SSD算法代码介绍（二）：训练算法整体架构](https://blog.csdn.net/u014380165/article/details/79332365)。**



