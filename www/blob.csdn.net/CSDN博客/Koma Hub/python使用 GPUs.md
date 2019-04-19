# python使用 GPUs - Koma Hub - CSDN博客
2019年03月27日 21:58:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：598
个人分类：[Python](https://blog.csdn.net/Rong_Toa/article/category/7257892)
**Table of Contents**
[支持的设备](#%E6%94%AF%E6%8C%81%E7%9A%84%E8%AE%BE%E5%A4%87-)
[记录设备指派情况](#%E8%AE%B0%E5%BD%95%E8%AE%BE%E5%A4%87%E6%8C%87%E6%B4%BE%E6%83%85%E5%86%B5-)
[手工指派设备](#%E6%89%8B%E5%B7%A5%E6%8C%87%E6%B4%BE%E8%AE%BE%E5%A4%87-)
[在多GPU系统里使用单一GPU](#%E5%9C%A8%E5%A4%9Agpu%E7%B3%BB%E7%BB%9F%E9%87%8C%E4%BD%BF%E7%94%A8%E5%8D%95%E4%B8%80gpu)
[使用多个 GPU](#%E4%BD%BF%E7%94%A8%E5%A4%9A%E4%B8%AA-gpu-)
## 支持的设备
在一套标准的系统上通常有多个计算设备. TensorFlow 支持 CPU 和 GPU 这两种设备. 我们用指定字符串`strings` 来标识这些设备. 比如:
- `"/cpu:0"`: 机器中的 CPU
- `"/gpu:0"`: 机器中的 GPU, 如果你有一个的话.
- `"/gpu:1"`: 机器中的第二个 GPU, 以此类推...
如果一个 TensorFlow 的 operation 中兼有 CPU 和 GPU 的实现, 当这个算子被指派设备时, GPU 有优先权. 比如`matmul`中 CPU 和 GPU kernel 函数都存在. 那么在 `cpu:0` 和 `gpu:0` 中, `matmul` operation 会被指派给 `gpu:0` .
## 记录设备指派情况
为了获取你的 operations 和 Tensor 被指派到哪个设备上运行, 用 `log_device_placement` 新建一个 `session`, 并设置为 `True`.
```
# 新建一个 graph.
a = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[2, 3], name='a')
b = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[3, 2], name='b')
c = tf.matmul(a, b)
# 新建session with log_device_placement并设置为True.
sess = tf.Session(config=tf.ConfigProto(log_device_placement=True))
# 运行这个 op.
print sess.run(c)
```
你应该能看见以下输出:
```
Device mapping:
/job:localhost/replica:0/task:0/gpu:0 -> device: 0, name: Tesla K40c, pci bus
id: 0000:05:00.0
b: /job:localhost/replica:0/task:0/gpu:0
a: /job:localhost/replica:0/task:0/gpu:0
MatMul: /job:localhost/replica:0/task:0/gpu:0
[[ 22.  28.]
 [ 49.  64.]]
```
## 手工指派设备
如果你不想使用系统来为 operation 指派设备, 而是手工指派设备, 你可以用 `with tf.device` 创建一个设备环境, 这个环境下的 operation 都统一运行在环境指定的设备上.
```
# 新建一个graph.
with tf.device('/cpu:0'):
  a = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[2, 3], name='a')
  b = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[3, 2], name='b')
c = tf.matmul(a, b)
# 新建session with log_device_placement并设置为True.
sess = tf.Session(config=tf.ConfigProto(log_device_placement=True))
# 运行这个op.
print sess.run(c)
```
你会发现现在 `a` 和 `b` 操作都被指派给了 `cpu:0`.
```
Device mapping:
/job:localhost/replica:0/task:0/gpu:0 -> device: 0, name: Tesla K40c, pci bus
id: 0000:05:00.0
b: /job:localhost/replica:0/task:0/cpu:0
a: /job:localhost/replica:0/task:0/cpu:0
MatMul: /job:localhost/replica:0/task:0/gpu:0
[[ 22.  28.]
 [ 49.  64.]]
```
## 在多GPU系统里使用单一GPU
如果你的系统里有多个 GPU, 那么 ID 最小的 GPU 会默认使用. 如果你想用别的 GPU, 可以用下面的方法显式的声明你的偏好:
```
# 新建一个 graph.
with tf.device('/gpu:2'):
  a = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[2, 3], name='a')
  b = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[3, 2], name='b')
  c = tf.matmul(a, b)
# 新建 session with log_device_placement 并设置为 True.
sess = tf.Session(config=tf.ConfigProto(log_device_placement=True))
# 运行这个 op.
print sess.run(c)
```
如果你指定的设备不存在, 你会收到 `InvalidArgumentError` 错误提示:
```
InvalidArgumentError: Invalid argument: Cannot assign a device to node 'b':
Could not satisfy explicit device specification '/gpu:2'
   [[Node: b = Const[dtype=DT_FLOAT, value=Tensor<type: float shape: [3,2]
   values: 1 2 3...>, _device="/gpu:2"]()]]
```
为了避免出现你指定的设备不存在这种情况, 你可以在创建的 `session` 里把参数 `allow_soft_placement` 设置为 `True`, 这样 tensorFlow 会自动选择一个存在并且支持的设备来运行 operation.
```
# 新建一个 graph.
with tf.device('/gpu:2'):
  a = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[2, 3], name='a')
  b = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[3, 2], name='b')
  c = tf.matmul(a, b)
# 新建 session with log_device_placement 并设置为 True.
sess = tf.Session(config=tf.ConfigProto(
      allow_soft_placement=True, log_device_placement=True))
# 运行这个 op.
print sess.run(c)
```
## 使用多个 GPU
如果你想让 TensorFlow 在多个 GPU 上运行, 你可以建立 multi-tower 结构, 在这个结构 里每个 tower 分别被指配给不同的 GPU 运行. 比如:
```
# 新建一个 graph.
c = []
for d in ['/gpu:2', '/gpu:3']:
  with tf.device(d):
    a = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[2, 3])
    b = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[3, 2])
    c.append(tf.matmul(a, b))
with tf.device('/cpu:0'):
  sum = tf.add_n(c)
# 新建session with log_device_placement并设置为True.
sess = tf.Session(config=tf.ConfigProto(log_device_placement=True))
# 运行这个op.
print sess.run(sum)
```
你会看到如下输出:
```
Device mapping:
/job:localhost/replica:0/task:0/gpu:0 -> device: 0, name: Tesla K20m, pci bus
id: 0000:02:00.0
/job:localhost/replica:0/task:0/gpu:1 -> device: 1, name: Tesla K20m, pci bus
id: 0000:03:00.0
/job:localhost/replica:0/task:0/gpu:2 -> device: 2, name: Tesla K20m, pci bus
id: 0000:83:00.0
/job:localhost/replica:0/task:0/gpu:3 -> device: 3, name: Tesla K20m, pci bus
id: 0000:84:00.0
Const_3: /job:localhost/replica:0/task:0/gpu:3
Const_2: /job:localhost/replica:0/task:0/gpu:3
MatMul_1: /job:localhost/replica:0/task:0/gpu:3
Const_1: /job:localhost/replica:0/task:0/gpu:2
Const: /job:localhost/replica:0/task:0/gpu:2
MatMul: /job:localhost/replica:0/task:0/gpu:2
AddN: /job:localhost/replica:0/task:0/cpu:0
[[  44.   56.]
 [  98.  128.]]
```
[cifar10 tutorial](http://www.tensorfly.cn/tfdoc/tutorials/deep_cnn.html) 这个例子很好的演示了怎样用GPU集群训练.
> 
原文:[using_gpu](http://tensorflow.org/how_tos/using_gpu/index.md) 翻译:[@lianghyv](https://github.com/lianghyv) 校对:[Wiki](https://github.com/jikexueyuanwiki)
