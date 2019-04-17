# tensorflow：使用tfrecords时的注意事项 - Keith - CSDN博客





2017年08月05日 12:09:01[ke1th](https://me.csdn.net/u012436149)阅读数：3081
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)








- 
**确保 string_input_producer 中的文件名字是正确的。 **

- 
`string_input_producer(file_names, num_epochs=100000, shuffle=True)` 当指定 num_epochs 时，在初始化模型参数的时候，一定要 记得 `tf.local_variables_initializer().run()` , 因为 tf 会将 num_epoch 作为 `local variable`。 否则的话，会报错 `Attempting to use uninitialized value ReadData/input_producer/limit_epochs/epochs`

- 
**解码 tfrecords 时的类型一定要和制作 tfreords 时的类型一致：**

这个问题主要出现在 bytestring 上，在保存图片数据时候，我们通常会
- 将图片  .tostring()  转成 bytestring
- 制作 tfrecords
- 然后在解码的时候，我们会用 decode_raw 将bytestring 解码出来。
- `decode_row(bytes, out_type)`

这里要注意的是，`out_type`一定要和 .tostring() 之前的数据类型一致。

- 
**`Cannot create initializer for non-floating point type.`**

if you encounted this error, you shoud check whether the image casted to float32 dtype

- 
需要 batch_size 个样本，但是获得 0 个: 这种问题应该检查
- string input producer 中的文件名是否正确
- 有没有 `start_queue_runner`
- 有没有不小心将 执行 `enqueue_op` 的线程给关了




