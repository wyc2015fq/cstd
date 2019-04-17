# tensorflow学习笔记（四十四）：用Queue保存更复杂的关系 - Keith - CSDN博客





2017年10月17日 12:22:36[ke1th](https://me.csdn.net/u012436149)阅读数：572
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









当查看 `tensorflow` 中提供的 `Queue` 的类的时候，会发现有个 `dtypes` 参数，这个参数代表 `Queue` 中元素是什么类型的，如果 `dtypes=[tf.string, tf.int64]` ，这个表示，`Queue` 中每个元素是 `(string, int)` 。

```python
import tensorflow as tf

queue = tf.FIFOQueue(capacity=100, dtypes=[tf.string, tf.int64])

# enqueue_many 的写法，两个元素放在两个列表里。
en_m = queue.enqueue_many([['hello', 'world'], [1, 2]])

# enqueue 的写法
en = queue.enqueue(['hello', 1])
deq = queue.dequeue()

with tf.Session() as sess:
    sess.run(en_m)
    print(sess.run(deq))
```



