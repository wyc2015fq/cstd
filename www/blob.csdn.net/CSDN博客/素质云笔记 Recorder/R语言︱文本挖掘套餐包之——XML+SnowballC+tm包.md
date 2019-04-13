
# R语言︱文本挖掘套餐包之——XML+SnowballC+tm包 - 素质云笔记-Recorder... - CSDN博客

2016年04月04日 11:38:40[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6918所属专栏：[R语言与自然语言处理](https://blog.csdn.net/column/details/13670.html)




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————

R语言︱文本挖掘套餐包之——XML+tm+SnowballC包


笔者寄语：文本挖掘、情感分析是目前非结构数据非常好用、有效的分析方式。
先针对文本挖掘这个套餐包做个简单了解。一般来说一个完整的文本挖掘解决流程是：

**网页爬取数据——数据格式转化（分隔）——建立语料库——词频去噪——提取词干——创建文档-词频矩阵——后续分析（聚类、词云等）**

XML包可以实现：网页爬取（还有Rcurl包）、格式转化
tm包可以实现：建立语料库、创建文档-词频矩阵、去噪（还有Rwordseg包是中文分词包）
SnowballC包可以实现：提取词干

本篇暂时不介绍XML包的数据爬取，先来看后面两个包的实现。


本文以一个案例介绍SnowballC包+tm包，使用的数据是R语言中自带的数据集，案例部分来源于参考[西门吹风博客](http://www.cnblogs.com/qianwen/p/3721903.html)。


# 一、函数调用、数据导入、生成语料库

`library(SnowballC)
library(tm)
#vignette("tm")   #调用函数包文件

##1.Data Import  导入自带的路透社的20篇xml文档
#找到/texts/crude的目录，作为DirSource的输入，读取20篇xml文档
reut21578 <- system.file("texts", "crude", package = "tm")
reuters <- Corpus(DirSource(reut21578), readerControl = list(reader = readReut21578XML))
#Corpus命令读取文本并生成语料库文件
##2.Data Export  将生成的语料库在磁盘上保存成多个纯文本文件
writeCorpus(reuters)
##3.Inspecting Corpora 查看语料库 
#can use inspect(),print(),summary()
#由于是从xml读取过来，所以现在的corpus还是非常杂乱
inspect(reuters)
print(reuters) 
summary(reuters)`

还有查看语料库的几个函数:inspect(),print(),summary()三个。

# 二、格式转化、去噪

`##4.Transformations 
#对于xml格式的文档用tm_map命令对语料库文件进行预处理，将其转为纯文本并去除多余空格，
#转换小写，去除常用词汇、合并异形同意词汇，如此才能得到类似txt文件的效果
#可以用inspect(reuters)查看此时的效果，明显好很多
reuters <- tm_map(reuters, PlainTextDocument)#将reuters转化为纯文本文件，去除标签
reuters <- tm_map(reuters, stripWhitespace)#去掉空白
reuters <- tm_map(reuters, tolower)#转换为小写
reuters <- tm_map(reuters, removeWords, stopwords("english"))#去停用词``</pre><pre code_snippet_id="1633870" snippet_file_name="blog_20160404_2_6556358" name="code" class="plain">#采用Porter's stemming 算法 提取词干
#Stem words in a text document using Porter's stemming algorithm
#install.packages("SnowballC")
tm_map(reuters, stemDocument)`

三、创建文档-词频矩阵

关于下面的DocumentTermMatrix，前面一定要跟tm_map(reuters,PlainTextDocument)，注意与前面的区别，以及执行代码的顺序。

`##5.创建文档矩阵 Creating Term-Document Matrices
#将处理后的语料库进行断字处理，生成词频权重矩阵(稀疏矩阵)也叫词汇文档矩阵``reuters <- tm_map(reuters, PlainTextDocument)#将reuters转化为纯文本文件，去除标签
dtm <- DocumentTermMatrix(reuters)  
#报错可看http://www.bubuko.com/infodetail-345849.html
#需先执行一下reuters <- tm_map(reuters, PlainTextDocument)``#查看词汇文档矩阵内容
inspect(dtm[1:5, 100:105])
#Non-/sparse entries: 1990/22390     ---非0/是0 
#Sparsity           : 92%            ---稀疏性  稀疏元素占全部元素的比例
#Maximal term length: 17             ---切词结果的字符最长那个的长度
#Weighting          : term frequency (tf)---词频率
#如果需要考察多个文档中特有词汇的出现频率，可以手工生成字典，
#并将它作为生成矩阵的参数``d<-c("price","crude","oil","use")   #以这几个关键词为查询工具
inspect(DocumentTermMatrix(reuters,control=list(dictionary=d)))`

DocumentTermMatrix生成的矩阵是文档-词频的稀疏矩阵，横向是文档文件，纵向是分出来的词，矩阵里面代表词频，如下图。

![](https://img-blog.csdn.net/20160404112145101)
创建好文档词频矩阵之后，可以通过一些方式查看这个矩阵的内容，或者用函数筛选出你想要的结果等。

`##6.在文本矩阵上实践 Operations on Term-Document Matrices
#找出次数超过50的词
findFreqTerms(dtm, 50)
#找出与‘opec’单词相关系数在0.8以上的词
findAssocs(dtm,"opec",0.8)

#因为生成的矩阵是一个稀疏矩阵，再进行降维处理，之后转为标准数据框格式
#我们可以去掉某些出现频次太低的词。
dtm1<- removeSparseTerms(dtm, sparse=0.6)
inspect(dtm1)
data <- as.data.frame(inspect(dtm1))`

# 四、后续分析——层次聚类
`#再之后就可以利用R语言中任何工具加以研究了，下面用层次聚类试试看
#先进行标准化处理，再生成距离矩阵，再用层次聚类
data.scale <- scale(data)
d <- dist(data.scale, method = "euclidean")
fit <- hclust(d, method="ward.D")

#绘制聚类图
#可以看到在20个文档中，489号和502号聚成一类，与其它文档区别较大。
plot(fit,main ="文件聚类分析")`

聚类说明了根据词频统计，哪些文档较为相近，说明这些文档存在同质。

——————————————————————————————————————————————————————————————————————————
应用一：snowball包中的词干与记号化去哪儿？
词干化：去掉ing，s之类的词，目前适用于英文，中文不适用
SnowballStemmer(c('functions', 'stemming', 'liked', 'doing'))
[1] "function" "stem" "like" "do"

记号化：将一段文本分割成叫做token(象征)过程，token可能是单词、短语、符号或其他有意义的元素。
NGramTokenizer(' 中华人民共和国成立于1949年')
[1] "中华人民共和国成立于" "成立于1949年" "中华人民共和国成立"
[4] "成立于" "于1949年" "中华人民共和国"
[7] "成立" "于" "1949年"


snowball现在这个包已经无法加载了，tm包调用SnowballC可以词干化，函数名字叫：stemDocument；
记号化在tm包中叫做getTokenizers函数。

**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


