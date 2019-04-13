
# 【Learning Notes】Gumbel 分布及应用浅析 - 丁丁的博客 - CSDN博客


2018年02月24日 23:33:32[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：6689



# 1. 分布的形式化
## 物理意义
[Gumbel](https://en.wikipedia.org/wiki/Gumbel_distribution)分布是一种极值型分布。举例而言，假设每次测量心率值为一个随机变量（服从某种[指数族分布](https://en.wikipedia.org/wiki/Exponential_family)，如正态分布），每天测量10次心率并取最大的一个心率值作为当天的心率测量值。显然，每天纪录的心率值也是一个随机变量，并且它的概率分布即为 Gumbel 分布。
## 概率密度函数（PDF）
Gumbel 分布的 PDF 如下：

$$
f(x;\mu,\beta) = e^{-z-e^{-z}},\ z= \frac{x - \mu}{\beta}
$$
公式中，$\mu$是位置系数（Gumbel 分布的众数是$\mu$），$\beta$是尺度系数（Gumbel 分布的方差是$\frac{\pi^2}{6}\beta^2$）。
![PDF](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)[ ](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
**图1. Gumble PDF 示例图【****[src](https://en.wikipedia.org/wiki/Gumbel_distribution)****】**
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
```python
def
```
```python
gumbel_pdf
```
```python
(x, mu=
```
```python
0
```
```python
, beta=
```
```python
1
```
```python
)
```
```python
:
```
```python
z = (x - mu) / beta
```
```python
return
```
```python
np.exp(-z - np.exp(-z)) / beta
print(gumbel_pdf(
```
```python
0.5
```
```python
,
```
```python
0.5
```
```python
,
```
```python
2
```
```python
))
```
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
## 累计密度函数（CDF）
[
](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)相应的，gumbel 分布的 CDF 的公式如下：
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
$$
F(x;\mu,\beta) = e^{-e^{-(x-\mu)/\beta}}
$$
[
](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
```python
def
```
```python
gumbel_cdf
```
```python
(x, mu=
```
```python
0
```
```python
, beta=
```
```python
1
```
```python
)
```
```python
:
```
```python
z = (x - mu) / beta
```
```python
return
```
```python
np.exp(-np.exp(-z))
print(gumbel_cdf(
```
```python
5
```
```python
,
```
```python
0.5
```
```python
,
```
```python
2
```
```python
))
```
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
## CDF 的反函数
[
](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)根据 CDF 容易得到其反函数：
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
$$
F^{-1}(y;\mu,\beta) = \mu - \beta \ln(-\ln(y))
$$
[
](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)我们可以利用反函数法和生成服从 Gumbel 分布的随机数。
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
```python
def
```
```python
inv_gumbel_cdf
```
```python
(y, mu=
```
```python
0
```
```python
, beta=
```
```python
1
```
```python
, eps=
```
```python
1e-20
```
```python
)
```
```python
:
```
```python
return
```
```python
mu - beta * np.log(-np.log(y + eps))
print(inv_gumbel_cdf(gumbel_cdf(
```
```python
5
```
```python
,
```
```python
0.5
```
```python
,
```
```python
2
```
```python
),
```
```python
0.5
```
```python
,
```
```python
2
```
```python
))
```
```python
def
```
```python
sample_gumbel
```
```python
(shape)
```
```python
:
```
```python
p = np.random.random(shape)
```
```python
return
```
```python
inv_gumbel_cdf(p)
print(sample_gumbel([
```
```python
2
```
```python
,
```
```python
3
```
```python
]))
```
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
# 2. Gumbel-Max
[
](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)Gumbel 随机数可以用来对多项分布进行采样。
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
## 2.1 基于 softmax 的采样
[
](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)首先来看常规的采样方法。
[
](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)对于$logits = (x_1, \dots, x_K)$，首先利用 softmax 运算得到规一化的概率分布（多项分布）。
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
$$
\pi_k = \frac{e^{x_k}}{\sum_{k^\prime=1}^{K} e^{x_{k^\prime}}}
$$
[
](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)然后，利用轮盘赌的方式采样。下面的代码，直接使用 numpy 的[choice](https://docs.scipy.org/doc/numpy/reference/generated/numpy.random.choice.html)函数实现。
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
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
max_value = np.max(logits)
    exp = np.exp(logits - max_value)
    exp_sum = np.sum(exp)
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
sample_with_softmax
```
```python
(logits, size)
```
```python
:
```
```python
pros = softmax(logits)
```
```python
return
```
```python
np.random.choice(len(logits), size, p=pros)
```
[

](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)
## 2.2 基于 gumbel 的采样（gumbel-max）
[
](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Gumbel-Density.svg/488px-Gumbel-Density.svg.png)对于某组 logits，生成相同数量的 gumbel 随机数，并加到 logits 上。 然后选择数值最大的元素的编号作为采样值。
示例代码如下：
```python
def
```
```python
sample_with_gumbel_noise
```
```python
(logits, size)
```
```python
:
```
```python
noise = sample_gumbel((size, len(logits)))
```
```python
return
```
```python
np.argmax(logits + noise, axis=
```
```python
1
```
```python
)
```
可以[证明](https://hips.seas.harvard.edu/blog/2013/04/06/the-gumbel-max-trick-for-discrete-distributions/)，gumbel-max 方法的采样效果等效于基于 softmax 的方式。下面的实验直观地展示两种方法的采样效果。
```python
np.random.seed(
```
```python
1111
```
```python
)
logits = (np.random.random(
```
```python
10
```
```python
) -
```
```python
0.5
```
```python
) *
```
```python
2
```
```python
# (-1, 1)
```
```python
pop =
```
```python
100000
```
```python
softmax_samples = sample_with_softmax(logits, pop)
gumbel_samples = sample_with_gumbel_noise(logits, pop)
plt.subplot(
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
1
```
```python
)
plt.hist(softmax_samples)
plt.subplot(
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
2
```
```python
)
plt.hist(gumbel_samples)
```
![这里写图片描述](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**图2. Softmax 与 Gumbel-Max 采样结果对比**
[
](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)由于 Gumbel 随机数可以预先计算好，采样过程也不需要计算 softmax，因此，某些情况下，gumbel-max 方法相比于 softmax，在采样速度上会有优势。
[

](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 3. Gumbel-Softmax
[
](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)如果仅仅是提供一种常规 softmax 采样的替代方案， gumbel 分布似乎应用价值并不大。幸运的是，我们可以利用 gumbel 实现多项分布采样的 reparameterization（再参数化）。
[
](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)在介绍[VAE](http://blog.csdn.net/jackytintin/article/details/53641885)的时候讨论过，为了实现端到端的训练，VAE 采用的一个再参数化的技巧对高斯分布进行采样:
[

](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
def
```
```python
guassian_sample
```
```python
(shape)
```
```python
:
```
```python
epsilon = K.random_normal(shape, mean=
```
```python
0.
```
```python
,std=
```
```python
1
```
```python
)
```
```python
# 标准高斯分布
```
```python
z = z_mean + exp(z_log_var /
```
```python
2
```
```python
) * epsilon
```
[
](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)在介绍的 VAE 里，假设隐变量（latent variables）服从标准正态分布。下面将会看到，利用 gumbel-softmax 技巧，我们可以将隐变量建模为服从离散的多项分布。
[
](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)在上面的示例中，sample_with_softmax 使用了 choise 操作，而这个操作是不可导的。同样，观察 sample_with_gumbel_noise, armmax 操作同样不可导。然而，argmax 有一个 soft 版本，即**softmax**。
[
](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)我们首先扩展上面定义的 softmax 函数，添加一个 temperature 参数。
[

](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
def
```
```python
generalized_softmax
```
```python
(logits, temperature=
```
```python
1
```
```python
)
```
```python
:
```
```python
logits = logits / temperature
```
```python
return
```
```python
softmax(logits)
```
[
](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)temperature 是在大于零的参数，它控制着 softmax 的 soft 程度。温度越高，生成的分布越平滑；温度越低，生成的分布越接近离散的 one-hot 分布。下面示例对比了不同温度下，softmax 的结果。
[

](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
np.random.seed(
```
```python
1111
```
```python
)
n =
```
```python
10
```
```python
logits = (np.random.random(n) -
```
```python
0.5
```
```python
) *
```
```python
2
```
```python
# (-1, 1)
```
```python
x = range(n)
plt.subplot(
```
```python
1
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
1
```
```python
)
t =
```
```python
.1
```
```python
plt.bar(x, generalized_softmax(logits, t))
plt.subplot(
```
```python
1
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
2
```
```python
)
t =
```
```python
5
```
```python
plt.bar(x, generalized_softmax(logits, t))
plt.subplot(
```
```python
1
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
3
```
```python
)
t =
```
```python
50
```
```python
plt.bar(x, generalized_softmax(logits, t))
```
[
](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**图3. 不同温度系数下 softmax 结果对比**
[
](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)将 gumbel-max 中的 argmax 操作，替换为 softmax，便实现了对于原来不可导的采样操作的软化版的近似。训练中，可以通过逐渐降低温度，以逐步逼近真实的离散分布。利用 gumbel-softmax，VAE 的隐变量可以用多项分布进行建模，一个示例见[repo](https://github.com/DingKe/ml-tutorial/tree/master/gumbel)。这里，仅展示一个 toy 示例（代码[来自](http://amid.fish/humble-gumbel)）。
[

](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
import
```
```python
tensorflow
```
```python
as
```
```python
tf
sess = tf.Session()
```
```python
def
```
```python
differentiable_sample
```
```python
(logits, temperature=
```
```python
1
```
```python
)
```
```python
:
```
```python
noise = tf.random_uniform(tf.shape(logits), seed=
```
```python
11
```
```python
)
    logits_with_noise = logits - tf.log(-tf.log(noise))
```
```python
return
```
```python
tf.nn.softmax(logits_with_noise / temperature)
mean = tf.Variable(
```
```python
2.
```
```python
)
idxs = tf.Variable([
```
```python
0.
```
```python
,
```
```python
1.
```
```python
,
```
```python
2.
```
```python
,
```
```python
3.
```
```python
,
```
```python
4.
```
```python
])
```
```python
# An unnormalised approximately-normal distribution
```
```python
logits = tf.exp(-(idxs - mean) **
```
```python
2
```
```python
)
sess.run(tf.global_variables_initializer())
```
```python
def
```
```python
print_logit_vals
```
```python
()
```
```python
:
```
```python
logit_vals = sess.run(logits)
    print(
```
```python
" "
```
```python
.join([
```
```python
"{:.2f}"
```
```python
] * len(logit_vals)).format(*logit_vals))
print(
```
```python
"Logits: "
```
```python
)
print_logit_vals()
sample = differentiable_sample(logits)
sample_weights = tf.Variable([
```
```python
1.
```
```python
,
```
```python
2.
```
```python
,
```
```python
3.
```
```python
,
```
```python
4.
```
```python
,
```
```python
5.
```
```python
], trainable=
```
```python
False
```
```python
)
result = tf.reduce_sum(sample * sample_weights)
sess.run(tf.global_variables_initializer())
train_op = tf.train.GradientDescentOptimizer(learning_rate=
```
```python
1
```
```python
).minimize(-result)
print(
```
```python
"Distribution mean: {:.2f}"
```
```python
.format(sess.run(mean)))
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
5
```
```python
):
    sess.run(train_op)
    print(
```
```python
"Distribution mean: {:.2f}"
```
```python
.format(sess.run(mean)))
```
[
](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`Logits: 
0.02 0.37 1.00 0.37 0.02
Distribution mean: 2.00
Distribution mean: 2.13
Distribution mean: 2.23
Distribution mean: 2.60
Distribution mean: 2.75
Distribution mean: 3.23`[
](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)可以看到，利用 gumbel-softmax 训练参数向着预期的方向改变。
[

](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
import
```
```python
tensorflow
```
```python
as
```
```python
tf
sess = tf.Session()
mean = tf.Variable(
```
```python
2.
```
```python
)
idxs = tf.Variable([
```
```python
0.
```
```python
,
```
```python
1.
```
```python
,
```
```python
2.
```
```python
,
```
```python
3.
```
```python
,
```
```python
4.
```
```python
])
```
```python
# An unnormalised approximately-normal distribution
```
```python
logits = tf.exp(-(idxs - mean) **
```
```python
2
```
```python
)
sess.run(tf.global_variables_initializer())
```
```python
def
```
```python
print_logit_vals
```
```python
()
```
```python
:
```
```python
logit_vals = sess.run(logits)
    print(
```
```python
" "
```
```python
.join([
```
```python
"{:.2f}"
```
```python
] * len(logit_vals)).format(*logit_vals))
print(
```
```python
"Logits: "
```
```python
)
print_logit_vals()
sample = tf.nn.softmax(logits)
sample_weights = tf.Variable([
```
```python
1.
```
```python
,
```
```python
2.
```
```python
,
```
```python
3.
```
```python
,
```
```python
4.
```
```python
,
```
```python
5.
```
```python
], trainable=
```
```python
False
```
```python
)
result = tf.reduce_sum(sample * sample_weights)
sess.run(tf.global_variables_initializer())
train_op = tf.train.GradientDescentOptimizer(learning_rate=
```
```python
1
```
```python
).minimize(-result)
print(
```
```python
"Distribution mean: {:.2f}"
```
```python
.format(sess.run(mean)))
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
5
```
```python
):
    sess.run(train_op)
    print(
```
```python
"Distribution mean: {:.2f}"
```
```python
.format(sess.run(mean)))
```
[
](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`Logits: 
0.02 0.37 1.00 0.37 0.02
Distribution mean: 2.00
Distribution mean: 2.32
Distribution mean: 2.65
Distribution mean: 2.87
Distribution mean: 3.10
Distribution mean: 3.36`[

](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 讨论
[
](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)乍看起来，gumbel-softmax 的用处令人费解。比如上面的代码示例，直接使用 softmax，也可以达到类似的参数训练效果。但两者有着根本的区别。
原理上，常规的 softmax 直接建模了一个概率分布（多项分布），基于交叉熵的训练准则使分布尽可能靠近目标分布；而 gumbel-softmax 则是对多项分布采样的一个近似。使用上，常规的有监督学习任务（分类器训练）中，直接学习输出的概率分布是自然的选择；而对于涉及采样的学习任务（VAE 隐变量采样、强化学习中对actions 集合进行采样以确定下一步的操作），gumbel-softmax 提供了一种再参数化的方法，使得模型可以以端到端的方式进行训练。
> 完整的 jupyter notebook 版本参见
> [github](https://github.com/DingKe/ml-tutorial/blob/master/gumbel/gumbel-distribution.ipynb)
> 。

# References
[http://amid.fish/humble-gumbel](http://amid.fish/humble-gumbel)
[proof of Gumbel based sampling](https://hips.seas.harvard.edu/blog/2013/04/06/the-gumbel-max-trick-for-discrete-distributions/)
[https://blog.evjang.com/2016/11/tutorial-categorical-variational.html](https://blog.evjang.com/2016/11/tutorial-categorical-variational.html)
Jang et al.[CATEGORICAL REPARAMETERIZATION WITH GUMBEL-SOFTMAX](https://arxiv.org/abs/1611.01144).
[
						](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
	](https://img-blog.csdn.net/201803011724349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
  ](https://img-blog.csdn.net/20180224182108527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)