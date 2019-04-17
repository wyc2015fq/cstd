# keras系列︱利用fit_generator最小化显存占用比率/数据Batch化 - 素质云笔记/Recorder... - CSDN博客





2017年07月10日 14:33:41[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：30783








> 
本文主要参考两篇文献： 

  1、[《深度学习theano/tensorflow多显卡多人使用问题集》](https://zhuanlan.zhihu.com/p/23250782)

  2、[基于双向LSTM和迁移学习的seq2seq核心实体识别](http://kexue.fm/archives/3942/)
运行机器学习算法时，很多人一开始都会有意无意将数据集默认直接装进显卡显存中，如果处理大型数据集（例如图片尺寸很大）或是网络很深且隐藏层很宽，也可能造成显存不足。

这个情况随着工作的深入会经常碰到，解决方法其实很多人知道，就是分块装入。以keras为例，默认情况下用fit方法载数据，就是全部载入。换用fit_generator方法就会以自己手写的方法用yield逐块装入。这里稍微深入讲一下fit_generator方法。 

.

#### — fit_generator源码

```python
def fit_generator(self, generator, samples_per_epoch, nb_epoch,
                      verbose=1, callbacks=[],
                      validation_data=None, nb_val_samples=None,
                      class_weight=None, max_q_size=10, **kwargs):
```

.

#### — generator 该怎么写？

其中generator参数传入的是一个方法，validation_data参数既可以传入一个方法也可以直接传入验证数据集，通常我们都可以传入方法。这个方法需要我们自己手写，伪代码如下：

```python
def generate_batch_data_random(x, y, batch_size):
    """逐步提取batch数据到显存，降低对显存的占用"""
    ylen = len(y)
    loopcount = ylen // batch_size
    while (True):
        i = randint(0,loopcount)
        yield x[i * batch_size:(i + 1) * batch_size], y[i * batch_size:(i + 1) * batch_size]
```

. 
**为什么推荐在自己写的方法中用随机呢？**
- 因为fit方法默认shuffle参数也是True，fit_generator需要我们自己随机打乱数据。
- 另外，在方法中需要用while写成死循环，因为每个epoch不会重新调用方法，这个是新手通常会碰到的问题。

当然，如果原始数据已经随机打乱过，那么可以不在这里做随机处理。否则还是建议加上随机取数逻辑（如果数据集比较大则可以保证基本乱序输出）。**深度学习中随机打乱数据是非常重要的**，具体参见《深度学习Deep Learning》一书的8.1.3节：《Batch and Minibatch Algorithm》。（2017年5月25日补充说明） 

.

#### 调用示例：

```
model.fit_generator(self.generate_batch_data_random(x_train, y_train, batch_size),                                                      
    samples_per_epoch=len(y_train)//batch_size*batch_size,
    nb_epoch=epoch, 
    validation_data=self.generate_valid_data(x_valid, y_valid,batch_size),
    nb_val_samples=(len(y_valid)//batch_size*batch_size), 
    verbose=verbose,
    callbacks=[early_stopping])
```

这样就可以将对显存的占用压低了，配合第一部分的方法可以方便同时执行多程序。

. 

来看看一个[《基于双向LSTM和迁移学习的seq2seq核心实体识别》](http://kexue.fm/archives/3942/)实战案例：

```python
'''
gen_matrix实现从分词后的list来输出训练样本
gen_target实现将输出序列转换为one hot形式的目标
超过maxlen则截断，不足补0
'''
gen_matrix = lambda z: np.vstack((word2vec[z[:maxlen]], np.zeros((maxlen-len(z[:maxlen]), word_size))))
gen_target = lambda z: np_utils.to_categorical(np.array(z[:maxlen] + [0]*(maxlen-len(z[:maxlen]))), 5)

#从节省内存的角度，通过生成器的方式来训练
def data_generator(data, targets, batch_size): 
    idx = np.arange(len(data))
    np.random.shuffle(idx)
    batches = [idx[range(batch_size*i, min(len(data), batch_size*(i+1)))] for i in range(len(data)/batch_size+1)]
    while True:
        for i in batches:
            xx, yy = np.array(map(gen_matrix, data[i])), np.array(map(gen_target, targets[i]))
            yield (xx, yy)


batch_size = 1024
history = model.fit_generator(data_generator(d['words'], d['label'], batch_size), samples_per_epoch=len(d), nb_epoch=200)
model.save_weights('words_seq2seq_final_1.model')
```

## 延伸一：[edwardlib/observations](https://github.com/edwardlib/observations) 规范数据导入、数据Batch化

```python
def generator(array, batch_size):
  """Generate batch with respect to array's first axis."""
  start = 0  # pointer to where we are in iteration
  while True:
    stop = start + batch_size
    diff = stop - array.shape[0]
    if diff <= 0:
      batch = array[start:stop]
      start += batch_size
    else:
      batch = np.concatenate((array[start:], array[:diff]))
      start = diff
    yield batch
```

To use it, simply write

```python
from observations import cifar10
(x_train, y_train), (x_test, y_test) = cifar10("~/data")
x_train_data = generator(x_train, 256)
```


**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






