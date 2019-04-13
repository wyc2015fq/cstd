
# 【Learning Notes】CTC 原理及实现 - 丁丁的博客 - CSDN博客


2018年03月05日 09:47:32[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：5050


CTC（ Connectionist Temporal Classification，连接时序分类）是一种用于序列建模的工具，其核心是定义了特殊的**目标函数/优化准则**[1]。
> jupyter notebook 版见
> [repo](https://github.com/DingKe/ml-tutorial/tree/master/ctc)
> .

# 1. 算法
这里大体根据 Alex Graves 的开山之作[1]，讨论 CTC 的算法原理，并基于 numpy 从零实现 CTC 的推理及训练算法。
## 1.1 序列问题形式化。
序列问题可以形式化为如下函数：

$$
\mathcal{N}_w: (\mathcal{R}^m)^T \rightarrow (\mathcal{R}^n)^T
$$
其中，序列目标为字符串（词表大小为$n$），即$\mathcal{N}_w$输出为$n$维多项概率分布（e.g. 经过 softmax 处理）。
网络输出为：$y = \mathcal{N}_w$，其中，$y_k^t$$t$表示时刻第$k$项的概率。
![](https://distill.pub/2017/ctc/assets/full_collapse_from_audio.svg)[ ](https://distill.pub/2017/ctc/assets/full_collapse_from_audio.svg)
**图1. 序列建模【****[src](https://distill.pub/2017/ctc/)****】**
[
](https://distill.pub/2017/ctc/assets/full_collapse_from_audio.svg)虽然并没为限定$\mathcal{N}_w$具体形式，下面为假设其了某种神经网络（e.g. RNN）。
下面代码示例 toy$\mathcal{N}_w$：
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
np.random.seed(
```
```python
1111
```
```python
)
T, V =
```
```python
12
```
```python
,
```
```python
5
```
```python
m, n =
```
```python
6
```
```python
, V
x = np.random.random([T, m])
```
```python
# T x m
```
```python
w = np.random.random([m, n])
```
```python
# weights, m x n
```
```python
def
```
```python
softmax
```
```python
(logits)
```
```python
:
```
```python
max_value = np.max(logits, axis=
```
```python
1
```
```python
, keepdims=
```
```python
True
```
```python
)
    exp = np.exp(logits - max_value)
    exp_sum = np.sum(exp, axis=
```
```python
1
```
```python
, keepdims=
```
```python
True
```
```python
)
    dist = exp / exp_sum
```
```python
return
```
```python
dist
```
```python
def
```
```python
toy_nw
```
```python
(x)
```
```python
:
```
```python
y = np.matmul(x, w)
```
```python
# T x n
```
```python
y = softmax(y)
```
```python
return
```
```python
y
y = toy_nw(x)
print(y)
print(y.sum(
```
```python
1
```
```python
, keepdims=
```
```python
True
```
```python
))
```
`[[ 0.24654511  0.18837589  0.16937668  0.16757465  0.22812766]
 [ 0.25443629  0.14992236  0.22945293  0.17240658  0.19378184]
 [ 0.24134404  0.17179604  0.23572466  0.12994237  0.22119288]
 [ 0.27216255  0.13054313  0.2679252   0.14184499  0.18752413]
 [ 0.32558002  0.13485564  0.25228604  0.09743785  0.18984045]
 [ 0.23855586  0.14800386  0.23100255  0.17158135  0.21085638]
 [ 0.38534786  0.11524603  0.18220093  0.14617864  0.17102655]
 [ 0.21867406  0.18511892  0.21305488  0.16472572  0.21842642]
 [ 0.29856607  0.13646801  0.27196606  0.11562552  0.17737434]
 [ 0.242347    0.14102063  0.21716951  0.2355229   0.16393996]
 [ 0.26597326  0.10009752  0.23362892  0.24560198  0.15469832]
 [ 0.23337289  0.11918746  0.28540761  0.20197928  0.16005275]]
[[ 1.]
 [ 1.]
 [ 1.]
 [ 1.]
 [ 1.]
 [ 1.]
 [ 1.]
 [ 1.]
 [ 1.]
 [ 1.]
 [ 1.]
 [ 1.]]`
## 1.2 align-free 变长映射
上面的形式是输入和输出的一对一的映射。序列学习任务一般而言是多对多的映射关系（如语音识别中，上百帧输出可能仅对应若干音节或字符，并且每个输入和输出之间，也没有清楚的对应关系）。CTC 通过引入一个特殊的 blank 字符（用 % 表示），解决多对一映射问题。
扩展原始词表$L$为$L^\prime = L \cup \{\text{blank}\}$。对输出字符串，定义操作$\mathcal{B}$：1）合并连续的相同符号；2）去掉 blank 字符。
例如，对于 “aa%bb%%cc”，应用$\mathcal{B}$，则实际上代表的是字符串 “abc”。同理“%a%b%cc%” 也同样代表 “abc”。

$$
\mathcal{B}(aa\%bb\%\%cc) = \mathcal{B}(\%a\%b\%cc\%) = abc
$$
通过引入blank 及$\mathcal{B}$，可以实现了变长的映射。

$$
L^{\prime T} \rightarrow L^{\le T}
$$
`因为这个原因，CTC 只能建模输出长度小于输入长度的序列问题。`
## 1.3 似然计算
和大多数有监督学习一样，CTC 使用最大似然标准进行训练。
给定输入$x$，输出$l$的条件概率为：

$$
p(l|x) =  \sum_{\pi \in \mathcal{B}^{-1}(l)} p(\pi|x)
$$
其中，$\mathcal{B}^{-1}(l)$表示了长度为$T$且示经过$\mathcal{B}$结果为$l$字符串的集合。
**CTC 假设输出的概率是（相对于输入）条件独立的**，因此有：

$$
p(\pi|x) = \prod y^t_{\pi_t}, \forall \pi \in L^{\prime T}
$$
然而，直接按上式我们没有办理有效的计算似然值。下面用动态规划解决似然的计算及梯度计算, 涉及前向算法和后向算法。
## 1.4 前向算法
在前向及后向计算中，CTC 需要将输出字符串进行扩展。具体的，$(a_1,\cdots,a_m)$每个字符之间及首尾分别插入 blank，即扩展为$(\%, a_1,\%,a_2, \%,\cdots,\%, a_m,\%)$。下面的$l$为原始字符串，$l^\prime$指为扩展后的字符串。
定义

$$
\alpha_t(s) \stackrel{def}{=} \sum_{\pi \in N^T: \mathcal{B}(\pi_{1:t}) = l_{1:s}} \prod_{t^\prime=1}^t y^t_{\pi^\prime}
$$
显然有，

$$
\begin{align}
\alpha_1(1) = y_b^1,\\
\alpha_1(2) = y_{l_1}^1,\\
\alpha_1(s) = 0, \forall s > 2
\end{align}
$$
根据$\alpha$的定义，有如下递归关系：

$$
\alpha_t(s) = \{  \begin{array}{l}
(\alpha_{t-1}(s)+\alpha_{t-1}(s-1)) y^t_{l^\prime_s},\  \  \    if\  l^\prime_s = b \ or\  l_{s-2}^\prime = l_s^{\prime}  \\
(\alpha_{t-1}(s)+\alpha_{t-1}(s-1) + \alpha_{t-1}(s-2)) y^t_{l^\prime_s} \ \ otherwise
\end{array}
$$

### 1.4.1 Case 2
递归公式中 case 2 是一般的情形。如图所示，$t$时刻字符为$s$为 blank 时，它可能由于两种情况扩展而来：1）重复上一字符，即上个字符也是 a，2）字符发生转换，即上个字符是非 a 的字符。第二种情况又分为两种情形，2.1）上一字符是 blank；2.2）a 由非 blank 字符直接跳转而来（$\mathcal{B}$） 操作中， blank 最终会被去掉，因此 blank 并不是必须的）。
![](https://distill.pub/2017/ctc/assets/cost_regular.svg)[ ](https://distill.pub/2017/ctc/assets/cost_regular.svg)
**图2. 前向算法 Case 2 示例【****[src](https://distill.pub/2017/ctc/)****】**
[

](https://distill.pub/2017/ctc/assets/cost_regular.svg)
### 1.4.2 Case 1
[
](https://distill.pub/2017/ctc/assets/cost_regular.svg)递归公式 case 1 是特殊的情形。
如图所示，$t$时刻字符为$s$为 blank 时，它只能由于两种情况扩展而来：1）重复上一字符，即上个字符也是 blank，2）字符发生转换，即上个字符是非 blank 字符。$t$时刻字符为$s$为非 blank 时，类似于 case 2，但是这时两个相同字符之间的 blank 不能省略（否则无法区分”aa”和”a”），因此，也只有两种跳转情况。
![](https://distill.pub/2017/ctc/assets/cost_no_skip.svg)[ ](https://distill.pub/2017/ctc/assets/cost_no_skip.svg)
**图3. 前向算法 Case 1 示例【****[src](https://distill.pub/2017/ctc/)****】**
[
](https://distill.pub/2017/ctc/assets/cost_no_skip.svg)我们可以利用动态规划计算所有$\alpha$的值，算法时间和空间复杂度为$O(T * L)$。
[
](https://distill.pub/2017/ctc/assets/cost_no_skip.svg)似然的计算只涉及乘加运算，因此，CTC 的似然是可导的，可以尝试 tensorflow 或 pytorch 等具有自动求导功能的工具自动进行梯度计算。下面介绍如何手动高效的计算梯度。
[

](https://distill.pub/2017/ctc/assets/cost_no_skip.svg)
```python
def
```
```python
forward
```
```python
(y, labels)
```
```python
:
```
```python
T, V = y.shape
    L = len(labels)
    alpha = np.zeros([T, L])
```
```python
# init
```
```python
alpha[
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
] = y[
```
```python
0
```
```python
, labels[
```
```python
0
```
```python
]]
    alpha[
```
```python
0
```
```python
,
```
```python
1
```
```python
] = y[
```
```python
0
```
```python
, labels[
```
```python
1
```
```python
]]
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
, T):
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
range(L):
            s = labels[i]
            a = alpha[t -
```
```python
1
```
```python
, i]
```
```python
if
```
```python
i -
```
```python
1
```
```python
>=
```
```python
0
```
```python
:
                a += alpha[t -
```
```python
1
```
```python
, i -
```
```python
1
```
```python
]
```
```python
if
```
```python
i -
```
```python
2
```
```python
>=
```
```python
0
```
```python
and
```
```python
s !=
```
```python
0
```
```python
and
```
```python
s != labels[i -
```
```python
2
```
```python
]:
                a += alpha[t -
```
```python
1
```
```python
, i -
```
```python
2
```
```python
]
            alpha[t, i] = a * y[t, s]
```
```python
return
```
```python
alpha
labels = [
```
```python
0
```
```python
,
```
```python
3
```
```python
,
```
```python
0
```
```python
,
```
```python
3
```
```python
,
```
```python
0
```
```python
,
```
```python
4
```
```python
,
```
```python
0
```
```python
]
```
```python
# 0 for blank
```
```python
alpha = forward(y, labels)
print(alpha)
```
[
](https://distill.pub/2017/ctc/assets/cost_no_skip.svg)最后可以得到似然$p(l|x) = \alpha_T(|l^\prime|) + \alpha_T(|l^\prime|-1)$。
[

](https://distill.pub/2017/ctc/assets/cost_no_skip.svg)
```python
p = alpha[-
```
```python
1
```
```python
, labels[-
```
```python
1
```
```python
]] + alpha[-
```
```python
1
```
```python
, labels[-
```
```python
2
```
```python
]]
print(p)
```
[
](https://distill.pub/2017/ctc/assets/cost_no_skip.svg)`6.81811271177e-06`[

](https://distill.pub/2017/ctc/assets/cost_no_skip.svg)
## 1.5 后向计算
[
](https://distill.pub/2017/ctc/assets/cost_no_skip.svg)类似于前向计算，我们定义后向计算。
首先定义

$$
\beta_t(s)   \stackrel{def}{=} \sum_{\pi \in N^T: \mathcal{B}(\pi_{t:T}) = l_{s:|l|}} \prod_{t^\prime=t}^T y^t_{\pi^\prime}
$$
显然，

$$
\begin{align}
\beta_T(|l^\prime|) = y_b^T,\\
\beta_T(|l^\prime|-1) = y_{l_{|l|}}^T,\\
\beta_T(s) = 0, \forall s < |l^\prime| - 1
\end{align}
$$
易得如下递归关系：

$$
\beta_t(s) = \{  \begin{array}{l}
(\beta_{t+1}(s)+\beta_{t+1}(s+1)) y^t_{l^\prime_s},\  \  \    if\  l^\prime_s = b \ or\  l_{s+2}^\prime = l_s^{\prime}  \\
(\beta_{t+1}(s)+\beta_{t+1}(s+1) + \beta_{t+1}(s+2)) y^t_{l^\prime_s} 
\end{array}
$$

```python
def
```
```python
backward
```
```python
(y, labels)
```
```python
:
```
```python
T, V = y.shape
    L = len(labels)
    beta = np.zeros([T, L])
```
```python
# init
```
```python
beta[-
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
] = y[-
```
```python
1
```
```python
, labels[-
```
```python
1
```
```python
]]
    beta[-
```
```python
1
```
```python
, -
```
```python
2
```
```python
] = y[-
```
```python
1
```
```python
, labels[-
```
```python
2
```
```python
]]
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
range(T -
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
range(L):
            s = labels[i]
            a = beta[t +
```
```python
1
```
```python
, i]
```
```python
if
```
```python
i +
```
```python
1
```
```python
< L:
                a += beta[t +
```
```python
1
```
```python
, i +
```
```python
1
```
```python
]
```
```python
if
```
```python
i +
```
```python
2
```
```python
< L
```
```python
and
```
```python
s !=
```
```python
0
```
```python
and
```
```python
s != labels[i +
```
```python
2
```
```python
]:
                a += beta[t +
```
```python
1
```
```python
, i +
```
```python
2
```
```python
]
            beta[t, i] = a * y[t, s]
```
```python
return
```
```python
beta
beta = backward(y, labels)
print(beta)
```
## 1.6 梯度计算
下面，我们利用前向、后者计算的$\alpha$和$\beta$来计算梯度。
根据$\alpha$、$\beta$的定义，我们有：

$$
\alpha_t(s)\beta_t(s) = \sum_{\pi \in  \mathcal{B}^{-1}(l):\pi_t=l_s^\prime} y^t_{l_s^\prime} \prod_{t=1}^T y^t_{\pi_t} = y^t_{l_s^\prime} \cdot \sum_{\pi \in  \mathcal{B}^{-1}(l):\pi_t=l_s^\prime} \prod_{t=1}^T y^t_{\pi_t}
$$
则

$$
\frac{\alpha_t(s)\beta_t(s)}{ y^t_{l_s^\prime}} = \sum_{\pi \in  \mathcal{B}^{-1}(l):\pi_t=l_s^\prime} \prod_{t=1}^T y^t_{\pi_t}  = \sum_{\pi \in  \mathcal{B}^{-1}(l):\pi_t=l_s^\prime} p(\pi|x)
$$
于是，可得似然

$$
p(l|x) = \sum_{s=1}^{|l^\prime|} \sum_{\pi \in  \mathcal{B}^{-1}(l):\pi_t=l_s^\prime} p(\pi|x) =  \sum_{s=1}^{|l^\prime|}  \frac{\alpha_t(s)\beta_t(s)}{ y^t_{l_s^\prime}}
$$
为计算$\frac{\partial p(l|x)}{\partial y^t_k}$，观察上式右端求各项，仅有$s=k$的项包含$y^t_k$，因此，其他项的偏导都为零，不用考虑。于是有：

$$
\frac{\partial p(l|x)}{\partial y^t_k} = \frac{\partial \frac{\alpha_t(k)\beta_t(k)}{ y^t_{k}} }{\partial y^t_k}
$$
利用除法的求导准则有：

$$
\frac{\partial p(l|x)}{\partial y^t_k} =  \frac{\frac{2 \cdot \alpha_t(k)\beta_t(k)}{ y^t_{k}} \cdot y^t_{k} -  \alpha_t(k)\beta_t(k) \cdot 1}{{y^t_k}^2} = \frac{\alpha_t(k)\beta_t(k)}{{y^t_k}^2}
$$

> 求导中，分子第一项是因为
> \alpha(k)\beta(k)
> 中包含为两个
> y^t_k
> 乘积项（即
> {y^t_k}^2
> ），其他均为与
> y^t_k
> 无关的常数。
$l$中可能包含多个$k$字符，它们计算的梯度要进行累加，因此，最后的梯度计算结果为：

$$
\frac{\partial p(l|x)}{\partial y^t_k} = \frac{1}{{y^t_k}^2} \sum_{s \in lab(l, k)} \alpha_t(s)\beta_t(s)
$$
其中，$lab(s)=\{s: l_s^\prime = k\}$。
一般我们优化似然函数的对数，因此，梯度计算如下：

$$
\frac{\partial \ln(p(l|x))}{\partial y^t_k} =\frac{1}{p(l|x)} \frac{\partial p(l|x)}{\partial y^t_k}
$$
其中，似然值在前向计算中已经求得：$p(l|x) = \alpha_T(|l^\prime|) + \alpha_T(|l^\prime|-1)$。
对于给定训练集$D$，待优化的目标函数为：

$$
O(D, N_w) = -\sum_{(x,z)\in D} \ln(p(z|x))
$$
得到梯度后，我们可以利用任意优化方法（e.g. SGD， Adam）进行训练。
```python
def
```
```python
gradient
```
```python
(y, labels)
```
```python
:
```
```python
T, V = y.shape
    L = len(labels)
    alpha = forward(y, labels)
    beta = backward(y, labels)
    p = alpha[-
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
] + alpha[-
```
```python
1
```
```python
, -
```
```python
2
```
```python
]
    grad = np.zeros([T, V])
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
range(T):
```
```python
for
```
```python
s
```
```python
in
```
```python
range(V):
            lab = [i
```
```python
for
```
```python
i, c
```
```python
in
```
```python
enumerate(labels)
```
```python
if
```
```python
c == s]
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
lab:
                grad[t, s] += alpha[t, i] * beta[t, i] 
            grad[t, s] /= y[t, s] **
```
```python
2
```
```python
grad /= p
```
```python
return
```
```python
grad
grad = gradient(y, labels)
print(grad)
```
将基于前向-后向算法得到梯度与基于数值的梯度比较，以验证实现的正确性。
```python
def
```
```python
check_grad
```
```python
(y, labels, w=-
```
```python
1
```
```python
, v=-
```
```python
1
```
```python
, toleration=
```
```python
1e-3
```
```python
)
```
```python
:
```
```python
grad_1 = gradient(y, labels)[w, v]
    delta =
```
```python
1e-10
```
```python
original = y[w, v]
    y[w, v] = original + delta
    alpha = forward(y, labels)
    log_p1 = np.log(alpha[-
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
] + alpha[-
```
```python
1
```
```python
, -
```
```python
2
```
```python
])
    y[w, v] = original - delta
    alpha = forward(y, labels)
    log_p2 = np.log(alpha[-
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
] + alpha[-
```
```python
1
```
```python
, -
```
```python
2
```
```python
])
    y[w, v] = original
    grad_2 = (log_p1 - log_p2) / (
```
```python
2
```
```python
* delta)
```
```python
if
```
```python
np.abs(grad_1 - grad_2) > toleration:
        print(
```
```python
'[%d, %d]：%.2e'
```
```python
% (w, v, np.abs(grad_1 - grad_2)))
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
1e-5
```
```python
,
```
```python
1e-6
```
```python
]:
    print(
```
```python
'%.e'
```
```python
% toleration)
```
```python
for
```
```python
w
```
```python
in
```
```python
range(y.shape[
```
```python
0
```
```python
]):
```
```python
for
```
```python
v
```
```python
in
```
```python
range(y.shape[
```
```python
1
```
```python
]):
            check_grad(y, labels, w, v, toleration)
```
`1e-05
1e-06
[0, 3]：3.91e-06
[1, 0]：3.61e-06
[1, 3]：2.66e-06
[2, 0]：2.67e-06
[2, 3]：3.88e-06
[3, 0]：4.71e-06
[3, 3]：3.39e-06
[4, 0]：1.24e-06
[4, 3]：4.79e-06
[5, 0]：1.57e-06
[5, 3]：2.98e-06
[6, 0]：5.03e-06
[6, 3]：4.89e-06
[7, 0]：1.05e-06
[7, 4]：4.19e-06
[8, 4]：5.57e-06
[9, 0]：5.95e-06
[9, 3]：3.85e-06
[10, 0]：1.09e-06
[10, 3]：1.53e-06
[10, 4]：3.82e-06`可以看到，前向-后向及数值梯度两种方法计算的梯度差异都在 1e-5 以下，误差最多在 1e-6 的量级。这初步验证了前向-后向梯度计算方法原理和实现的正确性。
## 1.7 logits 梯度
在实际训练中，为了计算方便，可以将 CTC 和 softmax 的梯度计算合并，公式如下：

$$
\frac{\partial \ln(p(l|x))}{\partial y^t_k} = y^t_k - \frac{1}{y^t_k \cdot p(l|x)} \sum_{s \in lab(l, k)} \alpha_t(s)\beta_t(s)
$$
这是因为，softmax 的梯度反传公式为：

$$
\frac{\partial \ln(p(l|x))}{\partial u^t_k} = y^t_k (\frac{\partial \ln(p(l|x))}{\partial y^t_k}  - \sum_{j=1}^{V} \frac{\partial \ln(p(l|x))}{\partial y^t_j} y^t_j)
$$
接合上面两式，有:

$$
\frac{\partial \ln(p(l|x))}{\partial u^t_k} = \frac{1}{y^t_k p(l|x)} \sum_{s \in lab(l, k)} \alpha_t(s)\beta_t(s) - y^t_k
$$

```python
def
```
```python
gradient_logits_naive
```
```python
(y, labels)
```
```python
:
```
```python
'''
    gradient by back propagation
    '''
```
```python
y_grad = gradient(y, labels)
    sum_y_grad = np.sum(y_grad * y, axis=
```
```python
1
```
```python
, keepdims=
```
```python
True
```
```python
)
    u_grad = y * (y_grad - sum_y_grad)
```
```python
return
```
```python
u_grad
```
```python
def
```
```python
gradient_logits
```
```python
(y, labels)
```
```python
:
```
```python
'''
    '''
```
```python
T, V = y.shape
    L = len(labels)
    alpha = forward(y, labels)
    beta = backward(y, labels)
    p = alpha[-
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
] + alpha[-
```
```python
1
```
```python
, -
```
```python
2
```
```python
]
    u_grad = np.zeros([T, V])
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
range(T):
```
```python
for
```
```python
s
```
```python
in
```
```python
range(V):
            lab = [i
```
```python
for
```
```python
i, c
```
```python
in
```
```python
enumerate(labels)
```
```python
if
```
```python
c == s]
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
lab:
                u_grad[t, s] += alpha[t, i] * beta[t, i] 
            u_grad[t, s] /= y[t, s] * p
    u_grad -= y
```
```python
return
```
```python
u_grad
grad_l = gradient_logits_naive(y, labels)
grad_2 = gradient_logits(y, labels)
print(np.sum(np.abs(grad_l - grad_2)))
```
`1.34961486431e-15`同上，我们利用数值梯度来初步检验梯度计算的正确性：
```python
def
```
```python
check_grad_logits
```
```python
(x, labels, w=-
```
```python
1
```
```python
, v=-
```
```python
1
```
```python
, toleration=
```
```python
1e-3
```
```python
)
```
```python
:
```
```python
grad_1 = gradient_logits(softmax(x), labels)[w, v]
    delta =
```
```python
1e-10
```
```python
original = x[w, v]
    x[w, v] = original + delta
    y = softmax(x)
    alpha = forward(y, labels)
    log_p1 = np.log(alpha[-
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
] + alpha[-
```
```python
1
```
```python
, -
```
```python
2
```
```python
])
    x[w, v] = original - delta
    y = softmax(x)
    alpha = forward(y, labels)
    log_p2 = np.log(alpha[-
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
] + alpha[-
```
```python
1
```
```python
, -
```
```python
2
```
```python
])
    x[w, v] = original
    grad_2 = (log_p1 - log_p2) / (
```
```python
2
```
```python
* delta)
```
```python
if
```
```python
np.abs(grad_1 - grad_2) > toleration:
        print(
```
```python
'[%d, %d]：%.2e, %.2e, %.2e'
```
```python
% (w, v, grad_1, grad_2, np.abs(grad_1 - grad_2)))
np.random.seed(
```
```python
1111
```
```python
)
x = np.random.random([
```
```python
10
```
```python
,
```
```python
10
```
```python
])
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
1e-5
```
```python
,
```
```python
1e-6
```
```python
]:
    print(
```
```python
'%.e'
```
```python
% toleration)
```
```python
for
```
```python
w
```
```python
in
```
```python
range(x.shape[
```
```python
0
```
```python
]):
```
```python
for
```
```python
v
```
```python
in
```
```python
range(x.shape[
```
```python
1
```
```python
]):
            check_grad_logits(x, labels, w, v, toleration)
```
# 2. 数值稳定性
CTC 的训练过程面临数值下溢的风险，特别是序列较大的情况下。下面介绍两种数值上稳定的工程优化方法：1）log 域（许多 CRF 实现的常用方法）；2）scale 技巧（原始论文 [1] 使用的方法）。
## 2.1 log 域计算
log 计算涉及 logsumexp 操作。
[经验表明](https://github.com/baidu-research/warp-ctc)，在 log 域计算，即使使用单精度，也表现出良好的数值稳定性，可以有效避免下溢的风险。稳定性的代价是增加了运算的复杂性——原始实现只涉及乘加运算，log 域实现则需要对数和指数运算。
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
### 2.1.1 log 域前向算法
基于 log 的前向算法实现如下：
```python
def
```
```python
forward_log
```
```python
(log_y, labels)
```
```python
:
```
```python
T, V = log_y.shape
    L = len(labels)
    log_alpha = np.ones([T, L]) * ninf
```
```python
# init
```
```python
log_alpha[
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
] = log_y[
```
```python
0
```
```python
, labels[
```
```python
0
```
```python
]]
    log_alpha[
```
```python
0
```
```python
,
```
```python
1
```
```python
] = log_y[
```
```python
0
```
```python
, labels[
```
```python
1
```
```python
]]
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
, T):
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
range(L):
            s = labels[i]
            a = log_alpha[t -
```
```python
1
```
```python
, i]
```
```python
if
```
```python
i -
```
```python
1
```
```python
>=
```
```python
0
```
```python
:
                a = logsumexp(a, log_alpha[t -
```
```python
1
```
```python
, i -
```
```python
1
```
```python
])
```
```python
if
```
```python
i -
```
```python
2
```
```python
>=
```
```python
0
```
```python
and
```
```python
s !=
```
```python
0
```
```python
and
```
```python
s != labels[i -
```
```python
2
```
```python
]:
                a = logsumexp(a, log_alpha[t -
```
```python
1
```
```python
, i -
```
```python
2
```
```python
])
            log_alpha[t, i] = a + log_y[t, s]
```
```python
return
```
```python
log_alpha
log_alpha = forward_log(np.log(y), labels)
alpha = forward(y, labels)
print(np.sum(np.abs(np.exp(log_alpha) - alpha)))
```
`8.60881935942e-17`
### 2.1.2 log 域后向算法
基于 log 的后向算法实现如下：
```python
def
```
```python
backward_log
```
```python
(log_y, labels)
```
```python
:
```
```python
T, V = log_y.shape
    L = len(labels)
    log_beta = np.ones([T, L]) * ninf
```
```python
# init
```
```python
log_beta[-
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
] = log_y[-
```
```python
1
```
```python
, labels[-
```
```python
1
```
```python
]]
    log_beta[-
```
```python
1
```
```python
, -
```
```python
2
```
```python
] = log_y[-
```
```python
1
```
```python
, labels[-
```
```python
2
```
```python
]]
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
range(T -
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
range(L):
            s = labels[i]
            a = log_beta[t +
```
```python
1
```
```python
, i]
```
```python
if
```
```python
i +
```
```python
1
```
```python
< L:
                a = logsumexp(a, log_beta[t +
```
```python
1
```
```python
, i +
```
```python
1
```
```python
])
```
```python
if
```
```python
i +
```
```python
2
```
```python
< L
```
```python
and
```
```python
s !=
```
```python
0
```
```python
and
```
```python
s != labels[i +
```
```python
2
```
```python
]:
                a = logsumexp(a, log_beta[t +
```
```python
1
```
```python
, i +
```
```python
2
```
```python
])
            log_beta[t, i] = a + log_y[t, s]
```
```python
return
```
```python
log_beta
log_beta = backward_log(np.log(y), labels)
beta = backward(y, labels)
print(np.sum(np.abs(np.exp(log_beta) - beta)))
```
`1.10399945005e-16`
### 2.1.3 log 域梯度计算
在前向、后向基础上，也可以在 log 域上计算梯度。
```python
def
```
```python
gradient_log
```
```python
(log_y, labels)
```
```python
:
```
```python
T, V = log_y.shape
    L = len(labels)
    log_alpha = forward_log(log_y, labels)
    log_beta = backward_log(log_y, labels)
    log_p = logsumexp(log_alpha[-
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
], log_alpha[-
```
```python
1
```
```python
, -
```
```python
2
```
```python
])
    log_grad = np.ones([T, V]) * ninf
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
range(T):
```
```python
for
```
```python
s
```
```python
in
```
```python
range(V):
            lab = [i
```
```python
for
```
```python
i, c
```
```python
in
```
```python
enumerate(labels)
```
```python
if
```
```python
c == s]
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
lab:
                log_grad[t, s] = logsumexp(log_grad[t, s], log_alpha[t, i] + log_beta[t, i]) 
            log_grad[t, s] -=
```
```python
2
```
```python
* log_y[t, s]
    log_grad -= log_p
```
```python
return
```
```python
log_grad
log_grad = gradient_log(np.log(y), labels)
grad = gradient(y, labels)
```
```python
#print(log_grad)
```
```python
#print(grad)
```
```python
print(np.sum(np.abs(np.exp(log_grad) - grad)))
```
`4.97588081849e-14`
## 2.2 scale
### 2.2.1 前向算法
为了避免下溢，在前向算法的每个时刻，都对计算出的$\alpha$的范围进行缩放：

$$
C_t \stackrel{def}{=} \sum_s\alpha_t(s)
$$

$$
\hat{\alpha}_t = \frac{\alpha_t(s)}{C_t}
$$
缩放后的$\alpha$，不会随着时刻的积累变得太小。$\hat{\alpha}$替代$\alpha$，进行下一时刻的迭代。
```python
def
```
```python
forward_scale
```
```python
(y, labels)
```
```python
:
```
```python
T, V = y.shape
    L = len(labels)
    alpha_scale = np.zeros([T, L])
```
```python
# init
```
```python
alpha_scale[
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
] = y[
```
```python
0
```
```python
, labels[
```
```python
0
```
```python
]]
    alpha_scale[
```
```python
0
```
```python
,
```
```python
1
```
```python
] = y[
```
```python
0
```
```python
, labels[
```
```python
1
```
```python
]]
    Cs = []
    C = np.sum(alpha_scale[
```
```python
0
```
```python
])
    alpha_scale[
```
```python
0
```
```python
] /= C
    Cs.append(C)
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
, T):
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
range(L):
            s = labels[i]
            a = alpha_scale[t -
```
```python
1
```
```python
, i]
```
```python
if
```
```python
i -
```
```python
1
```
```python
>=
```
```python
0
```
```python
:
                a += alpha_scale[t -
```
```python
1
```
```python
, i -
```
```python
1
```
```python
]
```
```python
if
```
```python
i -
```
```python
2
```
```python
>=
```
```python
0
```
```python
and
```
```python
s !=
```
```python
0
```
```python
and
```
```python
s != labels[i -
```
```python
2
```
```python
]:
                a += alpha_scale[t -
```
```python
1
```
```python
, i -
```
```python
2
```
```python
]
            alpha_scale[t, i] = a * y[t, s]
        C = np.sum(alpha_scale[t])
        alpha_scale[t] /= C
        Cs.append(C)
```
```python
return
```
```python
alpha_scale, Cs
```
由于进行了缩放，最后计算概率时要时行补偿：

$$
p(l|x) = \alpha_T(|l^\prime|) + \alpha_T(|l^\prime|-1) = (\hat\alpha_T(|l^\prime|) + \hat\alpha_T(|l^\prime|-1) * \prod_{t=1}^T C_t
$$

$$
\ln(p(l|x)) = \sum_t^T\ln(C_t) + \ln(\hat\alpha_T(|l^\prime|) + \hat\alpha_T(|l^\prime|-1))
$$

```python
labels = [
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
0
```
```python
]
```
```python
# 0 for blank
```
```python
alpha_scale, Cs = forward_scale(y, labels)
log_p = np.sum(np.log(Cs)) + np.log(alpha_scale[-
```
```python
1
```
```python
][labels[-
```
```python
1
```
```python
]] + alpha_scale[-
```
```python
1
```
```python
][labels[-
```
```python
2
```
```python
]])
alpha = forward(y, labels)
p = alpha[-
```
```python
1
```
```python
, labels[-
```
```python
1
```
```python
]] + alpha[-
```
```python
1
```
```python
, labels[-
```
```python
2
```
```python
]]
print(np.log(p), log_p, np.log(p) - log_p)
```
`(-13.202925982240107, -13.202925982240107, 0.0)`
### 2.2.2 后向算法
后向算法缩放类似于前向算法，公式如下：

$$
D_t \stackrel{def}{=} \sum_s\beta_t(s)
$$

$$
\hat{\beta}_t = \frac{\beta_t(s)}{D_t}
$$

```python
def
```
```python
backward_scale
```
```python
(y, labels)
```
```python
:
```
```python
T, V = y.shape
    L = len(labels)
    beta_scale = np.zeros([T, L])
```
```python
# init
```
```python
beta_scale[-
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
] = y[-
```
```python
1
```
```python
, labels[-
```
```python
1
```
```python
]]
    beta_scale[-
```
```python
1
```
```python
, -
```
```python
2
```
```python
] = y[-
```
```python
1
```
```python
, labels[-
```
```python
2
```
```python
]]
    Ds = []
    D = np.sum(beta_scale[-
```
```python
1
```
```python
,:])
    beta_scale[-
```
```python
1
```
```python
] /= D
    Ds.append(D)
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
range(T -
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
range(L):
            s = labels[i]
            a = beta_scale[t +
```
```python
1
```
```python
, i]
```
```python
if
```
```python
i +
```
```python
1
```
```python
< L:
                a += beta_scale[t +
```
```python
1
```
```python
, i +
```
```python
1
```
```python
]
```
```python
if
```
```python
i +
```
```python
2
```
```python
< L
```
```python
and
```
```python
s !=
```
```python
0
```
```python
and
```
```python
s != labels[i +
```
```python
2
```
```python
]:
                a += beta_scale[t +
```
```python
1
```
```python
, i +
```
```python
2
```
```python
]
            beta_scale[t, i] = a * y[t, s]
        D = np.sum(beta_scale[t])
        beta_scale[t] /= D
        Ds.append(D)
```
```python
return
```
```python
beta_scale, Ds[::-
```
```python
1
```
```python
]
beta_scale, Ds = backward_scale(y, labels)
print(beta_scale)
```
### 2.2.3 梯度计算

$$
\frac{\partial \ln(p(l|x))}{\partial y^t_k} = \frac{1}{p(l|x)} \frac{\partial p(l|x)}{\partial y^t_k} = \frac{1}{p(l|x)} \frac{1}{{y^t_k}^2} \sum_{s \in lab(l, k)} \alpha_t(s)\beta_t(s)
$$
考虑到

$$
p(l|x) =  \sum_{s=1}^{|l^\prime|}  \frac{\alpha_t(s)\beta_t(s)}{ y^t_{l_s^\prime}}
$$
以及

$$
\alpha_t(s) = \hat\alpha_t(s) \cdot \prod_{k=1}^t C_k
$$

$$
\beta_t(s) = \hat\beta_t(s) \cdot \prod_{k=t}^T D_k
$$

$$
\frac{\partial \ln(p(l|x))}{\partial y^t_k} = \frac{1}{\sum_{s=1}^{|l^\prime|} \frac{\hat\alpha_t(s)  \hat\beta_t(s)}{y^t_{l^\prime_s}}} \frac{1}{{y^t_k}^2} \sum_{s \in lab(l, k)} \hat\alpha_t(s)  \hat\beta_t(s)
$$
式中最右项中的各个部分我们都已经求得。梯度计算实现如下：
```python
def
```
```python
gradient_scale
```
```python
(y, labels)
```
```python
:
```
```python
T, V = y.shape
    L = len(labels)
    alpha_scale, _ = forward_scale(y, labels)
    beta_scale, _ = backward_scale(y, labels)
    grad = np.zeros([T, V])
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
range(T):
```
```python
for
```
```python
s
```
```python
in
```
```python
range(V):
            lab = [i
```
```python
for
```
```python
i, c
```
```python
in
```
```python
enumerate(labels)
```
```python
if
```
```python
c == s]
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
lab:
                grad[t, s] += alpha_scale[t, i] * beta_scale[t, i]
            grad[t, s] /= y[t, s] **
```
```python
2
```
```python
# normalize factor
```
```python
z =
```
```python
0
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
range(L):
            z += alpha_scale[t, i] * beta_scale[t, i] / y[t, labels[i]]
        grad[t] /= z
```
```python
return
```
```python
grad
labels = [
```
```python
0
```
```python
,
```
```python
3
```
```python
,
```
```python
0
```
```python
,
```
```python
3
```
```python
,
```
```python
0
```
```python
,
```
```python
4
```
```python
,
```
```python
0
```
```python
]
```
```python
# 0 for blank
```
```python
grad_1 = gradient_scale(y, labels)
grad_2 = gradient(y, labels)
print(np.sum(np.abs(grad_1 - grad_2)))
```
`6.86256607096e-15`
### 2.2.4 logits 梯度
类似于 y 梯度的推导，logits 梯度计算公式如下：

$$
\frac{\partial \ln(p(l|x))}{\partial u^t_k} = \frac{1}{y^t_k Z_t} \sum_{s \in lab(l, k)} \hat\alpha_t(s)\hat\beta_t(s) - y^t_k
$$
其中，

$$
Z_t \stackrel{def}{=} \sum_{s=1}^{|l^\prime|} \frac{\hat\alpha_t(s)\hat\beta_t(s)}{y^t_{l^\prime_s}}
$$

# 3. 解码
训练和的$N_w$可以用来预测新的样本输入对应的输出字符串，这涉及到解码。
按照最大似然准则，最优的解码结果为：

$$
h(x) = \underset{l \in L^{\le T}}{\mathrm{argmax}}\ p(l|x)
$$
然而，上式不存在已知的高效解法。下面介绍几种实用的近似破解码方法。
## 3.1 贪心搜索 （greedy search）
虽然$p(l|x)$难以有效的计算，但是由于 CTC 的独立性假设，对于某个具体的字符串$\pi$（去 blank 前），确容易计算：

$$
p(\pi|x) = \prod_{k=1}^T p(\pi_k|x)
$$
因此，我们放弃寻找使$p(l|x)$最大的字符串，退而寻找一个使$p(\pi|x)$最大的字符串，即：

$$
h(x) \approx B(\pi^\star)
$$
其中，

$$
\pi^\star = \underset{\pi \in N^T}{\mathrm{argmax}}\ p(\pi|x)
$$
简化后，解码过程（构造$\pi^\star$）变得非常简单（基于独立性假设）： 在每个时刻输出概率最大的字符:

$$
\pi^\star = cat_{t=1}^T(\underset{s \in L^\prime}{\mathrm{argmax}}\ y^t_s)
$$

```python
def
```
```python
remove_blank
```
```python
(labels, blank=
```
```python
0
```
```python
)
```
```python
:
```
```python
new_labels = []
```
```python
# combine duplicate
```
```python
previous =
```
```python
None
```
```python
for
```
```python
l
```
```python
in
```
```python
labels:
```
```python
if
```
```python
l != previous:
            new_labels.append(l)
            previous = l
```
```python
# remove blank
```
```python
new_labels = [l
```
```python
for
```
```python
l
```
```python
in
```
```python
new_labels
```
```python
if
```
```python
l != blank]
```
```python
return
```
```python
new_labels
```
```python
def
```
```python
insert_blank
```
```python
(labels, blank=
```
```python
0
```
```python
)
```
```python
:
```
```python
new_labels = [blank]
```
```python
for
```
```python
l
```
```python
in
```
```python
labels:
        new_labels += [l, blank]
```
```python
return
```
```python
new_labels
```
```python
def
```
```python
greedy_decode
```
```python
(y, blank=
```
```python
0
```
```python
)
```
```python
:
```
```python
raw_rs = np.argmax(y, axis=
```
```python
1
```
```python
)
    rs = remove_blank(raw_rs, blank)
```
```python
return
```
```python
raw_rs, rs
np.random.seed(
```
```python
1111
```
```python
)
y = softmax(np.random.random([
```
```python
20
```
```python
,
```
```python
6
```
```python
]))
rr, rs = greedy_decode(y)
print(rr)
print(rs)
```
`[1 3 5 5 5 5 1 5 3 4 4 3 0 4 5 0 3 1 3 3]
[1, 3, 5, 1, 5, 3, 4, 3, 4, 5, 3, 1, 3]`
## 3.2 束搜索（Beam Search）
显然，贪心搜索的性能非常受限。例如，它不能给出除最优路径之外的其他其优路径。很多时候，如果我们能拿到 nbest 的路径，后续可以利用其他信息来进一步优化搜索的结果。束搜索能近似找出 top 最优的若干条路径。
```python
def
```
```python
beam_decode
```
```python
(y, beam_size=
```
```python
10
```
```python
)
```
```python
:
```
```python
T, V = y.shape
    log_y = np.log(y)
    beam = [([],
```
```python
0
```
```python
)]
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
range(T):
```
```python
# for every timestep
```
```python
new_beam = []
```
```python
for
```
```python
prefix, score
```
```python
in
```
```python
beam:
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
# for every state
```
```python
new_prefix = prefix + [i]
                new_score = score + log_y[t, i]
                new_beam.append((new_prefix, new_score))
```
```python
# top beam_size
```
```python
new_beam.sort(key=
```
```python
lambda
```
```python
x: x[
```
```python
1
```
```python
], reverse=
```
```python
True
```
```python
)
        beam = new_beam[:beam_size]
```
```python
return
```
```python
beam
np.random.seed(
```
```python
1111
```
```python
)
y = softmax(np.random.random([
```
```python
20
```
```python
,
```
```python
6
```
```python
]))
beam = beam_decode(y, beam_size=
```
```python
100
```
```python
)
```
```python
for
```
```python
string, score
```
```python
in
```
```python
beam[:
```
```python
20
```
```python
]:
    print(remove_blank(string), score)
```
## 3.3 前缀束搜索（Prefix Beam Search）
直接的束搜索的一个问题是，在保存的 top N 条路径中，可能存在多条实际上是同一结果（经过去重复、去 blank 操作）。这减少了搜索结果的多样性。下面介绍的前缀搜索方法，在搜索过程中不断的合并相同的前缀[2]。参考[gist](https://gist.github.com/awni/56369a90d03953e370f3964c826ed4b0)，前缀束搜索实现如下：
```python
from
```
```python
collections
```
```python
import
```
```python
defaultdict
```
```python
def
```
```python
prefix_beam_decode
```
```python
(y, beam_size=
```
```python
10
```
```python
, blank=
```
```python
0
```
```python
)
```
```python
:
```
```python
T, V = y.shape
    log_y = np.log(y)
    beam = [(tuple(), (
```
```python
0
```
```python
, ninf))]
```
```python
# blank, non-blank
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
range(T):
```
```python
# for every timestep
```
```python
new_beam = defaultdict(
```
```python
lambda
```
```python
: (ninf, ninf))
```
```python
for
```
```python
prefix, (p_b, p_nb)
```
```python
in
```
```python
beam:
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
# for every state
```
```python
p = log_y[t, i]
```
```python
if
```
```python
i == blank:
```
```python
# propose a blank
```
```python
new_p_b, new_p_nb = new_beam[prefix]
                    new_p_b = logsumexp(new_p_b, p_b + p, p_nb + p)
                    new_beam[prefix] = (new_p_b, new_p_nb)
```
```python
continue
```
```python
else
```
```python
:
```
```python
# extend with non-blank
```
```python
end_t = prefix[-
```
```python
1
```
```python
]
```
```python
if
```
```python
prefix
```
```python
else
```
```python
None
```
```python
# exntend current prefix
```
```python
new_prefix = prefix + (i,)
                    new_p_b, new_p_nb = new_beam[new_prefix]
```
```python
if
```
```python
i != end_t:
                        new_p_nb = logsumexp(new_p_nb, p_b + p, p_nb + p)
```
```python
else
```
```python
:
                        new_p_nb = logsumexp(new_p_nb, p_b + p)
                    new_beam[new_prefix] = (new_p_b, new_p_nb)
```
```python
# keep current prefix
```
```python
if
```
```python
i == end_t:
                        new_p_b, new_p_nb = new_beam[prefix]
                        new_p_nb = logsumexp(new_p_nb, p_nb + p)
                        new_beam[prefix] = (new_p_b, new_p_nb)
```
```python
# top beam_size
```
```python
beam = sorted(new_beam.items(), key=
```
```python
lambda
```
```python
x : logsumexp(*x[
```
```python
1
```
```python
]), reverse=
```
```python
True
```
```python
)
        beam = beam[:beam_size]
```
```python
return
```
```python
beam
np.random.seed(
```
```python
1111
```
```python
)
y = softmax(np.random.random([
```
```python
20
```
```python
,
```
```python
6
```
```python
]))
beam = prefix_beam_decode(y, beam_size=
```
```python
100
```
```python
)
```
```python
for
```
```python
string, score
```
```python
in
```
```python
beam[:
```
```python
20
```
```python
]:
    print(remove_blank(string), score)
```
## 3.4 其他解码方法
上述介绍了基本解码方法。实际中，搜索过程可以接合额外的信息，提高搜索的准确度（例如在语音识别任务中，加入语言模型得分信息,[前缀束搜索+语言模型](https://github.com/PaddlePaddle/DeepSpeech/blob/develop/decoders/decoders_deprecated.py)）。
本质上，CTC 只是一个训练准则。训练完成后，$N_w$输出一系列概率分布，这点和常规基于交叉熵准则训练的模型完全一致。因此，特定应用领域常规的解码也可以经过一定修改用来 CTC 的解码。例如在语音识别任务中，利用 CTC 训练的声学模型可以无缝融入原来的 WFST 的解码器中[5]（e.g. 参见[EESEN](https://github.com/srvk/eesen)）。
此外，[1] 给出了一种利用 CTC 顶峰特点的启发式搜索方法。
# 4. 工具
[warp-ctc](https://github.com/baidu-research/warp-ctc)是百度开源的基于 CPU 和 GPU 的高效并行实现。warp-ctc 自身提供 C 语言接口，对于流利的机器学习工具（[torch](https://github.com/baidu-research/warp-ctc/tree/master/torch_binding)、[pytorch](https://github.com/SeanNaren/deepspeech.pytorch)和[tensorflow](https://github.com/baidu-research/warp-ctc/tree/master/tensorflow_binding)、[chainer](https://github.com/jheymann85/chainer_ctc)）都有相应的接口绑定。
[cudnn 7](https://developer.nvidia.com/cudnn)以后开始提供 CTC 支持。
Tensorflow 也原生支持[CTC loss](https://www.tensorflow.org/api_docs/python/tf/nn/ctc_loss)，及 greedy 和 beam search 解码器。
# 小结
CTC 可以建模无对齐信息的多对多序列问题（输入长度不小于输出），如语音识别、连续字符识别 [3,4]。
CTC 不需要输入与输出的对齐信息，可以实现端到端的训练。
CTC 在 loss 的计算上，利用了整个 labels 序列的全局信息，某种意义上相对逐帧计算损失的方法，”更加区分性”。
# References
Graves et al.[Connectionist Temporal Classification : Labelling Unsegmented Sequence Data with Recurrent Neural Networks](ftp://ftp.idsia.ch/pub/juergen/icml2006.pdf).
Hannun et al.[First-Pass Large Vocabulary Continuous Speech Recognition using Bi-Directional Recurrent DNNs](https://arxiv.org/abs/1408.2873).
Graves et al.[Towards End-To-End Speech Recognition with Recurrent Neural Networks](http://jmlr.org/proceedings/papers/v32/graves14.pdf).
Liwicki et al.[A novel approach to on-line handwriting recognition based on bidirectional long short-term memory networks](https://www.cs.toronto.edu/~graves/icdar_2007.pdf).
Zenkel et al.[Comparison of Decoding Strategies for CTC Acoustic Models](https://arxiv.org/abs/1708.004469).
Huannun.[Sequence Modeling with CTC](https://distill.pub/2017/ctc/).

