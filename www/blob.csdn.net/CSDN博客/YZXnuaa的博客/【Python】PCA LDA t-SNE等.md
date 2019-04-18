# 【Python】PCA LDA t-SNE等 - YZXnuaa的博客 - CSDN博客
2018年03月26日 10:11:08[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：161
## 本文转自：http://blog.csdn.net/u012162613/article/details/45920827
## 1.流形学习的概念
流形学习方法(Manifold Learning)，简称流形学习，自2000年在著名的科学杂志《Science》被首次提出以来，已成为信息科学领域的研究热点。在理论和应用上，流形学习方法都具有重要的研究意义。
假设数据是均匀采样于一个高维欧氏空间中的低维流形，流形学习就是从高维采样数据中恢复低维流形结构，即找到高维空间中的低维流形，并求出相应的嵌入映射，以实现维数约简或者数据可视化。它是从观测到的现象中去寻找事物的本质，找到产生数据的内在规律。
> 
以上选自[百度百科](http://baike.baidu.com/link?url=vQmr30kzWc3gXfZM-6ANTtPdWJ1JyUsJR0pzoOWfjG79QK4zVZ_PvFN8BRfgHeGkqFPR-HZGsguaYuZrSTEcwK)
简单地理解，流形学习方法可以用来对高维数据降维，如果将维度降到2维或3维，我们就能将原始数据可视化，从而对数据的分布有直观的了解，发现一些可能存在的规律。
## 2.流形学习的分类
可以将流形学习方法分为线性的和非线性的两种，线性的流形学习方法如我们熟知的主成份分析（PCA），非线性的流形学习方法如等距映射（Isomap）、拉普拉斯特征映射（Laplacian eigenmaps，LE）、局部线性嵌入(Locally-linear embedding，LLE)。
当然，流形学习方法不止这些，因学识尚浅，在此我就不展开了，对于它们的原理，也不是一篇文章就能说明白的。对各种流形学习方法的介绍，网上有一篇不错的读物（原作已找不到）： [流形学习 (Manifold Learning)](http://blog.csdn.net/zhulingchen/article/details/2123129)
## 3.高维数据降维与可视化
对于数据降维，有一张图片总结得很好（同样，我不知道原始出处）：
![这里写图片描述](https://img-blog.csdn.net/20150522194801297)
图中基本上包括了大多数流形学习方法，不过这里面没有t-SNE,相比于其他算法，t-SNE算是比较新的一种方法，也是效果比较好的一种方法。t-SNE是深度学习大牛Hinton和lvdmaaten（他的弟子？）在2008年提出的，lvdmaaten对t-SNE有个主页介绍：[tsne](http://lvdmaaten.github.io/tsne/),包括论文以及各种编程语言的实现。
接下来是一个小实验，对MNIST数据集降维和可视化，采用了十多种算法，算法在sklearn里都已集成，画图工具采用matplotlib。大部分实验内容都是参考sklearn这里的[example](http://scikit-learn.org/stable/auto_examples/manifold/plot_lle_digits.html)，稍微做了些修改。
Matlab用户可以使用lvdmaaten提供的工具箱: [drtoolbox](http://lvdmaaten.github.io/drtoolbox/)
### - 加载数据
MNIST数据从sklearn集成的datasets模块获取，代码如下，为了后面观察起来更明显，我这里只选取`n_class=5`，也就是0～4这5种digits。每张图片的大小是8*8，展开后就是64维。
<code
 class="hljs matlab has-numbering" style="display: block; padding: 0px; 
color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', 
monospace;font-size:undefined; white-space: pre; border-radius: 0px; 
word-wrap: normal; background: transparent;">digits = <span 
class="hljs-transposed_variable" style="margin: 0px; padding: 0px; 
box-sizing: 
border-box;">datasets.</span>load_digits(n_class=<span 
class="hljs-number" style="margin: 0px; padding: 0px; color: rgb(0, 102,
 102); box-sizing: border-box;">5</span>)
X = <span class="hljs-transposed_variable" style="margin: 0px; 
padding: 0px; box-sizing: border-box;">digits.</span>data
y = <span class="hljs-transposed_variable" style="margin: 0px; 
padding: 0px; box-sizing: border-box;">digits.</span>target
print <span class="hljs-transposed_variable" style="margin: 0px; 
padding: 0px; box-sizing: border-box;">X.</span>shape
n_img_per_row = <span class="hljs-number" style="margin: 0px; 
padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">20</span>
img = <span class="hljs-transposed_variable" style="margin: 0px; 
padding: 0px; box-sizing: border-box;">np.</span><span 
class="hljs-built_in" style="margin: 0px; padding: 0px; color: rgb(102, 
0, 102); box-sizing: border-box;">zeros</span>((<span 
class="hljs-number" style="margin: 0px; padding: 0px; color: rgb(0, 102,
 102); box-sizing: border-box;">10</span> * n_img_per_row, 
<span class="hljs-number" style="margin: 0px; padding: 0px; color: 
rgb(0, 102, 102); box-sizing: border-box;">10</span> * 
n_img_per_row))
<span class="hljs-keyword" style="margin: 0px; padding: 0px; color: 
rgb(0, 0, 136); box-sizing: border-box;">for</span> <span 
class="hljs-built_in" style="margin: 0px; padding: 0px; color: rgb(102, 
0, 102); box-sizing: border-box;">i</span> in 
range(n_img_per_row):
    ix = <span class="hljs-number" style="margin: 0px; padding: 0px; 
color: rgb(0, 102, 102); box-sizing: border-box;">10</span> * 
<span class="hljs-built_in" style="margin: 0px; padding: 0px; color: 
rgb(102, 0, 102); box-sizing: border-box;">i</span> + <span 
class="hljs-number" style="margin: 0px; padding: 0px; color: rgb(0, 102,
 102); box-sizing: border-box;">1</span>
    <span class="hljs-keyword" style="margin: 0px; padding: 0px; 
