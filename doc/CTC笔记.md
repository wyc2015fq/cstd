# CTC笔记

# 一、简介



## 背景

Connectionist temporal classification简称CTC，翻译不太清楚，可以理解为基于神经网络的时序类分类。其中classification比较好理解，表示分类问题；temporal可以理解为时序类问题，比如语音识别的一帧数据，很难给出一个label，但是几十帧数据就容易判断出对应的发音label，这个词也给出CTC最核心的意义；connectionist可以理解为神经网络中的连接。 
语音识别声学模型的训练属于监督学习，需要知道每一帧对应的label才能进行有效的训练，在训练的数据准备阶段必须要对语音进行强制对齐。 
CTC的引入可以放宽了这种一一对应的限制要求，只需要一个输入序列和一个输出序列即可以训练。有两点好处：不需要对数据对齐和一一标注；CTC直接输出序列预测的概率，不需要外部的后处理。 
![这里写图片描述](https://img-blog.csdn.net/20160626184846419)
如上图，传统的Framewise训练需要进行语音和音素发音的对齐，比如“s”对应的一整段语音的标注都是s；而CTC引入了blank（该帧没有预测值），“s”对应的一整段语音中只有一个spike（尖峰）被认为是s，其他的认为是blank。对于一段语音，CTC最后的输出是spike的序列，不关心每一个音素对应的时间长度。

## 输出

语音识别中的DNN训练，每一帧都有相应的状态标记，比如有5帧输入x1,x2,x3,x4,x5，对应的标注分别是状态a1,a1,a1,a2,a2。 
CTC的不同之处在于输出状态引入了一个blank，输出和label满足如下的等价关系： 
$$
F(a-ab-)=F(-aa--abb)=aab
$$
多个输出序列可以映射到一个输出。

# 二、训练和公式推导


## 整体思路

训练流程和传统的神经网络类似，构建loss function，然后根据BP算法进行训练，不同之处在于传统的神经网络的训练准则是针对每帧数据，即每帧数据的训练误差最小，而CTC的训练准则是基于序列（比如语音识别的一整句话）的，比如最大化p(z|x)p(z|x)，序列化的概率求解比较复杂，因为一个输出序列可以对应很多的路径，所有引入前后向算法来简化计算。

## 前期准备

1. 输入x,长度为T
2. 输出集合 
   A表示正常的输出 
   $A'=A \bigcup \{blank\}$表示输出全集 
   $A'^T$表示输入x对应的输出元素集合
3. 输出序列 
   $π$表示输出路径 
   $l$表示输出label序列 
   $F$表示路径到label序列的映射关系
4. 概率 
   $y_k^t$表示时间t输出k的概率 
   $p(\pi|x)=\displaystyle\prod_{t=1}^{T}y_{\pi_t}^{t}$表示基于输入x的输出ππ路径的概率 
   $p(l|x)=\displaystyle\sum_{\pi \in \mathcal{F}^{-1}(l)}p(\pi|x)$表示输出label序列的概率是多条路径的概率和。

## 前后向算法

![这里写图片描述](https://img-blog.csdn.net/20160626185316096)
考虑到计算$p(l|x)​$需要计算很多条路径的概率，随着输入长度呈指数化增加，可以引入类似于HMM的前后向算法来计算该概率值。 
为了引入blank节点，在label首尾以及中间插入blank节点，如果label序列原来的长度为U，那么现在变为U’=2U+1。

### 前向

前向变量为$α(t,u)$，表示t时刻在节点u的前向概率值，其中$u∈[1,2U+1]$. 
初始化值如下： 
$$
\alpha(1,1)=y_b^1\\
\alpha(1,2)=y_{l_1}^1\\
\alpha(1,u)=0, \forall u>2
$$
递推关系：

$$
\alpha(t,u)=y_{l'_{u}}^t\displaystyle\sum_{i=f(u)}^{u}\alpha(t-1,i)
$$
其中

$$
f(u) =
  \begin{cases}
    u-1       & \quad \text{if } l'_u=blank \text{ or }l'_{u-2}=l'_u\\
    u-2      & \quad \text{otherwise}\\
  \end{cases}
$$
注：如果l表示{c,a,t}，那么l’表示为{b,c,b,a,b,t,b}，所以原来在l中的下标u为2，在l’中的下标u变为4。

$$
\alpha(t,u)=0  \forall u< U'-2(T-t)-1
$$
对应于上图中的右上角部分，因为时间的限制，有些节点不可能到达最后的终止节点。

根据上图，很容易理解前向的递推关系。



### 后向

初始化值： 

$$
\beta(T,U')=1\\
\beta(T,U'-1)=1\\
\beta(T,u)=0, \forall u<U'-2\\
\alpha(1,u)=0, \forall u>2
$$
递推关系：

$$
\beta(t,u)=\displaystyle\sum_{i=u}^{g(u)}\beta(t+1,i)y_{l'_{i}}^{t+1}
$$
其中

$$
g(u) =
  \begin{cases}
    u+1       & \quad \text{if } l'_u=blank \text{ or }l'_{u+2}=l'_u\\
    u+2      & \quad \text{otherwise}\\
  \end{cases}
$$


### 取log

概率计算在log计算，避免underflow，其中log加可以通过以下形式转化： 

$$
ln(a+b)=lna+ln(1+e^{lnb-lna})
$$


## 训练

### loss function

CTC的loss function使用最大似然： 

$$
L(S)=\displaystyle\sum_{(x,z)\in S}L(x,z)\\
L(x,z)=-lnp(z|x)
$$
根据前后向变量，可以求得：

$$
p(z|x)=\displaystyle\sum_{u=1}^{|z'|}\alpha(t,u)\beta(t,u)
$$
$|z'|$表示z对应的label长度的U’，$α(t,u)β(t,u)$表示t时刻经过节点u的所有路径的概率和。

$$
L(x,z)=-ln\displaystyle\sum_{u=1}^{|z'|}\alpha(t,u)\beta(t,u)
$$


### bp训练

$y_k^t$表示t时刻输出k的概率 
$a_k^t$表示t时刻对应输出节点k在做softmax转换之前的值 
$$
\frac{\partial L(x,z)}{\partial y_k^t}=-\frac{1}{p(z|x)}\frac{\partial p(z|x)}{\partial y_k^t}
$$
只需要考虑t时刻经过k节点的路径即可

$$
\frac{\partial p(z|x)}{\partial y_k^t}=\displaystyle\sum_{u\in B(z,k)}\frac{\partial \alpha(t,u)\beta(t,u)}{\partial y_k^t}
$$
其中$B(z,k)$表示节点为k的集合

考虑到 

$$
\alpha(t,u)\beta(t,u)=\displaystyle\sum_{\pi \in X(t,u)}\displaystyle\prod_{t=1}^{T}y_{\pi_t}^{t}
$$
其中$X(t,u)$表示所有在t时刻经过节点u的路径。

所以

$$
\frac{\partial p(z|x)}{\partial y_k^t}=\displaystyle\sum_{u\in B(z,k)}\frac{\alpha(t,u)\beta(t,u)}{y_k^t}
$$
可以到损失函数对$y_k^t$偏导数
$$
\frac{\partial L(x,z)}{\partial y_k^t}=-\frac{1}{p(z|x)y_k^t}\displaystyle\sum_{u\in B(z,k)}{\alpha(t,u)\beta(t,u)}
$$
同时可以得到损失函数对于$a_k^t$偏导数

$$
\frac{\partial L(x,z)}{\partial a_k^t}=y_k^t-\frac{1}{p(z|x)}\displaystyle\sum_{u\in B(z,k)}{\alpha(t,u)\beta(t,u)}
$$
推导参考：

 后续可以使用

BPTT算法

得到损失函数对神经网络参数的偏导。



# 三、解码

解码是对于输入序列x找出概率最大的输出序列l，而不是概率最大的一条输出路径，因为输出路径和输出序列是多对一关系。 
$$
l^*=argmax \{p(l|x)\}\{p(l|G)\}
$$

## best path decoding

最优路径找出每一帧输出的最大概率组成的输出序列即为最后的解码结果，这种方式会引入问题。 
![这里写图片描述](https://img-blog.csdn.net/20160702004318551)
对于上图，这种方法解码出来的结果是blank，但是A的概率反而更高。

## constrained decoding

对于语音识别，可以引入语言模型等grammar限制，求解问题变为如下形式： 



l∗=argmax{p(l|x)}{p(l|G)}l∗=argmax{p(l|x)}{p(l|G)}

其中G表示grammar。

 

可以使用传统的token传播算法进行解码。

 

算法流程图如下：

 ![这里写图片描述](https://img-blog.csdn.net/20160702004203111)



假设词典D包含2个单词w，分别是{北,京}，每个单词对应两个因素，所以$|w'|=5$

```bash
北 b ei
京 j ing12
```

![这里写图片描述](https://img-blog.csdn.net/20160702004239878)
当t=1的时候，每个w的前两个tok被激活 
当t=2的时候，每个w的tok只能在单词内传播，对于“北”来说，tok(北,3,2)和tok(北,4,2)将会被激活，同时tok(北,2,2)有两条路径可以达到，这里取两条路径的最大概率，加上b音素在t=2时刻对应的概率的对数值，作为tok(北,2,2)的得分。与此同时，b->ei这条路径完成了“北”这个单词对应的输出，所以此时“北”将会有对应的输出tok(北,-1,2)。 
当t=3的时候，对于“京”这个单词，除了正常的单词内tok传播，还将涉及到单词和单词之间的tok传播，找到所有单词w的输出tok得分和p(京|w)之和的最大值，作为tok(京,0,3)，并且将该w对应的单词放到tok(京,0,3)的history。tok(京,0,3)将可以向tok(京,1,4)传播。 
以此类推……. 
整体上来看，解码过程类似于hmm的维特比，找出最大概率对应的路径，不同之处在于ctc解码引入了blank节点用于得到最终的输出序列，而不关心每一帧的输出结果。 
通过设定beam，可以对每个时刻t对应的word输出tok进行剪枝，加快解码速度。



# 四、解码-WFST

类似于HCLG的wfst结构，[EESEN: END-TO-END SPEECH RECOGNITION USING DEEP RNN MODELS AND WFST-BASED DECODING](http://arxiv.org/pdf/1507.08240v3.pdf)文章提出了TLG的网络结构

## Grammar

![这里写图片描述](https://img-blog.csdn.net/20160704221701104)

## Lexicon

有两种形式，可以基于characters和phonemes。 
![这里写图片描述](https://img-blog.csdn.net/20160704221759589)

## Token

对应于传统的state，前后添加blank，而且状态存在自旋 
![这里写图片描述](https://img-blog.csdn.net/20160704221808105)

## 网络

$$
S=T\circ min(det(L\circ G))
$$



## 结论

相比于传统的hybrid方法，准确率差不多，解码速度有三倍以上的提升。原因在于，状态数从几千个降到了几十个，减小了网络复杂度。





## 参考

[《Supervised Sequence Labelling with Recurrent Neural Networks》 chapter7](https://www.cs.toronto.edu/~graves/preprint.pdf)







# 五、eesen训练源码



essen源码参考<https://github.com/yajiemiao/eesen>，这里简单说一下涉及到训练前后向的核心算法源码实现。 
以单句训练为准（多句并行类似），用到的变量

| 变量       | 含义                                                         |
| ---------- | ------------------------------------------------------------ |
| phones_num | 最后一层输出节点个数，对应于\|phones\|+1                     |
| labels_num | 一句话对应的标注扩展blank以后的个数，比如”123”扩展为”b1b2b3b” |
| frames_num | 一句话对应的总的帧数，对应于时间t                            |
| $y_k^t$    | 最后一层输出                                                 |
| $a_k^t$    | softmax层的输入                                              |

## CTC error

```cpp
ctc.Eval(net_out, targets, &obj_diff);
```

涉及到的变量的维度：

| 变量       | 维度                  |
| ---------- | --------------------- |
| net_out    | frames_num*phones_num |
| alpha/beta | frames_num*labes_num  |
| ctc_error  | frames_num*phones_num |

本来可以使用最终的公式求出对atkakt的error，代码中却分成了两部求解，可能逻辑上能体现出error反向传播的过程，但是实际感觉没有必要。

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
$$
\frac{\partial L}{\partial a_k^t}=\sum_{k'}\frac{\partial L}{\partial y_{k'}^t}y_{k'}^t\delta_{kk'}-\sum_{k'}\frac{\partial L}{\partial y_{k'}^t}y_{k'}^ty_k^t\\
=\frac{\partial L}{\partial y_{k}^t}y_{k}^t-\sum_{k'}\frac{\partial L}{\partial y_{k'}^t}y_{k'}^ty_k^t
$$


注意上式最后一项有一个求和的过程，即将t时刻对应的$y_k^t$

的所有节点的error累加。



## 沿网络反向传播error

| 变量    | 含义                 |
| ------- | -------------------- |
| x       | 每一层的输入         |
| y       | 每一层的输出         |
| d_x     | 关于x的error         |
| d_y     | 关于y的error         |
| dim_in  | 输入维度             |
| dim_out | 输出维度             |
| W       | 每一层对应的参数矩阵 |

error依次经过affine-trans-layer和多层lstm-layer，每一层有两个目的： 
\- 求d_x: 将error传递到每一层的输入，以往后继续传播 
\- 求$\Delta W$: 计算当前层的参数的error，以根据error更新参数

### affine layer

| 变量  | 维度               |
| ----- | ------------------ |
| x/d_x | frames_num*dim_in  |
| y/d_y | frames_num*dim_out |
| W     | dim_out*dim_in     |

#### 前向

$$
y=x*W^T
$$

#### 后向

$$
\Delta W(t)=d\_y^T*x+momentum*\Delta W(t-1)
$$

这里参数更新有一个求和的过程，把所有时刻对应的$\Delta W$进行累加，相当于把所有时间的error数据进行了求和作为最终的error。

### lstm layer

![这里写图片描述](https://img-blog.csdn.net/20161124205816811)
参考[2]，eesen采用的lstm单元如上图，但是代码中变量的含义和论文中不一致。

### 前向

$$
i_t=\delta(x_tW_{ix}^T+m_{t-1}W_{im}^T+c_{t-1}W_{ic}^T+b_i)\\
f_t=\delta(x_tW_{fx}^T+m_{t-1}W_{fm}^T+c_{t-1}W_{fc}^T+b_i)\\
g_t=\delta(x_tW_{cx}^T+m_{t-1}W_{cm}^T+b_c)\\
c_t=f_t\odot c_{t-1}+i_t\odot g_t\\
o_t=\delta(x_tW_{ox}^T+m_{t-1}W_{om}^T+c_{t}W_{oc}^T+b_o)\\
h_t=\phi (c_t)\\
m_t=o_t\odot h_t
$$

有两方面的并行

\- gifo合并成一个矩阵 

\- 批量计算输入x（不依赖于t），然后再分帧计算其他变量



### 后向

$$
D_i=\frac{\partial L}{\partial (x_tW_{ix}^T+m_{t-1}W_{im}^T+c_{t-1}W_{ic}^T+b_i)}\\
D_f=\frac{\partial L}{\partial (x_tW_{fx}^T+m_{t-1}W_{fm}^T+c_{t-1}W_{fc}^T+b_i)}\\
D_g=\frac{\partial L}{\partial (x_tW_{cx}^T+m_{t-1}W_{cm}^T+b_c)}\\
D_o=\frac{\partial L}{\partial (x_tW_{ox}^T+m_{t-1}W_{om}^T+c_{t}W_{oc}^T+b_o)}\\
D_c=\frac{\partial L}{\partial (f_t\odot c_{t-1}+i_t\odot g_t)}
$$



有两个注意点

\-$m_t​$的error除了来自于t时刻的error，还有来至于t+1时刻的$D_i/D_f/D_o/Dg​$

 \-$c_t$的error除了来自于t时刻的error，还有来自于t+1时刻的$D_i/D_f/D_c$



## 参考文献

[1].[Connectionist Temporal Classification: Labelling Unsegmented Sequence Data with Recurrent Neural Networks](http://www.cs.toronto.edu/~graves/icml_2006.pdf) 
[2].Essen: End-to-End Speech Recognition Using Deep Rnn Models and WFST-Based Decoding