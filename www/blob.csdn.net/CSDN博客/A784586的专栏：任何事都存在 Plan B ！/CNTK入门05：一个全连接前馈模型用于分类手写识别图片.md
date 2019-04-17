# CNTK入门05：一个全连接前馈模型用于分类手写识别图片 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年05月26日 15:52:50[QuJack](https://me.csdn.net/A784586)阅读数：605








**CNTK入门05：一个全连接前馈模型用于分类手写识别图片**


------- A fully connected feed-forward model for classification of MNIST images.

**1.  **.\Examples\Image\DataSets\MNIST下有两个文件，运行Python install_mnist.py下载手写识别图片。

其中60000作为测试训练数据，10000张作为测试数据。

代码：install_mnist.py



```python
from __future__ import print_function
import os
import mnist_utils as ut

if __name__ == "__main__":
	#改变当前工作目录到指定的路径
    os.chdir(os.path.abspath(os.path.dirname(__file__)))
    train = ut.load('http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz',
        'http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz', 60000)
    print ('Writing train text file...')
    ut.savetxt(r'./Train-28x28_cntk_text.txt', train)
    print ('Done.')
	
    test = ut.load('http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz',
        'http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz', 10000)
    print ('Writing test text file...')
    ut.savetxt(r'./Test-28x28_cntk_text.txt', test)
    print ('Done.')
```




上面主函数调用了如下的文件中的函数：mnist_utils.py



```python
from __future__ import print_function
try: 
    from urllib.request import urlretrieve 
except ImportError: 
    from urllib import urlretrieve
import sys
import gzip
import shutil
import os
import struct
import numpy as np

def loadData(src, cimg):
    print ('Downloading ' + src)
	#retrieve,取回；src 下载地址，./delete.me是存储地址；返回tuple(filename,header)
	#filename 表示保存到本地的路径，header 表示服务器的响应头
    gzfname, h = urlretrieve(src, './delete.me')
    print ('Done.')
    try:
        with gzip.open(gzfname) as gz:
			#按照给定的格式(fmt)解析字节流string，返回解析出来的tuple
            n = struct.unpack('I', gz.read(4))
            # Read magic number.
            if n[0] != 0x3080000:
                raise Exception('Invalid file: unexpected magic number.')
            # Read number of entries.
            n = struct.unpack('>I', gz.read(4))[0]
            if n != cimg:
                raise Exception('Invalid file: expected {0} entries.'.format(cimg))
            crow = struct.unpack('>I', gz.read(4))[0]
            ccol = struct.unpack('>I', gz.read(4))[0]
            if crow != 28 or ccol != 28:
                raise Exception('Invalid file: expected 28 rows/cols per image.')
            # Read data.
            res = np.fromstring(gz.read(cimg * crow * ccol), dtype = np.uint8)
    finally:
        os.remove(gzfname)
    return res.reshape((cimg, crow * ccol))

def loadLabels(src, cimg):
    print ('Downloading ' + src)
    gzfname, h = urlretrieve(src, './delete.me')
    print ('Done.')
    try:
        with gzip.open(gzfname) as gz:
            n = struct.unpack('I', gz.read(4))
            # Read magic number.
            if n[0] != 0x1080000:
                raise Exception('Invalid file: unexpected magic number.')
            # Read number of entries.
            n = struct.unpack('>I', gz.read(4))
            if n[0] != cimg:
                raise Exception('Invalid file: expected {0} rows.'.format(cimg))
            # Read labels.
            res = np.fromstring(gz.read(cimg), dtype = np.uint8)
    finally:
        os.remove(gzfname)
    return res.reshape((cimg, 1))#改变矩阵为cimg行，1列

#参数：训练的数据，训练数据的标签类别，图片张数
def load(dataSrc, labelsSrc, cimg):
    data = loadData(dataSrc, cimg)
    labels = loadLabels(labelsSrc, cimg)
	#
    return np.hstack((data, labels))

def savetxt(filename, ndarray):
    with open(filename, 'w') as f:
        labels = list(map(' '.join, np.eye(10, dtype=np.uint).astype(str)))
        for row in ndarray:
            row_str = row.astype(str)
            label_str = labels[row[-1]]
            feature_str = ' '.join(row_str[:-1])
            f.write('|labels {} |features {}\n'.format(label_str, feature_str))
```

