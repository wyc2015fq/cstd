# 【Deep Learning】Tensorflow实现简单神经网络 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年04月23日 12:20:56[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：258








```python
'''
""" Neural Network.
A 2-Hidden Layers Fully Connected Neural Network (a.k.a Multilayer Perceptron)
implementation with TensorFlow. This example is using the MNIST database
of handwritten digits (http://yann.lecun.com/exdb/mnist/).
Links:
    [MNIST Dataset](http://yann.lecun.com/exdb/mnist/).
Author: Aymeric Damien
Project: https://github.com/aymericdamien/TensorFlow-Examples/
'''
from __future__ import print_function
import tensorflow as tf
import os
from sklearn.model_selection import train_test_split
import pandas as pd
import numpy as np
from sklearn import preprocessing

# Import data
def loadDataSet(fileName):
        dataMat =[]
        fr = open (fileName)
        for line in fr.readlines():
            lineArr = line.strip().split('\t')
            dataMat.append([float(lineArr[0]),float(lineArr[1]),float(lineArr[2])])
        return dataMat
homedir = os.getcwd()#get now path
dataMat = loadDataSet(homedir+'/testSet.txt')
dataDf=pd.DataFrame(dataMat)
Xtr, Xte, ytr, yte = train_test_split(dataDf.ix[:,0:1], dataDf.ix[:,2], test_size=0.1, random_state=0)#X[:,m:n],m to n-1
'''
enc = preprocessing.OneHotEncoder()
enc.fit(ytr).transform()
ytr=enc.transform(ytr).toarray()
'''
# Parameters
learning_rate = 0.001
num_steps = 500
display_step = 100

# Network Parameters
n_hidden_1 = 20 # 1st layer number of neurons
n_hidden_2 = 20 # 2nd layer number of neurons
num_input = 2 #  data input (2 dimensions)
num_classes = 2 #  total classes (0-1 digits)

#y labels one-hot
def dense_to_one_hot(labels_dense, num_classes):  
    """Convert class labels from scalars to one-hot vectors."""  
    num_labels = labels_dense.shape[0]  
    index_offset = np.arange(num_labels) * num_classes  
    labels_one_hot = np.zeros((num_labels, num_classes))  
    labels_one_hot.flat[[index_offset + labels_dense.ravel()]] = 1  
    return labels_one_hot 
ytr = dense_to_one_hot(ytr,num_classes)
yte = dense_to_one_hot(yte,num_classes)

# tf Graph input
X = tf.placeholder("float", [None, num_input])
Y = tf.placeholder("float", [None, num_classes]) 

# Store layers weight & bias
weights = {
    'h1': tf.Variable(tf.random_normal([num_input, n_hidden_1])),#num_input*n_hidden_1
    'h2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
    'out': tf.Variable(tf.random_normal([n_hidden_2, num_classes]))
}
biases = {
    'b1': tf.Variable(tf.random_normal([n_hidden_1])),
    'b2': tf.Variable(tf.random_normal([n_hidden_2])),
    'out': tf.Variable(tf.random_normal([num_classes]))
}

# Create model
def neural_net(x):
    # Hidden fully connected layer with 256 neurons
    layer_1 = tf.add(tf.matmul(x, weights['h1']), biases['b1'])
    # Hidden fully connected layer with 256 neurons
    layer_2 = tf.add(tf.matmul(layer_1, weights['h2']), biases['b2'])
    # Output fully connected layer with a neuron for each class
    out_layer = tf.matmul(layer_2, weights['out']) + biases['out']
    return out_layer

# Construct model
logits = neural_net(X)
prediction = tf.nn.sigmoid(logits)#two classifier

# Define loss and optimizer
loss_op = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=logits, labels=Y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate)
train_op = optimizer.minimize(loss_op)

# Evaluate model
correct_pred = tf.equal(tf.argmax(prediction, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))

# Initialize the variables (i.e. assign their default value)
init = tf.global_variables_initializer()

# Start training
with tf.Session() as sess:

    # Run the initializer
    sess.run(init)
    for step in range(1, num_steps+1):
        # Run optimization op (backprop)
        sess.run(train_op, feed_dict={X: Xtr, Y: ytr})
        if step % display_step == 0 or step == 1:
            # Calculate batch loss and accuracy
            loss, acc = sess.run([loss_op, accuracy], feed_dict={X: Xtr,Y: ytr})
            print("Step " + str(step) + ", Minibatch Loss= " +  "{:.4f}".format(loss) + ", Training Accuracy= " +"{:.3f}".format(acc))
    print("Optimization Finished!")

    # Calculate accuracy for MNIST test images
    print("Testing Accuracy:", sess.run(accuracy, feed_dict={X: Xte,Y: yte}))
```

