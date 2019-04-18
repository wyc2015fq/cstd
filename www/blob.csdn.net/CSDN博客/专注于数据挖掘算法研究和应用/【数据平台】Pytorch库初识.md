# 【数据平台】Pytorch库初识 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年12月20日 11:46:25[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：562
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









PyTorch是使用GPU和CPU优化的深度学习张量库。




1、安装，参考官网：http://pytorch.org/


`conda install pytorch torchvision -c pytorch`

2、认识，参考：

https://github.com/yunjey/pytorch-tutorial

https://github.com/jcjohnson/pytorch-examples

http://pytorch-cn.readthedocs.io/zh/latest/




3、demo：



```java
# Code in file tensor/two_layer_net_tensor.py
import torch

dtype = torch.FloatTensor
# dtype = torch.cuda.FloatTensor # Uncomment this to run on GPU

# N is batch size; D_in is input dimension;
# H is hidden dimension; D_out is output dimension.
N, D_in, H, D_out = 64, 1000, 100, 10

# Create random input and output data
x = torch.randn(N, D_in).type(dtype)
y = torch.randn(N, D_out).type(dtype)

# Randomly initialize weights
w1 = torch.randn(D_in, H).type(dtype)
w2 = torch.randn(H, D_out).type(dtype)

learning_rate = 1e-6
for t in range(500):
    # Forward pass: compute predicted y
    h = x.mm(w1)
    h_relu = h.clamp(min=0)
    y_pred = h_relu.mm(w2)

    # Compute and print loss
    loss = (y_pred - y).pow(2).sum()
    print(t, loss)

    # Backprop to compute gradients of w1 and w2 with respect to loss
    grad_y_pred = 2.0 * (y_pred - y)
    grad_w2 = h_relu.t().mm(grad_y_pred)
    grad_h_relu = grad_y_pred.mm(w2.t())
    grad_h = grad_h_relu.clone()
    grad_h[h < 0] = 0
    grad_w1 = x.t().mm(grad_h)

    # Update weights using gradient descent
    w1 -= learning_rate * grad_w1
    w2 -= learning_rate * grad_w2
```
结果如下：

![](https://img-blog.csdn.net/20171220114858040)




而同样np跑出来的结果是：

![](https://img-blog.csdn.net/20171220114921374)

代码如下：



```python
# Code in file tensor/two_layer_net_numpy.py
import numpy as np

# N is batch size; D_in is input dimension;
# H is hidden dimension; D_out is output dimension.
N, D_in, H, D_out = 64, 1000, 100, 10

# Create random input and output data
x = np.random.randn(N, D_in)
y = np.random.randn(N, D_out)

# Randomly initialize weights
w1 = np.random.randn(D_in, H)
w2 = np.random.randn(H, D_out)

learning_rate = 1e-6
for t in range(500):
  # Forward pass: compute predicted y
  h = x.dot(w1)
  h_relu = np.maximum(h, 0)
  y_pred = h_relu.dot(w2)
  
  # Compute and print loss
  loss = np.square(y_pred - y).sum()
  print(t, loss)
  
  # Backprop to compute gradients of w1 and w2 with respect to loss
  grad_y_pred = 2.0 * (y_pred - y)
  grad_w2 = h_relu.T.dot(grad_y_pred)
  grad_h_relu = grad_y_pred.dot(w2.T)
  grad_h = grad_h_relu.copy()
  grad_h[h < 0] = 0
  grad_w1 = x.T.dot(grad_h)
 
  # Update weights
  w1 -= learning_rate * grad_w1
  w2 -= learning_rate * grad_w2
```


根据实际应用场景，后续可深入学习，重点是gpu了。



