# 基于矩阵分解(MF,Matrix Factorization)的推荐算法 - Orisun - 博客园







# [基于矩阵分解(MF,Matrix Factorization)的推荐算法](https://www.cnblogs.com/zhangchaoyang/articles/5517186.html)





## LFM

LFM即隐因子模型，我们可以把隐因子理解为主题模型中的主题、HMM中的隐藏变量。比如一个用户喜欢《推荐系统实践》这本书，背后的原因可能是该用户喜欢推荐系统、或者是喜欢数据挖掘、亦或者是喜欢作者项亮本人等等，假如真的是由于这3个原因导致的，那如果项亮出了另外一本数据挖掘方面的书，我们可以推测该用户也会喜欢，这“背后的原因”我们称之为隐因子。所以LFM的其中思路就是先计算用户对各个隐因子的喜好程度$(p_1,p_2,...,p_f)$，再计算物品在各个隐因子上的概率分布$(q_1,q_2,...,q_f)$，两个向量做内积即得到用户对物品的喜好程度，下面就讲这两个向量怎么求。

假设我们已经有了一个评分矩阵$R_{m,n}$，$m$个用户对$n$个物品的评分全在这个矩阵里，当然这是一个高度稀疏的矩阵，我们用$r_{u,i}$表示用户$u$对物品$i$的评分。LFM认为$R_{m,n}=P_{m,F}\cdot{Q_{F,n}}$即R是两个矩阵的乘积(所以LFM又被称为矩阵分解法，MF，matrix factorization model)，F是隐因子的个数，P的每一行代表一个用户对各隐因子的喜欢程序，Q的每一列代表一个物品在各个隐因子上的概率分布。

\begin{equation}\hat{r}_{ui}=\sum_{f=1}^{F}{P_{uf}Q_{fi}}\label{lfm}\end{equation}

机器学习训练的目标是使得对所有的$\color{red}{r_{ui}\ne0}$，$r_{u,i}$和$\hat{r}_{ui}$尽可能接近，即

\begin{equation}min:\ \ Loss=\sum_{\color{red}{r_{ui}\ne0}}{(r_{u,i}-\hat{r}_{ui})^2}\end{equation}

为防止过拟合，加个正则项，以防止$P_{uf},Q_{fi}$过大或过小。

\begin{equation}min:\ \ Loss=\sum_{\color{red}{r_{ui}\ne0}}{(r_{u,i}-\hat{r}_{ui})^2}+\lambda(\sum{P_{uf}^2}+\sum{Q_{fi}^2})=f(P,Q)\label{target_lfm}\end{equation}

采用梯度下降法求解上面的无约束最优化问题，在第$t+1$轮迭代中$P$和$Q$的值分别应该是

\begin{equation}P^{(t+1)}=P^{(t)}-\alpha\frac{\partial{Loss}}{\partial{P^{(t)}}},Q^{(t+1)}=Q^{(t)}-\alpha\frac{\partial{Loss}}{\partial{Q^{(t)}}}\end{equation}

\begin{equation}\frac{\partial{Loss}}{\partial{P^{(t)}}}=\left[\begin{array}{cc}\frac{\partial{Loss}}{\partial{P_{11}^{(t)}}}\ ...\ \frac{\partial{Loss}}{\partial{P_{1F}^{(t)}}}\\...\ \frac{\partial{Loss}}{\partial{P_{uf}^{(t)}}}\ ...\\\frac{\partial{Loss}}{\partial{P_{m1}^{(t)}}}\ ...\ \frac{\partial{Loss}}{\partial{P_{mF}^{(t)}}}\end{array}\right]\end{equation}

\begin{equation}\frac{\partial{Loss}}{\partial{P_{uf}^{(t)}}}=\sum_{\color{red}{i,r_{ui}\ne0}}{-2(r_{u,i}-\hat{r}_{ui})\frac{\partial{\hat{r}_{ui}}}{\partial{P_{uf}^{(t)}}}}+2\lambda{P_{uf}^{(t)}}=\sum_{\color{red}{i,r_{ui}\ne0}}{-2(r_{u,i}-\hat{r}_{ui})Q_{fi}^{(t)}}+2\lambda{P_{uf}^{(t)}}\end{equation}

