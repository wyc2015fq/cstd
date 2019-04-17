# TensorEditor ：一个小白都能快速玩转的神经网络搭建工具 - 知乎
# 



机器之心整理

**参与：思源**

> 近日，机器之心发现一个非常有意思的工具，可以用可视化的方式轻松添加卷积层、全连接层和池化层等层级，然后生成可执行的 TensorFlow 代码。此外，我们也尝试搭建一个简单的卷积架构，并在本地 TensorFlow 环境下测试生成的代码。

工具地址：[https://www.tensoreditor.com/](https://link.zhihu.com/?target=https%3A//www.tensoreditor.com/)

TensorEditor 是一个强大的机器学习工具，甚至小白都能以可视化的方式快速生成整个模型的代码。通过 TensorEditor，小白可以连接卷积层、全连接层和池化层等可视化结点创建整个模型，且我们可以将它们转化为 TensorFlow 和 Python 代码，并进一步在自己的环境中运行。

基本上，TensorEditor 的步骤即定义我们的数据集、图像或特征，然后创建深度神经网络并下载 Python 2.7 的代码，最后就需要在我们自己的 TensorFLow 环境下运行就好了。

通过 TensorEditor，我们不仅可以创建深度网络并避免一些常见的代码问题，同时还能生成基于 TensorFlow Estimator 的高效代码。如下所示，机器之心尝试构建了一个简单的卷积网络，我们使用了两个卷积层、两个池化层和一个全连接层，并在最后的 Estimator 使用了交叉熵损失函数和 Adagrad 最优化方法。
![](https://pic2.zhimg.com/v2-455efa6af2ed3f91833a4304410558d1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='393'></svg>)
上述简单搭建的卷积网络同样可以生成完全可执行的代码，这样可以避免大量的一般代码问题与重复性工作。




`import tensorflow as tf`

`import pandas as pd`




`tf.logging.set_verbosity(tf.logging.INFO)`




`project_name="CNN"`

`train_csv_file=""`

`test_csv_file=""`

`image_resize=[28,28]`




`def model_fn(features, labels, mode, params):`




`    convolutional_2d_1 = tf.layers.conv2d(`

`            inputs=features,`

`            filters=32,`

`            kernel_size=[3,3],`

`            strides=(1,1),`

`            padding="same",`

`            data_format="channels_last",`

`            dilation_rate=(1,1),`

`            activation=tf.nn.relu,`

`            use_bias=True)`




`    max_pool_2d_1 = tf.layers.max_pooling2d(`

`        inputs=convolutional_2d_1,`

`        pool_size=[2,2],`

`        strides=[2,2],`

`        padding='same',`

`        data_format='channels_last')`




`    convolutional_2d_2 = tf.layers.conv2d(`

`            inputs=max_pool_2d_1,`

`            filters=64,`

`            kernel_size=[3,3],`

`            strides=(1,1),`

`            padding="same",`

`            data_format="channels_last",`

`            dilation_rate=(1,1),`

`            activation=tf.nn.relu,`

`            use_bias=True)`




`    max_pool_2d_2 = tf.layers.max_pooling2d(`

`        inputs=max_pool_2d_1,`

`        pool_size=[2,2],`

`        strides=[2,2],`

`        padding='same',`

`        data_format='channels_last')`




`    convolutional_2d_3 = tf.layers.conv2d(`

`            inputs=max_pool_2d_2,`

`            filters=128,`

`            kernel_size=[3,3],`

`            strides=(1,1),`

`            padding="same",`

`            data_format="channels_last",`

`            dilation_rate=(1,1),`

`            activation=tf.nn.relu,`

`            use_bias=True)`




`    max_pool_2d_3 = tf.layers.max_pooling2d(`

`        inputs=convolutional_2d_3,`

`        pool_size=[2,2],`

`        strides=[2,2],`

`        padding='same',`

`        data_format='channels_last')`




`    flatten_1 = tf.reshape(max_pool_2d_3,``[-1,``2048])`




`    dense_1 = tf.layers.dense(inputs=flatten_1, units=1024, activation=tf.nn.relu)`




`    dropout_1= tf.layers.dropout(inputs=dense_1, rate=0.4, training=mode == tf.estimator.ModeKeys.TRAIN)`




`    dense_2 = tf.layers.dense(inputs=dropout_1, units=256, activation=tf.nn.relu)`




`    logits=dense_2`




`    predictions =``{`

`"classes": tf.argmax(input=logits, axis=1),`

`"probabilities": tf.nn.softmax(logits, name="softmax_tensor")`

`}`

`#Prediction and training`

`if mode == tf.estimator.ModeKeys.PREDICT:`

`return tf.estimator.EstimatorSpec(mode=mode, predictions=predictions)`




`#``Calculate``Loss``(for both TRAIN and EVAL modes)`

`    onehot_labels = tf.one_hot(indices=tf.cast(labels, tf.int32), depth=256)`

`    loss = tf.losses.softmax_cross_entropy(`

`        onehot_labels=onehot_labels, logits=logits)`




`#``Compute evaluation metrics.`

`    accuracy = tf.metrics.accuracy(labels=labels,`

`                                   predictions=predictions["classes"],`

`                                   name='acc_op')`

`    metrics =``{'accuracy': accuracy}`

`    tf.summary.scalar('accuracy', accuracy[1])`




`#``Configure the Training``Op``(for TRAIN mode)`

`if mode == tf.estimator.ModeKeys.TRAIN:`

`        optimizer = tf.train.AdagradOptimizer(learning_rate=0.001)`

`        train_op = optimizer.minimize(`

`            loss=loss,`

`            global_step=tf.train.get_global_step())`

`return tf.estimator.EstimatorSpec(mode=mode, loss=loss, train_op=train_op)`




`#``Add evaluation metrics (for EVAL mode)`

`    eval_metric_ops =``{`

`"accuracy": tf.metrics.accuracy(`

`            labels=labels, predictions=predictions["classes"])}`

`return tf.estimator.EstimatorSpec(`

`        mode=mode, loss=loss, eval_metric_ops=eval_metric_ops)`







`#``Parse CSV input file and resize image`

`def _parse_csv(line):`

`    parsed_line= tf.decode_csv(line,``[[""],``[]])`

`    filename = parsed_line[0]`

`    label = parsed_line[1]`

`    image_string = tf.read_file(filename)`

`    image_decoded = tf.image.decode_jpeg(image_string, channels=3)`

`    image_resized = tf.image.resize_images(image_decoded, image_resize)`

`    image_gray = tf.image.rgb_to_grayscale(image_resized)`




`return image_gray, label`







`def data_train_estimator():`

`    dataset = tf.data.TextLineDataset(train_csv_file).map(_parse_csv)``#``Map each line to convert the data`

`    dataset = dataset.batch(100)`

`    dataset = dataset.shuffle(1000)`

`    dataset = dataset.repeat()`

`    iterator = dataset.make_one_shot_iterator()``# create one shot iterator`

`    feature, label = iterator.get_next()`

`return feature, label`




`def data_test_estimator():`

`    dataset = tf.data.TextLineDataset(test_csv_file).map(_parse_csv)``#``Map each line to convert the data`

`    dataset = dataset.batch(100)`

`    iterator = dataset.make_one_shot_iterator()``# create one shot iterator`

`    feature, label = iterator.get_next()`

`return feature, label`







`def main(unused_argv):`

`# MAIN ENTRY`




`#``Create the Estimator`

`    classifier = tf.estimator.Estimator(`

`        model_fn=model_fn,`

`        model_dir="/tmp/"+project_name,`

`        params={`

`# PARAMS`

`}`

`)`




`    classifier.train(input_fn=data_train_estimator, steps=30000)`




`    eval_results = classifier.evaluate(input_fn=data_test_estimator)`

`    tf.summary.scalar("Accuracy", eval_results["accuracy"])`

`    print(eval_results)`




`if __name__ ==``"__main__":`

`    tf.app.run()`

TensorEditor 主要有以下特点：
- 易于使用：我们只需要添加模块、连接模块并在最后加入评估模块，就能完成搭建。
- 由易到难：只需要叠加不同的模块，我们就能创建如 VGG 那样的复杂深度网络。
- 参数直观：可以轻松修改各结点的配置与参数，从而搭建定制化的深度网络。
- 生成代码：搭建完深度架构，我们就能直接生成可执行的 TensorFlow 代码（Python 2.7）。

**90 秒的 MNIST 教程**
[https://v.qq.com/x/page/f134168kl8y.html​v.qq.com](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/f134168kl8y.html)
在上面的视频中，开发者展示了如何使用 TensorEditor 在 90 秒内快速搭建一个可用于 MNIST 手写数字识别的简单网络。对于 TensorEditor 这种构建序贯 CNN 模型的简单工具，我们只需要准备两件事就能开始搭建模型模型：
- 下载 MNIST 手写数据集：[https://github.com/damiles/TensorEditor_SampleData/raw/master/mnist_png.tar.gz](https://link.zhihu.com/?target=https%3A//github.com/damiles/TensorEditor_SampleData/raw/master/mnist_png.tar.gz)
- 确定网络架构：[https://www.tensorflow.org/tutorials/layers#building_the_cnn_mnist_classifier](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/tutorials/layers%23building_the_cnn_mnist_classifier)

TensorEditor 接受 CSV 格式的特征数据集或具有 CSV 标签的图像数据集作为数据输入，并且需要训练和测试/评估两个 CSV 文件。当我们从上面的链接下载数据集并提取图像数据时，我们会有两个 CSV 文件和两个包含所有图像的文件夹（测试和训练）。

现在我们就可以在 TensorEditor 中创建将要用于手写数字识别的卷积网络架构，下面展示的架构和 TensorFlow 文档中保持一致。
- 卷积层 1：使用 32 个 5x5 大小的卷积核和 ReLU 激活函数
- 池化层 1：使用 2x2 滤波器和步幅为 2 的最大池化运算（池化区域不重叠）
- 卷积层 2：使用 64 个 5x5 大小的卷积核和 ReLU 激活函数
- 池化层 2：同样使用 2x2 滤波器和步幅为 2 的最大池化运算
- 全连接层 1：1024 个神经元，Dropout 正则化率为 0.4
- 分类层：10 个神经元，每个神经元表示 0 到 9 这十个数字。

我们只需要按步骤先添加一个输入 csv 数据集模块，并设置 train.csv 和 test.csv 的地址。然后依次添加上述的卷积和全连接等模块，并设置好对应的参数，如卷积核大小、卷积核数量和激活函数等。最后主需要添加 Estimator 模块，并设置损失函数、最优化方法和学习率等配置就能完成架构上的搭建。如下所示为使用可视化方法搭建的架构：
![](https://pic4.zhimg.com/v2-22252397f0c7990a8ce8528f70b890bb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='432'></svg>)






最后上面的网络就能生成对应的代码，我们可直接复制到本地代码编辑器中并执行：






![](https://pic2.zhimg.com/v2-c5f0ef9f70379764f579b0fa8252c691_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='521'></svg>)



