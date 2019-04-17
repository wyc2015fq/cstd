# CTC学习笔记（五） eesen训练源码 - xmdxcsj的专栏 - CSDN博客





2016年11月24日 20:58:58[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3919








essen源码参考[https://github.com/yajiemiao/eesen](https://github.com/yajiemiao/eesen)，这里简单说一下涉及到训练前后向的核心算法源码实现。 

以单句训练为准（多句并行类似），用到的变量
|变量|含义|
|----|----|
|phones_num|最后一层输出节点个数，对应于|phones|+1|
|labels_num|一句话对应的标注扩展blank以后的个数，比如”123”扩展为”b1b2b3b”|
|frames_num|一句话对应的总的帧数，对应于时间t|
|$y_k^t$|最后一层输出|
|$a_k^t$|softmax层的输入|

## CTC error

```cpp
ctc.Eval(net_out, targets, &obj_diff);
```

涉及到的变量的维度：
|变量|维度|
|----|----|
|net_out|frames_num*phones_num|
|alpha/beta|frames_num*labes_num|
|ctc_error|frames_num*phones_num|

本来可以使用最终的公式求出对$a_k^t$的error，代码中却分成了两部求解，可能逻辑上能体现出error反向传播的过程，但是实际感觉没有必要。

### 计算关于$y_k^t$的error

```cpp
ctc_err_.ComputeCtcError(alpha_, beta_, net_out, label_expand_, pzx);
```

参考[1]给出的公式(15)

### 计算关于$u_k^t$的error

```cpp
ctc_err_.MulElements(net_out);
CuVector<BaseFloat> row_sum(num_frames, kSetZero);
row_sum.AddColSumMat(1.0, ctc_err_, 0.0);
CuMatrix<BaseFloat> net_out_tmp(net_out);
net_out_tmp.MulRowsVec(row_sum);
diff->CopyFromMat(ctc_err_);
diff->AddMat(-1.0, net_out_tmp);
```

主要是$y_k^t$对$a_k^t$进行求导，推导参考[前面的博客](http://blog.csdn.net/xmdxcsj/article/details/51763886)，结论如下： 


$\frac{\partial L}{\partial a_k^t}=\sum_{k'}\frac{\partial L}{\partial y_{k'}^t}y_{k'}^t\delta_{kk'}-\sum_{k'}\frac{\partial L}{\partial y_{k'}^t}y_{k'}^ty_k^t$


$=\frac{\partial L}{\partial y_{k}^t}y_{k}^t-\sum_{k'}\frac{\partial L}{\partial y_{k'}^t}y_{k'}^ty_k^t$

注意上式最后一项有一个求和的过程，即将t时刻对应的$y_k^t$的所有节点的error累加。
## 沿网络反向传播error
|变量|含义|
|----|----|
|x|每一层的输入|
|y|每一层的输出|
|d_x|关于x的error|
|d_y|关于y的error|
|dim_in|输入维度|
|dim_out|输出维度|
|W|每一层对应的参数矩阵|

error依次经过affine-trans-layer和多层lstm-layer，每一层有两个目的： 

- 求d_x: 将error传递到每一层的输入，以往后继续传播 

- 求$\Delta W$: 计算当前层的参数的error，以根据error更新参数
### affine layer
|变量|维度|
|----|----|
|x/d_x|frames_num*dim_in|
|y/d_y|frames_num*dim_out|
|W|dim_out*dim_in|

#### 前向



$y=x*W^T$

#### 后向



$\Delta W(t)=d\_y^T*x+momentum*\Delta W(t-1)$

这里参数更新有一个求和的过程，把所有时刻对应的$\Delta W$进行累加，相当于把所有时间的error数据进行了求和作为最终的error。

### lstm layer

![这里写图片描述](https://img-blog.csdn.net/20161124205816811)

参考[2]，eesen采用的lstm单元如上图，但是代码中变量的含义和论文中不一致。
### 前向



$i_t=\delta(x_tW_{ix}^T+m_{t-1}W_{im}^T+c_{t-1}W_{ic}^T+b_i)$


$f_t=\delta(x_tW_{fx}^T+m_{t-1}W_{fm}^T+c_{t-1}W_{fc}^T+b_i)$


$g_t=\delta(x_tW_{cx}^T+m_{t-1}W_{cm}^T+b_c)$


$c_t=f_t\odot c_{t-1}+i_t\odot g_t$


$o_t=\delta(x_tW_{ox}^T+m_{t-1}W_{om}^T+c_{t}W_{oc}^T+b_o)$


$h_t=\phi (c_t)$


$m_t=o_t\odot h_t$

有两方面的并行 

- gifo合并成一个矩阵 

- 批量计算输入x（不依赖于t），然后再分帧计算其他变量
### 后向



$D_i=\frac{\partial L}{\partial (x_tW_{ix}^T+m_{t-1}W_{im}^T+c_{t-1}W_{ic}^T+b_i)}$


$D_f=\frac{\partial L}{\partial (x_tW_{fx}^T+m_{t-1}W_{fm}^T+c_{t-1}W_{fc}^T+b_i)}$


$D_g=\frac{\partial L}{\partial (x_tW_{cx}^T+m_{t-1}W_{cm}^T+b_c)}$


$D_o=\frac{\partial L}{\partial (x_tW_{ox}^T+m_{t-1}W_{om}^T+c_{t}W_{oc}^T+b_o)}$


$D_c=\frac{\partial L}{\partial (f_t\odot c_{t-1}+i_t\odot g_t)}$

有两个注意点 

- $m_t$的error除了来自于t时刻的error，还有来至于t+1时刻的$D_i$/$D_f$/$D_o$/$D_g$

- $c_t$的error除了来自于t时刻的error，还有来自于t+1时刻的$D_i$/$D_f$/$D_c$
## 参考文献

[1].[Connectionist Temporal Classification: Labelling Unsegmented Sequence Data with Recurrent Neural Networks](http://www.cs.toronto.edu/~graves/icml_2006.pdf)

[2].Essen: End-to-End Speech Recognition Using Deep Rnn Models and WFST-Based Decoding








