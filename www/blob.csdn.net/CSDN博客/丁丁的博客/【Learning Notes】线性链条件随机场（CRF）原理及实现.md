
# 【Learning Notes】线性链条件随机场（CRF）原理及实现 - 丁丁的博客 - CSDN博客


2018年03月17日 21:40:20[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：1532



# 1. 概述
条件随机场（Conditional Random Field, CRF）是概率图模型（Probabilistic Graphical Model）与区分性分类( Discriminative Classification）的一种接合，能够用来对“结构预测”（structured prediction，e.g. 序列标注）问题进行建模。
如图1，论文 [1] 阐释了 CRF 与其他模型之间的关系。
![](http://ww3.sinaimg.cn/large/6cbb8645jw1en4lqd5qq3j21510p1jvn.jpg)[ ](http://ww3.sinaimg.cn/large/6cbb8645jw1en4lqd5qq3j21510p1jvn.jpg)
**图1. CRF 与 其他机器学习模型对比【****[src](http://www.hankcs.com/nlp/segment/crf-segmentation-of-the-pure-java-implementation.html)****】**
[
](http://ww3.sinaimg.cn/large/6cbb8645jw1en4lqd5qq3j21510p1jvn.jpg)本文我们重点关注输入结点独立的“线性链条件随机场”（Linear-Chain CRF）（如图2）的原理与实现。线性链 CRF 通过与双向 LSTM（Bi-LSTM）的接合，可以用来建模更一般的线性链 CRF（图3），提高模型的建模能力。
[
](http://ww3.sinaimg.cn/large/6cbb8645jw1en4lqd5qq3j21510p1jvn.jpg)![](https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=1645229592,3117226322&fm=27&gp=0.jpg)[ ](https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=1645229592,3117226322&fm=27&gp=0.jpg)
**图2. 简单的线性链 CRF【****[src](https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=1645229592,3117226322&fm=27&gp=0.jpg)****】**
[
](https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=1645229592,3117226322&fm=27&gp=0.jpg)![](https://avisingh599.github.io/images/kinect_activity/crf.png)[ ](https://avisingh599.github.io/images/kinect_activity/crf.png)
**图3. 一般性线性链 CRF【****[src](https://avisingh599.github.io/images/kinect_activity/crf.png)****】**
[

](https://avisingh599.github.io/images/kinect_activity/crf.png)
# 2.  CRF 算法
[

](https://avisingh599.github.io/images/kinect_activity/crf.png)
## 2.1 模型形式化
[
](https://avisingh599.github.io/images/kinect_activity/crf.png)给定长度为$m$的序列， 以及状态集$S$。 对于任意状态序列$(s_1,\cdots,s_m), s_i \in S$, 定义其“势”（potential）如下：

$$
\psi(s_1,\dots, s_m) = \prod_{i=1}^m\psi (s_{i−1}, s_i , i)
$$
我们定义$s_0$为特殊的开始符号$*$。这里对$s, s^\prime ∈ S, i \in {1 , \dots, m}$，势函数$\psi(s, s^\prime, i) \ge 0$。也即，势函数是非负的，它对序列第$i$位置发生的$s$到$s^\prime$的状态转移都给出一个非负值。
根据概率图模型的因子分解理论[1]，我们有：

$$
p(s_1,\dots,s_m|x_1,\dots, x_m) = \frac{\psi(s_1,\dots, s_m) }{\sum_{s^\prime_1,\dots,s^\prime_m} \psi(s^\prime_1,\dots, s^\prime_m)}
$$
$Z = \sum_{s^\prime_1,\dots,s^\prime_m} \psi(s^\prime_1,\dots, s^\prime_m)$为归一化因子。
同 HMM 类似，CRF 也涉及三类基本问题：评估（计算某一序列的似然值）、解码（给定输入，寻找似然最大的序列）及训练（根据数据估计 CRF 的参数），解决这三个问题也都涉及前向算法、后向算法及 Viterbi 算法。
CRF 的势函数类似于概率，只不过没有归一化，因此这里介绍的 CRF 前向算法、Viterbi 算法、后向算法，同 HMM 基本一致。
## 2.2 前向算法
定义：

$$
\alpha(i, s) = \sum_{s_1,\dots,s_{i-1}} \psi(s_1,\dots,s_{i-1}, s)
$$
表示，以$s$结尾的长度为$i$的子序列的势。
显然，$\alpha(1, s) = \psi(*, s_1, 1)$
根据定义，我们有如下递归关系：

$$
\alpha(i, s) = \sum_{s^\prime \in S} \alpha(i-1, s^\prime) \times \psi(s^\prime, s, i)
$$
归一化因子可以计算如下：

$$
Z = \sum_{s_1,\dots,s_m} \psi(s_1,\dots s_m) = \sum_{s\in S}\sum_{s_1,\dots,s_{m-1}} \psi(s_1,\dots s_{m-1}, s)= \sum_{s\in S} \alpha(m, s)
$$
对于给定的序列$(s_1,\cdots,s_m)$，其中条件概率（似然）可以计算：

$$
p(s_1,\dots,s_m|x_1,\dots, x_m) = \frac{\prod_{i=1}^m\psi (s_{i−1}, s_i , i)}{\sum_{s\in S} \alpha(m, s)}
$$
** 通过前向算法，我们解决了评估问题，计算和空间复杂度为**O(m\cdot|S|^2)**。**
> 似然的计算过程中，只涉及乘法和加法，都是可导操作。因此，只需要实现前向操作，我们就可以借具有自动梯度功能的学习库（e.g. pytorch、tensorflow）实现基于最大似然准则的训练。一个基于 pytorch 的 CRF 实现见
> [repo](https://github.com/DingKe/ml-tutorial/blob/master/crf/crf.py#L39)
> 。

```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
def
```
```python
forward
```
```python
(psi)
```
```python
:
```
```python
m, V, _ = psi.shape
    alpha = np.zeros([m, V])
    alpha[
```
```python
0
```
```python
] = psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, :]
```
```python
# assume psi[0, 0, :] := psi(*,s,1)
```
```python
for
```
```python
t
```
```python
in
```
```python
range(
```
```python
1
```
```python
, m):
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
'''
            for k in range(V):
                alpha[t, i] += alpha[t - 1, k] * psi[t, k, i]
            '''
```
```python
alpha[t, i] = np.sum(alpha[t -
```
```python
1
```
```python
, :] * psi[t, :, i])
```
```python
return
```
```python
alpha
```
```python
def
```
```python
pro
```
```python
(seq, psi)
```
```python
:
```
```python
m, V, _ = psi.shape
    alpha = forward(psi)
    Z = np.sum(alpha[-
```
```python
1
```
```python
])
    M = psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, seq[
```
```python
0
```
```python
]]
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
1
```
```python
, m):
        M *= psi[i, seq[i-
```
```python
1
```
```python
], seq[i]]
    p = M / Z
```
```python
return
```
```python
p
np.random.seed(
```
```python
1111
```
```python
)
V, m =
```
```python
5
```
```python
,
```
```python
10
```
```python
log_psi = np.random.random([m, V, V])
psi = np.exp(log_psi)
```
```python
# nonnegative
```
```python
seq = np.random.choice(V, m)
alpha = forward(psi)
p = pro(seq, psi)
print(p)
print(alpha)
```
`2.69869828108e-08
[[  1.10026295e+00   2.52187760e+00   1.40997704e+00   1.36407554e+00
    1.00201186e+00]
 [  1.27679086e+01   1.03890052e+01   1.44699134e+01   1.15244329e+01
    1.52767179e+01]
 [  9.30306192e+01   1.09450375e+02   1.26777728e+02   1.28529576e+02
    1.16835669e+02]
 [  9.81861108e+02   8.70384204e+02   9.35531558e+02   7.98228277e+02
    9.89225754e+02]
 [  6.89790063e+03   8.71016058e+03   8.84778486e+03   9.21051594e+03
    6.56093883e+03]
 [  7.56109978e+04   7.00773298e+04   8.60611103e+04   5.63567069e+04
    5.99238226e+04]
 [  6.69236243e+05   6.42107210e+05   7.81638452e+05   6.32533145e+05
    5.71122492e+05]
 [  6.62242340e+06   5.24446290e+06   5.54750409e+06   4.68782248e+06
    4.49353155e+06]
 [  4.31080734e+07   4.09579660e+07   4.62891972e+07   4.60100937e+07
    4.63083098e+07]
 [  2.66620185e+08   4.91942550e+08   4.48597546e+08   3.42214705e+08
    4.10510463e+08]]`
## 2.3 Viterbi 解码
Viterbi 利用动态规划，寻找似然最大的序列。Viterbi 与前向算法非常相似，只是将求和操作替换为最大值操作。

$$
\alpha(j, s) = \underset{s_1,\dots,s_{j-1}}{\mathrm{max}}\psi(s_1,\dots,s_{j-1}, s)
$$
显然，$\alpha(1, s) = \psi(*, s_1, 1)$
根据定义，我们有如下递归关系：

$$
\alpha(j, s) = \underset{s^\prime \in S}{\mathrm{max}}\  \alpha(j-1, s^\prime) \cdot \psi(s^\prime, s, j)
$$
在所有$|s|^m$条可能的序列中，概率最大的路径的未归一化的值为：

$$
\max \alpha(m, s)
$$
沿着前向推导的反方向，可以得到最优的路径，算法复杂度是$O(m \cdot |S|^2)$。demo 实现如下：
```python
def
```
```python
viterbi_1
```
```python
(psi)
```
```python
:
```
```python
m, V, _ = psi.shape
    alpha = np.zeros([V])
    trans = np.ones([m, V]).astype(
```
```python
'int'
```
```python
) * -
```
```python
1
```
```python
alpha[:] = psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, :]
```
```python
# assume psi[0, 0, :] := psi(*,s,1)
```
```python
for
```
```python
t
```
```python
in
```
```python
range(
```
```python
1
```
```python
, m):
        next_alpha = np.zeros([V])
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
            tmp = alpha * psi[t, :, i]
            next_alpha[i] = np.max(tmp)
            trans[t, i] = np.argmax(tmp)
        alpha = next_alpha
    end = np.argmax(alpha)
    path = [end]
```
```python
for
```
```python
t
```
```python
in
```
```python
range(m -
```
```python
1
```
```python
,
```
```python
0
```
```python
, -
```
```python
1
```
```python
):
        cur = path[-
```
```python
1
```
```python
]
        pre = trans[t, cur]
        path.append(pre)
```
```python
return
```
```python
path[::-
```
```python
1
```
```python
]
```
```python
def
```
```python
viterbi_2
```
```python
(psi)
```
```python
:
```
```python
m, V, _ = psi.shape
    alpha = np.zeros([m, V])
    alpha[
```
```python
0
```
```python
] = psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, :]
```
```python
# assume psi[0, 0, :] := psi(*,s,1)
```
```python
for
```
```python
t
```
```python
in
```
```python
range(
```
```python
1
```
```python
, m):
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
            tmp = alpha[t -
```
```python
1
```
```python
, :] * psi[t, :, i]
            alpha[t, i] = np.max(tmp)
    end = np.argmax(alpha[-
```
```python
1
```
```python
])
    path = [end]
```
```python
for
```
```python
t
```
```python
in
```
```python
range(m -
```
```python
1
```
```python
,
```
```python
0
```
```python
, -
```
```python
1
```
```python
):
        cur = path[-
```
```python
1
```
```python
]
        pre = np.argmax(alpha[t -
```
```python
1
```
```python
] * psi[t, :, cur])
        path.append(pre)
```
```python
return
```
```python
path[::-
```
```python
1
```
```python
]
np.random.seed(
```
```python
1111
```
```python
)
V, m =
```
```python
5
```
```python
,
```
```python
10
```
```python
log_psi = np.random.random([m, V, V])
psi = np.exp(log_psi)
```
```python
# nonnegative
```
```python
path_1 = viterbi_1(psi)
path_2 = viterbi_2(psi)
print(path_1)
print(path_2)
```
`[1, 4, 2, 4, 3, 0, 3, 0, 3, 1]
[1, 4, 2, 4, 3, 0, 3, 0, 3, 1]`
## 2.4 后向算法
为了训练 CRF, 我们需要计算相应的梯度。为了手动计算梯度（这也为后续优化打开大门），需要用到后向算法。
定义：

$$
\beta(j, s) = \sum_{s_{j+1},\dots, s_m} \psi(s_{j+1},\dots, s_m|s_j=s)
$$
其中，令$\beta(m, s) = 1$。
> 可以认为序列结尾存在特殊的符号。为简单起见，不讨论结尾边界的特殊性，可以都参考前向边界的处理及参见
> [实现](https://github.com/DingKe/ml-tutorial/blob/master/crf/crf.py#L154)
> 。
根据定义，我们有如下递归关系：

$$
\beta(j, s) = \sum_{s^\prime \in S} \beta(j+1, s^\prime) \cdot \psi(s, s^\prime, j+1)
$$

```python
def
```
```python
backward
```
```python
(psi)
```
```python
:
```
```python
m, V, _ = psi.shape
    beta = np.zeros([m, V])
    beta[-
```
```python
1
```
```python
] =
```
```python
1
```
```python
for
```
```python
t
```
```python
in
```
```python
range(m -
```
```python
2
```
```python
, -
```
```python
1
```
```python
, -
```
```python
1
```
```python
):
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
'''
            for k in range(V):
                beta[t, i] += beta[t + 1, k] * psi[t + 1, i, k]
            '''
```
```python
beta[t, i] = np.sum(beta[t +
```
```python
1
```
```python
, :] * psi[t +
```
```python
1
```
```python
, i, :])
```
```python
return
```
```python
beta
np.random.seed(
```
```python
1111
```
```python
)
V, m =
```
```python
5
```
```python
,
```
```python
10
```
```python
log_psi = np.random.random([m, V, V])
psi = np.exp(log_psi)
```
```python
# nonnegative
```
```python
seq = np.random.choice(V, m)
beta = backward(psi)
print(beta)
```
`[[  2.95024144e+08   2.61620644e+08   3.16953747e+08   2.02959597e+08
    2.51250862e+08]
 [  2.73494359e+07   2.31521489e+07   3.62404054e+07   2.84752625e+07
    3.38820012e+07]
 [  2.92799244e+06   3.00539203e+06   4.18174216e+06   3.30814155e+06
    3.45104724e+06]
 [  4.40588351e+05   4.18060894e+05   3.95721271e+05   4.50117410e+05
    4.38635065e+05]
 [  4.51172884e+04   5.40496888e+04   4.37931199e+04   4.98898498e+04
    5.04357771e+04]
 [  6.50740169e+03   5.21859026e+03   5.66773856e+03   4.73895449e+03
    5.79578682e+03]
 [  4.83173340e+02   5.36538120e+02   6.01820173e+02   7.07538756e+02
    6.54966046e+02]
 [  7.60936291e+01   7.90609361e+01   9.08681883e+01   5.80503199e+01
    5.89976569e+01]
 [  8.15414542e+00   7.95904764e+00   9.64664115e+00   8.69502743e+00
    9.41073532e+00]
 [  1.00000000e+00   1.00000000e+00   1.00000000e+00   1.00000000e+00
    1.00000000e+00]]`
## 2.5 梯度计算

$$
Z = \sum_{s_1,\dots,s_m} \psi(s_1,\dots, s_m) = \sum_{s^\prime_{i-1} \in S, s^\prime_i \in S} \sum_{s_{i-1}=s^\prime_{i-1}, s_i=s^\prime_i} \psi(s_1,\dots, s_m) = \sum_{s^\prime_{i-1} \in S, s^\prime_i \in S} \alpha(i-1, s^\prime_{i-1}) \cdot \beta(i, s^\prime_i) \cdot \psi (s^\prime_{i−1}, s^\prime_i , i) \ \ \ 1 < i \le m
$$
对于$i = 1$的边界情况：

$$
Z = \sum_{s^\prime_1 \in S} \beta(1, s^\prime_i) \cdot \psi (*, s^\prime_1 , 1)
$$
对于路径$(s_1, \cdots, s_m)$，

$$
p(s_1,\dots,s_m|x_1,\dots, x_m) = \frac{\psi(s_1,\dots, s_m)}{Z} = \frac{\prod_{i=1}^m\psi (s_{i−1}, s_i , i)}{Z} = \frac{\prod_{i=1}^m\psi^i_{s_{i−1}, s_i}}{Z}
$$
其中，$\psi^i_{s^\prime, s} = \psi(s^\prime, s, i),\ s^\prime, s \in S$。
记分子$\prod_{i=1}^m\psi (s_{i−1}, s_i , i) = M$则：

$$
\frac{\partial p(s_1,\dots,s_m|x_1,\dots, x_m)}{\partial \psi^k_{s^\prime, s}} = \frac{1}{Z} [ \frac{M}{\psi^k_{s^\prime, s}} \cdot \delta_{s^\prime = s_{k-1} \&s = s_k} - p \cdot \alpha(k-1, s^\prime) \cdot \beta(k, s)]
$$
其中，$\delta_{true} = 1, \delta_{false} = 0$。

$$
\frac{\partial \ln p(s_1,\dots,s_m|x_1,\dots, x_m)}{\partial \psi^k_{s^\prime, s}}  = \frac{1}{p} \cdot \frac{\partial p(s_1,\dots,s_m|x_1,\dots, x_m)}{\partial \psi^k_{s^\prime, s}} = \frac{\delta_{s^\prime = s_{k-1} \&amp;s = s_k}}{\psi^k_{s^\prime, s}} - \frac{1}{Z} \alpha(k-1, s^\prime) \cdot \beta(k, s)
$$

```python
def
```
```python
gradient
```
```python
(seq, psi)
```
```python
:
```
```python
m, V, _ = psi.shape
    grad = np.zeros_like(psi)
    alpha = forward(psi)
    beta = backward(psi)
    Z = np.sum(alpha[-
```
```python
1
```
```python
])
```
```python
for
```
```python
t
```
```python
in
```
```python
range(
```
```python
1
```
```python
, m):
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
for
```
```python
j
```
```python
in
```
```python
range(V):
                grad[t, i, j] = -alpha[t -
```
```python
1
```
```python
, i] * beta[t, j] / Z
```
```python
if
```
```python
i == seq[t -
```
```python
1
```
```python
]
```
```python
and
```
```python
j == seq[t]:
                    grad[t, i, j] +=
```
```python
1.
```
```python
/ psi[t, i, j]
```
```python
# corner cases
```
```python
grad[
```
```python
0
```
```python
,
```
```python
0
```
```python
, :] = -beta[
```
```python
0
```
```python
, :] / Z
    grad[
```
```python
0
```
```python
,
```
```python
0
```
```python
, seq[
```
```python
0
```
```python
]] +=
```
```python
1.
```
```python
/ psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, seq[
```
```python
0
```
```python
]]
```
```python
return
```
```python
grad
np.random.seed(
```
```python
1111
```
```python
)
V, m =
```
```python
5
```
```python
,
```
```python
10
```
```python
log_psi = np.random.random([m, V, V])
psi = np.exp(log_psi)
```
```python
# nonnegative
```
```python
seq = np.random.choice(V, m)
grad = gradient(seq, psi)
print(grad[
```
```python
0
```
```python
, :, :])
```
`[[ 0.75834232 -0.13348772 -0.16172055 -0.10355687 -0.12819671]
 [ 0.          0.          0.          0.          0.        ]
 [ 0.          0.          0.          0.          0.        ]
 [ 0.          0.          0.          0.          0.        ]
 [ 0.          0.          0.          0.          0.        ]]`
```python
def
```
```python
check_grad
```
```python
(seq, psi, i, j, k, toleration=
```
```python
1e-5
```
```python
, delta=
```
```python
1e-10
```
```python
)
```
```python
:
```
```python
m, V, _ = psi.shape
    grad_1 = gradient(seq, psi)[i, j, k]
    original = psi[i, j, k]
```
```python
# p1
```
```python
psi[i, j, k] = original - delta
    p1 = np.log(pro(seq, psi))
```
```python
# p2
```
```python
psi[i, j, k] = original + delta
    p2 = np.log(pro(seq, psi))
    psi[i, j, k] = original
    grad_2 = (p2 - p1) / (
```
```python
2
```
```python
* delta)
    diff = np.abs(grad_1 - grad_2)
```
```python
if
```
```python
diff > toleration:
        print(
```
```python
"%d, %d, %d, %.2e, %.2e, %.2e"
```
```python
% (i, j, k, grad_1, grad_2, diff))
np.random.seed(
```
```python
1111
```
```python
)
V, m =
```
```python
5
```
```python
,
```
```python
10
```
```python
log_psi = np.random.random([m, V, V])
psi = np.exp(log_psi)
```
```python
# nonnegative
```
```python
seq = np.random.choice(V, m)
print(seq)
```
```python
for
```
```python
toleration
```
```python
in
```
```python
[
```
```python
1e-4
```
```python
,
```
```python
5e-5
```
```python
,
```
```python
1.5e-5
```
```python
]:
    print(toleration)
```
```python
for
```
```python
i
```
```python
in
```
```python
range(m):
```
```python
for
```
```python
j
```
```python
in
```
```python
range(V):
```
```python
for
```
```python
k
```
```python
in
```
```python
range(V):
                check_grad(seq, psi, i, j, k, toleration)
```
`[0 1 4 1 3 0 0 3 3 1]
0.0001
5e-05
1.5e-05
2, 1, 2, -2.22e-02, -2.22e-02, 1.55e-05
4, 3, 3, -2.03e-02, -2.03e-02, 1.55e-05`首先定义基本的 log 域加法操作（[参见](https://github.com/DingKe/ml-tutorial/blob/master/ctc/CTC.ipynb)）。
```python
ninf = -np.float(
```
```python
'inf'
```
```python
)
```
```python
def
```
```python
_logsumexp
```
```python
(a, b)
```
```python
:
```
```python
'''
    np.log(np.exp(a) + np.exp(b))
    '''
```
```python
if
```
```python
a < b:
        a, b = b, a
```
```python
if
```
```python
b == ninf:
```
```python
return
```
```python
a
```
```python
else
```
```python
:
```
```python
return
```
```python
a + np.log(
```
```python
1
```
```python
+ np.exp(b - a))
```
```python
def
```
```python
logsumexp
```
```python
(*args)
```
```python
:
```
```python
'''
    from scipy.special import logsumexp
    logsumexp(args)
    '''
```
```python
res = args[
```
```python
0
```
```python
]
```
```python
for
```
```python
e
```
```python
in
```
```python
args[
```
```python
1
```
```python
:]:
        res = _logsumexp(res, e)
```
```python
return
```
```python
res
```
```python
def
```
```python
forward_log
```
```python
(log_psi)
```
```python
:
```
```python
m, V, _ = log_psi.shape
    log_alpha = np.ones([m, V]) * ninf
    log_alpha[
```
```python
0
```
```python
] = log_psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, :]
```
```python
# assume psi[0, 0, :] := psi(*,s,1)
```
```python
for
```
```python
t
```
```python
in
```
```python
range(
```
```python
1
```
```python
, m):
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
for
```
```python
j
```
```python
in
```
```python
range(V):
                log_alpha[t, j] = logsumexp(log_alpha[t, j], log_alpha[t -
```
```python
1
```
```python
, i] + log_psi[t, i, j])
```
```python
return
```
```python
log_alpha
```
```python
def
```
```python
pro_log
```
```python
(seq, log_psi)
```
```python
:
```
```python
m, V, _ = log_psi.shape
    log_alpha = forward_log(log_psi)
    log_Z = logsumexp(*[e
```
```python
for
```
```python
e
```
```python
in
```
```python
log_alpha[-
```
```python
1
```
```python
]])
    log_M = log_psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, seq[
```
```python
0
```
```python
]]
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
1
```
```python
, m):
        log_M = log_M + log_psi[i, seq[i -
```
```python
1
```
```python
], seq[i]]
    log_p = log_M - log_Z
```
```python
return
```
```python
log_p
np.random.seed(
```
```python
1111
```
```python
)
V, m =
```
```python
5
```
```python
,
```
```python
10
```
```python
log_psi = np.random.random([m, V, V])
psi = np.exp(log_psi)
```
```python
# nonnegative
```
```python
seq = np.random.choice(V, m)
alpha = forward(psi)
log_alpha = forward_log(log_psi)
print(np.sum(np.abs(np.log(alpha) - log_alpha)))
p = pro(seq, psi)
log_p = pro_log(seq, log_psi)
print(np.sum(np.abs(np.log(p) - log_p)))
```
`3.03719722983e-14
0.0`
```python
def
```
```python
backward_log
```
```python
(log_psi)
```
```python
:
```
```python
m, V, _ = log_psi.shape
    log_beta = np.ones([m, V]) * ninf
    log_beta[-
```
```python
1
```
```python
] =
```
```python
0
```
```python
for
```
```python
t
```
```python
in
```
```python
range(m -
```
```python
2
```
```python
, -
```
```python
1
```
```python
, -
```
```python
1
```
```python
):
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
for
```
```python
j
```
```python
in
```
```python
range(V):
                log_beta[t, i] = logsumexp(log_beta[t, i], log_beta[t +
```
```python
1
```
```python
, j] + log_psi[t +
```
```python
1
```
```python
, i, j])
```
```python
return
```
```python
log_beta
np.random.seed(
```
```python
1111
```
```python
)
V, m =
```
```python
5
```
```python
,
```
```python
10
```
```python
log_psi = np.random.random([m, V, V])
psi = np.exp(log_psi)
```
```python
# nonnegative
```
```python
seq = np.random.choice(V, m)
beta = backward(psi)
log_beta = backward_log(log_psi)
print(np.sum(np.abs(beta - np.exp(log_beta))))
print(np.sum(np.abs(log_beta - np.log(beta))))
```
`1.46851337579e-06
1.86517468137e-14`
```python
def
```
```python
gradient_log
```
```python
(seq, log_psi)
```
```python
:
```
```python
m, V, _ = log_psi.shape
    grad = np.zeros_like(log_psi)
    log_alpha = forward_log(log_psi)
    log_beta = backward_log(log_psi)
    log_Z = logsumexp(*[e
```
```python
for
```
```python
e
```
```python
in
```
```python
log_alpha[-
```
```python
1
```
```python
]])
```
```python
for
```
```python
t
```
```python
in
```
```python
range(
```
```python
1
```
```python
, m):
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
for
```
```python
j
```
```python
in
```
```python
range(V):
                grad[t, i, j] -= np.exp(log_alpha[t -
```
```python
1
```
```python
, i] + log_beta[t, j] - log_Z)
```
```python
if
```
```python
i == seq[t -
```
```python
1
```
```python
]
```
```python
and
```
```python
j == seq[t]:
                    grad[t, i, j] += np.exp(-log_psi[t, i, j])
```
```python
# corner cases
```
```python
grad[
```
```python
0
```
```python
,
```
```python
0
```
```python
, :] -= np.exp(log_beta[
```
```python
0
```
```python
, :] - log_Z)
    grad[
```
```python
0
```
```python
,
```
```python
0
```
```python
, seq[
```
```python
0
```
```python
]] += np.exp(-log_psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, seq[
```
```python
0
```
```python
]])
```
```python
return
```
```python
grad
np.random.seed(
```
```python
1111
```
```python
)
V, m =
```
```python
5
```
```python
,
```
```python
10
```
```python
log_psi = np.random.random([m, V, V])
psi = np.exp(log_psi)
```
```python
# nonnegative
```
```python
seq = np.random.choice(V, m)
grad_1 = gradient(seq, psi)
grad_2 = gradient_log(seq, log_psi)
print(grad_1[
```
```python
0
```
```python
, :, :])
print(grad_2[
```
```python
0
```
```python
, :, :])
print(np.sum(np.abs(grad_1 - grad_2)))
```
`[[ 0.75834232 -0.13348772 -0.16172055 -0.10355687 -0.12819671]
 [ 0.          0.          0.          0.          0.        ]
 [ 0.          0.          0.          0.          0.        ]
 [ 0.          0.          0.          0.          0.        ]
 [ 0.          0.          0.          0.          0.        ]]
[[ 0.75834232 -0.13348772 -0.16172055 -0.10355687 -0.12819671]
 [ 0.          0.          0.          0.          0.        ]
 [ 0.          0.          0.          0.          0.        ]
 [ 0.          0.          0.          0.          0.        ]
 [ 0.          0.          0.          0.          0.        ]]
1.11508025036e-14`在 log 域， 我们一般直接计算目标函数相对与$\ln\psi$的梯度计算公式如下：

$$
\frac{\partial \ln p(s_1,\dots,s_m|x_1,\dots, x_m)}{\partial \ln \psi^k_{s^\prime, s}}  =
\frac{\partial \ln p(s_1,\dots,s_m|x_1,\dots, x_m)}{\partial \psi^k_{s^\prime, s}} \cdot \frac{\partial \psi^k_{s^\prime, s}}{\partial \ln \psi^k_{s^\prime, s}} = \delta_{s^\prime = s_{k-1} \&s = s_k} - \exp(\ln\alpha(k-1, s^\prime) + \ln \beta(k, s) - \ln Z + \ln\psi^k_{s^\prime, s})
$$
只需将上面的 grad_log 稍做改动即可，不再赘述。
# 3. CRF + 人工神经网络
## 3.1 势函数选择
目前为止，我们都假设函数已经知道，在此基础上推导 CRF 的相关计算。理论上，除了非负性的要求 ，势函数可以灵活的选择。为也便于计算和训练，CRF 中一般选择指数的形式。假设输入为$x_1,\dots,x_m$，则势函数定义为：

$$
\psi(s^\prime, s, i) = \exp(w \cdot \phi(x_1,\dots,x_m, s^\prime, s, i))
$$
则

$$
\psi(s_1,\dots, s_m) = \prod_{i=1}^m\psi (s_{i−1}, s_i , i) = \prod_{i=1}^m\exp(w \cdot \phi(x_1,\dots,x_m, s_{i-1}, s_i,i))
$$
其中，$\phi(x_1,\dots,x_m, s^\prime, s, i) \in R^d$是特征向量，$w \in R^d$是参数向量。
对于线性链模型，简化势函数为：

$$
\psi(s^\prime, s, i) = t(s|s^\prime)e(s|x_i)
$$
转移势函数定义为：

$$
t(s|s^\prime) = \exp(v \cdot g(s^\prime, s))
$$
发射势函数定义为：

$$
e(s|x_i) = \exp(w \cdot f(s, x_i))
$$
则：

$$
\psi(s_1,\dots, s_m) = \prod_{j=1}^m\psi (s_{j−1}, s_j , j) = \prod_{j=1}^m t(s_j|s_{j-1})e(s | x_j) = \prod_{j=1}^m  \exp(v \cdot g(s_{j-1}, s_j)) \cdot \exp(w \cdot f(s_j, x_j))
$$

$$
\psi(s_1,\dots, s_m) = \exp(\sum_{i=1}^m v \cdot g(s_{i-1}, s_i) + \sum_{i=1}^m w \cdot f(s_i, x_i))
$$
如果我们取对数，则我们得到一个线性模型，定义：

$$
score_t(s|s^\prime) = \log t(s|s^\prime) = v \cdot g(s^\prime, s)
$$

$$
score_e(s|x_i) = \log e(s|x_i) = w \cdot f(s, x_i)
$$
则

$$
\log \psi(s_1,\dots, s_m) = \sum_{i=1}^m v \cdot g(s_{i-1}, s_i) + \sum_{i=1}^m w \cdot f(s_i, x_i) = \sum_{i=1}^m  score_t(s_{i-1}|s_i) + \sum_{i=1}^m score_e(s_i|x_i)
$$
具体的，可以定义

$$
score_t(s_j|s_i) = P_{ij}
$$
其中，$P$是$|S| \times |S|$的转移矩阵。
如果$x = (x_1,\cdots,x_m) \in R^m$，则有：

$$
score_e(s_j|x_i) = W_j \cdot x_i
$$
其中，$W \in R^{|s| \times n}$是权重矩阵。

$$
\log \psi(s_1,\dots, s_m) = \sum_{i=1}^m  score_t(s|s^\prime) + \sum_{i=1}^m score_e(s|x_i) = \sum_{i=1}^m P_{s_{i-1} s_{i}} +  \sum_{i=1}^m W_{s_i} \cdot x_i
$$
这里，为简单起见，我们令$x_i$是一个标量，实际中$x_i$往往是向量。
从$x$到$\log\psi$再到$\psi$都是可导的操作（四则运算和指数、对数运算），而$\psi$的梯度我们上面已经推导可以求得。因此，我们可以利用误差反传计算$W$等参数的梯度，从而利用 SGD 等优化方法训练包括 CRF 在内的整个模型的参数。
```python
def
```
```python
score
```
```python
(seq, x, W, P, S)
```
```python
:
```
```python
m = len(seq)
    V = len(W)
    log_psi = np.zeros([m, V, V])
```
```python
# corner cases
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
# emit
```
```python
log_psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, i] += S[i]
```
```python
# transmit
```
```python
log_psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, i] += x[
```
```python
0
```
```python
] * W[i]
```
```python
for
```
```python
t
```
```python
in
```
```python
range(
```
```python
1
```
```python
, m):
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
for
```
```python
j
```
```python
in
```
```python
range(V):
```
```python
# emit
```
```python
log_psi[t, i, j] += x[t] * W[j]
```
```python
# transmit
```
```python
log_psi[t, i, j] += P[i, j]
```
```python
return
```
```python
log_psi
```
```python
def
```
```python
gradient_param
```
```python
(seq, x, W, P, S)
```
```python
:
```
```python
m = len(seq)
    V = len(W)
    log_psi = score(seq, x, W, P, S)
    grad_psi = gradient_log(seq, log_psi)
    grad_log_psi = np.exp(log_psi) * grad_psi
    grad_x = np.zeros_like(x)
    grad_W = np.zeros_like(W)
    grad_P = np.zeros_like(P)
    grad_S = np.zeros_like(S)
```
```python
# corner cases
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
# emit
```
```python
grad_S[i] += grad_log_psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, i]
```
```python
# transmit
```
```python
grad_W[i] += grad_log_psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, i] * x[
```
```python
0
```
```python
]
        grad_x[
```
```python
0
```
```python
] += grad_log_psi[
```
```python
0
```
```python
,
```
```python
0
```
```python
, i] * W[i]
```
```python
for
```
```python
t
```
```python
in
```
```python
range(
```
```python
1
```
```python
, m):
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
for
```
```python
j
```
```python
in
```
```python
range(V):
```
```python
# emit
```
```python
grad_W[j] += grad_log_psi[t, i, j] * x[t]
                grad_x[t] += grad_log_psi[t, i, j] * W[j]
```
```python
# transmit
```
```python
grad_P[i, j] += grad_log_psi[t, i, j]
```
```python
return
```
```python
grad_x, grad_W, grad_P, grad_S
np.random.seed(
```
```python
1111
```
```python
)
V, m =
```
```python
5
```
```python
,
```
```python
7
```
```python
seq = np.random.choice(V, m)
x = np.random.random(m)
W = np.random.random(V)
P = np.random.random([V, V])
S = np.random.random(V)
grad_x, grad_W, grad_P, grad_S = gradient_param(seq, x, W, P, S)
print(grad_x)
print(grad_W)
print(grad_P)
print(grad_S)
```
`[ 0.03394788 -0.11666261  0.02592661  0.07931277  0.02549323  0.11371901
  0.02198856]
[-0.62291675 -0.38050215 -0.18983737 -0.65300231  1.84625859]
[[-0.34655117 -0.27314013 -0.16800195 -0.28352514  0.73359469]
 [-0.22747135 -0.2967193  -0.27009443 -0.2664594   0.87349324]
 [-0.27906702 -0.27747362 -0.33689934 -0.18786182  0.82788735]
 [-0.2701056  -0.16940564 -0.2624276  -0.29133856 -0.25558298]
 [ 0.72105085  0.86080584  0.76931185 -0.2103895  -0.11362927]]
[-0.17736447 -0.21489701 -0.20747999 -0.19735031  0.79709179]`梯度正确性检验如下：
```python
def
```
```python
check_grad
```
```python
(seq, x, W, P, S, toleration=
```
```python
1e-5
```
```python
, delta=
```
```python
1e-10
```
```python
)
```
```python
:
```
```python
m, V, _ = psi.shape
    grad_x, grad_W, grad_P, grad_S = gradient_param(seq, x, W, P, S)
```
```python
def
```
```python
llk
```
```python
(seq, x, W, P, S)
```
```python
:
```
```python
log_psi = score(seq, x, W, P, S)
        spi = np.exp(log_psi)
        log_p = np.log(pro(seq, spi))
```
```python
return
```
```python
log_p
```
```python
# grad_x
```
```python
print(
```
```python
'Check X'
```
```python
)
```
```python
for
```
```python
i
```
```python
in
```
```python
range(len(x)):
        original = x[i]
        grad_1 = grad_x[i]
```
```python
# p1
```
```python
x[i] = original - delta
        p1 = llk(seq, x, W, P, S)
```
```python
# p2
```
```python
x[i] = original + delta
        p2 = llk(seq, x, W, P, S)
        x[i] = original
        grad_2 = (p2 - p1) / (
```
```python
2
```
```python
* delta)
        diff = np.abs(grad_1 - grad_2) / np.abs(grad_2)
```
```python
if
```
```python
diff > toleration:
            print(
```
```python
"%d, %.2e, %.2e, %.2e"
```
```python
% (i, grad_1, grad_2, diff))
```
```python
# grad_W
```
```python
print(
```
```python
'Check W'
```
```python
)
```
```python
for
```
```python
i
```
```python
in
```
```python
range(len(W)):
        original = W[i]
        grad_1 = grad_W[i]
```
```python
# p1
```
```python
W[i] = original - delta
        p1 = llk(seq, x, W, P, S)
```
```python
# p2
```
```python
W[i] = original + delta
        p2 = llk(seq, x, W, P, S)
        W[i] = original
        grad_2 = (p2 - p1) / (
```
```python
2
```
```python
* delta)
        diff = np.abs(grad_1 - grad_2) / np.abs(grad_2)
```
```python
if
```
```python
diff > toleration:
            print(
```
```python
"%d, %.2e, %.2e, %.2e"
```
```python
% (i, grad_1, grad_2, diff))
```
```python
# grad_P
```
```python
print(
```
```python
'Check P'
```
```python
)
```
```python
for
```
```python
i
```
```python
in
```
```python
range(V):
```
```python
for
```
```python
j
```
```python
in
```
```python
range(V):
            original = P[i][j]
            grad_1 = grad_P[i][j]
```
```python
# p1
```
```python
P[i][j] = original - delta
            p1 = llk(seq, x, W, P, S)
```
```python
# p2
```
```python
P[i][j] = original + delta
            p2 = llk(seq, x, W, P, S)
            P[i][j] = original
            grad_2 = (p2 - p1) / (
```
```python
2
```
```python
* delta)
            diff = np.abs(grad_1 - grad_2) / np.abs(grad_2)
```
```python
if
```
```python
diff > toleration:
                print(
```
```python
"%d, %.2e, %.2e, %.2e"
```
```python
% (i, grad_1, grad_2, diff))
```
```python
# grad_S
```
```python
print(
```
```python
'Check S'
```
```python
)
```
```python
for
```
```python
i
```
```python
in
```
```python
range(len(S)):
        original = S[i]
        grad_1 = grad_S[i]
```
```python
# p1
```
```python
S[i] = original - delta
        p1 = llk(seq, x, W, P, S)
```
```python
# p2
```
```python
S[i] = original + delta
        p2 = llk(seq, x, W, P, S)
        S[i] = original
        grad_2 = (p2 - p1) / (
```
```python
2
```
```python
* delta)
        diff = np.abs(grad_1 - grad_2) / np.abs(grad_2)
```
```python
if
```
```python
diff > toleration:
            print(
```
```python
"%d, %.2e, %.2e, %.2e"
```
```python
% (i, grad_1, grad_2, diff))
np.random.seed(
```
```python
1111
```
```python
)
V, m =
```
```python
5
```
```python
,
```
```python
10
```
```python
seq = np.random.choice(V, m)
x = np.random.random(m)
W = np.random.random(V)
P = np.random.random([V, V])
S = np.random.random(V)
check_grad(seq, x, W, P, S)
```
`Check X
1, 6.75e-02, 6.75e-02, 5.74e-05
2, 5.14e-01, 5.14e-01, 1.47e-05
3, -3.17e-01, -3.17e-01, 1.51e-05
5, -6.42e-02, -6.42e-02, 7.82e-05
8, -4.38e-02, -4.38e-02, 1.08e-04
Check W
0, -6.55e-01, -6.55e-01, 1.13e-05
2, -1.33e-03, -1.33e-03, 3.77e-04
3, 5.88e-02, 5.89e-02, 1.15e-04
Check P
0, -4.50e-01, -4.51e-01, 1.03e-05
0, -2.70e-01, -2.70e-01, 2.53e-05
1, -2.11e-01, -2.11e-01, 3.13e-05
1, -2.35e-01, -2.35e-01, 1.80e-05
2, -2.93e-01, -2.93e-01, 1.76e-05
2, -1.50e-01, -1.50e-01, 2.15e-05
2, -1.72e-01, -1.72e-01, 3.40e-05
2, -3.48e-01, -3.48e-01, 1.02e-05
3, -1.90e-01, -1.90e-01, 3.10e-05
3, -3.60e-01, -3.60e-01, 1.78e-05
4, 5.47e-01, 5.47e-01, 1.50e-05
Check S
0, -2.02e-01, -2.02e-01, 2.13e-05
1, -1.97e-01, -1.97e-01, 1.82e-05
2, -1.05e-01, -1.05e-01, 6.22e-05`
## 3.2 Bi-LSTM + CRF
CRF 是强大的序列学习准则。配合双向循环神经网络（e.g. Bi-LSTM）的特征表征和学习能力，在许多序列学习任务上都取得了领先的结果[5~7]。
基本模型如下：
![](https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=2428423495,1906169408&fm=27&gp=0.jpg)[ ](https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=2428423495,1906169408&fm=27&gp=0.jpg)
**图4. Bi-LSTM CRF 模型【****[src](https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=2428423495,1906169408&fm=27&gp=0.jpg)****】**
[
](https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=2428423495,1906169408&fm=27&gp=0.jpg)Bi-LSTM 对整个输入序列进行特征提取和建模，用非线性的模型建模发射得分；转移得分用另外的$P$表示，作为 CRF 自身的参数。相对于常规的用于神经网络训练的目标函数，CRF 是带参数的损失函数。
[
](https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=2428423495,1906169408&fm=27&gp=0.jpg)基于 pytorch 的 CRFLoss 实现见[repo](https://github.com/DingKe/ml-tutorial/blob/master/crf/crf.py)以及[3, 4]，BiLSTM + CRF 的 实现应用见[8]。
[

](https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=2428423495,1906169408&fm=27&gp=0.jpg)
# 讨论
[
](https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=2428423495,1906169408&fm=27&gp=0.jpg)CRF 广泛应用于序列标注任务。
由于是区分性模型，因此在分类任务上，CRF 相比 HMM 可能会更高效。
CRF 对于输出之间的关系进行了建模，这不同于 直接的 RNN 或 CTC 模型（相应的，CRF 训练和预测的计算量也更大）。
[

](https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=2428423495,1906169408&fm=27&gp=0.jpg)
# References
[
](https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=2428423495,1906169408&fm=27&gp=0.jpg)Sutton and McCuallum.[An Introduction to Conditional Random Fields](http://homepages.inf.ed.ac.uk/csutton/publications/crftut-fnt.pdf).
Michael Collins.[The Forward-Backward Algorithm](http://www.cs.columbia.edu/~mcollins/fb.pdf).
[Pytorch CRF Forward and Viterbi Implementation](http://pytorch.org/tutorials/beginner/nlp/advanced_tutorial.html).
[BiLSTM-CRF on PyTorch](https://github.com/kaniblu/pytorch-bilstmcrf).
Collobert.[Deep Learning for Efficient Discriminative Parsing](http://ronan.collobert.com/pub/matos/2011_parsing_aistats.pdf).
Collobert et al.[Natural Language Processing (Almost) from Scratch](http://ronan.collobert.com/pub/matos/2011_nlp_jmlr.pdf).
Huang et al.[Bidirectional LSTM-CRF Models for Sequence Tagging](https://arxiv.org/abs/1508.01991).
[Bi-LSTM-CRF for NLP](https://github.com/UKPLab/emnlp2017-bilstm-cnn-crf).
[            ](https://ss1.bdstatic.com/70cFuXSh_Q1YnxGkpoWK1HF6hhy/it/u=2428423495,1906169408&fm=27&gp=0.jpg)

