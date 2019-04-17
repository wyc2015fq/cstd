# tensorflow的坑 - zqjackking的博客 - CSDN博客





2018年12月10日 14:35:21[zqjackking](https://me.csdn.net/zqjackking)阅读数：39








TFRecordReader “OutOfRangeError (see above for traceback): RandomShuffleQueue ‘_1_shuffle_batch/random_shuffle_queue’ is closed and has insufficient elements (requested 1, current size 0)” 问题原因总结；
- 
tf.decode_raw(features[‘image_raw’],tf.uint8) 解码时，数据类型有没有错？tf.float32 和tf.uint8有没有弄混？？？

- 
tf.train.string_input_producer([data_file],num_epochs=1) 如果设置num_epochs=1参数，请添加上 tf.local_variables_initializer()

- 
你的数据集通道有没有搞错？真的都是三通道或者都是单通道么？有没有可能单通道图像混在在三通道图像当中呢？

- 
图像的大小之前resize了么？前后一直么？


总结一下：一般遇到这个错误，代码本身没有问题，基本上都是参数不一致导致的，注意检查各个参数。



