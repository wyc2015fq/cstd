
# 门控循环单元GRUCell详解及反向传播的梯度求导 - BrightLamp的博客 - CSDN博客


2018年12月19日 22:44:56[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：153



## 摘要
本文给出门控循环单元GRUCell的定义公式, 并求解其在反向传播中的梯度.
给出的相关公式是完整的, 编程导向的, 可以直接用于代码实现, 已通过 Python 验证.
## 相关
*配套代码, 请参考文章 :*
纯 Python 和 PyTorch 对比实现门控循环单元 GRU 及反向传播
*Affine 变换的定义和梯度, 请参考文章 :*
affine/linear(仿射/线性)变换函数详解及全连接层反向传播的梯度求导
系列文章索引 :
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. GRUCell 定义
### 1.1 一次迭代
n考虑输入一个 3 阶张量$X_{lmn}$, 该张量可以表示为$l$个尺寸为$m \times n$的矩阵$X_{mn}$, 同时表明循环单元的输入尺寸为$n$.
设第一个输入矩阵为$X_{mn}^{(1)}$, 对应的 3 个变换矩阵分别为$W_r,W_u,W_c$, 偏置向量为$a_r,a_u,a_c$.
设初始隐含层矩阵为$H_{mr}^{(0)}$, 对应的 2 个变换矩阵分别为$V_r,V_u,V_c$, 偏置向量为$b_r,b_u,b_c$.
则一次 GRUCell 循环变换为 :
$$
A_r =X^{(1)}{W_{r}}^T + a_{r} + H^{(0)}V_{r}^T + b_{r}\\
A_u =X^{(1)}{W_{u}}^T + a_{u} + H^{(0)}V_{u}^T + b_{u}\\
g_r =sigmoid(A_r)\\
g_u =sigmoid(A_u)\\
\;\\
A_c =X^{(1)}{W_{c}}^T + a_{c} + g_r \odot (H^{(0)}V_{c}^T + b_{c})\\
g_c =tanh(A_c)\\
H^{(1)} =(1 - g_u)\odot g_c + g_u \odot H^{(0)}
$$
上式中的$\odot$表示 element-wise 元素积, 将以上过程记为 :
$$
H^{(1)} = GRUCell(X^{(1)},H^{(0)})
$$
循环到下一次时, 将$H^{(1)},C^{(1)}$代入$H^{(0)},C^{(0)}$的位置, 与下一个$X^{(2)}$重新进行运算.
### 1.2 循环迭代
下面使用迭代记法表示 GRUCell 运算.
使用$H^{(0)}$表示初始隐含层矩阵, 对于 :
$$
X_{lmn} = X_{mn}^{(1)},X_{mn}^{(2)},X_{mn}^{(3)},\cdots,X_{mn}^{(l)}
$$
则 :
$$
H^{(1)} = GRUCell(X^{(1)},H^{(0)})\\
\;\\
H^{(2)} = GRUCell(X^{(2)},H^{(1)})\\
\;\\
H^{(3)} = GRUCell(X^{(3)},H^{(2)})\\
\vdots\\
H^{(l)} = GRUCell(X^{(l)},H^{(l-1)})\\
$$
展开最后一层作为示例 :
$$
A_r =X^{(l)}{W_{r}}^T + a_{r} + H^{(l-1)}V_{r}^T + b_{r}\\
A_u =X^{(l)}{W_{u}}^T + a_{u} + H^{(l-1)}V_{u}^T + b_{u}\\
g_r =sigmoid(A_r)\\
g_u =sigmoid(A_u)\\
\;\\
A_c =X^{(l)}{W_{c}}^T + a_{c} + g_r \odot (H^{(l-1)}V_{c}^T + b_{c})\\
g_c =tanh(A_c)\\
H^{(l)} =(1 - g_u)\odot g_c + g_u \odot H^{(l-1)}
$$
在迭代的过程中$W, \; V , \; a, \; b$是共享的, 不变的.
### 1.3 张量公式
使用 3 阶张量表示 :
$$
H_{lmr} = GRUCell^{(l)}(X_{lmn},H_{mr}^{(0)})
$$
GRUCell 的上标$(l)$表示经过$l$次循环迭代计算, 输入尺寸为$l \times m \times n$的张量$X_{lmn}$将输出尺寸为$l \times m \times r$的张量$H_{lmr}$.
## 2. 反向传播
考虑输入一个 3 阶张量$X_{lmn}$, 经过 GRUCell 运算后, 输出 3 阶张量$H_{lmr}$, 往前 forward 传播得到误差值 error ( 标量 e ), e 对$H_{lmr}$的梯度$\nabla e_{(H_{lmr})}$已由上游给出, 求 e 对$X_{lmn}$的梯度.
$$
H_{ijn},C_{ijn} = RNNCell^{(i)}(X_{ijk},H_{jn}^{(0)},C_{jn}^{(0)})\\
\;\\
e = forward(H_{ijn})
$$

### 2.1 相关函数的梯度
从 GRUCell 运算的定义可以看出, 每一次循环迭代都是由 Affine 计算和激活函数计算组合而成.
Affine 计算的定义及梯度求导公式已在上面的 <相关> 中给出.
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

### 2.2 关于
$$
g_u
$$
### 的梯度
GRUCell 的运算是循环迭代的, 每一次梯度不仅受到上游 forward 运算的影响, 还受到自身上一步运算的影响.
为了避免符号混乱, 将上游 forward 运算传递到 H 的梯度$\nabla e_{(H_{lmr})}$记为$\nabla e_{(F_{lmr})}$,$\nabla e_{(H_{lmr})}$用于迭代过程中的内部计算.
从最后一步开始算起 :
$$
\frac{de}{dg_u^{(l)}}=\frac{de}{dF^{(l)}}\odot(-g_c^{(l)}+H^{(l-1)})\\
$$
在这一步, 同样可以得到$de / dH^{(l-1)}$, 这个结果不依赖于$de / dg_u^{(l)}$, 是独立的.$de / dH^{(l-1)}$的计算过程比较长, 放到下文, 这里先拿来使用.
按顺序往下迭代 :
$$
\frac{de}{dg_u^{(l)}}=\frac{de}{dF^{(l)}}\odot(-g_c^{(l)}+H^{(l-1)})\\
\frac{de}{dg_u^{(l-1)}}=(\frac{de}{dF^{(l-1)}}+\frac{de}{dH^{(l-1)}})\odot(-g_c^{(l-1)}+H^{(l-2)})\\
\frac{de}{dg_u^{(l-2)}}=(\frac{de}{dF^{(l-2)}}+\frac{de}{dH^{(l-2)}})\odot(-g_c^{(l-2)}+H^{(l-3)})\\
\vdots\\
\frac{de}{dg_u^{(1)}}=(\frac{de}{dF^{(1)}}+\frac{de}{dH^{(1)}})\odot(-g_c^{(1)}+H^{(0)})
$$

### 2.2 关于
$$
g_c
$$
### 的梯度
$$
\frac{de}{dg_c^{(l)}}=\frac{de}{dF^{(l)}}\odot(1-g_u^{(l)})\\
\frac{de}{dg_c^{(l-1)}}=(\frac{de}{dF^{(l-1)}}+\frac{de}{dH^{(l-1)}})\odot(1-g_u^{(l-1)})\\
\frac{de}{dg_c^{(l-2)}}=(\frac{de}{dF^{(l-2)}}+\frac{de}{dH^{(l-2)}})\odot(1-g_u^{(l-2)})\\
\vdots\\
\frac{de}{dg_c^{(1)}}=(\frac{de}{dF^{(1)}}+\frac{de}{dH^{(1)}})\odot(1-g_u^{(1)})
$$

### 2.3 关于
$$
A_u, A_c
$$
### 的梯度 :
$$
\frac{de}{dA_u}=\frac{de}{dg_u}\odot g_u\odot(1-g_u)\\
\;\\
\frac{de}{dA_c}=\frac{de}{dg_c}\odot (1-g_c^2)\\
$$
这里不涉及迭代, 分步计算即可.
### 2.4 关于
$$
g_r, A_r
$$
### 的梯度
$$
\frac{de}{dg_r}=\frac{de}{dA_c}\odot (H^{(l-1)}V_c^T + b_c)\\
\;\\
\frac{de}{dA_r}=\frac{de}{dg_r}\odot g_r (1-g_r)\\
$$
这里不涉及迭代, 分步计算即可.
### 2.5 关于 H 的梯度
这里涉及迭代, 按顺序计算 :
$$
\frac{de}{dH^{(l-1)}}=\frac{de}{dF^{(l)}}\odot g_u^{(l)}+\frac{de}{dA_r^{(l)}}V_r+\frac{de}{dA_u^{(l)}}V_u+(\frac{de}{dA_c^{(l)}}\odot g_r^{(l)}) V_c\\
\frac{de}{dH^{(l-2)}}=(\frac{de}{dF^{(l-1)}}+\frac{de}{dH^{(l-1)}})\odot g_u^{(l-1)}+\frac{de}{dA_r^{(l-1)}}V_r+\frac{de}{dA_u^{(l-1)}}V_u+(\frac{de}{dA_c^{(l-1)}}\odot g_r^{(l-1)}) V_c\\
\frac{de}{dH^{(l-3)}}=(\frac{de}{dF^{(l-2)}}+\frac{de}{dH^{(l-2)}})\odot g_u^{(l-2)}+\frac{de}{dA_r^{(l-2)}}V_r+\frac{de}{dA_u^{(l-2)}}V_u+(\frac{de}{dA_c^{(l-2)}}\odot g_r^{(l-2)}) V_c\\
\vdots\\
\frac{de}{dH^{(0)}}=(\frac{de}{dF^{(1)}}+\frac{de}{dH^{(1)}})\odot g_u^{(1)}+\frac{de}{dA_r^{(1)}}V_r+\frac{de}{dA_u^{(1)}}V_u+(\frac{de}{dA_c^{(1)}}\odot g_r^{(1)}) V_c\\
$$

### 2.6 关于 X 的梯度
$$
\frac{de}{dX}=\frac{de}{dA_r}W_r+\frac{de}{dA_u}W_u+(\frac{de}{dA_c}\odot g_r) W_c
$$
这里不涉及迭代, 分步计算即可.
### 2.7 关于 W 的梯度
$$
\frac{de}{dW_r}=(\frac{de}{dA_r})^T X\\
\;\\
\frac{de}{dW_u}=(\frac{de}{dA_u})^T X\\
\;\\
\frac{de}{dW_c}=(\frac{de}{dA_c})^T X
$$
这里不涉及迭代, 分步计算即可.
### 2.8 关于 V 的梯度
$$
\frac{de}{dW_r}=(\frac{de}{dA_r})^T H^{(l-1)}\\
\;\\
\frac{de}{dW_u}=(\frac{de}{dA_u})^T H^{(l-1)}\\
\;\\
\frac{de}{dW_c}=(\frac{de}{dA_c}\odot g_r)^T H^{(l-1)}
$$
这里不涉及迭代, 分步计算即可.
### 2.9 关于 a,b 的梯度
$$
\frac{de}{da_r}=sum(\nabla (\frac{de}{dA_r})^T,\; axis=0)\\
\;\\
\frac{de}{da_u}=sum(\nabla (\frac{de}{dA_u})^T,\; axis=0)\\
\;\\
\frac{de}{da_c}=sum(\nabla (\frac{de}{dA_c})^T,\; axis=0)
$$
这里不涉及迭代, 分步计算即可. 同样的 :
$$
\frac{de}{da}=\frac{de}{db}
$$


