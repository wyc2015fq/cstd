# CNN练习之Digit Recognizer - 一个跳popping的quant的博客 - CSDN博客





2018年01月14日 20:06:01[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1475
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









经过了考试月，终于有时间写下代码了。但是发现手生了好多，很多东西都忘记了，于是就在kaggle中那个Digit Recognizer模块下，用CNN写了个简单的网络来熟悉下代码。

CNN之前在DQN中有写到，用它来进行特征提取。它在图片识别，以及对图片采样时作用很大。

先是从kaggle中下载了测试集和训练集，将它们放到项目中。

先将测试集和训练集的数据进行处理。

```
# -------------训练集处理-----------------
df = pd.read_csv('./data/train.csv')
df = df[:40000]
df_matrix = df.as_matrix().astype(float)

label = df_matrix[:, 0]
label = np.array(label)[:, np.newaxis]
x = df_matrix[:, 1:]

# 输入标签的OneHot转换 exg:[[0 0 0 1 0],[1 0 0 0 0]
ohe = OneHotEncoder()
ohe.fit(label)
label = ohe.fit_transform(label).toarray()

# 输入x的归一化 exg:[[0.1 0.2 0.3],[0.4 0.5 0.6]]
mm = MinMaxScaler()
mm.fit(x)
x = mm.fit_transform(x)

# -----------------测试集处理---------------
# len 28000
df_test = pd.read_csv('./data/test.csv')
df_test_matrix = df_test.as_matrix().astype(float)
test_input = df_test_matrix[:, :]
mm = MinMaxScaler()
mm.fit(test_input)
test_input = mm.fit_transform(test_input)
```

接下来进行模型设计，在进行卷积操作的时候用到的是tf.layers.conv2d和tf.layers.max_pooling2d来分别进行卷积和最大化池化操作。

tensorflow封装的这两个方法在使用时都比较直观。在下面的代码中，一些重要的参数加了注释。

其中28×28的图片经过第一次的卷积操作后还是28×28，第一次池化后变为14×14，第二次卷积后为14×14，第二次池化后变为7×7，最后得到了64张 7×7的平面。

卷积神经网络后接了两个全连接的神经网络。使用的是tf.layers.dense的方法。

```
x_input = tf.placeholder(tf.float32, [None, 784])
label_input = tf.placeholder(tf.float32, [None, 10])

# 改变x的格式转为4D的向量[batch, in_height, in_width, in_channel]
# in_channel彩色通道
x_image = tf.reshape(x_input, [-1, 28, 28, 1])

conv1 = tf.layers.conv2d(
    inputs=x_image,
    filters=32, # 卷积核的数量
    kernel_size=[5,5], # 采样窗口的大小
    padding='same',  # 边界进行填充0
    activation=tf.nn.relu,
    strides=(1,1) # 上下移动的步长是1
)

pool1 = tf.layers.max_pooling2d(
    inputs=conv1,
    pool_size=[2,2],  # 池化的窗口大小
    strides=2
)

conv2 = tf.layers.conv2d(
    inputs=pool1,
    filters=64,
    kernel_size=[5,5],
    padding='same',
    activation=tf.nn.relu,
    strides=(1,1)
)

pool2 = tf.layers.max_pooling2d(
    inputs=conv2,
    pool_size=[2,2],
    strides=2
)

pool2_flat = tf.reshape(pool2, [-1, 7*7*64])

# 全连接层的构建
l1 = tf.layers.dense(
    inputs=pool2_flat,
    units=1024,
    activation=tf.nn.relu
)

l2 = tf.layers.dense(
    inputs=l1,
    units=10,
    activation=tf.nn.softmax
)
```

接下来是构造Loss函数以及参数训练。

因为最后的输出层使用的是softmax函数，所以Loss函数使用tf.nn.softmax_cross_entropy_with_logits（），优化器使用AdamOptimizer（）。

```
loss = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels=label_input, logits=l2))

train = tf.train.AdamOptimizer(1e-4).minimize(loss)

label_prediction = tf.argmax(l2, axis=1)

correct_prediction = tf.equal(tf.argmax(l2, axis=1), tf.argmax(label_input, axis=1))

accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

init = tf.global_variables_initializer()
```

最后创建会话。 

其中需要注意的是在进行测试的时候，测试数据也要分batch进行训练，因为测试数据有28000条，如果直接丢进模型的话会容易内存不足而报Process finished with exit code -1073740791 (0xC0000409)的错误。

```
with tf.Session() as sess:
    sess.run(init)
    batch_total = 200
    batch_size = 40
    for epoch in range(18):
        for i in range(batch_total):
            x_batch = x[i*batch_size:(i+1)*batch_size, :]
            label_batch = label[i*batch_size:(i+1)*batch_size, :]
            sess.run(train, feed_dict={x_input:x_batch, label_input:label_batch})
        acc = sess.run(accuracy, feed_dict={x_input:x[:4000,:], label_input:label[:4000,:]})
        print('epoch'+str(epoch)+':'+str(acc))

    result = []
    batch_size = 200
    batch_test_total = len(test_input) // batch_size
    for i in range(batch_test_total):
        x_batch = test_input[i*batch_size:(i+1)*batch_size, :]
        list = sess.run(label_prediction, feed_dict={x_input: x_batch})
        result.extend(list)
    result_df = pd.DataFrame({'ImageId': range(1, len(result) + 1), 'Label': result})
    result_df.to_csv('./data/predictionOfcnn.csv', index=False)
```

最后就可以把生成的csv文件直接提交到kaggle上了。



