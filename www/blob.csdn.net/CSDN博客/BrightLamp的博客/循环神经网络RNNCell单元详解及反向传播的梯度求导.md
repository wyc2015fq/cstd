
# 循环神经网络RNNCell单元详解及反向传播的梯度求导 - BrightLamp的博客 - CSDN博客


2018年12月15日 16:25:33[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：104



## 摘要
本文给出循环神经网络RNNCell单元的定义公式, 并求解其在反向传播中的梯度.
本文给出的相关公式是编程导向的, 完整的, 可以直接用于代码实现, 已通过 Python 验证.
## 相关
*配套代码, 请参考文章 :*
纯Python和PyTorch对比实现循环神经网络RNN及反向传播
纯Python和PyTorch对比实现循环神经网络RNNCell及反向传播
Affine 变换的定义和梯度, 请参考:
affine/linear(仿射/线性)变换函数详解及全连接层反向传播的梯度求导
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. RNNCell 定义
### 1.1 一次循环
考虑输入一个 3 阶张量$X_{ijk}$, 该张量可以表示为$i$个尺寸为$j \times k$的矩阵$X_{jk}$, 同时表明循环单元的输入尺寸为$k$.
若单元输出尺寸为$n$, 输入层矩阵$X_{jk}$,变换矩阵为$W_{nk}$, 偏置向量$a_{1 \times n}$, 变换后的矩阵为$Y_{jn}$.
设初始隐含层矩阵为$H_{jn}$, 变换矩阵为$V_{n \times n}$, 偏置向量$b_{1 \times n}$, 变换后的矩阵为$Z_{jn}$.
设激活函数为 tanh, 则一次 RNNCell 循环变换为 :
$$
Y_{jn} = X_{jk}W_{nk}^T+a_{1 \times n}\\
\;\\
Z_{jn} = H_{jn}V_{nn}^T+b_{1 \times n}\\
\;\\
A_{jn}=Y_{jn}+Z_{jn}\\
\;\\
O_{jn} = tanh(A_{jn})
$$
将以上过程记为 :
$$
O_{jn} = RNNCell(X_{jk},H_{jn})
$$
循环到下一次时, 将$O_{jn}$代入$H_{jn}$, 与下一个$X_{jk}$重新进行运算.
### 1.2 循环迭代
下面使用迭代记法表示 RNNCell 运算.
使用$H_{jn}^{(0)}$表示初始隐含层矩阵, 对于 :
$$
X_{ijk} = X_{jk}^{(1)},X_{jk}^{(2)},X_{jk}^{(3)},\cdots,X_{jk}^{(i)}
$$
则 :
$$
H_{jn}^{(1)} = RNNCell(X_{jk}^{(1)},H_{jn}^{(0)})\\
\;\\
H_{jn}^{(2)} = RNNCell(X_{jk}^{(2)},H_{jn}^{(1)})\\
\;\\
H_{jn}^{(3)} = RNNCell(X_{jk}^{(3)},H_{jn}^{(2)})\\
\vdots\\
H_{jn}^{(i)} = RNNCell(X_{jk}^{(i)},H_{jn}^{(i-1)})\\
$$
展开最后一层作为示例 :
$$
Y_{jn}^{(i)} = X_{jk}^{(i)}W_{nk}^T+a_{1 \times n}\\
\;\\
Z_{jn}^{(i)} = H_{jn}^{(i-1)}V_{nn}^T+b_{1 \times n}\\
\;\\
A_{jn}^{(i)}=Y_{jn}^{(i)}+Z_{jn}^{(i)}\\
\;\\
H_{jn}^{(i)} = tanh(A_{jn}^{(i)})
$$
在迭代的过程中,$W_{nk}^T,\; V_{nn}^T,\; a_{1 \times n},\; b_{1 \times n}$都是共享的, 不变的.
### 1.3 张量公式
使用 3 阶张量表示 :
$$
H_{ijn} = RNNCell^{(i)}(X_{ijk},H_{jn}^{(0)})
$$
RNNCell 的上标$(i)$表示经过$i$次循环迭代运算.
注意, 经过 RNNCell 运算后, 输入尺寸为$i \times j \times k$的张量$X_{ijk}$将输出尺寸为$i \times j \times n$的张量$H_{ijn}$.
## 2. 反向传播
考虑输入一个 3 阶张量$X_{ijk}$, 经过 RNNCell 运算后, 输出 3 阶张量$H_{ijn}$, 往前 forward 传播得到误差值 error ( 标量 e ), e 对$H_{ijn}$的梯度$\nabla e_{(H_{ijn})}$已由上游给出, 求 e 对$X_{ijk}$的梯度.
$$
H_{ijn} = RNNCell^{(i)}(X_{ijk},H_{jn}^{(0)})\\
\;\\
e = forward(H_{ijn})
$$
为了避免符号混乱, 将上游传递的梯度记为$\nabla e_{(Q_{ijn})} = \nabla e_{(H_{ijn})}$,$\nabla e_{(H_{ijn})}$用于迭代计算的中间结果.
### 2.1 关于 H 的梯度
从 RNNCell 运算的定义可以看出, 每一次循环迭代运算都是由 Affine 运算和激活函数运算组合而成.
对于 :
$$
y = tanh(x)=\frac{e^x-e^{-x}}{e^x+e^{-x}} \\
\;\\
\frac{dy}{dx}= 1-y^2
$$
则 :
$$
\nabla {e}_{(A_{jn}^{(i)})}=\nabla e_{(Q_{jn}^{(i)})} \odot \nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})}\\
\;\\
\nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})} = \frac{d H_{jn}^{(i)}}{d A_{jn}^{(i)}}=1-{H_{jn}^{(i)}}^2\\
$$
上式中的$\nabla e_{(Q_{jn}^{(i)})}$已由上游给出,$\odot$表示元素积, 即矩阵同位元素分别相乘.
注意, 在这里, 我们得到了$\nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})}$的实际值, 因为$H_{jn}^{(i)}$是已知的.
因为激活函数不一定是$tanh$, 为了不失一般性, 下面我们直接使用$\nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})}$符号.
加法运算, 梯度直接往下游传递:
$$
A_{jn}=Y_{jn}+Z_{jn}\\
\;\\
\nabla {e}_{(Z_{jn}^{(i)})}=\nabla e_{(A_{jn}^{(i)})} \odot 1=\nabla e_{(A_{jn}^{(i)})}
$$
Affine 运算的定义及梯度求导公式已在上面的 <相关> 文章给出.
$$
Z_{jn} = H_{jn}V_{nn}^T+b_{1 \times n}\\
\;\\
\nabla {e}_{(H_{jn}^{(i-1)})}=\nabla e_{(Z_{jn}^{(i)})} V_{nn}=\nabla e_{(A_{jn}^{(i)})} V_{nn}
\;\\
\nabla {e}_{(H_{jn}^{(i-1)})}=(\nabla e_{(Q_{jn}^{(i)})} \odot \nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})})V_{nn}
$$
注意, 这里的矩阵的上标$(i)$特指张量$X_{ijk}$的最后一个矩阵, 即第$i$个矩阵.
根据 RNNCell 循环迭代计算的特性, 上次循环的梯度也影响本次循环, 所以梯度求导也是按顺序循环迭代的.
$$
\nabla {e}_{(H_{jk}^{(i-2)})}=\nabla e_{(Q_{jn}^{(i-1)})} \odot \nabla {H_{jn}^{(i-1)}}_{(A_{jn}^{(i-1)})}V_{nn}+\nabla e_{(H_{jn}^{(i-1)})}\odot \nabla {H_{jn}^{(i-1)}}_{(A_{jn}^{(i-1)})} V_{nn}\\
\;\\
\nabla {e}_{(H_{jk}^{(i-2)})}=(\nabla e_{(Q_{jn}^{(i-1)})}+\nabla e_{(H_{jn}^{(i-1)})}) \odot \nabla {H_{jn}^{(i-1)}}_{(A_{jn}^{(i-1)})}V_{nn}
$$
为了版面简洁, 上式中$V_{nn}$前面省略了一个小括号, 请按照从左至右的顺序计算.
公式汇总如下 :
$$
\nabla {e}_{(H_{jn}^{(i-1)})}=(\nabla e_{(Q_{jn}^{(i)})} \odot \nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})})V_{nn}\\
\;\\
\nabla {e}_{(H_{jk}^{(i-2)})}=(\nabla e_{(Q_{jn}^{(i-1)})}+\nabla e_{(H_{jn}^{(i-1)})}) \odot \nabla {H_{jn}^{(i-1)}}_{(A_{jn}^{(i-1)})}V_{nn}\\
\;\\
\nabla {e}_{(H_{jk}^{(i-3)})}=(\nabla e_{(Q_{jn}^{(i-2)})}+\nabla e_{(H_{jn}^{(i-2)})}) \odot \nabla {H_{jn}^{(i-2)}}_{(A_{jn}^{(i-2)})}V_{nn}\\
\vdots\\
\nabla {e}_{(H_{jk}^{(0)})}=(\nabla e_{(Q_{jn}^{(1)})}+\nabla e_{(H_{jn}^{(1)})}) \odot \nabla {H_{jn}^{(1)}}_{(A_{jn}^{(1)})}V_{nn}\\
$$

