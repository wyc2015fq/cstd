
# TensorFlow 1.0 新增功能和部分改善 - lyc_yongcai的博客 - CSDN博客


2017年06月14日 21:08:25[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：302个人分类：[TensorFlow																](https://blog.csdn.net/lyc_yongcai/article/category/6970987)


TensorFlow 1.0 新增功能及改善

XLA（实验版）：初始版本的XLA，针对TensorFlow图（graph）的专用编译器，面向CPU和GPU。
TensorFlow Debugger（tfdbg）：命令行界面和API。
添加了新的python 3 docker图像。
使pip包兼容pypi。TensorFlow 现在可以通过 [pip install tensorflow] 命令安装。
更改了几个python API的调用方式，使其更类似 NumPy。
新的（实验版）Java API。
Android：全新人物检测+跟踪演示实现——“Scalable Object Detection using DNN”（带有额外的YOLO对象检测器支持）。
Android：全新基于摄像头的图像风格转换演示，使用了神经网络艺术风格转换技术。
重大 API 变动
为了帮助您升级现有的TensorFlow Python代码以匹配以下API更改，我们准备了一个转换脚本，详见：https://github.com/tensorflow/tensorflow/tree/master/tensorflow/tools/compatibility
TensorFlow / models 已经被移动到一个单独的github库。
除法和模运算符（/，//，％）现在匹配Python（flooring）语义。这也适用于 [tf.div] 和 [tf.mod]。要获取基于强制整数截断的行为，可以使用 [tf.truncatediv] 和 [tf.truncatemod]。
现在推荐使用 [tf.divide()] 作为除法函数。[tf.div()] 将保留，但它的语义不会回应 Python 3 或 [from future] 机制。
tf.reverse() 现在取轴的索引要反转。例如 [tf.reverse（a，[True，False，True]）] 现在必须写为 [tf.reverse（a，[0，2]）]。 [tf.reverse_v2（）] 将保持到 TensorFlow 1.0 最终版。
[tf.mul，tf.sub ] 和 [tf.neg] 不再使用，改为 [tf.multiply]，[tf.subtract] 和 [tf.negative]。
[tf.pack] 和 [tf.unpack] 弃用，改为 [tf.stack] 和 [tf.unstack]。
[TensorArray.pack] 和 [TensorArray.unpack] 在弃用过程中，将来计划启用 [TensorArray.stack] 和 [TensorArray.unstack]。
以下Python函数的参数在引用特定域时，全部改为使用 [axis]。目前仍将保持旧的关键字参数的兼容性，但计划在 1.0 最终版完成前删除。
tf.listdiff 已重命名为 tf.setdiff1d 以匹配 NumPy 命名。
tf.inv 已被重命名 为tf.reciprocal（组件的倒数），以避免与 np.inv 的混淆，后者是矩阵求逆。
tf.round 现在使用 banker 的舍入（round to even）语义来匹配 NumPy。
tf.split 现在以相反的顺序并使用不同的关键字接受参数。我们现在将 NumPy order 匹配为tf.split（value，num_or_size_splits，axis）。
tf.sparse_split 现在采用相反顺序的参数，并使用不同的关键字。我们现在将  NumPy order 匹配为tf.sparse_split（sp_input，num_split，axis）。注意：我们暂时要求 tf.sparse_split 需要关键字参数。
tf.concat 现在以相反的顺序并使用不同的关键字接受参数。特别地，我们现在将NumPy order匹配为tf.concat（values，axis，name）。
默认情况下，tf.image.decode_jpeg 使用更快的DCT方法，牺牲一点保真度来提高速度。通过指定属性 dct_method ='INTEGER_ACCURATE'，可以恢复到旧版行为。
tf.complex_abs 已从 Python 界面中删除。 tf.abs 支持复杂张量，现在应该使用  tf.abs。
Template.var_scope 属性重命名为 .variable_scope
SyncReplicasOptimizer 已删除，SyncReplicasOptimizerV2 重命名为 SyncReplicasOptimizer。
tf.zeros_initializer（）和tf.ones_initializer（）现在返回一个必须用initializer参数调用的可调用值，在代码中用tf.zeros_initializer（）替换 tf.zeros_initializer。
SparseTensor.shape 已重命名为 SparseTensor.dense_shape。与 SparseTensorValue.shape 相同。
分别替换tf.scalar_summary，tf.histogram_summary，tf.audio_summary，tf.image_summary 与tf.summary.scalar，tf.summary.histogram，tf.summary.audio，tf.summary.image。新的摘要ops以名字而不是标签作为它们的第一个参数，意味着摘要ops现在尊重TensorFlow名称范围。
使用 tf.summary.FileWriter 和 tf.summary.FileWriterCache 替换 tf.train.SummaryWriter 和tf.train.SummaryWriterCache。
从公共 API中 删除 RegisterShape。使用C++形状函数注册。
Python API 中的 _ref dtypes 已经弃用。
在C++ API（in tensorflow/cc）中，Input，Output等已经从 tensorflow::ops 命名空间移动到 tensorflow。
将 {softmax，sparse_softmax，sigmoid} _cross_entropy_with_logits的arg order 更改为（labels，predictions），并强制使用已命名的args。
Bug 修改及其他变动
新的运算 op：parallel_stack。
为 RecordReader/RecordWriter 增加了 tf io 压缩选项常量。
添加了 sparse_column_with_vocabulary_file，指定将字符串特征转换为ID的特征栏（feature column）。
添加了 index_to_string_table，返回一个将索引映射到字符串的查找表。
添加 string_to_index_table，返回一个将字符串与索引匹配的查找表。
添加 ParallelForWithWorkerId 函数。
添加 string_to_index_table，返回一个将字符串与索引匹配的查找表。
支持从 contrib / session_bundle 中的v2中的检查点文件恢复会话。
添加了 tf.contrib.image.rotate 函数，进行任意大小角度旋转。
添加了 tf.contrib.framework.filter_variables 函数，过滤基于正则表达式的变量列表。
make_template（）可以添加 custom_getter_ param。
添加了关于如何处理 recursive_create_dir 现有目录的注释。
添加了QR因式分解的操作。
Python API 中的分割和 mod 现在使用 flooring（Python）语义。
Android：预构建的libs现在每晚构建。
Android： TensorFlow 推理库 cmake/gradle build 现在归在 contrib/android/cmake下面
Android：更强大的会话初始化（Session initialization）代码。
Android：当调试模式激活时，TF stats现在直接显示在demo和日志中
Android：全新/更好的 README.md 文档
saved_model可用作tf.saved_model。
Empty op 现在是有状态的。
提高CPU上ASSIGN运算的 scatter_update 的速度。
更改 reduce_join，使其处理 reduction_indices 的方式与其他 reduce_ops 相同。
将 TensorForestEstimator 移动到 contrib/tensor_forest。
默认情况下启用编译器优化，并允许在 configure 中进行配置。
使指标权重 broadcasting 更加严格。
添加新的类似队列的 StagingArea 和新运算 ops：stages 和 unstage。



