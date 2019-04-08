# 深度学习AI美颜系列---性别识别

2018年05月09日 16:08:59

 

Trent1985

 

阅读数：3325

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/80253642

性别识别作为该系列的第一篇博文，是因为一张人像照片放在我们面前，我们首先需要判断这个人像照片是男是女，然后才能根据男女分开进行相应的美颜!

本文使用Tensorflow来实现性别识别。

**算法**

性别识别是一个简单的分类，男和女两类，我们使用简单的CNN来实现，CNN的网络结构如下：

![img](https://img-blog.csdn.net/20180509152114721)

Fig.1 性别识别CNN网络结构示意图

输入图片为大小为92X112的单通道灰度图像，如Fig.2所示，类别标签(男标签[1,0]，女标签[0,1])，所有参数均在网络结构图中标注。

![img](https://img-blog.csdn.net/20180509155833232)

Fig.2输入样例图

**工程代码**

数据集使用的是网络已有的数据集，下载连接在文末：

代码分为GenderUtils.py/GenderTrain.py/GenderTest.py三部分

GenderUtils.py中定义了相关的函数，如下：

```python
# AGE
import matplotlib.image as img
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
from tensorflow.python.framework import ops
import math
import os
import csv
 
def create_placeholders(n_H0, n_W0, n_C0, n_y):
    """
    Creates the placeholders for the tensorflow session.
    
    Arguments:
    n_H0 -- scalar, height of an input image
    n_W0 -- scalar, width of an input image
    n_C0 -- scalar, number of channels of the input
    n_y -- scalar, number of classes
        
    Returns:
    X -- placeholder for the data input, of shape [None, n_H0, n_W0, n_C0] and dtype "float"
    Y -- placeholder for the input labels, of shape [None, n_y] and dtype "float"
    """
 
    X = tf.placeholder(name='X', shape=(None, n_H0, n_W0, n_C0), dtype=tf.float32)
    Y = tf.placeholder(name='Y', shape=(None, n_y), dtype=tf.float32)    
    return X, Y
 
def random_mini_batches(X, Y, mini_batch_size = 64, seed = 0):
    """
    Creates a list of random minibatches from (X, Y)
    
    Arguments:
    X -- input data, of shape (input size, number of examples) (m, Hi, Wi, Ci)
    Y -- true "label" vector (containing 0 if cat, 1 if non-cat), of shape (1, number of examples) (m, n_y)
    mini_batch_size - size of the mini-batches, integer
    seed -- this is only for the purpose of grading, so that you're "random minibatches are the same as ours.
    
    Returns:
    mini_batches -- list of synchronous (mini_batch_X, mini_batch_Y)
    """
    
    m = X.shape[0]                  # number of training examples
    mini_batches = []
    np.random.seed(seed)
    
    # Step 1: Shuffle (X, Y)
    permutation = list(np.random.permutation(m))
    shuffled_X = X[permutation,:,:,:]
    shuffled_Y = Y[permutation,:]
 
    # Step 2: Partition (shuffled_X, shuffled_Y). Minus the end case.
    num_complete_minibatches = int(math.floor(m / mini_batch_size)) # number of mini batches of size mini_batch_size in your partitionning
    for k in range(0, int(num_complete_minibatches)):
        mini_batch_X = shuffled_X[k * mini_batch_size : k * mini_batch_size + mini_batch_size,:,:,:]
        mini_batch_Y = shuffled_Y[k * mini_batch_size : k * mini_batch_size + mini_batch_size,:]
        mini_batch = (mini_batch_X, mini_batch_Y)
        mini_batches.append(mini_batch)
    
    # Handling the end case (last mini-batch < mini_batch_size)
    if m % mini_batch_size != 0:
        mini_batch_X = shuffled_X[num_complete_minibatches * mini_batch_size : m,:,:,:]
        mini_batch_Y = shuffled_Y[num_complete_minibatches * mini_batch_size : m,:]
        mini_batch = (mini_batch_X, mini_batch_Y)
        mini_batches.append(mini_batch)
    
    return mini_batches
 
def row_csv2dict(csv_file):
    dict_club={}
    with open(csv_file)as f:
        reader=csv.reader(f,delimiter=',')
        for row in reader:
            dict_club[row[0]]=row[1]
    return dict_club
 
def input_data():
    
    path = "data/train/"
    train_num = sum([len(x) for _, _, x in os.walk(os.path.dirname(path))])
    image_train = np.zeros((train_num,112,92))
    label_train = np.ones((train_num,2))
    train_label_dict = row_csv2dict("data/train.csv")
    count = 0
    for key in train_label_dict:
        if int(train_label_dict[key]) == 0:
            label_train[count, 0] = 1
            label_train[count, 1] = 0
        else:
            label_train[count, 1] = 1
            label_train[count, 0] = 0
        filename = path + str(key)
        image_train[count] = img.imread(filename)
        count = count + 1
    path = "data/test/" 
    test_num = sum([len(x) for _, _, x in os.walk(os.path.dirname(path))])
    image_test = np.zeros((test_num, 112,92))
    label_test = np.ones((test_num,2))
    test_label_dict = row_csv2dict("data/test.csv")
    count = 0
    for key in test_label_dict:
        if int(test_label_dict[key]) == 0:
            label_test[count, 0] = 1
            label_test[count, 1] = 0
        else:
            label_test[count, 1] = 1
            label_test[count, 0] = 0
        filename = path + str(key)
        image_test[count] = img.imread(filename)
        count = count + 1
    return image_train, label_train,image_test, label_test
 
def weight_variable(shape,name):
    return tf.Variable(tf.truncated_normal(shape, stddev = 0.1),name=name)
 
def bias_variable(shape,name):
    return tf.Variable(tf.constant(0.1, shape = shape),name=name)
 
def conv2d(x,w,padding="SAME"):
    if padding=="SAME" :
        return tf.nn.conv2d(x, w, strides = [1,1,1,1], padding = "SAME")
    else:
        return tf.nn.conv2d(x, w, strides = [1,1,1,1], padding = "VALID")
    
def max_pool(x, kSize, Strides):
    return tf.nn.max_pool(x, ksize = [1,kSize,kSize,1],strides = [1,Strides,Strides,1], padding = "SAME")    
 
def compute_cost(Z3, Y):
    """
    Computes the cost
    
    Arguments:
    Z3 -- output of forward propagation (output of the last LINEAR unit), of shape (6, number of examples)
    Y -- "true" labels vector placeholder, same shape as Z3
    
    Returns:
    cost - Tensor of the cost function
    """
    cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=Z3, labels=Y))    
    return cost
 
def initialize_parameters():
    tf.set_random_seed(1)
    W1 = tf.cast(weight_variable([5,5,1,32],"W1"), dtype = tf.float32)
    b1 = tf.cast(bias_variable([32],"b1"), dtype = tf.float32)
    W2 = tf.cast(weight_variable([5,5,32,64],"W2"), dtype = tf.float32)
    b2 = tf.cast(bias_variable([64],"b2"), dtype = tf.float32)
    W3 = tf.cast(weight_variable([5,5,64,128],"W3"), dtype = tf.float32)
    b3 = tf.cast(bias_variable([128],"b3"), dtype = tf.float32)
    
    W4 = tf.cast(weight_variable([14*12*128,500],"W4"), dtype = tf.float32)
    b4 = tf.cast(bias_variable([500],"b4"), dtype = tf.float32)
    W5 = tf.cast(weight_variable([500,500],"W5"), dtype = tf.float32)
    b5 = tf.cast(bias_variable([500],"b5"), dtype = tf.float32)
    W6 = tf.cast(weight_variable([500,2],"W6"), dtype = tf.float32)
    b6 = tf.cast(bias_variable([2],"b6"), dtype = tf.float32)
    parameters = {"W1":W1,
                 "b1":b1,
                 "W2":W2,
                 "b2":b2,
                 "W3":W3,
                 "b3":b3,
                 "W4":W4,
                 "b4":b4,
                 "W5":W5,
                 "b5":b5,
                 "W6":W6,
                 "b6":b6}
    return parameters
 
def cnn_net(x, parameters, keep_prob = 1.0):
    #frist convolution layer
    w_conv1 = parameters["W1"]
    b_conv1 = parameters["b1"]
    h_conv1 = tf.nn.relu(conv2d(x,w_conv1) + b_conv1)  #output size 112x92x32
    h_pool1 = max_pool(h_conv1,2,2)    #output size 56x46x32
    
    #second convolution layer
    w_conv2 = parameters["W2"]
    b_conv2 = parameters["b2"]
    h_conv2 = tf.nn.relu(conv2d(h_pool1, w_conv2) + b_conv2) #output size 56x46x64
    h_pool2 = max_pool(h_conv2,2,2) #output size 28x23x64
    
    #third convolution layer
    w_conv3 = parameters["W3"]
    b_conv3 = parameters["b3"]
    h_conv3 = tf.nn.relu(conv2d(h_pool2,w_conv3) + b_conv3) #output size 28x23x128
    h_pool3 = max_pool(h_conv3,2,2) #output size 14x12x128
    
    #full convolution layer 
    w_fc1 = parameters["W4"]
    b_fc1 = parameters["b4"]
    h_fc11 = tf.reshape(h_pool3,[-1,14*12*128])
    h_fc1 = tf.nn.relu(tf.matmul(h_fc11,w_fc1) + b_fc1)
    
    w_fc2 = parameters["W5"]
    b_fc2 = parameters["b5"]
    h_fc2 = tf.nn.relu(tf.matmul(h_fc1,w_fc2)+b_fc2)
    h_fc2_drop = tf.nn.dropout(h_fc2,keep_prob)
    
    w_fc3 = parameters["W6"]
    b_fc3 = parameters["b6"]
    y_conv = tf.matmul(h_fc2_drop, w_fc3) + b_fc3
    #y_conv = tf.nn.softmax(tf.matmul(h_fc2_drop, w_fc3) + b_fc3)
    #rmse = tf.sqrt(tf.reduce_mean(tf.square(y_ - y_conv)))
    #cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels = y, logits = y_conv))
    #train_step = tf.train.GradientDescentOptimizer(0.001).minimize(cross_entropy)
    #correct_prediction  = tf.equal(tf.argmax(y_conv, 1), tf.argmax(y,1))
    #accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
    return y_conv
 
def save_model(saver,sess,save_path):
    path = saver.save(sess, save_path)
    print 'model save in :{0}'.format(path)
```

GenderTrain.py如下：

```python
# AGE
import matplotlib.image as img
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
from tensorflow.python.framework import ops
import math
import os
import csv
from GenderUtils import input_data,create_placeholders,random_mini_batches,row_csv2dict,weight_variable,bias_variable,conv2d,max_pool,compute_cost,initialize_parameters,cnn_net,save_model
np.random.seed(1)
tf.reset_default_graph()
 
def model(X_train, Y_train, X_test, Y_test,learning_rate = 0.001, num_epochs = 100, minibatch_size = 64, print_cost = True):
    """
    Implements a three-layer ConvNet in Tensorflow:
    CONV2D -> RELU -> MAXPOOL -> CONV2D -> RELU -> MAXPOOL -> FLATTEN -> FULLYCONNECTED
    Arguments:
    X_train -- training set, of shape (None, 112, 92, 1)
    Y_train -- test set, of shape (None, n_y = 2)
    X_test -- training set, of shape (None, 112, 92, 1)
    Y_test -- test set, of shape (None, n_y = 2)
    learning_rate -- learning rate of the optimization
    num_epochs -- number of epochs of the optimization loop
    minibatch_size -- size of a minibatch
    print_cost -- True to print the cost every 100 epochs
    Returns:
    train_accuracy -- real number, accuracy on the train set (X_train)
    test_accuracy -- real number, testing accuracy on the test set (X_test)
    parameters -- parameters learnt by the model. They can then be used to predict.
    """
#     ops.reset_default_graph()                         # to be able to rerun the model without overwriting tf variables
    tf.set_random_seed(1)                             # to keep results consistent (tensorflow seed)
    seed = 3                                          # to keep results consistent (numpy seed)   
    (m, n_H0, n_W0,n_C0) = X_train.shape   
    n_y = Y_train.shape[1]
    costs = [] 
    SAVE_PATH = "model/mymodel"
    print("X_train shape:",str(X_train.shape))
    # Create Placeholders of the correct shape
    X, Y = create_placeholders(n_H0, n_W0, n_C0, n_y)
    print("Y shape:", str(Y))
    # Initialize parameters
    parameters = initialize_parameters()
    # cnn
    Z3 = cnn_net(X, parameters)
    # Cost function
    cost = compute_cost(Z3, Y)
    # Backpropagation:Define the tensorflow optimizer.
    optimizer = tf.train.AdamOptimizer(learning_rate).minimize(cost)
    # Inizialize all the variables globally
    init = tf.global_variables_initializer()
    # training process
    saver = tf.train.Saver(max_to_keep=3)
    with tf.Session() as sess:
        # Run the initialization
        sess.run(init)
        # Do the training loop
        for epoch in range(num_epochs):
            minibatch_cost = 0.
            num_minibatches = int(m / minibatch_size)
            seed = seed + 1
            minibatches = random_mini_batches(X_train, Y_train, minibatch_size, seed)
            for minibatch in minibatches:
                # Select a minibatch
                (minibatch_X,minibatch_Y) = minibatch
                _,temp_cost = sess.run([optimizer, cost], feed_dict = {X:minibatch_X, Y:minibatch_Y})
                minibatch_cost += temp_cost / num_minibatches
            if print_cost == True and epoch % 5 == 0:
                print("Cost after epoch %i : %f" % (epoch, minibatch_cost))
            if print_cost == True and epoch % 1 == 0:
                costs.append(minibatch_cost)
        # plot the cost
        #plt.plot(np.squeeze(costs))
        #plt.ylabel("cost")
        #plt.xlabel("iterations (per tens)")
        #plt.title("Lerning ratge =" + str(learning_rate))
        #plt.show()
        # Calculate the correct predictions
        predict_op = tf.argmax(Z3, 1)
        correct_prediction = tf.equal(predict_op, tf.argmax(Y, 1))
        
        # Calculate accuracy on the test
        accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))
        print(accuracy)
        train_batch_num = int(math.floor(X_train.shape[0] / minibatch_size))
        train_accuracy = 0.
        for i in range(train_batch_num):
            train_accuracy += 1.0 / train_batch_num * accuracy.eval({X: X_train[i * minibatch_size:(i+1)*minibatch_size,:,:,:],Y:Y_train[i * minibatch_size:(i+1)*minibatch_size,:]})
        test_batch_num = int(X_test.shape[0] / minibatch_size)
        test_accuracy = 0.
        for i in range(test_batch_num):
            test_accuracy += 1.0 / test_batch_num * accuracy.eval({X: X_test[i * minibatch_size:(i+1)*minibatch_size,:,:,:],Y:Y_test[i * minibatch_size:(i+1)*minibatch_size,:]})
        print("Train Accuracy:", train_accuracy)
        print("Test Accuracy:", test_accuracy)
        save_model(saver,sess,SAVE_PATH)
        print("Z3's shape:", str(Z3.shape))
        return train_accuracy, test_accuracy, parameters
 
image_train, label_train, image_test, label_test = input_data()
image_train = image_train.reshape(image_train.shape[0],image_train.shape[1],image_train.shape[2],1)
image_test = image_test.reshape(image_test.shape[0],image_test.shape[1],image_test.shape[2],1)
image_train = image_train / 255.
image_test = image_test / 255.
model(image_train, label_train, image_test, label_test)

```

测试部分GenderTest.py:
```python
# AGE
import matplotlib.image as img
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
from tensorflow.python.framework import ops
import math
from GenderUtils import create_placeholders,weight_variable,bias_variable,conv2d,max_pool,compute_cost,initialize_parameters,cnn_net
np.random.seed(1)
tf.reset_default_graph()
 
parameters = initialize_parameters()
saver = tf.train.Saver()
with tf.Session() as sess:
    tf.set_random_seed(1) 
    sess.run(tf.global_variables_initializer())
    ckpt = tf.train.get_checkpoint_state(checkpoint_dir = 'model/')
    print(ckpt.model_checkpoint_path)
    saver.restore(sess,ckpt.model_checkpoint_path)
    parameters = {"W1":sess.run(parameters["W1"]),
                 "b1":sess.run(parameters["b1"]),
                 "W2":sess.run(parameters["W2"]),
                 "b2":sess.run(parameters["b2"]),
                 "W3":sess.run(parameters["W3"]),
                 "b3":sess.run(parameters["b3"]),
                 "W4":sess.run(parameters["W4"]),
                 "b4":sess.run(parameters["b4"]),
                 "W5":sess.run(parameters["W5"]),
                 "b5":sess.run(parameters["b5"]),
                 "W6":sess.run(parameters["W6"]),
                 "b6":sess.run(parameters["b6"])}
    #the image inputs is gray image with three channels.
    image = img.imread("data/T3.bmp")
    image_test = image[:,:,0]
    print("image_test shape:", str(image_test.shape))
    image = image_test.reshape(1,image_test.shape[0],image_test.shape[1],1)
#     image = image.reshape(1,image_test.shape[0],image_test.shape[1],1)
    image = image / 255.
    imaget = tf.image.convert_image_dtype(image, tf.float32)
    print("image shape: %", str(imaget.shape))
    res = cnn_net(imaget, parameters)
    print("result: ",sess.run(tf.argmax(res, 1)))
    print(str(res.shape))
    print(res.eval())
```

**结果**

由于样本比较少，训练结果如下：

![img](https://img-blog.csdn.net/20180509155423439)

Fig.3训练结果图

**下载连接**

最后给出整个工程代码及数据集的下载连接：

百度网盘：[链接](https://pan.baidu.com/s/1gRexoseFJAy99J2UiyyRUg) 密码: 5wst

Github：[连接](https://github.com/XiuSdk/DL-GenderRecognize/tree/master)

关于训练和测试数据读取的问题，请参考博客：[点击打开链接](https://blog.csdn.net/trent1985/article/details/80255414)