### 2.2 关于 X 的梯度
参考上例以及 Affine 层的求导公式得 :
$$
Y_{jn} = X_{jk}W_{nk}^T+a_{1 \times n}\\
\;\\
\nabla {e}_{(Y_{jn}^{(i)})}=\nabla e_{(A_{jn}^{(i)})} \odot 1=\nabla e_{(A_{jn}^{(i)})}\\
\;\\
\nabla {e}_{(X_{jk}^{(i)})}=\nabla e_{(Y_{jn}^{(i)})} W_{nk}=\nabla e_{(A_{jn}^{(i)})}W_{nk}
$$
同样的, 循环迭代公式如下 :
$$
\nabla {e}_{(X_{jn}^{(i)})}=(\nabla e_{(Q_{jn}^{(i)})} \odot \nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})}) W_{nk}\\
\;\\
\nabla {e}_{(X_{jk}^{(i-1)})}=(\nabla e_{(Q_{jn}^{(i-1)})}+\nabla e_{(H_{jn}^{(i-1)})}) \odot \nabla {H_{jn}^{(i-1)}}_{(A_{jn}^{(i-1)})}W_{nk}\\
\;\\
\nabla {e}_{(X_{jk}^{(i-2)})}=(\nabla e_{(Q_{jn}^{(i-2)})}+\nabla e_{(H_{jn}^{(i-2)})}) \odot \nabla {H_{jn}^{(i-2)}}_{(A_{jn}^{(i-2)})}W_{nk}\\
\vdots\\
\nabla {e}_{(X_{jk}^{(1)})}=(\nabla e_{(Q_{jn}^{(1)})}+\nabla e_{(H_{jn}^{(1)})}) \odot \nabla {H_{jn}^{(1)}}_{(A_{jn}^{(1)})}W_{nk}\\
$$
为了版面简洁, 上式中$W_{nk}$前面省略了一个小括号, 请按照从左至右的顺序计算.
$\nabla e_{(H_{ijn})}$已由上例计算给出, 请注意区分$\nabla e_{(Q_{ijn})}$.
### 2.3 关于 W 的梯度
$W_{nk}$在所有的迭代步骤中是共享的, 一样的, 根据链式求导法则:
$$
\nabla {e}_{(W_{nk})}=\nabla {e}_{(Q_{jn}^{(1)})}\odot\frac{dH_{jn}^{(1)}}{dW_{nk}}+\nabla {e}_{(Q_{jn}^{(2)})}\odot\frac{dH_{jn}^{(2)}}{dW_{nk}}+\cdots+\nabla {e}_{(Q_{jn}^{(i)})}\odot\frac{dH_{jn}^{(i)}}{dW_{nk}}
$$
或 :
$$
\nabla {e}_{(W_{nk})}=\nabla e_{(W_{nk}^{(1)})}+\nabla e_{(W_{nk}^{(2)})}+\nabla e_{(W_{nk}^{(3)})}+\cdots+\nabla e_{(W_{nk}^{(i)})}
$$
由于 :
$$
\nabla {e}_{(A_{jn}^{(i)})}=\nabla e_{(Q_{jn}^{(i)})} \odot \nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})}\\
\;\\
\nabla e_{(W_{nk}^{(i)})}=(\nabla e_{(Y_{jn}^{(i)})})^T X_{jk}^{(i)}=(\nabla e_{(A_{jn}^{(i)})})^T X_{jk}^{(i)}=(\nabla e_{(Q_{jn}^{(i)})} \odot \nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})})^TX_{jk}^{(i)}
$$
上式$W_{nk}$的上标$(i)$表示第$i$步计算中得到的梯度. 在循环迭代的过程中 :
$$
\nabla e_{(W_{nk}^{(i)})}=(\nabla e_{(Q_{jn}^{(i)})} \odot \nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})})^T X_{jk}^{(i)}\\
\;\\
\nabla e_{(W_{nk}^{(i-1)})}=((\nabla e_{(Q_{jn}^{(i-1)})}+\nabla e_{(H_{jn}^{(i-1)})}) \odot \nabla {H_{jn}^{(i-1)}}_{(A_{jn}^{(i-1)})})^TX_{jk}^{(i-1)}\\
\;\\
\nabla e_{(W_{nk}^{(i-2)})}=((\nabla e_{(Q_{jn}^{(i-2)})}+\nabla e_{(H_{jn}^{(i-2)})}) \odot \nabla {H_{jn}^{(i-2)}}_{(A_{jn}^{(i-2)})})^TX_{jk}^{(i-2)}\\
\vdots\\
\nabla e_{(W_{nk}^{(1)})}=((\nabla e_{(Q_{jn}^{(1)})}+\nabla e_{(H_{jn}^{(1)})}) \odot \nabla {H_{jn}^{(1)}}_{(A_{jn}^{(1)})})^TX_{jk}^{(1)}\\
$$
最后, 将上面的结果加起来即可:
$$
\nabla {e}_{(W_{nk})}=\nabla e_{(W_{nk}^{(1)})}+\nabla e_{(W_{nk}^{(2)})}+\nabla e_{(W_{nk}^{(3)})}+\cdots+\nabla e_{(W_{nk}^{(i)})}
$$

