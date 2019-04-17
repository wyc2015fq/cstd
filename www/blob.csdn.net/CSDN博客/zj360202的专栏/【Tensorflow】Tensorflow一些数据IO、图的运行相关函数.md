# 【Tensorflow】Tensorflow一些数据IO、图的运行相关函数 - zj360202的专栏 - CSDN博客





2017年12月07日 16:03:48[zj360202](https://me.csdn.net/zj360202)阅读数：281标签：[tensorflow																[graph																[数据io																[graph函数](https://so.csdn.net/so/search/s.do?q=graph函数&t=blog)
个人分类：[tensorflow](https://blog.csdn.net/zj360202/article/category/6872046)






摘要：本系列主要对tf的一些常用概念与方法进行描述。本文主要针对tensorflow的数据IO、图的运行等相关函数进行讲解。为‘Tensorflow一些常用基本概念与函数’系列之三。

## 1、序言


本文所讲的内容主要为以下相关函数：
|操作组|操作|
|----|----|
|Data IO (Python functions)|TFRecordWrite，rtf_record_iterator|
|Running Graphs|Session management，Error classes|

## 2、tf函数

### 2.1 数据IO {Data IO (Python functions)}


一个TFRecords 文件为一个字符串序列。这种格式并非随机获取，它比较适合大规模的数据流，而不太适合需要快速分区或其他非序列获取方式。

#### 数据IO {Data IO (Python functions)}
|操作|描述|
|----|----|
|class tf.python_io.TFRecordWriter|一个用于将记录(records)写入TFRecords文件的类|
|tf.python_io.TFRecordWriter.__init__(path, options=None)|打开文件路径，并创建一个TFRecordWriter以供写入|
|tf.python_io.TFRecordWriter.write(record)|将一个字符串records写入文件中|
|tf.python_io.TFRecordWriter.close()|关闭文件|
|tf.python_io.tf_record_iterator(path, options=None)|从TFRecords文件中读取记录的迭代器|

### 2.2 运行图(Running Graphs)

#### 会话管理 (Session management)
|操作|描述|
|----|----|
|class tf.Session|运行TF操作的类,一个Session对象将操作节点op封装在一定的环境内运行，同时tensor对象将被计算求值|
|tf.Session.__init__(target=”, graph=None, config=None)|创建一个新的会话|
|tf.Session.run(fetches, feed_dict=None, options=None, run_metadata=None)|运行fetches中的操作节点并求其值|
|tf.Session.close()|关闭会话|
|tf.Session.graph|返回加载值该会话的图(graph)|
|tf.Session.as_default()|设置该对象为默认会话，并返回一个上下文管理器|
|tf.Session.reset(target, containers=None, config=None)|重设target的资源容器，并关闭所有连接的会话在0.10版本该功能仅应用在分布会话中target:为执行引擎所连接的目标，其包含有资源容器，该资源容器分布在同一个集群的所有works上|
|class tf.InteractiveSession|使用在交互式上下文环境的tf会话，比如shell，ipython|
|tf.InteractiveSession.close()|关闭一个InteractiveSession|
|tf.get_default_session()|返回当前线程的默认会话|

tf.Session



```python
#一个简单的tf.Session例子
# 建立一个graph.
a = tf.constant(5.0)
b = tf.constant(6.0)
c = a * b

# 将graph载入到一个会话session中
sess = tf.Session()

# 计算tensor `c`.
print(sess.run(c))
```


```python
#一个会话可能会占用一些资源，比如变量、队列和读取器(reader)。释放这些不再使用的资源非常重要。
#使用close()方法关闭会话，或者使用上下文管理器，释放资源。
# 使用`close()`方法.
sess = tf.Session()
sess.run(...)
sess.close()

# 使用上下文管理器
with tf.Session() as sess:
  sess.run(...)
```




tf.Session()的变量设置， ConfigProto protocol buffer为会话提供了不同的配置选项。比如，创建一个会话，对设备布局使用软约束条件，以及对分布



```python
# Launch the graph in a session that allows soft device placement and
# logs the placement decisions.
sess = tf.Session(config=tf.ConfigProto(allow_soft_placement=True,
                                        log_device_placement=True))
```





tf.Session.run



```python
a = tf.constant([10, 20])
   b = tf.constant([1.0, 2.0])
   # 'fetches' 可以为单个数
   v = session.run(a)
   # v is the numpy array [10, 20]
   # 'fetches' 可以为一个list.
   v = session.run([a, b])
   # v a Python list with 2 numpy arrays: the numpy array [10, 20] and the
   # 1-D array [1.0, 2.0]
   # 'fetches' 可以是 lists, tuples, namedtuple, dicts中的任意:
   MyData = collections.namedtuple('MyData', ['a', 'b'])
   v = session.run({'k1': MyData(a, b), 'k2': [b, a]})
   # v 为一个dict，并有
   # v['k1'] is a MyData namedtuple with 'a' the numpy array [10, 20] and
   # 'b' the numpy array [1.0, 2.0]
   # v['k2'] is a list with the numpy array [1.0, 2.0] and the numpy array
   # [10, 20].
```



tf.Session.as_default()

使用关键字with指定会话， 可以在会话中执行[Operation.run()](http://blog.csdn.net/lenbow/article/details/52181159)或[Tensor.eval()](http://blog.csdn.net/lenbow/article/details/52181159)，以得到运行的tensor结果



```python
c = tf.constant(..)
sess = tf.Session()

with sess.as_default():
  assert tf.get_default_session() is sess
  print(c.eval())
```




使用函数tf.get_default_session()来得到当前默认的会话 

需要注意的是，退出该as_default上下文管理器时，并没有关闭该会话(session )，必须明确的关闭会话



```python
c = tf.constant(...)
sess = tf.Session()
with sess.as_default():
  print(c.eval())
# ...
with sess.as_default():
  print(c.eval())
#关闭会话
sess.close()
#使用 with tf.Session()方式可以创建并自动关闭会话
```



tf.InteractiveSession



```python
sess = tf.InteractiveSession()
a = tf.constant(5.0)
b = tf.constant(6.0)
c = a * b
# 我们直接使用'c.eval()' 而没有通过'sess'
print(c.eval())
sess.close()
```




以上的例子，在非交互会话的版本中为，



```python
a = tf.constant(5.0)
b = tf.constant(6.0)
c = a * b
with tf.Session():
  # We can also use 'c.eval()' here.
  print(c.eval())
```



# ABC

#### 错误类 (Error classes)
|操作|描述|
|----|----|
|class tf.OpError|一个基本的错误类型，在当TF执行失败时候报错|
|tf.OpError.op|返回执行失败的操作节点，有的操作如Send或Recv可能不会返回，那就要用用到node_def方法|
|tf.OpError.node_def|以NodeDef proto形式表示失败的op|
|tf.OpError.error_code|描述该错误的整数错误代码|
|tf.OpError.message|返回错误信息|
|class tf.errors.CancelledError|当操作或者阶段呗取消时候报错|
|class tf.errors.UnknownError|未知错误类型|
|class tf.errors.InvalidArgumentError|在接收到非法参数时候报错|
|class tf.errors.NotFoundError|当发现不存在所请求的一个实体时候，比如文件或目录|
|class tf.errors.AlreadyExistsError|当创建的实体已经存在的时候报错|
|class tf.errors.PermissionDeniedError|没有执行权限做某操作的时候报错|
|class tf.errors.ResourceExhaustedError|资源耗尽时报错|
|class tf.errors.FailedPreconditionError|系统没有条件执行某个行为时候报错|
|class tf.errors.AbortedError|操作中止时报错，常常发生在并发情形|
|class tf.errors.OutOfRangeError|超出范围报错|
|class tf.errors.UnimplementedError|某个操作没有执行时报错|
|class tf.errors.InternalError|当系统经历了一个内部错误时报出|
|class tf.errors.DataLossError|当出现不可恢复的错误例如在运行 [tf.WholeFileReader.read()](http://blog.csdn.net/lenbow/article/details/52181159)读取整个文件的同时文件被删减|
|tf.errors.XXXXX.__init__(node_def, op, message)|使用该形式方法创建以上各种错误类|


相关链接：


[1] 安装Tensorflow（Linux ubuntu） [http://blog.csdn.net/lenbow/article/details/51203526](http://blog.csdn.net/lenbow/article/details/51203526)

[2] ubuntu下CUDA编译的GCC降级安装 [http://blog.csdn.net/lenbow/article/details/51596706](http://blog.csdn.net/lenbow/article/details/51596706)

[3] ubuntu手动安装最新Nvidia显卡驱动 [http://blog.csdn.net/lenbow/article/details/51683783](http://blog.csdn.net/lenbow/article/details/51683783)

[4] Tensorflow的CUDA升级，以及相关配置 [http://blog.csdn.net/lenbow/article/details/52118116](http://blog.csdn.net/lenbow/article/details/52118116)

[5] 基于gensim的Doc2Vec简析 [http://blog.csdn.net/lenbow/article/details/52120230](http://blog.csdn.net/lenbow/article/details/52120230)

[6] TensorFlow的分布式学习框架简介 [http://blog.csdn.net/lenbow/article/details/52130565](http://blog.csdn.net/lenbow/article/details/52130565)

[7] Tensorflow一些常用基本概念与函数（1） [http://blog.csdn.net/lenbow/article/details/52152766](http://blog.csdn.net/lenbow/article/details/52152766)

[8] Tensorflow一些常用基本概念与函数（2） [http://blog.csdn.net/lenbow/article/details/52181159](http://blog.csdn.net/lenbow/article/details/52181159)](https://so.csdn.net/so/search/s.do?q=数据io&t=blog)](https://so.csdn.net/so/search/s.do?q=graph&t=blog)](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)




