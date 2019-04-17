# python numpy 三行代码打乱训练数据 - Snoopy_Dream - CSDN博客





2018年12月04日 20:27:36[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：165








```python
>>> permutation = np.random.permutation(train_label.shape[0])#记下第一维度的打乱顺序
>>> shuffled_dataset = train_data[permutation, :, :]#按照顺序索引
>>> shuffled_labels = train_label[permutation]
```

首先,和np.random.shuffle的区别，shuffle是直接在源数据上进行操作，而permutation是复制操作，如果是多个维度，默认对第一维度进行操作。但是注意np.random.permulation(a) a能使np.array，可以是list等。

[参考链接：https://www.cnblogs.com/jermmyhsu/p/8195658.html](https://www.cnblogs.com/jermmyhsu/p/8195658.html)



