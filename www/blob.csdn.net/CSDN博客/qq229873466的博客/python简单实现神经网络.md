# python简单实现神经网络 - qq229873466的博客 - CSDN博客

2017年05月13日 22:09:48[qq229873466](https://me.csdn.net/qq229873466)阅读数：368


初学python，参照matlab tool代码实现神经网络。[数据在这](http://download.csdn.net/detail/qq229873466/9841764)

```python
# -*- coding: utf-8 -*-
"""
Created on Fri May 12 15:05:00 2017

@author: li
"""
import scipy.io as sio
import numpy as np

def zscore(data):
    mu = np.mean(data, axis=0)
    sigma = np.std(data, axis=0, ddof=1)
    sigma = np.array([sigma[i] if sigma[i]!=0 else 1 for i in range(sigma.shape[0]) ])
    x = (data - mu) / sigma
    return x, mu, sigma

def normalize(data, mu, sigma):
    return (data - mu) / sigma
    
def sigm(x):
    return 1.0 / (1 + np.exp(-x))
    
def checkgrad(nn,x,y):
    EPS = 1e-6
    er = 1e-7
    n = nn['n'] - 1
    for l in range(n):    
        for i in range(nn['W'][l].shape[0]):
            for j in range(nn['W'][l].shape[1]):
                nn_m = nn
                nn_p = nn
                nn_m['W'][l][i,j] = nn['W'][l][i,j] - EPS
                nn_p['W'][l][i,j] = nn['W'][l][i,j] + EPS
                nn_m = forward(nn_m,x,y)
                nn_p = forward(nn_p,x,y)
                dW = (nn_p['L'] -  nn_m['L']) / (2 * EPS)
                e = abs(dW - nn['dW'][l][i,j])
                print('l=%d, i=%d, j=%d' % (l,i,j))
                assert e < er
    
def setup(size):
    LEN = len(size)
    nn = {
        'size': size,
        'n': LEN,
        'learningRate': 2,
        'momentum': 0.5,
        'scaling_learningRate': 1,
        'W': [[]] * (LEN-1),
        'vW': [[]] * (LEN-1),
        'dW': [[]] * (LEN-1),
        'L': 0,
        'a': [[]] * LEN,
        'e': [],
    }
    for i in range(1,nn['n']):
        nn['W'][i-1] = (np.random.random([nn['size'][i], nn['size'][i-1]+1]) - 0.5)\
                * 2 * 4 * np.sqrt(6.0 / (nn['size'][i] + nn['size'][i-1]))
        nn['vW'][i-1] = np.zeros(nn['W'][i-1].shape)
    return nn

def forward(nn, batch_x, batch_y):
   """
       1.activation:sigm | tanh | ReLU
       2.drop out
       3.sparsity penalty
       4.output:sigm | linear | softmax
   """
   m =  batch_x.shape[0]
   n = nn['n']-1
   
   nn['a'][0] = np.hstack((np.ones([m,1]), batch_x))
   for i in range(1, n):
       nn['a'][i] = sigm( np.dot(nn['a'][i-1], nn['W'][i-1].T) )
       nn['a'][i] = np.hstack(( np.ones([m,1]), nn['a'][i] ))
   nn['a'][n] = sigm( np.dot(nn['a'][n-1], nn['W'][n-1].T) )
   
   nn['e'] = batch_y - nn['a'][n]
   nn['L'] = 0.5 * np.sum(np.sum(nn['e']**2)) / m
   return nn
   
def backward(nn):
    n = nn['n']-1
    d = [[]] * nn['n']
    d[n] = -nn['e']
    for i in range(n-1,0,-1):
        d_act = nn['a'][i] * (1 - nn['a'][i])
        d[i] = np.dot(d[i+1], nn['W'][i]) * d_act if i+1==n else\
                np.dot(d[i+1][:,1::], nn['W'][i]) * d_act

    for i in range(n):
        nn['dW'][i] = np.dot(d[i+1].T, nn['a'][i]) / d[i+1].shape[0]\
                        if i+1==n else\
                      np.dot(d[i+1][:,1::].T, nn['a'][i]) / d[i+1].shape[0]  
    return nn
        
def upgrads(nn):
    """
        1.penalty
        2.momentum
        3.normal(W -= dW)
    """
    for i in range(nn['n']-1):  
        nn['vW'][i] = nn['momentum'] * nn['vW'][i] + nn['learningRate'] * nn['dW'][i]
        nn['W'][i] -= nn['vW'][i]
    return nn
    
def train(nn, train_x, train_y, opts):
    m = train_x.shape[0]
    numbatches = m / opts['batchsize']
    L = np.zeros([opts['numepochs']*numbatches, 1])
    n = 0
    for i in range(opts['numepochs']):
        kk = range(m)
        np.random.shuffle(kk)
        for l in range(numbatches):
            batch_x = train_x[kk[l * opts['batchsize']\
                        : (l+1) * opts['batchsize']], :]
            batch_y = train_y[kk[l * opts['batchsize']\
                        : (l+1) * opts['batchsize']], :]
            
            nn = forward(nn, batch_x, batch_y)
            nn = backward(nn)
            nn = upgrads(nn)
            
            L[n] = nn['L']
            n += 1
        nn['learningRate'] = nn['learningRate'] * nn['scaling_learningRate']        
    return nn,L
    
def test(nn, test_x, test_y):
    nn = forward(nn, test_x, np.zeros([test_x.shape[0], nn['size'][-1]]))
    labels = np.argmax(nn['a'][-1], axis=1)
    expected = np.argmax(test_y, axis=1)
    bad = []
    for i in range(expected.shape[0]):
        if labels[i]!=expected[i]:
            bad.append(i)
    er = np.array(bad).size*1.0 / test_x.shape[0]
    return er,bad
    
def main():
    data = sio.loadmat('mnist.mat')
    train_x, mu, sigma = zscore(data['train_x'])
    test_x = normalize(data['test_x'], mu, sigma)
    
    opts = {
        'numepochs':1, 
        'batchsize':100, 
    }
    nn = setup([784, 100, 10])
    nn,L = train(nn, train_x, data['train_y'], opts)
    er, bad = test(nn,  test_x, data['test_y'])
#    checkgrad(nn,train_x,data['train_y'])
    print('error rate: %f' % (er))

if __name__ == '__main__':
    main()
```


