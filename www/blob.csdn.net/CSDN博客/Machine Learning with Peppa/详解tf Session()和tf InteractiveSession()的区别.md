# 详解tf.Session()和tf.InteractiveSession()的区别 - Machine Learning with Peppa - CSDN博客





2018年09月28日 16:36:56[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1215








他们之间的区别就是后者加载自身作为默认的Session。tensor.eval()和operation.run()可以直接使用

下面这三个是等价的：

```python
sess = tf.InteractiveSession()
sess = tf.Session()
with sess.as_default():
with tf.Session() as sess:
```

如下就会错：

```python
import tensorflow as tf

a = tf.constant(4)
b = tf.constant(7)
c = a + b
sess = tf.Session()
print(c.eval())
```

 raise ValueError("Cannot evaluate tensor using `eval()`: No default "

ValueError: Cannot evaluate tensor using `eval()`: No default session is registered. Use `with sess.as_default()` or pass an explicit session to `eval(session=sess)`

如果这样就没问题

```python
a = tf.constant(4)
b = tf.constant(7)

c = a + b
# sess = tf.Session()
with tf.Session() as sess:
    print(c.eval())
a = tf.constant(4)
b = tf.constant(7)

c = a + b
sess = tf.InteractiveSession()
print(c.eval())
```

下面是这两个函数的定义注释（其实官方讲的的很详细了）



```python
class Session(BaseSession):
  """A class for running TensorFlow operations.

  A `Session` object encapsulates the environment in which `Operation`
  objects are executed, and `Tensor` objects are evaluated. For
  example:

  ```python
  # Build a graph.
  a = tf.constant(5.0)
  b = tf.constant(6.0)
  c = a * b

  # Launch the graph in a session.
  sess = tf.Session()

  # Evaluate the tensor `c`.
  print(sess.run(c))
  ```

  A session may own resources, such as
  @{tf.Variable}, @{tf.QueueBase},
  and @{tf.ReaderBase}. It is important to release
  these resources when they are no longer required. To do this, either
  invoke the @{tf.Session.close} method on the session, or use
  the session as a context manager. The following two examples are
  equivalent:

  ```python
  # Using the `close()` method.
  sess = tf.Session()
  sess.run(...)
  sess.close()

  # Using the context manager.
  with tf.Session() as sess:
    sess.run(...)
  ```

  The
  [`ConfigProto`](https://www.tensorflow.org/code/tensorflow/core/protobuf/config.proto)
  protocol buffer exposes various configuration options for a
  session. For example, to create a session that uses soft constraints
  for device placement, and log the resulting placement decisions,
  create a session as follows:

  ```python
  # Launch the graph in a session that allows soft device placement and
  # logs the placement decisions.
  sess = tf.Session(config=tf.ConfigProto(allow_soft_placement=True,
                                          log_device_placement=True))
  ```
  """
```





