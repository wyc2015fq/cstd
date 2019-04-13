
# keras系列︱keras是如何指定显卡且限制显存用量（GPU-CPU使用） - 素质云笔记-Recorder... - CSDN博客

2017年07月21日 10:59:24[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：17676


keras在使用GPU的时候有个特点，就是默认全部占满显存。
若单核GPU也无所谓，若是服务器GPU较多，性能较好，全部占满就太浪费了。
**于是乎有以下五种情况：**
1、指定GPU
2、使用固定显存的GPU
3、指定GPU + 固定显存
4    GPU动态增长
5    CPU充分占用
---一、固定显存的GPU

---本节来源于：深度学习theano/tensorflow多显卡多人使用问题集（参见：Limit the resource usage for tensorflow backend · Issue \#1538 · fchollet/keras · GitHub）

---在使用keras时候会出现总是占满GPU显存的情况，可以通过重设backend的GPU占用情况来进行调节。

---import tensorflow as tf
from keras.backend.tensorflow_backend import set_session
config = tf.ConfigProto()
config.gpu_options.per_process_gpu_memory_fraction = 0.3
set_session(tf.Session(config=config))

---需要注意的是，虽然代码或配置层面设置了对显存占用百分比阈值，但在实际运行中如果达到了这个阈值，程序有需要的话还是会突破这个阈值。换而言之如果跑在一个大数据集上还是会用到更多的显存。以上的显存限制仅仅为了在跑小数据集时避免对显存的浪费而已。（2017年2月20日补充）

---二、指定GPU

---import os
os.environ["CUDA_VISIBLE_DEVICES"] = "2"

---此时的代码为选择了编号为2 的GPU

---\# python设置系统变量的方法
os.environ["CUDA_VISIBLE_DEVICES"] = "8,9,10,11,12,13,14,15"

---注意，在代码中指定设备时，重新从0开始计，而不是从8开始。

---来源：

---[Tensorflow 学习笔记（七） ———— 多GPU操作](https://applenob.github.io/tf_7.html)

---三、指定GPU + 固定显存

---上述两个连一起用就行：

---import os
import tensorflow as tf
os.environ["CUDA_VISIBLE_DEVICES"] = "2"
from keras.backend.tensorflow_backend import set_session
config = tf.ConfigProto()
config.gpu_options.per_process_gpu_memory_fraction = 0.3
set_session(tf.Session(config=config))

---那么在命令行，可以使用：

---[https://github.com/tensorflow/nmt/issues/60](https://github.com/tensorflow/nmt/issues/60)

---CUDA_VISIBLE_DEVICES=0 python -m nmt.nmt

---四、GPU动态增长

---import keras.backend.tensorflow_backend as KTF
import tensorflow as tf
import os

os.environ["CUDA_VISIBLE_DEVICES"] = "1"
config = tf.ConfigProto()
config.gpu_options.allow_growth=True   \#不全部占满显存, 按需分配
sess = tf.Session(config=config)
KTF.set_session(sess)

---os.environ指的是占用的GPU编号；allow_growth为动态申请显存占用。

---五、tensorflow + CPU充分使用

---来自博客：

---[TensorFlow如何充分使用所有CPU核数，提高TensorFlow的CPU使用率，以及Intel的MKL加速](http://nooverfit.com/wp/tensorflow%E5%A6%82%E4%BD%95%E5%85%85%E5%88%86%E4%BD%BF%E7%94%A8%E6%89%80%E6%9C%89cpu%E6%A0%B8%E6%95%B0%EF%BC%8C%E6%8F%90%E9%AB%98tensorflow%E7%9A%84cpu%E4%BD%BF%E7%94%A8%E7%8E%87%EF%BC%8C%E4%BB%A5/)

---num_cores = 4
config = tf.ConfigProto(intra_op_parallelism_threads=num_cores, inter_op_parallelism_threads=num_cores,
                        allow_soft_placement=True, device_count={'CPU': 4})
session = tf.Session(config=config)
K.set_session(session)

---其中：

---device_count, 告诉tf Session使用CPU数量上限，如果你的CPU数量较多，可以适当加大这个值

---inter_op_parallelism_threads和intra_op_parallelism_threads告诉session操作的线程并行程度，如果值越小，线程的复用就越少，越可能使用较多的CPU核数。如果值为0，TF会自动选择一个合适的值。

---allow_soft_placement=True，   有时候，不同的设备，它的cpu和gpu是不同的，如果将这个选项设置成True，那么当运行设备不满足要求时，会自动分配GPU或者CPU。

---六 tf.keras使用多GPU

---DistributionStrategy API是构建多设备/机器训练的简单方式，开发者只需要在现有模型上做少量的修改，就可以用它们进行分布式训练。另外，DistributionStrategy在设计时考虑了同时兼容动态图(eager)和静态图。

---参考：

---[TensorFlow 1.11.0发布，一键多GPU（训练、预测和评价tf.keras模型）](https://mp.weixin.qq.com/s?__biz=MzU2OTA0NzE2NA==&mid=2247497759&idx=1&sn=e75864afb3f1e4f93c5ad97ae18d07d8&chksm=fc86190ccbf1901a84730118c82de91f1cd88dedbe3cd6a75c89f49aac86239d00dae4ca7ab3&mpshare=1&scene=1&srcid=0914TsCSCktd7FyyDMJD9Dbi#rd)

---目前TensorFlow支持三种DistributionStrategy：

---MirroredStrategy

---CollectiveAllReduceStrategy

---ParameterServerStrategy

---在tf.keras中直接使用DistributionStrategy

---最新的TensorFlow Github中给出了在tf.keras中直接使用DistributionStrategy的例子。

---用tf.keras构建一个单层网络：

---inputs = tf.keras.layers.Input(shape=(1,))
predictions = tf.keras.layers.Dense(1)(inputs)
model = tf.keras.models.Model(inputs=inputs, outputs=predictions)

---目前，使用DistributionStrategy需要使用tf.data.Dataset来作为数据输入：

---features = tf.data.Dataset.from_tensors([1.]).repeat(10000).batch(10)
labels = tf.data.Dataset.from_tensors([1.]).repeat(10000).batch(10)
train_dataset = tf.data.Dataset.zip((features, labels))

---这里我们为模型指定使用MirroredStrategy进行多GPU训练，代码非常简单：

---distribution = tf.contrib.distribute.MirroredStrategy()
model.compile(loss='mean_squared_error',
              optimizer=tf.train.GradientDescentOptimizer(learning_rate=0.2),
              distribute=distribution)

---使用常规的训练、评价和预测方法会自动在多GPU上进行：

---model.fit(train_dataset, epochs=5, steps_per_epoch=10)
model.evaluate(eval_dataset)
model.predict(predict_dataset)

---将tf.keras模型迁移到多GPU上运行只需要上面这些代码，它会自动切分输入、在每个设备（GPU）上复制层和变量、合并和更新梯度。

---**公众号“素质云笔记”定期更新博客内容：**

---![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


