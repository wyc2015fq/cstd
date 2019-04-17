# MXNet源码解读：数据读取高级类（1）—mxnet.io.MXDataIter - AI之路 - CSDN博客





2018年03月27日 22:11:25[AI之路](https://me.csdn.net/u014380165)阅读数：1599
所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)









·**mxnet.io.ImageRecordIter接口是MXNet框架中用于图像算法相关的数据读取接口，只需将准备好的record文件（后缀是.rec）作为该接口的输入即可训练模型。mxnet.io.ImageRecordIter接口的源码是mxnet.io.MXDataIter类，该类同样继承自MXNet框架下的数据读取基础类mxnet.io.DataIter并重写了其中的一些方法。**在了解了比较抽象和基础的mxnet.io.DataIter类后（参考博客：[MXNet源码解读：数据读取基础类—mxnet.io.DataIter](https://blog.csdn.net/u014380165/article/details/79720131)），再来看实际构造数据迭代器时候使用的mxnet.io.MXDataIter类就会更加清晰。总的来说了解mxnet.io.MXDataIter类的实现细节对于了解MXNet框架的图像数据读取以及灵活封装更high level的API非常有帮助。

mxnet.io.MXDataIter类的[源码地址](https://raw.githubusercontent.com/apache/incubator-mxnet/master/python/mxnet/io.py)

源码如下。从注释可以看出该接口是C++底层数据迭代器的python封装，继承自mxnet.io.DataIter基础类。当你初始化一个mxnet.io.ImageRecordIter类时会得到一个MXDataIter实例，然后当你调用该实例的时候就会调用MXDataIter类的底层C++数据迭代器读取数据（后面会介绍是通过next方法实现的）。MXDataIter类是个非常基础的类，许多high level的数据读取接口都是调用该类来实现的，比如用于图像分类的mxnet.io.ImageRecordIter接口，用于目标检测的mxnet.io.ImageDetRecordIter接口以及CSVIter、MNISTIter等接口。

```python
class MXDataIter(DataIter):
    """A python wrapper a C++ data iterator.

    This iterator is the Python wrapper to all native C++ data iterators, such
    as `CSVIter`, `ImageRecordIter`, `MNISTIter`, etc. When initializing
    `CSVIter` for example, you will get an `MXDataIter` instance to use in your
    Python code. Calls to `next`, `reset`, etc will be delegated to the
    underlying C++ data iterators.

    Usually you don't need to interact with `MXDataIter` directly unless you are
    implementing your own data iterators in C++. To do that, please refer to
    examples under the `src/io` folder.

    Parameters
    ----------
    handle : DataIterHandle, required
        The handle to the underlying C++ Data Iterator.
    data_name : str, optional
        Data name. Default to "data".
    label_name : str, optional
        Label name. Default to "softmax_label".

    See Also
    --------
    src/io : The underlying C++ data iterator implementation, e.g., `CSVIter`.
    """

# init方法中self.first_batch = None，self.first_batch = self.next()这两行是
# 对第一个batch数据的初始化，返回的是DataBatch类数据，后面会介绍。然后
# self.provide_data = [DataDesc(data_name, data.shape, data.dtype)]， 
# self.provide_label = [DataDesc(label_name, label.shape, label.dtype)]通
# 过DataDesc类来保存数据相关的信息，具体而言是通过调用mxnet.io.DataDesc类的
# __new__方法来保存数据的name、shape、type、layout等信息，该类在
# mxnet.io.DataIter博客中已经介绍了。需要注意的是self.provide_data和
# self.provide_label都是列表形式，因此假设你新增一个层需要额外的label信息时，可
# 以直接在self.provide_label列表中添加。因为init方法在数据集初始化的时候就会调
# 用，得到的self.provide_data和self.provide_label在网络结构bind的时候会用到，
# 因此如果提供的数据信息不对，会在bind的时候报错。
    def __init__(self, handle, data_name='data', label_name='softmax_label', **_):
        super(MXDataIter, self).__init__()
        self.handle = handle
        # debug option, used to test the speed with io effect eliminated
        self._debug_skip_load = False

        # load the first batch to get shape information
        self.first_batch = None
        self.first_batch = self.next()
        data = self.first_batch.data[0]
        label = self.first_batch.label[0]

        # properties
        self.provide_data = [DataDesc(data_name, data.shape, data.dtype)]
        self.provide_label = [DataDesc(label_name, label.shape, label.dtype)]
        self.batch_size = data.shape[0]

    def __del__(self):
        check_call(_LIB.MXDataIterFree(self.handle))

    def debug_skip_load(self):
        # Set the iterator to simply return always first batch. This can be used
        # to test the speed of network without taking the loading delay into
        # account.
        self._debug_skip_load = True
        logging.info('Set debug_skip_load to be true, will simply return first batch')

    def reset(self):
        self._debug_at_begin = True
        self.first_batch = None
        check_call(_LIB.MXDataIterBeforeFirst(self.handle))


# next方法是该类的重要方法。当用mxnet.io.ImageRecordIter类实例化得到一个
# MXDataIter实例后，就基本上构造好了数据迭代器，然后就可以调用next方法来读取数据迭
# 代器的每个batch数据。_LIB是在import mxnet的时候就初始化得到的对象，准确讲_LIB
# 是在io/base.py脚本中通过ctypes模块的CDLL接口加载的~mxnet/lib/libmxnet.so对象
# （动态链接库），该对象也是我们拉取mxnet的github代码后编译生成的主要内容。ctypes
# 模块是Python中用来调用动态链接库函数的功能模块，后面还会用到。在next方法中最重要
# 的就是通过DataBatch接口进一步封装数据并返回，返回的DataBatch实例就可以用在
# forward和backward中调用了，DataBatch接口的输入中包含：1、data，通过
# self.getdata()方法得到，是NDArray的list，每个NDArray包含一个batch的数据；
# 2、label，通过self.getlabel()方法得到，是NDArray的list；3、pad，通过
# self.getpad()方法得到，是int；4、index，通过self.getindex()方法得到，
# 是size为batch size的numpy array。
    def next(self):
        if self._debug_skip_load and not self._debug_at_begin:
            return  DataBatch(data=[self.getdata()], label=[self.getlabel()], pad=self.getpad(),
                              index=self.getindex())
        if self.first_batch is not None:
            batch = self.first_batch
            self.first_batch = None
            return batch
        self._debug_at_begin = False
        next_res = ctypes.c_int(0)
        check_call(_LIB.MXDataIterNext(self.handle, ctypes.byref(next_res)))
        if next_res.value:
            return DataBatch(data=[self.getdata()], label=[self.getlabel()], pad=self.getpad(),
                             index=self.getindex())
        else:
            raise StopIteration

    def iter_next(self):
        if self.first_batch is not None:
            return True
        next_res = ctypes.c_int(0)
        check_call(_LIB.MXDataIterNext(self.handle, ctypes.byref(next_res)))
        return next_res.value

# getdata方法中NDArrayHandle()就是ctypes模块中的c_void_p，也就是类似C中的
# void *。_LIB的含义在前面介绍next方法的时候也已经说明了，简单讲就是动态链接库对
# 象，因此这里读取数据都是调用底层的C++代码实现的，所以可以达到较高的读取效率。该方
# 法最后返回的就是NDArray，维度是NCHW。
    def getdata(self):
        hdl = NDArrayHandle()
        check_call(_LIB.MXDataIterGetData(self.handle, ctypes.byref(hdl)))
        return _ndarray_cls(hdl, False)

# getlabel方法和getdata方法类似
    def getlabel(self):
        hdl = NDArrayHandle()
        check_call(_LIB.MXDataIterGetLabel(self.handle, ctypes.byref(hdl)))
        return _ndarray_cls(hdl, False)

# getindex方法同样用到了ctypes模块，这里用来调用C/C++底层代码。c_uint64是ctypes
# 中的数据类型，与C和Python中的指定数据类型对应；POINTER则是ctypes中创建指针用
# 的。index_size.value对应batch size。
    def getindex(self):
        index_size = ctypes.c_uint64(0)
        index_data = ctypes.POINTER(ctypes.c_uint64)()
        check_call(_LIB.MXDataIterGetIndex(self.handle,
                                           ctypes.byref(index_data),
                                           ctypes.byref(index_size)))
        if index_size.value:
            address = ctypes.addressof(index_data.contents)
            dbuffer = (ctypes.c_uint64* index_size.value).from_address(address)
            np_index = np.frombuffer(dbuffer, dtype=np.uint64)
            return np_index.copy()
        else:
            return None

# getpad方法只有在数据不满足batch size的时候才会有值，并指导后面构造batch数据时候
# 进行填充。
    def getpad(self):
        pad = ctypes.c_int(0)
        check_call(_LIB.MXDataIterGetPadNum(self.handle, ctypes.byref(pad)))
        return pad.value
```

mxnet.io.DataBatch接口是MxNet框架中数据封装的最后一步，在mxnet.io.MXDataIter类每次调用next方法的时候会调用到，主要是用来将原本独立的data、label、pad和index再做一层封装，整合在一起，这样就可以通过类似data_batch.data、data_batch.label的形式来读取一个batch的数据和label。需要说明的是经过mxnet.io.DataBatch接口封装得到的数据就可以通过迭代器作为模型forward和backward的输入来用。

```
class DataBatch(object):
    """A data batch.

    MXNet's data iterator returns a batch of data for each `next` call.
    This data contains `batch_size` number of examples.

    If the input data consists of images, then shape of these images depend on
    the `layout` attribute of `DataDesc` object in `provide_data` parameter.

    If `layout` is set to 'NCHW' then, images should be stored in a 4-D matrix
    of shape ``(batch_size, num_channel, height, width)``.
    If `layout` is set to 'NHWC' then, images should be stored in a 4-D matrix
    of shape ``(batch_size, height, width, num_channel)``.
    The channels are often in RGB order.

    Parameters
    ----------
    data : list of `NDArray`, each array containing `batch_size` examples.
          A list of input data.
    label : list of `NDArray`, each array often containing a 1-dimensional array. optional
          A list of input labels.
    pad : int, optional
          The number of examples padded at the end of a batch. It is used when the
          total number of examples read is not divisible by the `batch_size`.
          These extra padded examples are ignored in prediction.
    index : numpy.array, optional
          The example indices in this batch.
    bucket_key : int, optional
          The bucket key, used for bucketing module.
    provide_data : list of `DataDesc`, optional
          A list of `DataDesc` objects. `DataDesc` is used to store
          name, shape, type and layout information of the data.
          The *i*-th element describes the name and shape of ``data[i]``.
    provide_label : list of `DataDesc`, optional
          A list of `DataDesc` objects. `DataDesc` is used to store
          name, shape, type and layout information of the label.
          The *i*-th element describes the name and shape of ``label[i]``.
    """
    def __init__(self, data, label=None, pad=None, index=None,
                 bucket_key=None, provide_data=None, provide_label=None):
        if data is not None:
            assert isinstance(data, (list, tuple)), "Data must be list of NDArrays"
        if label is not None:
            assert isinstance(label, (list, tuple)), "Label must be list of NDArrays"
        self.data = data
        self.label = label
        self.pad = pad
        self.index = index

        self.bucket_key = bucket_key
        self.provide_data = provide_data
        self.provide_label = provide_label

    def __str__(self):
        data_shapes = [d.shape for d in self.data]
        if self.label:
            label_shapes = [l.shape for l in self.label]
        else:
            label_shapes = None
        return "{}: data shapes: {} label shapes: {}".format(
            self.__class__.__name__,
            data_shapes,
            label_shapes)
```

DataDesc类主要用来保存数据的name、shape、type、layout等信息，主要放在对象的provide_data和provide_label属性中。

```python
class DataDesc(namedtuple('DataDesc', ['name', 'shape'])):
    """DataDesc is used to store name, shape, type and layout
    information of the data or the label.

    The `layout` describes how the axes in `shape` should be interpreted,
    for example for image data setting `layout=NCHW` indicates
    that the first axis is number of examples in the batch(N),
    C is number of channels, H is the height and W is the width of the image.

    For sequential data, by default `layout` is set to ``NTC``, where
    N is number of examples in the batch, T the temporal axis representing time
    and C is the number of channels.

    Parameters
    ----------
    cls : DataDesc
         The class.
    name : str
         Data name.
    shape : tuple of int
         Data shape.
    dtype : np.dtype, optional
         Data type.
    layout : str, optional
         Data layout.
    """
    def __new__(cls, name, shape, dtype=mx_real_t, layout='NCHW'): # pylint: disable=super-on-old-class
        ret = super(cls, DataDesc).__new__(cls, name, shape)
        ret.dtype = dtype
        ret.layout = layout
        return ret

    def __repr__(self):
        return "DataDesc[%s,%s,%s,%s]" % (self.name, self.shape, self.dtype,
                                          self.layout)

    @staticmethod
    def get_batch_axis(layout):
        """Get the dimension that corresponds to the batch size.

        When data parallelism is used, the data will be automatically split and
        concatenated along the batch-size dimension. Axis can be -1, which means
        the whole array will be copied for each data-parallelism device.

        Parameters
        ----------
        layout : str
            layout string. For example, "NCHW".

        Returns
        -------
        int
            An axis indicating the batch_size dimension.
        """
        if layout is None:
            return 0
        return layout.find('N')

    @staticmethod
    def get_list(shapes, types):
        """Get DataDesc list from attribute lists.

        Parameters
        ----------
        shapes : a tuple of (name, shape)
        types : a tuple of  (name, type)
        """
        if types is not None:
            type_dict = dict(types)
            return [DataDesc(x[0], x[1], type_dict[x[0]]) for x in shapes]
        else:
            return [DataDesc(x[0], x[1]) for x in shapes]
```