在目录 .\Examples\Image\Classification\MLP\Python\SimpleMNIST.py是手写识别的程序；

 Python 
SimpleMNIST.py 运行即可：



```python
import argparse
import numpy as np
import sys
import os
from cntk.train import Trainer, minibatch_size_schedule 
from cntk.io import MinibatchSource, CTFDeserializer, StreamDef, StreamDefs, INFINITELY_REPEAT
from cntk.device import cpu, try_set_default_device
from cntk.learners import adadelta, learning_rate_schedule, UnitType
from cntk.ops import input, relu, element_times, constant
from cntk.losses import cross_entropy_with_softmax
from cntk.metrics import classification_error
from cntk.train.training_session import *
from cntk.logging import ProgressPrinter, TensorBoardProgressWriter

abs_path = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(abs_path, "..", "..", "..", "..", "common"))
from nn import fully_connected_classifier_net

def check_path(path):
    if not os.path.exists(path):
        readme_file = os.path.normpath(os.path.join(
            os.path.dirname(path), "..", "README.md"))
        raise RuntimeError(
            "File '%s' does not exist. Please follow the instructions at %s to download and prepare it." % (path, readme_file))

def create_reader(path, is_training, input_dim, label_dim):
    return MinibatchSource(CTFDeserializer(path, StreamDefs(
        features  = StreamDef(field='features', shape=input_dim, is_sparse=False),
        labels    = StreamDef(field='labels',   shape=label_dim, is_sparse=False)
    )), randomize=is_training, max_sweeps = INFINITELY_REPEAT if is_training else 1)


# Creates and trains a feedforward classification model for MNIST images

def simple_mnist(tensorboard_logdir=None):
    input_dim = 784
    num_output_classes = 10
    num_hidden_layers = 1
    hidden_layers_dim = 200

    # Input variables denoting the features and label data
    feature = input(input_dim, np.float32)
    label = input(num_output_classes, np.float32)

    # Instantiate the feedforward classification model
    scaled_input = element_times(constant(0.00390625), feature)
    z = fully_connected_classifier_net(
        scaled_input, num_output_classes, hidden_layers_dim, num_hidden_layers, relu)

    ce = cross_entropy_with_softmax(z, label)
    pe = classification_error(z, label)

    data_dir = os.path.join(abs_path, "..", "..", "..", "DataSets", "MNIST")

    path = os.path.normpath(os.path.join(data_dir, "Train-28x28_cntk_text.txt"))
    check_path(path)

    reader_train = create_reader(path, True, input_dim, num_output_classes)

    input_map = {
        feature  : reader_train.streams.features,
        label  : reader_train.streams.labels
    }

    # Training config
    minibatch_size = 64
    num_samples_per_sweep = 60000
    num_sweeps_to_train_with = 10

    # Instantiate progress writers.
    #training_progress_output_freq = 100
    progress_writers = [ProgressPrinter(
        #freq=training_progress_output_freq,
        tag='Training',
        num_epochs=num_sweeps_to_train_with)]

    if tensorboard_logdir is not None:
        progress_writers.append(TensorBoardProgressWriter(freq=10, log_dir=tensorboard_logdir, model=z))

    # Instantiate the trainer object to drive the model training
    trainer = Trainer(z, (ce, pe), adadelta(z.parameters), progress_writers)

    training_session(
        trainer=trainer,
        mb_source = reader_train,
        mb_size = minibatch_size,
        model_inputs_to_streams = input_map,
        max_samples = num_samples_per_sweep * num_sweeps_to_train_with,
        progress_frequency=num_samples_per_sweep
    ).train()
    
    # Load test data
    path = os.path.normpath(os.path.join(data_dir, "Test-28x28_cntk_text.txt"))
    check_path(path)

    reader_test = create_reader(path, False, input_dim, num_output_classes)

    input_map = {
        feature  : reader_test.streams.features,
        label  : reader_test.streams.labels
    }

    # Test data for trained model
    test_minibatch_size = 1024
    num_samples = 10000
    num_minibatches_to_test = num_samples / test_minibatch_size
    test_result = 0.0
    for i in range(0, int(num_minibatches_to_test)):
        mb = reader_test.next_minibatch(test_minibatch_size, input_map=input_map)
        eval_error = trainer.test_minibatch(mb)
        test_result = test_result + eval_error

    # Average of evaluation errors of all test minibatches
    return test_result / num_minibatches_to_test


if __name__=='__main__':
    # Specify the target device to be used for computing, if you do not want to
    # use the best available one, e.g.
    # try_set_default_device(cpu())

    parser = argparse.ArgumentParser()
    parser.add_argument('-tensorboard_logdir', '--tensorboard_logdir',
                        help='Directory where TensorBoard logs should be created', required=False, default=None)
    args = vars(parser.parse_args())

    error = simple_mnist(args['tensorboard_logdir'])
    print("Error: %f" % error)
```



