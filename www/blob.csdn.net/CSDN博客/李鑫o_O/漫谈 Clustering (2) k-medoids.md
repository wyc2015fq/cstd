# 漫谈 Clustering (2): k-medoids - 李鑫o_O - CSDN博客





2016年05月13日 17:19:36[hustlx](https://me.csdn.net/HUSTLX)阅读数：624








[上一次](http://blog.pluskid.org/?p=17)我们了解了一个最基本的 clustering 办法 k-means ，这次要说的 k-medoids 算法，其实从名字上就可以看出来，和 k-means 肯定是非常相似的。事实也确实如此，k-medoids 可以算是 k-means 的一个变种。


k-medoids 和 k-means 不一样的地方在于中心点的选取，在 k-means 中，我们将中心点取为当前 cluster 中所有数据点的平均值：

![\displaystyle \mu_k=\frac{\sum_n r_{nk}x_n}{\sum_n r_{nk}}](http://blog.pluskid.org/latexrender/pictures/a0aa5b1fd15778697fc5f5c6f1c3f348.png)




![Rough Collie](http://blog.pluskid.org/wp-content/uploads/2009/01/rough-collie.jpg)

Rough Collie




并且我们已经证明在固定了各个数据点的 assignment 的情况下，这样选取的中心点能够把目标函数 ![J](http://blog.pluskid.org/latexrender/pictures/ff44570aca8241914870afbc310cdb85.png) 最小化。然而在 k-medoids 中，我们将中心点的选取限制在当前 cluster 所包含的数据点的集合中。换句话说，在
 k-medoids 算法中，我们将从当前 cluster 中选取这样一个点——它到其他所有（当前 cluster 中的）点的距离之和最小——作为中心点。k-means 和 k-medoids 之间的差异就类似于一个数据样本的均值 (mean) 和中位数 ([median](http://en.wikipedia.org/wiki/Median))
 之间的差异：前者的取值范围可以是连续空间中的任意值，而后者只能在给样本给定的那些点里面选。那么，这样做的好处是什么呢？

一个最直接的理由就是 k-means 对数据的要求太高了，它使用欧氏距离描述数据点之间的差异 (dissimilarity) ，从而可以直接通过求均值来计算中心点。这要求数据点处在一个欧氏空间之中。


然而并不是所有的数据都能满足这样的要求，对于数值类型的特征，比如身高，可以很自然地用这样的方式来处理，但是类别 (categorical) 类型的特征就不行了。举一个简单的例子，如果我现在要对犬进行聚类，并且希望直接在所有犬组成的空间中进行，k-means 就无能为力了，因为欧氏距离 ![\|x_i-x_j\|^2](http://blog.pluskid.org/latexrender/pictures/db49a5b307fd724c6760690b7ad233ee.png) 在这里不能用了：一只 [Samoyed](http://en.wikipedia.org/wiki/Samoyed_(dog))减去一只 [Rough
 Collie](http://en.wikipedia.org/wiki/Rough_Collie) 然后在平方一下？天知道那是什么！再加上一只[German Shepherd Dog](http://en.wikipedia.org/wiki/German_Shepherd_Dog) 然后求一下平均值？根本没法算，k-means 在这里寸步难行！


在 k-medoids 中，我们把原来的目标函数 ![J](http://blog.pluskid.org/latexrender/pictures/ff44570aca8241914870afbc310cdb85.png) 中的欧氏距离改为一个任意的 dissimilarity measure 函数 ![\mathcal{V}](http://blog.pluskid.org/latexrender/pictures/d8584311108d7851f1594414a743063e.png)：


最常见的方式是构造一个 dissimilarity matrix ![\mathbf{D}](http://blog.pluskid.org/latexrender/pictures/86e66312d234b4e2a7008b17a7412da4.png) 来代表 ![\mathcal{V}](http://blog.pluskid.org/latexrender/pictures/d8584311108d7851f1594414a743063e.png)，其中的元素 ![\mathbf{D}_{ij}](http://blog.pluskid.org/latexrender/pictures/bed1de03baf3d934f3a64b1e4b38ebb8.png)表示第 ![i](http://blog.pluskid.org/latexrender/pictures/865c0c0b4ab0e063e5caa3387c1a8741.png) 只狗和第 ![j](http://blog.pluskid.org/latexrender/pictures/363b122c528f54df4a0446b6bab05515.png) 只狗之间的差异程度，例如，两只 [Samoyed](http://en.wikipedia.org/wiki/Samoyed_(dog)) 之间的差异可以设为
 0 ，一只 [German Shepherd Dog](http://en.wikipedia.org/wiki/German_Shepherd_Dog) 和一只 [Rough
 Collie](http://en.wikipedia.org/wiki/Rough_Collie) 之间的差异是 0.7，和一只 [Miniature Schnauzer](http://en.wikipedia.org/wiki/Miniature_Schnauzer) 之间的差异是 1 ，等等。


除此之外，由于中心点是在已有的数据点里面选取的，因此相对于 k-means 来说，不容易受到那些由于误差之类的原因产生的 [Outlier](http://en.wikipedia.org/wiki/Outlier) 的影响，更加 robust 一些。


扯了这么多，还是直接来看看 k-medoids 的效果好了，由于 k-medoids 对数据的要求比 k-means 要低，所以 k-means 能处理的情况自然 k-medoids 也能处理，为了能先睹为快，我们偷一下懒，直接在[上一篇文章](http://blog.pluskid.org/?p=17)中的 k-means
 代码的基础上稍作一点修改，还用同样的例子。将代码的 45 到 47 行改成下面这样：



```cpp
for j in range(k):
            idx_j = (labels == j).nonzero()
            distj = distmat(X[idx_j], X[idx_j])
            distsum = ml.sum(distj, axis=1)
            icenter = distsum.argmin()
            centers[j] = X[idx_j[0][icenter]]
```




可以看到 k-medoids 在这个例子上也能得到很好的结果：

![iter_06](http://blog.pluskid.org/wp-content/uploads/2009/01/iter_06.png)


而且，同 k-means 一样，运气不好的时候也会陷入局部最优解中：

![iter_08](http://blog.pluskid.org/wp-content/uploads/2009/01/iter_08.png)


如果仔细看上面那段代码的话，就会发现，从 k-means 变到 k-medoids ，时间复杂度陡然增加了许多：在 k-means 中只要求一个平均值 ![O(N)](http://blog.pluskid.org/latexrender/pictures/33697ce7dfa48ba80980d298c8089378.png) 即可，而在 k-medoids 中则需要枚举每个点，并求出它到所有其他点的距离之和，复杂度为 ![O(N^2)](http://blog.pluskid.org/latexrender/pictures/8e9c5fee65a4f32abccd0e83ff203e39.png) 。


看完了直观的例子，让我们再来看一个稍微实际一点的例子好了：Document Clustering ——那个永恒不变的主题，不过我们这里要做的聚类并不是针对文档的主题，而是针对文档的语言。实验数据是从 [Europarl](http://www.statmt.org/europarl/) 下载的包含 Danish、German、Greek、English、Spanish、Finnish、French、Italian、Dutch、Portuguese
 和 Swedish 这些语言的文本数据集。


在 [N-gram-based text categorization](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.53.9367) 这篇 paper 中描述了一种计算由不同语言写成的文档的相似度的方法。一个（以字符为单位的） [N-gram](http://en.wikipedia.org/wiki/N-gram) 就相当于长度为
 N 的一系列连续子串。例如，由 hello 产生的 3-gram 为：hel、ell 和 llo ，有时候还会在划分 N-gram 之前在开头和末尾加上空格（这里用下划线表示）：_he、hel、ell、llo、lo_ 和 o__ 。按照 [Zipf’s
 law](http://en.wikipedia.org/wiki/Zipf%27s_law)：

> 

The *n*th most common word in a human language text occurs with a frequency inversely proportional to *n*.



这里我们用 N-gram 来代替 word 。这样，我们从一个文档中可以得到一个 N-gram 的频率分布，按照频率排序一下，只保留频率最高的前 k 个（比如，300）N-gram，我们把叫做一个“Profile”。正常情况下，某一种语言（至少是西方国家的那些类英语的语言）写成的文档，不论主题或长短，通常得出来的 Profile 都差不多，亦即按照出现的频率排序所得到的各个 N-gram 的序号不会变化太大。这是非常好的一个性质：通常我们只要各个语言选取一篇（比较正常的，也不需要很长）文档构建出一个 Profile
 ，在拿到一篇未知文档的时候，只要和各个 Profile 比较一下，差异最小的那个 Profile 所对应的语言就可以认定是这篇未知文档的语言了——准确率很高，更可贵的是，所需要的训练数据非常少而且容易获得，训练出来的模型也是非常小的。


不过，我们这里且撇开分类（Classification）的问题，回到聚类（Clustering）上，按照前面的说法，在 k-medoids 聚类中，只需要定义好两个东西之间的距离（或者 dissimilarity ）就可以了，对于两个 Profile ，它们之间的 dissimilarity 可以很自然地定义为对应的 N-gram 的序号之差的绝对值，在 Python 中用下面这样一个类来表示：



```python
class Profile(object):
    def __init__(self, path, N=3, psize=400):
        self.N = N
        self.psize = psize
        self.build_profile(path)
 
    sep = re.compile(r'\W+')
    def build_profile(self, path):
        grams = {}
        with open(path) as inf:
            for line in inf:
                for tok in self.sep.split(line):
                    for n in range(self.N):
                        self.feed_ngram(grams, tok, n+1)
        self.create_profile(grams.items())
 
    def create_profile(self, grams):
        # keep only the top most psize items
        grams.sort(key=itemgetter(1), reverse=True)
        grams = grams[:self.psize]
 
        self.profile = dict()
        for i in range(len(grams)):
            self.profile[grams[i][0]] = i
 
    def __getitem__(self, key):
        idx = self.profile.get(key)
        if idx is None:
            return len(self.profile)
        return idx
 
    def dissimilarity(self, o):
        dis = 0
        for tok in self.profile.keys():
            dis += abs(self[tok]-o[tok])
        for tok in o.profile.keys():
            dis += abs(self[tok]-o[tok])
        return dis
 
    def feed_ngram(self, grams, tok, n):
        if n != 0:
            tok = '_' + tok
        tok = tok + '_' * (n-1)
        for i in range(len(tok)-n+1):
            gram = tok[i:i+n]
            grams.setdefault(gram, 0)
            grams[gram] += 1
```
europarl 数据集共有 11 种语言的文档，每种语言包括大约 600 多个文档。我为这七千多个文档建立了 Profile 并构造出一个 7038×7038 的 dissimilarity matrix ，最后在这上面用 k-medoids 进行聚类。构造 dissimilarity matrix 的过程很慢，在我这里花了将近 10 个小时。相比之下，k-medoids 的过程在内存允许的情况下，采用向量化的方法来做实际上还是很快的，并且通常只要数次迭代就能收敛了。实际的
 k-medoids 实现可以在 mltk 中找到，今后如果有时间的话，我会陆续地把一些相关的比较通用的代码放到那里面。

```python

```




最后，然我们来看看聚类的结果，由于聚类和分类不同，只是得到一些 cluster ，而并不知道这些 cluster 应该被打上什么标签，或者说，在这个问题里，包含了哪种语言的文档。由于我们的目的是衡量聚类算法的 performance ，因此直接假定这一步能实现最优的对应关系，将每个 cluster 对应到一种语言上去。一种办法是枚举所有可能的情况并选出最优解，另外，对于这样的问题，我们还可以用 [Hungarian
 algorithm](http://en.wikipedia.org/wiki/Hungarian_algorithm) 来求解。


我们这里有 11 种语言，全排列有 11! = 39916800 种情况， 对于每一种排列，我们需要遍历一次 label list ，并数出真正的 label （语言）与聚类得出的结果相同的文档的个数，再除以总的文档个数，得到 accuracy 。假设每次遍历并求出 accuracy 只需要 1 毫秒的时间的话，总共也需要 11 个小时才能得到结果。看上去好像也不是特别恐怖，不过相比起来，用 Hungarian algorithm 的话，我们可以几乎瞬间得到结果。由于文章的篇幅已经很长了，就不在这里介绍具体的算法了，感兴趣的同学可以参考[Wikipedia](http://en.wikipedia.org/wiki/Hungarian_algorithm) ，这里我直接使用了一个现有的 [Python
 实现](http://www.clapper.org/software/python/munkres/)。


虽然这个实验非常折腾，不过最后的结果其实就是一个数字：accuracy ——在我这里达到了 88.97% ，证明 k-medoids 聚类和 N-gram Profile 识别语言这两种方法都是挺不错的。最后，如果有感兴趣的同学，代码可以从[这里](http://blog.pluskid.org/wp-content/uploads/2009/01/ngram_cluster.zip)下载。需要最新版的 [scipy](http://www.scipy.org/)， [munkres.py](http://www.clapper.org/software/python/munkres/) 和 [mltk](http://github.com/pluskid/mltk/) 以及
 Python 2.6 。



