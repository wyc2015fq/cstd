
# Tensorflow模型预测时，若用CPU计算，如何设置？ 并行计算-多核(multicore)，多线程(multi-thread) - 郭云飞的专栏 - CSDN博客


2018年07月05日 17:29:25[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1692标签：[tensorflow																](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)[deploy																](https://so.csdn.net/so/search/s.do?q=deploy&t=blog)[cpu																](https://so.csdn.net/so/search/s.do?q=cpu&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=deploy&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)个人分类：[ubuntu																](https://blog.csdn.net/guoyunfei20/article/category/7042343)[python																](https://blog.csdn.net/guoyunfei20/article/category/6976659)[机器学习																](https://blog.csdn.net/guoyunfei20/article/category/7178819)[
							](https://blog.csdn.net/guoyunfei20/article/category/6976659)
[
																								](https://blog.csdn.net/guoyunfei20/article/category/7042343)

[
		](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)

```python
在标准系统上通常有多个计算设备.
TensorFlow 支持 CPU 和 GPU 这两种设备. 用指定字符串来标识这些设备. 比如:
"/cpu:0": 机器中的 CPU
"/gpu:0": 机器中的 GPU, 如果你有一个的话.
"/gpu:1": 机器中的第二个 GPU, 以此类推...
在Tensorflow程序中，我们会经常看到 with tf.device("/cpu:0"): 这个语句。
单独使用这个语句，而不做其他限制，实际上默认tensorflow程序占用所有可以使用的内存资源
和CPU核，比如如果你的linux服务器是8核CPU，那么该程序会迅速占用可以使用的任意CPU，
使用接近100%，最终结果就是影响整台服务器的其他程序。
因此我们会想到需要限制使用的CPU核的个数和资源。
一、多核并行：CPU多核的运算和资源调用
在构建tf.Session() 变量时，可以通过传入tf.ConfigProto()参数
来改变一个tensorflow的session会话所使用的CPU核的个数以及线程数等。
config = tf.ConfigProto(device_count={"CPU": 4}, # limit to num_cpu_core CPU usage
                inter_op_parallelism_threads = 1, 
                intra_op_parallelism_threads = 1,
                log_device_placement=True)
with tf.Session(config = config) as sess:
    # To Do
上面代码中我们通过 “device_count={"CPU": 4}” 参数来构建一个ConfigProto()类，、
传入tf.Session()来使每个会话分配相应的资源，这里我们给tensorflow程序共分配了4个CPU core。
二、多线程，设置Multi-threads
在进行tf.ConfigProto()初始化时，我们也可以通过设置intra_op_parallelism_threads参数和
inter_op_parallelism_threads参数，来控制每个操作符op并行计算的线程个数。
二者的区别在于:
intra_op_parallelism_threads 控制运算符op内部的并行
当运算符op为单一运算符，并且内部可以实现并行时，如矩阵乘法，reduce_sum之类的操作，
可以通过设置此参数来并行, intra代表内部。
inter_op_parallelism_threads 控制多个运算符op之间的并行计算
当有多个运算符op，并且他们之间比较独立，运算符和运算符之间没有直接的路径Path相连。
Tensorflow会尝试并行地计算他们，此参数来控制数量的一个线程池。
config = tf.ConfigProto(device_count={"CPU": 4}, # limit to num_cpu_core CPU usage
                inter_op_parallelism_threads = 1, 
                intra_op_parallelism_threads = 4,
                log_device_placement=True)
with tf.Session(config = config) as sess:
    # To Do
总结，在固定CPUcore的资源限制下，通过合理设置线程thread个数可以明显提升tensorflow程序运行速度。
三、分割图模型Graph：将Tensorflow的图运算分配到不同计算单元
有时我们构建的深度网络的结构十分复杂，会出现这种情况：
多个CPU core同时运行时，有的核比较空闲，有的核使用率却达到100%的情况。
我们需要尽量避免这种运算符计算不均衡的情况。
这时，如果我们将Graph拆分为多个部分，将每个部分（如每一层网络结构）指定到不同的CPU 
核上运算，优化计算量的分配，可以使运算速度得到提升。
一个很直观的设计就是按照不同的层来划分，把运算量大的Layer分配单独的CPU，
把运算量小的Layer合并分配到同一个CPU core上。
下面是我们做的一个测试，还是tensorflow官网上的 convolutional.py 例子改写，
将不同层分配到不同的CPU device上，优化了计算资源，使得程序的速度得以提升，
例子为convolutional_graph_partitioned.py。
声明了device_id全局变量记录已经使用的CPU的ID；
调用next_device() 函数返回下一个可用的CPU device id, 
如果有可用的则分配并使全局变量device_id 加1, 最终获得的可用的device_id 
不会超过在 FLAGS.num_cpu_core中定义的核的总个数。
在model()函数构建图的过程中，通过with tf.device(next_device()): 
语句，来将当成的Conv, Pool等运算符分配到单独的CPU上。最终结果为每个batch 平均时间229 ms。
device_id = -1 # Global Variable Counter for device_id used
 
def next_device(use_cpu = True):
    ''' See if there is available next device;
        Args: use_cpu, global device_id
        Return: new device id
    '''
  global device_id
  if (use_cpu):
    if ((device_id + 1) < FLAGS.num_cpu_core):
      device_id += 1
    device = '/cpu:%d' % device_id
  else:
    if ((device_id + 1) < FLAGS.num_gpu_core):
      device_id += 1
    device = '/gpu:%d' % device_id
  return device
   
with tf.device(next_device()):
  # To Do Insert Your Code
  conv = ...
  pool = ...
```


