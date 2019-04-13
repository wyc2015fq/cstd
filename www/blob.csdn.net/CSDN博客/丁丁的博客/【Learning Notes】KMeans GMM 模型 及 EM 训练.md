
# 【Learning Notes】KMeans GMM 模型 及 EM 训练 - 丁丁的博客 - CSDN博客


2018年03月03日 09:38:23[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：265


kmeans 和 GMM 的简单实现及基于 EM 的训练。
> jupyter notebook 版本见
> [repo](https://github.com/DingKe/ml-tutorial/blob/master/gmm/GMM-EM.ipynb)
> 。
首先，导入相关的模块。
```python
from
```
```python
__future__
```
```python
import
```
```python
print_function
```
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
import
```
```python
matplotlib
```
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
%matplotlib inline
```
从正态分布，随机生成两组一维数据，正态分布的期望和方差如代码所示：
```python
np.random.seed(
```
```python
1111
```
```python
)
mu1, mu2 =
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
sigma1, sigma2 =
```
```python
0.7
```
```python
,
```
```python
0.4
```
```python
num1, num2 =
```
```python
20
```
```python
,
```
```python
20
```
```python
data1 = np.random.randn(num1) * sigma1 + mu1
data2 = np.random.randn(num2) * sigma2 + mu2
print(
```
```python
'cluster 1 with mean {} std {}'
```
```python
.format(mu1, sigma1))
print(
```
```python
'cluster 2 with mean {} std {}'
```
```python
.format(mu2, sigma2))
```
`cluster 1 with mean 1 std 0.7
cluster 2 with mean 3 std 0.4`生成的数据图示如下：
```python
def
```
```python
plot
```
```python
(y, **kwargs)
```
```python
:
```
```python
plt.scatter(y, np.zeros_like(y), **kwargs)
plot(data1, c=
```
```python
'r'
```
```python
)
plt.scatter(mu1,
```
```python
0
```
```python
, c=
```
```python
'r'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
plot(data2, c=
```
```python
'g'
```
```python
)
plt.scatter(mu2,
```
```python
0
```
```python
, c=
```
```python
'g'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
```
![这里写图片描述](https://img-blog.csdn.net/20180209151420274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
许多场景下，我们提前不知道数据是如何生成的，即数据没有标注：
```python
data = np.concatenate([data1, data2])
plot(data)
```
![这里写图片描述](https://img-blog.csdn.net/20180209151432360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
K-Means 聚类是一个迭代算法。给定目标聚类数目 K,算法流程如下：
1. 初始化 K 个中心点。
2. 对每个中心点$c_i$，计算各个数据点$x_j$到此中心点的距离$dist_{ij}$（e.g. 欧式距离）。
3. 对每个数据点，将其归为距离最近的中心。
4. 对每个中心点$c_i$，利用其下的数据点，重新估计值$c_i = \sum_{x_j \in c_i} x_j$。
5. 重复 2 ~ 4，至于满足终止条件。
```python
def
```
```python
kmeans
```
```python
(x, nb_class=
```
```python
2
```
```python
, max_iter=
```
```python
10
```
```python
, verbose=False)
```
```python
:
```
```python
# random initiailize centers, step 1
```
```python
means = x[np.random.choice(range(len(x)), nb_class, replace=
```
```python
False
```
```python
)]
    dist = np.zeros([nb_class, len(x)])
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
range(max_iter):
```
```python
# calculate distance, step 2
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
range(nb_class):
```
```python
for
```
```python
k, point
```
```python
in
```
```python
enumerate(x):
                dist[j][k] = (means[j] - point) **
```
```python
2
```
```python
# update means, step 3 & 4
```
```python
min_idx = np.argmin(dist,
```
```python
0
```
```python
)
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
range(nb_class):
            ele = [x[k]
```
```python
for
```
```python
k, idx
```
```python
in
```
```python
enumerate(min_idx)
```
```python
if
```
```python
idx == j]
            means[j] = np.mean(ele)
```
```python
# calculate total distance for early stop
```
```python
total_dist =
```
```python
0
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
range(nb_class):
            total_dist = sum([(means[j] - x[k]) **
```
```python
2
```
```python
for
```
```python
k, idx
```
```python
in
```
```python
enumerate(min_idx)
```
```python
if
```
```python
idx == j])
```
```python
if
```
```python
verbose:
            print(
```
```python
"Iter {}, total dist {}"
```
```python
.format(i, total_dist))
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
range(nb_class):
```
```python
for
```
```python
k, point
```
```python
in
```
```python
enumerate(x):
                dist[j][k] = (means[j] - point) **
```
```python
2
```
```python
min_idx = np.argmin(dist,
```
```python
0
```
```python
)
    clusters = []
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
range(nb_class):
        ele = [x[k]
```
```python
for
```
```python
k, idx
```
```python
in
```
```python
enumerate(min_idx)
```
```python
if
```
```python
idx == i]
        clusters.append(np.asarray(ele))
```
```python
return
```
```python
means, clusters
```
```python
# original clusters
```
```python
plt.subplot(
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
,
```
```python
1
```
```python
)
plot(data1, c=
```
```python
'r'
```
```python
)
plt.scatter(mu1,
```
```python
0
```
```python
, c=
```
```python
'r'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
plot(data2, c=
```
```python
'g'
```
```python
)
plt.scatter(mu2,
```
```python
0
```
```python
, c=
```
```python
'g'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
```
```python
# k-mean clusters
```
```python
centers, clusters = kmeans(data, max_iter=
```
```python
10
```
```python
, verbose=
```
```python
True
```
```python
)
print(
```
```python
'Estimated Center 1 - val: {:.2f}'
```
```python
.format(centers[
```
```python
0
```
```python
]))
print(
```
```python
'Estimated Center 2 - val: {:.2f}'
```
```python
.format(centers[
```
```python
1
```
```python
]))
plt.subplot(
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
,
```
```python
2
```
```python
)
plot(clusters[
```
```python
0
```
```python
], c=
```
```python
'r'
```
```python
)
plt.scatter(centers[
```
```python
0
```
```python
],
```
```python
0
```
```python
, c=
```
```python
'r'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
plot(clusters[
```
```python
1
```
```python
], c=
```
```python
'g'
```
```python
)
plt.scatter(centers[
```
```python
1
```
```python
],
```
```python
0
```
```python
, c=
```
```python
'g'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
```
`Iter 0, total dist 24.6199215453
Iter 1, total dist 10.047227048
Iter 2, total dist 5.06138695227
Iter 3, total dist 5.06138695227
Iter 4, total dist 5.06138695227
Iter 5, total dist 5.06138695227
Iter 6, total dist 5.06138695227
Iter 7, total dist 5.06138695227
Iter 8, total dist 5.06138695227
Iter 9, total dist 5.06138695227
Estimated Center 1 - val: 0.70
Estimated Center 2 - val: 2.80`![这里写图片描述](https://img-blog.csdn.net/20180209151452365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
kmeans 算法对初始值非常敏感。这里我们随机挑选两个数据点做为中心点的初始值。实际中可能需要更鲁棒初始化方法，此处不再展开。
kmeans 采用的是硬件判决，即距离哪个中心点的距离最近，分配给谁。在上面的示例，两类重合的点，被错误归为了另一类。缺乏置信度。下面介绍本文的主题——高斯混合模型（GMM）。GMM 采用的是软判决的方式来进行聚类。
GMM 假设数据服从如下公布：

$$
p(x) = \sum_i^n \pi_i \cdot g_i(x)
$$
其中，$g_i = N(\mu_i, \sigma_i)$，且$\sum_i^n \pi_i = 1$。
在我们例子中，$\mu_1 = 1$，$\mu_2 = 3$$\sigma_1 = \sigma_2 = 1$$\pi_1 = \pi_2 = 0.5$。当然，实际中，我们是往往不知道其真实分布，但 GMM 有很强的建模能力，能够对许多实际问题进行良好的近似建模。
GMM 一般用 EM（期望-最大化算法）来优化。给定目标聚类数目 K,算法流程如下：
初始化 K 个高斯的参数\mu_i,\sigma_i,\pi_i,i=1,\dots, K。
（期望）：对每个高斯g_i，对每个数据点x_i，计算\gamma_{ij} = \frac{\pi_i \cdot g_i(x_j)}{\sum_{k-1}^K \pi_k \cdot g_k(x_j) }。
（最大化）：对每个高斯g_i，重新估计其参数\hat{\mu}_i = \frac{\sum_{j=1}^N \gamma_{ij} \cdot x_j}{ \sum_{j=1}^N \gamma_{ij} },\hat{\sigma}_i = \frac{\sum_{j=1}^N \gamma_{ij} \cdot (x_j - \hat{\mu}_i)^2}{ \sum_{j=1}^N \gamma_{ij} },\hat{\pi}_i = \frac{\sum_{j=1}^N \gamma_{ij}}{N} = \frac{\sum_{j=1}^N \gamma_{ij}}{\sum_{j=1}^N \sum_{i=1}^K  \gamma_{ij}}。
重复 2 ~ 3，至于满足终止条件。
GMM 的 EM 训练中，不同于 kmeans 步骤 2、3 的硬件分配，步骤 2 计算每个数据点的“归属度”，软性的更新参数（步骤3）。
GMM 对初始值敏感。一般先用kmean方法聚类，利用聚类数据点的均值和（协）方差作为各个高斯分量的初始值。
```python
def
```
```python
norm_dist
```
```python
(x, mu, sigma)
```
```python
:
```
```python
return
```
```python
np.exp(-(x - mu) **
```
```python
2
```
```python
/
```
```python
2
```
```python
/ sigma **
```
```python
2
```
```python
) / ((
```
```python
2
```
```python
* np.pi) **
```
```python
0.5
```
```python
* sigma)
```
```python
def
```
```python
gmm
```
```python
(x, nb_class=
```
```python
2
```
```python
, max_iter=
```
```python
100
```
```python
, threshold=
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
# initialize using kmeans clustering
```
```python
mus, clusters = kmeans(x,
```
```python
2
```
```python
)
    sigmas = np.zeros(nb_class)
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
range(nb_class):
        sigmas[i] = np.std(clusters[i])
    pis = np.asarray([len(cluster) / float(len(x))
```
```python
for
```
```python
cluster
```
```python
in
```
```python
clusters])
    gammas = np.zeros([nb_class, len(x)])
    cur_lld = -np.float(
```
```python
'inf'
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
range(max_iter):
```
```python
# calculate probability， step 2
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
range(nb_class):
```
```python
for
```
```python
k, point
```
```python
in
```
```python
enumerate(x):
                gammas[j][k] =  norm_dist(point, mus[j], sigmas[j]) * pis[j]
        gammas = gammas / np.sum(gammas,
```
```python
0
```
```python
)
```
```python
# update estimation, step 3
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
range(nb_class):
            total, total_squared, norm_factor =
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
,
```
```python
0
```
```python
# mu
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
range(len(x)):
                total += x[k] * gammas[j][k]
                norm_factor += gammas[j][k]
            mus[j] = total / norm_factor
```
```python
# sigma
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
range(len(x)):
                total_squared += (x[k] - mus[j]) **
```
```python
2
```
```python
* gammas[j][k]
            sigmas[j] = (total_squared / norm_factor) **
```
```python
0.5
```
```python
# pi
```
```python
pis[j] = norm_factor / len(x)
```
```python
# likelihood
```
```python
lld =
```
```python
0
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
x:
            ld =
```
```python
0
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
range(nb_class):
                ld += norm_dist(e, mus[j], sigmas[j]) * pis[j]
            lld += np.log(ld)
        cur_lld, last_lld = lld, cur_lld
        print(
```
```python
"Iter {}, log likelihood {}"
```
```python
.format(i, lld))
```
```python
if
```
```python
cur_lld - last_lld <= threshold:
```
```python
break
```
```python
return
```
```python
mus, sigmas, pis
```
```python
# original clusters
```
```python
plt.subplot(
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
,
```
```python
1
```
```python
)
plot(data1, c=
```
```python
'r'
```
```python
)
plt.scatter(mu1,
```
```python
0
```
```python
, c=
```
```python
'r'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
plot(data2, c=
```
```python
'g'
```
```python
)
plt.scatter(mu2,
```
```python
0
```
```python
, c=
```
```python
'g'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)

x1 = np.linspace(
```
```python
0
```
```python
,
```
```python
3.5
```
```python
,
```
```python
100
```
```python
)
plt.plot(x1, norm_dist(x1, mu1, sigma1), c=
```
```python
'r'
```
```python
)
x2 = np.linspace(
```
```python
0
```
```python
,
```
```python
3.5
```
```python
,
```
```python
100
```
```python
)
plt.plot(x2, norm_dist(x2, mu2, sigma2), c=
```
```python
'g'
```
```python
)
```
```python
# k-mean clusters
```
```python
centers, clusters = kmeans(data)
plt.subplot(
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
,
```
```python
2
```
```python
)
plot(clusters[
```
```python
0
```
```python
], c=
```
```python
'r'
```
```python
)
plt.scatter(centers[
```
```python
0
```
```python
],
```
```python
0
```
```python
, c=
```
```python
'r'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
plot(clusters[
```
```python
1
```
```python
], c=
```
```python
'g'
```
```python
)
plt.scatter(centers[
```
```python
1
```
```python
],
```
```python
0
```
```python
, c=
```
```python
'g'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
```
```python
# gmm
```
```python
mus, sigmas, pis = gmm(data, threshold=
```
```python
0.05
```
```python
)
print(
```
```python
'Estimated Gaussian 1 - mu: {:.2f}, sigma: {:.2f}, pi: {:.2f}'
```
```python
.format(mus[
```
```python
0
```
```python
], sigmas[
```
```python
0
```
```python
], pis[
```
```python
0
```
```python
]))
print(
```
```python
'Estimated Gaussian 2 - mu: {:.2f}, sigma: {:.2f}, pi: {:.2f}'
```
```python
.format(mus[
```
```python
1
```
```python
], sigmas[
```
```python
1
```
```python
], pis[
```
```python
1
```
```python
]))

plt.subplot(
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
,
```
```python
3
```
```python
)
plot(data, c=
```
```python
'b'
```
```python
)
plt.scatter(mus[
```
```python
0
```
```python
],
```
```python
0
```
```python
, c=
```
```python
'r'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
plt.scatter(mus[
```
```python
1
```
```python
],
```
```python
0
```
```python
, c=
```
```python
'g'
```
```python
, s=
```
```python
100
```
```python
, marker=
```
```python
'^'
```
```python
)
x1 = np.linspace(
```
```python
0
```
```python
,
```
```python
3.5
```
```python
,
```
```python
100
```
```python
)
plt.plot(x1, norm_dist(x1, mus[
```
```python
0
```
```python
], sigmas[
```
```python
0
```
```python
]), c=
```
```python
'r'
```
```python
)
x2 = np.linspace(
```
```python
0
```
```python
,
```
```python
3.5
```
```python
,
```
```python
100
```
```python
)
plt.plot(x2, norm_dist(x2, mus[
```
```python
1
```
```python
], sigmas[
```
```python
1
```
```python
]), c=
```
```python
'g'
```
```python
)
```
`Iter 0, log likelihood -52.2221578339
Iter 1, log likelihood -51.7372519942
Iter 2, log likelihood -51.1416631328
Iter 3, log likelihood -50.5280229913
Iter 4, log likelihood -50.189923029
Iter 5, log likelihood -50.1243246025
Iter 6, log likelihood -50.1175371708
Estimated Gaussian 1 - mu: 2.97, sigma: 0.27, pi: 0.49
Estimated Gaussian 2 - mu: 0.97, sigma: 0.72, pi: 0.51`![这里写图片描述](https://img-blog.csdn.net/20180209151514358?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# References
Bishop. Pattern Recognition and Machine Learning (Chap 9).

