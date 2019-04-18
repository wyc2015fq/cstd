# 简单卷积神经网络的tensorboard可视化 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:09:54[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：5194

tensorboard是tensorflow官方提供的可视化工具。可以将模型训练中的数据汇总、显示出来。本文是基于tensorflow1.2版本的。这个版本的tensorboard的界面如图：
![1240](https://upload-images.jianshu.io/upload_images/4685306-e417d6c558fabf01.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
tensorboard支持8种可视化，也就是上图中的8个选项卡，它们分别是：
- SCALARS：标量曲线。例如准确率、损失率、权重和偏置等变化。在代码中tf.summary.scalar()定义需要在这个选项卡下展示的标量。
- IMAGES：数据图像。对于图像分类问题，可以将输入或者训练过程中的图片展示出来。在代码中用tf.summary.image()定义需要在这个选项卡中展示的数据（直接绘制成图像展示），默认是绘制3张图片。
- AUDIO：音频数据。还没有做过语音分析的例子，估计同上。
- GRAPHS：tensorflow的数据流图。数据流图的完善性，都需要在程序中详细定义。用with tf.name_scope()或者with  tf.name_scope() as scope来定义。
- DISTRIBUTIONS：数据分布图。可以用来显示激活前和激活后数据的分布情况，辅助设计分析。
- HISTOGRAMS：数据柱状图。在代码中用tf.summary.histogram()定义。
- EMBEDDINGS：用于文本分析，展示词向量的投影分布（例如word2vec）
tensorboard通过运行一个本地服务器，监听6006端口，在浏览器发出请求时，分析训练时记录的数据，绘制训练过程中的数据曲线、图像。
## tensorboard的代码实现：
首先需要申明数据保存的地址
` log_dir = './simple_cnn_log' # log dir to store all data and graph structure`
为了在tensorboard中展示节点名称，在设计网络时采用with tf.name_scope() 限定命名空间：
```
with tf.name_scope('input'):    
        x = tf.placeholder(tf.float32, [None, 784], name='x')
        y_ = tf.placeholder(tf.float32, [None, 10], name='y_')
        keep_prob = tf.placeholder(tf.float32,name='kp')
```
这个在`with  tf.name_scope('input')`中的所有节点都会被自动命名为“input/XXX”的格式。
定义variable_summaries函数对数据进行简单的统计分析，例如均值、方差、和极值。值得注意的是，这个函数中的输入参数var应该是用tf.Variable定义的权重、偏差。
```
def variable_summaries(name,var):
        with tf.name_scope(name+'_summaries'):
            mean = tf.reduce_mean(var)
        tf.summary.scalar(name+'_mean', mean)
        with tf.name_scope(name+'_stddev'):
            stddev = tf.sqrt(tf.reduce_mean(tf.square(var-mean)))
        tf.summary.scalar(name+'_stddev', stddev)
        tf.summary.scalar(name+'_max',tf.reduce_max(var))
        tf.summary.scalar(name+'_min',tf.reduce_min(var))
        tf.summary.histogram(name+'_histogram', var)
```
对于所有用tf.summary定义汇总记录的数据，需要进行汇总，以便程序输出和保存。
```
merged = tf.summary.merge_all()
    #merged = tf.summary.merge([input_summary,acc_summary])
    train_writer = tf.summary.FileWriter(log_dir+'/train',sess.graph)
    test_writer = tf.summary.FileWriter(log_dir+'/test')
```
这一步汇总的工作非常重要，通常用tf.summary.merge_all()函数实现，但是使用这个函数会有一些隐含的风险，尤其是交叉验证的时候，测试和训练需要的模型可能略有不同（例如测试时不需要dropout但是训练时需要dropout等类似的情况），但是会发生graph重叠的情况，可能会报错：
PlaceHolder error
并提示某个placeholder定义的变量没有赋值。这种情况可能的原因是在运行时多个graph混淆，前一次的tf .scalar_summary变量也需要被计算，所以会要求给前几次的placeholder变量赋值。
这种情况有两个解决方案：
1）在需要记录并可视化的数据不多的时候，可以分别定义，并用tf.merge_summary()函数实现汇总。
```
accuracy_summary = tf.scalar_summary("accuracy", accuracy)
loss_summary = tf.scalar_summary("loss", C)
merged = tf.merge_summary([accuracy_summary, loss_summary])
```
2)定义默认的graph。可以用with.with tf.Graph().as_default():的python结构定义默认graph，也可以声明每次运行都将当前运行作为默认graph，也就是声明：
`tf.reset_default_graph()`
## 交叉验证
实际执行具体的训练、测试和日志记录等操作。用tf.train.Saver()来创建模型保存器。之后进入训练循环后，每1步执行一次数据的记录和准确度的测试，每隔100步用tf.RunOptions()定义模型的运行选项、用tf.RunMetadata()定义元信息，对模型进行记录和保存。
在交叉验证训练的过程中，需要把训练结果保存到指定的路径log_dir。在会话运行时，将merged函数句柄传入sess.run中执行，得到的结果保存在summary，用train_writer.add_summary(summary，i)就可以保存运行数据了。
```
saver = tf.train.Saver()
    for i in range(max_steps):
        if i%10 == 0:
            summary, acc = sess.run([merged, accuarcy], feed_dict=feed_dict(False))
            test_writer.add_summary(summary, i)
            print('Accuracy at step %s: %s'%(i, acc))
        else:
            if i%100 == 99:
                continue
                run_options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
                run_metadata = tf.RunMetadata()
                summary, _ = sess.run([merged, train_step], feed_dict=feed_dict(True),
                                      options=run_options, run_metadata=run_metadata)
                train_writer.add_run_metadata(run_metadata,'step%03d'%i)
                train_writer.add_summary(summary,i)
                saver.save(sess,log_dir+'/model.ckpt',i)
                print('Adding run metadata for', i)
            else:
                summary, _ = sess.run([merged, train_step], feed_dict=feed_dict(True))
                train_writer.add_summary(summary, i)
```
## 启动tensorboard
完成代码后，重新打开一个终端后输入：
`tensorboard --logdir=./simple_cnn_log`
可以得到一个链接地址，将链接地址复制，用chrome浏览器打开，就可以看到刚才设置tensorboard的全貌了。
## 完整实现
最后附上简单卷积神经网络的tensorboard实现。
```
# ========================================
#       simple cnn with tensorboard
# ========================================
# define conv kernel
def weight_variable(shape):
    initial = tf.truncated_normal(shape,stddev=0.1)
    weight = tf.Variable(initial_value=initial)
    return weight
# define conv bias
def bias_variable(shape):
    initial = tf.constant(0.1,shape=shape)
    bias = tf.Variable(initial_value=initial)
    return bias
# define a simple conv operation 
def conv_op(in_tensor, kernel, strides=[1,1,1,1], padding='SAME'):
    conv_out = tf.nn.conv2d(in_tensor, kernel, strides=strides, padding=padding)
    return conv_out
# define max pooling operation
def max_pool_2x2(in_tensor,ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME'):
    max_pool = tf.nn.max_pool(in_tensor, ksize, strides, padding)
    return max_pool       
def simple_cnn_tensorboard(mnist):
    '''
    simple cnn with tensorboard visulization
    '''
    tf.reset_default_graph()
    log_dir = './simple_cnn_log' # log dir to store all data and graph structure
    sess = tf.InteractiveSession()
    # cnn structure
    max_steps = 1000
    learning_rate = 0.001
    dropout = 0.9
    w1 = [5,5,1,32]
    b1 = [32]
    w2 = [5,5,32,64]
    b2 = [64]
    wfc1 = [7*7*64,1024]
    bfc1 = [1024]
    wfc2 = [1024,10]
    bfc2 = [10]
    def variable_summaries(name,var):
        with tf.name_scope(name+'_summaries'):
            mean = tf.reduce_mean(var)
        tf.summary.scalar(name+'_mean', mean)
        with tf.name_scope(name+'_stddev'):
            stddev = tf.sqrt(tf.reduce_mean(tf.square(var-mean)))
        tf.summary.scalar(name+'_stddev', stddev)
        tf.summary.scalar(name+'_max',tf.reduce_max(var))
        tf.summary.scalar(name+'_min',tf.reduce_min(var))
        tf.summary.histogram(name+'_histogram', var)
    with tf.name_scope('input'):    
        x = tf.placeholder(tf.float32, [None, 784], name='x')
        y_ = tf.placeholder(tf.float32, [None, 10], name='y_')
        keep_prob = tf.placeholder(tf.float32,name='kp')
    with tf.name_scope('image_reshape'):
        x_image = tf.reshape(x,[-1, 28, 28, 1]) # 28*28 pic of 1 channel
        tf.summary.image('input', x_image)
    # 1st layer    
    with tf.name_scope('conv_layr1'):
        W_conv1 = weight_variable(w1);    variable_summaries('w1',W_conv1)
        b_conv1 = bias_variable(b1);      variable_summaries('b1',b_conv1)
        with tf.name_scope('Wx_plus_b'):
            pre_act = conv_op(x_image, W_conv1)+b_conv1
            tf.summary.histogram('pre_act',pre_act)
        h_conv1 = tf.nn.relu(pre_act, name='activiation')       
        h_pool1 = max_pool_2x2(h_conv1)
    # 2nd layer
    with tf.name_scope('conv_layr2'):
        W_conv2 = weight_variable(w2);    variable_summaries('w2',W_conv2)
        b_conv2 = bias_variable(b2);      variable_summaries('b2',b_conv2)
        h_conv2 = tf.nn.relu(conv_op(h_pool1, W_conv2)+b_conv2)
        h_pool2 = max_pool_2x2(h_conv2)
    # fc1
    with tf.name_scope('fc1'):
        h_pool2_flat = tf.reshape(h_pool2,[-1,7*7*64])
        W_fc1 = weight_variable(wfc1);    variable_summaries('w_fc1',W_fc1)
        b_fc1 = bias_variable(bfc1);      variable_summaries('b_fc1',b_fc1)
        h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1)+b_fc1, name='_act')
        # drop out
        h_fc1_drop = tf.nn.dropout(h_fc1,keep_prob=keep_prob)
    # fc2
    with tf.name_scope('fc2'):
        W_fc2 = weight_variable(wfc2);    variable_summaries('w_fc2',W_fc2)
        b_fc2 = bias_variable(bfc2);      variable_summaries('b_fc2',b_fc2)
        y_conv = tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2)+b_fc2,name='fc2_softmax')
        #tf.summary.scalar('softmax', y_conv)
    # loss function
    with tf.name_scope('cross_entropy'):
        cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_*tf.log(y_conv),reduction_indices=[1]), name='cross_entropy')
        #tf.summary.scalar('cross_entropy', cross_entropy)
    with tf.name_scope('train'):    
        train_step = tf.train.AdamOptimizer(learning_rate).minimize(cross_entropy)
    # estimate accuarcy
    with tf.name_scope('accuracy'):
        correct_prediction = tf.equal(tf.arg_max(y_conv,1), tf.arg_max(y_,1))
        accuarcy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
    #acc_summary = tf.summary.scalar('accuarcy', accuarcy)
    tf.summary.scalar('accuarcy', accuarcy)
    # summary all 
    merged = tf.summary.merge_all()
    #merged = tf.summary.merge([input_summary,acc_summary])
    train_writer = tf.summary.FileWriter(log_dir+'/train',sess.graph)
    test_writer = tf.summary.FileWriter(log_dir+'/test')
    tf.global_variables_initializer().run()
    def feed_dict(train):
        if train:
            xs, ys = mnist.train.next_batch(100)
            k = dropout
        else:
            xs, ys = mnist.test.images, mnist.test.labels
            k = 1.0
        return {x:xs, y_:ys, keep_prob:k}
    saver = tf.train.Saver()
    for i in range(max_steps):
        if i%10 == 0:
            summary, acc = sess.run([merged, accuarcy], feed_dict=feed_dict(False))
            test_writer.add_summary(summary, i)
            print('Accuracy at step %s: %s'%(i, acc))
        else:
            if i%100 == 99:
                continue
                run_options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
                run_metadata = tf.RunMetadata()
                summary, _ = sess.run([merged, train_step], feed_dict=feed_dict(True),
                                      options=run_options, run_metadata=run_metadata)
                train_writer.add_run_metadata(run_metadata,'step%03d'%i)
                train_writer.add_summary(summary,i)
                saver.save(sess,log_dir+'/model.ckpt',i)
                print('Adding run metadata for', i)
            else:
                summary, _ = sess.run([merged, train_step], feed_dict=feed_dict(True))
                train_writer.add_summary(summary, i)                
    train_writer.close()
    test_writer.close()  
    return
```
运行后得到tensorboard的可视化如下图。首先是SCALAR选项卡，可以看到，按照代码中的with语句，已经将标量数据分别存放。
![1240](https://upload-images.jianshu.io/upload_images/4685306-d89630d9b7360520.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
点击accuarcy后可以看到准确度的变化情况：
![1240](https://upload-images.jianshu.io/upload_images/4685306-5684fc23df94680a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
在IMAGE选项卡中，是输入的图像数据。分test和train两类。
在DISTRIBUTION选项卡中，可以看到激活前数据、权重和偏置的分布情况：
![1240](https://upload-images.jianshu.io/upload_images/4685306-31d80af9c11e919d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
HISTOGRAM选项卡：
![1240](https://upload-images.jianshu.io/upload_images/4685306-0829058701dc7bc8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
最后，给出tensorflow graph：
![1240](https://upload-images.jianshu.io/upload_images/4685306-5396e44e62737f2c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
参考：
1《tensorflow实战》
2《tensorflow技术解析与实战》
3 [https://stackoverflow.com/questions/35413618/tensorflow-placeholder-error-when-using-tf-merge-all-summaries](https://stackoverflow.com/questions/35413618/tensorflow-placeholder-error-when-using-tf-merge-all-summaries)
4 [https://segmentfault.com/a/1190000007846181](https://segmentfault.com/a/1190000007846181)
