# FFM代码实现 - Orisun - 博客园







# [FFM代码实现](https://www.cnblogs.com/zhangchaoyang/articles/8410719.html)





上一篇我们讲了《[FFM原理及公式推导](http://www.cnblogs.com/zhangchaoyang/articles/8157893.html)》，现在来编码实现一下。

把写代码所需要所有公式都列出来，各符号与原文《[Field-aware Factorization Machines for CTR Prediction](https://www.csie.ntu.edu.tw/~cjlin/papers/ffm.pdf)》中的保持一致。

## 符号约定：

$n$:特征的维数

$m$:域的个数

$k$:隐向量的维度

$j$:在特征中的下标

$f$:在域中的下标

$d$:在隐向量中的下标

$l$:样本的总数

粗体字母表示向量或矩阵

## 特征组合

### 最基本的线性加权

$$\phi_{LM}(\textbf{w},\textbf{x})=\sum_{i=1}^n{w_ix_i}$$

### 任意特征两两组合

$$\phi_{poly2}(\textbf{w},\textbf{x})=\sum_{j1=1}^n{\sum_{j2=j1+1}^n{w_{j1,j2}x_{j1}x_{j2}}}$$

$\textbf{w}$是一个对称方阵，即$w_{j1,j2}=w_{j2,j1}$，可以用矩阵分解法来拟合$\textbf{w}$。

$$w_{j1,j2}=\textbf{v}_{j1}\cdot\textbf{v}_{j2}=\textbf{v}_{j2}\cdot\textbf{v}_{j1}=w_{j2,j1}$$

矩阵$\textbf{w}$的规模是$n\times n$，矩阵$\textbf{v}$的规模是$n\times k$，$k\ll n$。实际上我们已经推导出了因子分解法。

### 因子分解法FM

$$\phi_{FM}(\textbf{w},\textbf{x})=\sum_{j1=1}^n{\sum_{j2=j1+1}^n{\textbf{w}_{j1}\cdot \textbf{w}_{j2}x_{j1}x_{j2}}}$$

这里的$\textbf{w}_j$相当于上面的$\textbf{v}_j$。

### 域感知的因子分解法FFM

$$\phi_{FFM}(\textbf{w},\textbf{x})=\sum_{j1=1}^n{\sum_{j2=j1+1}^n{\textbf{w}_{j1,f2}\cdot \textbf{w}_{j2,f1}x_{j1}x_{j2}}}$$

在FM中$\textbf{w}$是规模为$n\times k_{FM}$的二维矩阵，而在FFM中$\textbf{w}$是规模为$n\times m \times k_{FFM}$的三维矩阵，$k_{FFM}  \ll k_{FM}$。

## 逻辑回归二分类

### 决策函数

$$\hat{y}=\frac{1}{1+exp(\phi_{FFM}(\textbf{w},\textbf{x}))}$$

### 带L2正则的目标函数

$$\min_{\textbf{w}}\;\;\frac{\lambda}{2}\parallel w\parallel_2^2+\sum_{i=1}^llog(1+exp(-y_i\phi_{FFM}(\textbf{w},\textbf{x}_i)))$$

其中$y_i \in \{-1,1\}$

在SGD中每次只需要考虑一个样本的损失，此时目标函数为

$$\min_{\textbf{w}}\;\;\frac{\lambda}{2}\parallel w\parallel_2^2+log(1+exp(-y\phi_{FFM}(\textbf{w},\textbf{x})))$$ 

### 梯度

$$\textbf{g}_{j1,f2}=\lambda\cdot\textbf{w}_{j1,f2}+\kappa\cdot\textbf{w}_{j2,f1}x_{j1}x_{j2}$$ 

$$\textbf{g}_{j2,f1}=\lambda\cdot\textbf{w}_{j2,f1}+\kappa\cdot\textbf{w}_{j1,f2}x_{j1}x_{j2}$$ 

梯度之所会这么简单，依赖一个很重要的前提：同一个域下的各个特征只有一个是非0值。

其中

$$\kappa=\frac{\partial log(1+exp(-y\phi_{FFM}(\textbf{w},\textbf{x})))}{\partial\phi_{FFM}(\textbf{w},\textbf{x})}=\frac{-y}{1+exp(y\phi_{FFM}(\textbf{w},\textbf{x}))}$$

### AdaGrad更新w

至于为什么要用AdaGrad替代传统的梯度下降法，请参见我之前写的《[优化方法](https://wenku.baidu.com/view/435728f032d4b14e852458fb770bf78a64293a53)》。

$$(G_{j1,f2})_d\gets(G_{j1,f2})_d+(g_{j1,f2})_d^2$$

$$(G_{j2,f1})_d\gets(G_{j2,f1})_d+(g_{j2,f1})_d^2$$

$$(w_{j1,f2})_d\gets(w_{j1,f2})_d-\frac{\eta}{\sqrt{(G_{j1,f2})_d}}(g_{j1,f2})_d$$

$$(w_{j2,f1})_d\gets(w_{j2,f1})_d-\frac{\eta}{\sqrt{(G_{j2,f1})_d}}(g_{j2,f1})_d$$

初始化$G_d=1$，这样在计算$\frac{\eta}{\sqrt{G_d}}$时既可以防止分母为0，又可以避免该项太大或太小。

$\eta$是学习率，通常可取0.01。

初始的$\textbf{w}$可以从均匀分布中抽样$\textbf{w}\sim U(0,\frac{1}{\sqrt{k}})$

实现发现将每个$\textbf{x}$归一化，即模长为1，在测试集得到的准确率会稍微好一点且对参数不太敏感。

## 代码实现

只要把公式推导搞明白了，写代码就非常容易了，直接把公式直译成代码即可。

# -*- coding: utf-8 -*-
# @Date    : 3/2/18
# @Author  : zhangchaoyang

import numpy as np

np.random.seed(0)
import math
from logistic import Logistic


class FFM_Node(object):
    '''
    通常x是高维稀疏向量，所以用链表来表示一个x，链表上的每个节点是个3元组(j,f,v)
    '''
    __slots__ = ['j', 'f', 'v']  # 按元组（而不是字典）的方式来存储类的成员属性

    def __init__(self, j, f, v):
        '''
        :param j: Feature index (0 to n-1)
        :param f: Field index (0 to m-1)
        :param v: value
        '''
        self.j = j
        self.f = f
        self.v = v


class FFM(object):
    def __init__(self, m, n, k, eta, lambd):
        '''
        :param m: Number of fields
        :param n: Number of features
        :param k: Number of latent factors
        :param eta: learning rate
        :param lambd: regularization coefficient
        '''
        self.m = m
        self.n = n
        self.k = k
        # 超参数
        self.eta = eta
        self.lambd = lambd
        # 初始化三维权重矩阵w~U(0,1/sqrt(k))
        self.w = np.random.rand(n, m, k) / math.sqrt(k)
        # 初始化累积梯度平方和为，AdaGrad时要用到，防止除0异常
        self.G = np.ones(shape=(n, m, k), dtype=np.float64)
        self.log = Logistic()

    def phi(self, node_list):
        '''
        特征组合式的线性加权求和
        :param node_list: 用链表存储x中的非0值
        :return:
        '''
        z = 0.0
        for a in xrange(len(node_list)):
            node1 = node_list[a]
            j1 = node1.j
            f1 = node1.f
            v1 = node1.v
            for b in xrange(a + 1, len(node_list)):
                node2 = node_list[b]
                j2 = node2.j
                f2 = node2.f
                v2 = node2.v
                w1 = self.w[j1, f2]
                w2 = self.w[j2, f1]
                z += np.dot(w1, w2) * v1 * v2
        return z

    def predict(self, node_list):
        '''
        输入x，预测y的值
        :param node_list: 用链表存储x中的非0值
        :return:
        '''
        z = self.phi(node_list)
        y = self.log.decide_by_tanh(z)
        return y

    def sgd(self, node_list, y):
        '''
        根据一个样本来更新模型参数
        :param node_list: 用链表存储x中的非0值
        :param y: 正样本1，负样本-1
        :return:
        '''
        kappa = -y / (1 + math.exp(y * self.phi(node_list)))
        for a in xrange(len(node_list)):
            node1 = node_list[a]
            j1 = node1.j
            f1 = node1.f
            v1 = node1.v
            for b in xrange(a + 1, len(node_list)):
                node2 = node_list[b]
                j2 = node2.j
                f2 = node2.f
                v2 = node2.v
                c = kappa * v1 * v2
                # self.w[j1,f2]和self.w[j2,f1]是向量，导致g_j1_f2和g_j2_f1也是向量
                g_j1_f2 = self.lambd * self.w[j1, f2] + c * self.w[j2, f1]
                g_j2_f1 = self.lambd * self.w[j2, f1] + c * self.w[j1, f2]
                # 计算各个维度上的梯度累积平方和
                self.G[j1, f2] += g_j1_f2 ** 2  # 所有G肯定是大于0的正数，因为初始化时G都为1
                self.G[j2, f1] += g_j2_f1 ** 2
                # AdaGrad
                self.w[j1, f2] -= self.eta / np.sqrt(self.G[j1, f2]) * g_j1_f2  # sqrt(G)作为分母，所以G必须是大于0的正数
                self.w[j2, f1] -= self.eta / np.sqrt(
                    self.G[j2, f1]) * g_j2_f1  # math.sqrt()只能接收一个数字作为参数，而numpy.sqrt()可以接收一个array作为参数，表示对array中的每个元素分别开方

    def train(self, sample_generator, max_echo, max_r2):
        '''
        根据一堆样本训练模型
        :param sample_generator: 样本生成器，每次yield (node_list, y)，node_list中存储的是x的非0值。通常x要事先做好归一化，即模长为1，这样精度会略微高一点
        :param max_echo: 最大迭代次数
        :param max_r2: 拟合系数r2达到阈值时即可终止学习
        :return:
        '''
        for itr in xrange(max_echo):
            print "echo", itr
            y_sum = 0.0
            y_square_sum = 0.0
            err_square_sum = 0.0  # 误差平方和
            population = 0  # 样本总数
            for node_list, y in sample_generator:
                y = 0.0 if y == -1 else y  # 真实的y取值为{-1,1}，而预测的y位于(0,1)，计算拟合效果时需要进行统一
                self.sgd(node_list, y)
                y_hat = self.predict(node_list)
                y_sum += y
                y_square_sum += y ** 2
                err_square_sum += (y - y_hat) ** 2
                population += 1
            var_y = y_square_sum - y_sum * y_sum / population  # y的方差
            r2 = 1 - err_square_sum / var_y
            print "r2=",r2
            if r2 > max_r2:  # r2值越大说明拟合得越好
                print 'r2 have reach', r2
                break

    def save_model(self, outfile):
        '''
        序列化模型
        :param outfile:
        :return:
        '''
        np.save(outfile, self.w)

    def load_model(self, infile):
        '''
        加载模型
        :param infile:
        :return:
        '''
        self.w = np.load(infile)


完整代码见 [https://github.com/Orisun/ffm](https://github.com/Orisun/ffm)