color: rgb(0, 0, 136); box-sizing: border-box;">for</span> 
<span class="hljs-built_in" style="margin: 0px; padding: 0px; color: 
rgb(102, 0, 102); box-sizing: border-box;">j</span> in 
range(n_img_per_row):
        iy = <span class="hljs-number" style="margin: 0px; padding: 
0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">10</span> * <span class="hljs-built_in" 
style="margin: 0px; padding: 0px; color: rgb(102, 0, 102); box-sizing: 
border-box;">j</span> + <span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">1</span>
        img<span class="hljs-matrix" style="margin: 0px; padding: 
0px; box-sizing: border-box;">[ix:ix + <span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">8</span>, iy:iy + <span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">8</span>]</span> = X<span 
class="hljs-matrix" style="margin: 0px; padding: 0px; box-sizing: 
border-box;">[i * n_img_per_row + j].</span><span 
class="hljs-built_in" style="margin: 0px; padding: 0px; color: rgb(102, 
0, 102); box-sizing: border-box;">reshape</span>((<span 
class="hljs-number" style="margin: 0px; padding: 0px; color: rgb(0, 102,
 102); box-sizing: border-box;">8</span>, <span 
class="hljs-number" style="margin: 0px; padding: 0px; color: rgb(0, 102,
 102); box-sizing: border-box;">8</span>))
<span class="hljs-transposed_variable" style="margin: 0px; padding: 
0px; box-sizing: border-box;">plt.</span>imshow(img, 
cmap=<span class="hljs-transposed_variable" style="margin: 0px; 
padding: 0px; box-sizing: border-box;">plt.</span><span 
class="hljs-transposed_variable" style="margin: 0px; padding: 0px; 
box-sizing: border-box;">cm.</span>binary)
<span class="hljs-transposed_variable" style="margin: 0px; padding: 
0px; box-sizing: border-box;">plt.</span>title(<span 
class="hljs-string" style="margin: 0px; padding: 0px; color: rgb(0, 136,
 0); box-sizing: border-box;">'A selection from the 64-dimensional 
digits dataset'</span>)</code><ul class="pre-numbering" 
style="margin: 0px; padding: 6px 0px 40px; box-sizing: border-box; 
position: absolute; width: 50px; top: 0px; left: 0px; 
border-right-width: 1px; border-right-style: solid; border-right-color: 
rgb(221, 221, 221); list-style: none; text-align: right; 
background-color: rgb(238, 238, 238);"><li style="margin: 0px; 
padding: 0px 5px; box-sizing: border-box;">1</li><li 
style="margin: 0px; padding: 0px 5px; box-sizing: 
border-box;">2</li><li style="margin: 0px; padding: 0px 5px;
 box-sizing: border-box;">3</li><li style="margin: 0px; 
