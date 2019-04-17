# 【Python-ML】神经网络-深度学习库Keras - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月26日 17:27:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：309
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月26日
@author: Jason.F
@summary: pip install Keras
Keras 神经网络训练库，可使用GPU加速神经网络的训练，基于张量库Theano支持多维数组处理
'''
import numpy as np
import time
import os
import struct
import theano
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers.core import Dense
from keras.optimizers import SGD
       
def load_mnist(path,kind='train'):
    #load mnist data from path
    labels_path = os.path.join(path,'%s-labels.idx1-ubyte'%kind)
    images_path = os.path.join(path,'%s-images.idx3-ubyte'%kind)
    with open(labels_path,'rb') as lbpath:
        magic,n =struct.unpack('>II',lbpath.read(8))
        labels = np.fromfile(lbpath,dtype = np.uint8)
    with open(images_path,'rb') as imgpath:
        magic,num,rows,cols =struct.unpack('>IIII',imgpath.read(16))
        images = np.fromfile(imgpath,dtype = np.uint8).reshape(len(labels),784)#28X28像素
    return images,labels  
    
if __name__ == "__main__":   
    start = time.clock()  
    
    #导入数据集
    homedir = os.getcwd()#获取当前文件的路径
    X_train,y_train = load_mnist(homedir+'\\mnist', kind='train')
    print ('Rows:%d,columns:%d'%(X_train.shape[0],X_train.shape[1]))
    X_test,y_test = load_mnist(homedir+'\\mnist', kind='t10k')
    print ('Rows:%d,columns:%d'%(X_test.shape[0],X_test.shape[1]))
    #将MNIST图像的数组转换为32位浮点数格式
    theano.config.floatX='float32'
    X_train = X_train.astype(theano.config.floatX)
    X_test = X_test.astype(theano.config.floatX)
    #onehot编码
    print ('First 3 labels:',y_train[:3])
    y_train_ohe = np_utils.to_categorical(y_train)
    print('\nFirst 3 labels(one-hot):\n',y_train_ohe[:3])
    #实现神经网络，隐层使用双曲正切函数，输出层使用softmax函数
    np.random.seed(1)
    model = Sequential()
    model.add(Dense(input_dim=X_train.shape[1],output_dim=50,init='uniform',activation='tanh'))
    model.add(Dense(input_dim=50,output_dim=50,init='uniform',activation='tanh'))
    model.add(Dense(input_dim=50,output_dim=y_train_ohe.shape[1],init='uniform',activation='tanh'))
    sgd =SGD(lr=0.001,decay=1e-7,momentum=.9)
    model.compile(loss='categorical_crossentropy',optimizer=sgd)
    model.fit(X_train,y_train_ohe,nb_epoch=50,batch_size=300,verbose=1,validation_split=0.1)#show_accuracy=True
    y_train_pred = model.predict_classes(X_train,verbose=0)
    print ('First 3 predictions:',y_train_pred[:3])
    train_acc = np.sum(y_train==y_train_pred,axis=0)/float(X_train.shape[0])
    print ('Training accuracy:%.2f%%'%(train_acc*100))
    y_test_pred=model.predict_classes(X_test,verbose=0)
    test_acc = np.sum(y_test==y_test_pred,axis=0)/float(X_test.shape[0])
    print ('Test accuracy:%.2f%%'%(test_acc*100))
    
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```

设置：

windows，pip install keras后，到C:\Users\user\.keras目录下找到keras.json文件，默认设置如下图：

![](https://img-blog.csdn.net/20180126173419384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

修改backend为：

![](https://img-blog.csdn.net/20180126173557693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180126173536896?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

结果：



```
Training accuracy:13.63%
Test accuracy:13.45%
finish all in 13968.1836241
```
执行一次好长时间，准确率这么低要找下原因。







