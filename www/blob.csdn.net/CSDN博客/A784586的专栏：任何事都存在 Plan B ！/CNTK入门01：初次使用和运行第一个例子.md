# CNTK入门01：初次使用和运行第一个例子 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年05月15日 20:25:56[QuJack](https://me.csdn.net/A784586)阅读数：759








**Python API for CNTK**


参看官网：https://www.cntk.ai/pythondocs/gettingstarted.html

1.输出cntk的版本号：



```python
>>> import cntk
>>> cntk.__version__
'2.0rc2'
```



2.两个数组相减求值  ；         evaluation评估，求值


```python
>>> cntk.minus([1, 2, 3], [4, 5, 6]).eval()
array([-3., -3., -3.], dtype=float32)
```



3.计算 (2-4)**2+(1-6)**2



```python
>>> import numpy as np
>>> x = cntk.input(2)
>>> y = cntk.input(2)
>>> x0 = np.asarray([[2., 1.]], dtype=np.float32)
>>> y0 = np.asarray([[4., 6.]], dtype=np.float32)
>>> cntk.squared_error(x, y).eval({x:x0, y:y0})
array([ 29.], dtype=float32)
```



4.asarray() 方法返回NumPy接口：



```python
>>> import cntk as C
>>> c = C.constant(3, shape=(2,3))
>>> c.asarray()
array([[ 3., 3., 3.],
[ 3., 3., 3.]], dtype=float32)
>>> np.ones_like(c.asarray())
array([[ 1., 1., 1.],
[ 1., 1., 1.]], dtype=float32)
```



5.综述和第一次运行

CNTK2是之前版本的大修改，使得它现在有强大的数据控制能力，如何读入数据，训练，测试和minibatch构建；

Python相关的提供直接构建网络图和数据读入前的操作不仅仅是为了更强大和更复杂的网络，也是为了当一个模型被创建和被调试时候的Python交互；

CNTK2包含一个准备继续扩展的例子库和层库（a layers libary）。允许简单的通过组合各个块，构建一个强大的深度网络。

例如：CNN，RNN（LSTMs）,FCN.




**第一次基本的使用例子：一个标准的FCN  （a standardfully connected deep network）**

在CNTK上训练或者运行一个网络，第一步需要决定运行在哪一个设备上，如果你有可用的GPU，训练时间将会极大的被改进。为了明确使用设备，设置使用设备如下：



```python
from cntk.device import set_default_device, gpu
set_default_device(gpu(0))
```



利用全连接层FCN训练一个分类器的网络例子

用函数：Sequential()和Dense()

网络结构：

a 2-layer fully connected
 deep neural network with 50 hidden dimensions per layer。


`ce` is
 the cross entropy which defined our model’s loss function

`pe` is
 the classification error.





代码如下;



```python
#encoding=utf-8
from __future__ import print_function
import numpy as np
import cntk as C
from cntk.learners import sgd, learning_rate_schedule, UnitType
from cntk.logging import ProgressPrinter
from cntk.layers import Dense, Sequential

def generate_random_data(sample_size, feature_dim, num_classes):
     # Create synthetic data using NumPy.
     Y = np.random.randint(size=(sample_size, 1), low=0, high=num_classes)

     # Make sure that the data is separable
     X = (np.random.randn(sample_size, feature_dim) + 3) * (Y + 1)
     X = X.astype(np.float32)
     # converting class 0 into the vector "1 0 0",
     # class 1 into vector "0 1 0", ...
     class_ind = [Y == class_number for class_number in range(num_classes)]
     Y = np.asarray(np.hstack(class_ind), dtype=np.float32)
     return X, Y

def ffnet():
    inputs = 2
    outputs = 2
    layers = 2
    hidden_dimension = 50
	#设置模型
    # input variables denoting the features and label data
    features = C.input((inputs), np.float32)
    label = C.input((outputs), np.float32)

    # Instantiate the feedforward classification model 前馈分类器模型
    my_model = Sequential ([
                    Dense(hidden_dimension, activation=C.sigmoid),
                    Dense(outputs)])
    z = my_model(features)

    ce = C.cross_entropy_with_softmax(z, label)
    pe = C.classification_error(z, label)
	#初始化训练器
    # Instantiate the trainer object to drive the model training
    lr_per_minibatch = learning_rate_schedule(0.125, UnitType.minibatch)
    progress_printer = ProgressPrinter(0)
    trainer = C.Trainer(z, (ce, pe), [sgd(z.parameters, lr=lr_per_minibatch)], [progress_printer])

    # Get minibatches of training data and perform model training
    minibatch_size = 25
    num_minibatches_to_train = 1024

    aggregate_loss = 0.0
    for i in range(num_minibatches_to_train):
        train_features, labels = generate_random_data(minibatch_size, inputs, outputs)
        #训练
		# Specify the mapping of input variables in the model to actual minibatch data to be trained with
        trainer.train_minibatch({features : train_features, label : labels})
        sample_count = trainer.previous_minibatch_sample_count
        aggregate_loss += trainer.previous_minibatch_loss_average * sample_count

    last_avg_error = aggregate_loss / trainer.total_number_of_samples_seen

    test_features, test_labels = generate_random_data(minibatch_size, inputs, outputs)
    avg_error = trainer.test_minibatch({features : test_features, label : test_labels})
    print(' error rate on an unseen minibatch: {}'.format(avg_error))
    return last_avg_error, avg_error

if __name__=='__main__':
	np.random.seed(98052)
	ffnet()
```
命令行下运行：Python simplenet.py



![](https://img-blog.csdn.net/20170515202122401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)














