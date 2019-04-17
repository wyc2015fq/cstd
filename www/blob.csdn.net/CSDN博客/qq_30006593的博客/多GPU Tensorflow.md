# 多GPU Tensorflow - qq_30006593的博客 - CSDN博客





2017年08月16日 10:46:42[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：181








### Tensorflow的并行方法

[变量共享](https://www.tensorflow.org/programmers_guide/variable_scope#understanding_tfget_variable)

[collection]：创建变量到collection中

### TFRecord格式转换

[minist例子](https://github.com/tensorflow/tensorflow/blob/r1.2/tensorflow/examples/how_tos/reading_data/convert_to_records.py)与[官方文档](https://www.tensorflow.org/programmers_guide/reading_data)，一个不错的[博客](http://ycszen.github.io/2016/08/17/TensorFlow%E9%AB%98%E6%95%88%E8%AF%BB%E5%8F%96%E6%95%B0%E6%8D%AE/)
[TFRecord格式](https://www.tensorflow.org/api_guides/python/python_io#tfrecords_format_details)，使用[循环冗余校验](https://zh.wikipedia.org/wiki/%E5%BE%AA%E7%92%B0%E5%86%97%E9%A4%98%E6%A0%A1%E9%A9%97)，数据的格式为[Example的序列内存块](https://github.com/tensorflow/tensorflow/blob/r1.2/tensorflow/core/example/example.proto)，使用TFRecordWriter写成.tfrecords文件。

### 数据输入

[Reading Data](https://www.tensorflow.org/programmers_guide/reading_data#filenames_shuffling_and_epoch_limits)
[训练集与验证集间转换](https://stackoverflow.com/questions/44092387/how-to-use-tf-maybe-batch-to-switch-between-train-val-pipelines)
[api1.3 Data](https://www.tensorflow.org/versions/r1.3/programmers_guide/datasets#decoding_image_data_and_resizing_it)
[Queue博客](http://adventuresinmachinelearning.com/introduction-tensorflow-queuing/)
### 多GPU训练

[官方文档](https://www.tensorflow.org/tutorials/deep_cnn#code_organization)和[cifar10代码例子](https://github.com/tensorflow/models/blob/master/tutorials/image/cifar10/cifar10_multi_gpu_train.py)
[显存管理，占满问题](http://blog.csdn.net/sunwuhuang1/article/details/53946462)




