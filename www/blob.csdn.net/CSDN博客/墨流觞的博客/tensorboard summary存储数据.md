# tensorboard summary存储数据 - 墨流觞的博客 - CSDN博客





2018年09月10日 00:00:54[墨氲](https://me.csdn.net/dss_dssssd)阅读数：333








### 利用summary存储数据

总共分为4步 
![](p.jpg)![使用步骤](https://img-blog.csdn.net/20180909235850885?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 添加记录节点 

tf.summary.scalar(name, tensor)
- 汇总记录节点 

summary_op = tf.summary.merge_all()
- 运行汇总节点 

summary_str = sess.run(summary, feed_dict=feed_dict)
- 将数据存到文件中 
- summary_write = tf.summary.FileWriter(logdir, 

                                      graph=sess.graph) 
- summary_write.add_summary(summary_str, global_step=global_step)

## 可视化显示

tensorboard –logdir=绝对路径

然后在浏览器中打开相应网站，比如[http://desktop-45es9ip:6006](http://desktop-45es9ip:6006)即可

**注意，路径不需要加引号**

## 几个注意事项
- 
global_step 是tensorflow内部会自动维护的变量，在使用时,先用以下代码创建，trainable=False表明此 变量为非训练参数。

```python
global_step = tf.Variable(0, name='global_step', trainable=False)
```

然后将参数传递给optimizer.minimize中的默认参数global_step中，每训练一次，global_step会+1，之后在别的需要此参数的函数调用即可

```python
optimizer = tf.train.GradientDescentOptimizer(learning_rate)
    train_op = optimizer.minimize(cost, global_step=global_step)


# 在后边add_summary函数中调用

summary_write.add_summary(summary_str, global_step=sess.run(global_step))
```

- 
以下代码不可以写在for循环中

> 
summary_write = tf.summary.FileWriter(logdir,graph=sess.graph)



## 例子

接下来有一个例子，用逻辑回归和三层的神经网络结构来在mnist识别手写体，并将这两天的知识点包含在其中了。

```python
import tensorflow as tf
    from tensorflow.examples.tutorials.mnist import input_data

    data_dir = 'temp'
    mnist = input_data.read_data_sets(data_dir, one_hot=True)
    # import pickle

    # def load_from_pickle(data_type):
    #   file_name = '{}.pickle'.format(data_type)
    #   with open(file_name, 'rb') as f:
    #       return pickle.load(f)
    #
    # # load data from disk
    # train = load_from_pickle('train')
    # validation = load_from_pickle('validation')
    # test = load_from_pickle('test')
    # train_image, train_label = train['image'], train['label']
    # validation_image, validation_label = validation['image'], validation['label']
    # test_image, test_label = test['image'], test['label']

    def layer(input, weight_shape, bias_shape):
        # how to set stddev ?
        # the variance of weights in a network should be 2 / n_in,
        # n_in is the nummber inputs coming into the neuron
        weight_stddev = (2.0 / weight_shape[0]) ** 0.5
        w_innit = tf.random_normal_initializer(stddev=weight_stddev)
        bias_init = tf.constant_initializer(value=0.)
        W = tf.get_variable('W', weight_shape, initializer=w_innit)
        b = tf.get_variable('b', bias_shape, initializer=bias_init)

        return tf.nn.relu(tf.matmul(input, W) + b)


    def inference(x, model=0):
        '''

        :param x:
        :param model: 0 --> logistic regression   1--> multilayer neural network
        :return:
        '''
        if model == 0:
            init = tf.constant_initializer(value=0)

            W = tf.get_variable('W', [784, 10], initializer=init)
            b = tf.get_variable('b', [10], initializer=init)

            output = tf.nn.softmax(tf.matmul(x, W)) + b
            return output
        elif model == 1:
            with tf.variable_scope("hidden_1"):
                hidden_1 = layer(x, [784, 256], [256])
            with tf.variable_scope("hidden_2"):
                hidden_2 = layer(hidden_1, [256, 256], [256])
            with tf.variable_scope("hidden_3"):
                output = layer(hidden_2, [256, 10], [10])
                return output

    def loss(output, y, choice=0):
        '''

        :param output:
        :param y:
        :param choice: 0 --> implement by self  1--> call tf implementation
        :return:
        '''
        # implemented by self
        if choice == 0:

            # cross_entropy
            dot_product = y * tf.log(output)
            # sum in row
            xentropy = -tf.reduce_sum(dot_product, axis=1)
        # tf implementation
        elif choice == 1:
            xentropy = tf.nn.softmax_cross_entropy_with_logits(labels=y,
                                                               logits=output)

        # mean on every item
        loss = tf.reduce_mean(xentropy)

        return loss

    def training(cost, global_step, learning_rate):
        # add to summary
        # summary  records data used for tensorboard
        tf.summary.scalar('cost', cost)  # !!!!!!!! 1

        optimizer = tf.train.GradientDescentOptimizer(learning_rate)
        train_op = optimizer.minimize(cost, global_step=global_step)
        return train_op

    def evaluate(output, y):
        correct_prediction = tf.equal(tf.argmax(output, axis=1),
                                      tf.argmax(y, axis=1))
        accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
        return accuracy

    # python script
    # parameters
    learning_rate = 0.01
    training_epochs = 10
    batch_size = 1000
    display_step = 1

    with tf.Graph().as_default():
        # mnist data image of shape 28 * 28 = 784
        x = tf.placeholder(dtype=tf.float32, shape=[None, 784])
        # 0~9 digits recognition => 10 classes
        y = tf.placeholder(dtype=tf.float32, shape=[None, 10])

        output = inference(x, model=1)
        cost = loss(output, y, choice=1)

        # create a not trainable variable to record train step
        global_step = tf.Variable(0, name='global_step', trainable=False)

        train_op = training(cost, global_step, learning_rate)
        eval_op = evaluate(output, y)


        # record data
        summary_op = tf.summary.merge_all() # !!!!!! 2


        saver = tf.train.Saver()
        with tf.Session() as sess:

            summary_write = tf.summary.FileWriter("logs",
                                    graph=sess.graph) # !!!!!!!!!!!! 3
            init_op = tf.global_variables_initializer()
            sess.run(init_op)

            # training cycle
            for epoch in range(training_epochs):

                avg_cost = 0.
                total_batch = int(mnist.train.num_examples / batch_size)

                # loop over all batches
                for i in range(total_batch):
                    mbatch_x, mbatch_y = mnist.train.next_batch(batch_size)
                    # fit training using batch data
                    feed_dict = {x: mbatch_x, y: mbatch_y}
                    sess.run(train_op, feed_dict=feed_dict)

                    # compute average loss
                    minibatch_cost = sess.run(cost, feed_dict=feed_dict)
                    avg_cost += minibatch_cost / total_batch


                    # display logs every 1 epoch step
                    if epoch % display_step == 0:
                        val_feed_dict = {
                            x: mnist.validation.images,
                            y: mnist.validation.labels
                        }

                        accuracy = sess.run(eval_op, feed_dict=val_feed_dict)

                        print("Validation error: {}".format(1 - accuracy))


                        ##    !!!!!!!!!!!!!!! 4
                        summary_str = sess.run(summary_op, feed_dict=feed_dict)
                        summary_write.add_summary(summary_str, global_step=sess.run(global_step))
                        last_global_step = sess.run(global_step)
                        print("训练次数：", last_global_step)
                        # saver.save(sess, 'logistic_logs/model-checkpoint', global_step=global_step)
            print('optimization finished')
            test_feed_dict = {
                x: mnist.test.images,
                y: mnist.test.labels
            }
            accuracy = sess.run(eval_op, feed_dict=test_feed_dict)
            print("Test Accuracy:{}".format(accuracy))
            summary_write.close()
```





