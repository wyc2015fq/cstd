# Hello World - PeterBishop - CSDN博客





2018年12月15日 14:46:53[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：18
个人分类：[TensorFlow](https://blog.csdn.net/qq_40061421/article/category/8461142)









```python
import tensorflow as tf

# Simple hello world using TensorFlow

# Create a Constant op
# The op is added as a node to the default graph.
#
# The value returned by the constructor represents the output
# of the Constant op.

hello = tf.constant('Hello, TensorFlow!')

# Start tf session
sess = tf.Session()

# Run graph
print(sess.run(hello))
#Hello, TensorFlow!
```





