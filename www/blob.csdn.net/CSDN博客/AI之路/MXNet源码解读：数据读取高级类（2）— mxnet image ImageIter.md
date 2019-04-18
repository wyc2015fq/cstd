# MXNet源码解读：数据读取高级类（2）— mxnet.image.ImageIter - AI之路 - CSDN博客





2018年03月28日 22:38:39[AI之路](https://me.csdn.net/u014380165)阅读数：1076








**mxnet.image.ImageIter接口同样继承自MXNet框架下的基础数据迭代器构造类mxnet.io.DataIter，该接口是python代码实现的图像数据迭代器，且既可读取.rec文件，也可以以图像+.lst方式来读取数据，这是和mxnet.io.ImageRecordIter接口不同的地方。**由于mxnet.image.ImageIter接口在以原图像+.lst文件形式读取数据时是基于python代码实现的，因此在速度上会比基于C++代码实现的mxnet.io.ImageRecordIter接口效率低，尤其是当数据是存储在机械硬盘上时。当然python实现版本可读性还是比较强的，接下来就介绍下该接口的源码，基本上代码都在github的~mxnet/python/mxnet/image/image.py脚本中。因此该博客是介绍该接口的源码，而关于如何使用该接口读取数据并应用在实际模型训练中可以参考另一篇博客：[MXNet如何用mxnet.image.ImageIter直接导入图像](https://blog.csdn.net/u014380165/article/details/74906061)

mxnet.image.ImageIter类[源码地址](https://github.com/apache/incubator-mxnet/blob/master/python/mxnet/image/image.py)

源码如下。由另一篇博客[MXNet源码解读：数据读取基础类—mxnet.io.DataIter](https://blog.csdn.net/u014380165/article/details/79720131)中关于mxnet.io.DataIter基础类的介绍可以看出主要关注的方法是初始化init和next方法。其他方法一般是这两个方法的辅助方法。

```python
class ImageIter(io.DataIter):
    """Image data iterator with a large number of augmentation choices.
    This iterator supports reading from both .rec files and raw image files.

    To load input images from .rec files, use `path_imgrec` parameter and to load from raw image
    files, use `path_imglist` and `path_root` parameters.

    To use data partition (for distributed training) or shuffling, specify `path_imgidx` parameter.

    Parameters
    ----------
    batch_size : int
        Number of examples per batch.
    data_shape : tuple
        Data shape in (channels, height, width) format.
        For now, only RGB image with 3 channels is supported.
    label_width : int, optional
        Number of labels per example. The default label width is 1.
    path_imgrec : str
        Path to image record file (.rec).
        Created with tools/im2rec.py or bin/im2rec.
    path_imglist : str
        Path to image list (.lst).
        Created with tools/im2rec.py or with custom script.
        Format: Tab separated record of index, one or more labels and relative_path_from_root.
    imglist: list
        A list of images with the label(s).
        Each item is a list [imagelabel: float or list of float, imgpath].
    path_root : str
        Root folder of image files.
    path_imgidx : str
        Path to image index file. Needed for partition and shuffling when using .rec source.
    shuffle : bool
        Whether to shuffle all images at the start of each iteration or not.
        Can be slow for HDD.
    part_index : int
        Partition index.
    num_parts : int
        Total number of partitions.
    data_name : str
        Data name for provided symbols.
    label_name : str
        Label name for provided symbols.
    kwargs : ...
        More arguments for creating augmenter. See mx.image.CreateAugmenter.
    """

# 数据读取操作是在初始化方法init上进行的。这里主要分两种情况：1、读取的是record文
# 件（后缀是.rec），那么读取数据就通过recordio.MXIndexedRecordIO接口或
# recordio.MXRecordIO接口来读取，取决于你是否提供了.idx文件，而.idx文件是在生
# 成.rec文件时附带生成的。不管是recordio.MXIndexedRecordIO接口还是
# recordio.MXRecordIO接口，都是调用底层的C++代码来读取数据，具体可以参看
# ~mxnet/python/mxnet/recordio.py脚本，因此效率跟和mxnet.io.ImageRecordIter
# 接口没有太大差别。2、读取的是原图像+.lst文件，那么读取数据就通过python的open语
# 句来读取，维护了一个imglist字典来存放读取到的.lst文件内容，其中键是index，值是
# 标签和图像路径组成的tuple。在初始化中还有一步比较重要：            
# if aug_list is None:
#    self.auglist = CreateAugmenter(data_shape, **kwargs)
# else:
#    self.auglist = aug_list
# 这个if语句是关于数据预处理的设置，可以看出如果你要添加一些数据预处理操作，可以在
# mxnet.image.ImageIter接口的aug_list参数处设置，如果不设置的话就采用默认的
# CreateAugmenter(data_shape, **kwargs)函数，而这个默认的函数基本上没进行什么
# 预处理操作。那么aug_list参数要怎么给呢？参考CreateAugmenter函数的返回值可以看
# 出（CreateAugmenter函数的介绍可以参考博客：），aug_list是一个列表，列表中的每
# 个值存放一种数据处理类，这些数据预处理类的定义也在imag.py脚本中，比如
# ColorJitterAug类用来调整亮度、对比度和色度，ResizeAug类用来对短边做resize到指
# 定尺寸等。最后，self.seq 这个变量，一般当你采用第一种数据读取方式时为None，当采
# 用第二种数据读取方式时有值。
    def __init__(self, batch_size, data_shape, label_width=1,
                 path_imgrec=None, path_imglist=None, path_root=None, path_imgidx=None,
                 shuffle=False, part_index=0, num_parts=1, aug_list=None, imglist=None,
                 data_name='data', label_name='softmax_label', **kwargs):
        super(ImageIter, self).__init__()
        assert path_imgrec or path_imglist or (isinstance(imglist, list))
        num_threads = os.environ.get('MXNET_CPU_WORKER_NTHREADS', 1)
        logging.info('Using %s threads for decoding...', str(num_threads))
        logging.info('Set enviroment variable MXNET_CPU_WORKER_NTHREADS to a'
                     ' larger number to use more threads.')
        class_name = self.__class__.__name__
        if path_imgrec:
            logging.info('%s: loading recordio %s...',
                         class_name, path_imgrec)
            if path_imgidx:
                self.imgrec = recordio.MXIndexedRecordIO(path_imgidx, path_imgrec, 'r')  # pylint: disable=redefined-variable-type
                self.imgidx = list(self.imgrec.keys)
            else:
                self.imgrec = recordio.MXRecordIO(path_imgrec, 'r')  # pylint: disable=redefined-variable-type
                self.imgidx = None
        else:
            self.imgrec = None

        if path_imglist:
            logging.info('%s: loading image list %s...', class_name, path_imglist)
            with open(path_imglist) as fin:
                imglist = {}
                imgkeys = []
                for line in iter(fin.readline, ''):
                    line = line.strip().split('\t')
                    label = nd.array([float(i) for i in line[1:-1]])
                    key = int(line[0])
                    imglist[key] = (label, line[-1])
                    imgkeys.append(key)
                self.imglist = imglist
        elif isinstance(imglist, list):
            logging.info('%s: loading image list...', class_name)
            result = {}
            imgkeys = []
            index = 1
            for img in imglist:
                key = str(index)  # pylint: disable=redefined-variable-type
                index += 1
                if len(img) > 2:
                    label = nd.array(img[:-1])
                elif isinstance(img[0], numeric_types):
                    label = nd.array([img[0]])
                else:
                    label = nd.array(img[0])
                result[key] = (label, img[-1])
                imgkeys.append(str(key))
            self.imglist = result
        else:
            self.imglist = None
        self.path_root = path_root

        self.check_data_shape(data_shape)
        self.provide_data = [(data_name, (batch_size,) + data_shape)]
        if label_width > 1:
            self.provide_label = [(label_name, (batch_size, label_width))]
        else:
            self.provide_label = [(label_name, (batch_size,))]
        self.batch_size = batch_size
        self.data_shape = data_shape
        self.label_width = label_width

        self.shuffle = shuffle
        if self.imgrec is None:
            self.seq = imgkeys
        elif shuffle or num_parts > 1:
            assert self.imgidx is not None
            self.seq = self.imgidx
        else:
            self.seq = None

        if num_parts > 1:
            assert part_index < num_parts
            N = len(self.seq)
            C = N // num_parts
            self.seq = self.seq[part_index * C:(part_index + 1) * C]
        if aug_list is None:
            self.auglist = CreateAugmenter(data_shape, **kwargs)
        else:
            self.auglist = aug_list
        self.cur = 0
        self.reset()

    def reset(self):
        """Resets the iterator to the beginning of the data."""
        if self.shuffle:
            random.shuffle(self.seq)
        if self.imgrec is not None:
            self.imgrec.reset()
        self.cur = 0

# next_sample方法是后面next方法中调用的一个辅助方法，用来读取图像数据。这里也是分
# 两种情况：1、读取的是record文件，一般self.seq为None，则通过
# self.imgrec.read()接口来读取数据，本质上调用的是C++底层代码。2、读取的是原图像
# +.lst文件，则通过self.read_image(fname)这一行来读取图像文件，这种读取方式就是
# 采用python来实现，后面会介绍read_image方法。
    def next_sample(self):
        """Helper function for reading in next sample."""
        if self.seq is not None:
            if self.cur >= len(self.seq):
                raise StopIteration
            idx = self.seq[self.cur]
            self.cur += 1
            if self.imgrec is not None:
                s = self.imgrec.read_idx(idx)
                header, img = recordio.unpack(s)
                if self.imglist is None:
                    return header.label, img
                else:
                    return self.imglist[idx][0], img
            else:
                label, fname = self.imglist[idx]
                return label, self.read_image(fname)
        else:
            s = self.imgrec.read()
            if s is None:
                raise StopIteration
            header, img = recordio.unpack(s)
            return header.label, img

# next方法是读取数据迭代器的下一个batch数据的方法。首先初始化得到batch_data和
# batch_label两个NDArray用来保存一个batch的数据和标签，注意batch_data初始化时的
# 维度是(batch_size,c,h,w)。label, s = self.next_sample()这一行是通过调用前面
# 介绍的next_sample()方法来读取数据，得到的是图像的二进制内容；然后
# self.imdecode(s)是对读取到的数据格式进行编码，返回MXNet框架的基础数据结构
# NDArray。self.check_valid_image(data)是检查数据是否有效。
# self.augmentation_transform(data)是对图像做预处理操作，
# self.postprocess_data(data)是对图像做维度顺序上的调整，这是因为读取和编码后的
# 图像默认是HWC，而模型需要的是CHW，因此需要做调整。最后return的时候需要通过
# io.DataBatch接口进行最后一层封装，这个接口在mxnet.io.ImageRecordIter那篇博客
# 中已经介绍了，没有太复杂的内容，只是把一些信息合并在一起。这个返回的DataBatch类
# 型数据就可以作为模型的输入了。
    def next(self):
        """Returns the next batch of data."""
        batch_size = self.batch_size
        c, h, w = self.data_shape
        batch_data = nd.empty((batch_size, c, h, w))
        batch_label = nd.empty(self.provide_label[0][1])
        i = 0
        try:
            while i < batch_size:
                label, s = self.next_sample()
                data = self.imdecode(s)
                try:
                    self.check_valid_image(data)
                except RuntimeError as e:
                    logging.debug('Invalid image, skipping:  %s', str(e))
                    continue
                data = self.augmentation_transform(data)
                assert i < batch_size, 'Batch size must be multiples of augmenter output length'
                batch_data[i] = self.postprocess_data(data)
                batch_label[i] = label
                i += 1
        except StopIteration:
            if not i:
                raise StopIteration

        return io.DataBatch([batch_data], [batch_label], batch_size - i)

    def check_data_shape(self, data_shape):
        """Checks if the input data shape is valid"""
        if not len(data_shape) == 3:
            raise ValueError('data_shape should have length 3, with dimensions CxHxW')
        if not data_shape[0] == 3:
            raise ValueError('This iterator expects inputs to have 3 channels.')

    def check_valid_image(self, data):
        """Checks if the input data is valid"""
        if len(data[0].shape) == 0:
            raise RuntimeError('Data shape is wrong')

# imdecode方法是对二进制图像内容编码。该方法其实在img = imdecode(s)这一行调用了
# 该类外部的一个同名函数imdecode来执行编码操作，具体可以看image.py脚本中的
# imdecode函数，最终返回的是NDArray。
    def imdecode(self, s):
        """Decodes a string or byte string to an NDArray.
        See mx.img.imdecode for more details."""
        def locate():
            """Locate the image file/index if decode fails."""
            if self.seq is not None:
                idx = self.seq[self.cur - 1]
            else:
                idx = self.cur - 1
            if self.imglist is not None:
                _, fname = self.imglist[idx]
                msg = "filename: {}".format(fname)
            else:
                msg = "index: {}".format(idx)
            return "Broken image " + msg
        try:
            img = imdecode(s)
        except Exception as e:
            raise RuntimeError("{}, {}".format(locate(), e))
        return img

# read_image方法是前面next_sample方法中读取图像数据时调用的方法，从代码可以看出采
# 用的是python的内置函数open()，返回的是图像的二进制内容。
    def read_image(self, fname):
        """Reads an input image `fname` and returns the decoded raw bytes.

        Example usage:
        ----------
        >>> dataIter.read_image('Face.jpg') # returns decoded raw bytes.
        """
        with open(os.path.join(self.path_root, fname), 'rb') as fin:
            img = fin.read()
        return img

# augmentation_transform方法用来管理数据预处理，前面介绍过self.auglist列表存的
# 是数据预处理类，而该方法就是遍历列表中的数据预处理类，以此进行指定类操作。
    def augmentation_transform(self, data):
        """Transforms input data with specified augmentation."""
        for aug in self.auglist:
            data = aug(data)
        return data

# postprocess_data方法是做维度上的顺序调整，表示将原来的通道2调到0的位置，同时把
# 原来的通道0和1调到1和2的位置，这样就完成了从HWC到CHW的调整。
    def postprocess_data(self, datum):
        """Final postprocessing step before image is loaded into the batch."""
        return nd.transpose(datum, axes=(2, 0, 1))
```



