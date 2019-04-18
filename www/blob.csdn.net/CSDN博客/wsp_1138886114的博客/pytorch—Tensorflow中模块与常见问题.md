# pytorch—Tensorflow中模块与常见问题 - wsp_1138886114的博客 - CSDN博客





2018年10月09日 16:49:33[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：464








### 一、指定运行的GPU：torch.nn.DataParallel

PyTorch默认使用从0开始的GPU，如果GPU0正在运行程序，需要指定其他GPU。

有如下两种方法来指定需要使用的GPU。

tensorflow指定GPU的方式，推荐使用 **CUDA_VISIBLE_DEVICES** 。

```python
1. 直接终端中设定：
CUDA_VISIBLE_DEVICES=1 python file_name.py

2. python代码中设定：(默认使用从 0 开始的GPU)

# 2.1  指定单 GPU 运行
import os
os.environ["CUDA_VISIBLE_DEVICES"] = "2"

# 2.2  指定多 GPU 运行
device_ids = [0,1,2,3]
model = model.cuda(device_ids[0])
model = nn.DataParallel(model, device_ids=device_ids)

optimizer = optim.SGD(model.parameters(), lr=learning_rate, 
                      momentum=0.9, weight_decay=0.001) 
optimizer = nn.DataParallel(optimizer, device_ids=device_ids)

optimizer.module.step()
for param_lr in optimizer.module.param_groups:  # 同样是要加module
        #     param_lr['lr'] = param_lr['lr'] * 0.999
```

##### 加载多GPU预训练模型

多GPU训练 官方文档:[https://pytorch.org/docs/stable/nn.html#dataparallel-layers-multi-gpu-distributed](https://pytorch.org/docs/stable/nn.html#dataparallel-layers-multi-gpu-distributed)

教程：[https://pytorch.org/tutorials/beginner/blitz/data_parallel_tutorial.html?highlight=dataparallel](https://pytorch.org/tutorials/beginner/blitz/data_parallel_tutorial.html?highlight=dataparallel)

```python
torch.nn.DataParallel(module, device_ids=None, output_device=None, dim=0)

"""
该函数实现了在module级别上的数据并行使用，注意batch size要大于GPU的数量。
	module：        需要多GPU训练的网络模型
	device_ids：    GPU的编号（默认全部GPU）
	output_device：（默认是device_ids[0])
	dim：tensors   被分散的维度，默认是0

"""
# 使用方法：
net = torch.nn.DataParallel(model, device_ids=[0, 1, 2])
out = net(input)
```

### 二、tensorflow.contrib.slim

**`tf.train.Coordinator 线程协调器``tf.train.start_queue_runners 入队线程启动器`**

TensorFlow 的 Session对象是支持多线程的，可以在同一个会话（Session）中创建多个线程，并行执行。在Session中的所有线程都必须能被同步终止，异常必须能被正确捕获并报告，会话终止的时候， 队列必须能被正确地关闭。

TensorFlow提供了`tf.Coordinator和 tf.QueueRunner`，这两个类来实现对Session中多线程的管理：
- Coordinator 类用来管理在Session中的多个线程，可以用来同时停止多个工作线程并且向那个在等待所有工作线程终止的程序报告异常，该线程捕获到这个异常之后就会终止所有线程。

使用 tf.train.Coordinator()来创建一个线程管理器（协调器）对象。
- QueueRunner 类用来启动tensor的入队线程，可以用来启动多个工作线程同时将多个tensor（训练数据）推送入文件名称队列中，具体执行函数是 tf.train.start_queue_runners ， 只有调用 tf.train.start_queue_runners 之后，才会真正把tensor推入内存序列中，供计算单元调用，否则会由于内存序列为空，数据流图会处于一直等待状态。

流程如下：
- 调用 tf.train.slice_input_producer，从 本地文件里抽取tensor，准备放入Filename Queue（文件名队列）中;
- 调用 tf.train.batch，从文件名队列中提取tensor，使用单个或多个线程，准备放入文件队列;
- 调用 tf.train.Coordinator() 来创建一个线程协调器，用来管理之后在Session中启动的所有线程;
- 调用tf.train.start_queue_runners, 启动入队线程，由多个或单个线程，按照设定规则，把文件读入Filename Queue中。函数返回线程ID的列表，一般情况下，系统有多少个核，就会启动多少个入队线程（入队具体使用多少个线程在tf.train.batch中定义）;
- 文件从 Filename Queue中读入内存队列的操作不用手动执行，由tf自动完成;
- 调用sess.run 来启动数据出列和执行计算;
- 使用 coord.should_stop()来查询是否应该终止所有线程，当文件队列（queue）中的所有文件都已经读取出列的时候，会抛出一个OutofRangeError 的异常，这时候就应该停止Sesson中的所有线程了;
- 使用coord.request_stop()来发出终止所有线程的命令，使用coord.join(threads)把线程加入主线程，等待threads结束。

其他项目代码：
[http://www.cnblogs.com/skyfsm/p/8443107.html](http://www.cnblogs.com/skyfsm/p/8443107.html)

```python
import tensorflow as tf
import numpy as np
 
sample_num=5                                  # 样本个数
epoch_num = 2                                 # 设置迭代次数
batch_size = 3                                # 设置一个批次中包含样本个数
batch_total = int(sample_num/batch_size)+1    # 计算每一轮epoch中含有的batch个数
 
def generate_data(sample_num=sample_num):     # 数据生成器：4个数据和标签
    labels = np.asarray(range(0, sample_num))
    images = np.random.random([sample_num, 224, 224, 3])
    print('image size {},label size :{}'.format(images.shape, labels.shape))
    return images,labels
 
def get_batch_data(batch_size=batch_size):
    images, label = generate_data()
    images = tf.cast(images, tf.float32)      # 数据类型转换为tf.float32
    label = tf.cast(label, tf.int32)
 
    #从tensor列表中按顺序或随机抽取一个tensor准备放入文件名称队列
    input_queue = tf.train.slice_input_producer([images, label], num_epochs=epoch_num, shuffle=False)
 
    #从文件名称队列中读取文件准备放入文件队列
    image_batch, label_batch = tf.train.batch(input_queue, batch_size=batch_size, num_threads=2, capacity=64, allow_smaller_final_batch=False)
    return image_batch, label_batch
 
image_batch, label_batch = get_batch_data(batch_size=batch_size)
 
 
with tf.Session() as sess:
    sess.run(tf.global_variables_initializer() # 先执行初始化工作
    sess.run(tf.local_variables_initializer())
 
    coord = tf.train.Coordinator()                       # 开启一个协调器
    threads = tf.train.start_queue_runners(sess, coord)  # 使用start_queue_runners 启动队列填充
 
    try:
        while not coord.should_stop():
            print ('************')
            # 获取每一个batch中batch_size个样本和标签
            image_batch_v, label_batch_v = sess.run([image_batch, label_batch])
            print(image_batch_v.shape, label_batch_v)
    except tf.errors.OutOfRangeError:              # 如果读取到文件队列末尾会抛出此异常
        print("done! now lets kill all the threads……")
    finally:
        coord.request_stop()                       # 协调器coord发出所有线程终止信号
        print('all threads are asked to stop!')
    coord.join(threads)                            # 把开启的线程加入主线程，等待threads结束
    print('all threads are stopped!')
```



