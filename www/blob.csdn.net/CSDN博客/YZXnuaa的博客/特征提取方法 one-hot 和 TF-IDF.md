# 特征提取方法: one-hot 和 TF-IDF - YZXnuaa的博客 - CSDN博客
2018年04月13日 17:59:04[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：476
one-hot 和 TF-IDF是目前最为常见的用于提取文本特征的方法，本文主要介绍两种方法的思想以及优缺点。
## 1. one-hot
### 1.1 one-hot编码
　　什么是one-hot编码？one-hot编码，又称独热编码、一位有效编码。其方法是使用N位状态寄存器来对N个状态进行编码，每个状态都有它独立的寄存器位，并且在任意时候，其中只有一位有效。举个例子，假设我们有四个样本（行），每个样本有三个特征（列），如图：
![](https://images2017.cnblogs.com/blog/1251096/201710/1251096-20171030163200996-742440926.png)
上图中我们已经对每个特征进行了普通的数字编码：我们的feature_1有两种可能的取值，比如是男/女，这里男用1表示，女用2表示。那么one-hot编码是怎么搞的呢？我们再拿feature_2来说明：
这里feature_2 有4种取值（状态），我们就用4个状态位来表示这个特征，one-hot编码就是保证每个样本中的单个特征只有1位处于状态1，其他的都是0。
![](https://images2017.cnblogs.com/blog/1251096/201710/1251096-20171030164625793-1703194224.png)
对于2种状态、三种状态、甚至更多状态都是这样表示，所以我们可以得到这些样本特征的新表示：
![](https://images2017.cnblogs.com/blog/1251096/201710/1251096-20171030165132480-1341638518.png)
one-hot编码将每个状态位都看成一个特征。对于前两个样本我们可以得到它的特征向量分别为
![](https://images2017.cnblogs.com/blog/1251096/201710/1251096-20171030165731683-1946521226.png)
### 1.2 one-hot在提取文本特征上的应用
　　one hot在特征提取上属于词袋模型（bag of words）。关于如何使用one-hot抽取文本特征向量我们通过以下例子来说明。假设我们的语料库中有三段话：
　　　　我爱中国
　　　　爸爸妈妈爱我
　　　　爸爸妈妈爱中国
我们首先对预料库分离并获取其中所有的词，然后对每个此进行编号：
　　　　1 我； 2 爱； 3 爸爸； 4 妈妈；5 中国
然后使用one hot对每段话提取特征向量：
![](https://images2017.cnblogs.com/blog/1251096/201711/1251096-20171106103420622-1037006116.png)；![](https://images2017.cnblogs.com/blog/1251096/201711/1251096-20171106103811778-399990946.png)；![](https://images2017.cnblogs.com/blog/1251096/201711/1251096-20171106103938778-1278932628.png)
因此我们得到了最终的特征向量为
　　　　我爱中国 　->　　　1，1，0，0，1
　　　　爸爸妈妈爱我　　->　　1，1，1，1，0
　　　　爸爸妈妈爱中国　　->　　0，1，1，1，1
### 优缺点分析
**优点：**一是解决了分类器不好处理离散数据的问题，二是在一定程度上也起到了扩充特征的作用（上面样本特征数从3扩展到了9）
**缺点：**在文本特征表示上有些缺点就非常突出了。首先，它是一个词袋模型，不考虑词与词之间的顺序（文本中词的顺序信息也是很重要的）；其次，它假设词与词相互独立（在大多数情况下，词与词是相互影响的）；最后，它得到的特征是离散稀疏的。
###  sklearn实现one hot encode
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
from sklearn import preprocessing  
      
enc = preprocessing.OneHotEncoder()  # 创建对象
enc.fit([[0,0,3],[1,1,0],[0,2,1],[1,0,2]])   # 拟合
array = enc.transform([[0,1,3]]).toarray()  # 转化
print(array)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
## 2. TF-IDF
　　IF-IDF是信息检索（IR）中最常用的一种文本表示法。算法的思想也很简单，就是统计每个词出现的词频（TF），然后再为其附上一个权值参数（IDF）。举个例子：
　　现在假设我们要统计一篇文档中的前10个关键词，应该怎么下手？首先想到的是统计一下文档中每个词出现的频率（TF），词频越高，这个词就越重要。但是统计完你可能会发现你得到的关键词基本都是“的”、“是”、“为”这样没有实际意义的词（停用词），这个问题怎么解决呢？你可能会想到为每个词都加一个权重，像这种”停用词“就加一个很小的权重（甚至是置为0），这个权重就是IDF。下面再来看看公式：
![](https://images2017.cnblogs.com/blog/1251096/201710/1251096-20171030184644027-2546042.png)![](https://images2017.cnblogs.com/blog/1251096/201710/1251096-20171030185050808-1214386978.png)
IF应该很容易理解就是计算词频，IDF衡量词的常见程度。为了计算IDF我们需要事先准备一个语料库用来模拟语言的使用环境，如果一个词越是常见，那么式子中分母就越大，逆文档频率就越小越接近于0。这里的分母+1是为了避免分母为0的情况出现。TF-IDF的计算公式如下：
![](https://images2017.cnblogs.com/blog/1251096/201710/1251096-20171030194100761-48755487.png)
根据公式很容易看出，TF-IDF的值与该词在文章中出现的频率成正比，与该词在整个语料库中出现的频率成反比，因此可以很好的实现提取文章中关键词的目的。
### 优缺点分析
**优点：**简单快速，结果比较符合实际
**缺点：**单纯考虑词频，忽略了词与词的位置信息以及词与词之间的相互关系。
###  sklearn实现tfidf
![复制代码](https://common.cnblogs.com/images/copycode.gif)
from sklearn.feature_extraction.text import CountVectorizer  
from sklearn.feature_extraction.text import TfidfTransformer
tag_list = ['青年 吃货 唱歌',  
            '少年 游戏 叛逆',  
            '少年 吃货 足球'] 
vectorizer = CountVectorizer() #将文本中的词语转换为词频矩阵  
X = vectorizer.fit_transform(tag_list) #计算个词语出现的次数"""
word_dict = vectorizer.vocabulary_
{'唱歌': 2, '吃货': 1, '青年': 6, '足球': 5, '叛逆': 0, '少年': 3, '游戏': 4}
"""
transformer = TfidfTransformer()  
tfidf = transformer.fit_transform(X)  #将词频矩阵X统计成TF-IDF值  print(tfidf.toarray())
