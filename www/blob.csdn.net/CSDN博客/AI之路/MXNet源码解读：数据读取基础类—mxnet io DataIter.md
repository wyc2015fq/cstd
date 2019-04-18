# MXNet源码解读：数据读取基础类—mxnet.io.DataIter - AI之路 - CSDN博客





2018年03月27日 21:59:42[AI之路](https://me.csdn.net/u014380165)阅读数：1174
所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)









**mxnet.io.DataIter是MXNet框架中构造数据迭代器的基础类，在MXNet框架下只要和数据读取相关的接口基本上都继承该类，比如我们常用的图像算法相关的mxnet.io.ImageRecordIter类或mxnet.image.ImageIter类都直接或间接继承mxnet.io.DataIter类进行封装。**因此熟悉mxnet.io.DataIter类对于其他high level的数据读取接口以及自己封装数据非常有帮助。

mxnet.io.DataIter类的[源码地址](https://github.com/apache/incubator-mxnet/blob/master/python/mxnet/io.py)

源码如下。从注释可以看出该类是MXNet框架下构造数据迭代器的基础类，比较抽象。在该类中主要通过调用next方法返回下一个batch的数据，如果数据已迭代读取完毕，则抛出StopIteration。

```python
class DataIter(object):
    """The base class for an MXNet data iterator.

    All I/O in MXNet is handled by specializations of this class. Data iterators
    in MXNet are similar to standard-iterators in Python. On each call to `next`
    they return a `DataBatch` which represents the next batch of data. When
    there is no more data to return, it raises a `StopIteration` exception.

    Parameters
    ----------
    batch_size : int, optional
        The batch size, namely the number of items in the batch.

    See Also
    --------
    NDArrayIter : Data-iterator for MXNet NDArray or numpy-ndarray objects.
    CSVIter : Data-iterator for csv data.
    LibSVMIter : Data-iterator for libsvm data.
    ImageIter : Data-iterator for images.
    """
    def __init__(self, batch_size=0):
        self.batch_size = batch_size

    def __iter__(self):
        return self

    def reset(self):
        """Reset the iterator to the begin of the data."""
        pass


# next方法是构造数据迭代器的重要方法，在该方法中主要通过mxnet.io.DataBatch类来将
# 独立的data、label、pad和index信息封装在一起。在调用mxnet.io.DataBatch接口时需
# 要传递数据(self.getdata())、标签(self.getlabel())、pad方式(self.getpad())
# 和index信息(self.getindex())，这几个方法都是mxnet.io.DataIter类的方法，这几
# 个方法一般在继承时候需要重写，可以看另一篇博客中介绍的mxnet.io.MXDataIter接口,
# 官方实现中多采用调用C++底层代码实现数据读取。另外说明一下pad是用来当剩余数据不足
# 一个batch size时进行填充的，因此一般只会在读取最后一个batch时才有可能有效。
    def next(self):
        """Get next data batch from iterator.

        Returns
        -------
        DataBatch
            The data of next batch.

        Raises
        ------
        StopIteration
            If the end of the data is reached.
        """
        if self.iter_next():
            return DataBatch(data=self.getdata(), label=self.getlabel(), \
                    pad=self.getpad(), index=self.getindex())
        else:
            raise StopIteration

    def __next__(self):
        return self.next()

    def iter_next(self):
        """Move to the next batch.

        Returns
        -------
        boolean
            Whether the move is successful.
        """
        pass

    def getdata(self):
        """Get data of current batch.

        Returns
        -------
        list of NDArray
            The data of the current batch.
        """
        pass

    def getlabel(self):
        """Get label of the current batch.

        Returns
        -------
        list of NDArray
            The label of the current batch.
        """
        pass

    def getindex(self):
        """Get index of the current batch.

        Returns
        -------
        index : numpy.array
            The indices of examples in the current batch.
        """
        return None

    def getpad(self):
        """Get the number of padding examples in the current batch.

        Returns
        -------
        int
            Number of padding examples in the current batch.
        """
        pass
```



