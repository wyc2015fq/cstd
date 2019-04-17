# hxy系列9-神经网络小例子 - 战斗蜗牛的专栏 - CSDN博客





2016年09月29日 15:44:13[vbskj](https://me.csdn.net/vbskj)阅读数：490









作者：[寒小阳](http://blog.csdn.net/han_xiaoyang?viewmode=contents)

时间：2016年1月。 

出处：[http://blog.csdn.net/han_xiaoyang/article/details/50521072](http://blog.csdn.net/han_xiaoyang/article/details/50521072)

声明：版权所有，转载请联系作者并注明出处
## 1.引言


前面8小节，算从神经网络的结构、简单原理、数据准备与处理、神经元选择、损失函数选择等方面把神经网络过了一遍。这个部分我们打算把知识点串一串，动手实现一个简单的2维平面神经网络分类器，去分割平面上的不同类别样本点。为了循序渐进，我们打算先实现一个简单的线性分类器，然后再拓展到非线性的2层神经网络。我们可以看到简单的浅层神经网络，在这个例子上就能够有分割程度远高于线性分类器的效果。

## 2.样本数据的产生


为了凸显一下神经网络强大的空间分割能力，我们打算产生出一部分对于线性分类器不那么容易分割的样本点，比如说我们生成一份螺旋状分布的样本点，如下：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">N = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">100</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 每个类中的样本点</span>
D = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">2</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 维度</span>
K = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">3</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 类别个数</span>
X = np.zeros((N*K,D)) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 样本input</span>
y = np.zeros(N*K, dtype=<span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'uint8'</span>) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 类别标签</span>
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">for</span> j <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">in</span> xrange(K):
  ix = range(N*j,N*(j+<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>))
  r = np.linspace(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.0</span>,<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>,N) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># radius</span>
  t = np.linspace(j*<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">4</span>,(j+<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>)*<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">4</span>,N) + np.random.randn(N)*<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.2</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># theta</span>
  X[ix] = np.c_[r*np.sin(t), r*np.cos(t)]
  y[ix] = j
<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 可视化一下我们的样本点</span>
plt.scatter(X[:, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>], X[:, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>], c=y, s=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">40</span>, cmap=plt.cm.Spectral)</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li></ul>

得到如下的样本分布： 


![螺旋样本点集](http://cs231n.github.io/assets/eg/spiral_raw.png)




紫色，红色和黄色分布代表不同的3种类别。


一般来说，拿到数据都要做预处理，包括之前提到的去均值和方差归一化。不过我们构造的数据幅度已经在-1到1之间了，所以这里不用做这个操作。

## 3.使用Softmax线性分类器

### 3.1 初始化参数


我们先在训练集上用softmax线性分类器试试。如我们在[之前的章节](http://blog.csdn.net/han_xiaoyang/article/details/49999583)提到的，我们这里用的softmax分类器，使用的是一个线性的得分函数/score function，使用的损失函数是交叉熵损失/cross-entropy loss。包含的参数包括得分函数里面用到的权重矩阵`W`和偏移量`b`，我们先随机初始化这些参数。
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#随机初始化参数</span>
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">import</span> numpy <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">as</span> np
<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#D=2表示维度，K=3表示类别数</span>
W = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.01</span> * np.random.randn(D,K)
b = np.zeros((<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>,K))</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li></ul>
### 3.2 计算得分


线性的得分函数，将原始的数据映射到得分域非常简单，只是一个直接的矩阵乘法。
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#使用得分函数计算得分</span>
scores = np.dot(X, W) + b</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li></ul>

在我们给的这个例子中，我们有2个2维点集，所以做完乘法过后，矩阵得分`scores`其实是一个[300*3]的矩阵，每一行都给出对应3各类别(紫，红，黄)的得分。

### 3.3 计算损失


然后我们就要开始使用我们的损失函数计算`损失`了，我们之前也提到过，损失函数计算出来的结果代表着预测结果和真实结果之间的吻合度，我们的目标是最小化这个结果。直观一点理解，我们希望对每个样本而言，对应正确类别的得分高于其他类别的得分，如果满足这个条件，那么损失函数计算的结果是一个比较低的值，如果判定的类别不是正确类别，则结果值会很高。我们[之前](http://blog.csdn.net/han_xiaoyang/article/details/49999583)提到了，softmax分类器里面，使用的损失函数是交叉熵损失。一起回忆一下，假设`f`是得分向量，那么我们的交叉熵损失是用如下的形式定义的：








直观地理解一下上述形式，就是Softmax分类器把类别得分向量`f`中每个值都看成对应三个类别的log似然概率。因此我们在求每个类别对应概率的时候，使用指数函数还原它，然后归一化。从上面形式里面大家也可以看得出来，得到的值总是在0到1之间的，因此从某种程度上说我们可以把它理解成概率。如果判定类别是错误类别，那么上述公式的结果就会趋于无穷，也就是说损失相当相当大，相反，如果判定类别正确，那么损失就接近。这和我们直观理解上要最小化`损失`是完全吻合的。


当然，当然，别忘了，完整的损失函数定义，一定会加上正则化项，也就是说，完整的损失`L`应该有如下的形式：








好，我们实现以下，根据上面计算得到的得分`scores`，我们计算以下各个类别上的概率：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 用指数函数还原</span>
exp_scores = np.exp(scores)
<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 归一化</span>
probs = exp_scores / np.sum(exp_scores, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>, keepdims=<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">True</span>)</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li></ul>

在我们的例子中，我们最后得到了一个[300*3]的概率矩阵`prob`，其中每一行都包含属于3个类别的概率。然后我们就可以计算完整的互熵损失了：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#计算log概率和互熵损失</span>
corect_logprobs = -np.log(probs[range(num_examples),y])
data_loss = np.sum(corect_logprobs)/num_examples
<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#加上正则化项</span>
reg_loss = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.5</span>*reg*np.sum(W*W)
loss = data_loss + reg_loss</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li></ul>

正则化强度在上述代码中是reg，最开始的时候我们可能会得到`loss=1.1`，是通过`np.log(1.0/3)`得到的(假定初始的时候属于3个类别的概率一样)，我们现在想最小化损失`loss`

### 3.4 计算梯度与梯度回传


我们能够用损失函数评估预测值与真实值之间的差距，下一步要做的事情自然是最小化这个值。我们用传统的梯度下降来解决这个问题。多解释一句，梯度下降的过程是：我们先选取一组随机参数作为初始值，然后计算损失函数在这组参数上的梯度(负梯度的方向表明了损失函数减小的方向)，接着我们朝着负梯度的方向迭代和更新参数，不断重复这个过程直至损失函数最小化。为了清楚一点说明这个问题，我们引入一个中间变量，它是归一化后的概率向量，如下：








我们现在希望知道朝着哪个方向调整权重能够减小损失，也就是说，我们需要计算梯度。损失从计算而来，再退一步，依赖于。于是我们又要做高数题，使用链式求导法则了，不过梯度的结果倒是非常简单：








解释一下，公式的最后一个部分表示的时候，取值为1。整个公式其实非常的优雅和简单。假设我们计算的概率p=[0.2,
 0.3, 0.5]，而中间的类别才是真实的结果类别。根据梯度求解公式，我们得到梯度。我们想想梯度的含义，其实这个结果是可解释性非常高的：大家都知道，梯度是最快上升方向，我们减掉它乘以步长才会让损失函数值减小。第1项和第3项(其实就是不正确的类别项)梯度为正，表明增加它们只会让最后的损失/loss增大，而我们的目标是减小loss；中间的梯度项-0.7其实再告诉我们，增加这一项，能减小损失，达到我们最终的目的。


我们依旧记`probs`为所有样本属于各个类别的概率，记`dscores`为得分上的梯度，我们可以有以下的代码：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">dscores = probs
dscores[range(num_examples),y] -= <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>
dscores /= num_examples</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li></ul>

我们计算的得分`scores = np.dot(X, W)+b`，因为上面已经算好了`scores`的梯度`dscores`，我们现在可以回传梯度计算W和b了：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">dW = np.dot(X.T, dscores)
db = np.sum(dscores, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>, keepdims=<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">True</span>)
<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#得记着正则化梯度哈</span>
dW += reg*W</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li></ul>

我们通过矩阵的乘法得到梯度部分，权重W的部分加上了正则化项的梯度。因为我们在设定正则化项的时候用了系数`0.5`（因为），因此直接用`reg*W`就可以表示出正则化的梯度部分。

### 3.5 参数迭代与更新


在得到所需的所有部分之后，我们就可以进行参数更新了：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#参数迭代更新</span>
W += -step_size * dW
b += -step_size * db</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li></ul>
### 3.6 大杂合：训练SoftMax分类器
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#代码部分组一起，训练线性分类器</span>

<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#随机初始化参数</span>
W = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.01</span> * np.random.randn(D,K)
b = np.zeros((<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>,K))

<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#需要自己敲定的步长和正则化系数</span>
step_size = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1e-0</span>
reg = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1e-3</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#正则化系数</span>

<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#梯度下降迭代循环</span>
num_examples = X.shape[<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>]
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">for</span> i <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">in</span> xrange(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">200</span>):

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 计算类别得分, 结果矩阵为[N x K]</span>
  scores = np.dot(X, W) + b 

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 计算类别概率</span>
  exp_scores = np.exp(scores)
  probs = exp_scores / np.sum(exp_scores, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>, keepdims=<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">True</span>) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># [N x K]</span>

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 计算损失loss(包括互熵损失和正则化部分)</span>
  corect_logprobs = -np.log(probs[range(num_examples),y])
  data_loss = np.sum(corect_logprobs)/num_examples
  reg_loss = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.5</span>*reg*np.sum(W*W)
  loss = data_loss + reg_loss
  <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">if</span> i % <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">10</span> == <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>:
    <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">print</span> <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"iteration %d: loss %f"</span> % (i, loss)

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 计算得分上的梯度</span>
  dscores = probs
  dscores[range(num_examples),y] -= <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>
  dscores /= num_examples

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 计算和回传梯度</span>
  dW = np.dot(X.T, dscores)
  db = np.sum(dscores, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>, keepdims=<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">True</span>)

  dW += reg*W <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 正则化梯度</span>

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#参数更新</span>
  W += -step_size * dW
  b += -step_size * db</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li><li style="box-sizing: border-box; padding: 0px 5px;">40</li><li style="box-sizing: border-box; padding: 0px 5px;">41</li><li style="box-sizing: border-box; padding: 0px 5px;">42</li><li style="box-sizing: border-box; padding: 0px 5px;">43</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li><li style="box-sizing: border-box; padding: 0px 5px;">40</li><li style="box-sizing: border-box; padding: 0px 5px;">41</li><li style="box-sizing: border-box; padding: 0px 5px;">42</li><li style="box-sizing: border-box; padding: 0px 5px;">43</li></ul>

得到结果：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1.096956</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">10</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.917265</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">20</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.851503</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">30</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.822336</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">40</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.807586</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">50</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.799448</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">60</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.794681</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">70</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.791764</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">80</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.789920</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">90</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.788726</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">100</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.787938</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">110</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.787409</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">120</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.787049</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">130</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.786803</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">140</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.786633</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">150</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.786514</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">160</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.786431</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">170</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.786373</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">180</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.786331</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">190</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.786302</span></code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li></ul>

190次循环之后，结果大致收敛了。我们评估一下准确度：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#评估准确度</span>
scores = np.dot(X, W) + b
predicted_class = np.argmax(scores, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>)
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">print</span> <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'training accuracy: %.2f'</span> % (np.mean(predicted_class == y))</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li></ul>

输出结果为49%。不太好，对吧？实际上也是可理解的，你想想，一份螺旋形的数据，你偏执地要用一个线性分类器去分割，不管怎么调整这个线性分类器，都非常非常困难。我们可视化一下数据看看决策边界(decision boundaries)： 


![线性分类器与决策边界](http://cs231n.github.io/assets/eg/spiral_linear.png)



## 4.使用神经网络分类


从刚才的例子里可以看出，一个线性分类器，在现在的数据集上效果并不好。我们知道神经网络可以做非线性的分割，那我们就试试神经网络，看看会不会有更好的效果。对于这样一个简单问题，我们用单隐藏层的神经网络就可以了，这样一个神经网络我们需要2层的权重和偏移量：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 初始化参数</span>
h = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">100</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 隐层大小(神经元个数)</span>
W = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.01</span> * np.random.randn(D,h)
b = np.zeros((<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>,h))
W2 = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.01</span> * np.random.randn(h,K)
b2 = np.zeros((<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>,K))</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li></ul>

然后前向计算的过程也稍有一些变化：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#2层神经网络的前向计算</span>
hidden_layer = np.maximum(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>, np.dot(X, W) + b) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 用的 ReLU单元</span>
scores = np.dot(hidden_layer, W2) + b2</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li></ul>

注意到这里，和之前线性分类器中的得分计算相比，多了一行代码计算，我们首先计算第一层神经网络结果，然后作为第二层的输入，计算最后的结果。哦，对了，代码里大家也看的出来，我们这里使用的是ReLU神经单元。


其他的东西都没太大变化。我们依旧按照之前的方式去计算loss，然后计算梯度`dscores`。不过反向回传梯度的过程形式上也有一些小小的变化。我们看下面的代码，可能觉得和Softmax分类器里面看到的基本一样，但注意到我们用`hidden_layer`替换掉了之前的`X`:
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 梯度回传与反向传播</span>
<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 对W2和b2的第一次计算</span>
dW2 = np.dot(hidden_layer.T, dscores)
db2 = np.sum(dscores, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>, keepdims=<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">True</span>)</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li></ul>

恩，并没有完事啊，因为`hidden_layer`本身是一个包含其他参数和数据的函数，我们得计算一下它的梯度：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">dhidden = np.dot(dscores, W2.T)</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li></ul>

现在我们有隐层输出的梯度了，下一步我们要反向传播到ReLU神经元了。不过这个计算非常简单，因为，同时我们又有。用链式法则串起来后，我们可以看到，回传的梯度大于0的时候，经过ReLU之后，保持原样；如果小于0，那本次回传就到此结束了。因此，我们这一部分非常简单：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#梯度回传经过ReLU</span>
dhidden[hidden_layer <= <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>] = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span></code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li></ul>

终于，翻山越岭，回到第一层，拿到总的权重和偏移量的梯度：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">dW = np.dot(X.T, dhidden)
db = np.sum(dhidden, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>, keepdims=<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">True</span>)</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li></ul>

来，来，来。组一组，我们把整个神经网络的过程串起来：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 随机初始化参数</span>
h = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">100</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 隐层大小</span>
W = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.01</span> * np.random.randn(D,h)
b = np.zeros((<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>,h))
W2 = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.01</span> * np.random.randn(h,K)
b2 = np.zeros((<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>,K))

<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 手动敲定的几个参数</span>
step_size = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1e-0</span>
reg = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1e-3</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 正则化参数</span>

<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 梯度迭代与循环</span>
num_examples = X.shape[<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>]
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">for</span> i <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">in</span> xrange(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">10000</span>):

  hidden_layer = np.maximum(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>, np.dot(X, W) + b) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#使用的ReLU神经元</span>
  scores = np.dot(hidden_layer, W2) + b2

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 计算类别概率</span>
  exp_scores = np.exp(scores)
  probs = exp_scores / np.sum(exp_scores, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>, keepdims=<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">True</span>) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># [N x K]</span>

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 计算互熵损失与正则化项</span>
  corect_logprobs = -np.log(probs[range(num_examples),y])
  data_loss = np.sum(corect_logprobs)/num_examples
  reg_loss = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.5</span>*reg*np.sum(W*W) + <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.5</span>*reg*np.sum(W2*W2)
  loss = data_loss + reg_loss
  <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">if</span> i % <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1000</span> == <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>:
    <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">print</span> <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"iteration %d: loss %f"</span> % (i, loss)

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 计算梯度</span>
  dscores = probs
  dscores[range(num_examples),y] -= <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>
  dscores /= num_examples

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 梯度回传</span>
  dW2 = np.dot(hidden_layer.T, dscores)
  db2 = np.sum(dscores, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>, keepdims=<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">True</span>)

  dhidden = np.dot(dscores, W2.T)

  dhidden[hidden_layer <= <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>] = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>
  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 拿到最后W,b上的梯度</span>
  dW = np.dot(X.T, dhidden)
  db = np.sum(dhidden, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>, keepdims=<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">True</span>)

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 加上正则化梯度部分</span>
  dW2 += reg * W2
  dW += reg * W

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 参数迭代与更新</span>
  W += -step_size * dW
  b += -step_size * db
  W2 += -step_size * dW2
  b2 += -step_size * db2</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li><li style="box-sizing: border-box; padding: 0px 5px;">40</li><li style="box-sizing: border-box; padding: 0px 5px;">41</li><li style="box-sizing: border-box; padding: 0px 5px;">42</li><li style="box-sizing: border-box; padding: 0px 5px;">43</li><li style="box-sizing: border-box; padding: 0px 5px;">44</li><li style="box-sizing: border-box; padding: 0px 5px;">45</li><li style="box-sizing: border-box; padding: 0px 5px;">46</li><li style="box-sizing: border-box; padding: 0px 5px;">47</li><li style="box-sizing: border-box; padding: 0px 5px;">48</li><li style="box-sizing: border-box; padding: 0px 5px;">49</li><li style="box-sizing: border-box; padding: 0px 5px;">50</li><li style="box-sizing: border-box; padding: 0px 5px;">51</li><li style="box-sizing: border-box; padding: 0px 5px;">52</li><li style="box-sizing: border-box; padding: 0px 5px;">53</li><li style="box-sizing: border-box; padding: 0px 5px;">54</li><li style="box-sizing: border-box; padding: 0px 5px;">55</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li><li style="box-sizing: border-box; padding: 0px 5px;">40</li><li style="box-sizing: border-box; padding: 0px 5px;">41</li><li style="box-sizing: border-box; padding: 0px 5px;">42</li><li style="box-sizing: border-box; padding: 0px 5px;">43</li><li style="box-sizing: border-box; padding: 0px 5px;">44</li><li style="box-sizing: border-box; padding: 0px 5px;">45</li><li style="box-sizing: border-box; padding: 0px 5px;">46</li><li style="box-sizing: border-box; padding: 0px 5px;">47</li><li style="box-sizing: border-box; padding: 0px 5px;">48</li><li style="box-sizing: border-box; padding: 0px 5px;">49</li><li style="box-sizing: border-box; padding: 0px 5px;">50</li><li style="box-sizing: border-box; padding: 0px 5px;">51</li><li style="box-sizing: border-box; padding: 0px 5px;">52</li><li style="box-sizing: border-box; padding: 0px 5px;">53</li><li style="box-sizing: border-box; padding: 0px 5px;">54</li><li style="box-sizing: border-box; padding: 0px 5px;">55</li></ul>

输出结果：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1.098744</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1000</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.294946</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">2000</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.259301</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">3000</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.248310</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">4000</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.246170</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">5000</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.245649</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">6000</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.245491</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">7000</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.245400</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">8000</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.245335</span>
iteration <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">9000</span>: loss <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0.245292</span></code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li></ul>

现在的训练准确度为：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#计算分类准确度</span>
hidden_layer = np.maximum(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>, np.dot(X, W) + b)
scores = np.dot(hidden_layer, W2) + b2
predicted_class = np.argmax(scores, axis=<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>)
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">print</span> <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'training accuracy: %.2f'</span> % (np.mean(predicted_class == y))</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li></ul>

你猜怎么着，准确度为98%，我们可视化一下数据和现在的决策边界： 


![神经网络决策边界](http://cs231n.github.io/assets/eg/spiral_net.png)




看起来效果比之前好多了，这充分地印证了我们在[手把手入门神经网络系列(1)_从初等数学的角度初探神经网络](http://blog.csdn.net/han_xiaoyang/article/details/50100367)中提到的，神经网络对于空间强大的分割能力，对于非线性的不规则图形，能有很强的划分区域和区分类别能力。

## 参考资料与原文

[cs231n 神经网络小例子](http://cs231n.github.io/neural-networks-case-study/)




