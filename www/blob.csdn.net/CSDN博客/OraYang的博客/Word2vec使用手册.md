
# Word2vec使用手册 - OraYang的博客 - CSDN博客

2017年12月04日 14:24:06[OraYang](https://me.csdn.net/u010665216)阅读数：5737所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



## 引言
Google开源的Word2vec是自然语言处理领域里面，比较好用的工具。这篇博客主要就是介绍如何在一些开源工具中使用这套工具。官方网址：[传送门](https://radimrehurek.com/gensim/index.html)
## 安装
Gensim开发了一套工具箱叫做gensim，里面继承了Word2vec方法。
因此安装方法就是：
```python
pip install --upgrade gensim
```
Note：依赖环境
> Python >= 2.6

> NumPy >= 1.3

> SciPy >= 0.7

> 准备输入

> gensim的word2vec希望能以句子列表作为输入，每个句子由一系列的单词构成：

> \# import modules & set up logging
> import
> gensim, logging
logging.basicConfig(format=
> '%(asctime)s : %(levelname)s : %(message)s'
> , level=logging.INFO)
sentences = [[
> 'first'
> ,
> 'sentence'
> ], [
> 'second'
> ,
> 'sentence'
> ]]
> \# train word2vec on the two sentences
> model = gensim.models.Word2Vec(sentences, min_count=
> 1
> )
> 上面代码中的sentences使用的是python的内置列表结构，这个方法是方便快捷的，但是在面对巨大的语料库的时候，这个列表结构就会占用大量的RAM。因此我们采用yield这个迭代生成器。可以从输入文件中逐行处理：

> class
> MySentences
> (object)
> :
> def
> __init__
> (self, dirname)
> :
> self.dirname = dirname
> def
> __iter__
> (self)
> :
> for
> fname
> in
> os.listdir(self.dirname):
> for
> line
> in
> open(os.path.join(self.dirname, fname)):
> yield
> line.split()
sentences = MySentences(
> '/some/directory'
> )
> \# a memory-friendly iterator
> model = gensim.models.Word2Vec(sentences)
> 训练

> Word2vec接受一些参数来调整训练速度与训练质量，主要有以下三种参数：

> 第一个是剪枝，参数是min_count，这个参数是去除小于min_count的单词：

> model
> =
> Word2Vec
> (sentences, min_count=
> 10
> )  \#
> default
> value is 5

> 第二个是神经网络层数，参数是size，这个参数一般需要根据训练数据来调整大小：

> model
> =
> Word2Vec
> (sentences, size=
> 200
> )  \#
> default
> value is 100

> 最后一个是线程数，参数是workers，这个则是根据你的电脑处理器配置来设置的：

> model
> =
> Word2Vec
> (sentences, workers=
> 4
> ) \#
> default
> = 1 worker = no parallelization

> 评价

> Word2vec的训练是无监督任务，因此你想要进行准确的评估是不太可行的，这种方法主要依赖于你的终端应用。也就是我们常说的具体问题具体分析。Google开放了两万个语法语义测试样本：
> [传送门](https://raw.githubusercontent.com/RaRe-Technologies/gensim/develop/gensim/test/test_data/questions-words.txt)

> gensim也支持这种格式的测试样本，调用方法如下：

> model.accuracy(
> '/tmp/questions-words.txt'
> )
> 存储及加载模型

> gensim可以将训练好的模型存储起来，也能加载存储好的模型：

> model.save(
> '/tmp/mymodel'
> )
new_model = gensim.models.Word2Vec.load(
> '/tmp/mymodel'
> )
> gensim也能以二进制或非二进制形式将模型存储为txt文本：

> model = Word2Vec.load_word2vec_format(
> '/tmp/vectors.txt'
> , binary=
> False
> )
> \# using gzipped/bz2 input works too, no need to unzip:
> model = Word2Vec.load_word2vec_format(
> '/tmp/vectors.bin.gz'
> , binary=
> True
> )
> Note：由于以txt文本存储的模型并没有保留训练模型某些重要参数，导致加载该模型时，无法在原先训练好的模型上继续学习。

> 使用模型

> 这个模型的应用主要就是相似性分析：

> model.most_similar(positive=[
> 'woman'
> ,
> 'king'
> ], negative=[
> 'man'
> ], topn=
> 1
> )
[(
> 'queen'
> ,
> 0.50882536
> )]
model.doesnt_match(
> "breakfast cereal dinner lunch"
> ;.split())
> 'cereal'
> model.similarity(
> 'woman'
> ,
> 'man'
> )
> 0.73723527
> 总结

> 这个方法调用起来还是挺简单的，如果对Word2vec的理论做进一步了解，还需要看一些论文，后期会总结，待续。


