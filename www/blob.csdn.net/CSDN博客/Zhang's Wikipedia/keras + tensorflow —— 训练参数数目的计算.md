
# keras + tensorflow —— 训练参数数目的计算 - Zhang's Wikipedia - CSDN博客


2018年10月30日 21:42:08[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：471



## 1. RNN 模型
Embedding
Embedding(input_dim, output_dim,input_length)input_dim 表示字典的大小；
outpu_dim 则表示嵌入的维度；
训练参数的数目为：input_dim✖️output_dim；
model = Sequential()
model.add(Embedding(10000, 64, input_length=10))
input_array = np.random.randint(10000, size=(32, 10))
	\# batch_size 为 32，每一条样本的长度为 10，input_array 的最大值不超过 10000-1，
model.compile('rmsprop', 'mse')
output_array = model.predict(input_array)
assert output_array.shape == (32, 10, 64)
	\# 32 仍为 batch_size
	\# 10,64：则表示输入样本的每一维均映射为 64 维的嵌入向量；
SimpleRNN
[https://www.cnblogs.com/wdmx/p/9284037.html](https://www.cnblogs.com/wdmx/p/9284037.html)
SimpleRNN(units,input_shape)units：该层神经元的数目；
对于 RNN
$$
\begin{array}{l}
  s_t=\tanh(Ux_t+Ws_{t-1})\\
  o_t=\text{Softmax}(Vs_t)
  \end{array}
$$
则训练参数的数目为：dim(U)+dim(W)+dim(V)，如果隐层神经元的数目为 n，输出的维数为 k，输入的维度为 m，也即
$$
U_{n\times m}, W_{n\times n}, V_{n\times k}
$$
，训练参数的数目为：
$$
n^2+nm+kn
$$
，如果考虑到biases，还需加上
$$
n
$$
。
对于 SimpleRNN，计算其可训练参数时，并未计入到输出层的矩阵
$$
V
$$
，此时训练参数的数目为
$$
n^2+nm+n
$$
，
model = Sequential()
model.add(Embedding(1000, 64))
		\# 1000*64 ⇒ 64000
model.add(SimpleRNN(32))
		\# 64*32+32*32+32=3104
model.summary()[https://stackoverflow.com/questions/50134334/number-of-parameters-for-keras-simplernn](https://stackoverflow.com/questions/50134334/number-of-parameters-for-keras-simplernn)
GRU
$$
3 \times (n^2 + nm + n)
$$
3 倍于 SimpleRNN 参数的数目；
[https://stats.stackexchange.com/questions/328926/how-many-parameters-are-in-a-gated-recurrent-unit-gru-recurrent-neural-network](https://stats.stackexchange.com/questions/328926/how-many-parameters-are-in-a-gated-recurrent-unit-gru-recurrent-neural-network)
LSTM
input_dim: m, units: n
可训练参数数目为：4(nm+n+n^2)
model = Sequential()
model.add(LSTM(256, input_shape=(16, 4096)))
	\# 4*(4096*256+256+256**2)
model.summary()
## 2. CNN
Conv1D(None, 500, 128)，经过 (32, 7)（32 表示 filter 的数量，7 为 kernel size）的一维卷积之后的大小为：(None, 494, 32)，训练参数的数目为：712832+32 ⇒ 28704;


