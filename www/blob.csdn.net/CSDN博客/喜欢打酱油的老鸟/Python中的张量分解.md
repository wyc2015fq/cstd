
# Python中的张量分解 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月17日 08:24:39[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：490


[https://www.toutiao.com/a6643769342720737795/](https://www.toutiao.com/a6643769342720737795/)
2019-01-07 22:58:12
本文是讲解如何在Python中实现CP张量分解，包括张量分解的简要介绍。主要目的是专注于Python中张量分解的实现。根据这个目标，我们将使用Python中提供的两个库（TensorLy和tensortools）实现张量分解，并使用Numpy（通过交替优化）实现张量分解的简单实现。此外，在重构误差和执行时间两方面对三种方法的结果进行比较。
张量分解
让我们简单地从标题中定义每个术语。
**张量**：张量是一个**多维数组**。也称为**d-way阵列**。因此，我们使用的几乎所有几何数据结构都是张量。直到= 2以上，这些张量具有特定的名称：
zero-way张量：标量
one-way张量：向量
two-way张量：矩阵
这是一个直观的表示
![Python中的张量分解](http://p9.pstatp.com/large/pgc-image/ca2943111d664c8888d86e3a73f624d9)
**分解**：分解是分解为构成要素的过程。在数学分析中，它意味着d-way张量的分解。在系统科学中，它包括根据子系统找到系统的最优化分。一般而言，分解的动机是需要获得更简单的组成体，这些组成提最能代表给定的系统（或数据）。
**张量分解**：数据可以组成为d-way张量。因此，这种数据的分解称为d-way（张量）分解。
矩阵分解（又称Two-way分解）
two-way分解的方法已经很好地建立，包括主成分分析（PCA），独立成分分析（ICA），非负矩阵分解（NMF）和稀疏成分分析（SCA）。这些技术已成为例如盲源分离（BSS），特征提取或分类的标准工具。
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/ec1eb90b1652488482d9a21afbcc87d6)
其中R是我们数据的新（简化）维度，通常称为**秩**。此运算只是每一列的外积总和和，其中列索引由指定，如下所示：
![Python中的张量分解](http://p3.pstatp.com/large/pgc-image/ed694597d79145b8ab697f409ec42ede)
这种分解称为*因子分析*。上述公式存在称为**旋转问题**。也就是说，我们可以在上面的公式中插入任何非奇异旋转矩阵，并且仍然以的相同近似值结束（假设列的振幅为1）。
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/4d854ec8cc654fd69ad5864e11a9b716)
因此，如果上述公式不受约束，则会导致和有无限多组合。线性代数中的标准矩阵分解方法，例如QR分解，特征值分解（EVD）和奇异值分解（SVD），只是上述公式的特殊情况，它们的唯一性是由于三角性和正交性等硬约束条件。
Three-way张量分解
三元分解仅仅是双元分解的扩展。然而，尽管在双元情况下必须对问题施加显式约束以产生唯一解，但张量格式的高维度好处是 - 包括获得紧凑表示的可能性，分解的唯一性，约束选择的灵活性，以及可以识别成分的的普遍性。
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/cd3dc3dcebcb4b8ea5998f1338079797)
作为这种分解的结果，我们将得到三个矩阵∈ℝ 维度为*（*×*）*，∈ℝ 维度为*（*×*）*，∈ℝ维度为*（*×*）*。此运算是简单地求出每列A,B,C的外积之和，其中通过指定为列的索引，描述如下：
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/327fbcb2f36e4f79ab1b57c40987f0a7)
如何找到A，B和C？
我们将重点介绍使用两个Python库实现三元张量分解：TensorLy和tensortools。此外，我们还将使用Numpy和交替优化算法实现一个非常简单的三元张量分解器。
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/ab75871d648249f4b5009ff160833373)
让我们首先导入我们需要的库和函数：
```python
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.decomposition import FactorAnalysis, PCA
import tensortools as tt
from tensortools.operations import unfold as tt_unfold, khatri_rao
import tensorly as tl
from tensorly import unfold as tl_unfold
from tensorly.decomposition import parafac
# import some useful functions (they are available in utils.py)
from utils import *
```
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/4b94bcf871d545f19017c912bb5028c9)
加载张量
让我们加载数据 - 我已经创建了数据，如上图所示。
```python
time_factor = np.load("data/time_factor.npy")
neuron_factor = np.load("data/neuron_factor.npy")
trial_factor = np.load("data/trial_factor.npy")
latent = np.load("data/latent.npy")
observed = np.load("data/observed.npy")
```
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/8d552f4dc23c45d0959a197339b255d1)
下面我们可以看到每个潜在因素（神经元）的振幅随时间和试验的变化：
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/5909475230ab4ed5a89a88791172e51e)
安装（使用库）
关于库，使用非常相似：只需调用分解函数并传递张量和秩（因子数）。在**张量中**，调用函数，该函数称为parafac通过交替最小二乘（ALS）实现Canocical Polyadic（CP）分解。这个名称由来已久，但它也被称为正则分解（CANDECOMP）以及并行因子分析（PARAFAC）。在**tensortools中，**它被称为cp_als实现相同的功能。
下面是在两个库中实现CP张量分解的最少代码：
指定张量和秩（因子数）
使用该函数分解张量
我们可以重建估值和重构函数至脚本。
```python
# Specify the tensor, and the rank (np. of factors)
X, rank = observed, 3
# Perform CP decompositon using TensorLy
factors_tl = parafac(X, rank=rank)
# Perform CP decomposition using tensortools
U = tt.cp_als(X, rank=rank, verbose=False)
factors_tt = U.factors.factors
# Reconstruct M, with the result of each library
M_tl = reconstruct(factors_tl)
M_tt = reconstruct(factors_tt)
# plot the decomposed factors
plot_factors(factors_tl)
plot_factors(factors_tt)
```
![Python中的张量分解](http://p3.pstatp.com/large/pgc-image/39d7f833f1e74ed0b7edf1da15225e6e)
在我们的例子中，产生的因素图如下所示：
![Python中的张量分解](http://p3.pstatp.com/large/pgc-image/529e2178e7bd4196bba1ec3d18bbc490)
使用Numpy进行张量分解
优化问题
最终，我们希望最小化X（基本事实）和（估值模型，即的近似值）之间的差异。因此，我们可以将损失函数表示为 和M 之间的平方误差：
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/47602aab9fe64a928945c140463b923f)
计算使用三个矩阵，，C。找到它们的一种方法是优化一个，同时修复其他两个。一旦我们优化了一个，我们将其用作固定矩阵，同时优化另一个。我们，，之间交替优化直到收敛或停止。因此，我们要写出每个矩阵的损失函数，如下所示：
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/ab18c49989344894ac85a9f1ba49954c)
其中（0）表示张量X在矩阵中以模式-0展开，依此类推。（⊙）T表示在矩阵中B和C的 Khatri-Rao乘积。一般来说，这是一个非凸问题; 然而，当我们每次优化一个矩阵时，这是一个凸问题。
```python
def decompose_three_way(tensor, rank, max_iter=501, verbose=False):
 # a = np.random.random((rank, tensor.shape[0]))
 b = np.random.random((rank, tensor.shape[1]))
 c = np.random.random((rank, tensor.shape[2]))
 for epoch in range(max_iter):
 # optimize a
 input_a = khatri_rao([b.T, c.T])
 target_a = tl.unfold(tensor, mode=0).T
 a = np.linalg.solve(input_a.T.dot(input_a), input_a.T.dot(target_a))
 # optimize b
 input_b = khatri_rao([a.T, c.T])
 target_b = tl.unfold(tensor, mode=1).T
 b = np.linalg.solve(input_b.T.dot(input_b), input_b.T.dot(target_b))
 # optimize c
 input_c = khatri_rao([a.T, b.T])
 target_c = tl.unfold(tensor, mode=2).T
 c = np.linalg.solve(input_c.T.dot(input_c), input_c.T.dot(target_c))
 if verbose and epoch % int(max_iter * .2) == 0:
 res_a = np.square(input_a.dot(a) - target_a)
 res_b = np.square(input_b.dot(b) - target_b)
 res_c = np.square(input_c.dot(c) - target_c)
 print("Epoch:", epoch, "| Loss (C):", res_a.mean(), "| Loss (B):", res_b.mean(), "| Loss (C):", res_c.mean())
 return a.T, b.T, c.T
```
![Python中的张量分解](http://p3.pstatp.com/large/pgc-image/03c37132368b46bd93a23c0695caab03)
现在，和库类似，我们可以分解张量，给定秩（因子数）：
```python
factors_np = decompose_three_way(X, rank)
```
样本结果和比较
以下是我们实施的Numpy优化算法的张量分解的样本结果，该算法成功地估计了神经元，时间和试验因素。此外，出于好奇，我还在重构错误和执行时间方面比较了这三种不同实现的性能。
![Python中的张量分解](http://p1.pstatp.com/large/pgc-image/4f5bf17a0107482e815f85678a3a967c)
![Python中的张量分解](http://p3.pstatp.com/large/pgc-image/ed737b28b3774f94bf492008a603854e)

