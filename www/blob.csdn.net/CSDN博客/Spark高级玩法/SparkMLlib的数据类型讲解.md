# SparkMLlib的数据类型讲解 - Spark高级玩法 - CSDN博客
2017年12月19日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1543
**SparkMLlib的数据类型讲解**
Mllib支持单机上存储的本地向量和矩阵，也支持由一个或者多个RDD支持的分布式矩阵。本地向量和本地矩阵是简单的数据模型，用作公共接口。由Breeze提供基本的线性代数运算。。在监督学习中使用的训练示例在MLlib中被称为“labeled point”
**一 本地向量**
本地向量存储于单台机器，其拥有整类型的行，从0开始的索引，和double类型的值。Mllib支持两种类型的本地向量:密集向量(dense)和稀疏向量(sparse)。密集向量只有一个浮点数组组成，而一个稀疏向量必须有索引和一个浮点向量组成。例如，(2.1,3.2,4.3)代表一个密集向量。(3，[1.1,2.3],[5.6,4.3,4.4])代表一个稀疏向量。
本地向量的基类是Vector，并且他有两个实现:DenseVector和SparseVector。
为了避免scala.collection.immutable.Vector该scala包被导入，你要引入的包是org.apache.spark.mllib.linalg.Vector
importorg.apache.spark.mllib.linalg.{Vector,Vectors}
// Create a dense vector (1.0, 0.0, 3.0).
val dv:Vector=Vectors.dense(1.0,0.0,3.0)
// Create a sparse vector (1.0, 0.0, 3.0) by specifying its indices and values corresponding to nonzero entries.
val sv1:Vector=Vectors.sparse(3,Array(0,2),Array(1.0,3.0))
// Create a sparse vector (1.0, 0.0, 3.0) by specifying its nonzero entries.
val sv2:Vector=Vectors.sparse(3,Seq((0,1.0),(2,3.0)))
**二 标签的向量**
Labeled point是一个本地向量，密集向量或者稀疏向量，并且带有一个标签。标签的向量用于监督学习中。使用double存储一个标签，所以标签数据可以用于回归或者分类。对于二分类，一个标签应该要么是0要么是1.对于多分类，标签应该零开始的索引：0，1,2,3,4
带标签的向量类在Spark Mllib中，叫做labeledPoint。
importorg.apache.spark.mllib.linalg.Vectors
importorg.apache.spark.mllib.regression.LabeledPoint
// Create a labeled point with a positive label and a dense feature vector.
val pos =LabeledPoint(1.0,Vectors.dense(1.0,0.0,3.0))
// Create a labeled point with a negative label and a sparse feature vector.
val neg =LabeledPoint(0.0,Vectors.sparse(3,Array(0,2),Array(1.0,3.0)))
**稀疏数据**
在实际生产中训练数据是稀疏数据很常见。Mllib支持以libsvm格式存储的稀疏矩阵。这是一个txt格式文件，其中每一行代表一个打标签的稀疏特征向量，格式如下：
label index1:value1 index2:value2 ...
其中，索引是从1开始的，递增的顺序。加载之后，特征索引就转化为了从0开始的。
MLUtils.loadLibSVMFile可以用来加载一libsvm格式存储的数据。
importorg.apache.spark.mllib.regression.LabeledPoint
importorg.apache.spark.mllib.util.MLUtils
importorg.apache.spark.rdd.RDD
val examples:RDD[LabeledPoint]=MLUtils.loadLibSVMFile(sc,"data/mllib/sample_libsvm_data.txt")
**三 本地矩阵**
本地矩阵是存储与单台机器上的，有整类型的row，列索引，double类型的值。Mllib支持密集矩阵，其输入值按照列column-major顺序存储在单个double数组中。稀疏矩阵是其非零值按照column-major顺序以压缩稀疏列（CSC）格式存储。
如下面的密集矩阵
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUd4W8NmVrFrL5XAJscbr0NrA6ZOVMKOkTM7icDYibjib43ibTkyibH8n3vMbaZvUqUWXEzPLSeWkvppXg/?wx_fmt=png)
密集矩阵的存储方式下，表现为：[1.0, 3.0, 5.0, 2.0, 4.0, 6.0]，矩阵大小为(3, 2)
本地矩阵的基类是Matrix，有两个实现：DenseMatrix和SparseMatrix。用工厂方法来创建本地矩阵。请记住，Mllib中的本地矩阵按照column-major的顺序存储。
importorg.apache.spark.mllib.linalg.{Matrix,Matrices}
// Create a dense matrix ((1.0, 2.0), (3.0, 4.0), (5.0, 6.0))
val dm:Matrix=Matrices.dense(3,2,Array(1.0,3.0,5.0,2.0,4.0,6.0))
// Create a sparse matrix ((9.0, 0.0), (0.0, 8.0), (0.0, 6.0))
val sm:Matrix=Matrices.sparse(3,2,Array(0,1,3),Array(0,2,1),Array(9,6,8))
**四 分布式矩阵**
一个分布式矩阵有一个long型的行，列索引，double类型的值，以一个或者多个RDD的形式分布式存储。存储巨大和分布式的矩阵需要选择一个正确的存储格式。将一个分布式矩阵转换为一个不同的格式可能需要一个全局的shuffle，代价是非常高的。目前为止，总共有四种类型的分布式矩已经被实现了。
一个RowMatrix矩阵是一个面向行的矩阵，行索引是没有意义的，比如，一个特征向量。RDD的每个行都是一个本地行向量。这里面RowMatrix假设的是列数不多，所以一个本地向量可以本传到driver端，可以轻松的被一个节点处理和存储。
一个IndexedRowMatrix跟RowMatrix很相似，但是它有一个行索引，该索引可以用于识别行并且执行joins。
CoordinateMatrix是一个以coordinate list (COO)格式存储的分布式矩阵。(row, column, value)
BlockMatrix是一个分布式矩阵，格式是(Int,Int,Matrix)。
**注意**
**分****布式矩阵的底层RDD必须是确定性的，因为我们缓存矩阵大小。一般来说，使用非确定性的RDD可能会导致错误。**
**五 RowMatrix**
A RowMatrix是一个面向行的分布式矩阵，没有有意义的行索引，由行的RDD支持，每行都是局部向量。由于每一行由一个局部向量表示，所以列的数量受整数范围的限制，但实际上列数应该小得多。
一个RowMatrix可以从一个RDD[Vector]实例创建。然后我们可以计算它的列汇总统计和分解。 QR分解的形式为A = QR，其中Q是正交矩阵，R是上三角矩阵。对于奇异值分解（SVD）和主成分分析（PCA），后面会出文章介绍。
importorg.apache.spark.mllib.linalg.Vector
importorg.apache.spark.mllib.linalg.distributed.RowMatrix
val rows:RDD[Vector]=...// an RDD of local vectors
// Create a RowMatrix from an RDD[Vector].
val mat:RowMatrix=newRowMatrix(rows)
// Get its size.
val m = mat.numRows()
val n = mat.numCols()
// QR decomposition 
val qrResult = mat.tallSkinnyQR(true)
**六 IndexedRowMatrix**
一个IndexedRowMatrix类似于一个RowMatrix但有意义的行索引。它由索引行的RDD支持，因此每行都由索引（long-typed）和局部向量表示。
一个 IndexedRowMatrix 可以从创建RDD[IndexedRow]。例如，在 IndexedRow中存储格式是(Long, Vector)。一个IndexedRowMatrix可以被转换为RowMatrix通过删除其行索引。
importorg.apache.spark.mllib.linalg.distributed.{IndexedRow,IndexedRowMatrix,RowMatrix}
val rows:RDD[IndexedRow]=...// an RDD of indexed rows
// Create an IndexedRowMatrix from an RDD[IndexedRow].
val mat:IndexedRowMatrix=newIndexedRowMatrix(rows)
// Get its size.
val m = mat.numRows()
val n = mat.numCols()
// Drop its row indices.
val rowMat:RowMatrix= mat.toRowMatrix()
**七 CoordinateMatrix**
A CoordinateMatrix是由RDD支持的分布式矩阵。每个条目是一个元组(i: Long, j: Long, value: Double)，其中i行索引，j是列索引，并且 value是条目值。CoordinateMatrix只有当矩阵的两个维度都很大且矩阵非常稀疏时才应该使用.
CoordinateMatrix 可以从被创建RDD[MatrixEntry]，格式(Long, Long, Double)。通过调用toIndexedRowMatrix一个CoordinateMatrix矩阵可以转化为IndexedRowMatrix。CoordinateMatrix目前还不支持其他计算 。
importorg.apache.spark.mllib.linalg.distributed.{CoordinateMatrix,MatrixEntry}
val entries:RDD[MatrixEntry]=...// an RDD of matrix entries
// Create a CoordinateMatrix from an RDD[MatrixEntry].
val mat:CoordinateMatrix=newCoordinateMatrix(entries)
// Get its size.
val m = mat.numRows()
val n = mat.numCols()
// Convert it to an IndexRowMatrix whose rows are sparse vectors.
val indexedRowMatrix = mat.toIndexedRowMatrix()
**八 BlockMatrix**
BlockMatrix是由一个MatrixBlocks 类型RDD支持的分布式矩阵，MatrixBlock由一组((Int, Int), Matrix)组成，(Int, Int)是当前块的索引。Matrix索引指定的子矩阵，其大小是rowsPerBlock * colsPerBlock。BlockMatrix支持的方法有add，multiply。BlockMatrix还有一个方法是validate，用来检测该BlockMatrix是否是正确设置。
BlockMatrix可以通过IndexedRowMatrix或者CoordinateMatrix通过调用toBlockMatrix轻松生成。toBlockMatrix默认创建的是1024* 1024大小的block。通过指定参数来自定义block大小toBlockMatrix(rowsPerBlock, colsPerBlock)。
importorg.apache.spark.mllib.linalg.distributed.{BlockMatrix,CoordinateMatrix,MatrixEntry}
val entries:RDD[MatrixEntry]=...// an RDD of (i, j, v) matrix entries
// Create a CoordinateMatrix from an RDD[MatrixEntry].
val coordMat:CoordinateMatrix=newCoordinateMatrix(entries)
// Transform the CoordinateMatrix to a BlockMatrix
val matA:BlockMatrix= coordMat.toBlockMatrix().cache()
// Validate whether the BlockMatrix is set up properly. Throws an Exception when it is not valid.
// Nothing happens if it is valid.
matA.validate()
// Calculate A^T A.
val ata = matA.transpose.multiply(matA)
**推荐阅读：**
1，[CountVectorizer](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484313&idx=1&sn=939b59d6ae32963d4418390db988dc85&chksm=9f38e0b1a84f69a773fe140c21eaae240fd3f398d780d8afe79212c3f89701c66e85b3fa34d5&scene=21#wechat_redirect)
2，[基于DF的Tokenizer分词](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484300&idx=1&sn=d09e8213b4572b3310fcf5c0f260306a&chksm=9f38e0a4a84f69b281c16a454ab71d7b628737d432d42c309cb92f6e05810ce253a42cd8ce74&scene=21#wechat_redirect)
3，[基于DataFrame的StopWordsRemover处理](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484300&idx=2&sn=92e6130aadc992be9d6fd35348b037f6&chksm=9f38e0a4a84f69b2d56f92bf7bfcc1cf24df980741274c16c5f12adcea960ed0963a19b81748&scene=21#wechat_redirect)
4，[大数据集群安全系列之kafka使用SSL加密认证](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483774&idx=1&sn=55897a71ea16af9cc3674f9911572d90&chksm=9f38e256a84f6b40dce5b7a68e9d934a7ef839d952ff628f9a965e70b4b19222036b88f96321&scene=21#wechat_redirect)
**本文主要参考翻译整理自Spark官网，打原创标签纯属为了保证，翻译劳动成果，谢谢大家谅解。**
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