数据testSet.txt

```
-0.017612	14.053064	0
-1.395634	4.662541	1
-0.752157	6.538620	0
-1.322371	7.152853	0
0.423363	11.054677	0
0.406704	7.067335	1
0.667394	12.741452	0
-2.460150	6.866805	1
0.569411	9.548755	0
-0.026632	10.427743	0
0.850433	6.920334	1
1.347183	13.175500	0
1.176813	3.167020	1
-1.781871	9.097953	0
-0.566606	5.749003	1
0.931635	1.589505	1
-0.024205	6.151823	1
-0.036453	2.690988	1
-0.196949	0.444165	1
1.014459	5.754399	1
1.985298	3.230619	1
-1.693453	-0.557540	1
-0.576525	11.778922	0
-0.346811	-1.678730	1
-2.124484	2.672471	1
1.217916	9.597015	0
-0.733928	9.098687	0
-3.642001	-1.618087	1
0.315985	3.523953	1
1.416614	9.619232	0
-0.386323	3.989286	1
0.556921	8.294984	1
1.224863	11.587360	0
-1.347803	-2.406051	1
1.196604	4.951851	1
0.275221	9.543647	0
0.470575	9.332488	0
-1.889567	9.542662	0
-1.527893	12.150579	0
-1.185247	11.309318	0
-0.445678	3.297303	1
1.042222	6.105155	1
-0.618787	10.320986	0
1.152083	0.548467	1
0.828534	2.676045	1
-1.237728	10.549033	0
-0.683565	-2.166125	1
0.229456	5.921938	1
-0.959885	11.555336	0
0.492911	10.993324	0
0.184992	8.721488	0
-0.355715	10.325976	0
-0.397822	8.058397	0
0.824839	13.730343	0
1.507278	5.027866	1
0.099671	6.835839	1
-0.344008	10.717485	0
1.785928	7.718645	1
-0.918801	11.560217	0
-0.364009	4.747300	1
-0.841722	4.119083	1
0.490426	1.960539	1
-0.007194	9.075792	0
0.356107	12.447863	0
0.342578	12.281162	0
-0.810823	-1.466018	1
2.530777	6.476801	1
1.296683	11.607559	0
0.475487	12.040035	0
-0.783277	11.009725	0
0.074798	11.023650	0
-1.337472	0.468339	1
-0.102781	13.763651	0
-0.147324	2.874846	1
0.518389	9.887035	0
1.015399	7.571882	0
-1.658086	-0.027255	1
1.319944	2.171228	1
2.056216	5.019981	1
-0.851633	4.375691	1
-1.510047	6.061992	0
-1.076637	-3.181888	1
1.821096	10.283990	0
3.010150	8.401766	1
-1.099458	1.688274	1
-0.834872	-1.733869	1
-0.846637	3.849075	1
1.400102	12.628781	0
1.752842	5.468166	1
0.078557	0.059736	1
0.089392	-0.715300	1
1.825662	12.693808	0
0.197445	9.744638	0
0.126117	0.922311	1
-0.679797	1.220530	1
0.677983	2.556666	1
0.761349	10.693862	0
-2.168791	0.143632	1
1.388610	9.341997	0
0.317029	14.739025	0
```

执行结果：

```
Step 1, Minibatch Loss= 45.5055, Training Accuracy= 0.489
Step 100, Minibatch Loss= 0.1006, Training Accuracy= 0.967
Step 200, Minibatch Loss= 0.0741, Training Accuracy= 0.967
Step 300, Minibatch Loss= 0.0736, Training Accuracy= 0.967
Step 400, Minibatch Loss= 0.0731, Training Accuracy= 0.967
Step 500, Minibatch Loss= 0.0726, Training Accuracy= 0.967
Optimization Finished!
Testing Accuracy: 0.8
```




