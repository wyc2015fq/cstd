# R语言︱情感分析—词典型代码实践（最基础）（一） - 素质云笔记/Recorder... - CSDN博客





2016年05月04日 12:29:27[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：17972
所属专栏：[R语言与自然语言处理](https://blog.csdn.net/column/details/13670.html)













**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


笔者寄语：词典型情感分析对词典要求极高，词典中的词语需要人工去选择，但是这样的选择会很有目标以及针对性。本文代码大多来源于《数据挖掘之道》的情感分析章节。本书中还提到了监督算法式的情感分析，可见博客： [R语言︱情感分析—基于监督算法R语言实现笔记](http://blog.csdn.net/sinat_26917383/article/details/51302425)。



可以与博客 [R语言︱词典型情感分析文本操作技巧汇总（打标签、词典与数据匹配等）](http://blog.csdn.net/sinat_26917383/article/details/51265179)对着看。






词典型情感分析大致有以下几个步骤：

**训练数据集、neg/pos情感词典、分词+数据清洗清洗（一、二、三级清洗步骤）、计算情感得分、模型评价**




————————————————————————————————————————————




**《数据挖掘之道》书中几点赠言：**




（1）在分析过程中，难免会产生很多中间变量，它们会占用大量内存。书中提到通常会将所有的临时中间变量命名为temp，只需要保证下一个temp出现之前，临时变量不会再延用就可以了。





（2）毫无疑问，如果不追求高大上的算法的话，词典法不失为一种好方法，其实有时候我们使用了很多方法，结果发现并没有什么质变，也浪费了大量时间；




比如在优化词典的时候，我希望使用高大上的算法解决问题，自动分辨出情感词，结果浪费了大量的时间，尝试了卡方统计量、各种分类器等等，结果可想而知，最后还是使用人工的方法将词典优化了一遍，是的，是用肉眼。
其实有时候看起来最笨的方法也许是现阶段最有效最合适最省事的方法，只是它看起来很low，这也许就是笨方法的高深之处，“聪明人”是不屑于使用这些方法的。


（3）仅仅使用词汇并不能非常准确的识别一条文本所表达的情感倾向。一些修辞手法例如反讽、欲扬先抑等等也会给基于词典的情感分析造成困难。





————————————————————————————————————————————————





# 一、训练数据集




文本作为非结构化数据，在构造训练集的时候，很少会发给你完整的数据集，可能需要批量读取txt字符，读取方式见：[R语言︱词典型情感分析文本操作技巧汇总（打标签、词典与数据匹配等）](http://blog.csdn.net/sinat_26917383/article/details/51265179)，第一节。




`read.csv`函数读取文件时，可能报警：“EOF within quoted string”，一般为数据中不正常的符号所致，常见的方法是将`quote = ""`设置为空，这样做虽然避免了警告，但是仍然解决不了问题，有时数据会对不上号，所以最好从符号上着手将一些特殊符号去除，还有一些文本的正则表达式的问题，可见博客： [R语言︱文本（字符串）处理与正则表达式](http://blog.csdn.net/sinat_26917383/article/details/51122043)。





本文中导入的数据集是清华大学李军标注的近24000个酒店评论文本和谭松波整理的12000个来自京东、携程、当当网的跨行业评论文本。并给出了每个文本数据的评分。李军老师的数据是众多的txt文件的评论文本+用rlabelclass文件来存放文本标签，可以用read.table来调用。

其他的一些市面上的免费语料库可见博客：[情感分析︱网络公开的免费文本语料训练数据集汇总](http://blog.csdn.net/sinat_26917383/article/details/51321505)






```
train<- read.csv("./train.csv",quote = "",sep = "\"", header = T, stringsAsFactors = F)
#没有quote，会出现Warning message:EOF within quoted string
#读入csv格式的时候，出现所有字符变成双引号，需要sep = "\""，来划分开,字符串分隔符的问题？
```






会出现的问题：

（1）EOF within quoted string


解决方法：quote=""；




（2）CSV格式被读入R内存中时，所有字符、变量内容都被加了双引号？

解决方案：需要调整，需要sep = "\""，来划分开。除了英文逗号可能引起`read.csv`函数读取csv文件报错以外，
#还有英文单引号（'）、英文双引号（"）、波浪号（~），都会引起读取时发生警告，带来csv文件或txt文件读取不完整的后果




————————————————————————————————————————————————————————————————————





# 二、正向、逆向情感词典




## 1、词典导入与处理




市面上关于情感词典，有多家研究机构进行了分析，并且公布了结果，比如大连理工、汉语情感词极值表、台湾大学情感NTUSD、知网Hownet情感词、中文褒贬义词典v1.0（清华大学李军）等，有些词典分为正向、逆向单词两个部分；有些放在一起，然后有单独的标签，可以cbind合并在一起。本文引用的是谭松波老师的正向、逆向情感词典。






```
#1、情感正向词，词组+打“+1”-label
pos <- read.csv("./pos.csv", header = T, sep = ",", stringsAsFactors = F)
weight <- rep(1, length(pos[,1]))
pos <- cbind(pos, weight)

#2、情感负向词，词组+打“-1”-label
neg <- read.csv("./neg.csv", header = T, sep = ",", stringsAsFactors = F)
weight <- rep(-1, length(neg[,1]))
neg <- cbind(neg, weight)
```
代码解读：weight是标签，主动贴在正向、逆向词典上。然后进行正向、逆向词典的合并。






```
#3、正、负向词组合并
posneg <- rbind(pos, neg)  #正负词典合并
names(posneg) <- c("term", "weight")
posneg <- posneg[!duplicated(posneg$term), ]#`duplicated`函数的作用和`unique`函数比较相似，它返回重复项的位置编号
```



各个词典对情感词的倾向定义可能矛盾，出现同一个词具有情感正向和负向两种倾向的情况，尽管这种情况更加符合现实，但是违背了基于词典的情感分析的原假设，所以要将这些词去重，我们的方法是一个词如果同时属于正向和负向，仅保留正向分类。用duplicated语句，保留重复的第一个词语，详细可见博客：[R语言︱数据去重](http://blog.csdn.net/sinat_26917383/article/details/51187728)。



![](https://img-blog.csdn.net/20160504115541369)

图1 




## 2、词典读入词库




另外既然整合了大量的词典，就要尽量保证分词器能够把这些情感词汇分出来，所以需要将情感词典添加到分词器的词典中去，虽然这种方法在特殊情况下并不一定凑效。


已知了词典，需要把情感词放到词库里面，以备后续的匹配、分词。在这分词选用Rwordseg包来进行分词。关于这个包如何下载，是个非常头疼的问题，参考博客：[R语言·文本挖掘︱Rwordseg/rJava两包的安装（安到吐血）](http://blog.csdn.net/sinat_26917383/article/details/50867851)






```
dict <- posneg[, "term"]
#library(Rwordseg)
#listDict()  #查看已有词库
#uninstallDict() #删除安装的词典  
insertWords(dict)
```




关于Rwordseg包，如果已经存放了词库，应该先删除原有的词库。



listDict函数是查看词库，uninstallDict函数是删除词库，insertWords是把单词加入词库。加入的词库，应该是单词，所以需要posneg[,"term"]项。



————————————————————————————————————————————————————————————————————





# 三、数据清洗+分词




## 1、一、二级清洗




文本挖掘中，对文本的清洗工作尤为重要，会出现比如：英文逗号、波浪线、英文单引号、英文双引号、分隔符等。一级清洗去掉一些特殊符号，二级清洗去掉一些内容较少、空缺值。详情见：[R语言︱词典型情感分析文本操作技巧汇总（打标签、词典与数据匹配等）](http://blog.csdn.net/sinat_26917383/article/details/51265179)，第二节。







```
sentence <- as.vector(train.test$msg) #文本内容转化为向量sentence
sentence <- gsub("[[:digit:]]*", "", sentence) #清除数字[a-zA-Z]
sentence <- gsub("[a-zA-Z]", "", sentence)   #清除英文字符
sentence <- gsub("\\.", "", sentence)      #清除全英文的dot符号
train.test <- train.test[!is.na(sentence), ]          #清除一些空值文本（文本名）
sentence <- sentence[!is.na(sentence)]   #清除对应sentence里面的空值（文本内容），要先执行文本名
train.test <- train.test[!nchar(sentence) < 2, ]  #筛选字符数小于2的文本
sentence <- sentence[!nchar(sentence) < 2] #`nchar`函数对字符计数，英文叹号为R语言里的“非”函数
```




## 2、分词




每次可能耗费时间较长的过程，都要使用少量数据预估一下时间，这是一个优秀的习惯






`system.time(x <- segmentCN(strwords = sentence)) `





分词之后需要分出来的词语，把ID、label加上，如图2所示。参考 [R语言︱词典型情感分析文本操作技巧汇总（打标签、词典与数据匹配等）](http://blog.csdn.net/sinat_26917383/article/details/51265179)第四节






```
temp <- lapply(x, length)                       #每一个元素的长度,即文本分出多少个词
temp <- unlist(temp)                            #lapply返回的是一个list，所以3行unlist

id <- rep(train.test[, "id"], temp)             #将每一个对应的id复制相应的次数，就可以和词汇对应了

label <- rep(train.test[, "label"], temp)       #id对应的情感倾向标签复制相同的次数
term <- unlist(x)                               #6行将list解散为向量

testterm <- as.data.frame(cbind(id, term, label), stringsAsFactors = F) #生成一个单词-文档-数据框
```






## 3、三级清洗——去停用词




虽然算法已经足够简单，没有必要去除停用词，但是为了显示诚意，文本分析里每一个环节都不能少，这里还是认真的去除停用词，真的不是走过场哦。







```
stopword <- read.csv("./stopword.csv", header = T, sep = ",", stringsAsFactors = F)
stopword <- stopword[!stopword$term %in% posneg$term,]#函数`%in%`在posneg$term中查找stopword的元素，如果查到了就返回真值，没查到就返回假
testterm <- testterm[!testterm$term %in% stopword,]#去除停用词
```






最后生成了图2中的前三列，weght是下面关联情感权重的结果。




![](https://img-blog.csdn.net/20160504120832642)

图2




————————————————————————————————————————————————————————————————————





# 四、情感得分




## 1、关联情感权重




已经获得了训练集的分词，而且也有了情感词典+情感词权重，那么如何把情感词典中的情感权重，加入到训练集的数据集中呢？

这时候需要进行词库之间的匹配，可见博客[R语言︱词典型情感分析文本操作技巧汇总（打标签、词典与数据匹配等）](http://blog.csdn.net/sinat_26917383/article/details/51265179)第五节。

用plyr包中的join函数就可以匹配、并合并。






```
library(plyr)
testterm <- join(testterm, posneg)
testterm <- testterm[!is.na(testterm$weight), ]
head(testterm)
```





## 2、计算情感得分




关联了情感权重，那么每个文档的得分自然而然可以求得，以weight为例，进行分组汇总即可，用aggregate函数。






```
#2、计算情感指数
dictresult <- aggregate(weight ~ id, data = testterm, sum)
dictlabel <- rep(-1, length(dictresult[, 1]))
dictlabel[dictresult$weight > 0] <- 1          #很有技巧地把情感词语正负赋值到情感得分表中
dictresult <- as.data.frame(cbind(dictresult, dictlabel), stringsAsFactors = F)
```



![](https://img-blog.csdn.net/20160504121659486)


图3
得到了如图3中weight的数列，为了与原来的文本分类进行比较，需要简单知道每个文本的情感偏向，得分>0则偏向为1，得分<0,偏向为-1，这时候引入了一个辅助列，dictlabel来进行这样的操作。
dictlabel[dictresult$weight > 0] <- 1是辅助列运算的精华语句。




## 3、模型评价



```
###模型评价
temp <- unique(testterm[, c("id", "label")])
dictresult <- join(dictresult, temp)
evalue <- table(dictresult$dictlabel, dictresult$label)
```



最后可以和原先的分类进行混淆矩阵评价。从结果查看，并不是很精确。
从执行的过程中我们也发现，很多不具有情感色彩的词被定义为了情感词，例如的、了、还、在、我、都、把、上等字词，这些字词都是高频字词，而我们的计算方法按照出现频次重复计算，所以导致上面的结果偏差很大。
暂时的改进办法：修改优化词典，去除这类词汇，或者更改为去重计算，即一条评论中某词无论出现多少次都只计算一次权重。






**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————