### 2.4 关于 V 的梯度
$V_{nn}$在所有的迭代步骤中是共享的, 根据链式求导法则, 参考上例, 易得
$$
\nabla e_{(V_{nn}^{(i)})}=(\nabla e_{(Q_{jn}^{(i)})} \odot \nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})})^T H_{jn}^{(i-1)}\\
\;\\
\nabla e_{(V_{nn}^{(i-1)})}=((\nabla e_{(Q_{jn}^{(i-1)})}+\nabla e_{(H_{jn}^{(i-1)})}) \odot \nabla {H_{jn}^{(i-1)}}_{(A_{jn}^{(i-1)})})^T H_{jn}^{(i-2)}\\
\;\\
\nabla e_{(V_{nn}^{(i-2)})}=((\nabla e_{(Q_{jn}^{(i-2)})}+\nabla e_{(H_{jn}^{(i-2)})}) \odot \nabla {H_{jn}^{(i-2)}}_{(A_{jn}^{(i-2)})})^T H_{jn}^{(i-3)}\\
\vdots\\
\nabla e_{(V_{nn}^{(1)})}=((\nabla e_{(Q_{jn}^{(1)})}+\nabla e_{(H_{jn}^{(1)})}) \odot \nabla {H_{jn}^{(1)}}_{(A_{jn}^{(1)})})^T H_{jn}^{(0)}\\
$$
将上面的结果加起来即可:
$$
\nabla {e}_{(V_{nn})}=\nabla e_{(V_{nn}^{(1)})}+\nabla e_{(V_{nn}^{(2)})}+\nabla e_{(V_{nn}^{(3)})}+\cdots+\nabla e_{(V_{nn}^{(i)})}
$$

