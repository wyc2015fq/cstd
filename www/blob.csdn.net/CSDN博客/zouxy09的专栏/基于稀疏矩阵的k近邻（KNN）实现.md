
# 基于稀疏矩阵的k近邻（KNN）实现 - zouxy09的专栏 - CSDN博客


2014年12月31日 17:54:09[zouxy09](https://me.csdn.net/zouxy09)阅读数：23266个人分类：[机器学习																](https://blog.csdn.net/zouxy09/article/category/1333962)



**基于稀疏矩阵的k近邻（KNN）实现**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
元旦了！要跨入2015了！呵呵，忙了挺久，没有更新博客了，博客也悄悄地蒙上了一层灰。在跨年之际，闲来无事，也总结下之前的一些散乱的东西，记录在博客中，恢复点生气。良久未笔，辞藻异常生疏。嗯，祝大家2015再谱人生华丽篇章！
**一、概述**
这里我们先来看看当我们的数据是稀疏时，如何用稀疏矩阵的特性为KNN算法加速。KNN算法在[之前的博文](http://blog.csdn.net/zouxy09/article/details/16955347)中有提到，当时写的测试程序是针对稠密矩阵数据的。但实际上我们也会遇到不少的稀疏数据，而且有很多是有意而为之的，因为稀疏数据具有稠密数据无法媲美的存储和计算特性，这对工程应用中的内存需求和实时需求是很重要的。所以这里我们也来关注下稀疏矩阵的存储和其在knn算法中的应用举例。
![](https://img-blog.csdn.net/20141231175134703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
大家都知道，所谓稀疏矩阵，就是很多元素为零的矩阵，或者说矩阵中非零元素的个数远远小于矩阵元素的总数，如上图。如果我们只存储这些少量的非零元素，就可以大大的降低这个矩阵的存储空间。例如一个1000x1000的矩阵，里面只有100个非零的元素。如果全部存储这个矩阵，那么需要1000x1000x4Byte=4MB的空间（假设矩阵元素是float型，占4个字节）。但如果只存储那100个非零元素，那只需要100x4Byte=0.4KB的空间。这强大的差别对对寸土寸金的内存来说是非常讨人喜欢的。哎哟，你雪亮的眼睛可以看出问题了，只占0.4KB的空间就可以描述这个稀疏矩阵了？矩阵每个元素不是具有位置意义的么？那不是还得存储每个元素在哪一行和哪一列么？嗯，没错，我们还需要提供辅助数组来描述非零元素在原矩阵中的位置，一个萝卜一个坑，得标记好。
对于矩阵操作而言，不同的稀疏数据组织方式会有不同的特点，所以这就出现了多种不同的稀疏矩阵的存储格式，但也万变不离其宗，即存储矩阵所有的非零元素到一个线性数组中，并提供辅助数组来描述非零元素在原矩阵中的位置。例如著名的scipy库的稀疏矩阵模块就提供了以下几种存储方式：
bsr_matrix:   Block Sparse Row matrix
coo_matrix:   A sparse matrix in COOrdinate format.
csc_matrix:   Compressed Sparse Column matrix
csr_matrix:   Compressed Sparse Row matrix
dia_matrix:   Sparse matrix with DIAgonal storage
dok_matrix:   Dictionary Of Keys based sparse matrix.
lil_matrix:    Row-basedlinked list sparse matrix
关于其中的差别，我们可以看看后面的参考资料和scipy的[官方文档](http://docs.scipy.org/doc/scipy-0.14.0/reference/sparse.html)。这里只介绍下主流的CSR格式。
**二、CSR稀疏矩阵存储方式**
CSR全名是 CompressedSparse Row Format。它的存储方式通过下面一张图可以清晰的看出来：
![](https://img-blog.csdn.net/20141231175211371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到，一个稠密矩阵被存储到三个数组里面。其中把矩阵的所有非零元素值按照行的顺序保存到values数组中。就是先第一行的非零元素站出来排好队，然后第二行的跟上……这个矩阵有9个非零元素是不是，所以values数组就是9个元素。那怎么记录这些非零元素在原矩阵中的位置呢？矩阵的位置不就是哪一行哪一列吗？那我们再分别添加一个行数组和一个列数组来辅助。这是很自然的，水到渠成的想法。首先，需要记录values数组中每个元素（也就是原矩阵的非零元素）所在的行号是多少。这样我们就新建一个数组叫row indices，它的值是[0 0 1 1 2 2 2 3 3]。还需要新建一个数组column indices来记录values数组中每个元素的列号是多少，它的值是[0 1 1 2 0 2 3 1 3]。很明显，这三个数组一一对应起来就可以描述原数组了。例如，values的第三个非零元素是2，它在原矩阵的行是row indices的第三个元素，也就是1，所在的列是column indices中的第三个元素，也是1，所以非零元素在原矩阵的位置是(1,1)。
到这里我们的目标就达到了，但需要的存储空间是“非零元素个数x3”。那还能再小点吗？Smaller than smaller!答案是可以的，CSR就是其中一种方法了。它的思想也很简单，矩阵中很多非零元素都属于同一行对不对？例如上面例子中，values数组中的第1和第2个元素都属于矩阵的第1行，第5,6,7个元素都属于矩阵的第3行。我们在row indices中也可以看出来，它的值是[0 0 1 1 2 2 2 3 3]，可以看到很多元素是连续相同而且基数递增的，那我们就可以把这些连续相同的数据合并，只标记每行的开始和结尾即可。也可以说只记录偏移量。这个数组名字就叫row offset，它的大小就是原矩阵的行数+1，它用自己的第i和第i+1位置上的元素值来标记values数组中的offset[i]到offset[i+1]-1位置上的数都是原矩阵第i行的元素。所以上面例子中row offset是[0 2 4 7 9]。不知道说清楚了没有，不清楚自己再慢慢看下图。
**三、基于csr的knn实现**
基于稀疏矩阵的运算是比较快的，例如矩阵乘法，因为矩阵中很多元素为0，任何数与0相乘都等于0。目前也有很多库实现了稀疏矩阵的运算。在这里我们不涉及细节的实现过程，我们只运用现有的优化过的库来搭搭积木。这里我们使用scipy包的csr_matrix模块，关于它的说明，请参考[官方文档](http://docs.scipy.org/doc/scipy-0.14.0/reference/generated/scipy.sparse.csr_matrix.html#scipy.sparse.csr_matrix)。我们是在python环境下做实验，建议使用Anaconda这个python发行版本，最新的版本提供了多达195个流行的python包，包含了我们常用的numpy、scipy等等科学计算的包。有了它，妈妈再也不用担心我焦头烂额地安装一个又一个依赖包了。Anaconda在手，轻松我有！下载地址如下：[http://www.continuum.io/downloads](http://www.continuum.io/downloads)
这里我们拿knn来做实验。因为它涉及到稀疏矩阵的乘法等。对KNN，一般有两个矩阵，一个是存储N个训练样本的矩阵A，假设矩阵每一行代表一个训练样本。还有一个是存储M个测试样本的矩阵B。KNN要求我们计算：对每个B中的样本，计算它与矩阵A中N个样本的欧式距离（这里采用），然后找出距离最小的K个。我们知道欧式距离可以展开：|**a**-**b**|2=**a**2-2**ab**+**b**2.所以在程序中，我们可以先分别计算矩阵A和B每一行的平方和，也就是**a**2和**b**2。因为矩阵A中所有样本和B中所有样本的内积，所以可以统一到矩阵A和B相乘。这样就可以把所有的计算都统一到矩阵运算中去了，也就可以借助矢量化运算的神力了。故而得到了以下的程序：
**knn_sparse_csr.py**

```python
#****************************************************
#* 
#* Description: KNN with sparse data
#* Author: Zou Xiaoyi
#* Date:   2014-12-31
#* HomePage : http://blog.csdn.net/zouxy09
#* Email  : zouxy09@qq.com
#* 
#****************************************************
import numpy as np
from scipy.sparse import csr_matrix
def kNN_Sparse(local_data_csr, query_data_csr, top_k):
	# calculate the square sum of each vector
	local_data_sq = local_data_csr.multiply(local_data_csr).sum(1)
	query_data_sq = query_data_csr.multiply(query_data_csr).sum(1)
	
	# calculate the dot
	distance = query_data_csr.dot(local_data_csr.transpose()).todense()
	
	# calculate the distance
	num_query, num_local = distance.shape
	distance = np.tile(query_data_sq, (1, num_local)) + np.tile(local_data_sq.T, (num_query, 1)) - 2 * distance
	
	# get the top k
	topK_idx = np.argsort(distance)[:, 0:top_k]
	topK_similarity = np.zeros((num_query, top_k), np.float32)
	for i in xrange(num_query):
		topK_similarity[i] = distance[i, topK_idx[i]]
	
	return topK_similarity, topK_idx

def run_knn():
	top_k = np.array(2, dtype=np.int32)
	local_data_offset = np.array([0, 1, 2, 4, 6], dtype=np.int64)
	local_data_index = np.array([0, 1, 0, 1, 0, 2], dtype=np.int32)
	local_data_value = np.array([1, 2, 3, 4, 8, 9], dtype=np.float32)
	local_data_csr = csr_matrix((local_data_value, local_data_index, local_data_offset), dtype = np.float32)
	print local_data_csr.todense()
	
	query_offset = np.array([0, 1, 4], dtype=np.int64)
	query_index = np.array([0, 0, 1, 2], dtype=np.int32)
	query_value = np.array([1.1, 3.1, 4, 0.1], dtype=np.float32)
	query_csr = csr_matrix((query_value, query_index, query_offset), dtype = np.float32)
	print query_csr.todense()
	topK_similarity, topK_idx = kNN_Sparse(local_data_csr, query_csr, top_k)
	
	for i in range(query_offset.shape[0]-1):
		print "for %d image, top %d is " % (i, top_k) , topK_idx[i]
		print "corresponding similarity: ", topK_similarity[i]

if __name__ == '__main__':
	run_knn()
```

程序输出如下：

```python
[[ 1.  0.  0.]
 [ 0.  2.  0.]
 [ 3.  4.  0.]
 [ 8.  0.  9.]]
[[ 1.10000002  0.          0.        ]
 [ 3.0999999   4.          0.1       ]]
for 0 image, top 2 is  [[0 1]]
corresponding similarity:  [ 0.00999999  5.21000004]
for 1 image, top 2 is  [[2 1]]
corresponding similarity:  [  0.02000046  13.61999893]
```

这个代码在这个toy数据下运行看不到什么加速的。但如果处理庞大的高维数据，而且稀疏度较高，那稀疏的计算加速比还是很惊人的。PS：上述程序在大矩阵中运行过。
**四、参考资料：**
[1][稀疏矩阵的存储格式（Sparse Matrix Storage Formats）](http://blog.csdn.net/anshan1984/article/details/8580952)
[2][http://www.bu.edu/pasi/files/2011/01/NathanBell1-10-1000.pdf](http://www.bu.edu/pasi/files/2011/01/NathanBell1-10-1000.pdf)