\begin{equation}\frac{\partial{Loss}}{\partial{Q_{fi}^{(t)}}}=\sum_{\color{red}{u,r_{ui}\ne0}}{-2(r_{u,i}-\hat{r}_{ui})\frac{\partial{\hat{r}_{ui}}}{\partial{Q_{fi}^{(t)}}}}+2\lambda{Q_{fi}^{(t)}}=\sum_{\color{red}{u,r_{ui}\ne0}}{-2(r_{u,i}-\hat{r}_{ui})P_{uf}^\color{red}{(t)}}+2\lambda{Q_{fi}^{(t)}}\end{equation}

以上就是梯度下降法的所有公式，我们注意到：
- 求$\frac{\partial{Loss}}{\partial{P_{uf}^{(t)}}}$时用到了用户$u$对物品的所有评分
- 求$\frac{\partial{Loss}}{\partial{P^{(t)}}}$时用到了整个评分矩阵$R$，时间复杂度为$m\times{F}\times{n'}$，$n'$是平均一个用户对多少个物品有过评分

随机梯度下降法(SGD,Stochastic Gradient Descent)没有严密的理论证明，但是在实践中它通常比传统的梯度下降法需要更少的迭代次数就可以收敛，它有两个特点：
- 单独更新参数$P_{uf}^{(t+1)}=P_{uf}^{(t)}-\alpha\frac{\partial{Loss}}{\partial{P_{uf}^{(t)}}}$，而原始的梯度下降法要整体更新参数$P^{(t+1)}=P^{(t)}-\alpha\frac{\partial{Loss}}{\partial{P^{(t)}}}$。在$t+1$轮次中计算其他参数的梯度时直接使用$P_{uf}$的最新值$P_{uf}^{(t+1)}$
- 计算$\frac{\partial{Loss}}{\partial{P_{uf}^{(t)}}}$时只利用用户$u$对一个物品的评分，而不是利用用户$u$的所有评分，即
\begin{equation}\frac{\partial{Loss}}{\partial{P_{uf}^{(t)}}}=-2(r_{u,i}-\hat{r}_{ui})Q_{fi}^{(t)}+2\lambda{P_{uf}^{(t)}}\end{equation}
从而\begin{equation}P_{uf}^{(t+1)}=P_{uf}^{(t)}+\alpha[(r_{u,i}-\hat{r}_{ui})Q_{fi}^{(t)}-\lambda{P_{uf}^{(t)}}]\label{pp}\end{equation}
同理可得\begin{equation}Q_{fi}^{(t+1)}=Q_{fi}^{(t)}+\alpha[(r_{u,i}-\hat{r}_{ui})P_{uf}^\color{red}{(t+1)}-\lambda{Q_{fi}^{(t)}}]\label{pq}\end{equation}

SGD单轮迭代的时间复杂度也是$m\times{F}\times{n'}$，但由于它是单个参数地更新，且更新单个参数时只利用到一个样本（一个评分），更新后的参数立即可用于更新剩下的参数，所以SGD比批量的梯度下降需要更少的迭代次数。

在训练模型的时候我们只要求模型尽量拟合$r_{ui}\ne{0}$的情况，对于$r_{ui}=0$的情况我们也不希望$\hat{r}_{ui}=0$，因为$r_{ui}=0$只表示用户$u$没有对物品$i$评分，并不代表用$u$户对物品$i$的喜好程度为0。而恰恰$\hat{r}_{ui}$能反映用$u$户对物品$i$的喜好程度，对所有$\hat{r}_{ui}(i\in{\{1,2,...,n\}})$降序排列，取出topK就是用户$u$的推荐列表。

LFM.py

# coding:utf-8
__author__ = "orisun"

import random
import math


class LFM(object):

    def __init__(self, rating_data, F, alpha=0.1, lmbd=0.1, max_iter=500):
        '''rating_data是list<(user,list<(position,rate)>)>类型
        '''
        self.F = F
        self.P = dict()  # R=PQ^T，代码中的Q相当于博客中Q的转置
        self.Q = dict()
        self.alpha = alpha
        self.lmbd = lmbd
        self.max_iter = max_iter
        self.rating_data = rating_data

        '''随机初始化矩阵P和Q'''
        for user, rates in self.rating_data:
            self.P[user] = [random.random() / math.sqrt(self.F)
                            for x in xrange(self.F)]
            for item, _ in rates:
                if item not in self.Q:
                    self.Q[item] = [random.random() / math.sqrt(self.F)
                                    for x in xrange(self.F)]

    def train(self):
        '''随机梯度下降法训练参数P和Q
        '''
        for step in xrange(self.max_iter):
            for user, rates in self.rating_data:
                for item, rui in rates:
                    hat_rui = self.predict(user, item)
                    err_ui = rui - hat_rui
                    for f in xrange(self.F):
                        self.P[user][f] += self.alpha * (err_ui * self.Q[item][f] - self.lmbd * self.P[user][f])
                        self.Q[item][f] += self.alpha * (err_ui * self.P[user][f] - self.lmbd * self.Q[item][f])
            self.alpha *= 0.9  # 每次迭代步长要逐步缩小

    def predict(self, user, item):
        '''预测用户user对物品item的评分
        '''
        return sum(self.P[user][f] * self.Q[item][f] for f in xrange(self.F))

if __name__ == '__main__':
    '''用户有A B C，物品有a b c d'''
    rating_data = list()
    rate_A = [('a', 1.0), ('b', 1.0)]
    rating_data.append(('A', rate_A))
    rate_B = [('b', 1.0), ('c', 1.0)]
    rating_data.append(('B', rate_B))
    rate_C = [('c', 1.0), ('d', 1.0)]
    rating_data.append(('C', rate_C))

    lfm = LFM(rating_data, 2)
    lfm.train()
    for item in ['a', 'b', 'c', 'd']:
        print item, lfm.predict('A', item)		#计算用户A对各个物品的喜好程度


输出：

a 0.860198578815
b 0.901207650363
c 0.853149604409
d 0.814338291689

## SVD

在公式$\ref{lfm}$中加入偏置项：

\begin{equation}\hat{r}_{ui}=\sum_{f=1}^{F}{P_{uf}Q_{fi}}+\mu+b_u+b_i\label{bias_lfm}\end{equation}

$\mu$表示训练集中的所有评分的平均值。$b_u$是用户偏置，代表一个用户评分的平均值。$b_i$是物品偏置，代表一个物品被评分的平均值。所以“偏置”这东西反应的是事物固有的、不受外界影响的属性，用公式$\ref{lfm}$去预估用户对物品的评分时没有考虑这个用户是宽容的还是苛刻的，他倾向于给物品打高分还是打低分，所以在公式$\ref{bias_lfm}$加入了偏置$b_u$。

带偏置的LFM又被称为SVD法，但到此为止我没有看到它跟奇异值分解的关系在哪里。

 $\mu$直接由训练集统计得到，$b_u$和$b_i$需要通过机器学习训练得来。对比公式$\ref{target_lfm}$此时我们目标函数变为

\begin{equation}min:\ \ Loss=\sum_{\color{red}{r_{ui}\ne0}}{(r_{u,i}-\hat{r}_{ui})^2}+\lambda(\sum{P_{uf}^2}+\sum{Q_{fi}^2}+\sum{b_u^2}+\sum{b_i^2})\end{equation}

由随机梯度下降法得到$b_u$和$b_i$的更新方法为

\begin{equation}b_u^{(t+1)}=b_u^{(t)}+\alpha*(r_{u,i}-\hat{r}_{ui}-\lambda*b_u^{(t)})\end{equation}

\begin{equation}b_i^{(t+1)}=b_i^{(t)}+\alpha*(r_{u,i}-\hat{r}_{ui}-\lambda*b_i^{(t)})\end{equation}

$P_{uf}$和$Q_{fi}$的更新方法不变，参见公式$\ref{pp}$和公式$\ref{pq}$。

初始化时把$b_u$和$b_i$全初始化为0即可。

biasLFM.py

# coding:utf-8
__author__ = "orisun"

import random
import math


class BiasLFM(object):

    def __init__(self, rating_data, F, alpha=0.1, lmbd=0.1, max_iter=500):
        '''rating_data是list<(user,list<(position,rate)>)>类型
        '''
        self.F = F
        self.P = dict()
        self.Q = dict()     #相当于博客中Q的转置
        self.bu = dict()
        self.bi = dict()
        self.alpha = alpha
        self.lmbd = lmbd
        self.max_iter = max_iter
        self.rating_data = rating_data
        self.mu = 0.0

        '''随机初始化矩阵P和Q'''
        cnt = 0
        for user, rates in self.rating_data:
            self.P[user] = [random.random() / math.sqrt(self.F)
                            for x in xrange(self.F)]
            self.bu[user] = 0
            cnt += len(rates)
            for item, rate in rates:
                self.mu += rate
                if item not in self.Q:
                    self.Q[item] = [random.random() / math.sqrt(self.F)
                                    for x in xrange(self.F)]
                self.bi[item] = 0
        self.mu /= cnt

    def train(self):
        '''随机梯度下降法训练参数P和Q
        '''
        for step in xrange(self.max_iter):
            for user, rates in self.rating_data:
                for item, rui in rates:
                    hat_rui = self.predict(user, item)
                    err_ui = rui - hat_rui
                    self.bu[user] += self.alpha * (err_ui - self.lmbd * self.bu[user])
                    self.bi[item] += self.alpha * (err_ui - self.lmbd * self.bi[item])
                    for f in xrange(self.F):
                        self.P[user][f] += self.alpha * (err_ui * self.Q[item][f] - self.lmbd * self.P[user][f])
                        self.Q[item][f] += self.alpha *  (err_ui * self.P[user][f] - self.lmbd * self.Q[item][f])
            self.alpha *= 0.9  # 每次迭代步长要逐步缩小

    def predict(self, user, item):
        '''预测用户user对物品item的评分
        '''
        return sum(self.P[user][f] * self.Q[item][f] for f in xrange(self.F)) + self.bu[user] + self.bi[item] + self.mu

if __name__ == '__main__':
    '''用户有A B C，物品有a b c d'''
    rating_data = list()
    rate_A = [('a', 1.0), ('b', 1.0)]
    rating_data.append(('A', rate_A))
    rate_B = [('b', 1.0), ('c', 1.0)]
    rating_data.append(('B', rate_B))
    rate_C = [('c', 1.0), ('d', 1.0)]
    rating_data.append(('C', rate_C))

    lfm = BiasLFM(rating_data, 2)
    lfm.train()
    for item in ['a', 'b', 'c', 'd']:
        print item, lfm.predict('A', item)  # 计算用户A对各个物品的喜好程度

## SVD++

由BiasLFM(即SVD)继续演化就可以得到SVD++，虽然在SVD++里我还是没看到奇异值分解的影子。

SVD++认为任何用户只要对物品$i$有过评分，不论评分是多少，就已经在一定程度上反应了他对各个隐因子的喜好程度$y_i=(y_{i1},y_{i2},...,y_{iF},)$，$y$是物品所携带的属性，就如同$Q$一样。在公式$\ref{bias_lfm}$的基础上，SVD++得出了：

\begin{equation}\hat{r}_{ui}=\sum_{f=1}^{F}{(P_{uf}+\frac{\sum_{j\in{N(u)}}{Y_{jf}}}{\sqrt{|N(u)|}})Q_{fi}}+\mu+b_u+b_i\label{svdpp}\end{equation}

$N(u)$是用户$u$评价过的物品集合。

跟上文讲的一样，还是基于评分的误差平方和建立目标函数，正则项里加一个$\lambda\sum{Y_{jf}^2}$，采用随机梯度下降法解这个优化问题。$\hat{r_{ui}}$对$b_u$、$b_i$、$P_{uf}$的偏导都跟BiasLFM中的一样，而$\frac{\partial{\hat{r_{ui}}}}{\partial{Q_{fi}}}$会有变化

\begin{equation}\frac{\partial{\hat{r_{ui}}}}{\partial{Q_{fi}}}=P_{uf}+\frac{\sum_{j\in{N(u)}}{Y_{jf}}}{\sqrt{|N(u)|}}\end{equation}

另外引入了$Y$矩阵，所以也需要对$Y_{jf}$求偏导

\begin{equation}\frac{\partial{\hat{r_{ui}}}}{\partial{Y_{jf}}}=\frac{Q_{fi}}{\sqrt{|N(u)|}}\end{equation}

svdpp.py

# coding:utf-8
__author__ = "orisun"

import random
import math


class SVDPP(object):

    def __init__(self, rating_data, F, alpha=0.1, lmbd=0.1, max_iter=500):
        '''rating_data是list<(user,list<(position,rate)>)>类型
        '''
        self.F = F
        self.P = dict()
        self.Q = dict()  # 相当于博客中Q的转置
        self.Y = dict()
        self.bu = dict()
        self.bi = dict()
        self.alpha = alpha
        self.lmbd = lmbd
        self.max_iter = max_iter
        self.rating_data = rating_data
        self.mu = 0.0

        '''随机初始化矩阵P、Q、Y'''
        cnt = 0
        for user, rates in self.rating_data:
            self.P[user] = [random.random() / math.sqrt(self.F)
                            for x in xrange(self.F)]
            self.bu[user] = 0
            cnt += len(rates)
            for item, rate in rates:
                self.mu += rate
                if item not in self.Q:
                    self.Q[item] = [random.random() / math.sqrt(self.F)
                                    for x in xrange(self.F)]
                if item not in self.Y:
                    self.Y[item] = [random.random() / math.sqrt(self.F)
                                    for x in xrange(self.F)]
                self.bi[item] = 0
        self.mu /= cnt

    def train(self):
        '''随机梯度下降法训练参数P和Q
        '''
        for step in xrange(self.max_iter):
            for user, rates in self.rating_data:
                z = [0.0 for f in xrange(self.F)]
                for item, _ in rates:
                    for f in xrange(self.F):
                        z[f] += self.Y[item][f]
                ru = 1.0 / math.sqrt(1.0 * len(rates))
                s = [0.0 for f in xrange(self.F)]
                for item, rui in rates:
                    hat_rui = self.predict(user, item, rates)
                    err_ui = rui - hat_rui
                    self.bu[user] += self.alpha *  (err_ui - self.lmbd * self.bu[user])
                    self.bi[item] += self.alpha *  (err_ui - self.lmbd * self.bi[item])
                    for f in xrange(self.F):
                        s[f] += self.Q[item][f] * err_ui
                        self.P[user][f] += self.alpha *   (err_ui * self.Q[item][f] - self.lmbd * self.P[user][f])
                        self.Q[item][f] += self.alpha * (err_ui * (self.P[user][f] + z[f] * ru) - self.lmbd * self.Q[item][f])
                for item, _ in rates:
                    for f in xrange(self.F):
                        self.Y[item][f] += self.alpha *  (s[f] * ru - self.lmbd * self.Y[item][f])
            self.alpha *= 0.9  # 每次迭代步长要逐步缩小

    def predict(self, user, item, ratedItems):
        '''预测用户user对物品item的评分
        '''
        z = [0.0 for f in xrange(self.F)]
        for ri, _ in ratedItems:
            for f in xrange(self.F):
                z[f] += self.Y[ri][f]
        return sum((self.P[user][f] + z[f] / math.sqrt(1.0 * len(ratedItems))) * self.Q[item][f] for f in xrange(self.F)) + self.bu[user] + self.bi[item] + self.mu

if __name__ == '__main__':
    '''用户有A B C，物品有a b c d'''
    rating_data = list()
    rate_A = [('a', 1.0), ('b', 1.0)]
    rating_data.append(('A', rate_A))
    rate_B = [('b', 1.0), ('c', 1.0)]
    rating_data.append(('B', rate_B))
    rate_C = [('c', 1.0), ('d', 1.0)]
    rating_data.append(('C', rate_C))

    lfm = SVDPP(rating_data, 2)
    lfm.train()
    for item in ['a', 'b', 'c', 'd']:
        print item, lfm.predict('A', item, rate_A)  # 计算用户A对各个物品的喜好程度