----------------------------------------------------------------------------------------------------------------------------

附录：

1. os.chdir(）  #改变当前工作目录到指定的路径

2.np.hstack()的解释：

stack() Join a sequence of arrays along a new axis.

hstack() Stack arrays in sequence horizontally (column wise).

dstack() Stack arrays in sequence depth wise (along third dimension).

concatenate() Join a sequence of arrays along an existing axis.

vsplit () Split array into a list of multiple sub-arrays vertically. 

2.1 stack() ：通过个新轴，连接两个数组。




```python
>>> arrays = [np.random.randn(3, 4) for _ in range(10)]  
>>> np.stack(arrays, axis=0).shape  
(10, 3, 4)   
>>> np.stack(arrays, axis=1).shape  
(3, 10, 4)  
>>> np.stack(arrays, axis=2).shape  
(3, 4, 10)  
>>> a = np.array([1, 2, 3])  
>>> b = np.array([2, 3, 4])  
>>> np.stack((a, b))  
array([[1, 2, 3],  
       [2, 3, 4]])  
>>> np.stack((a, b), axis=-1)  
array([[1, 2],  
       [2, 3],  
       [3, 4]])
```




np.random.randn(3, 4)返回一个3行4列的矩阵，数字是服从标准正态分布的。


2.2 numpy.hstack()函数


函数原型：numpy.hstack(tup)


其中tup是arrays序列，The arrays must have the same shape, except in the dimensioncorresponding to axis (the first, by default).


等价于：np.concatenate(tup, axis=1)



```java
>>> a = np.array((1,2,3))  
>>> b = np.array((2,3,4))  
>>> np.hstack((a,b))  
array([1, 2, 3, 2, 3, 4])  
>>> a = np.array([[1],[2],[3]])  
>>> b = np.array([[2],[3],[4]])  
>>> np.hstack((a,b))  
array([[1, 2],  
       [2, 3],  
       [3, 4]])
```
2.3 numpy.vstack()函数



函数原型：numpy.vstack(tup)


等价于：np.concatenate(tup, axis=0） if tup contains arrays thatare at least 2-dimensional.


程序实例：



```python
>>> a = np.array([1, 2, 3])  
>>> b = np.array([2, 3, 4])  
>>> np.vstack((a,b))  
array([[1, 2, 3],  
       [2, 3, 4]])  
>>> a = np.array([[1], [2], [3]])  
>>> b = np.array([[2], [3], [4]])  
>>> np.vstack((a,b))  
array([[1],  
       [2],  
       [3],  
       [2],  
       [3],  
       [4]])
```




**2.4   numpy.dstack(）函数**


函数原型：numpy.dstack(tup)



等价于：np.concatenate(tup, axis=2)


程序实例：



```python
>>> a = np.array((1,2,3))  
>>> b = np.array((2,3,4))  
>>> np.dstack((a,b))  
array([[[1, 2],  
        [2, 3],  
        [3, 4]]])  
  
>>>  
  
>>> a = np.array([[1],[2],[3]])  
>>> b = np.array([[2],[3],[4]])  
>>> np.dstack((a,b))  
array([[[1, 2]],  
       [[2, 3]],  
       [[3, 4]]])
```



**2.5 numpy.concatenate()函数**


函数原型：numpy.concatenate((a1, a2, ...), axis=0)


程序实例：

```python
>>> a = np.array([[1, 2], [3, 4]])  
>>> b = np.array([[5, 6]])  
>>> np.concatenate((a, b), axis=0)  
array([[1, 2],  
       [3, 4],  
       [5, 6]])  
>>> np.concatenate((a, b.T), axis=1)  
array([[1, 2, 5],  
       [3, 4, 6]])  
  
This function will not preserve masking of MaskedArray inputs.  
>>>  
  
>>> a = np.ma.arange(3)  
>>> a[1] = np.ma.masked  
>>> b = np.arange(2, 5)  
>>> a  
masked_array(data = [0 -- 2],  
             mask = [False  True False],  
       fill_value = 999999)  
>>> b  
array([2, 3, 4])  
>>> np.concatenate([a, b])  
masked_array(data = [0 1 2 2 3 4],  
             mask = False,  
       fill_value = 999999)  
>>> np.ma.concatenate([a, b])  
masked_array(data = [0 -- 2 2 3 4],  
             mask = [False  True False False False False],  
       fill_value = 999999)
```

**2.6 numpy.vsplit()函数**


函数原型：numpy.vsplit(ary, indices_or_sections)


程序实例：

```python
>>> x = np.arange(16.0).reshape(4, 4)  
>>> x  
array([[  0.,   1.,   2.,   3.],  
       [  4.,   5.,   6.,   7.],  
       [  8.,   9.,  10.,  11.],  
       [ 12.,  13.,  14.,  15.]])  
>>> np.vsplit(x, 2)  
[array([[ 0.,  1.,  2.,  3.],  
       [ 4.,  5.,  6.,  7.]]),  
 array([[  8.,   9.,  10.,  11.],  
       [ 12.,  13.,  14.,  15.]])]  
>>> np.vsplit(x, np.array([3, 6]))  
[array([[  0.,   1.,   2.,   3.],  
       [  4.,   5.,   6.,   7.],  
       [  8.,   9.,  10.,  11.]]),  
 array([[ 12.,  13.,  14.,  15.]]),  
 array([], dtype=float64)]  
  
With a higher dimensional array the split is still along the first axis.  
>>>  
  
>>> x = np.arange(8.0).reshape(2, 2, 2)  
>>> x  
array([[[ 0.,  1.],  
        [ 2.,  3.]],  
       [[ 4.,  5.],  
        [ 6.,  7.]]])  
>>> np.vsplit(x, 2)  
[array([[[ 0.,  1.],  
        [ 2.,  3.]]]),  
 array([[[ 4.,  5.],  
        [ 6.,  7.]]])]
```


------------------------------------------------------------------------------------
3.url.urlretrieve(**filename**[,
**reporthook**[, **data**]]])的使用

urllib.urlretrieve(url,filename)下载网络文件,第一个元素就是目标url,第二个参数是保存的文件绝对路径(含文件名),返回值是一个tuple(filename,header),filename
 表示保存到本地的路径，header 表示服务器的响应头.如果urlretrieve仅提供1个参数,返回值的filename就是产生的临时文件名,函数执行完毕后该临时文件会被删除参数。参数reporthook是一个回调函数，当连接上服务器、以及相应的数据块传输完毕的时候会触发该回调。其中回调函数名称可任意，但是参数必须为三个。一般直接使用reporthook(block_read,block_size,total_size)定义回调函数，block_size是每次读取的数据块的大小，block_read是每次读取的数据块个数，taotal_size是一一共读取的数据量，单位是byte。可以使用reporthook函数来显示读取进度。