padding: 0px 5px; box-sizing: border-box;">4</li><li 
style="margin: 0px; padding: 0px 5px; box-sizing: 
border-box;">5</li><li style="margin: 0px; padding: 0px 5px;
 box-sizing: border-box;">6</li><li style="margin: 0px; 
padding: 0px 5px; box-sizing: border-box;">7</li><li 
style="margin: 0px; padding: 0px 5px; box-sizing: 
border-box;">8</li><li style="margin: 0px; padding: 0px 5px;
 box-sizing: border-box;">9</li><li style="margin: 0px; 
padding: 0px 5px; box-sizing: border-box;">10</li><li 
style="margin: 0px; padding: 0px 5px; box-sizing: 
border-box;">11</li><li style="margin: 0px; padding: 0px 
5px; box-sizing: border-box;">12</li><li style="margin: 0px;
 padding: 0px 5px; box-sizing: border-box;">13</li></ul>
运行代码，获得X的大小是(901,64)，也就是901个样本。下图显示了部分样本：
![这里写图片描述](https://img-blog.csdn.net/20150522195128952)
### - 降维
以t-SNE为例子，代码如下，n_components设置为3，也就是将64维降到3维，init设置embedding的初始化方式，可选random或者pca，这里用pca，比起random init会更stable一些。
<code
 class="hljs erlang has-numbering" style="display: block; padding: 0px; 
color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', 
monospace;font-size:undefined; white-space: pre; border-radius: 0px; 
word-wrap: normal; background: transparent;"><span 
class="hljs-function" style="margin: 0px; padding: 0px; box-sizing: 
border-box;"><span class="hljs-title" style="margin: 0px; padding:
 0px; box-sizing: border-box;">print</span><span 
class="hljs-params" style="margin: 0px; padding: 0px; color: rgb(102, 0,
 102); box-sizing: border-box;">(<span class="hljs-string" 
style="margin: 0px; padding: 0px; color: rgb(0, 136, 0); box-sizing: 
border-box;">"Computing t-SNE embedding"</span>)</span>
<span class="hljs-title" style="margin: 0px; padding: 0px; 
box-sizing: border-box;">tsne</span> = <span 
class="hljs-title" style="margin: 0px; padding: 0px; box-sizing: 
border-box;">manifold</span>.TSNE<span class="hljs-params" 
style="margin: 0px; padding: 0px; color: rgb(102, 0, 102); box-sizing: 
border-box;">(n_components=<span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">3</span>, init='pca', random_state=<span 
class="hljs-number" style="margin: 0px; padding: 0px; color: rgb(0, 102,
 102); box-sizing: border-box;">0</span>)</span>
<span class="hljs-title" style="margin: 0px; padding: 0px; 
box-sizing: border-box;">t0</span> = <span 
class="hljs-title" style="margin: 0px; padding: 0px; box-sizing: 
border-box;">time</span><span class="hljs-params" 
style="margin: 0px; padding: 0px; color: rgb(102, 0, 102); box-sizing: 
border-box;">()</span>
X_<span class="hljs-title" style="margin: 0px; padding: 0px; 
box-sizing: border-box;">tsne</span> = <span 
class="hljs-title" style="margin: 0px; padding: 0px; box-sizing: 
border-box;">tsne</span>.<span class="hljs-title" 
style="margin: 0px; padding: 0px; box-sizing: 
border-box;">fit_transform</span><span class="hljs-params" 
style="margin: 0px; padding: 0px; color: rgb(102, 0, 102); box-sizing: 
border-box;">(<span class="hljs-variable" style="margin: 0px; 
padding: 0px; box-sizing: border-box;">X</span>)</span>
<span class="hljs-title" style="margin: 0px; padding: 0px; 
box-sizing: border-box;">plot_embedding_2d</span><span 
class="hljs-params" style="margin: 0px; padding: 0px; color: rgb(102, 0,
 102); box-sizing: border-box;">(<span class="hljs-variable" 
style="margin: 0px; padding: 0px; box-sizing: 
border-box;">X_tsne</span>[:,<span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">0</span>:<span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">2</span>],<span class="hljs-string" 
style="margin: 0px; padding: 0px; color: rgb(0, 136, 0); box-sizing: 
border-box;">"t-SNE 2D"</span>)</span>
<span class="hljs-title" style="margin: 0px; padding: 0px; 
box-sizing: border-box;">plot_embedding_3d</span><span 
class="hljs-params" style="margin: 0px; padding: 0px; color: rgb(102, 0,
 102); box-sizing: border-box;">(<span class="hljs-variable" 
