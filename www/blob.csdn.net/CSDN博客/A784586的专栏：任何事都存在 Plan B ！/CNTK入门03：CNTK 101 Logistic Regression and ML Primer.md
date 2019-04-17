# CNTK入门03：CNTK 101: Logistic Regression and ML Primer - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年05月15日 15:06:43[QuJack](https://me.csdn.net/A784586)阅读数：690










# CNTK 101: Logistic Regression and ML Primer




参考：https://github.com/Microsoft/CNTK/blob/v2.0.rc2/Tutorials/CNTK_101_LogisticRegression.ipynb



官网是CNTK_101_LogisticRegression.ipynb文件，一种交互谁笔记本展示的。访问官网上述网址即可访问，也可以访问本地自己下载的Tutorials文件夹下，具体使用如下：

Jupyter Notebook的使用

Jupyter Notebook（此前被称为 IPython notebook）是一个交互式笔记本，支持运行 40 多种编程语言。

如何打开自己下载的demo，Tutorials文件夹下的** .ipnb**
 文件？

ipython从版本4.0以后就把notebook从[**Python**](http://lib.csdn.net/base/python)独立出来了，不仅支持ipython还支持R,matlab，并且改名为jupyter

sudo pip install jupyter

在本地包含 .ipnb
 文件夹下运行如下命令：

jupyter notebook




学习算法的五个步骤：

读数据；

数据预处理：数据归一化等问题；

建立模型；

学习模型；

验证模型；



CNTK开放了Python接口：

参看官网101例子，整合成文件形式的代码：

在cmd下运行：python  test101.py 



```python
#encoding=utf-8
from __future__ import print_function
import numpy as np
import sys
import os
from cntk import *
from IPython.display import Image

import matplotlib.pyplot as plt
# Select the right target device when this notebook is being tested:
if 'TEST_DEVICE' in os.environ:
    import cntk
    if os.environ['TEST_DEVICE'] == 'cpu':
        cntk.device.try_set_default_device(cntk.device.cpu())
    else:
        cntk.device.try_set_default_device(cntk.device.gpu(0))

# Ensure we always get the same amount of randomness
np.random.seed(0)

# 生成模拟数据
def generate_random_data_sample(sample_size, feature_dim, num_classes):
    Y = np.random.randint(size=(sample_size, 1), low=0, high=num_classes)
    X = (np.random.randn(sample_size, feature_dim)+3) * (Y+1)
    X = X.astype(np.float32)    
    class_ind = [Y==class_number for class_number in range(num_classes)]
    Y = np.asarray(np.hstack(class_ind), dtype=np.float32)
    return X, Y   

#画出特征和标签的数据
def show_data(features, labels):
	colors = ['r' if l == 0 else 'b' for l in labels[:,0]]

	plt.scatter(features[:,0], features[:,1], c=colors)
	plt.xlabel("Scaled age (in yrs)")
	plt.ylabel("Tumor size (in cm)")
	plt.show() 

#从输入到输出的计算；times函数是cntk包中的；
mydict = {"w":None,"b":None}
def linear_layer(input_var, output_dim):#(2,2)
    input_dim = input_var.shape[0]#返回input_var第一维形状,2
    weight_param = parameter(shape=(input_dim, output_dim))
    bias_param = parameter(shape=(output_dim))
    
    mydict['w'], mydict['b'] = weight_param, bias_param

    return times(input_var, weight_param) + bias_param

# Define a utility function to compute the moving average sum.
def moving_average(a, w=10):
    if len(a) < w: 
        return a[:]    
    return [val if idx < w else sum(a[(idx-w):idx])/w for idx, val in enumerate(a)]


# Defines a utility that prints the training progress
def print_training_progress(trainer, mb, frequency, verbose=1):
    training_loss, eval_error = "NA", "NA"
    if mb % frequency == 0:
        training_loss = trainer.previous_minibatch_loss_average
        eval_error = trainer.previous_minibatch_evaluation_average
        if verbose: 
            print ("Minibatch: {0}, Loss: {1:.4f}, Error: {2:.2f}".format(mb, training_loss, eval_error))
        
    return mb, training_loss, eval_error

#画图
def show_plot01(plotdata):
	plt.figure(1)
	plt.subplot(211)
	plt.plot(plotdata["batchsize"], plotdata["avgloss"], 'b--')
	plt.xlabel('Minibatch number')
	plt.ylabel('Loss')
	plt.title('Minibatch run vs. Training loss')

	plt.show()
#画图
def show_plot02(plotdata):
	plt.subplot(212)
	plt.plot(plotdata["batchsize"], plotdata["avgerror"], 'r--')
	plt.xlabel('Minibatch number')
	plt.ylabel('Label Prediction Error')
	plt.title('Minibatch run vs. Label Prediction Error')
	plt.show()		
def show_plot03(features,labels,bias_vector,weight_matrix):			
	colors = ['r' if l == 0 else 'b' for l in labels[:,0]]
	plt.scatter(features[:,0], features[:,1], c=colors)
	plt.plot([0, bias_vector[0]/weight_matrix[0][1]], 
         [ bias_vector[1]/weight_matrix[0][0], 0], c = 'g', lw = 3)
	plt.xlabel("Scaled age (in yrs)")
	plt.ylabel("Tumor size (in cm)")
	plt.show()
	
if __name__=='__main__':
	#产生数据，展示产生的数据
	mysamplesize = 32
	input_dim = 2
	num_output_classes = 2
	#features, labels = generate_random_data_sample(mysamplesize, input_dim, num_output_classes)
	#show_data(features, labels)
	
	#设置训练模型
	feature = input(input_dim, np.float32)
	output_dim = num_output_classes
	label = input((num_output_classes), np.float32)
	z = linear_layer(feature, output_dim)
	loss = cross_entropy_with_softmax(z, label)
	eval_error = classification_error(z, label)
	
	learning_rate = 0.5
	lr_schedule = learning_rate_schedule(learning_rate, UnitType.minibatch) 
	learner = sgd(z.parameters, lr_schedule)
	trainer = Trainer(z, (loss, eval_error), [learner])
	
	#训练
	minibatch_size = 25
	num_samples_to_train = 20000
	num_minibatches_to_train = int(num_samples_to_train  / minibatch_size)
	#参数的配置
	training_progress_output_freq = 50
	plotdata = {"batchsize":[], "loss":[], "error":[]}
	
	for i in range(0, num_minibatches_to_train):
		features, labels = generate_random_data_sample(minibatch_size, input_dim, num_output_classes)
		#训练
		trainer.train_minibatch({feature : features, label : labels})
		#打印训练中的参数
		batchsize, loss, error = print_training_progress(trainer, i, 
                                                     training_progress_output_freq, verbose=1)
		if not (loss == "NA" or error =="NA"):
			plotdata["batchsize"].append(batchsize)
			plotdata["loss"].append(loss)
			plotdata["error"].append(error)
			
	# Compute the moving average loss to smooth out the noise in SGD
	plotdata["avgloss"] = moving_average(plotdata["loss"])
	plotdata["avgerror"] = moving_average(plotdata["error"])	
	show_plot01(plotdata)
	show_plot02(plotdata)
	
	#测试
	test_minibatch_size = 25
	features, labels = generate_random_data_sample(test_minibatch_size, input_dim, num_output_classes)
	print(trainer.test_minibatch({feature : features, label : labels}))
	#查看预测
	out = softmax(z)
	result = out.eval({feature : features})
	print("Label    :", [np.argmax(label) for label in labels])
	print("Predicted:", [np.argmax(result[i,:]) for i in range(len(result))])
	#打印模型训练得到的参数
	print(mydict['b'].value)
	bias_vector   = mydict['b'].value
	weight_matrix = mydict['w'].value
	#可视化测试的数据预测
	show_plot03(features,labels,bias_vector,weight_matrix)
```













