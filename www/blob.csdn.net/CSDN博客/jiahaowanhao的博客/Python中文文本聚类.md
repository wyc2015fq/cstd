
# Python中文文本聚类 - jiahaowanhao的博客 - CSDN博客


2018年04月03日 21:41:24[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：314


[Python中文文本聚类](http://cda.pinggu.org/view/25167.html)
查看百度搜索中文文本聚类我失望的发现，网上竟然没有一个完整的关于Python实现的中文文本聚类（乃至搜索关键词python 中文文本聚类也是如此），网上大部分是关于文本聚类的Kmeans聚类的原理，Java实现，R语言实现，甚至都有一个C++的实现。
正好我写的一些文章，我没能很好的分类，我想能不能通过聚类的方法将一些相似的文章进行聚类，然后我再看每个聚类大概的主题是什么，给每个聚类一个标签，这样也是完成了分类。
中文文本聚类主要有一下几个步骤，下面将分别详细介绍：
切词
去除停用词
构建词袋空间VSM(vector space model)
TF-IDF构建词权重
使用K-means算法
一、 切词
这里中文切词使用的是结巴切词，github项目主页，作者微博
github项目主页上有结巴切词的详细安装方式，以及示例说明，这里不再详述，一般情况下，可以使用如下方式安装。
\# pip install jieba
或者
\# easy_install jieba
还可以参考一下文章：
1.Python中文分词组件 jieba
2.python 结巴分词(jieba)学习
二、 去除停用词
结巴分词虽然有去除停用词的功能，但是好像只是给jieba.analyse组建使用的，并不给jieba.cut使用，所以这里我们还是要自己构建停用词文件，以及去除停用词。
常见的中文停用词有：
1. 中文停用词表(比较全面,有1208个停用词)
2. 最全中文停用词表整理（1893个）
实现代码如下（代码比较水）：
def read_from_file(file_name):
with open(file_name,"r") as fp:
words = fp.read()
return words
def stop_words(stop_word_file):
words = read_from_file(stop_word_file)
result = jieba.cut(words)
new_words = []
for r in result:
new_words.append(r)
return set(new_words)
def del_stop_words(words,stop_words_set):
\#   words是已经切词但是没有去除停用词的文档。
\#   返回的会是去除停用词后的文档
result = jieba.cut(words)
new_words = []
for r in result:
if r not in stop_words_set:
new_words.append(r)
return new_words
三、 构建词袋空间VSM(vector space model)
接下来是构建词袋空间，我们的步骤如下
1. 将所有文档读入到程序中，再将每个文档切词。
2. 去除每个文档中的停用词。
3. 统计所有文档的词集合（sk-learn有相关函数，但是我知道能对中文也使用）。
4. 对每个文档，都将构建一个向量，向量的值是词语在本文档中出现的次数。
这举个例子，假设有两个文本，1.我爱上海，我爱中国2.中国伟大,上海漂亮
那么切词之后就有一下词语：我，爱，上海，中国，伟大，漂亮，,(逗号也可能被切词)。
再假设停用词是我 ，，那么去除停用词后，剩余的词语就是
爱，上海，中国，伟大，漂亮
然后我们对文档1和文档2构建向量，那么向量将如下：
文本     爱     上海     中国     伟大     漂亮
文档1     2     1     1     0     0
文档2     0     1     1     1     1
代码如下：
def get_all_vector(file_path,stop_words_set):
names = [ os.path.join(file_path,f) for f in os.listdir(file_path) ]
posts = [ open(name).read() for name in names ]
docs = []
word_set = set()
for post in posts:
doc = del_stop_words(post,stop_words_set)
docs.append(doc)
word_set |= set(doc)
\#print len(doc),len(word_set)
word_set = list(word_set)
docs_vsm = []
\#for word in word_set[:30]:
\#print word.encode("utf-8"),
for doc in docs:
temp_vector = []
for word in word_set:
temp_vector.append(doc.count(word) * 1.0)
\#print temp_vector[-30:-1]
docs_vsm.append(temp_vector)
docs_matrix = np.array(docs_vsm)
在python中表示可能如下[[2,1,1,0,0],[0,1,1,1,]]，我们尽可能将其放入到numpy的array或者matrix中方便下面TF-IDF的计算。
四、 将单词出现的次数转化为权值（TF-IDF）
换句话说，我们的vsm保存的本来已经是向量的形式，我们为什么还需要TF-IDF的形式呢？我认为这就是为了将单词出现的次数转化为权值。
关于TF-IDF的介绍可以参考网上的文章：
1. 基本文本聚类方法
2. TF-IDF百度百科
3. TF-IDF维基百科英文版(需要翻墙)
这里需要注意的是关于TF(term frequency)的计算，关于IDF(Inverse document frequency)的计算，我看公式基本上都是一样的：
逆向文件频率（inverse document frequency，IDF）是一个词语普遍重要性的度量。某一特定词语的IDF，可以由总文件数目除以包含该词语之文件的数目，再将得到的商取对数得到：
本公式用编辑，推荐一个令人惊叹的网站：Detexify
其中
：语料库中的文件总数
：包含词语的文件数目（即的文件数目）如果该词语不在语料库中，就会导致分母为零，因此一般情况下使用作为分母。
然而百度百科以及网上大部分关于TF的介绍其实是有问题的，TF-IDF百度百科中说词频（term frequency，TF）指的是某一个给定的词语在该文件中出现的频率，那么很明显这个计算公式就为：
然而这种计算方式常常会导致TF过小，其实TF-IDF并不是只有一种计算方式，而是多种，这个时候就体现出维基百科的威力了，具体的关于TF-IDF的介绍还是要参照维基百科。
如果不熟悉numpy，可以参考numpy官方文档
column_sum = [ float(len(np.nonzero(docs_matrix[:,i])[0])) for i in range(docs_matrix.shape[1]) ]
column_sum = np.array(column_sum)
column_sum = docs_matrix.shape[0] / column_sum
idf =  np.log(column_sum)
idf =  np.diag(idf)
\# 请仔细想想，根绝IDF的定义，计算词的IDF并不依赖于某个文档，所以我们提前计算好。
\# 注意一下计算都是矩阵运算，不是单个变量的运算。
for doc_v in docs_matrix:
if doc_v.sum() == 0:
doc_v = doc_v / 1
else:
doc_v = doc_v / (doc_v.sum())
tfidf = np.dot(docs_matrix,idf)
return names,tfidf
现在我们拥有的矩阵的性质如下，
列是所有文档总共的词的集合。
每行代表一个文档。
每行是一个向量，向量的每个值是这个词的权值。
五、 用K-means算法进行聚类
到这个时候，我们可以使用kmeans算法进行聚类，对kmeans算法来说，它看到已经不是文本了，只是矩阵而已，所以我们用的也是通用的kmeans算法就可以了。
关于kmeans的介绍可以见于如下的文章：
1. 基本Kmeans算法介绍及其实现
2. K-means百度百科
3. 浅谈Kmeans聚类
所不同的是，在大部分的文本聚类中，人们通常用余弦距离（很好的介绍文章）而不是欧氏距离进行计算，难道是因为稀疏矩阵的原因，我并不太明白。
下面的代码来自《机器学习实战》第十章的代码：
def gen_sim(A,B):
num = float(np.dot(A,B.T))
denum = np.linalg.norm(A) * np.linalg.norm(B)
if denum == 0:
denum = 1
cosn = num / denum
sim = 0.5 + 0.5 * cosn
return sim
def randCent(dataSet, k):
n = shape(dataSet)[1]
centroids = mat(zeros((k,n)))\#create centroid mat
for j in range(n):\#create random cluster centers, within bounds of each dimension
minJ = min(dataSet[:,j])
rangeJ = float(max(dataSet[:,j]) - minJ)
centroids[:,j] = mat(minJ + rangeJ * random.rand(k,1))
return centroids
def kMeans(dataSet, k, distMeas=gen_sim, createCent=randCent):
m = shape(dataSet)[0]
clusterAssment = mat(zeros((m,2)))\#create mat to assign data points
\#to a centroid, also holds SE of each point
centroids = createCent(dataSet, k)
clusterChanged = True
counter = 0
while counter <= 50:
counter += 1
clusterChanged = False
for i in range(m):\#for each data point assign it to the closest centroid
minDist = inf;
minIndex = -1
for j in range(k):
distJI = distMeas(centroids[j,:],dataSet[i,:])
if distJI < minDist:
minDist = distJI;
minIndex = j
if clusterAssment[i,0] != minIndex:
clusterChanged = True
clusterAssment[i,:] = minIndex,minDist**2
\#print centroids
for cent in range(k):\#recalculate centroids
ptsInClust = dataSet[nonzero(clusterAssment[:,0].A==cent)[0]]\#get all the point in this cluster
centroids[cent,:] = mean(ptsInClust, axis=0) \#assign centroid to mean
return centroids, clusterAssment
六、 总结
基本上到这里为止，一个可用的中文文本聚类工具已经完成了，github项目地址。
其效果到底怎么样呢？
我自己有一些未分类的文章属于人生感悟（羞羞脸）类别的共有182篇，在切词以及去除停用词之后，共得到13202个词语，我设置K=10，嗯，效果并不是太好，当然可能有一下原因：
文档本身已经属于高度分类的了，基于词频的聚类并不能发现关于这些文章间的细微的区别。
算法需要优化，可能有些地方可以设置修改一下。
总之，在学习若干天机器学习后，第一次实践之旅算是结束了。
[数据分析师](http://cda.pinggu.org/index.php?ac=search&keyword=%E6%95%B0%E6%8D%AE%E5%88%86%E6%9E%90%E5%B8%88)[Python](http://cda.pinggu.org/index.php?ac=search&keyword=Python)

