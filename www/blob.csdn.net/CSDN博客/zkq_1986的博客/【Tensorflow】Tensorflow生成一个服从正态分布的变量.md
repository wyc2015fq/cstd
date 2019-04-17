# 【Tensorflow】Tensorflow生成一个服从正态分布的变量 - zkq_1986的博客 - CSDN博客





2018年10月23日 18:20:32[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：202








Tensorflow生成一个服从正态分布的变量：

```python
import tensorflow as tf

a = tf.get_variable('a',shape=[2,3],initializer=tf.random_normal_initializer(stddev=1))
with tf.Session() as sess:
    tf.initialize_all_variables().run()
    print(sess.run(a))
```

输出：

[[ 0.7048022  -0.23355268  0.84619343]

 [-0.96346605  0.78628266 -0.721249  ]]