style="margin: 0px; padding: 0px; box-sizing: 
border-box;">X_tsne</span>,<span class="hljs-string" 
style="margin: 0px; padding: 0px; color: rgb(0, 136, 0); box-sizing: 
border-box;">"t-SNE 3D (time %.2fs)"</span> <span 
class="hljs-comment" style="margin: 0px; padding: 0px; color: rgb(136, 
0, 0); box-sizing: border-box;">%(time() - 
t0))</span></span></span></code><ul 
class="pre-numbering" style="margin: 0px; padding: 6px 0px 40px; 
box-sizing: border-box; position: absolute; width: 50px; top: 0px; left:
 0px; border-right-width: 1px; border-right-style: solid; 
border-right-color: rgb(221, 221, 221); list-style: none; text-align: 
right; background-color: rgb(238, 238, 238);"><li style="margin: 
0px; padding: 0px 5px; box-sizing: border-box;">1</li><li 
style="margin: 0px; padding: 0px 5px; box-sizing: 
border-box;">2</li><li style="margin: 0px; padding: 0px 5px;
 box-sizing: border-box;">3</li><li style="margin: 0px; 
padding: 0px 5px; box-sizing: border-box;">4</li><li 
style="margin: 0px; padding: 0px 5px; box-sizing: 
border-box;">5</li><li style="margin: 0px; padding: 0px 5px;
 box-sizing: border-box;">6</li></ul>
降维后得到X_ tsne，大小是(901,3)，plot_ embedding_ 2d()将前2维数据可视化，plot_ embedding_ 3d()将3维数据可视化。
函数plot_ embedding_ 3d定义如下：
<code
 class="hljs python has-numbering" style="display: block; padding: 0px; 
color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', 
monospace;font-size:undefined; white-space: pre; border-radius: 0px; 
word-wrap: normal; background: transparent;"><span 
class="hljs-function" style="margin: 0px; padding: 0px; box-sizing: 
border-box;"><span class="hljs-keyword" style="margin: 0px; 
padding: 0px; color: rgb(0, 0, 136); box-sizing: 
border-box;">def</span> <span class="hljs-title" 
style="margin: 0px; padding: 0px; box-sizing: 
border-box;">plot_embedding_3d</span><span 
class="hljs-params" style="margin: 0px; padding: 0px; color: rgb(102, 0,
 102); box-sizing: border-box;">(X, 
title=None)</span>:</span>
    <span class="hljs-comment" style="margin: 0px; padding: 0px; 
color: rgb(136, 0, 0); box-sizing: 
border-box;">#坐标缩放到[0,1]区间</span>
    x_min, x_max = np.min(X,axis=<span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">0</span>), np.max(X,axis=<span 
class="hljs-number" style="margin: 0px; padding: 0px; color: rgb(0, 102,
 102); box-sizing: border-box;">0</span>)
    X = (X - x_min) / (x_max - x_min)
    <span class="hljs-comment" style="margin: 0px; padding: 0px; 
