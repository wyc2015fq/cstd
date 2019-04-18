# MXNet的数据读取：data.py源码详解 - AI之路 - CSDN博客





2017年06月29日 22:36:06[AI之路](https://me.csdn.net/u014380165)阅读数：2092
所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)









**这篇博文介绍MXNet里面比较重要的一个脚本data.py，路径：`~/mxnet/example/image-classification/common/data.py`。也是在fine-tune.py脚本中用到的数据读入脚本（参考[MXNet的fine-tune.py源码详解](http://blog.csdn.net/u014380165/article/details/73864457)）。数据读取在深度学习模型训练中算是比较重要的一块，MXNet官网中主要介绍的是.rec格式的数据的读取，不过这个格式比较占磁盘空间（正常情况下和图像所占磁盘空间差不多），虽然一次读入比较省事。但是还是建议其他格式的读入比如利用lst文件从磁盘读入图像，SSD的读取速度还是可以的。这个脚本主要包括数据的一些配置以及最重要的数据导入，接下来直接解读源码。**

```python
import mxnet as mx
import random
from mxnet.io import DataBatch, DataIter
import numpy as np


#这个函数主要是输入数据的一些配置，比如最重要的--data-train和--data-val是训练和验证数据集，
# --rgb-mean是均值，--num-class是类别数，--num-examples是训练样本数
def add_data_args(parser):
    data = parser.add_argument_group('Data', 'the input images')
    data.add_argument('--data-train', type=str, help='the training data')
    data.add_argument('--data-val', type=str, help='the validation data')
    data.add_argument('--rgb-mean', type=str, default='123.68,116.779,103.939',
                      help='a tuple of size 3 for the mean rgb')
    data.add_argument('--pad-size', type=int, default=0,
                      help='padding the input image')
    data.add_argument('--image-shape', type=str,
                      help='the image shape feed into the network, e.g. (3,224,224)')
    data.add_argument('--num-classes', type=int, help='the number of classes')
    data.add_argument('--num-examples', type=int, help='the number of training examples')
    data.add_argument('--data-nthreads', type=int, default=4,
                      help='number of threads for data decoding')
    data.add_argument('--benchmark', type=int, default=0,
                      help='if 1, then feed the network with synthetic data')
    data.add_argument('--dtype', type=str, default='float32',
                      help='data type: float32 or float16')
    return data


#这个函数也是数据的一些配置，包括随机裁剪，镜像等等
def add_data_aug_args(parser):
    aug = parser.add_argument_group(
        'Image augmentations', 'implemented in src/io/image_aug_default.cc')
    aug.add_argument('--random-crop', type=int, default=1,
                     help='if or not randomly crop the image')
    aug.add_argument('--random-mirror', type=int, default=1,
                     help='if or not randomly flip horizontally')
    aug.add_argument('--max-random-h', type=int, default=0,
                     help='max change of hue, whose range is [0, 180]')
    aug.add_argument('--max-random-s', type=int, default=0,
                     help='max change of saturation, whose range is [0, 255]')
    aug.add_argument('--max-random-l', type=int, default=0,
                     help='max change of intensity, whose range is [0, 255]')
    aug.add_argument('--max-random-aspect-ratio', type=float, default=0,
                     help='max change of aspect ratio, whose range is [0, 1]')
    aug.add_argument('--max-random-rotate-angle', type=int, default=0,
                     help='max angle to rotate, whose range is [0, 360]')
    aug.add_argument('--max-random-shear-ratio', type=float, default=0,
                     help='max ratio to shear, whose range is [0, 1]')
    aug.add_argument('--max-random-scale', type=float, default=1,
                     help='max ratio to scale')
    aug.add_argument('--min-random-scale', type=float, default=1,
                     help='min ratio to scale, should >= img_size/input_shape. otherwise use --pad-size')
    return aug

def set_data_aug_level(aug, level):
    if level >= 1:
        aug.set_defaults(random_crop=1, random_mirror=1)
    if level >= 2:
        aug.set_defaults(max_random_h=36, max_random_s=50, max_random_l=50)
    if level >= 3:
        aug.set_defaults(max_random_rotate_angle=10, max_random_shear_ratio=0.1, max_random_aspect_ratio=0.25)


class SyntheticDataIter(DataIter):
    def __init__(self, num_classes, data_shape, max_iter, dtype):
        self.batch_size = data_shape[0]
        self.cur_iter = 0
        self.max_iter = max_iter
        self.dtype = dtype
        label = np.random.randint(0, num_classes, [self.batch_size,])
        data = np.random.uniform(-1, 1, data_shape)
        self.data = mx.nd.array(data, dtype=self.dtype)
        self.label = mx.nd.array(label, dtype=self.dtype)
    def __iter__(self):
        return self
    @property
    def provide_data(self):
        return [mx.io.DataDesc('data', self.data.shape, self.dtype)]
    @property
    def provide_label(self):
        return [mx.io.DataDesc('softmax_label', (self.batch_size,), self.dtype)]
    def next(self):
        self.cur_iter += 1
        if self.cur_iter <= self.max_iter:
            return DataBatch(data=(self.data,),
                             label=(self.label,),
                             pad=0,
                             index=None,
                             provide_data=self.provide_data,
                             provide_label=self.provide_label)
        else:
            raise StopIteration
    def __next__(self):
        return self.next()
    def reset(self):
        self.cur_iter = 0


# 这个函数比较重要，也是在训练的时候fit函数在获取数据的时候调用的函数，这个函数通过mxnet.io.ImageRecordIter，
# 可以从.rec文件生成data_iter然后作为fit函数的输入之一。
def get_rec_iter(args, kv=None):
    image_shape = tuple([int(l) for l in args.image_shape.split(',')])
    dtype = np.float32;
    if 'dtype' in args:
        if args.dtype == 'float16':
            dtype = np.float16
    if 'benchmark' in args and args.benchmark:
        data_shape = (args.batch_size,) + image_shape
        train = SyntheticDataIter(args.num_classes, data_shape, 50, dtype)
        return (train, None)
    if kv:
        (rank, nworker) = (kv.rank, kv.num_workers)
    else:
        (rank, nworker) = (0, 1)
    rgb_mean = [float(i) for i in args.rgb_mean.split(',')]

#这里采用的是mxnet.io.ImageRecordlter这个函数，这个函数的具体介绍可以参考：
#http://mxnet.io/api/python/io.html#mxnet.io.ImageRecordIter，因为这个函数必须要以.rec文件作为输入，
#这个文件还是比较占磁盘空间的，有点像Caffe中的lmdb。因此推荐另一种读取数据的函数：mxnet.image.ImageIter，
#这个函数既可以以.rec作为输入，也可以以图像和.lst文件作为输入，写起来也比较简单。
    train = mx.io.ImageRecordIter(
        path_imgrec         = args.data_train,
        label_width         = 1, #表示一个图像对应一个标签，如果你是要进行多标签分类的话，这个地方需要做修改
        mean_r              = rgb_mean[0], #这三个是图像均值，r，g，b三个通道
        mean_g              = rgb_mean[1],
        mean_b              = rgb_mean[2],
        data_name           = 'data',
        label_name          = 'softmax_label',
        data_shape          = image_shape,
        batch_size          = args.batch_size,
        rand_crop           = args.random_crop, #随机裁剪
        max_random_scale    = args.max_random_scale,
        pad                 = args.pad_size,
        fill_value          = 127,
        min_random_scale    = args.min_random_scale,
        max_aspect_ratio    = args.max_random_aspect_ratio,
        random_h            = args.max_random_h,
        random_s            = args.max_random_s,
        random_l            = args.max_random_l,
        max_rotate_angle    = args.max_random_rotate_angle,
        max_shear_ratio     = args.max_random_shear_ratio,
        rand_mirror         = args.random_mirror,
        preprocess_threads  = args.data_nthreads,
        shuffle             = True, #是否将样本随机排序
        num_parts           = nworker,
        part_index          = rank)
    if args.data_val is None:
        return (train, None)
    val = mx.io.ImageRecordIter(
        path_imgrec         = args.data_val,
        label_width         = 1,
        mean_r              = rgb_mean[0],
        mean_g              = rgb_mean[1],
        mean_b              = rgb_mean[2],
        data_name           = 'data',
        label_name          = 'softmax_label',
        batch_size          = args.batch_size,
        data_shape          = image_shape,
        preprocess_threads  = args.data_nthreads,
        rand_crop           = False,
        rand_mirror         = False,
        num_parts           = nworker,
        part_index          = rank)
    return (train, val)
```



