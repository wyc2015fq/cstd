# keras上手之：与tensorflow混合编程 - happyhorizon的算法天空 - CSDN博客
2017年12月02日 21:33:58[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：7054
tensorflow具备许多优秀的函数和功能，比如tensorboard，keras作为tensorflow的高级API， 封装很多tensorflow的代码，使得代码模块化，非常方便。 
当然，由于keras的模型和层与tensorflow的张量高度兼容，可以用keras建模，用tensorflow输入输出。 
例如下面的例子：
```
import tensorflow as tf
from keras import backend as K
from keras.layers import Dense
from keras.objectives import categorical_crossentropy
from keras.metrics import categorical_accuracy as accuracy
from tensorflow.examples.tutorials.mnist import input_data
# create a tf session，and register with keras。
sess = tf.Session()
K.set_session(sess)
# this place holder is the same with input layer in keras
img = tf.placeholder(tf.float32, shape=(None, 784))
# keras layers can be called on tensorflow tensors
x = Dense(128, activation='relu')(img)
x = Dense(128, activation='relu')(x)
preds = Dense(10, activation='softmax')(x)
# label
labels = tf.placeholder(tf.float32, shape=(None, 10))
# loss function
loss = tf.reduce_mean(categorical_crossentropy(labels, preds))
train_step = tf.train.GradientDescentOptimizer(0.5).minimize(loss)
mnist_data = input_data.read_data_sets('MNIST_data', one_hot=True)
# initialize all variables
init_op = tf.global_variables_initializer()
sess.run(init_op)
with sess.as_default():
    for i in range(1000):
        batch = mnist_data.train.next_batch(50)
        train_step.run(feed_dict={img:batch[0],
                                  labels:batch[1]})
acc_value = accuracy(labels, preds)
with sess.as_default():
    print(acc_value.eval(feed_dict={img:mnist_data.test.images,
                                    labels:mnist_data.test.labels}))
```
上述代码中，在训练阶段直接采用了tf的方式，甚至都没有定义keras的model！最重要的一步就是这里:
```
# create a tf session，and register with keras。
sess = tf.Session()
K.set_session(sess)
```
创建一个TensorFlow会话并且注册Keras。这意味着Keras将使用我们注册的会话来初始化它在内部创建的所有变量。 
keras的层和模型都充分兼容tensorflow的各种scope, 例如name scope，device scope和graph scope。修改一下，在tensorboard输出训练过程中的loss曲线：
```python
import tensorflow as tf
from keras import backend as K
from keras.layers import Dense
from keras.objectives import categorical_crossentropy
from keras.metrics import categorical_accuracy as accuracy
from tensorflow.examples.tutorials.mnist import input_data
sess = tf.Session()
K.set_session(sess)
with tf.name_scope('input'):
    # this place holder is the same with input layer in keras
    img = tf.placeholder(tf.float32, shape=(None, 784))
    labels = tf.placeholder(tf.float32, shape=(None, 10))
mnist_data = input_data.read_data_sets('MNIST_data', one_hot=True)
def feed_dict(train):
    if train:
        xs, ys = mnist_data.train.next_batch(50)
    else:
        xs, ys = mnist_data.test.images, mnist_data.test.labels
    return {img:xs, labels:ys}
# keras layers can be called on tensorflow tensors
with tf.name_scope('NN'):
    x = Dense(128, activation='relu')(img)
    x = Dense(128, activation='relu')(x)
    preds = Dense(10, activation='softmax')(x)
with tf.name_scope('loss'):
    loss = tf.reduce_mean(categorical_crossentropy(labels, preds))
# tensorboard
writer = tf.summary.FileWriter('./keras_tensorflow_log/')
outloss = tf.summary.scalar('loss', loss)
merged = tf.summary.merge([outloss])
with tf.name_scope('train'):
    train_step = tf.train.GradientDescentOptimizer(0.5).minimize(loss)
# initialize all variables
init_op = tf.global_variables_initializer()
sess.run(init_op)
with sess.as_default():
    for i in range(1000):
        summary, loss = sess.run([merged, train_step], 
                 feed_dict=feed_dict(True))
        writer.add_summary(summary, global_step=i)
writer.close()
```
在命令行输入： 
`tensorboard --logdir=./keras_tensorflow_log`
打开tensorboard就可以看到loss history了： 
![这里写图片描述](https://img-blog.csdn.net/20171202213321874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
