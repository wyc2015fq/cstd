# 【Python-ML】神经网络-多层感知器 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月26日 16:11:10[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：377
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月26日
@author: Jason.F
@summary: 多层感知器实现
训练集：http://yann.lecun.com/exdb/mnist/
train-images-idx3-ubyte: training set images
train-labels-idx1-ubyte: training set labels
t10k-images-idx3-ubyte:  test set images
t10k-labels-idx1-ubyte:  test set labels 
'''
import pandas as pd
import numpy as np
import time
import sys
import os
import struct
from scipy.special import expit
import matplotlib.pyplot as plt

class NeuralNetMLP(object):
    def __init__(self,n_output,n_features,n_hidden=30,l1=0.0,l2=0.0,epochs=500,eta=0.001,alpha=0.0,decrease_const=0.0,shuffle=True,minibatches=1,random_state=None):
        np.random.seed(random_state)
        self.n_output=n_output #输出层数量
        self.n_features=n_features #输入层数量
        self.n_hidden=n_hidden #隐层数量
        self.w1,self.w2 = self._initial_weights() #初始化权值系数
        self.l1=l1 #l1正则化系数
        self.l2=l2 #l2正则化系数
        self.epochs=epochs #迭代次数
        self.eta=eta #学习速率
        self.alpha=alpha #动量学习进度的参数，在上一轮迭代基础上增加一个因子，用于加快权重更新的学习
        self.decrease_const=decrease_const #用于降低自适应学习速率n的常熟d,随着迭代次数的增加而递减颗更好地收敛
        self.shuffle=shuffle 
        self.minibatches=minibatches#训练批次
        
    def _ecode_labels(self,y,k):
        onehot = np.zeros((k,y.shape[0]))
        for idx ,val in enumerate(y):
            onehot[val,idx]=1.0
        return onehot
    
    def _initial_weights(self):
        w1 = np.random.uniform(-1.0,1.0,size=self.n_hidden*(self.n_features+1))
        w1 = w1.reshape(self.n_hidden,self.n_features+1)
        w2 = np.random.uniform(-1.0,1.0,size=self.n_output*(self.n_hidden+1))
        w2 = w2.reshape(self.n_output,self.n_hidden+1)
        return w1,w2
    
    def _sigmoid(self,z):
        #expit is equivalent to 1.0/(1.0+np.exp(-z))
        return expit(z)
    
    def _sigmoid_gradient(self,z):
        sg =self._sigmoid(z)
        return sg*(1-sg)
    
    def _add_bias_unit(self,X,how='column'):
        if how == 'column':#列
            X_new = np.ones((X.shape[0],X.shape[1]+1))
            X_new[:,1:]=X
        elif how =='row':#行
            X_new = np.ones((X.shape[0]+1,X.shape[1]))
            X_new[1:,:]=X
        else:
            raise AttributeError('`how` must be `column` or `row`')
        return X_new
    
    def _feedforwrd(self,X,w1,w2):
        a1=self._add_bias_unit(X, how='column')
        z2=w1.dot(a1.T)
        a2=self._sigmoid(z2)
        a2=self._add_bias_unit(a2, how='row')
        z3=w2.dot(a2)
        a3=self._sigmoid(z3)
        return a1,z2,a2,z3,a3
    
    def _L2_reg(self,lambda_,w1,w2):
        return (lambda_/2.0)*(np.sum(w1[:,1:]**2)+np.sum(w2[:,1:]**2))
    
    def _L1_reg(self,lambda_,w1,w2):
        return (lambda_/2.0)*(np.abs(w1[:,1:]).sum()+np.abs(w2[:,1:]).sum())
    
    def _get_cost(self,y_enc,output,w1,w2):
        term1 = -y_enc *(np.log(output))
        term2 = (1-y_enc) * np.log(1-output)
        cost = np.sum(term1-term2)
        L1_term =self._L1_reg(self.l1, w1, w2)
        L2_term =self._L2_reg(self.l2, w1, w2)
        cost =cost + L1_term +L2_term
        return cost
    
    def _get_gradient(self,a1,a2,a3,z2,y_enc,w1,w2):
        #backpropagation
        sigma3 = a3-y_enc
        z2 = self._add_bias_unit(z2, how='row')
        sigma2 = w2.T.dot(sigma3) * self._sigmoid_gradient(z2)
        sigma2 = sigma2[1:,:]
        grad1 = sigma2.dot(a1)
        grad2 = sigma3.dot(a2.T)
        #regularize
        grad1[:,1:] += (w1[:,1:] * (self.l1+self.l2))
        grad2[:,1:] += (w2[:,1:] * (self.l1+self.l2))
        return grad1,grad2
    
    def predict(self,X):
        a1,z2,a2,z3,a3 = self._feedforwrd(X, self.w1, self.w2)
        y_pred = np.argmax(z3,axis=0)
        return y_pred
    
    def fit(self,X,y,print_progress=False):
        self.cost_=[]
        X_data,y_data =X.copy(),y.copy()
        y_enc = self._ecode_labels(y, self.n_output)
        delta_w1_prev =np.zeros(self.w1.shape)
        delta_w2_prev =np.zeros(self.w2.shape)
        for i in range(self.epochs):
            #adaptive learning rate
            self.eta /= (1+self.decrease_const*i)
            if print_progress:
                sys.stderr.write('\rEpoch:%d/%d'%(i+1,self.epochs))
                sys.stderr.flush()
            if self.shuffle:
                idx = np.random.permutation(y_data.shape[0])
                X_data,y_data = X_data[idx],y_data[idx]
            mini = np.array_split(range(y_data.shape[0]),self.minibatches)
            for idx in mini:
                #feedback
                a1,z2,a2,z3,a3 = self._feedforwrd(X[idx], self.w1, self.w2)
                cost = self._get_cost(y_enc=y_enc[:,idx], output=a3, w1=self.w1, w2=self.w2)
                self.cost_.append(cost)
                #compute gradient via backpropagation
                grad1,grad2 = self._get_gradient(a1=a1,a2=a2,a3=a3,z2=z2,y_enc=y_enc[:,idx],w1=self.w1,w2=self.w2)
                #update weights
                delta_w1,delta_w2 = self.eta *grad1,self.eta*grad2
                self.w1 -= (delta_w1 +(self.alpha * delta_w1_prev))
                self.w2 -= (delta_w2 +(self.alpha * delta_w2_prev))
                delta_w1_prev,delta_w2_prev=delta_w1,delta_w2
        return self          

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
    '''
    #将特征矩阵的784像素向量还原成18X28图像
    fig,ax = plt.subplots(nrows=2,ncols=5,sharex=True,sharey=True)
    ax=ax.flatten()
    for i in range(10):
        img = X_train[y_train==i][0].reshape(28,28)
        ax[i].imshow(img,cmap='Greys',interpolation='nearest')
    ax[0].set_xticks([])
    ax[0].set_yticks([])
    plt.tight_layout()
    plt.show()
    #绘制相同数字的多个示例
    fig,ax = plt.subplots(nrows=5,ncols=5,sharex=True,sharey=True)
    ax=ax.flatten()
    for i in range(25):
        img = X_train[y_train==7][i].reshape(28,28)
        ax[i].imshow(img,cmap='Greys',interpolation='nearest')
    ax[0].set_xticks([])
    ax[0].set_yticks([])
    plt.tight_layout()
    plt.show()
    #将数据存储为CSV格式
    np.savetxt('train_img.csv',X_train,fmt='%i',delimiter=',')#指定存储数据类型为整型，分隔符为,
    np.savetxt('train_labels.csv',y_train,fmt='%i',delimiter=',')
    np.savetxt('test_img.csv',X_test,fmt='%i',delimiter=',')
    np.savetxt('test_labels.csv',y_test,fmt='%i',delimiter=',')
    #从csv加载数据
    X_train = np.genfromtxt('train_img.csv',dtype=int,delimiter=',')
    y_train = np.genfromtxt('train_labels.csv',dtype=int,delimiter=',')
    X_test = np.genfromtxt('test_img.csv',dtype=int,delimiter=',')
    y_test = np.genfromtxt('test_labels.csv',dtype=int,delimiter=',')
    '''
    #创建感知器模型
    nn = NeuralNetMLP(n_output=10,n_features=X_train.shape[1],n_hidden=50,l2=0.1,l1=0.0,epochs=1000,eta=0.001,alpha=0.001,decrease_const=0.00001,shuffle=True,minibatches=50,random_state=1)
    nn.fit(X_train,y_train,print_progress=True)
    #绘制代价函数图像，按批次
    batches = np.array_split(range(len(nn.cost_)),1000)
    cost_ary = np.array(nn.cost_)
    cost_avgs = [np.mean(cost_ary[i]) for i in batches]
    plt.plot(range(len(cost_avgs)),cost_avgs,color='red')
    plt.ylim([0,2000])
    plt.ylabel('Cost')
    plt.xlabel('Epochs')
    plt.tight_layout()
    plt.show()
    #模型性能评估
    y_train_pred = nn.predict(X_train)
    acc = np.sum(y_train ==y_train_pred,axis=0)/float(X_train.shape[0])
    print ('Training accurcy:%.2f%%'%(acc*100))
    y_test_pred =nn.predict(X_test)
    acc = np.sum(y_test==y_test_pred,axis=0)/float(X_test.shape[0])
    print ('Test accurcy:%.2f%%'%(acc*100))
    #观察识别错误的图像
    miscl_img = X_test[y_test!=y_test_pred][:25]
    correct_lab = y_test[y_test!=y_test_pred][:25]
    miscl_lab = y_test_pred[y_test!=y_test_pred][:25]
    fig,ax = plt.subplots(nrows=5,ncols=5,sharex=True,sharey=True)
    ax=ax.flatten()
    for i in range(25):
        img = miscl_img[i].reshape(28,28)
        ax[i].imshow(img,cmap='Greys',interpolation='nearest')
        ax[i].set_title('%d) t:%d p:%d'%(i+1,correct_lab[i],miscl_lab[i]))
    ax[0].set_xticks([])
    ax[0].set_yticks([])
    plt.tight_layout()
    plt.show()
    
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```


结果：



```
Rows:60000,columns:784
Rows:10000,columns:784
Training accurcy:97.58%
Test accurcy:95.90%
finish all in 1398.59068407
```
![](https://img-blog.csdn.net/20180126161054482?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20180126161101974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