color: rgb(136, 0, 0); box-sizing: border-box;">#降维后的坐标为（X[i, 0], 
X[i, 1],X[i,2]），在该位置画出对应的digits</span>
    fig = plt.figure()
    ax = fig.add_subplot(<span class="hljs-number" style="margin: 
0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">1</span>, <span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">1</span>, <span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">1</span>, projection=<span class="hljs-string" 
style="margin: 0px; padding: 0px; color: rgb(0, 136, 0); box-sizing: 
border-box;">'3d'</span>)
    <span class="hljs-keyword" style="margin: 0px; padding: 0px; 
color: rgb(0, 0, 136); box-sizing: border-box;">for</span> i 
<span class="hljs-keyword" style="margin: 0px; padding: 0px; color: 
rgb(0, 0, 136); box-sizing: border-box;">in</span> 
range(X.shape[<span class="hljs-number" style="margin: 0px; padding: 
0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">0</span>]):
        ax.text(X[i, <span class="hljs-number" style="margin: 0px; 
padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">0</span>], X[i, <span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">1</span>], X[i,<span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">2</span>],str(digits.target[i]),
                 color=plt.cm.Set1(y[i] / <span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">10.</span>),
                 fontdict={<span class="hljs-string" style="margin: 
0px; padding: 0px; color: rgb(0, 136, 0); box-sizing: 
border-box;">'weight'</span>: <span class="hljs-string" 
style="margin: 0px; padding: 0px; color: rgb(0, 136, 0); box-sizing: 
border-box;">'bold'</span>, <span class="hljs-string" 
style="margin: 0px; padding: 0px; color: rgb(0, 136, 0); box-sizing: 
border-box;">'size'</span>: <span class="hljs-number" 
style="margin: 0px; padding: 0px; color: rgb(0, 102, 102); box-sizing: 
border-box;">9</span>})
    <span class="hljs-keyword" style="margin: 0px; padding: 0px; 
color: rgb(0, 0, 136); box-sizing: border-box;">if</span> title
 <span class="hljs-keyword" style="margin: 0px; padding: 0px; color: 
rgb(0, 0, 136); box-sizing: border-box;">is</span> <span 
class="hljs-keyword" style="margin: 0px; padding: 0px; color: rgb(0, 0, 
136); box-sizing: border-box;">not</span> <span 
class="hljs-keyword" style="margin: 0px; padding: 0px; color: rgb(0, 0, 
136); box-sizing: border-box;">None</span>:
        plt.title(title)</code><ul class="pre-numbering" 
style="margin: 0px; padding: 6px 0px 40px; box-sizing: border-box; 
position: absolute; width: 50px; top: 0px; left: 0px; 
border-right-width: 1px; border-right-style: solid; border-right-color: 
rgb(221, 221, 221); list-style: none; text-align: right; 
background-color: rgb(238, 238, 238);"><li style="margin: 0px; 
padding: 0px 5px; box-sizing: border-box;">1</li><li 
style="margin: 0px; padding: 0px 5px; box-sizing: 
border-box;">2</li><li style="margin: 0px; padding: 0px 5px;
 box-sizing: border-box;">3</li><li style="margin: 0px; 
padding: 0px 5px; box-sizing: border-box;">4</li><li 
style="margin: 0px; padding: 0px 5px; box-sizing: 
border-box;">5</li><li style="margin: 0px; padding: 0px 5px;
 box-sizing: border-box;">6</li><li style="margin: 0px; 
padding: 0px 5px; box-sizing: border-box;">7</li><li 
style="margin: 0px; padding: 0px 5px; box-sizing: 
border-box;">8</li><li style="margin: 0px; padding: 0px 5px;
 box-sizing: border-box;">9</li><li style="margin: 0px; 
padding: 0px 5px; box-sizing: border-box;">10</li><li 
style="margin: 0px; padding: 0px 5px; box-sizing: 
border-box;">11</li><li style="margin: 0px; padding: 0px 
5px; box-sizing: border-box;">12</li><li style="margin: 0px;
 padding: 0px 5px; box-sizing: border-box;">13</li></ul>
### - 看看效果
十多种算法，结果各有好坏，总体上t-SNE表现最优，但它的计算复杂度也是最高的。下面给出PCA、LDA、t-SNE的结果: 
![这里写图片描述](https://img-blog.csdn.net/20150522195334439)
![这里写图片描述](https://img-blog.csdn.net/20150522195314420)
![这里写图片描述](https://img-blog.csdn.net/20150522195347336)
![这里写图片描述](https://img-blog.csdn.net/20150522195443173)
![这里写图片描述](https://img-blog.csdn.net/20150522195502751)
![这里写图片描述](https://img-blog.csdn.net/20150522195440501)
### - 代码获取
[MachineLearning/ManifoldLearning/DimensionalityReduction_DataVisualizing](https://github.com/wepe/MachineLearning)
转载请注明出处：[http://blog.csdn.net/u012162613/article/details/45920827](http://blog.csdn.net/u012162613/article/details/45920827)
最后给大家几个有用的连接
论文：Visualizing data using t-SNE   [http://jmlr.org/papers/volume9/vandermaaten08a/vandermaaten08a.pdf](http://jmlr.org/papers/volume9/vandermaaten08a/vandermaaten08a.pdf)
User's Guide for t-SNE Software [http://homepage.tudelft.nl/19j49/t-SNE_files/User%20guide_1.pdf](http://homepage.tudelft.nl/19j49/t-SNE_files/User%20guide_1.pdf)
code 下载： [http://homepage.tudelft.nl/19j49/t-SNE.html](https://blog.csdn.net/sunshine_in_moon/article/details/51160012)
