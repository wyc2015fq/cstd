# lstm(三) 模型压缩lstmp - xmdxcsj的专栏 - CSDN博客





2016年11月24日 21:05:19[xmucas](https://me.csdn.net/xmdxcsj)阅读数：4760








## lstmp结构

对于传统的lstm而言 


$i_t=\delta(W_{ix}x_t+W_{im}m_{t-1}+W_{ic}c_{t-1}+b_i)$


$f_t=\delta(W_{fx}x_t+W_{fm}m_{t-1}+W_{fc}c_{t-1}+b_i)$


$c_t=f_t\odot c_{t-1}+i_t\odot g(W_{cx}x_t+W_{cm}m_{t-1}+b_c)$


$o_t=\delta(W_{ox}x_t+W_{om}m_{t-1}+W_{oc}c_{t}+b_o)$


$m_t=o_t\odot h(c_t)$


$y_t=\phi (W_{ym}m_t+b_y)$

假设一层中的cell个数为$n_c$，输入维度为$n_i$，输出维度为$n_o$，那么对应的参数量为： 


$W=n_c*n_c*4+n_i*n_c*4+n_c*n_o+n_c*3$
![Alt text](./1479303077237.png)

lstmp是lstm with recurrent projection layer的简称，在原有lstm基础之上增加了一个projection layer，并将这个layer连接到lstm的输入。此时的网络结构变为 


$i_t=\delta(W_{ix}x_t+W_{ir}r_{t-1}+W_{ic}c_{t-1}+b_i)$


$f_t=\delta(W_{fx}x_t+W_{fr}r_{t-1}+W_{fc}c_{t-1}+b_i)$


$c_t=f_t\odot c_{t-1}+i_t\odot g(W_{cx}x_t+W_{cr}r_{t-1}+b_c)$


$o_t=\delta(W_{ox}x_t+W_{or}r_{t-1}+W_{oc}c_{t}+b_o)$


$m_t=o_t\odot h(c_t)$


$r_t=W_{rm}m_t$


$y_t=\phi (W_{yr}r_t+b_y)$

projection layer的维度设为$n_r$，那么总的参数量将会变为： 


$W=n_c*n_r*4+n_i*n_c*4+n_r*n_o+n_c*n_r+n_c*3$

通过设置$n_r$的大小，可以缩减总的参数量。
## lstm压缩

### 直接训练lstmp网络结构

为了减少矩阵的参数量，重点优化，以$W_{ix}$和$W_{im}$为例，相关参数量的变化如下： 
![这里写图片描述](https://img-blog.csdn.net/20161124210309787)
### 对lstm的参数做SVD压缩

参考[3]，对已有的参数做压缩，主要两个矩阵：inter-layer矩阵$[W_{ix},W_{fx},W_{ox},W_{cx}]^T$和recurrent 矩阵$[W_{im},W_{fm},W_{om},W_{cm}]^T$

通过奇异值的设定将两个矩阵转化为三个小矩阵，其中一个小矩阵作为lstmp中projection layer的参数。

## 参考

[1].Long Short-Term Memory Based Recurrent Neural Network Architectures for Large Vocabulary Speech Recognition 

[2].long short-term memory recurrent neural network architectures for large scale acoustic modeling 

[3].ON THE COMPRESSION OF RECURRENT NEURAL NETWORKS WITH AN APPLICATION TO LVCSR ACOUSTIC MODELING FOR EMBEDDED SPEECH RECOGNITION





