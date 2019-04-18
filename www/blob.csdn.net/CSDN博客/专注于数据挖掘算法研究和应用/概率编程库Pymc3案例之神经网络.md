# 概率编程库Pymc3案例之神经网络 - 专注于数据挖掘算法研究和应用 - CSDN博客





2019年04月04日 12:21:35[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：23
个人分类：[Algorithm](https://blog.csdn.net/fjssharpsword/article/category/6309933)









参考：[https://docs.pymc.io/getting_started.html](https://docs.pymc.io/getting_started.html)

[https://twiecki.io/blog/2016/06/01/bayesian-deep-learning/](https://twiecki.io/blog/2016/06/01/bayesian-deep-learning/)

1、生成非线性可分的二分类数据

```python
%matplotlib inline
import theano
import pymc3 as pm
import sklearn
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from warnings import filterwarnings
filterwarnings('ignore')
sns.set_style('white')
from sklearn import datasets
from sklearn.preprocessing import scale
from sklearn.cross_validation import train_test_split
from sklearn.datasets import make_moons
```

```python
X, Y = make_moons(noise=0.2, random_state=0, n_samples=1000)
X = scale(X)
X = X.astype(float)
Y = Y.astype(float)
X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=.5)
fig, ax = plt.subplots(figsize=(12, 8))
ax.scatter(X[Y==0, 0], X[Y==0, 1], label='Class 0')
ax.scatter(X[Y==1, 0], X[Y==1, 1], color='r', label='Class 1')
sns.despine(); ax.legend()
ax.set(xlabel='X', ylabel='Y', title='Toy binary classification data set');
```

![](https://img-blog.csdnimg.cn/20190404084809927.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_16,color_FFFFFF,t_70)

2、定义贝叶斯神经网络模型

构建2 hidden layers with 5 neurons，权值用正态先验分布约束。这里表示的不确定性用先验约束，而预测的不确定性用Bernoulli分布。

```python
def construct_nn(ann_input, ann_output):
    n_hidden = 5
    
    # Initialize random weights between each layer
    init_1 = np.random.randn(X.shape[1], n_hidden).astype(float)
    init_2 = np.random.randn(n_hidden, n_hidden).astype(float)
    init_out = np.random.randn(n_hidden).astype(float)
        
    with pm.Model() as neural_network:
        # Weights from input to hidden layer
        weights_in_1 = pm.Normal('w_in_1', 0, sd=1, 
                                 shape=(X.shape[1], n_hidden), 
                                 testval=init_1)
        
        # Weights from 1st to 2nd layer
        weights_1_2 = pm.Normal('w_1_2', 0, sd=1, 
                                shape=(n_hidden, n_hidden), 
                                testval=init_2)
        
        # Weights from hidden layer to output
        weights_2_out = pm.Normal('w_2_out', 0, sd=1, 
                                  shape=(n_hidden,), 
                                  testval=init_out)
        
        # Build neural-network using tanh activation function
        act_1 = pm.math.tanh(pm.math.dot(ann_input,weights_in_1))
        act_2 = pm.math.tanh(pm.math.dot(act_1, weights_1_2))
        act_out = pm.math.sigmoid(pm.math.dot(act_2, weights_2_out))
        
        # Binary classification -> Bernoulli likelihood
        out = pm.Bernoulli('out', 
                           act_out,
                           observed=ann_output,
                           total_size=Y_train.shape[0] # IMPORTANT for minibatches
                          )
    return neural_network

# Trick: Turn inputs and outputs into shared variables. 
# It's still the same thing, but we can later change the values of the shared variable 
# (to switch in the test-data later) and pymc3 will just use the new data. 
# Kind-of like a pointer we can redirect.
# For more info, see: http://deeplearning.net/software/theano/library/compile/shared.html
ann_input = theano.shared(X_train)
ann_output = theano.shared(Y_train)
neural_network = construct_nn(ann_input, ann_output)
```

3、变分推断最优化

模型求解可以用MCMC采样法，如NUTS，但面对深度网络时，变分推断更快。

we will use [ADVI](https://arxiv.org/abs/1603.00788) variational inference algorithm which was recently added to `PyMC3`, and updated to use the operator variational inference (OPVI) framework. This is much faster and will scale better. Note, that this is a mean-field approximation so we ignore correlations in the posterior.

we can very quickly draw samples from the variational approximation using the `sample` method (this is just sampling from Normal distributions, so not at all the same like MCMC)

lets predict on the hold-out set using a posterior predictive check (PPC).

```python
from pymc3.theanof import set_tt_rng, MRG_RandomStreams
set_tt_rng(MRG_RandomStreams(42))
%time

with neural_network:
    inference = pm.ADVI()
    approx = pm.fit(n=50000, method=inference)
```

```python
CPU times: user 4 µs, sys: 1e+03 ns, total: 5 µs
Wall time: 13.4 µs
Average Loss = 128.96: 100%|██████████| 50000/50000 [00:43<00:00, 1138.57it/s]
Finished [100%]: Average Loss = 129
```

```python
trace = approx.sample(draws=5000)
plt.plot(-inference.hist)
plt.ylabel('ELBO')
plt.xlabel('iteration');
```

![](https://img-blog.csdnimg.cn/20190404122053184.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_16,color_FFFFFF,t_70)

```python
# Replace arrays our NN references with the test data
ann_input.set_value(X_test)
ann_output.set_value(Y_test)

with neural_network:
    ppc = pm.sample_ppc(trace, samples=500, progressbar=False)

# Use probability of > 0.5 to assume prediction of class 1
pred = ppc['out'].mean(axis=0) > 0.5
fig, ax = plt.subplots()
ax.scatter(X_test[pred==0, 0], X_test[pred==0, 1])
ax.scatter(X_test[pred==1, 0], X_test[pred==1, 1], color='r')
sns.despine()
ax.set(title='Predicted labels in testing set', xlabel='X', ylabel='Y');
print('Accuracy = {}%'.format((Y_test == pred).mean() * 100))
```

![](https://img-blog.csdnimg.cn/20190404122129139.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_16,color_FFFFFF,t_70)



