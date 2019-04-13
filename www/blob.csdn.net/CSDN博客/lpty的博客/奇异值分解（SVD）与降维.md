
# 奇异值分解（SVD）与降维 - lpty的博客 - CSDN博客

2018年04月17日 17:55:38[lpty](https://me.csdn.net/sinat_33741547)阅读数：1207标签：[降维																](https://so.csdn.net/so/search/s.do?q=降维&t=blog)[SVD																](https://so.csdn.net/so/search/s.do?q=SVD&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=降维&t=blog)个人分类：[机器学习																](https://blog.csdn.net/sinat_33741547/article/category/6482345)



## 一、特征分解
> 许多数学对象可以通过将它们分解成多个组成部分或者找到它们地 一些属性来更好的理解。这些属性是通用的，而不是我们选择表示它们的方式而产生的。如：我们可以用十进制或二进制等方式表示12，但12=2*2*3永远是对的。

### 1、特征分解
特征分解，即将矩阵分解成一组特征值和特征向量。
### 2、特征值和特征向量
假如存在A是一个n*n的矩阵，x是一个n维向量，λ为标量，满足：Ax=λx，那么，λ是矩阵A的一个特征值，而x是矩阵A的特征值λ对应的特征向量。
> 即，方阵A的特征向量是指与A相乘后相当于对该向量进行缩放的非零向量v，缩放幅度λ为特征值。
如果求出了矩阵A的n个特征值λ1≤λ2≤...≤λn$\lambda_1 \leq \lambda_2 \leq ... \leq \lambda_n$以及这n个特征值对应的特征向量{q1,q2,...qn}$\{q_1,q_2,...q_n\}$，那么矩阵A可以用以下的特征分解表示：
A=QΣQ$A=Q\Sigma Q^{-1}$
其中Q是A的特征向量组成的n维正交矩阵，Σ$\Sigma$是n个特征值组成的对角矩阵，也可以用以下方式表示：
A=QΣQT$A=Q\Sigma Q^T$
要进行特征分解，矩阵必须满足为方阵，如果为非方阵可以使用奇异值分解。
## 二、奇异值分解
奇异值分解（SVD）与特征分解类似，是将矩阵分解为奇异向量与奇异值。通过上面可以知道，只有方阵才能进行特征分解，但是每一个矩阵都有一个奇异值分解。
### 1、定义
对于奇异值分解，我们将其定义为以下形式，与特征分解类似：
A=UΣVT$A=U\Sigma V^T$
其中A是一个m*n的矩阵，U是一个m*m的矩阵，Σ$\Sigma$是一个m*n的矩阵，V是一个n*n的矩阵，注意D不一定是方阵，如下所示：
![这里写图片描述](https://img-blog.csdn.net/20180417165620213?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180417165620213?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
对角矩阵Σ$\Sigma$对角线上的元素称为矩阵A的奇异值，矩阵U的列向量称为左奇异向量，矩阵V的列向量称为右奇异向量。
[
](https://img-blog.csdn.net/20180417165620213?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2、推导
[
](https://img-blog.csdn.net/20180417165620213?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)对于矩阵A的奇异值分解，可以用与A相关的特征分解推导出来，A的左奇异向量是AAT$AA^T$特征向量，A的右奇异向量是ATA$A^TA$的特征向量，A的非零奇异值是AAT$AA^T$或ATA$A^TA$特征值的平方根，推导如下：
![这里写图片描述](https://img-blog.csdn.net/20180417171851423?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 三、降维
上述关于的特征分解或奇异值分解的过程，实际上并没有体现出降维的过程，如果给定一个大小为20000*10000的矩阵，我们会发现通过奇异值分解得到三个矩阵U，Σ$\Sigma$，V大小依次是：20000*20000、20000*10000,、10000*10000，此时，分解出来的三个矩阵合起来更加的大，增大了计算机的存储空间，那么，奇异值分解是怎么体现降维的呢？
### 1、截断奇异值分解
事实上，奇异值分解体现在其低秩逼近问题上，在这里也称为截断奇异值分解（Truncated SVD）。
对于奇异值，按照惯例，我们通常降序排列Σ$\Sigma$的元素，而奇异值拥有一种特征，它减少的速度特别快，通常前10%甚至1%的奇异值的和就占了全部的奇异值之和的99%以上，也就是说我们可以用前面出现的k个奇异值和对应的左右奇异向量来近似的描述矩阵，即：
A=UΣVT≈UΣVT$A_{m \times n} = U_{m \times m}\Sigma_{m \times n} V^T_{n \times n} \approx U_{m \times k}\Sigma_{k \times k} V^T_{k \times n}$
在这里k要比n小得多，如下所示：
![这里写图片描述](https://img-blog.csdn.net/20180417173026721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180417173026721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
假设取k=100，大小为20000*10000的矩阵可以分解为大小为20000*100、100*100、10000*100的三个矩阵，这样便大大的减少了计算的存储开销。由于这个性质，SVD才可以用在降维，数据压缩等等。
[
](https://img-blog.csdn.net/20180417173026721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2、实战
[
](https://img-blog.csdn.net/20180417173026721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`from sklearn.decomposition import TruncatedSVD
from sklearn.random_projection import sparse_random_matrix
X = sparse_random_matrix(100, 100, density=0.01, random_state=42)
svd = TruncatedSVD(n_components=min(X.shape)-1, n_iter=7, random_state=42)
svd.fit(X)
total_variance, n_components = 0.0, 0
for variance in svd.explained_variance_ratio_:
    total_variance += variance
    n_components += 1
    if total_variance > 0.9: break
svd = TruncatedSVD(n_components=n_components, n_iter=7, random_state=42)
x = svd.fit_transform(X)`[
](https://img-blog.csdn.net/20180417173026721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)上述代码定义了一个100*100的稀疏矩阵X，使用TSVD进行拟合。拟合完成后，通过variance占比进行维数定义，当占比超过90%时选取该点，这也就是上面提到的奇异值降维性质。
之后再使用当前选取的维数进行重新拟合，转化，得到100*42维的矩阵x，完成降维过程。
## 三、参考
1、《深度学习》  Ian Goodfellow等
2、[https://www.cnblogs.com/pinard/p/6251584.html](https://www.cnblogs.com/pinard/p/6251584.html)

[
](https://img-blog.csdn.net/20180417173026721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
