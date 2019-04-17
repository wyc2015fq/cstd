# 【Tensorflow】tf.nn.dropout函数 - zj360202的专栏 - CSDN博客





2017年04月20日 12:36:41[zj360202](https://me.csdn.net/zj360202)阅读数：4799








### `tf.nn.dropout(x, keep_prob, noise_shape=None, seed=None, name=None)`

此函数是为了防止在训练中过拟合的操作，将训练输出按一定规则进行变换




参数：

- x：输入
- keep_prob：保留比例。        取值 (0,1] 。每一个参数都将按这个比例随机变更
- noise_shape：干扰形状。     
此字段默认是None，表示第一个元素的操作都是独立，但是也不一定。比例：数据的形状是shape(x)=[k, l, m, n]，而noise_shape=[k, 1, 1, n]，则第1和4列是独立保留或删除，第2和3列是要么全部保留，要么全部删除。
- seed：了解不多
- name


返回：Tnesor




```java
tf.nn.dropout(20,0.8)
```



