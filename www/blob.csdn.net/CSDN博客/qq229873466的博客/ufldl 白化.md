# ufldl 白化 - qq229873466的博客 - CSDN博客

2017年02月18日 15:39:40[qq229873466](https://me.csdn.net/qq229873466)阅读数：224


假设X为矩阵，行数为特征数L，列数为样本数M。

**SVD：**![](https://img-blog.csdn.net/20170218103720897)

**对角化：![](https://img-blog.csdn.net/20170705221658340)**

**求SVD：**![](https://img-blog.csdn.net/20170218104956679)，![](https://img-blog.csdn.net/20170218105035961)转化为求对角化（![](https://img-blog.csdn.net/20170218110022688)）。

**求对角化：**![](https://img-blog.csdn.net/20170218105347544)求出所有特征值，对应的u排列成矩阵就是U。

**PCA降维：**![](https://img-blog.csdn.net/20170218144817099)，其中![](https://img-blog.csdn.net/20170218161420506)。

**一些解释：**![](https://img-blog.csdn.net/20170218154157725)对X进行旋转变换，![](https://img-blog.csdn.net/20170218154201381)特征选择，![](https://img-blog.csdn.net/20170218154208241)恢复原数据域。

**白化：**假设训练数据是图像，由于图像中相邻像素之间具有很强的相关性，所以用于训练时输入是冗余的。白化的目的就是降低输入的冗余性；更正式的说，我们希望通过白化过程使得学习算法的输入具有如下性质：(i)特征之间相关性较低；(ii)所有特征具有相同的方差。

**PCA白化操作：**![](https://img-blog.csdn.net/20170218155724217)，其中![](https://img-blog.csdn.net/20170218161113943)为D对角线元素取倒数。

**ZCA白化操作：**![](https://img-blog.csdn.net/20170218160650035)，其中![](https://img-blog.csdn.net/20170218160731192)。

**正则化：**![](https://img-blog.csdn.net/20170218160911304)，其中![](https://img-blog.csdn.net/20170218160930351)为D对角线元素加上![](https://img-blog.csdn.net/20170218161014161)，然后取倒数。主要用来防止对角线元素很小时的溢出问题。

**验证：**原数据的协方差矩阵![](https://img-blog.csdn.net/20170218112351183)；旋转变换后的协方差矩阵![](https://img-blog.csdn.net/20170218154346132)，是对角矩阵满足(i)；![](https://img-blog.csdn.net/20170218160224502)，方差都相同满足(ii)。



参考网站：[http://ufldl.stanford.edu/wiki/index.php/%E4%B8%BB%E6%88%90%E5%88%86%E5%88%86%E6%9E%90](http://ufldl.stanford.edu/wiki/index.php/%E4%B8%BB%E6%88%90%E5%88%86%E5%88%86%E6%9E%90)

