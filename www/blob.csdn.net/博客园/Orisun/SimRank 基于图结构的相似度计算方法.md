# SimRank:基于图结构的相似度计算方法 - Orisun - 博客园







# [SimRank:基于图结构的相似度计算方法](https://www.cnblogs.com/zhangchaoyang/articles/4575809.html)





## 简单理解SimRank

![](https://images0.cnblogs.com/blog2015/103496/201506/142131387234346.png)

图1.二部图

所谓二部图(bipartite graphs)，是指图中的节点可以分这两个子集，任意一条边关联的两个节点分别来自于这两个子集。用I(v)和O(v)分别表示节点v的in-neighbors和out-neighbors。看上面的二部图，我们把A、B当成两个人，把a、b、c当成三件商品，有向边代表人购买的商品。simrank的基本思想是：如果两个实体相似，那么跟它们相关的实体应该也相似。比如在上图中如果a和c相似，那么A和B应该也相似，因为A和a相关，而B和c相关。

SimRank的基本公式：

\begin{equation}s(a,b)=\frac{C}{|I(a)||I(b)|}\sum_{i=1}^{|I(a)|}\sum_{j=1}^{|I(b)|}s(I_i(a),I_j(b))\label{basic}\end{equation}

s(a,b)是节点a和b的相似度，当a=b时，s(a,b)=1。$I_i(a)$表示a的第i个in-neighbor。当$I(a)=\emptyset$或$I(b)=\emptyset$时式\eqref{basic}为0。\eqref{basic}式用一句话描述就是：a和b的相似度等于a的in-neighbors和b的in-neighbors相似度的平均值。参数C是个阻尼系数，它的含义可以这么理解：假如I(a)=I(b)={A}，按照\eqref{basic}式计算出sim(a,b)=C*sim(A,A)=C，所以$C\in(0,1)$。

把式\eqref{basic}应用于图1所示的二部图就是：

\begin{equation}s(A,B)=\frac{C_1}{|O(A)||O(B)|}\sum_{i=1}^{|O(A)|}\sum_{j=1}^{|O(B)|}s(O_i(A),O_j(B))\ \ \ \ for\ A\ne{B}\label{out}\end{equation}

\begin{equation}s(a,b)=\frac{C_2}{|I(a)||I(b)|}\sum_{i=1}^{|I(a)|}\sum_{j=1}^{|I(b)|}s(I_i(a),I_j(b))\ \ \ \ for\ a\ne{b}\label{in}\end{equation}

忽略$C_1$和$C_2$，\eqref{out}式是说买家A和B的相似度等于他们购买的物品之间相似度的平均值，\eqref{in}式是说物品a和b的相似度是购买它们的买家之间相似度的平均值。

对于非二部图的情况，一个节点既可能有in-neighbors也可能有out-neighbors，比如在论文引用的场景下，一篇论文既可能引用其他论文，也可能被其他论文引用。站在引用的角度，两篇论文的相似度为

\begin{equation}s_1(a,b)=\frac{C_1}{|O(a)||O(b)|}\sum_{i=1}^{|O(a)|}\sum_{j=1}^{|O(b)|}s_2(O_i(a),O_j(b))\label{hout}\end{equation}

站在被引用的角度，两篇论文的相似度为

\begin{equation}s_2(a,b)=\frac{C_2}{|I(a)||I(b)|}\sum_{i=1}^{|I(a)|}\sum_{j=1}^{|I(b)|}s_1(I_i(a),I_j(b))\label{hin}\end{equation}

根据实际的应用场景，最终的s(a,b)可以采用\eqref{hout}和\eqref{hin}其中的任何一个，或者是两者的综合。

## Naive SimRank

SimRank迭代算法：

$R_0(a,b)=\left\{\begin{matrix}0\ \ \ \ if\ a\ne{b}\\1\ \ \ \ if\ a=b\end{matrix}\right.$

$R_{k+1}(a,b)=\left\{\begin{matrix}\frac{C}{|I(a)||I(b)|}\sum_{i=1}^{|I(a)|}\sum_{j=1}^{|I(b)|}R_k(I_i(a),I_j(b))\ \ \ \ if\ a\ne{b}\\1\ \ \ \ if\ a=b\end{matrix}\right.$

$R_k(*,*)$是k的单调不减函数，$lim_{k\to\infty}R_k(a,b)=s(a,b)$，实践中发现$R_k(*,*)$收敛得很快，k不需要设得太大。

 下面给出矩阵的形式，因为直接使用上面的迭代公式很难展开并行计算，数量稍微大一些（比如上十万）时在单机上跑时间和空间开销非常大。

\begin{equation}\left\{\begin{matrix}S^{(0)}=(1-c)\cdot{I_n}\\S^{(k+1)}=c\cdot{Q^T}\cdot{S^{(k)}}\cdot{Q}+(1-c)\cdot{I_n}\end{matrix}\right.\label{juzhen}\end{equation}

S是相似度矩阵。Q是转移概率矩阵，它的每一列和为1，如果从节点i可以转移到节点j，并且这样的节点i一共有n个，则$Q_{i,j}=\frac{1}{n}$

迭代误差的上界：

\begin{equation}\|S^{(k)}-S\|_{max}\le{c}^{k+1}\ \ \ \ (\vee{k}=0,1,2\ldots)\label{err_ceil}\end{equation}

矩阵的max范数定义为：

$\|X_{p\times{q}}\|_{max}\stackrel{def}{=}max_{i=1}^{p}max_{j=1}^{q}\mid{x}_{i,j}\mid$

好了，来做个小练习。图2是一张网页链接关系图，表示一所大学的主页上放了A、B两个教授的个人主页的链接，教授B和学生B的个人主页互相链接了对方，等等。下面我们要通过这种链接关系求这5个节点的相似度。

![](https://images0.cnblogs.com/blog2015/103496/201506/192218090605725.png)

图2. 网页链接关系图

首先用一个文本文件来存储上面的有向图。

linkgraph

univ	profA	profB
profA	studentA
studentA	univ
profB	studentB
studentB	profB


文件中每一行的首列是一个节点，后面的列是首列的out-neighbors，即在图上游走时只能顺着箭头的方向。对于二部图情况就不一样了，\eqref{out}式是顺着二部图箭头的方向，\eqref{in}式是逆着二部图箭头的方向，即每一条边都是允许双向游走的。于是图1所示的二部图可以表示为：

linkgraph_bipartite

A    a    b
B    b    c
a    A
b    A    B
c    B


naive_simrank.py

#!/usr/bin/env python
# coding=utf-8

import numpy as np
import scipy as sp

nodes = []  # 所有的节点存入数组
nodesnum = 0  # 所有节点的数目
nodes_index = {}  # <节点名，节点在nodes数组中的编号>
damp = 0.8  # 阻尼系数
trans_matrix = np.matrix(0)  # 转移概率矩阵
sim_matrix = np.matrix(0)  # 节点相似度矩阵


def initParam(graphFile):
    '''
    构建nodes、nodes_index、trans_matrix和第0代的sim_matrix.
    输入文件行格式要求：node\toutneighbor\toutneighbor\t...或 node\tinneighbor\tinneighbor\t...
    '''
    global nodes
    global nodes_index
    global trans_matrix
    global sim_matrix
    global damp
    global nodesnum

    link_in = {}
    for line in open(graphFile, "r", 1024):
        arr = line.strip("\n").split()
        node = arr[0]
        nodeid = -1
        if node in nodes_index:
            nodeid = nodes_index[node]
        else:
            nodeid = len(nodes)
            nodes_index[node] = nodeid
            nodes.append(node)
        for ele in arr[1:]:
            outneighbor = ele
            outneighborid = -1
            if outneighbor in nodes_index:
                outneighborid = nodes_index[outneighbor]
            else:
                outneighborid = len(nodes)
                nodes_index[outneighbor] = outneighborid
                nodes.append(outneighbor)
            inneighbors = []
            if outneighborid in link_in:
                inneighbors = link_in[outneighborid]
            inneighbors.append(nodeid)
            link_in[outneighborid] = inneighbors

    nodesnum = len(nodes)
    trans_matrix = np.zeros((nodesnum, nodesnum))
    for node, inneighbors in link_in.items():
        num = len(inneighbors)
        prob = 1.0 / num
        for neighbor in inneighbors:
            trans_matrix[neighbor, node] = prob

    sim_matrix = np.identity(nodesnum) * (1 - damp)


def iterate():
    '''
    迭代更新相似度矩阵
    '''
    global trans_matrix
    global sim_matrix
    global damp
    global nodesnum

    sim_matrix = damp * np.dot(np.dot(trans_matrix.transpose(),
                                      sim_matrix), trans_matrix) + (1 - damp) * np.identity(nodesnum)


def printResult(sim_node_file):
    '''
    打印输出相似度计算结果
    '''
    global sim_matrix
    global link_out
    global link_in
    global nodes
    global nodesnum

    # 打印node之间的相似度
    f_out_user = open(sim_node_file, "w")
    for i in range(nodesnum):
        f_out_user.write(nodes[i] + "\t")
        neighbour = []
        for j in range(nodesnum):
            if i != j:
                sim = sim_matrix[i, j]
                if sim == None:
                    sim = 0
                if sim > 0:
                    neighbour.append((j, sim))
        # 按相似度由大到小排序
        neighbour = sorted(
            neighbour, cmp=lambda x, y: cmp(x[1], y[1]), reverse=True)
        for (u, sim) in neighbour:
            f_out_user.write(nodes[u] + ":" + str(sim) + "\t")
        f_out_user.write("\n")
    f_out_user.close()


def simrank(graphFile, maxIteration):
    global nodes_index
    global trans_matrix
    global sim_matrix

    initParam(graphFile)
    print "nodes:"
    print nodes_index
    print "trans ratio:"
    print trans_matrix
    for i in range(maxIteration):
        print "iteration %d:" % (i + 1)
        iterate()
        print sim_matrix


if __name__ == '__main__':
    graphFile = "../data/linkgraph"
    sim_node_file = "../data/nodesim_naive"
    maxIteration = 10
    simrank(graphFile, maxIteration)
    printResult(sim_node_file)


最终得到5个节点两两之间的相似度

nodesim_naive

univ	profB:0.10803511296	studentB:0.02203058176	
profA	profB:0.36478881792	studentB:0.08159625216	
profB	profA:0.36478881792	univ:0.10803511296	studentB:0.0642220032	studentA:0.03022258176	
studentA	studentB:0.28216737792	profB:0.03022258176	
studentB	studentA:0.28216737792	profA:0.08159625216	profB:0.0642220032	univ:0.02203058176	


##  平方缓存法

\eqref{err_ceil}已经证明了simrank的收敛速度是非常快的，下面给出一种可以加速收敛的方法--平方缓存法。

\begin{equation}\left\{\begin{matrix}S_{\left \langle 2 \right \rangle }^{(0)}=(1-c)\cdot{I_n}\\S_{\left \langle 2 \right \rangle }^{(k+1)}=S_{\left \langle 2 \right \rangle }^{(k)}+c^{2^k}\cdot{(Q^{2^k})^T}\cdot{S_{\left \langle 2 \right \rangle }^{(k)}}\cdot{Q^{2^k}}\end{matrix}\right.\label{square_cache}\end{equation}

平方缓存法的收敛速度更惊人：

\begin{equation}\|S_{\left \langle 2 \right \rangle }^{(k)}-S\|_{max}\le{c}^{2^k}\ \ \ \ (\vee{k}=0,1,2\ldots)\label{square_err_ceil}\end{equation}

注意$Q^{2^k}=(Q^{2^{(k-1)}})^2$即每次迭代不必从头计算$Q^{2^k}$，只需要在上一次迭代的基础上平方一下就可以了，这其实就是\eqref{square_cache}比\eqref{juzhen}快的全部原因。事实上：

$S_{\left \langle 2 \right \rangle }^{(k)}=S^{(2^k-1)}$

我们用平方缓存法重新计算图2中各节点的相似度。

 square_cache_simrank.py

#!/usr/bin/env python
# coding=utf-8

import numpy as np
import scipy as sp

nodes = []  # 所有的节点存入数组
nodesnum = 0  # 所有节点的数目
nodes_index = {}  # <节点名，节点在nodes数组中的编号>
damp = 0.8  # 阻尼系数
trans_matrix = np.matrix(0)  # 转移概率矩阵
sim_matrix = np.matrix(0)  # 节点相似度矩阵


def initParam(graphFile):
    '''
    构建nodes、nodes_index、trans_matrix和第0代的sim_matrix.
    输入文件行格式要求：node\toutneighbor\toutneighbor\t...或 node\tinneighbor\tinneighbor\t...
    '''
    global nodes
    global nodes_index
    global trans_matrix
    global sim_matrix
    global damp
    global nodesnum

    link_in = {}
    for line in open(graphFile, "r", 1024):
        arr = line.strip("\n").split()
        node = arr[0]
        nodeid = -1
        if node in nodes_index:
            nodeid = nodes_index[node]
        else:
            nodeid = len(nodes)
            nodes_index[node] = nodeid
            nodes.append(node)
        for ele in arr[1:]:
            outneighbor = ele
            outneighborid = -1
            if outneighbor in nodes_index:
                outneighborid = nodes_index[outneighbor]
            else:
                outneighborid = len(nodes)
                nodes_index[outneighbor] = outneighborid
                nodes.append(outneighbor)
            inneighbors = []
            if outneighborid in link_in:
                inneighbors = link_in[outneighborid]
            inneighbors.append(nodeid)
            link_in[outneighborid] = inneighbors

    nodesnum = len(nodes)
    trans_matrix = np.zeros((nodesnum, nodesnum))
    for node, inneighbors in link_in.items():
        num = len(inneighbors)
        prob = 1.0 / num
        for neighbor in inneighbors:
            trans_matrix[node, neighbor] = prob

    sim_matrix = np.identity(nodesnum) * (1 - damp)


def iterate():
    '''
    迭代更新相似度矩阵
    '''
    global trans_matrix
    global sim_matrix
    global damp
    global nodesnum

    damp=damp**2
    trans_matrix=np.dot(trans_matrix,trans_matrix)
    sim_matrix = damp * np.dot(np.dot(trans_matrix,
                                      sim_matrix), trans_matrix.transpose()) + sim_matrix


def printResult(sim_node_file):
    '''
    打印输出相似度计算结果
    '''
    global sim_matrix
    global link_out
    global link_in
    global nodes
    global nodesnum

    # 打印node之间的相似度
    f_out_user = open(sim_node_file, "w")
    for i in range(nodesnum):
        f_out_user.write(nodes[i] + "\t")
        neighbour = []
        for j in range(nodesnum):
            if i != j:
                sim = sim_matrix[i, j]
                if sim == None:
                    sim = 0
                if sim > 0:
                    neighbour.append((j, sim))
        # 按相似度由大到小排序
        neighbour = sorted(
            neighbour, cmp=lambda x, y: cmp(x[1], y[1]), reverse=True)
        for (u, sim) in neighbour:
            f_out_user.write(nodes[u] + ":" + str(sim) + "\t")
        f_out_user.write("\n")
    f_out_user.close()


def simrank(graphFile, maxIteration):
    global nodes_index
    global trans_matrix
    global sim_matrix

    initParam(graphFile)
    print "nodes:"
    print nodes_index
    print "trans ratio:"
    print trans_matrix
    for i in range(maxIteration):
        print "iteration %d:" % (i + 1)
        iterate()
        print sim_matrix


if __name__ == '__main__':
    graphFile = "../data/linkgraph"
    sim_node_file = "../data/nodesim_square"
    maxIteration = 10
    simrank(graphFile, maxIteration)
    printResult(sim_node_file)


最终算得的节点相似度虽然跟naive方法不一致，但相似度排序跟naive方法是完全一致的。

## Arnoldi迭代降维法

采用\eqref{square_cache}式每次迭代都要作矩阵的乘法，当矩阵稍微大一些时对时间和空间的消耗是非常惊人的，两个$n\times{n}$的方阵相乘时间复杂度为$n^3$，即使采用[strassen](https://en.wikipedia.org/wiki/Strassen_algorithm)分治法也需要$O(n^{2.81})$的时间复杂度，况且strassen要求两个矩阵是方阵且边长为2的整次幂。如果我们能够对\eqref{square_cache}式中的Q和S进行降维，势必会节省大量计算时间。

Arnoldi迭代法是一种对高维稀疏矩阵（说的是上文中的转移概率矩阵Q）进行降维的方法。

\begin{equation}Q=V\cdot{H}\cdot{V^T}\label{decompose}\end{equation}

其中V的各列正交，且模长为1。$V=[v_0\mid{v_1}\mid\ldots{v}_{r-1}]$，其中$v_i$是V的第i列。

H是$r\times{r}$的上三角矩阵，r是Q的秩，形如：
$H_{r\times{r}}=\begin{bmatrix}h_{1,1} & h_{1,2} & h_{1,3} & \cdots & h_{1,r}\\h_{2,1} & h_{2,2} & h_{2,3} & \cdots & h_{2,r}\\0 & h_{3,2} & h_{3,3} & \cdots & h_{3,r}\\\vdots & \ddots & \ddots & \ddots & \vdots\\0 & \cdots  & 0 & h_{r,r-1} & h_{r,r}\end{bmatrix}$

V和H的计算流程如下：

$step1.$

$v_0=[1,0,0,\ldots]^T$

$step2.$

$for\ k \in [1,\alpha]$

$\ \ \ \ v_k=Q\cdot{v_{k-1}}$

$\ \ \ \ for\ j \in [0,k)$

$\ \ \ \ \ \ \ \ H[j][k-1]=v_j^T\cdot{v_k}$

$\ \ \ \ \ \ \ \ v_k=v_k-H[j][k-1]\cdot{v_j}$

$\ \ \ \ norm2=\|v_k\|_2$

$\ \ \ \ if\ norm2=0$

$\ \ \ \ \ \ \ \ break$

$\ \ \ \ H[k][k-1]=norm2$

$\ \ \ \ v_k=\frac{v_k}{norm2}$

$step3.$

$V舍弃最后一列，H舍弃最后一行$

在step2中，如果是由于norm2=0导致的最外层for循退出，则得到的H的边长为Q的秩即r；如果是由于达到了人为设定的循环上限$\alpha$，则得到的H的边长为$\alpha$。

arnoldi.py

#!/usr/bin/env pyton
# coding=utf-8

import numpy as np
from sparse_matrix import SparseMatrix

def arnoldi_iteration(Q, n, rank):
	'''
	对Q进行分解，QV=VH。
	Q是输入参数，numpy.matrix类型，n行n列，Q的秩为r。
	V和H都是输出参数，numpy.matrix类型。
	V是n行r+1列，每列模长为1且各列正交。V的转置与逆相等。
	H是r+1行r列的上三角矩阵。
	rank用于限制循环次数，r<=rank。
	'''
	if rank > n or rank <= 0:
		rank = n
	V = np.zeros((n, 1))
	V[0, 0] = 1
	h_col_list=[]
	k = 1
	while k <= rank:
		h_col = []
		v_k = Q.mulMatrixCol(V,k-1)
		for j in range(k):
			product = np.dot(np.matrix(V[:,j]).reshape(n,1).transpose(), v_k)[0,0]
			h_col.append(product)
			v_k = v_k - product * (np.matrix(V[:,j]).reshape(n,1))
		norm2 = np.linalg.norm(v_k, ord=2)
		if norm2 == 0:
			print "norm2=0, will break"
			break
		h_col.append(norm2)
		h_col_list.append(h_col)
		v_k = v_k / norm2 
		V = np.hstack((V, np.matrix(v_k)))
		k += 1
	r = len(h_col_list)
	print "r=", r
	H = np.zeros((r, r))
	for i in range(r):
		h_col = h_col_list[i]
		for j in range(len(h_col)):
				if j < r:
					H[j, i] = h_col[j]
	V = V[:, :r]
	return (V, H)

if __name__ == '__main__':
	Q=SparseMatrix()
	Q.set(0,1,0.5)
	Q.set(0,2,1)
	Q.set(1,0,0.5)
	Q.set(1,3,1)
	Q.set(2,0,0.5)
	Q.set(3,1,0.5)
	(V,H)=arnoldi_iteration(Q,4,-1)
	print "V="
	print V
	print "H="
	print H
	print "VHV^T="
	print np.dot(np.dot(V,H),V.transpose())
	print "QV="
	print Q.mulMatrix(V)
	print "VH="
	print np.dot(V,H)

sparse_matrix.py

#!/usr/bin/env python
# coding=utf-8

import numpy as np


class SparseMatrix():

    '''
    用dict实现高度稀疏的矩阵，dict的key是元素在matrix中的二维坐标，dict的value是元素的值
    '''

    def __init__(self):
        self.arr = {}

    def get(self, row, col):
        key = (row, col)
        if key in self.arr:
            return self.arr[key]
        else:
            return 0

    def set(self, row, col, value):
        key = (row, col)
        self.arr[key] = value

    def mul(self, vec):
        '''
        与一个一维向量相乘，返回一个list
        '''
        length = len(vec)
        rect = [0] * length
        for k, v in self.arr.items():
            i = k[0]
            j = k[1]
            rect[i] += v * vec[j]
        return rect

    def mulMatrixCol(self, matrix, col):
        '''
        与矩阵的第col列相乘,返回一个n*1的矩阵
        '''
        length = matrix.shape[0]
        rect = np.zeros((length, 1))
        for k, v in self.arr.items():
            i = k[0]
            j = k[1]
            rect[i, 0] += v * matrix[j, col]
        return rect

    def mulMatrix(self, matrix):
        '''
        与一个矩阵相乘
        '''
        col_num = matrix.shape[1]
        rect = self.mulMatrixCol(matrix, 0)
        for i in range(1,col_num):
            rect = np.hstack((rect, self.mulMatrixCol(matrix, i)))
        return rect

    def transmul(self, vec):
        '''
        矩阵转置后与一维向量相乘
        '''
        length = len(vec)
        rect = [0] * length
        for k, v in self.arr.items():
            i = k[1]
            j = k[0]
            rect[i] += v * vec[i]
        return rect


用H代替\eqref{square_cache}中的Q，用H的边长代替\eqref{square_cache}中的n，执行SimRank迭代。迭代后得到的$\hat{S}$实际上就是S在krylov子空间上的映射，按照\eqref{back}式把$\hat{S}$还原回S。

\begin{equation}S=V\cdot{\hat{S}}\cdot{V^T}\label{back}\end{equation}

krylov_simrank.py

#!/usr/bin/env python
# coding=utf-8

import numpy as np
import scipy as sp
import arnoldi
from sparse_matrix import SparseMatrix
from topk import TopkHeap

nodes = []  # 所有的节点存入数组
nodesnum = 0  # 所有节点的数目
nodes_index = {}  # <节点名，节点在nodes数组中的编号>
damp = 0.8  # 阻尼系数
sim_matrix = np.matrix(0)  # 在krylov子空间上的节点相似度矩阵
# 转移概率矩阵分解为orthogonal*hessenberg*orthogonal.transpose()
alpha = 0
orthogonal = []  # nodesnum行，alpha列
hessenberg = np.matrix(0)  # alpha行，alpha列


def initParam(graphFile):
    '''
    构建nodes、nodes_index、trans_matrix和第0代的sim_matrix.
    输入文件行格式要求：node\toutneighbor\toutneighbor\t...或 node\tinneighbor\tinneighbor\t...
    '''
    global nodes
    global nodes_index
    global sim_matrix
    global damp
    global nodesnum
    global orthogonal
    global hessenberg
    global alpha

    link_in = {}
    for line in open(graphFile, "r", 1024):
        arr = line.strip("\n").split()
        node = arr[0]
        nodeid = -1
        if node in nodes_index:
            nodeid = nodes_index[node]
        else:
            nodeid = len(nodes)
            nodes_index[node] = nodeid
            nodes.append(node)
        for ele in arr[1:]:
            outneighbor = ele
            outneighborid = -1
            if outneighbor in nodes_index:
                outneighborid = nodes_index[outneighbor]
            else:
                outneighborid = len(nodes)
                nodes_index[outneighbor] = outneighborid
                nodes.append(outneighbor)
            inneighbors = []
            if outneighborid in link_in:
                inneighbors = link_in[outneighborid]
            inneighbors.append(nodeid)
            link_in[outneighborid] = inneighbors

    nodesnum = len(nodes)
    print "node count is  %d:" % (nodesnum)
    trans_matrix = SparseMatrix()  # 真实的转移概率矩阵
    for node, inneighbors in link_in.items():
        num = len(inneighbors)
        prob = 1.0 / num
        for neighbor in inneighbors:
            trans_matrix.set(node, neighbor, prob)
    (orthogonal, hessenberg) = arnoldi.arnoldi_iteration(
        trans_matrix, nodesnum, nodesnum / 100) #人为设定H的规模是trans_matrix的1/100
    alpha = hessenberg.shape[0]

    sim_matrix = np.identity(alpha) * (1 - damp)


def iterate():
    '''
    迭代更新相似度矩阵
    '''
    global hessenberg
    global sim_matrix
    global damp
    global nodesnum

    damp = damp ** 2
    hessenberg = np.dot(hessenberg, hessenberg)
    sim_matrix = damp * np.dot(np.dot(hessenberg,
                                      sim_matrix), hessenberg.transpose()) + sim_matrix


def printResult(sim_node_file):
    '''
    打印输出相似度计算结果
    '''
    # global变量在子进程中可以读，但修改无效
    global sim_matrix
    global orthogonal
    global nodes
    global nodesnum

    # 打印node之间的相似度
    f_out_user = open(sim_node_file, "w")
    for i in range(nodesnum):
        f_out_user.write(nodes[i] + "\t")
        heap = TopkHeap(10)  # 只取相似度最高的前10个节点
        similarity = restoreSim(orthogonal, sim_matrix, i)
        for j in range(nodesnum):
            if i != j:
                sim = similarity[j]
                if sim == None:
                    sim = 0
                if sim > 1E-2:
                    heap.Push((j, sim))
        neighbour = heap.TopK()
        # 按相似度由大到小排序
        # TODO 上线时可以不对neighbour排序，排序是为了数据验证阶段调参数
        neighbour = sorted(
            neighbour, cmp=lambda x, y: cmp(x[1], y[1]), reverse=True)
        simcnt = 0
        for (u, sim) in neighbour:
            f_out_user.write(nodes[u] + ":" + str(sim) + "\t")
            simcnt += 1
            if simcnt >= 100:
                break
        f_out_user.write("\n")
    f_out_user.close()


def restoreSim(A, B, i):
    '''
    还原第i个节点与其他节点的相似度
    '''
    m = A.shape[0]
    n = A.shape[1]
    part = [0] * n
    for q in range(n):
        for p in range(n):
            if q + 1 >= p:  # B是上三角矩阵，左下角的部分全为0，可以不管
                part[q] += A[i, p] * B[p, q]
    rect = [0] * m
    for j in range(m):
        for q in range(n):
            rect[j] += A[j, q] * part[q]
    return rect


def simrank(graphFile, maxIteration):
    initParam(graphFile)
    for i in range(maxIteration):
        print "iteration %d:" % (i + 1)
        iterate()
        print hessenberg


if __name__ == '__main__':
    graphFile = "../data/linkgraph"
    sim_node_file = "../data/nodesim_krylov"
    maxIteration = 10
    simrank(graphFile, maxIteration)
    printResult(sim_node_file)


topk.py

import heapq
import random

class TopkHeap(object):
    def __init__(self, k):
        self.k = k
        self.data = []

    def Push(self, elem):
        if len(self.data) < self.k:
            heapq.heappush(self.data, elem)
        else:
            topk_small = self.data[0][1]
            if elem[1] > topk_small:
                heapq.heapreplace(self.data, elem)

    def TopK(self):
        return [x for x in reversed([heapq.heappop(self.data) for x in xrange(len(self.data))])]

if __name__ == "__main__":
    K=10
    list_rand = random.sample(xrange(1000000), 100)
    th = TopkHeap(K)
    for i in list_rand:
        th.Push(('a',i))
    print th.TopK()
    print sorted(list_rand, reverse=True)[0:K]


最终得到的节点相似度与平方缓存法有局部的不一致，说明S在投影到低维空间后产生了精度损失。restoreSim()这个函数只负责根据H和V还原S的第i行，即第i个节点跟其他所有节点的相似度。如果要根据\eqref{decompose}还原整个S，那又是两个大规模（主要是V的规模大）矩阵的乘法，会很耗时，所以实践中restoreSim()函数应该被多进程并发调用。

## 分布式SimRank

通过上文我们也看到，即使我们通过平方缓存法优化了SimRank的迭代次数，通过对Q进行降维避开了大矩阵相乘带来的时空消耗的困扰，但是到最后还原S的时候还是避不开大矩阵相乘，所以最根本的解决办法还是使用Hadoop进行矩阵的并行运算。下面描述如何用[MapReduce实现矩阵的乘法](http://www.cnblogs.com/zhangchaoyang/articles/4646315.html)。

## SimRank++

simrankpp.py

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


  1 #!/usr/bin/env python
  2 # coding=utf-8
  3 
  4 from sym_matrix import SymMatrix
  5 import mathutil
  6 import math
  7 
  8 user_index = {}  # user编号，从0开始
  9 position_index = {}  # position编号，从0开始
 10 link_out = {}  # user到position的邻接矩阵
 11 link_in = {}  # position到user的邻接矩阵
 12 user_common_count = SymMatrix()  # user共同浏览过的职位数
 13 position_common_count = SymMatrix()  # position共同被浏览过的用户数
 14 user_sim_matrix = SymMatrix()  # user之间的相似度矩阵
 15 position_sim_matrix = SymMatrix()  # position之间的相似度矩阵
 16 user_damp = 0.8  # user阻尼系数
 17 position_damp = 0.8  # position阻尼系数
 18 evidence_size = 20
 19 evidence = []  # evidence常量数组
 20 user_weight_sum_variance = {}  # 1个user节点上所有边权值的和以及方差
 21 position_weight_sum_variance = {}  # 1个position节点上所有边权值的和以及方差
 22 
 23 
 24 def initEvidence():
 25     '''
 26     计算evidence常量数组
 27     '''
 28     global evidence_size
 29     global evidence
 30 
 31     for i in range(evidence_size):
 32         evidence.append(0)
 33     evidence[0] = 0.5
 34     for i in range(1, evidence_size):
 35         evidence[i] = evidence[i - 1] + 1.0 / math.pow(2, i + 1)
 36 
 37 
 38 def indexUserAndPosition(viewfile):
 39     '''
 40     读取user浏览position页面的记录文件，为user和position建立编号
 41     '''
 42     global user_index
 43     global position_index
 44     user_count = 0
 45     position_count = 0
 46     for line in open(viewfile, "r"):
 47         arr = line.strip("\n").split()
 48         user = arr[0]
 49         if user not in user_index:
 50             user_index[user] = user_count
 51             user_count += 1
 52         for ele in arr[1:]:
 53             brr = ele.split(":")
 54             if len(brr) == 2:
 55                 position = brr[0]
 56                 if position not in position_index:
 57                     position_index[position] = position_count
 58                     position_count += 1
 59 
 60 
 61 def readLink(viewfile):
 62     '''
 63     读取user浏览position页面的记录文件，建构出度和入度邻接矩阵
 64     '''
 65     global link_out
 66     global link_in
 67     global position_index
 68     global user_index
 69 
 70     for line in open(viewfile, "r"):
 71         arr = line.strip("\n").split()
 72         user = arr[0]
 73         userid = user_index[user]
 74         position_count_collection = {}
 75         for ele in arr[1:]:
 76             brr = ele.split(":")
 77             if len(brr) == 2:
 78                 position = brr[0]
 79                 positionid = position_index[position]
 80                 count = int(brr[1])
 81                 position_count_collection[positionid] = count
 82                 user_count_collection = {}
 83                 if positionid in link_in:
 84                     user_count_collection = link_in[positionid]
 85                 user_count_collection[userid] = count
 86                 link_in[positionid] = user_count_collection
 87         link_out[userid] = position_count_collection
 88 
 89 
 90 def initWeightSumAndVariance():
 91     '''
 92     计算user_weight_sum_variance和position_weight_sum_variance
 93     '''
 94     global link_out
 95     global link_in
 96 
 97     for k, v in link_out.items():
 98         ps = []
 99         for p, c in v.items():
100             ps.append(c)
101         tup = mathutil.getSumAndVariance(ps)
102         user_weight_sum_variance[k] = tup
103     for k, v in link_in.items():
104         us = []
105         for u, c in v.items():
106             us.append(c)
107         tup = mathutil.getSumAndVariance(us)
108         position_weight_sum_variance[k] = tup
109 
110 
111 def initSimMatrix():
112     '''
113     初始化相似度矩阵position_sim_matrix/user_sim_matrix
114     '''
115     global link_out
116     global link_in
117     global user_sim_matrix
118     global position_sim_matrix
119     global user_common_count
120     global position_common_count
121 
122     usernum = len(link_out)  # user节点的数量
123     positionnum = len(link_in)  # position节点的数量
124 
125     user_sim_matrix = SymMatrix()
126     position_sim_matrix = SymMatrix()
127     user_common_count = SymMatrix()
128     position_common_count = SymMatrix()
129 
130     # 计算user两两之间的相似度
131     # print "user_sim_matrix="
132     for k1, v1 in link_out.items():  # user到position的链接
133         ps1 = []
134         for p, c in v1.items():
135             ps1.append(p)
136         for k2, v2 in link_out.items():
137             if k1 < k2:
138                 ps2 = []
139                 for p, c in v2.items():
140                     ps2.append(p)
141                 common = mathutil.findCommonEle(
142                     sorted(ps1), sorted(ps2))  # 2个user链接到的position的交集
143                 commonLen = len(common)
144                 if commonLen > 0:
145                     user_common_count.set(k1, k2, commonLen)
146                     # 2个user链接到至少同一个position时，他们的初始相似度为user_damp
147                     user_sim_matrix.set(k1, k2, user_damp)
148             # print "(%d,%d)%f" % (k1, k2, user_sim_matrix.get(k1, k2)),
149         user_sim_matrix.set(k1, k1, 1)  # 自己与自己的相似度为1
150         # print
151 
152     # 计算position两两之间的相似度
153     # print "position_sim_matrix="
154     for k1, v1 in link_in.items():
155         usr1 = []
156         for u, c in v1.items():
157             usr1.append(u)
158         for k2, v2 in link_in.items():
159             if k1 < k2:
160                 usr2 = []
161                 for u, c in v2.items():
162                     usr2.append(u)
163                 common = mathutil.findCommonEle(sorted(usr1), sorted(usr2))
164                 commonLen = len(common)
165                 if commonLen > 0:
166                     position_common_count.set(k1, k2, commonLen)
167                     position_sim_matrix.set(k1, k2, position_damp)
168             # print "(%d,%d)%f" % (k1, k2, position_sim_matrix.get(k1, k2)),
169         position_sim_matrix.set(k1, k1, 1)
170         # print
171 
172 
173 def updateSim():
174     '''
175     迭代更新相似度
176     '''
177     global link_out
178     global link_in
179     global user_common_count
180     global user_sim_matrix
181     global position_common_count
182     global position_sim_matrix
183     global user_damp
184     global position_damp
185     global evidence_size
186     global evidence
187 
188     usernum = len(link_out)  # user节点的数量
189     positionnum = len(link_in)  # position节点的数量
190     # 计算user两两之间的相似度
191     # print "user_sim_matrix="
192     for k1, v1 in link_out.items():  # user到position的链接
193         ps1 = []  # 存储第1个user的out-neighbour
194         for p, c in v1.items():
195             ps1.append(p)
196         for k2, v2 in link_out.items():
197             commCount = user_common_count.get(k1, k2)  # 两user共同链接的position数
198             if commCount == None:
199                 commCount = 0
200             # 数据量大时可以采用这种修剪策略，认为没有相同in-neighbour(或out-neighbour)的节点之间相似度为0，迭代时不去更新它
201             if k1 < k2 and commCount > 0:
202                 # if k1 < k2:
203                 ps2 = []  # 存储第2个user的out-neighbour
204                 for p, c in v2.items():
205                     ps2.append(p)
206                 if commCount > evidence_size:
207                     commCount = evidence_size
208                 edv = evidence[commCount - 1]
209                 sum_part = 0.0
210                 for p1 in ps1:
211                     for p2 in ps2:
212                         sim_p1_p2 = position_sim_matrix.get(p1, p2)
213                         if sim_p1_p2 == None:
214                             continue
215                         spread_p1 = math.pow(
216                             math.e, -position_weight_sum_variance[p1][1])
217                         weight_u1_p1 = link_out[k1][p1]
218                         weight_u1_sum = user_weight_sum_variance[k1][0]
219                         normalized_weight_u1_p1 = 1.0 * \
220                             weight_u1_p1 / weight_u1_sum
221                         spread_p2 = math.pow(
222                             math.e, -position_weight_sum_variance[p2][1])
223                         weight_u2_p2 = link_out[k2][p2]
224                         weight_u2_sum = user_weight_sum_variance[k2][0]
225                         normalized_weight_u2_p2 = 1.0 * \
226                             weight_u2_p2 / weight_u2_sum
227                         sum_part += spread_p1 * normalized_weight_u1_p1 * \
228                             spread_p2 * normalized_weight_u2_p2 * sim_p1_p2
229                 new_sim = edv * user_damp * sum_part
230                 user_sim_matrix.set(k1, k2, new_sim)
231             # print "(%d,%d)%f" % (k1, k2, user_sim_matrix.get(k1, k2)),
232        # print
233 
234     # 计算position两两之间的相似度
235     # print "position_sim_matrix="
236     for k1, v1 in link_in.items():  # position到user的链接
237         us1 = []  # 存储第1个position的in-neighbour
238         for u, c in v1.items():
239             us1.append(u)
240         for k2, v2 in link_in.items():
241             commCount = position_common_count.get(
242                 k1, k2)  # 两position共同链接的user数
243             if commCount == None:
244                 commCount = 0
245             # 数据量大时可以采用这种修剪策略，认为没有相同in-neighbour(或out-neighbour)的节点之间相似度为0，迭代时不去更新它
246             if k1 < k2 and commCount > 0:
247                 # if k1 < k2:
248                 us2 = []  # 存储第2个position的in-neighbour
249                 for u, c in v2.items():
250                     us2.append(u)
251                 if commCount > evidence_size:
252                     commCount = evidence_size
253                 edv = evidence[commCount - 1]
254                 sum_part = 0.0
255                 for u1 in us1:
256                     for u2 in us2:
257                         sim_u1_u2 = user_sim_matrix.get(u1, u2)
258                         if sim_u1_u2 == None:
259                             continue
260                         spread_u1 = math.pow(
261                             math.e, -user_weight_sum_variance[u1][1])
262                         weight_p1_u1 = link_in[k1][u1]
263                         weight_p1_sum = position_weight_sum_variance[k1][0]
264                         normalized_weight_p1_u1 = 1.0 * \
265                             weight_p1_u1 / weight_p1_sum
266                         spread_u2 = math.pow(
267                             math.e, -user_weight_sum_variance[u2][1])
268                         weight_p2_u2 = link_in[k2][u2]
269                         weight_p2_sum = position_weight_sum_variance[k2][0]
270                         normalized_weight_p2_u2 = 1.0 * \
271                             weight_p2_u2 / weight_p2_sum
272                         sum_part += spread_u1 * normalized_weight_p1_u1 * \
273                             spread_u2 * normalized_weight_p2_u2 * sim_u1_u2
274                 new_sim = edv * position_damp * sum_part
275                 position_sim_matrix.set(k1, k2, new_sim)
276             # print "(%d,%d)%f" % (k1, k2, position_sim_matrix.get(k1, k2)),
277         # print
278 
279 
280 def simrank(linkfile, iteration):
281     initEvidence()
282     indexUserAndPosition(linkfile)
283     readLink(linkfile)
284     initWeightSumAndVariance()
285     print "iteration 0:"
286     initSimMatrix()
287     print
288     for i in range(iteration):
289         print "iteration %d:" % (i + 1)
290         updateSim()
291         print
292 
293 
294 def printResult(sim_user_file, sim_position_file):
295     '''
296     打印输出相似度计算结果
297     '''
298     global position_sim_matrix
299     global user_sim_matrix
300     global link_out
301     global link_in
302     usernum = len(link_out)  # user节点的数量
303     positionnum = len(link_in)  # position节点的数量
304 
305     # 打印user之间的相似度
306     f_out_user = open(sim_user_file, "w")
307     for i in range(usernum):
308         f_out_user.write(str(i) + "\t")
309         neighbour = []
310         for j in range(usernum):
311             if i != j:
312                 sim = user_sim_matrix.get(i, j)
313                 if sim == None:
314                     sim = 0
315                 if sim > 0:
316                     neighbour.append((j, sim))
317         # 按相似度由大到小排序
318         neighbour = sorted(
319             neighbour, cmp=lambda x, y: cmp(x[1], y[1]), reverse=True)
320         for (u, sim) in neighbour:
321             f_out_user.write(str(u) + ":" + str(sim) + "\t")
322         f_out_user.write("\n")
323     f_out_user.close()
324 
325     # 打印position之间的相似度
326     f_out_position = open(sim_position_file, "w")
327     for i in range(positionnum):
328         f_out_position.write(str(i) + "\t")
329         neighbour = []
330         for j in range(positionnum):
331             if i != j:
332                 sim = position_sim_matrix.get(i, j)
333                 if sim == None:
334                     sim = 0
335                 if sim > 0:
336                     neighbour.append((j, sim))
337         neighbour = sorted(
338             neighbour, cmp=lambda x, y: cmp(x[1], y[1]), reverse=True)
339         for (p, sim) in neighbour:
340             f_out_position.write(str(p) + ":" + str(sim) + "\t")
341         f_out_position.write("\n")
342     f_out_position.close()
343 
344 
345 if __name__ == '__main__':
346     linkfile = "../data/delivery"
347     sim_user_file = "../data/sim_user.txt"
348     sim_position_file = "../data/sim_position.txt"
349     simrank(linkfile, 5)
350     printResult(sim_user_file, sim_position_file)


View Code
```

mathutil.py

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1 #!/usr/bin/env python
 2 # coding=utf-8
 3 
 4 def findCommonEle(list1,list2):
 5     '''
 6     返回两个序列的公共元素。list1和list2中的元素必须按从小到大排好序
 7     '''
 8     rect=[]
 9     length1=len(list1)
10     length2=len(list2)
11     idx1=0
12     idx2=0
13     while idx1<length1 and idx2<length2:
14         ele1=list1[idx1]
15         ele2=list2[idx2]
16         if ele1==ele2:
17             rect.append(ele1)
18             idx1+=1
19             idx2+=1
20         elif ele1<ele2:
21             idx1+=1
22         else:
23             idx2+=1
24     return rect
25 
26 def getSumAndVariance(li):
27     '''
28     计算数列的和与方差
29     '''
30     assert len(li)>0
31     total=0.0
32     sumSquare=0.0
33     for ele in li:
34         total+=ele
35         sumSquare+=ele*ele
36     mean=total/len(li)
37     variance=sumSquare/len(li)-mean*mean
38     return (total,variance)


View Code
```






## 基于随机游走的SimRank







期望-相遇距离模型说明:在图 G 中,顶点 a 和 b 之间的相似度取决于 a 和 b 在图中随机游走直至相 遇所经过的路径的长度以及能相遇的次数,基于这一模型 SimRank 的计算如式(9)和式(10)所示。

\begin{equation}s(a,b)=\sum_{t(a,b)\to(x,x)}{P[t]C^{l(t)}}\label{rw_sim}\end{equation}

\begin{equation}s^{k+1}(a,b)=\sum_{t(a,b)\to(x,x);l(t)\le{k+1}}{P[t]C^{l(t)}}\label{rw_itr}\end{equation}

$t(a,b)\to(x,x)$表示以a、b为起点的两条随机游走路径，首次在x点相遇，两条路径的长度相同，均为$l(t)$。假设两条路径分别为 $t_1=(v_1,v_2,\cdots,v_m,x)$,$t_2=(w_1,w_2,\cdots,w_m,x)$。游 走 $t_1$的 概 率$P[t_1]=\prod_{i=1}^m{\frac{1}{\mid{O(v_i)}\mid}}$,游 走$t_2$的概率$P[t_2]=\prod_{i=1}^m{\frac{1}{\mid{O(w_i)}\mid}}$，a和b通过路径$t_1$和$t_2$在顶点 x 相遇的概率$P[t]=P[t_1]P[t_2]=\prod_{i=1}^m{\frac{1}{\mid{O(v_i)}\mid\mid{O(w_i)}\mid}}$。

通过随机游走路径方法来计算$s^{k+1}$ 时,需要进行如下步骤:
- 找到图中所有长度小于等于$k+1$的路径,并且记录路径的概率。
- *将终点和路径长度都相同的路径按照式\eqref{rw_sim}计算出起点的部分相似度。*
- *将第2步得到的部分相似度相加。 *