### 2.5 关于 a 和 b 的梯度
$$
Y_{jn} = X_{jk}W_{nk}^T+a_{1 \times n}\\
\;\\
\nabla {e}_{(Y_{jn}^{(i)})}=\nabla e_{(A_{jn}^{(i)})} \odot 1=\nabla e_{(A_{jn}^{(i)})}\\
\;\\
\nabla {e}_{(a_{1 \times n}^{i})}=\nabla e_{(Y_{jn}^{(i)})} \odot 1=\nabla e_{(A_{jn}^{(i)})}= \nabla e_{(Q_{jn}^{(i)})} \odot \nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})}
$$
$a_{1 \times n}$在所有的迭代步骤中是共享的, 一样的, 根据链式求导法则, 参考上例, 易得 :
$$
\nabla {e}_{(a_{1 \times n}^{(i)})}= \nabla e_{(Q_{jn}^{(i)})} \odot \nabla {H_{jn}^{(i)}}_{(A_{jn}^{(i)})}\\
\;\\
\nabla {e}_{(a_{1 \times n}^{(i-1)})}= (\nabla e_{(Q_{jn}^{(i-1)})} + \nabla e_{(H_{jn}^{(i-1)})}) \odot \nabla {H_{jn}^{(i-1)}}_{(A_{jn}^{(i-1)})}\\
\;\\
\nabla {e}_{(a_{1 \times n}^{(i-2)})}= (\nabla e_{(Q_{jn}^{(i-2)})} + \nabla e_{(H_{jn}^{(i-2)})}) \odot \nabla {H_{jn}^{(i-2)}}_{(A_{jn}^{(i-2)})}\\
\vdots\\
\nabla {e}_{(a_{1 \times n}^{(1)})}= (\nabla e_{(Q_{jn}^{(1)})} + \nabla e_{(H_{jn}^{(1)})}) \odot \nabla {H_{jn}^{(1)}}_{(A_{jn}^{(1)})}
$$
将上面的结果加起来即可 :
$$
\nabla {e}_{(a_{1 \times n})}=\nabla {e}_{(a_{1 \times n}^{(1)})}+\nabla {e}_{(a_{1 \times n}^{(2)})}+\nabla {e}_{(a_{1 \times n}^{(3)})}+\cdots+\nabla {e}_{(a_{1 \times n}^{(i)})}
$$
同理, 观察公式定义, 我们可以得到 :
$$
\nabla {e}_{(b_{1 \times n})}=\nabla {e}_{(a_{1 \times n})}
$$


