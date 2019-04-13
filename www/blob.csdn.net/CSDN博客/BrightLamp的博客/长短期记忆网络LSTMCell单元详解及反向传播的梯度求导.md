
# 长短期记忆网络LSTMCell单元详解及反向传播的梯度求导 - BrightLamp的博客 - CSDN博客


2018年12月18日 16:17:47[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：173



## 摘要
本文给出长短期记忆网络LSTMCell单元的定义公式, 并求解其在反向传播中的梯度.
求导过程比较长, 涉及的变量很多, 但求导过程并不难, 只要细心即可.
给出的相关公式是完整的, 编程导向的, 可以直接用于代码实现, 已通过 Python 验证.
## 相关
配套代码, 请参考文章 :
*纯 Python 和 PyTorch 对比实现循环神经网络 LSTM 及反向传播*
Affine 变换的定义和梯度, 请参考文章 :
*affine/linear(仿射/线性)变换函数详解及全连接层反向传播的梯度求导*
文章索引 :
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. LSTMCell 定义
### 1.1 一次迭代
n考虑输入一个 3 阶张量$X_{lmn}$, 该张量可以表示为$l$个尺寸为$m \times n$的矩阵$X_{mn}$, 同时表明循环单元的输入尺寸为$n$.
设第一个输入矩阵为$X_{mn}^{(1)}$, 对应的 4 个门的变换矩阵分别为$W_i,W_f,W_o,W_c$, 偏置向量为$a_i,a_f,a_o,a_c$.
设初始隐含层矩阵为$H_{mr}^{(0)}$, 对应的 4 个门的变换矩阵分别为$V_i,V_f,V_o,V_c$, 偏置向量为$b_i,b_f,b_o,b_c$.
设初始单元转态矩阵为$C _{mr}^{(0)}$, 则一次 LSTMCell 循环变换为 :
$$
A_i = X^{(1)}{W_{i}}^T + a_{i} + H^{(0)}V_{i}^T + b_{i}\\
A_f = X^{(1)}{W_{f}}^T + a_{f} + H^{(0)}V_{f}^T + b_{f}\\
A_o = X^{(1)}{W_{o}}^T + a_{o} + H^{(0)}V_{o}^T + b_{o}\\
A_c = X^{(1)}{W_{c}}^T + a_{c} + H^{(0)}V_{c}^T + b_{c}\\
g_i = sigmoid(A_i)\\
g_f = sigmoid(A_f)\\
g_o = sigmoid(A_o)\\
g_c = tanh(A_c)\\
C^{(1)} =g_i\odot g_c + g_f\odot C^{(0)}\\
H^{(1)} =g_o\odot tanh(C^{(1)})
$$
上式中的$\odot$表示 element-wise 元素积, 将以上过程记为 :
$$
H^{(1)},C^{(1)} = LSTMCell(X^{(1)},H^{(0)},C^{(0)})
$$
循环到下一次时, 将$H^{(1)},C^{(1)}$代入$H^{(0)},C^{(0)}$的位置, 与下一个$X^{(2)}$重新进行运算.
### 1.2 循环迭代
下面使用迭代记法表示 LSTMCell 运算.
使用$H^{(0)},C^{(0)}$表示初始隐含层矩阵和初始状态矩阵, 对于 :
$$
X_{lmn} = X_{mn}^{(1)},X_{mn}^{(2)},X_{mn}^{(3)},\cdots,X_{mn}^{(l)}
$$
则 :
$$
H^{(1)},C^{(1)} = LSTMCell(X^{(1)},H^{(0)},C^{(0)})\\
\;\\
H^{(2)},C^{(2)} = LSTMCell(X^{(2)},H^{(1)},C^{(1)})\\
\;\\
H^{(3)},C^{(3)} = LSTMCell(X^{(3)},H^{(2)},C^{(2)})\\
\vdots\\
H^{(l)},C^{(l)} = LSTMCell(X^{(l)},H^{(l-1)},C^{(l-1)})\\
$$
展开最后一层作为示例 :
$$
g_i = sigmoid(X^{(l)}{W_{i}}^T + a_{i} + H^{(l-1)}V_{i}^T + b_{i})\\
\;\\
g_f = sigmoid(X^{(l)}{W_{f}}^T + a_{f} + H^{(l-1)}V_{f}^T + b_{f})\\
\;\\
g_o = sigmoid(X^{(l)}{W_{o}}^T + a_{o} + H^{(l-1)}V_{o}^T + b_{o})\\
\;\\
g_c = tanh(X^{(l)}{W_{c}}^T + a_{c} + H^{(l-1)}V_{c}^T + b_{c})\\
\;\\
C^{(l)} =g_i\odot g_c + g_f\odot C^{(l-1)}\\
\;\\
H^{(l)} =g_o\odot tanh(C^{(l)})
$$
在迭代的过程中,$W, \; V , \; a, \; b$都是共享的, 不变的.
### 1.3 张量公式
使用 3 阶张量表示 :
$$
H_{lmr},C_{lmr} = LSTMCell^{(l)}(X_{lmn},H_{mr}^{(0)},C_{mr}^{(0)})
$$
LSTMCell 的上标$(l)$表示经过$l$次循环迭代运算, 输入尺寸为$l \times m \times n$的张量$X_{lmn}$将输出尺寸为$l \times m \times r$的张量$H_{lmr}, C_{lmr}$.  其中,$H_{lmr}$将参与神经网络的损失值运算,$C_{lmr}$仅为 LSTM 所用.
## 2. 反向传播
考虑输入一个 3 阶张量$X_{lmn}$, 经过 LSTMCell 运算后, 输出 3 阶张量$H_{lmr}$, 往前 forward 传播得到误差值 error ( 标量 e ), e 对$H_{lmr}$的梯度$\nabla e_{(H_{lmr})}$已由上游给出, 求 e 对$X_{lmn}$的梯度.
$$
H_{ijn},C_{ijn} = RNNCell^{(i)}(X_{ijk},H_{jn}^{(0)},C_{jn}^{(0)})\\
\;\\
e = forward(H_{ijn})
$$

### 2.1 相关函数的梯度
从 LSTMCell 运算的定义可以看出, 每一次循环迭代都是由 Affine 运算和激活函数运算组合而成.
Affine 运算的定义及梯度求导公式已在上面的 <相关> 中给出.
关于 Affine 的梯度 :
$$
A = XW^T + b\\
\;\\
\frac {d e}{d X} =\nabla e_{(A)}W\\
\;\\
\frac {d e}{d W} =\nabla e_{(A)}^TX\\
\;\\
\frac {de}{db}=sum(\nabla e_{(A)},\; axis=0)
$$
关于 tanh 的梯度 :
$$
y = tanh(x)=\frac{e^x-e^{-x}}{e^x+e^{-x}} \\
\;\\
\frac{dy}{dx}= 1-y^2
$$
关于 sigmoid 的梯度 :
$$
y = sigmoid(x)=\frac{1}{1+e^{-x}} \\
\;\\
\frac{dy}{dx}= y(1-y)
$$

### 2.2 关于 C 的梯度
LSTMCell 的运算是循环迭代的, 每一次梯度不仅受到上游 forward 运算的影响, 还受到自身上一步运算的影响.
为了避免符号混乱, 将上游 forward 运算传递到 H 的梯度$\nabla e_{(H_{lmr})}$记为$\nabla e_{(F_{lmr})}$,$\nabla e_{(H_{lmr})}$用于迭代过程中的内部计算.
另外, LSTMCell 有两个输出值, 本题隐含了一个条件$\nabla e_{(C_{lmr})} = 0$, 表示上游 forward 运算对输出张量$C_{ijn}$的梯度为零. 同样的, 将外部梯度$\nabla e_{(C_{lmr})}$记为$\nabla e_{(D_{lmr})}$,$\nabla e_{(C_{lmr})}$用于内部运算. 为了格式整齐和逻辑完整, 需要加上这个条件.
$$
\frac{de}{dC^{(l-1)}}=\frac{de}{dF^{(l)}}\odot \frac{dH^{(l)}}{dC^{(l-1)}}+\frac{de}{dD^{(l)}}\odot \frac{dC^{(l)}}{dC^{(l-1)}}\\
=\frac{de}{dF^{(l)}}\odot g_o^{(l)}\odot(1-tanh^2(C^{(l)}))\odot g_f^{(l)}+\frac{de}{dD^{(l)}}\odot g_f^{(l)}
令 :
$$
令 :
$$
T^{(l)}= g_o^{(l)}\odot(1-tanh^2(C^{(l)}))
$$
从最后一次开始求解 :
$$
\frac{de}{dC^{(l-1)}}=\frac{de}{dF^{(l)}}\odot T^{(l)}\odot g_f^{(l)}+\frac{de}{dD^{(l)}}\odot g_f^{(l)}
$$
在这一步, 同样可以得到$de / dH^{(l-1)}$, 这个结果不依赖于$de / dC^{(l-1)}$.$de / dH^{(l-1)}$的计算过程比较长, 放到下文, 这里先拿来使用.
综合上游和自身迭代的梯度, 迭代求解 :
$$
\frac{de}{dC^{(l-1)}} = \frac{de}{dF^{(l)}} T^{(l)} g_f^{(l)}+\frac{de}{dD^{(l)}} g_f^{(l)}\\
\;\\
\frac{de}{dC^{(l-2)}}=(\frac{de}{dF^{(l-1)}}+\frac{de}{dH^{(l-1)}}) T^{(l-1)} g_f^{(l-1)}+(\frac{de}{dD^{(l-1)}}+\frac{de}{dC^{(l-1)}}) g_f^{(l-1)}\\
\;\\
\frac{de}{dC^{(l-3)}}=(\frac{de}{dF^{(l-2)}}+\frac{de}{dH^{(l-2)}}) T^{(l-2)} g_f^{(l-2)}+(\frac{de}{dD^{(l-2)}}+\frac{de}{dC^{(l-2)}}) g_f^{(l-2)}\\
\vdots\\
\frac{de}{dC^{(0)}}=(\frac{de}{dF^{(1)}}+\frac{de}{dH^{(1)}}) T^{(1)} g_f^{(1)}+(\frac{de}{dD^{(1)}}+\frac{de}{dC^{(1)}}) g_f^{(1)}
$$
注意, 上面的乘法都是元素积运算, 为了美观, 符号$\odot$被省略了.
### 2.3 关于 4 个门
$$
g_i, g_f, g_o, g_c
$$
### 的梯度
$g_i, g_f, g_c$计算过程和上例类似, 综合上游和自身迭代的梯度 :
$$
\frac{de}{dg^{(l)}} = \frac{de}{dF^{(l)}} T^{(l)} G^{(l)}+\frac{de}{dD^{(l)}} G^{(l)}\\
\;\\
\frac{de}{dg^{(l-1)}}=(\frac{de}{dF^{(l-1)}}+\frac{de}{dH^{(l-1)}}) T^{(l-1)} G^{(l-1)}+(\frac{de}{dD^{(l-1)}}+\frac{de}{dC^{(l-1)}}) G^{(l-1)}\\
\;\\
\frac{de}{dg^{(l-2)}}=(\frac{de}{dF^{(l-2)}}+\frac{de}{dH^{(l-2)}}) T^{(l-1)} G^{(l-1)}+(\frac{de}{dD^{(l-2)}}+\frac{de}{dC^{(l-2)}}) G^{(l-2)}\\
\vdots\\
\frac{de}{dg^{(1)}}=(\frac{de}{dF^{(1)}}+\frac{de}{dH^{(1)}}) T^{(1)} G^{(1)}+(\frac{de}{dD^{(1)}}+\frac{de}{dC^{(1)}}) G^{(1)}
$$
按下面的表格替换上述公式中的$g, G$.
|序号|g|G|
|---|---|---|
|1
$$
g_i
$$
$$
g_c
$$
|
|2
$$
g_f
$$
$$
C^{(l-1)}
$$
|
|3
$$
g_c
$$
$$
g_i
$$
|
$g_o$计算过程不涉及 C 迭代 :
$$
\frac{de}{dg_o^{(l)}} = \frac{de}{dF^{(l)}} tanh(C^{(l)})\\
\;\\
\frac{de}{dg_o^{(l-1)}}=(\frac{de}{dF^{(l-1)}}+\frac{de}{dH^{(l-1)}}) tanh(C^{(l-1)})\\
\;\\
\frac{de}{dg_o^{(l-2)}}=(\frac{de}{dF^{(l-2)}}+\frac{de}{dH^{(l-2)}}) tanh(C^{(l-2)})\\
\vdots\\
\frac{de}{dg_o^{(1)}}=(\frac{de}{dF^{(1)}}+\frac{de}{dH^{(1)}}) tanh(C^{(1)})
$$
同样的, 上面的乘法都是元素积运算, 为了美观, 符号$\odot$被省略了.
### 2.4 关于 Affine 层的梯度
$$
\frac{de}{dA_i^{(l)}}=\frac{de}{dg_i^{(l)}}\odot g_i^{(l)}\odot (1-g_i^{(l)})\\
\;\\
\frac{de}{dA_f^{(l)}}=\frac{de}{dg_f^{(l)}}\odot g_f^{(l)}\odot (1-g_f^{(l)})\\
\;\\
\frac{de}{dA_o^{(l)}}=\frac{de}{dg_o^{(l)}}\odot g_o^{(l)}\odot (1-g_o^{(l)})\\
\;\\
\frac{de}{dA_c^{(l)}}=\frac{de}{dg_c^{(l)}} \odot (1-(g_c^{(l)})^2)\\
$$
上述梯度公式的格式在每一步迭代都是一样的, 将$(l)$替换为相应的步数即可.
涉及循环迭代的部分, 已在$g_i, g_f, g_o, g_c$的计算中考虑.
### 2.5 关于 X, H 的梯度
根据 Affine 的梯度公式, 易得 :
$$
\frac{de_i}{dX^{(l)}}=\frac{de}{dA_i^{(l)}}W_i\\
\;\\
\frac{de_f}{dX^{(l)}}=\frac{de}{dA_f^{(l)}}W_f\\
\;\\
\frac{de_o}{dX^{(l)}}=\frac{de}{dA_o^{(l)}}W_o\\
\;\\
\frac{de_c}{dX^{(l)}}=\frac{de}{dA_c^{(l)}}W_c\\
$$
注意, 这里不是元素积, 是标准的矩阵乘法.
$e_i$的下标$i$用于区分 e 通过$W_i$路径反向传播的梯度,$e_f, e_o, e_c$同理.
综合 4 个路径对 X 的影响, 得 :
$$
\frac{de}{dX^{(l)}} = \frac{de_i}{dX^{(l)}}+\frac{de_f}{dX^{(l)}}+\frac{de_o}{dX^{(l)}}+\frac{de_c}{dX^{(l)}}
$$
同理 :
$$
\frac{de_i}{dH^{(l-1)}}=\frac{de}{dA_i^{(l)}}V_i\\
\;\\
\frac{de_f}{dH^{(l-1)}}=\frac{de}{dA_f^{(l)}}V_f\\
\;\\
\frac{de_o}{dH^{(l-1)}}=\frac{de}{dA_o^{(l)}}V_o\\
\;\\
\frac{de_c}{dH^{(l-1)}}=\frac{de}{dA_c^{(l)}}V_c\\
$$

$$
\frac{de}{dH^{(l-1)}} = \frac{de_i}{dH^{(l-1)}}+\frac{de_f}{dH^{(l-1)}}+\frac{de_o}{dH^{(l-1)}}+\frac{de_c}{dH^{(l-1)}}
$$

### 2.3 关于 W, V 的梯度
根据 Affine 的梯度公式, 易得 :
$$
\frac{de}{dW_i^{(l)}}=(\frac{de}{dA_i^{(l)}})^TX^{(l)}\\
\;\\
\frac{de}{dW_f^{(l)}}=(\frac{de}{dA_f^{(l)}})^TX^{(l)}\\
\;\\
\frac{de}{dW_o^{(l)}}=(\frac{de}{dA_o^{(l)}})^TX^{(l)}\\
\;\\
\frac{de}{dW_c^{(l)}}=(\frac{de}{dA_c^{(l)}})^TX^{(l)}\\
$$
注意, 这里不是元素积, 是标准的矩阵乘法.
同理:
$$
\frac{de}{dV_i^{(l)}}=(\frac{de}{dA_i^{(l)}})^TH^{(l-1)}\\
\;\\
\frac{de}{dV_f^{(l)}}=(\frac{de}{dA_f^{(l)}})^TH^{(l-1)}\\
\;\\
\frac{de}{dV_o^{(l)}}=(\frac{de}{dA_o^{(l)}})^TH^{(l-1)}\\
\;\\
\frac{de}{dV_c^{(l)}}=(\frac{de}{dA_c^{(l)}})^TH^{(l-1)}\\
$$
最后, 将所有迭代步的梯度相加即可 :
$$
\frac{de}{dW}=\frac{de}{dW^{(1)}}+\frac{de}{dW^{(2)}}+\frac{de}{dW^{(3)}}+\cdots+\frac{de}{dW^{(l)}}\\
\;\\
\frac{de}{dV}=\frac{de}{dV^{(1)}}+\frac{de}{dV^{(2)}}+\frac{de}{dV^{(3)}}+\cdots+\frac{de}{dV^{(l)}}\\
$$
为了简洁起见, 上式不区分$W_i, W_f, W_o, W_c$和$V_i, V_f, V_o, V_c$, 格式都是一样的.
### 2.8 关于 a, b 的梯度
根据 Affine 的梯度公式, 易得 :
$$
\frac{de}{da_i^{(l)}}=sum(\frac{de}{dA_i^{(l)}},\; axis=0)\\
\;\\
\frac{de}{da_f^{(l)}}=sum(\frac{de}{dA_f^{(l)}},\; axis=0)\\
\;\\
\frac{de}{da_o^{(l)}}=sum(\frac{de}{dA_o^{(l)}},\; axis=0)\\
\;\\
\frac{de}{da_c^{(l)}}=sum(\frac{de}{dA_c^{(l)}},\; axis=0)\\
$$
最后, 将所有迭代步的梯度相加即可 :
$$
\frac{de}{da}=\frac{de}{da^{(1)}}+\frac{de}{da^{(2)}}+\frac{de}{da^{(3)}}+\cdots+\frac{de}{da^{(l)}}\\
$$
上式不区分$a_i, a_f, a_o, a_c$, 格式都是一样的.
同样的有 :
$$
\frac{de}{db}=\frac{de}{da}
$$


